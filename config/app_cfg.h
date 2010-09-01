#ifndef __APP_CFG_H__
#define __APP_CFG_H__

#include <avr/io.h>
#include "ucos_ii.h"

struct msgTask;

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

extern volatile unsigned char t1;
extern volatile unsigned char t2;
extern volatile unsigned char t3;
extern volatile unsigned char t4;
extern volatile unsigned char t5;
extern volatile unsigned char t6;

extern OS_EVENT *T2sem;
extern OS_EVENT *Tmbox;

extern struct msgTask T5mbox;
extern struct msgTask *pMsgTsk;

#endif