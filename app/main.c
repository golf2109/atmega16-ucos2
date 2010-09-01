#include <avr/io.h>
#include "ucos_ii.h"

#define CLK_IO_DIV_NO				0
#define CLK_IO_DIV_1				1
#define CLK_IO_DIV_8				2
#define CLK_IO_DIV_64				3
#define CLK_IO_DIV_256				4
#define CLK_IO_DIV_1024				5

#define TC2_CLK_DIV_NO				0
#define TC2_CLK_DIV_1				1
#define TC2_CLK_DIV_8				2
#define TC2_CLK_DIV_32				3
#define TC2_CLK_DIV_64				4
#define TC2_CLK_DIV_128				5
#define TC2_CLK_DIV_256				6
#define TC2_CLK_DIV_1024			7

#define TC0_CTC						(1<<WGM01)	
#define TC2_CTC						(1<<WGM21)	

#define EINT0_TRIGGER_MODE_LOW		0
#define EINT0_TRIGGER_MODE_CHANGE	1
#define EINT0_TRIGGER_MODE_DOWN		2
#define EINT0_TRIGGER_MODE_UP		3

#define EINT1_TRIGGER_MODE_LOW		0
#define EINT1_TRIGGER_MODE_CHANGE	1
#define EINT1_TRIGGER_MODE_DOWN		2
#define EINT1_TRIGGER_MODE_UP		3

#define EINT2_TRIGGER_MODE_DOWN		0
#define EINT2_TRIGGER_MODE_UP		1

#define INT_CTL_INT0_REQ			(1<<INT0)
#define INT_CTL_INT1_REQ			(1<<INT1)
#define INT_CTL_INT2_REQ			(1<<INT2)



OS_STK Task1Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task2Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task3Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task4Stk[OS_USER_TASK_STK_SIZE] = {0};
OS_STK Task5Stk[OS_USER_TASK_STK_SIZE] = {0};

volatile unsigned char t1 = 0;
volatile unsigned char t2 = 0;
volatile unsigned char t3 = 0;
volatile unsigned char t4 = 0;
volatile unsigned char t5 = 0;
volatile unsigned char t6 = 0;

OS_EVENT *T2sem = (OS_EVENT *)0;
OS_EVENT *Tmbox = (OS_EVENT *)0;

struct msgTask
{
	unsigned char cnt;
	char *s;
};

struct msgTask T5mbox = {1,"Hello"};
struct msgTask *pMsgTsk = &T5mbox;
 
void Task1(void *pdata)
{
	pdata = pdata;

	TIMSK &= ~((1<<OCIE0)|(1<<TOIE0));
	 
	#if OS_TICKS_PER_SEC <= (F_CPU/1024/256)
	 #error "OS_TICKS_PER_SEC < (F_CPU/1024/256)"
	#endif

	OCR0 = F_CPU/1024/OS_TICKS_PER_SEC;
	TCNT0 = 0;

	TCCR0 = TC0_CTC|CLK_IO_DIV_1024;

	TIMSK |= (1<<OCIE0);
	 
	DDRB |= 0x02;

	while(1)
	{
		OSTimeDly(2);
		t1++;
		PORTB ^= 0x02;
	}
}

void Task2(void *pdata)
{
	unsigned char err = 0;
	pdata   =	pdata;

	MCUCR   =	EINT0_TRIGGER_MODE_DOWN;
	GICR    =	INT_CTL_INT0_REQ;

	TCCR2   =	TC2_CTC;      // CTC 模式, TOP=OCR2
	OCR2    =	78;         // 7372800/1024/72=100
	TCCR2   |=	TC2_CLK_DIV_1024;     // 1024分频
	TIMSK   |=	(1<<OCIE2);     // 比较中断使能
	 
	while(1)
	{        
		OSSemPend(T2sem,0,&err);
		if(err == OS_NO_ERR) 
			t2++;
		if(err == OS_TIMEOUT)
		{};
		GIFR	|= (1<<INTF0);
		// OSTimeDly(10);	 
	}
}

void Task3(void *pdata)
{
	pdata = pdata;
	 
	while(1)
	{
		t3++;
		OSTimeDly(4);
		DDRA	|= 0x04;
		PORTA	|= 0x04;
		PORTA	&=~0x04;	 
	}
}

void Task4(void *pdata)
{
	pdata	= pdata;
	 
	MCUCR	|= 0x08;
	GICR	|= 0x80;

	while(1)
	{
		t4++;
		OSTimeDly(8);
		DDRA	|= 0x08;
		PORTA	|= 0x08;
		PORTA	&=~0x08;	 
	}
}

void Task5(void *pdata)
{
	unsigned char err = 0; 
	struct msgTask *p = ( struct msgTask *)0;
 
	pdata=pdata;
 
	while(1)
	{
		p = ( struct msgTask *)OSMboxPend(Tmbox,0,&err);
				   
		t5 = p->cnt;	 
	}
}

int main(void)
{
	OSInit();
	 
	OSTaskCreate(Task1,0,&Task1Stk[OS_USER_TASK_STK_SIZE-1],1);
	OSTaskCreate(Task2,0,&Task2Stk[OS_USER_TASK_STK_SIZE-1],2);
	OSTaskCreate(Task3,0,&Task3Stk[OS_USER_TASK_STK_SIZE-1],3); 
	OSTaskCreate(Task4,0,&Task4Stk[OS_USER_TASK_STK_SIZE-1],5); 
	OSTaskCreate(Task5,0,&Task5Stk[OS_USER_TASK_STK_SIZE-1],4); 

	T2sem = OSSemCreate(0);
	Tmbox = OSMboxCreate((void *)0); 

	OSStart();
}

