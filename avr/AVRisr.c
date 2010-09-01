// AVRisr.c
/*  AVR ISR */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "ucos_ii.h"

/*************************************************************************************************************/
extern OS_EVENT *T2sem;
void IsrEint0(void)
{
	OSSemPost(T2sem);

}
/*************************************************************************************************************/
extern OS_EVENT *Tmbox;

extern struct msgTask
{
	unsigned char cnt;
	char *s;
}*pMsgTsk;

extern unsigned char t4;

void IsrEint1(void)
{
	pMsgTsk->cnt=255-t4;
	pMsgTsk->s="uCOS2";
	
	OSMboxPost(Tmbox,(void *)pMsgTsk);
}
/*************************************************************************************************************/
extern volatile unsigned char t6;
ISR(SIG_OUTPUT_COMPARE2)
{
	t6++;
}

