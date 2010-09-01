;OS_CPU_A.S

#include <avr/io.h>

#define INC_FROM_OS_CPU_ASM

#include "../AVR/os_cpu.h"
#include "../Config/os_cfg.h"

;*************************************************************************************************************
;OS_EXT  INT8U             OSIntNesting;                        /* Interrupt nesting level                           */
;OS_EXT  BOOLEAN           OSRunning;                       /* Flag indicating that kernel is running         */
;OS_EXT  INT8U             OSPrioCur;                           /* Priority of current task                         */
;OS_EXT  INT8U             OSPrioHighRdy;                     /* Priority of highest priority task               */
;OS_EXT  OS_TCB           *OSTCBCur;                        /* Pointer to currently running TCB           */
;OS_EXT  OS_TCB           *OSTCBHighRdy;                  /* Pointer to highest priority TCB R-to-R   */

         .extern OSTCBCur
         .extern OSTCBHighRdy
         .extern OSPrioCur
         .extern OSPrioHighRdy

         .extern OSRunning
         .extern OSIntNesting


;*************************************************************************************************************
         .global OSStartHighRdy
         .type OSStartHighRdy,#function
         .section .text,"ax"
OSStartHighRdy:

#if      OS_CPU_HOOKS_EN > 0
         .extern OSTaskSwHook
         call     OSTaskSwHook
#endif  

         lds       r16,OSRunning
         inc       r16
         sts       OSRunning,r16
		 
         lds       r30,OSTCBHighRdy
         lds       r31,OSTCBHighRdy+1	 
         ld        r16,z+
         ld        r17,z
         out      _SFR_IO_ADDR(SPL),r16 ;
         out      _SFR_IO_ADDR(SPH),r17 ;
		 
		 pop     r0
		 out     _SFR_IO_ADDR(SREG),r0 ;
		 
		 pop     r0
		 pop     r1
		 pop     r2
		 pop     r3   
		 pop     r4
		 pop     r5
		 pop     r6
		 pop     r7 
		 pop     r8
		 pop     r9
		 pop     r10
		 pop     r11
		 pop     r12
		 pop     r13
		 pop     r14
		 pop     r15  
		 pop     r16
		 pop     r17
		 pop     r18
		 pop     r19   
		 pop     r20
		 pop     r21
		 pop     r22
		 pop     r23 
		 pop     r24
		 pop     r25
		 pop     r26
		 pop     r27 
		 pop     r28
		 pop     r29
		 pop     r30
		 pop     r31       

         ret   

;*************************************************************************************************************
         .global OSCtxSw
         .type OSCtxSw,#function
         .section .text,"ax"
OSCtxSw:
         push    r31
         push    r30
         push    r29
         push    r28
         push    r27
         push    r26
         push    r25
         push    r24
         push    r23
         push    r22
         push    r21
         push    r20
         push    r19
         push    r18
         push    r17
         push    r16
         push    r15
         push    r14
         push    r13
         push    r12
         push    r11
         push    r10
         push    r9
         push    r8
         push    r7
         push    r6
         push    r5
         push    r4
         push    r3
         push    r2
         push    r1
         push    r0
         in        r0,_SFR_IO_ADDR(SREG)
         push    r0 


         in        r16,_SFR_IO_ADDR(SPL)
         in        r17,_SFR_IO_ADDR(SPH)
         lds       r30,OSTCBCur
         lds       r31,OSTCBCur+1
         st        z+,r16
         st        z,r17

#if      OS_CPU_HOOKS_EN > 0
         .extern OSTaskSwHook
         call     OSTaskSwHook
#endif          

         lds       r16,OSTCBHighRdy
         lds       r17,OSTCBHighRdy+1
         sts       OSTCBCur,r16
         sts       OSTCBCur+1,r17

         lds       r16,OSPrioHighRdy
         sts       OSPrioCur,r16

         lds       r30,OSTCBHighRdy
         lds       r31,OSTCBHighRdy+1	 
         ld        r16,z+
         ld        r17,z
         out      _SFR_IO_ADDR(SPL),r16 ;
         out      _SFR_IO_ADDR(SPH),r17 ;
		 
		 pop     r0
		 out     _SFR_IO_ADDR(SREG),r0 ;
		 
		 pop     r0
		 pop     r1
		 pop     r2
		 pop     r3   
		 pop     r4
		 pop     r5
		 pop     r6
		 pop     r7 
		 pop     r8
		 pop     r9
		 pop     r10
		 pop     r11
		 pop     r12
		 pop     r13
		 pop     r14
		 pop     r15  
		 pop     r16
		 pop     r17
		 pop     r18
		 pop     r19   
		 pop     r20
		 pop     r21
		 pop     r22
		 pop     r23 
		 pop     r24
		 pop     r25
		 pop     r26
		 pop     r27 
		 pop     r28
		 pop     r29
		 pop     r30
		 pop     r31            

         ret       
;*************************************************************************************************************
         .global OSIntCtxSw
         .type OSIntCtxSw,#function
         .section .text,"ax"
OSIntCtxSw:
         in        r24,_SFR_IO_ADDR(SPL)
         in        r25,_SFR_IO_ADDR(SPH)
         adiw     r24,4
#if     OS_CRITICAL_METHOD == 2
         adiw     r24,1
#endif

#if     OS_CRITICAL_METHOD == 3
         adiw     r24,1
#endif

         out      _SFR_IO_ADDR(SPL),r24
         out      _SFR_IO_ADDR(SPH),r25

         lds       r30,OSTCBCur
         lds       r31,OSTCBCur+1
         st        z+,r24
         st        z,r25

#if      OS_CPU_HOOKS_EN > 0
         .extern OSTaskSwHook
         call     OSTaskSwHook
#endif          

         lds       r16,OSTCBHighRdy
         lds       r17,OSTCBHighRdy+1
         sts       OSTCBCur,r16
         sts       OSTCBCur+1,r17

         lds       r16,OSPrioHighRdy
         sts       OSPrioCur,r16

         lds       r30,OSTCBHighRdy
         lds       r31,OSTCBHighRdy+1	 
         ld        r16,z+
         ld        r17,z
         out      _SFR_IO_ADDR(SPL),r16 ;
         out      _SFR_IO_ADDR(SPH),r17 ;
		 
		 pop     r0
		 out     _SFR_IO_ADDR(SREG),r0 ;
		 
		 pop     r0
		 pop     r1
		 pop     r2
		 pop     r3   
		 pop     r4
		 pop     r5
		 pop     r6
		 pop     r7 
		 pop     r8
		 pop     r9
		 pop     r10
		 pop     r11
		 pop     r12
		 pop     r13
		 pop     r14
		 pop     r15  
		 pop     r16
		 pop     r17
		 pop     r18
		 pop     r19   
		 pop     r20
		 pop     r21
		 pop     r22
		 pop     r23 
		 pop     r24
		 pop     r25
		 pop     r26
		 pop     r27 
		 pop     r28
		 pop     r29
		 pop     r30
		 pop     r31            
         

         ret
;*************************************************************************************************************
         .extern OSTimeTick
         .extern OSIntExit
         .global OSTickISR
         .global SIG_OUTPUT_COMPARE0
SIG_OUTPUT_COMPARE0:
OSTickISR:
         push    r31
         push    r30
         push    r29
         push    r28
         push    r27
         push    r26
         push    r25
         push    r24
         push    r23
         push    r22
         push    r21
         push    r20
         push    r19
         push    r18
         push    r17
         push    r16
         push    r15
         push    r14
         push    r13
         push    r12
         push    r11
         push    r10
         push    r9
         push    r8
         push    r7
         push    r6
         push    r5
         push    r4
         push    r3
         push    r2
         push    r1
         push    r0
         in      r16,_SFR_IO_ADDR(SREG)
         ori     r16,0x80
         push    r16          

         lds     r16,OSIntNesting
         inc     r16
         sts     OSIntNesting,r16

         call     OSTimeTick
         call     OSIntExit

		 pop     r0
		 out     _SFR_IO_ADDR(SREG),r0 ;
		 
		 pop     r0
		 pop     r1
		 pop     r2
		 pop     r3   
		 pop     r4
		 pop     r5
		 pop     r6
		 pop     r7 
		 pop     r8
		 pop     r9
		 pop     r10
		 pop     r11
		 pop     r12
		 pop     r13
		 pop     r14
		 pop     r15  
		 pop     r16
		 pop     r17
		 pop     r18
		 pop     r19   
		 pop     r20
		 pop     r21
		 pop     r22
		 pop     r23 
		 pop     r24
		 pop     r25
		 pop     r26
		 pop     r27 
		 pop     r28
		 pop     r29
		 pop     r30
		 pop     r31            

         reti 
;*************************************************************************************************************
         .global   PushSREG
PushSREG:
         in      r0,_SFR_IO_ADDR(SREG)
         push  r0
         ret

         .global PopSreg
PopSREG:
         pop   r0
         out   _SFR_IO_ADDR(SREG),r0
         ret

     .end
