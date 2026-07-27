/***************************************************************************//**
 * @file		mod_pwm.c
 *
 * @creator		wca
 * @created		2022.06.23
 *
 * @brief  		Top application layer with main() entry point.
 *
 * @purpose             Hardware and LIN driver intialization, 
 *                      top application layer.
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 * $Id: $
 *
 * $Revision:  $
 *
 ******************************************************************************/

#include "mod_pwm.h"
#include "io_e52139b_IAR.h"

/* ****************************************************************************
 * @implementation
 * Initializes pwm example.
 *
 * @param   none
 * @return  none
 * *************************************************************************** */

#define PWM_PERIOD_CNT              (48000)



void pwm_init(void)
{
  SYS_STATE_CONTROL_bit.pwm_clk_sel = 1;//48MHz
  PWM_CONTROL    = 0x00F0u;

  PWM_PRESCALER0 = 0;
  PWM_PERIOD0    = PWM_PERIOD_CNT;
  PWM_START0     = 0u;
  PWM_STOP0      = 48;

  PWM_PRESCALER1 = 0;
  PWM_PERIOD1    = PWM_PERIOD_CNT;
  PWM_START1     = 0;
  PWM_STOP1      = 500;

  PWM_PRESCALER2 = 0;
  PWM_PERIOD2    = PWM_PERIOD_CNT;
  PWM_START2     = 0;
  PWM_STOP2      = 5000;
  
  PWM_PRESCALER3 = 0;
  PWM_PERIOD3    = PWM_PERIOD_CNT;
  PWM_START3     = 0;
  PWM_STOP3      = 0;

  /* Set pwm currents to 5mA. */
  PWM_DRIVER     = 0;
  /* Enable all pwm ch and ch drivers. */
  PWM_CONTROL    = 0x00FFu; 
}

void pwm_out_ch0(unsigned int val)
{
  PWM_START0     = 0u;
  PWM_STOP0      = val;  
}

void pwm_off(void)
{
    PWM_START0 = 0x0000;
    PWM_STOP0  = 0x0000;
    PWM_START1 = 0x0000;
    PWM_STOP1  = 0x0000;
    PWM_START2 = 0x0000;
    PWM_STOP2  = 0x0000;
    PWM_START3 = 0x0000;
    PWM_STOP3  = 0x0000;
}

