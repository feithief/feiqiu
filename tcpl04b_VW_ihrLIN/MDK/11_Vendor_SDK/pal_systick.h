/**
 *****************************************************************************
 * @brief   pal systick header file.
 *
 * @file    pal_systick.h
 * @author  AE/FAE team
 * @date    2024.01.01
 *****************************************************************************
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, TINYCHIP SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * <b>&copy; Copyright (c) 2024 Tinychip Microelectronics Co.,Ltd.</b>
 *
 *****************************************************************************
 */

#ifndef __PAL_SYSTICK_H__
#define __PAL_SYSTICK_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t systick_count_get(void);
uint32_t systick_diff(uint32_t start_tick);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif
#endif
