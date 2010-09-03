#include "config.h"

void board_init(void)
{
	//-----------�ⲿ�ж�0����----------
	MCUCR   =	EINT0_TRIGGER_MODE_DOWN;
	GICR    =	INT_CTL_INT0_REQ;
	
	//-----------��ʱ��0����------------
	TIMSK &= ~((1<<OCIE0) | (1<<TOIE0));
	 
	#if OS_TICKS_PER_SEC <= (F_CPU/1024/256)
		#error "OS_TICKS_PER_SEC < (F_CPU/1024/256)"
	#endif

	OCR0  = F_CPU/1024/OS_TICKS_PER_SEC;			//OS_TICKS_PER_SEC = 100
	TCNT0 = 0;
	TCCR0 = TC0_CTC|CLK_IO_DIV_1024;
	TIMSK |= (1<<OCIE0);							//����Ƚ��ж�ʹ��
}

void timer2_init(void)
{
	TCCR2   =	TC2_CTC;      						// CTC ģʽ, TOP=OCR2
	OCR2    =	78;         						// 8000000/1024/78=100
	TCCR2   |=	TC2_CLK_DIV_1024;     				// 1024��Ƶ
	TIMSK   |=	(1<<OCIE2);     					// �Ƚ��ж�ʹ��
}