#ifndef __MOD_ADC_H
#define __MOD_ADC_H

#include <stdint.h>
#include <el_helper.h>




#define ADC_CH_VDIFF        0     // Value related to DMA storage location 
#define ADC_CH_VS           1
#define ADC_CH_LED0         2
#define ADC_CH_LED1         3
#define ADC_CH_LED2         4
#define ADC_CH_OUT0         6
#define ADC_CH_OUT1         7
#define ADC_CH_OUT2         8
#define ADC_CH_OUT3         9
#define ADC_CH_VTEMP        11
#define ADC_PWM_TRIGGER_DELAY_SET(ch,value)   (*(volatile uint16_t *)(&ADC_CTRL_TRIGGER_DELAY0+ch) = (uint16_t)(value))

#define ADC_DMA_COPY_AREA_SIZE        (1)   // one channel at once. 
#define ADC_DMA_IMMEDIATE_AREA_SIZE   (16)  // 16 immediate locations. 
#define ADC_DMA_AREA_SIZE             (ADC_DMA_COPY_AREA_SIZE+ADC_DMA_IMMEDIATE_AREA_SIZE) 

extern volatile bool_t flagEnableLedDetection;




void moduleAdcInit(void);

uint16_t ReturnADCBuffer(unsigned char Chnl);





#endif                                                                  /* __MOD_ADC_H                   */
