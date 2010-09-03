#include "config.h"

void board_init(void)
{
	timer0_init();
	timer2_init();
	interrupt_init();
}

void timer0_init(void)
{
	//-----------定时器0设置------------
	TIMSK &= ~((1<<OCIE0) | (1<<TOIE0));
	 
	#if OS_TICKS_PER_SEC <= (F_CPU/1024/256)
		#error "OS_TICKS_PER_SEC < (F_CPU/1024/256)"
	#endif

	OCR0  = F_CPU/1024/OS_TICKS_PER_SEC;		//OS_TICKS_PER_SEC = 100
	TCNT0 = 0;
	TCCR0 = TC0_CTC|CLK_IO_DIV_1024;
	TIMSK |= (1<<OCIE0);						//输出比较中断使能
}

void timer2_init(void)
{
	TCCR2  = TC2_CTC;      						// CTC 模式, TOP=OCR2
	OCR2   = 78;         						// 8000000/1024/78=100
	TCCR2 |= TC2_CLK_DIV_1024;     				// 1024分频
	TIMSK |= (1<<OCIE2);     					// 比较中断使能
}

void interrupt_init(void)
{
	//-----------外部中断0设置----------
	MCUCR |= EINT0_TRIGGER_MODE_DOWN;
	GICR  |= EINT0_REQ_EN;
	
	//-----------外部中断1设置----------
	MCUCR |= EINT1_TRIGGER_MODE_DOWN;
	GICR  |= EINT1_REQ_EN;
}