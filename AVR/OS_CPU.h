#ifndef _OS_CPU_H_
#define _OS_CPU_H_

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

#define  OS_CRITICAL_METHOD    3

#ifndef  INC_FROM_OS_CPU_ASM
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char    BOOLEAN;
typedef unsigned char    INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char    INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long    INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long    INT32S;                   /* Signed   32 bit quantity                           */
typedef float             FP32;                     /* Single precision floating point                    */
typedef double            FP64;                     /* Double precision floating point                    */

typedef unsigned char   OS_STK;                   /* Each stack entry is 8-bit wide                    */
typedef unsigned char   OS_CPU_SR;                /* Define size of CPU status register (SREG = 8 bits) */



/* 
*********************************************************************************************************
*                              AVR 
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if 
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/

#include <avr/io.h>

#if      OS_CRITICAL_METHOD == 1
#define  OS_ENTER_CRITICAL()  asm volatile("cli"::)                    /* Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   asm volatile("sei"::)                    /* Enable  interrupts                        */
#endif


#if      OS_CRITICAL_METHOD == 2
#define  OS_ENTER_CRITICAL()  asm volatile ("in __tmp_reg__,__SREG__"  "\n\t"    \
                                            "cli"                      "\n\t"    \
                                            "push __tmp_reg__"                   \
											::)                                  /* Disable interrupts                        */  
#define  OS_EXIT_CRITICAL()   asm volatile ("pop __tmp_reg__"          "\n\t"    \
                                            "out __SREG__,__tmp_reg__"           \
											::)                                  /* Enable  interrupts                        */
#endif

#if      OS_CRITICAL_METHOD == 3
#define  OS_ENTER_CRITICAL()  asm volatile ("in %0,__SREG__"  "\n\t"    \
                                            "cli"             "\n\t"    \
                                            "push %0"                   \
											 :"=&r"(cpu_sr) : )                 /*(cpu_sr = OSCPUSaveSR()) Disable interrupts                        */
#define  OS_EXIT_CRITICAL()   asm volatile ("pop  %0"         "\n\t"    \
                                            "out __SREG__,%0"           \
						                     :"=&r"(cpu_sr) :)                  /*(OSCPURestoreSR(cpu_sr)) Enable  interrupts                        */
#endif

/*
*********************************************************************************************************
*                           Intel 80x86 (Real-Mode, Large Model) Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                       /* Stack grows from HIGH to LOW memory on AVR  */

#define  OS_TASK_SW()         OSCtxSw()


#endif


#endif
