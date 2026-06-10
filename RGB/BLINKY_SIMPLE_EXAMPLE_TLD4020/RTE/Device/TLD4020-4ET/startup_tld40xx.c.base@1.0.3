/*
 * Copyright (c) 2025 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
* \file     startup_tld40xx.c
*
* \brief    CMSIS-Core(M) Device Startup File for TLD40xx
*
* \version  V1.0.3
* \date     02. Sep 2025
*
* \note
*/

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** SL           Stefano Lissandron                                            **
** VO           Vanessa Ongaro                                                **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2024-07-22, VO:   [ATVCOMETLL-159] Added revision history, added   **
**                           work instruction, updated disclaimer             **
** V1.0.2: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
** V1.0.3: 2025-09-02, VO:   [ATVCOMETLL-205] Updated comment for IRQ3        **
*******************************************************************************/

/* Work instructions:
   When using this startup file, the project needs a linker script.
   The linker script is copied to the RTE/Device/--devicename-- folder of this
   project and needs to be added to the project manually.
   In Keil:
   - Under 'Options for target' > 'Linker' tab: uncheck tickbox 'Use Memory
     Layout from Target Dialog'
   - For Scatter file: '...': add the linker file mentioned above (tld40xx.sct)
*/

#include "device.h"
#include "bsl_defines.h"

/*---------------------------------------------------------------------------
  External References
 *---------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;

extern __NO_RETURN void __PROGRAM_START(void);

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler  (void);
void Default_Handler(void);

/*---------------------------------------------------------------------------
  Exception / Interrupt Handler
 *---------------------------------------------------------------------------*/
/* Exceptions */
void NMI_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler      (void) __attribute__ ((weak));
void SVC_Handler            (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler         (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler        (void) __attribute__ ((weak, alias("Default_Handler")));

/* Default interrupt handler */
void NVIC_IRQ0_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ1_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ2_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ3_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ4_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ5_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ6_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));
void NVIC_IRQ7_Handler      (void) __attribute__ ((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[VECTOR_TABLE_SIZE];
const VECTOR_TABLE_Type __VECTOR_TABLE[VECTOR_TABLE_SIZE] __VECTOR_TABLE_ATTRIBUTE =
{
  (VECTOR_TABLE_Type)(&__INITIAL_SP),       /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  NVIC_IRQ0_Handler,                        /* 0   PMU - first Device Interrupt */
  NVIC_IRQ1_Handler,                        /* 1   Reserved */
  NVIC_IRQ2_Handler,                        /* 2   LIN */
  NVIC_IRQ3_Handler,                        /* 3   UART2 */
  NVIC_IRQ4_Handler,                        /* 4   ADC */
  NVIC_IRQ5_Handler,                        /* 5   LED */
  NVIC_IRQ6_Handler,                        /* 6   LED */
  NVIC_IRQ7_Handler                         /* 7   GPIO */
};


extern const uint32_t CONFIG_TABLE[CONFIG_TABLE_SIZE];
#if defined(__ARMCC_VERSION)
  /* Arm Compiler V5 and V6 */
  const uint32_t CONFIG_TABLE[CONFIG_TABLE_SIZE] __attribute__((section(".ARM.__at_0x120000C0"), used)) =
#elif defined(__IAR_SYSTEMS_ICC__)
  /* IAR compiler */
  __root const uint32_t CONFIG_TABLE[CONFIG_TABLE_SIZE] @ "sec_BOOTROM_CFG" =
#endif
{
  BSL_FTO_NAD_NAC,
  0,
  BSL_UCODE_UBSL_SIZE
};

#if defined ( __GNUC__ )
  #pragma GCC diagnostic pop
#endif

/*---------------------------------------------------------------------------
  Reset Handler called on controller reset
 *---------------------------------------------------------------------------*/
__NO_RETURN void Reset_Handler(void)
{
  __set_PSP((uint32_t)(&__INITIAL_SP));
  SystemInit();                    /* CMSIS System Initialization */
  __PROGRAM_START();               /* Enter PreMain (C library entry point) */
}


#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

/*---------------------------------------------------------------------------
  Hard Fault Handler
 *---------------------------------------------------------------------------*/
void HardFault_Handler(void)
{
  while (1);
}

/*---------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *---------------------------------------------------------------------------*/
void Default_Handler(void)
{
  while (1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#endif
