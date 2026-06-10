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



static  uint32_t VRED = 600;
static  uint32_t VBLUE = 600;
static  uint32_t VGREEN = 600; 
static  uint32_t VWHITE = 600;
static  uint32_t VBAT = 1000;    


sint8 moduleAdcInit(void)
{
    return (sint8)ADC11_init();
}



sint16 ADC11_compensateLinAutoAddrDiff(sint16 s16_adcValue)
{
    sint32 s32_temp;

    if ((ADC11_CAL->CAL6.bit.LVAA_PCSW_GAIN != 0u) && (ADC11_CAL->CAL6.bit.LVAA_PCSW_CHK != 0u))
    {
        s32_temp = ((sint32)s16_adcValue * (sint32)ADC11_CAL->CAL6.bit.LVAA_PCSW_GAIN) >> 6;
        s32_temp += (sint32)((sint8)ADC11_CAL->CAL7.bit.LVAA_PCSW_OFFS);
        s32_temp = s32_temp >> 1;
    }
    else
    {
        s32_temp = s16_adcValue;
    }

    return (sint16)s32_temp;
}
uint32_t ReturnADCBuffer(uint32_t channel) {



    if ( channel ==  ADC_CH_VS )
    {
			
			return VBAT;
        
//      while (!ADC11->RESULT.bit.ADC_READY);
//        ADC11->CFG_1.bit.ADC_SOC = 0;
//        
//        ADC11->CFG_1.bit.ADC_CHNR = channel;
//        
//        
//    //    while (!(ADC11->RESULT.reg & 0x01000000));  
//        
//      ADC11->CFG_1.bit.ADC_SOC = 1;  
//    //    
//    //    while (!(ADC11->RESULT.reg & 0x00100000));  

////        while (!ADC11->RESULT.bit.ADC_VALID);	//delete

//        
//        if (((ADC11->RESULT.reg >> 12) & 0x1F) == channel)
//        {
//            //return ADC11->RESULT.reg & 0x0FFF;  
//            return ADC11->RESULT.bit.ADC_VALUE;
//        } 
//        else 
//        {
//            
//            return 0xFFFFFFFE;
//        }
    }
    else if ( channel >= ADC_CH_LED0 && channel <= ADC_CH_LED3 )
    {
       if ( channel == ADC_CH_LED0) 
       {
            return VWHITE;
       }

       if ( channel == ADC_CH_LED1) 
       {
            return VGREEN;
       }
       if ( channel == ADC_CH_LED2) 
       {
            return VBLUE;
       }
       if ( channel == ADC_CH_LED3)
       {
            return VRED;
       }

    }
    else if ( channel == ADC_CH_VTEMP)
    {
       /************************origin***********************/
       if (ADC11->TS_RESULT.bit.TS_OVT)
       {
           return 0;
       }
          

//        while (!ADC11->RESULT.bit.ADC_VALID);	//delete
        return ADC11->TS_RESULT.bit.TS_VALUE;
       /***********************************************/ 


//        while (!ADC11->RESULT.bit.ADC_READY);
//        ADC11->CFG_1.bit.ADC_SOC = 0;
//        
//        ADC11->CFG_1.bit.ADC_CHNR = channel;
//        
//        
//    //    while (!(ADC11->RESULT.reg & 0x01000000));  
//        
//      ADC11->CFG_1.bit.ADC_SOC = 1;  
//    //    
//    //    while (!(ADC11->RESULT.reg & 0x00100000));  

////        while (!ADC11->RESULT.bit.ADC_VALID);	//delete

//        
//        if (((ADC11->RESULT.reg >> 12) & 0x1F) == channel)
//        {
//            //return ADC11->RESULT.reg & 0x0FFF;  
//            return ADC11->RESULT.bit.ADC_VALUE;
//        } 
//        else 
//        {
//            
//            return 0xFFFFFFFE;
//        }
    }
    return 0xFFFFFFFE;

}


void RED_V(void)
{
//  if(!LDRV->DIAG_VFWD3.bit.VAL_F)
    VRED = LDRV->DIAG_VFWD3.bit.VFWD;
}       
void BLUE_V(void)
{
//  if(!LDRV->DIAG_VFWD2.bit.VAL_F)
    VBLUE = LDRV->DIAG_VFWD2.bit.VFWD;
}  
void GREEN_V(void)
{
//  if(!LDRV->DIAG_VFWD1.bit.VAL_F)
    VGREEN = LDRV->DIAG_VFWD1.bit.VFWD;
}
void WHITE_V(void)
{
//  if(!LDRV->DIAG_VFWD0.bit.VAL_F)
    VWHITE = LDRV->DIAG_VFWD0.bit.VFWD;
}
void EOD(void)
{
  return;
}   
void ADC_VS(void)
{
  VBAT = ADC11->RESULT.bit.ADC_VALUE;
}   
void ADC_TEMP(void)
{
//  if(!LDRV->DIAG_VFWD2.bit.VAL_F)
    return;
}
