/**
 *****************************************************************************
 * @brief   syscfg Source file.
 *
 * @file    tcpl04x_ll_sys.c
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

#include "tcpl04x_ll_sys.h"
#include "tcpl04x_ll_def.h"
#include "tcpl04x_ll_cortex.h"

/********************************************************
** \brief   Gets MCU die's revision ID and chip ID
**
** \param   uint8_t* revision_id:pointer to hold revision ID when it being read
** \param   uint16_t* chip_id:pointer to hold chip ID when it being read
**
** \retval  None
*********************************************************/
void ll_syscfg_info_get(uint8_t *revision_id, uint16_t *chip_id)
{
    SYSCFG_CONFIG_UNLOCK();

    if (NULL != chip_id)
    {
        *chip_id = (uint16_t)(SYSCFG->REVISION_F.CHIP_ID);
    }

    if (NULL != revision_id)
    {
        *revision_id = (uint8_t)(SYSCFG->REVISION_F.REV_ID);
    }

    SYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function initializes the wakeup function
**
** \param   wakeup_source_e     source:specifies the wakeup source
** \param   wakeup_time_e       time:specifies the max time the wakeup signal, cycle = 1/32KHz = 31.25 us
** \param   wakeup_filter_e     filter:specifies the max ammount of clocks to filter the wakeup signal
**
** \retval  None
*********************************************************/
void ll_wakeup_init(wakeup_source_e source, wakeup_time_e time, wakeup_filter_e filter)
{
    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->WU_CTRL_F.WU_SRC_SEL = source;
    ASYSCFG->WU_CTRL_F.WU_TIM = time;
    ASYSCFG->WU_CTRL_F.WU_FILTER_CNT_MAX = filter;
    ASYSCFG->WU_CTRL_F.WU_EN = 1;

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function enables  Asys_cfg related interrupt
**
** \param   asyscfg_isr_type_e     isr :it can be any value of @ref asyscfg_isr_type_e
** \param   bool                   enable
**
** \retval  None
*********************************************************/
void ll_syscfg_isr_enable(asyscfg_isr_type_e isr, bool enable)
{
    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->ICR |= (isr);

    if (enable)
    {
        ASYSCFG->IMR &= ~(isr);
    }
    else
    {
        ASYSCFG->IMR |= (isr);
    }

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function clears Asys_cfg related interrupt flag
**
** \param   asyscfg_isr_type_e     isr :it can be any value of @ref asyscfg_isr_type_e
**
** \retval  None
*********************************************************/
void ll_syscfg_isr_clear(asyscfg_isr_type_e isr)
{
    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->ICR |= (isr);

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function get Asys_cfg related interrupt flag
**
** \param   asyscfg_isr_type_e     isr :it can be any value of @ref asyscfg_isr_type_e
**
** \retval  status
*********************************************************/
uint8_t ll_syscfg_isr_get(asyscfg_isr_type_e isr)
{
    return (uint8_t)(ASYSCFG->ISR & (isr));
}

/********************************************************
** \brief   This function writes a word of data to a selected backup register
**
** \param   sys_backup_zone_e     zone
** \param   uint32_t              data
**
** \retval  None
*********************************************************/
void ll_syscfg_backup_reg_write(sys_backup_zone_e zone, uint32_t data)
{
    ASYSCFG_CONFIG_UNLOCK();

    if (SYSTEM_BACKUP_ZONE_0 == zone)
    {
        ASYSCFG->BKUP0 = data;
    }
    else
    {
        ASYSCFG->BKUP1 = data;
    }

    ASYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   This function reads the selected backup register's content
**
** \param   sys_backup_zone_e     zone
** \param   uint32_t              *data
**
** \retval  None
*********************************************************/
void ll_syscfg_backup_reg_read(sys_backup_zone_e zone, uint32_t *data)
{
    if (SYSTEM_BACKUP_ZONE_0 == zone)
    {
        *data = ASYSCFG->BKUP0;
    }
    else
    {
        *data = ASYSCFG->BKUP1;
    }
}

/********************************************************
** \brief   ll_syscfg_otp_enable
**
** \param   bool          enable
**
** \retval  void
*********************************************************/
void ll_syscfg_otp_enable(bool enable)
{
    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->PMU_IRQ_CTRL_F.OTP_IRQ_MODE = ASYSCFG_TRIGGER_POSEDGE;

    ASYSCFG_CONFIG_LOCK();

    ll_syscfg_isr_enable(ASYSCFG_INT_OTP, enable);
    NVIC_ClearPendingIRQ(AON_IRQn);

    if (enable)
    {
        NVIC_SetPriority(AON_IRQn, 3);
        NVIC_EnableIRQ(AON_IRQn);
    }
    else
    {
        NVIC_DisableIRQ(AON_IRQn);
    }
}

/********************************************************
** \brief   ll_syscfg_otp_status
**
** \param   None
**
** \retval  bool
*********************************************************/
bool ll_syscfg_otp_status(void)
{
    return !!(ASYSCFG->OPT_STATUS_F.OTP_OUT);
}

/********************************************************
** \brief   ll_syscfg_lvd_status
**
** \param   None
**
** \retval  bool
*********************************************************/
bool ll_syscfg_lvd_status(void)
{
    return !!(ASYSCFG->VS_LVD_CTRL_F.VS_LVD);
}

/********************************************************
** \brief   ll_syscfg_lvd_enable
**
** \param   bool                enable
** \param   ISR_FUNC_CALLBACK   callback
**
** \retval  void
*********************************************************/
void ll_syscfg_lvd_enable(bool enable, ISR_FUNC_CALLBACK callback)
{

    // lvd_isr_callback = callback;
    ll_syscfg_isr_enable(ASYSCFG_INT_VSLVD, enable);
    NVIC_ClearPendingIRQ(AON_IRQn);
    ASYSCFG_CONFIG_UNLOCK();
    ASYSCFG->PMU_IRQ_CTRL_F.VS_LVD_IRQ_MODE = ASYSCFG_TRIGGER_POSEDGE;
    ASYSCFG->VS_LVD_CTRL_F.VS_LVD_EN = enable;
    ASYSCFG_CONFIG_LOCK();

    if (enable)
    {
        NVIC_SetPriority(AON_IRQn, 3);
        NVIC_EnableIRQ(AON_IRQn);
    }
}

/********************************************************
** \brief   ll_syscfg_patch_set
**
** \param   None
**
** \retval  None
*********************************************************/
void ll_syscfg_patch_set(uint8_t patch)
{
    if (patch > 0x0F)
    {
        return;
    }

    SYSCFG_CONFIG_UNLOCK();

    SYSCFG->PATCH_BIT_F.PATCH_BIT = patch;

    SYSCFG_CONFIG_LOCK();
}

/********************************************************
** \brief   AON_IRQHandler
**
** \param   None
**
** \retval  None
*********************************************************/
void AON_IRQHandler(void)
{
    uint32_t isr = ASYSCFG->ISR;

    ASYSCFG_CONFIG_UNLOCK();

    ASYSCFG->ICR |= isr;

    ASYSCFG_CONFIG_LOCK();
}
