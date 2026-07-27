;**********************************************************************************************************************
;
; @file     startup_tld40xx.S
; @brief    CMSIS Core Device Startup File for
;           Infineon TLD40xx Device Series
; @version  V1.0.0
; @date     21-Oct-2022
;
;
; Copyright (c) 2022, Infineon Technologies AG
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
; following conditions are met:
;
;   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
;   disclaimer.
;
;   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
;   following disclaimer in the documentation and/or other materials provided with the distribution.
;
;   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
;   products derived from this software without specific prior written permission.
;
; This software is provided by the copyright holders and contributors "as is" and any express or implied warranties,
; including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are
; disclaimed. In no event shall the copyright holder or contributors be liable  for any direct, indirect, incidental,
; special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or
; services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability,
; whether in contract, strict liability,or tort (including negligence or otherwise) arising in any way out  of the
; use of this software, even if advised of the possibility of such damage.
;
;**********************************************************************************************************************/

;*******************************************************************************
;                       Author(s) Identity                                    **
;*******************************************************************************
;  Initials     Name                                                          **
;  ---------------------------------------------------------------------------**
;  CG           Christopher Gabriel                                           **
;  SL           Stefano Lissandron                                            **
;******************************************************************************/

;*******************************************************************************
;                       Revision Control History                              **
;*******************************************************************************
;  V1.0.0: 21-Oct-2022, SL:   Initial version                                 **
;******************************************************************************/

;
;-------- <<< Use Configuration Wizard in Context Menu >>> -------------------
;
;--------------------- Stack / Heap Configuration ---------------------------
; <h> Stack / Heap Configuration
;   <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;   <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
;----------------------------------------------------------------------------*/
Stack_Size      EQU     0x00000600
Heap_Size       EQU     0x00000000
;------------- <<< end of configuration section >>> ---------------------------
;
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                THUMB
                PRESERVE8


;*******************************************************************************
; Fill-up the Vector Table entries with the exceptions ISR address
;*******************************************************************************
                AREA    RESET, DATA, READONLY
                INCLUDE bsl_defines_ARM.inc
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size


                ; First page, starts at 0x12000000: Vector table
__Vectors       DCD     __initial_sp
                DCD     Reset_Handler
                DCD     NMI_Handler                ; -14: NMI Handler
                DCD     HardFault_Handler          ; -12: Hard Fault Handler
                DCD     0                          ; -12: Reserved
                DCD     0                          ; -11: Reserved
                DCD     0                          ; -10: Reserved
                DCD     0                          ;  -9: Reserved
                DCD     0                          ;  -8: Reserved
                DCD     0                          ;  -7: Reserved
                DCD     0                          ;  -6: Reserved
                DCD     SVC_Handler                ;  -5: SVCall Handler
                DCD     0                          ;  -4: Reserved
                DCD     0                          ;  -3: Reserved
                DCD     PendSV_Handler             ;  -2: PendSV Handler
                DCD     SysTick_Handler            ;  -1: SysTick Handler

                ; Second page, External Interrupts
                DCD     NVIC_IRQ0_Handler          ; 0: PMU
                DCD     NVIC_IRQ1_Handler          ; 1: Reserved
                DCD     NVIC_IRQ2_Handler          ; 2: LIN
                DCD     NVIC_IRQ3_Handler          ; 3: Reserved
                DCD     NVIC_IRQ4_Handler          ; 4: ADC
                DCD     NVIC_IRQ5_Handler          ; 5: LED
                DCD     NVIC_IRQ6_Handler          ; 6: LED
                DCD     NVIC_IRQ7_Handler          ; 7: GPIO

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.ARM.__at_0x120000C0|, DATA, READONLY
                ; Fourth page, device config
                DCD     BSL_FTO_NAD_NAC            ; 0x120000C0: FTO_NAD_NAC
                DCD     0                          ; 0x120000C4
                DCD     BSL_UCODE_UBSL_SIZE        ; 0x120000C8: UCODE_UBSL_SIZE
                DCD     0                          ; 0x120000CC
                DCD     0                          ; 0x120000D0
                DCD     0                          ; 0x120000D4
                DCD     0                          ; 0x120000D8
                DCD     0                          ; 0x120000DC
                DCD     0                          ; 0x120000E0
                DCD     0                          ; 0x120000E4
                DCD     0                          ; 0x120000E8
                DCD     0                          ; 0x120000EC
                DCD     0                          ; 0x120000F0
                DCD     0                          ; 0x120000F4
                DCD     0                          ; 0x120000F8
                DCD     0                          ; 0x120000FC

                AREA    |.text|, CODE, READONLY

;-------------------------------------------------------------------------------
; Reset Handler
;
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

;-------------------------------------------------------------------------------
; Dummy Exception Handlers (infinite loops which can be modified)
;
SVC_Handler_main\
                PROC
                EXPORT  SVC_Handler_main            [WEAK]
                B       .
                ENDP
NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                MOVS    R0, #4
                MOV     R1, LR
                TST     R0, R1
                BEQ     stacking_used_MSP
                MRS     R0, PSP ; first parameter - stacking was using PSP
                LDR     R1, =SVC_Handler_main
                BX      R1
stacking_used_MSP
                MRS     R0, MSP ; first parameter - stacking was using MSP
                LDR     R1, =SVC_Handler_main
                BX      R1
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  NVIC_IRQ0_Handler            [WEAK]
                EXPORT  NVIC_IRQ1_Handler            [WEAK]
                EXPORT  NVIC_IRQ2_Handler            [WEAK]
                EXPORT  NVIC_IRQ3_Handler            [WEAK]
                EXPORT  NVIC_IRQ4_Handler            [WEAK]
                EXPORT  NVIC_IRQ5_Handler            [WEAK]
                EXPORT  NVIC_IRQ6_Handler            [WEAK]
                EXPORT  NVIC_IRQ7_Handler            [WEAK]

NVIC_IRQ0_Handler
NVIC_IRQ1_Handler
NVIC_IRQ2_Handler
NVIC_IRQ3_Handler
NVIC_IRQ4_Handler
NVIC_IRQ5_Handler
NVIC_IRQ6_Handler
NVIC_IRQ7_Handler
                B       .
                ENDP

;-------------------------------------------------------------------------------
; User Initial Stack & Heap
;
                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END

