/**
 * @cond
 ***********************************************************************************************************************
 *
 * Copyright (c) 2018, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **********************************************************************************************************************/
#ifndef ISR_DEFINES_H
#define ISR_DEFINES_H

/* XML Version 1.2.0 */
#define ISR_XML_VERSION (10200)

#define ADC11_DTS_OVTEMP_CALLBACK place_your_function_call_back_here

#define ADC11_DTS_OVTEMP_INT_EN (0x0u) /*decimal 0*/

#define ADC11_FW_EOC_CALLBACK place_your_function_call_back_here

#define ADC11_FW_EOC_INT_EN (0x0u) /*decimal 0*/

#define CPU_HARDFAULT_CALLBACK place_your_function_call_back_here

#define CPU_HARDFAULT_INT_EN (0x0u) /*decimal 0*/

#define CPU_PENDSV_CALLBACK place_your_function_call_back_here

#define CPU_PENDSV_INT_EN (0x0u) /*decimal 0*/

#define CPU_SVCALL_CALLBACK place_your_function_call_back_here

#define CPU_SVCALL_INT_EN (0x0u) /*decimal 0*/

#define CPU_SYSTICK_CALLBACK place_your_function_call_back_here

#define CPU_SYSTICK_INT_EN (0x0u) /*decimal 0*/

#define GPIO_IEN0_CALLBACK place_your_function_call_back_here

#define GPIO_IEN0_INT_EN (0x0u) /*decimal 0*/

#define GPIO_IEN1_CALLBACK place_your_function_call_back_here

#define GPIO_IEN1_INT_EN (0x0u) /*decimal 0*/

#define LDRV_EOC_CH0_CALLBACK place_your_function_call_back_here

#define LDRV_EOC_CH0_INT_EN (0x0u) /*decimal 0*/

#define LDRV_EOC_CH1_CALLBACK place_your_function_call_back_here

#define LDRV_EOC_CH1_INT_EN (0x0u) /*decimal 0*/

#define LDRV_EOC_CH2_CALLBACK place_your_function_call_back_here

#define LDRV_EOC_CH2_INT_EN (0x0u) /*decimal 0*/

#define LDRV_EOD_CALLBACK place_your_function_call_back_here

#define LDRV_EOD_INT_EN (0x0u) /*decimal 0*/

#define LDRV_OVLF_CALLBACK place_your_function_call_back_here

#define LDRV_OVLF_INT_EN (0x0u) /*decimal 0*/

#define LDRV_PWM_SYNCH_CALLBACK place_your_function_call_back_here

#define LDRV_PWM_SYNCH_INT_EN (0x0u) /*decimal 0*/

#define LDRV_S2G_CH0_CALLBACK place_your_function_call_back_here

#define LDRV_S2G_CH0_INT_EN (0x0u) /*decimal 0*/

#define LDRV_S2G_CH1_CALLBACK place_your_function_call_back_here

#define LDRV_S2G_CH1_INT_EN (0x0u) /*decimal 0*/

#define LDRV_S2G_CH2_CALLBACK place_your_function_call_back_here

#define LDRV_S2G_CH2_INT_EN (0x0u) /*decimal 0*/

#define LDRV_TH_OVLD_CH0_CALLBACK place_your_function_call_back_here

#define LDRV_TH_OVLD_CH0_INT_EN (0x0u) /*decimal 0*/

#define LDRV_TH_OVLD_CH1_CALLBACK place_your_function_call_back_here

#define LDRV_TH_OVLD_CH1_INT_EN (0x0u) /*decimal 0*/

#define LDRV_TH_OVLD_CH2_CALLBACK place_your_function_call_back_here

#define LDRV_TH_OVLD_CH2_INT_EN (0x0u) /*decimal 0*/

#define LIN_AAERR_CALLBACK place_your_function_call_back_here

#define LIN_AAERR_INT_EN (0x0u) /*decimal 0*/

#define LIN_AAMEASRDY_CALLBACK place_your_function_call_back_here

#define LIN_AAMEASRDY_INT_EN (0x0u) /*decimal 0*/

#define LIN_EOF_CALLBACK place_your_function_call_back_here

#define LIN_EOF_INT_EN (0x0u) /*decimal 0*/

#define LIN_ERR_CALLBACK place_your_function_call_back_here

#define LIN_ERR_INT_EN (0x0u) /*decimal 0*/

#define LIN_EXF2_CALLBACK place_your_function_call_back_here

#define LIN_EXF2_INT_EN (0x0u) /*decimal 0*/

#define LIN_OC_CALLBACK place_your_function_call_back_here

#define LIN_OC_INT_EN (0x0u) /*decimal 0*/

#define LIN_OT_CALLBACK place_your_function_call_back_here

#define LIN_OT_INT_EN (0x0u) /*decimal 0*/

#define LIN_RX_CALLBACK place_your_function_call_back_here

#define LIN_RX_INT_EN (0x0u) /*decimal 0*/

#define LIN_TF2_CALLBACK place_your_function_call_back_here

#define LIN_TF2_INT_EN (0x0u) /*decimal 0*/

#define LIN_TMOUT_CALLBACK place_your_function_call_back_here

#define LIN_TMOUT_INT_EN (0x0u) /*decimal 0*/

#define LIN_TX_CALLBACK place_your_function_call_back_here

#define LIN_TX_INT_EN (0x0u) /*decimal 0*/

#define MEM_DSRAM_CALLBACK place_your_function_call_back_here

#define MEM_DSRAM_INT_EN (0x0u) /*decimal 0*/

#define MEM_NVMDBE_CALLBACK place_your_function_call_back_here

#define MEM_NVMDBE_INT_EN (0x0u) /*decimal 0*/

#define PMU_VDD1V5NVM_OV_CALLBACK place_your_function_call_back_here

#define PMU_VDD1V5NVM_OV_INT_EN (0x0u) /*decimal 0*/

#define PMU_VDD1V5_OV_CALLBACK place_your_function_call_back_here

#define PMU_VDD1V5_OV_INT_EN (0x0u) /*decimal 0*/

#define SCU_NVMCLKRATIO_FAIL_CALLBACK place_your_function_call_back_here

#define SCU_NVMCLKRATIO_FAIL_INT_EN (0x0u) /*decimal 0*/

#endif /* ISR_DEFINES_H */
