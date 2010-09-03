/************************************************
文件：config.h
用途：系统配置函数
************************************************/
/*
保存到ROM中的数据读取方法：
pgm_read_byte(OSUnMapTbl+OSRdyGrp);
*/
#ifndef __CONFIG_H__
#define __CONFIG_H__

//-------------标准头文件-------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <math.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <stdarg.h>  
#include <stdlib.h>     

typedef unsigned char 				bool;
#define FALSE						0x00
#define TRUE						0x01
#define ENABLE						0
#define DISABLE						1

#define CPU_FREQUENCY				8000000

#define BITNUM_0					0x01
#define BITNUM_1					0x02
#define BITNUM_2					0x04
#define BITNUM_3					0x08

#define REVERSE(BITNUM) 			PORTC = ((~(BITNUM)) & PORTC) | ((~PORTC) & (BITNUM))
#define set_bit(reg,bit)			reg |= _BV(bit)
#define clr_bit(reg,bit)			reg &= ~_BV(bit)

#define max(a,b) 					((a)>(b)?(a):(b))
#define min(a,b) 					((a)<(b)?(a):(b))

#define _NOP() 						asm("nop")

//-----------------任务优先级---------------
#define TASK1_PRIO					1
#define TASK2_PRIO					2
#define TASK3_PRIO					3
#define TASK4_PRIO					4
#define TASK5_PRIO					5

//-----------------GCC attribute------------
#define UNUSED 						__attribute__((unused))

//----------------常用宏--------------------
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

#define EINT0_TRIGGER_MODE_LOW		(0<<ISC00)
#define EINT0_TRIGGER_MODE_CHANGE	(1<<ISC00)
#define EINT0_TRIGGER_MODE_DOWN		(2<<ISC00)
#define EINT0_TRIGGER_MODE_UP		(3<<ISC00)

#define EINT1_TRIGGER_MODE_LOW		(0<<ISC10)
#define EINT1_TRIGGER_MODE_CHANGE	(1<<ISC10)
#define EINT1_TRIGGER_MODE_DOWN		(2<<ISC10)
#define EINT1_TRIGGER_MODE_UP		(3<<ISC10)

#define EINT2_TRIGGER_MODE_DOWN		(0<<ISC20)
#define EINT2_TRIGGER_MODE_UP		(1<<ISC20)

#define EINT0_REQ_EN				(1<<INT0)
#define EINT1_REQ_EN				(1<<INT1)
#define EINT2_REQ_EN				(1<<INT2)

//-------------端口设置----------------
#define PA_OUT_REV(x);				{DDRA  |= ((x) & 0x07); PORTA ^= ((x) & 0x07);}
#define PA_OUT_H(x);				{DDRA  |= ((x) & 0x07); PORTA |= ((x) & 0x07);}
#define PA_OUT_L(x);				{DDRA  |= ((x) & 0x07); PORTA &=~((x) & 0x07);}

//-------------LED灯-------------------
#define LED1						0
#define LED2						1
#define LED3						2
#define LED4						3
#define LED5						4
#define LED6						5
#define LED7						6
#define LED8						7

//-------------自定义头文件------------
#include "ucos_ii.h"
#include "data_queue.h"
#include "cmd_queue.h"
#include "os_cpu.h"
#include "os_cfg.h"
#include "uart.h"
#include "res_control.h"
#include "twi.h"
#include "pcf8563.h"
#include "board.h"
#include "adc.h"

#endif
