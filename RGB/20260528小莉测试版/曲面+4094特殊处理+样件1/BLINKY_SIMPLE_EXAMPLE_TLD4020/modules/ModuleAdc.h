#ifndef __MOD_ADC_H
#define __MOD_ADC_H

#include <stdint.h>
//#include <el_helper.h>
#include "adc11.h"



/* Keep the old application channel names, but use the TLD4020-4ET ADC11 channel map. */
#define ADC_CH_VS           ADC11_CH_VS
#define ADC_CH_LED0         ADC11_CH_VFWD_LED0
#define ADC_CH_LED1         ADC11_CH_VFWD_LED1
#define ADC_CH_LED2         ADC11_CH_VFWD_LED2
#define ADC_CH_LED3         ADC11_CH_VFWD_LED3
#define ADC_CH_VTEMP        ADC11_CH_TS
#define ADC_PWM_TRIGGER_DELAY_SET(ch,value)   (*(volatile uint16_t *)(&ADC_CTRL_TRIGGER_DELAY0+ch) = (uint16_t)(value))

#define ADC_DMA_COPY_AREA_SIZE        (1)   // one channel at once. 
#define ADC_DMA_IMMEDIATE_AREA_SIZE   (16)  // 16 immediate locations. 
#define ADC_DMA_AREA_SIZE             (ADC_DMA_COPY_AREA_SIZE+ADC_DMA_IMMEDIATE_AREA_SIZE) 







sint8 moduleAdcInit(void);
sint16 ADC11_compensateLinAutoAddrDiff(sint16 s16_adcValue);
uint32_t ReturnADCBuffer(uint32_t channel);

void RED_V(void);
void BLUE_V(void);
void GREEN_V(void);
void WHITE_V(void);
void EOD(void);
void ADC_VS(void);
void ADC_TEMP(void);






#endif                                                                  /* __MOD_ADC_H                   */
