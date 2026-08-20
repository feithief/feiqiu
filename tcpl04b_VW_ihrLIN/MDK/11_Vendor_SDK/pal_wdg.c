/**
 *****************************************************************************
 * @brief   pal wdg source file.
 *
 * @file    pal_wdg.c
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

#include "pal_wdg.h"

/********************************************************
** \brief   wdg_init
**
** \param   uint16_t        cnt_ms
**
** \retval  None
*********************************************************/
void wdg_init(uint16_t cnt_ms)
{
    wdg_config_t wdg =
    {
#if defined (__TCPL01X__)
        .clk_cfg =
        {
            .fclk_div = 32 * 5,
        },
#endif
        .isr_cfg = {
            .isr_enable = false,
            .priority = 3,
        },
#if defined (__TCPL01X__)
        .max_count = cnt_ms / 5,
#elif (defined (__TCPL03X__) || defined (__TCPL04X__) || defined (__TCPL08X__))
        .max_count = cnt_ms << 5, /* x32 */
#endif
        .reset_on_overflow = true,
    };
    ll_wdg_init(&wdg, NULL);

    // ll_wdg_isr_enable(true);
    ll_wdg_enable(true);
}

/********************************************************
** \brief   wdg_deinit
**
** \param   None
**
** \retval  None
*********************************************************/
void wdg_deinit(void)
{
    ll_wdg_deinit();
}

/********************************************************
** \brief   wdg_reload
**
** \param   None
**
** \retval  None
*********************************************************/
void wdg_reload(void)
{
    ll_wdg_reload();
}

/********************************************************
** \brief   wdg_enable
**
** \param   bool
**
** \retval  None
*********************************************************/
void wdg_enable(bool enable)
{
    ll_wdg_enable(enable);
}
