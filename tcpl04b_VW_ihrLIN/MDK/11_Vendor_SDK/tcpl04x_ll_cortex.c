/**
 *****************************************************************************
 * @brief   interrupt Source file.
 *
 * @file    tcpl04x_ll_cortex.c
 * @author  AE/FAE team
 * @date    2026.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2026 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#include "tcpl04x_ll_def.h"
#include "tcpl04x_ll_cortex.h"

/********************************************************
** \brief   NMI_Handler
**
** \param   None
**
** \retval  None
*********************************************************/
void NMI_Handler(void)
{
    LL_HARDWARE_RESET();
}

/********************************************************
** \brief   HardFault_Handler
**
** \param   None
**
** \retval  None
*********************************************************/
void HardFault_Handler(void)
{
    LL_HARDWARE_RESET();
}

/********************************************************
** \brief  EnableNvic
**
** \param   uint32_t        irq
** \param   uint8_t         level
** \param   bool            en
**
** \retval  None
**
** \note   the Cortex-M0 or Cortex-M0+/the dynamic changing of the priority
**         level of enabled interrupts or exceptions is not supported,
**         You should setup the priority levels before enabling the interrupts
*********************************************************/
void EnableNvic(uint32_t irq, uint8_t level, bool en)
{
    IRQn_Type enIrq = (IRQn_Type)irq;
    NVIC_DisableIRQ(enIrq);
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, level);

    if (true == en)
    {
        NVIC_EnableIRQ(enIrq);
    }
}
