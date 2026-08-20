/**
 *****************************************************************************
 * @brief   driver def header.
 *
 * @file    tcpl_04x_ll_def.h
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

#ifndef __TCPL04X_LL_DEF_H__
#define __TCPL04X_LL_DEF_H__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device_registers.h"

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef  USE_FULL_ASSERT
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif

/**
 * @defgroup CRG LOCK Configuratoins_Definitions
 */

#if 1
#define CRG_CONFIG_LOCK()           (CRG->CRG_LOCK = 0x12345678)
#define CRG_CONFIG_UNLOCK()         (CRG->CRG_LOCK = 0x5a5a5a5a)
#else
#define CRG_CONFIG_LOCK()
#define CRG_CONFIG_UNLOCK()
#endif

/**
 * @defgroup SYSCFG LOCK Configuratoins_Definitions
 */
#if 0
#define SYSCFG_CONFIG_LOCK()        (SYSCFG->SYSCFG_LOCK=0x12345678)
#define SYSCFG_CONFIG_UNLOCK()      (SYSCFG->SYSCFG_LOCK=0xaa55aa55)
#else
#define SYSCFG_CONFIG_LOCK()
#define SYSCFG_CONFIG_UNLOCK()
#endif

/**
 * @defgroup ASYSCFG CONFIG Definitions
 */
#if 0
#define ASYSCFG_CONFIG_LOCK()       (ASYSCFG->ASYSCFG_LOCK=0x12345678)
#define ASYSCFG_CONFIG_UNLOCK()     (ASYSCFG->ASYSCFG_LOCK=0xaa55aa55)
#else
#define ASYSCFG_CONFIG_LOCK()
#define ASYSCFG_CONFIG_UNLOCK()
#endif

/**
 * @defgroup Test write lock definitions
 */
#if 1
#define TEST_CONFIG_LOCK()          (TEST->TEST_LOCK=0xfedcba98)
#define TEST_CONFIG_UNLOCK()        (TEST->TEST_LOCK=0x76543210)
#else
#define TEST_CONFIG_LOCK()
#define TEST_CONFIG_UNLOCK()
#endif

/**
 * @brief Performes a software reset
 */
#define LL_SOFTWARE_RESET()                             \
        do {                                            \
            CRG_CONFIG_UNLOCK();                        \
            CRG->M0_CLKRST_CTRL_F.RST_M0=1;             \
            CRG_CONFIG_LOCK();                          \
        } while(0)

/**
 * @brief Performes a software reset
 */
#define LL_SOFTWARE_RESET_FLAG_CLEAR()                  \
        do {                                            \
            CRG_CONFIG_UNLOCK();                        \
            CRG->RST_CTRL_F.CLR_RST = 1;                \
            CRG_CONFIG_LOCK();                          \
        } while(0)

/**
 * @brief system reset
 */
#define LL_SYSTEM_RESET()                               \
        do{                                             \
            ll_wdg_enable(false);                       \
            NVIC_SystemReset();                         \
        }while(0)

/**
 * @brief Hardware reset
 */
#define LL_HARDWARE_RESET()                             \
        do{                                             \
            ASYSCFG_CONFIG_UNLOCK();                    \
            ASYSCFG->RST_CTRL_F.SW_POR_REQ = 1;         \
            ASYSCFG_CONFIG_LOCK();                      \
        }while(0)

/**
 * @brief  fclk_src_e enumeration
 */
typedef enum
{
    FCLK_SRC_HSI,
    FCLK_SRC_LSI,
    FCLK_SRC_MAX,
} fclk_src_e;

/**
 * @brief  lin snpd type enumeration
 */
typedef enum
{
    LIN_SNPD_TYPE_STEPS_4   = 0x00,
    LIN_SNPD_TYPE_STEPS_3,
    LIN_SNPD_TYPE_STEPS_2,
    LIN_SNPD_TYPE_STEPS_SW4 = 0x10,
    LIN_SNPD_TYPE_STEPS_SW3,
    LIN_SNPD_TYPE_STEPS_SW2,
} lin_snpd_type_e;

/**
 * @brief  ll status enumeration
 */
typedef enum
{
    LL_OK       = 0x00U,
    LL_ERROR    = 0x01U,
    LL_BUSY     = 0x02U,
    LL_TIMEOUT  = 0x03U,
    LL_COMM_ERROR  = 0x04U,
    LL_PARAM_INVALID  = 0x05U
} ll_status_e;

/**
 * @brief CALLBACK FUNC
 */
typedef void (*ISR_FUNC_CALLBACK)(uint32_t);

/**
 * @brief  ll clk struct
 */
typedef struct
{
    fclk_src_e clk_source;
    uint8_t  fclk_div;
} ll_clk_config_t;

/**
 * @brief  ll clk struct
 */
typedef struct
{
    uint32_t isr;
    uint8_t priority;
    bool isr_enable;
} ll_isr_config_t;

#include "tcpl04x_ll_cortex.h"
#include "tcpl04x_ll_sys.h"
#include "tcpl04x_ll_adc.h"
#include "tcpl04x_ll_flash.h"
#include "tcpl04x_ll_gpio.h"
#include "tcpl04x_ll_lpm.h"
#include "tcpl04x_ll_pwm.h"
#include "tcpl04x_ll_sci.h"
#include "tcpl04x_ll_print.h"
#include "tcpl04x_ll_timer.h"
#include "tcpl04x_ll_wdg.h"
#include "tcpl04x_ll_led.h"

#if defined(__cplusplus)
}
#endif
#endif /*__TCPL04X_LL_DEF_H__*/
