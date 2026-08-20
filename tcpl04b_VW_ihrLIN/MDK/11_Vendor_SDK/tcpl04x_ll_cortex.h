/**
 *****************************************************************************
 * @brief   tcpl04x_ll_cortex header file.
 *
 * @file    tcpl04x_ll_cortex.h
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

#ifndef __TCPL04X_LL_CORTEX_H__
#define __TCPL04X_LL_CORTEX_H__

#include "tcpl04x_ll_def.h"

#ifdef __cplusplus
extern "C"
{
#endif

void EnableNvic(uint32_t irq, uint8_t level, bool en);

#ifdef __cplusplus
}
#endif
#endif /* __TCPL04X_LL_CORTEX_H__ */
