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
typedef struct _MsgTask
{
	unsigned char cnt;
	char *msg;
}MsgTask;

MsgTask T5mbox 		= {1,"today"};
MsgTask *pMsgTask 	= &T5mbox;

volatile unsigned short t5 = 0;

void Task1(void *pdata)
{
	pdata = pdata;
	
	while(1)
	{
		PA_OUT_REV(LED1);
		
		com_putstring(pMsgTask->msg, 5);
		com_putenter();
		//com_printf("%s\n",pMsgTask->msg);
		OSTimeDly(OS_TICKS_PER_SEC/3);

	}
}

void Task2(void *pdata)
{
	unsigned char err = 0;
	pdata   =	pdata;
	 
	while(1)
	{        
		OSSemPend(T2sem, 0, &err);
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
	//adc_init();
	unsigned char err = 0;
	MsgTask *p = (MsgTask *)0;
	while(1)
	{
		//adc_switch_channel(ADC_VOLTAGE);
		//START_ADC();
		PA_OUT_REV(LED3);

		//t5 = ADC;
		//com_printf("ADC = %d\n", t5);
		pMsgTask->cnt = 255-t5;
		pMsgTask->msg = "Task3";
		OSMboxPost(Tmbox, (void *)pMsgTask);
		p = (MsgTask *)OSMboxPend(Tmbox,0,&err);
		com_putstring(p->msg, 5);
		com_putenter();
		
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}

void Task4(void *pdata)
{
	pdata	= pdata;
	
	while(1)
	{
		OSTimeDly(OS_TICKS_PER_SEC);
		PA_OUT_REV(LED8);
	}
}

void Task5(void *pdata)
{
	unsigned char err = 0; 
	MsgTask *p = (MsgTask *)0;
 
	pdata=pdata;
 
	while(1)
	{
		p = (MsgTask *)OSMboxPend(Tmbox,0,&err);		   
		t5 = p->cnt;
		com_putstring(p->msg, 5);
		com_putenter();
		
		p->msg = "Task5";
		OSMboxPost(Tmbox, (void *)p);
		PA_OUT_REV(LED5);	
	}
}

void task_create(void)
{
	OSTaskCreate(Task1,0,&Task1Stk[OS_USER_TASK_STK_SIZE-1],TASK1_PRIO);
	OSTaskCreate(Task2,0,&Task2Stk[OS_USER_TASK_STK_SIZE-1],TASK2_PRIO);
	OSTaskCreate(Task3,0,&Task3Stk[OS_USER_TASK_STK_SIZE-1],TASK3_PRIO); 
	OSTaskCreate(Task4,0,&Task4Stk[OS_USER_TASK_STK_SIZE-1],TASK4_PRIO); 
	OSTaskCreate(Task5,0,&Task5Stk[OS_USER_TASK_STK_SIZE-1],TASK5_PRIO); 
}

int main(void)
{
	board_init();
	uart_init();
	
	OSInit();
	 
	task_create();

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
	pMsgTask->cnt = 255-t5;
	pMsgTask->msg = "uCOS2";
	
	OSMboxPost(Tmbox, (void *)pMsgTask);
}
/*************************************************************************************************************/
ISR(SIG_OUTPUT_COMPARE2)
{
	t5++;
}
