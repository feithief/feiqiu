/**
 *****************************************************************************
 * @brief   pal timer header file.
 *
 * @file    pal_timer.h
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

#ifndef __PAL_TIMER_H__
#define __PAL_TIMER_H__

#include <stdint.h>
#include "pal_func_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TIMER_BASE_PERIOD
#define TIMER_BASE_PERIOD   (1000)
#endif


#ifndef TIMER_SRC_FDIV
#if defined (__TCPL01X__)       /* defined (__TCPL01X__) */
#define TIMER_SRC_FDIV      (0x10)
#elif  defined (__TCPL03X__) || defined (__TCPL04X__)    /* defined (__TCPL03X__) */
#define TIMER_SRC_FDIV      (0x2F)
#else    /* defined (__TCPL08X__) */
#define TIMER_SRC_FDIV      (0x47)
#endif /* define TIMER_SRC_FDIV */
#endif /* not defined (TIMER_SRC_FDIV) */

void pal_timer_init(uint16_t fclk_div, uint16_t period);
void pal_timer_deinit(void);
void pal_timer_enable(bool enable);
void pal_timer_isr_enable(bool enable);

#ifdef __cplusplus
}
#endif
#endif /* __PAL_TIMER_H__ */
