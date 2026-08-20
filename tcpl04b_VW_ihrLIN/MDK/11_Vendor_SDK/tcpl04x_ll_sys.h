/**
 *****************************************************************************
 * @brief   syscfg header file.
 *
 * @file    tcpl04x_ll_sys.h
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

#ifndef __TCPL04X_LL_SYS_H__
#define __TCPL04X_LL_SYS_H__

#include "tcpl04x_ll_def.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
  * @defgroup ASYSCFG_OTP_RST_Definitions
  */
#define ASYSCFG_OTP_RST_ENABLE()    (ASYSCFG->OTP_RST_CTRL_F.OTP_RST_EN=1)
#define ASYSCFG_OTP_RST_DISABLE()   (ASYSCFG->OTP_RST_CTRL_F.OTP_RST_EN=0)

/**
  * @defgroup ASYSCFG_HRC_CLOCK_Config_Macro
  */
/*Enable software HRC, when it enable the PMU high power mode ready*/
#define ASYSCFG_SW_HRC_ENABLE()                     (ASYSCFG->RCCLK_CTRL_F.SW_HRC_EN=1)
/*Disable software HRC, */
#define ASYSCFG_SW_HRC_DISABLE()                    (ASYSCFG->RCCLK_CTRL_F.SW_HRC_EN=0)
/*Disable software HRC clock in sleepwalk mode*/
#define ASYSCFG_HRC_CLOCK_DISABLE_ON_SLEEPWALK()    (ASYSCFG->PMU_CTRL_F.SLWK_HRC_EN=0)
/*Enable software HRC clock in sleepwalk mode*/
#define ASYSCFG_HRC_CLOCK_ENABLE_ON_SLEEPWALK()     (ASYSCFG->PMU_CTRL_F.SLWK_HRC_EN=1)

/** ]
  * @defgroup ASYSCFG_RESET_CAUSE_Definitions
  */
#define ASYSCFG_RST_CAUSE_SW_POR_REQ        0X01
#define ASYSCFG_RST_CAUSE_IO4_PAD_RST       0X02
#define ASYSCFG_RST_CAUSE_IWDG              0X04
#define ASYSCFG_RST_CAUSE_OTP               0X08
#define ASYSCFG_RST_CAUSE_VS_ALT            0X10
#define ASYSCFG_RST_CAUSE_CM0_RST           0X20    //CM0 Reset or CM0 lockup

/**
  * @defgroup ASYSCFG_RESET_CONTROL_Definitions
  */
#define ASYSCFG_CM0_LOCKUP_RST_ENABLE()     (ASYSCFG->RST_CTRL_F.M0_LOCKUP_EN=1)
#define ASYSCFG_CM0_LOCKUP_RST_DISABLE()    (ASYSCFG->RST_CTRL_F.M0_LOCKUP_EN=0)
/*Request a software reset, this will reset all logic*/
#define ASYSCFG_RST_REQUEST()               (ASYSCFG->RST_CTRL_F.SW_POR_REQ=1)
/*Get the reset cuase*/
#define ASYSCFG_RST_CAUSE_GET()             (ASYSCFG->RST_CTRL_F.RST_FLAG)
/*Clear the reset cuase*/
#define ASYSCFG_RST_CAUSE_CLEAR()           (ASYSCFG->RST_CTRL_F.CLR_RST=1)

/**
  * @defgroup SYSCFG_MISC_CONFIGS_Definitions
  */
#define SYSCFG_PWM_DISABLE_WHEN_OTP(status) \
    (SYSCFG->MISC_CTRL_F.OTP_DIS_PWM_EN=status)     /*!< Enable or disable pwm on OTP event happens
                                                         status can be ENABLE or DISABLE*/

#define SYSCFG_PWM_DISABLE_WHEN_VS_ALTER(status) \
    (SYSCFG->MISC_CTRL_F.VS_ALT_DIS_PWM_EN=status)  /*!< Enable or disable pwm on VS ALTER event happens
                                                         status can be ENABLE or DISABLE*/

#define SYSCFG_PWM_DISABLE_WHEN_VS_LVD(status) \
    (SYSCFG->MISC_CTRL_F.VS_LVD_DIS_PWM_EN=status)  /*!< Enable or disable pwm on VS LVD event happens
                                                         status can be ENABLE or DISABLE*/
/**
 * @brief  wakeup source enumeration
 */
typedef enum
{
    WAKEUP_SOUERCE_LIN  = 0,
    WAKEUP_SOURCE_IO0,
    WAKEUP_SOURCE_MAX
} wakeup_source_e;

/**
 * @brief  wakeup time enumeration, 1clk unit cycle = 1/32KHz = 31.25 us
 */
typedef enum
{
    WAKEUP_TIME_1  = 0,
    WAKEUP_TIME_2,
    WAKEUP_TIME_3,
    WAKEUP_TIME_4,
    WAKEUP_TIME_5,
    WAKEUP_TIME_6,
    WAKEUP_TIME_7,
    WAKEUP_TIME_8,
    WAKEUP_TIME_9,
    WAKEUP_TIME_10,
    WAKEUP_TIME_11,
    WAKEUP_TIME_12,
    WAKEUP_TIME_13,
    WAKEUP_TIME_14,
    WAKEUP_TIME_15,
    WAKEUP_TIME_16,
    WAKEUP_TIME_MAX
} wakeup_time_e;

/**
 * @brief  wakeup filter enumeration, 1clk unit cycle = 1/32KHz = 31.25 us
 */
typedef enum
{
    WAKEUP_FILTER_1  = 0,
    WAKEUP_FILTER_2,
    WAKEUP_FILTER_3,
    WAKEUP_FILTER_4,
    WAKEUP_FILTER_MAX
} wakeup_filter_e;

/**
 * @brief  syscfg Interrupt enumeration
 */
typedef enum
{
    ASYSCFG_INT_WAKEUP        = ASYSCFG_IMR_WU_INT_MSK_MASK,
    ASYSCFG_INT_VSLVD         = ASYSCFG_IMR_VS_LVD_INT_MSK_MASK,
    ASYSCFG_INT_OTP           = ASYSCFG_IMR_OTP_INT_MSK_MASK,
    ASYSCFG_INT_RESERVED12_OCP = ASYSCFG_ICR_RESERVED12_OCP_INT_CLR_MASK,
} asyscfg_isr_type_e;

/**
 * @brief  syscfg Interrupt triger flag enumeration
 */
typedef enum
{
    ASYSCFG_TRIGGER_NULL        = 0x00,  /* null */
    ASYSCFG_TRIGGER_POSEDGE     = 0x01,  /* posedge */
    ASYSCFG_TRIGGER_NEGEDGE     = 0x02,  /* negedge */
    ASYSCFG_TRIGGER_HIGH_LEVEL  = 0x04,  /* high level */
} asyscfg_trigger_flag_e;


/**
 * @brief  system backup zone enumeration
 */
typedef enum
{
    SYSTEM_BACKUP_ZONE_0,
    SYSTEM_BACKUP_ZONE_1,
    SYSTEM_BACKUP_ZONE_MAX

} sys_backup_zone_e;

void ll_syscfg_info_get(uint8_t *revision_id, uint16_t *chip_id);
void ll_wakeup_init(wakeup_source_e source, wakeup_time_e time, wakeup_filter_e filter);
void ll_syscfg_lvd_enable(bool enable, ISR_FUNC_CALLBACK callback);
void ll_syscfg_isr_enable(asyscfg_isr_type_e isr, bool enable);
void ll_syscfg_otp_enable(bool enable);
bool ll_syscfg_otp_status(void);
bool ll_syscfg_lvd_status(void);
void ll_syscfg_patch_set(uint8_t patch);
void ll_syscfg_isr_clear(asyscfg_isr_type_e isr);
uint8_t ll_syscfg_isr_get(asyscfg_isr_type_e isr);
void ll_syscfg_backup_reg_write(sys_backup_zone_e zone, uint32_t data);
void ll_syscfg_backup_reg_read(sys_backup_zone_e zone, uint32_t *data);

#if defined(__cplusplus)
}
#endif
#endif /* __TCPL04X_LL_SYS_H__ */
