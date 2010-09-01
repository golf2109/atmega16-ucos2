/******************************************************/
/********** OS_CPU_C.c*********************************/
/******************************************************/

#include <avr/io.h>
#include "os_cpu.h"
#include "os_cfg.h"
#include "ucos_ii.h"

OS_STK *OSTaskStkInit(void (*task)(void *pd),void *p_arg,OS_STK *ptos,INT16U opt)
{
	OS_STK *stk=ptos;
	 
	*stk--=((unsigned int)task)&0xff;
	*stk--=((unsigned int)task)>>8;

	*stk--=0x31;
	*stk--=0x30;
	*stk--=0x29;
	*stk--=0x28;
	*stk--=0x27;
	*stk--=0x26;
	*stk--=((unsigned int)p_arg)&0xff;
	*stk--=((unsigned int)p_arg)>>8;
	*stk--=0x23;
	*stk--=0x22;
	*stk--=0x21;
	*stk--=0x20;
	*stk--=0x19;
	*stk--=0x18;
	*stk--=0x17;
	*stk--=0x16;
	*stk--=0x15;
	*stk--=0x14;
	*stk--=0x13;
	*stk--=0x12;
	*stk--=0x11;
	*stk--=0x10;
	*stk--=0x09;
	*stk--=0x08;
	*stk--=0x07;
	*stk--=0x06;
	*stk--=0x05;
	*stk--=0x04;
	*stk--=0x03;
	*stk--=0x02;
	*stk--=0x00;
	*stk--=0x00;
	*stk--=0x80;

	return stk;
}

void OSDebugInit(void)
{

}

void OSTaskCreateHook(OS_TCB *ptcb)
{
	ptcb=ptcb;
}

void OSTaskDelHook(OS_TCB *ptcb)
{
    ptcb=ptcb;
}

void OSTaskIdleHook(void)
{
 
}

void OSTaskStatHook(void)
{

}

void OSTaskSwHook(void)
{

}

void OSTCBInitHook(OS_TCB *ptcb)
{
    ptcb=ptcb;
}


void OSTimeTickHook(OS_TCB *ptcb)
{

}

void OSInitHookBegin(void)
{

}

void OSInitHookEnd(void)
{

}

