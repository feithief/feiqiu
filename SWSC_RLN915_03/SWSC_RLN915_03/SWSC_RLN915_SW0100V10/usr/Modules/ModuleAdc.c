/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    ModuleAdc.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains basic ADC operations.
  *Others:      None.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#include "ModuleAdc.h"
#include "ModuleWatchdog.h"
#include "SystemType.h"
#include "ModuleLed.h"
//#include "api.h"

#include "adc_device.h"
#include "timer_device.h"

#include "gpio_device.h"


#define ADC_DMA_AREA_SIZE             (3+1+1)  //R,G,B,Vcc,Temperature

volatile uint16_t adc_DMAArea[ADC_DMA_AREA_SIZE];
Adc_channel Adc_channel_sel;

//#define ADC_MUX_DELAY       82  // adc timing  24MHZ           // 8us (sufficiently less than POST_SHIFT_DELAY, to complete sampling bevore next cycle starts)
                                            // since this example does not switch the MUX, value may be less then 10us according to spec.  

//#define ADC_SAMPLE_EXT      163 // adc timing  12MHZ             
//#define ADC_SAMPLE_EXT_VT   48              // default value


/* ============================================================================ */
/* -------------------------------Function Definition-------------------------- */
/* ============================================================================ */
/*it is one of a series of callback functions which defined below*/
void adcLedR(int sno)
{
  if (sno == ADC_CH_Red)
    moduleLedDetectionDone(ELedChannelRed);
}

void adcLedG(int sno)
{
  if (sno == ADC_CH_Green)
    moduleLedDetectionDone(ELedChannelGreen);
}

void adcLedB(int sno)
{
  if (sno == ADC_CH_Blue)
    moduleLedDetectionDone(ELedChannelBlue);
}


uint8_t measure_sel = 0;
void ADC_DoneISR(ADCMeasureParam_t param, uint16_t *const result)
{
  if(param.item == ADC_MEASURE_ALL_LED)
  {
    adc_DMAArea[ADC_CH_Red] = (result[0]>=0x800U)? 0U: result[0];
    adc_DMAArea[ADC_CH_Green] = (result[1]>=0x800U)? 0U: result[1];
    adc_DMAArea[ADC_CH_Blue] = (result[2]>=0x800U)? 0U: result[2];
    
    if(measure_sel == 0)
      adc_DMAArea[ADC_CH_VS]= (result[3]>=0x800U)? 0U: result[3];
    else
      adc_DMAArea[ADC_CH_VTEMP]= (result[3]>=0x800U)? 0U: result[3];
      
    measure_sel ^= 0x01;                                                 //VS和温度交替测量
    
    moduleLedDetectionDone(ELedChannelRed);
    moduleLedDetectionDone(ELedChannelGreen);
    moduleLedDetectionDone(ELedChannelBlue);
  }
  else
  {
    switch(Adc_channel_sel)
    {
    case ADC_CH_Red:
      adc_DMAArea[ADC_CH_Red] = (result[0]>=0x800U)? 0U: result[0];
      moduleLedDetectionDone(ELedChannelRed);
      break;
    case ADC_CH_Green:
      adc_DMAArea[ADC_CH_Green] = (result[0]>=0x800U)? 0U: result[0];
      moduleLedDetectionDone(ELedChannelGreen);
      break;
    case ADC_CH_Blue:
      adc_DMAArea[ADC_CH_Blue] = (result[0]>=0x800U)? 0U: result[0];
      moduleLedDetectionDone(ELedChannelBlue);
      break;
    case ADC_CH_VS:                                                     //VS和Temp一起测量                  
      adc_DMAArea[ADC_CH_VS] = (result[0]>=0x800U)? 0U: result[0];
      adc_DMAArea[ADC_CH_VTEMP] = (result[1]>=0x800U)? 0U: result[1];
      break;
    default:
      break;
    }
    Adc_channel_sel = (Adc_channel)(Adc_channel_sel >= ADC_CH_VS ? 0 : (Adc_channel_sel + 1));
  }
//   GPIO_Set(GPIO_PORT_2, GPIO_LOW);
}




/* 计时器满2ms 触发adc */
void Timer_adcISR(void)
{
//  ADC_Init(ADC_MEASURE_ALL_LED, 0U, (LedType_t)0U, ADC_TRIGGER_SOURCE_SOFT_INPUT);
  
  switch(Adc_channel_sel)
  {
  case ADC_CH_Red:
    ADC_Init(ADC_MEASURE_ITEM_LED, 0U, LED_RED, ADC_TRIGGER_SOURCE_SOFT_INPUT);
    break;
  case ADC_CH_Green:
    ADC_Init(ADC_MEASURE_ITEM_LED, 0U, LED_GREEN, ADC_TRIGGER_SOURCE_SOFT_INPUT);
    break;
  case ADC_CH_Blue:
    ADC_Init(ADC_MEASURE_ITEM_LED, 0U, LED_BLUE, ADC_TRIGGER_SOURCE_SOFT_INPUT);
    break;
  case ADC_CH_VS:                                                       //VS和Temp一起测量    
    ADC_Init(ADC_MEASURE_ITEM_VBAT_VTEMP, 0U, (LedType_t)0U, ADC_TRIGGER_SOURCE_SOFT_INPUT);
    break;
  default:
    break;
  }
  ADC_Start();
  
//  GPIO_Set(GPIO_PORT_2, GPIO_HIGH);

   
  
}



coeffParam_t adc_coeff[5];


void MeasureParamInit(void)
{
    /* led parameters init */
    uint16_t codeH,codeL;
    uint16_t codeHVolt,codeLVolt;
    
    codeH = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN4P0V_CODE;
    codeL = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN1P5V_CODE;
    codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN4P0V_VOLT;
    codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_R].LED_PN1P5V_VOLT;
    
    adc_coeff[ADC_CH_Red].coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
    adc_coeff[ADC_CH_Red].offset = (int32_t)codeLVolt*MEASURE_GAIN - adc_coeff[ADC_CH_Red].coefficient*(int32_t)codeL;
    
    codeH = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN4P0V_CODE;
    codeL = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN1P5V_CODE;
    codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN4P0V_VOLT;
    codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_G].LED_PN1P5V_VOLT;
    
    adc_coeff[ADC_CH_Green].coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
    adc_coeff[ADC_CH_Green].offset = (int32_t)codeLVolt*MEASURE_GAIN - adc_coeff[ADC_CH_Green].coefficient*(int32_t)codeL;
    
    codeH = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN4P0V_CODE;
    codeL = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN1P5V_CODE;
    
    codeHVolt = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN4P0V_VOLT;
    codeLVolt = HWCFG_SFRS->LED[PHY_CHANNEL_B].LED_PN1P5V_VOLT;
    
    adc_coeff[ADC_CH_Blue].coefficient = ((int32_t)codeHVolt - (int32_t)codeLVolt)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
    adc_coeff[ADC_CH_Blue].offset = (int32_t)codeLVolt*MEASURE_GAIN - adc_coeff[ADC_CH_Blue].coefficient*(int32_t)codeL;
    
     /* battery volt*/
    uint16_t batt13V5 = HWCFG_SFRS->VBAT_CODE_13V5;
    uint16_t batt8V0 = HWCFG_SFRS->VBAT_CODE_8V0;
    adc_coeff[ADC_CH_VS].coefficient = (int32_t)(13500 - 8000)*MEASURE_GAIN/((int32_t)batt13V5 - (int32_t)batt8V0);
    adc_coeff[ADC_CH_VS].offset      = (int32_t)(8000*MEASURE_GAIN) - adc_coeff[ADC_CH_VS].coefficient*(int32_t)batt8V0;
    
    /* temperature*/
    uint16_t adc0V5 = HWCFG_SFRS->ADC_0V5_CODE;
    uint16_t adc1V0 = HWCFG_SFRS->ADC_1V0_CODE;
    adc_coeff[ADC_CH_VTEMP].coefficient = (1000 -500)*MEASURE_GAIN/((int32_t)adc1V0 - (int32_t)adc0V5);
    adc_coeff[ADC_CH_VTEMP].offset =  500*MEASURE_GAIN - adc_coeff[ADC_CH_VTEMP].coefficient*(int32_t)adc0V5;
}


/**
*@details   Initialize ADC module.
*
*@retval    None.
*/
void moduleAdcInit(void)
{
    MeasureParamInit();                 /* 初始化ADC多项式系数*/
    ADC_GeneralInit(); 
    ADC_RegisterIRQ(ADC_DoneISR);
}



//static uint32_t DebugCNT1 = 0;
/* 在LED.c中被定义， 是PWM中断的回调函数 */
void Adc_LED_trigger_delay(void)
{
  //通过标志位判断是否需要启动ADC
    TIMER_Init(TIMER1, HW_TIMER_ONE_SHOT_MODE, MAIN_CPU_CLOCK * 2000 , Timer_adcISR); 
    
//  if(DebugCNT1++ % 2 == 0)
//  {
//    GPIO_Set(GPIO_PORT_1, GPIO_HIGH);
//  }
//  else
//  {
//    GPIO_Set(GPIO_PORT_1, GPIO_LOW);
//  }
}





uint16_t moduleADCValue(Adc_channel Chnl)
{
  uint16_t data = adc_DMAArea[Chnl];
  return data; 
}