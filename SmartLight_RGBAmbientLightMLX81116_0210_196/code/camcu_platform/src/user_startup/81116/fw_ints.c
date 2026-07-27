/**
 * @file
 * @brief Weak implementations of the interrupt vector handlers.
 * @internal
 *
 * @copyright (C) 2018 Melexis N.V.
 * git flash 137e5489
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @addtogroup fw_user_startup
 *
 * @details This file contains the implementations of weak interrupt vector handlers.
 */

#include <syslib.h>

#define __interrupt__ __attribute__((interrupt, weak))

extern void _fatal (void);

__interrupt__ void _MLX16_EXCHG_INT(void)
{
    _fatal();
}
__interrupt__ void _MLX16_DMAERR_INT(void)
{
    _fatal();
}
__interrupt__ void _AWD_ATT_INT(void)
{
    _fatal();
}
__interrupt__ void _IWD_ATT_INT(void)
{
    _fatal();
}
__interrupt__ void _EE_ECC_INT(void)
{
    _fatal();
}
__interrupt__ void _FL_ECC_INT(void)
{
    _fatal();
}
__interrupt__ void _UV_VDDA_INT(void)
{
    _fatal();
}
__interrupt__ void _UV_VS_INT(void)
{
    _fatal();
}
__interrupt__ void _OVT_INT(void)
{
    _fatal();
}
__interrupt__ void _STIMER_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER0_1_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER0_2_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER0_3_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER1_1_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER1_2_INT(void)
{
    _fatal();
}
__interrupt__ void _CTIMER1_3_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_MASTER1_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_MASTER1_END_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE1_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE2_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE3_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE4_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE5_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE6_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE7_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE8_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE9_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE10_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _PWM_SLAVE11_CMP_INT(void)
{
    _fatal();
}
__interrupt__ void _ADC_SAR_INT(void)
{
    _fatal();
}
__interrupt__ void _EE_COMPLETE_INT(void)
{
    _fatal();
}
__interrupt__ void _FL_COMPLETE_INT(void)
{
    _fatal();
}
__interrupt__ void _OV_VS_INT(void)
{
    _fatal();
}
__interrupt__ void _OVT_WARN_INT(void)
{
    _fatal();
}
__interrupt__ void _UV_VDD5_INT(void)
{
    _fatal();
}
__interrupt__ void _AA_IN_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_SB_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_RS_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_RR_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_TS_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_TR_INT(void)
{
    _fatal();
}
__interrupt__ void _UART_TE_INT(void)
{
    _fatal();
}
__interrupt__ void _MFT_INT(void)
{
    _fatal();
}
__interrupt__ void _MFR_INT(void)
{
    _fatal();
}
__interrupt__ void _MHR_INT(void)
{
    _fatal();
}
__interrupt__ void _MER_INT(void)
{
    _fatal();
}
__interrupt__ void _TX_TIMEOUT_INT(void)
{
    _fatal();
}
__interrupt__ void _MLX16_SOFT_INT(void)
{
    _fatal();
}

/* EOF */
