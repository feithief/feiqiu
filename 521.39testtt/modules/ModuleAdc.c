/***************************************************************************//**
 * @file		mod_adc.c
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

#include "ModuleAdc.h"
#include "io_e52139b_IAR.h"



volatile bool_t flagEnableLedDetection;   
   
volatile uint16_t adc_DMAArea[ADC_DMA_AREA_SIZE];


void moduleAdcInit(void)
{
    ADC_CTRL_DMA_BASE_ADDR = (uint16_t)&adc_DMAArea[0];
    ADC_CTRL_SAMPLE_EXT    = 0xEB;
    ADC_CTRL_SAMPLE_EXT_VT = 0x80;
    
    ADC_CTRL_PWM0_CONFIG = 0x0044;
    ADC_CTRL_PWM1_CONFIG = 0x0088;
    ADC_CTRL_PWM2_CONFIG = 0x0110;
    ADC_CTRL_PWM3_CONFIG = 0x0E23;
    
    ADC_CTRL_TRIGGER_DELAY0 = 500;
    ADC_CTRL_TRIGGER_DELAY1 = 500;
    ADC_CTRL_TRIGGER_DELAY2 = 500;
    ADC_CTRL_TRIGGER_DELAY3 = 500;
}

uint16_t ReturnADCBuffer(unsigned char Chnl)
{
  return adc_DMAArea[Chnl]; 
}