/*
 ***********************************************************************************************************************
 *
 * Copyright (c) 2022-2023, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * This software is provided by the copyright holders and contributors "as is" and any express or implied warranties,
 * including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are
 * disclaimed. In no event shall the copyright holder or contributors be liable  for any direct, indirect, incidental,
 * special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or
 * services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability,
 * whether in contract, strict liability,or tort (including negligence or otherwise) arising in any way out  of the
 * use of this software, even if advised of the possibility of such damage.
 *
 **********************************************************************************************************************/

 
 /**
 * \file     adc11.h
 *
 * \brief    Analog to Digital Converter low level access library
 *
 * \version  V1.0.2
 * \date     29. Nov 2023
 *
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** JO           Julia Ott                                                     **
** VO           Vanessa Ongaro                                                **
*******************************************************************************/


/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.2: 2023-11-29, VO:   [ATVCOMETLL-149] Add ADC ConfigSector comp func  **
**                           for LIN shunt current reading                    **
*******************************************************************************/
 
#if !defined(_ADC11_H_)
#define _ADC11_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "tld40xx.h"
#include "adc11_defines.h"


/*******************************************************************************
**                          Global Macro Declarations                         **
*******************************************************************************/
/** \brief Address of ADC11 calibration (ch 0): sector0_page22.ADC_CAL0 */
#define ADC_CAL0_ADDR        (0x11FFFD84)
/** \brief Address of ADC11 calibration (ch 0): sector0_page22.ADC_CAL1 */
#define ADC_CAL1_ADDR        (0x11FFFD88)
/** \brief Address of ADC11 calibration (ch1, ch2, ch3): sector0_page22.ADC_CAL2 */
#define ADC_CAL2_ADDR        (0x11FFFD8C)
/** \brief Address of ADC11 calibration (ch1, ch2, ch3): sector0_page22.ADC_CAL3 */
#define ADC_CAL3_ADDR        (0x11FFFD90)
/** \brief Address of ADC11 calibration (ch4, ch5, ch6): sector0_page22.ADC_CAL4 */
#define ADC_CAL4_ADDR        (0x11FFFD94)
/** \brief Address of ADC11 calibration (ch4, ch5, ch6): sector0_page22.ADC_CAL5 */
#define ADC_CAL5_ADDR        (0x11FFFD98)
/** \brief Address of ADC11 calibration (ch7, ch8): sector0_page22.ADC_CAL6 */
#define ADC_CAL6_ADDR        (0x11FFFD9C)
/** \brief Address of ADC11 calibration (ch7, ch8): sector0_page22.ADC_CAL7 */
#define ADC_CAL7_ADDR        (0x11FFFDA0)
/** \brief Address of ADC11 calibration (ch9): sector0_page22.ADC_CAL8 */
#define ADC_CAL8_ADDR        (0x11FFFDA4)
/** \brief Address of ADC11 calibration (ch9): sector0_page22.ADC_CAL9 */
#define ADC_CAL9_ADDR        (0x11FFFDA8)
/** \brief Address of ADC11 calibration guard for (ch7, ch8): sector0_page22.ADC_CAL6 */
#define ADC_CAL6_7_CHK_ADDR  (0x11FFFD9F)


/*******************************************************************************
**                        Global Function Declarations                        **
*******************************************************************************/
sint8 ADC11_init(void);
sint16 ADC11_compensateLinAutoAddrDiff(sint16 s16_adcValue);

#define ADC_CH_VS           0
#define ADC_CH_LED0         1
#define ADC_CH_LED1         2
#define ADC_CH_LED2         3
//#define ADC_CH_OUT0         4
//#define ADC_CH_OUT1         5
//#define ADC_CH_OUT2         8
//#define ADC_CH_OUT3         9
#define ADC_CH_VTEMP        9
sint8 moduleAdcInit(void);

#ifdef __cplusplus
}
#endif


#endif /*!defined(_ADC11_H_)*/
