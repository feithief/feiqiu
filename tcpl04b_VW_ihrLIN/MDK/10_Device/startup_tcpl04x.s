;
;*****************************************************************************
; @brief   startup_tcpl04x.s  file.
;
; @file    startup_tcpl04x.s
; @author  AE/FAE team
; @date    2024.01.01
;******************************************************************************
;
; THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
; WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
; TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
; DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
; FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
; CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;
; <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
;
; *****************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
    IF	:DEF:STACK_SIZE
Stack_Size      EQU     STACK_SIZE
    ELSE
Stack_Size      EQU     0x400
    ENDIF

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size      EQU     0x00

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                   ; Top of Stack
                DCD     Reset_Handler             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick
                ; External Interrupts
                DCD     FLASH_IRQHandler          ;
                DCD     ADC_IRQHandler            ;
                DCD     PWM_IRQHandler            ;
                DCD     TIMER_IRQHandler          ;
                DCD     IWDG_IRQHandler           ;
                DCD     SCI_IRQHandler            ;
                DCD     AON_IRQHandler            ;
                DCD     GPIO_IRQHandler           ;
                DCD     RESERVED_S_IRQHandler     ;
                DCD     RESERVED_T_IRQHandler     ;
                DCD     UART_IRQHandler           ;
                SPACE    ( 21 * 4)                ; Interrupts 11 .. 31 are left out

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler    PROC
                 EXPORT  Reset_Handler                 [WEAK]
                 IMPORT  TcMain
                 IMPORT  SystemInit
                 IMPORT  |Image$$RW_IRAM1$$ZI$$Base|
                 IMPORT  |Image$$RW_IRAM1$$ZI$$Limit|
                 ;interrupt disable
                 CPSID   I
                 ;stack space zero init
                 MOVS    R0, #0
                 LDR     R1, =|Image$$RW_IRAM1$$ZI$$Base|
                 LDR     R2, =|Image$$RW_IRAM1$$ZI$$Limit|
FillZero
                 STR     R0, [R1, #0]
                 ADDS    R1, R1, #4
                 CMP     R1, R2
                 BCC     FillZero

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =TcMain
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler                [WEAK]
                B       .
                ENDP
Default_Handler PROC
                EXPORT  FLASH_IRQHandler               [WEAK]
                EXPORT  ADC_IRQHandler                  [WEAK]
                EXPORT  PWM_IRQHandler                  [WEAK]
                EXPORT  TIMER_IRQHandler                [WEAK]
                EXPORT  IWDG_IRQHandler                 [WEAK]
                EXPORT  SCI_IRQHandler                  [WEAK]
                EXPORT  AON_IRQHandler                  [WEAK]
                EXPORT  GPIO_IRQHandler                 [WEAK]
                EXPORT  RESERVED_S_IRQHandler           [WEAK]
                EXPORT  RESERVED_T_IRQHandler           [WEAK]
                EXPORT  UART_IRQHandler                 [WEAK]

FLASH_IRQHandler
ADC_IRQHandler
PWM_IRQHandler
TIMER_IRQHandler
IWDG_IRQHandler
SCI_IRQHandler
AON_IRQHandler
GPIO_IRQHandler
RESERVED_S_IRQHandler
RESERVED_T_IRQHandler
UART_IRQHandler
                B       .
                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
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

;************************ (C) COPYRIGHT Tinychip Microelectronics Co.,Ltd*******
