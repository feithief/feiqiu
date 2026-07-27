/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */
#ifndef _MODULEADC_H_
#define _MODULEADC_H_
#include "SystemType.h"


/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */

typedef enum{
  ADC_CH_Red = 0,
  ADC_CH_Green,
  ADC_CH_Blue,
  ADC_CH_VS,
  ADC_CH_VTEMP,
}Adc_channel;

typedef struct{
  int32_t coefficient;
  int32_t offset;
}coeffParam_t;

#define ADC_INVALID_VALUE   0xffffU

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*(1UL << MEASURE_GAIN_POS)*/
/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */
/**@brief This flag can control the LED ADC convert process. Because
 *        LED ADC converts in interrupt.
 *It's dangerous to make this external, only for smaller code size.*/
extern volatile bool_t flagEnableLedDetection;

extern coeffParam_t adc_coeff[5];
/*******************************************************************************
*@details   Initialize GPIO  module.
*
*@retval    None.
*******************************************************************************/

void moduleAdcInit(void);

void Adc_LED_trigger_delay(void);

uint16_t moduleADCValue(Adc_channel Chnl);
#endif
