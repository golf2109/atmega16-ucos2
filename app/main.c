#include "config.h"

//--------------系统对象-----------------
OS_EVENT *T2sem = (OS_EVENT *)0;
OS_EVENT *Tmbox = (OS_EVENT *)0;

//--------------任务堆栈-----------------
OS_STK Task1Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task2Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task3Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task4Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task5Stk[OS_USER_TASK_STK_SIZE] = {0};

//-------------消息结构体----------------
typedef struct _msgTask
{
	unsigned char cnt;
	char *msg;
}msgTask;

msgTask T5mbox = {1,"Hello"};
msgTask *pMsgTask = &T5mbox;

volatile unsigned char t5 = 0;

void Task1(void *pdata)
{
	pdata = pdata;

	TIMSK &= ~((1<<OCIE0)|(1<<TOIE0));
	 
	#if OS_TICKS_PER_SEC <= (F_CPU/1024/256)
	 #error "OS_TICKS_PER_SEC < (F_CPU/1024/256)"
	#endif

	OCR0  = F_CPU/1024/OS_TICKS_PER_SEC;
	TCNT0 = 0;

	TCCR0 = TC0_CTC|CLK_IO_DIV_1024;

	TIMSK |= (1<<OCIE0);
	 
	DDRB |= 0x02;
	uart_init();
	
	while(1)
	{
		PORTB ^= 0x02;
		DDRA  |= 0x01;
		PORTA ^= 0x01;
		com_putstring(pMsgTask->msg, 5);
		com_putchar(0x0A);
		com_putchar(0x0D);
		com_printf("This is task1!\n");
		
		OSTimeDly(OS_TICKS_PER_SEC/3);
	}
}

void Task2(void *pdata)
{
	unsigned char err = 0;
	pdata   =	pdata;

	MCUCR   =	EINT0_TRIGGER_MODE_DOWN;
	GICR    =	INT_CTL_INT0_REQ;

	TCCR2   =	TC2_CTC;      			// CTC 模式, TOP=OCR2
	OCR2    =	78;         			// 8000000/1024/78=100
	TCCR2   |=	TC2_CLK_DIV_1024;     	// 1024分频
	TIMSK   |=	(1<<OCIE2);     		// 比较中断使能
	 
	while(1)
	{        
		OSSemPend(T2sem,0,&err);
		if(err == OS_NO_ERR) 
		{}
		if(err == OS_TIMEOUT)
		{}
		GIFR	|= (1<<INTF0);	 
	}
}

void Task3(void *pdata)
{
	pdata = pdata;
	 
	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC/2);
		DDRA	|= 0x04;
		PORTA	^= 0x04;	 
	}
}

void Task4(void *pdata)
{
	pdata	= pdata;
	 
	MCUCR	|= 0x08;
	GICR	|= 0x80;

	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC/3);
		DDRA	|= 0x08;
		PORTA	^= 0x08; 
	}
}

void Task5(void *pdata)
{
	unsigned char err = 0; 
	msgTask *p = (msgTask *)0;
 
	pdata=pdata;
 
	while(1)
	{
		p = (msgTask *)OSMboxPend(Tmbox,0,&err);
				   
		t5 = p->cnt;
		DDRA  |= 0x02;
		PORTA ^= 0x02;		
	}
}

int main(void)
{
	OSInit();
	 
	OSTaskCreate(Task1,0,&Task1Stk[OS_USER_TASK_STK_SIZE-1],TASK1_PRIO);
	OSTaskCreate(Task2,0,&Task2Stk[OS_USER_TASK_STK_SIZE-1],TASK2_PRIO);
	OSTaskCreate(Task3,0,&Task3Stk[OS_USER_TASK_STK_SIZE-1],TASK3_PRIO); 
	OSTaskCreate(Task4,0,&Task4Stk[OS_USER_TASK_STK_SIZE-1],TASK4_PRIO); 
	OSTaskCreate(Task5,0,&Task5Stk[OS_USER_TASK_STK_SIZE-1],TASK5_PRIO); 

	T2sem = OSSemCreate(0);
	Tmbox = OSMboxCreate((void *)0); 

	OSStart();
	
	return 0;
}

/*************************************************************************************************************/
void IsrEint0(void)
{
	OSSemPost(T2sem);
}
/*************************************************************************************************************/
void IsrEint1(void)
{
	pMsgTask->cnt=255-t5;
	pMsgTask->msg="uCOS2";
	
	OSMboxPost(Tmbox, (void *)pMsgTask);
}
/*************************************************************************************************************/
ISR(SIG_OUTPUT_COMPARE2)
{
	t5++;
}
