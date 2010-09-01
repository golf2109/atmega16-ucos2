;AVRvect.S
;AVR vectors

#include <avr/io.h>


;*************************************************************************************************************
;OS_EXT  INT8U             OSIntNesting;                        /* Interrupt nesting level                           */

        .extern    OSIntNesting
      
        .extern    OSTimeTick
        .extern    OSIntExit


;*************************************************************************************************************
.macro     PUSHREGS
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
.endm 

.macro     POPREGS
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
.endm


;*************************************************************************************************************
        .text

             .global SIG_INTERRUPT0
SIG_INTERRUPT0:
             PUSHREGS
             in        r16,_SFR_IO_ADDR(SREG)
             ori       r16,0x80
             push      r16
  
             lds      r16,OSIntNesting
             inc      r16
             sts      OSIntNesting,r16

             .extern  IsrEint0
             call     IsrEint0

             call     OSIntExit

             pop     r0
             out     _SFR_IO_ADDR(SREG),r0
             POPREGS
             reti
;*************************************************************************************************************
           .global SIG_INTERRUPT1
SIG_INTERRUPT1:
             PUSHREGS
             in        r16,_SFR_IO_ADDR(SREG)
             ori       r16,0x80
             push      r16
  
             lds      r16,OSIntNesting
             inc      r16
             sts      OSIntNesting,r16

             .extern  IsrEint1
             call       IsrEint1

             call     OSIntExit

             pop     r0
             out     _SFR_IO_ADDR(SREG),r0
             POPREGS
             reti

;*************************************************************************************************************
;*************************************************************************************************************


             .end
