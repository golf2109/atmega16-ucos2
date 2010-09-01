// AVRisr.c
/*  AVR ISR */
#include <avr/interrupt.h>
#include "app_cfg.h"

/*************************************************************************************************************/
void IsrEint0(void)
{
	OSSemPost(T2sem);
}
/*************************************************************************************************************/

extern struct msgTask
{
	unsigned char cnt;
	char *s;
}*pMsgTsk;


void IsrEint1(void)
{
	pMsgTsk->cnt=255-t4;
	pMsgTsk->s="uCOS2";
	
	OSMboxPost(Tmbox,(void *)pMsgTsk);
}
/*************************************************************************************************************/
ISR(SIG_OUTPUT_COMPARE2)
{
	t6++;
}

