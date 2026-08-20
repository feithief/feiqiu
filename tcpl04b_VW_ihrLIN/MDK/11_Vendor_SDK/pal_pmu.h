/**
 *****************************************************************************
 * @brief   pal pmu header file.
 *
 * @file    pal_pmu.h
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

#ifndef __PAL_PMU_H__
#define __PAL_PMU_H__

#include "pal_func_def.h"

#ifdef __cplusplus
extern "C" {
#endif

void pmu_lpm_enter(sleep_mode_e mode);
void pmu_lpm_exit(void);
void pmu_lpm_init(void);
void pmu_otp_enable(bool enable);
bool pmu_otp_status(void);
bool pmu_lvd_status(void);

#ifdef __cplusplus
}
#endif
#endif /*__PAL_PMU_H__*/
