/*
 * adc.c
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#include <plib.h>
#include <lib_adc.h>
#include <atomic.h>
#include <itc_helper.h>
#include "../Modules/ModuleAdc.h"
#include "SystemControl.h"

#define ADCENABLE 1u
#define ADCCLOCKDIVIDER 15u /* 32MHz/(15+1) -> 2MHz */
#define ADCVSDIVIDER 14u
#define GAINDIFF2 4u

/* ADC */
#define VALIDLEDVOLTAGEMEAS 1u
#define NUMBOTHERADCCHANNELS 2u

/* ADC channels * 2 */
/* HSWLC0<>LC0, HSWLC0<>LC1, HSWLC0<>LC2 -> LED1
 * HSWLC1<>LC0, HSWLC1<>LC1, HSWLC1<>LC2 -> LED4
 * HSWLC2<>LC3, HSWLC2<>LC4, HSWLC2<>LC5 -> LED2
 * HSW0<>LC3, HSW0<>LC4, HSW0<>LC5       -> LED5
 * HSW1<>LC6, HSW1<>LC7, HSW1<>LC8       -> LED3
 * HSW2<>LC6, HSW2<>LC7, HSW2<>LC8       -> LED6
 * VS
 * Temperature
 * all channels will be driven via PWM1 Slave compare
 * ADC reference voltage set to 2.5V
 */
const uint16_t ADCCHANNELLIST[40] = { \
    /* HSW0<>LC0 PWM1 compare trigger */
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (1u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  | (1u<<6u)  | (7u<<8u)  | (9u<<11u)), \
    /* HSW1<>LC0 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (1u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW0<>LC1 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (2u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW1<>LC1 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (2u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW0<>LC2 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (3u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW1<>LC2 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (3u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW2<>LC3 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (4u<<10u) | (3u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC0<>LC3 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (4u<<10u) | (0u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW2<>LC4 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (5u<<10u) | (3u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC0<>LC4 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (5u<<10u) | (0u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSW2<>LC5 PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (6u<<10u) | (3u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC0<>LC5 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (6u<<10u) | (0u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC1<>LC6 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (7u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC2<>LC6 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (7u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC1<>LC7 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (8u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC2<>LC7 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (8u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC1<>LC8 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (9u<<10u) | (1u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* HSWLC2<>LC8 PWM1 compare trigger */                                  \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (11u<<2u) | (3u<<7u)  | (9u<<10u) | (2u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (1u<<0u)  | (2u<<1u)  |  (1u<<6u) | (7u<<8u)  | (9u<<11u)), \
    /* VS PWM1 compare trigger */                                         \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (0u<<2u)  | (3u<<7u)  | (0u<<10u) | (0u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (0u<<6u) | (0u<<8u)  | (0u<<11u)), \
    /* Temperature PWM1 compare trigger */                                \
    /* dummy|  adc_ch   | adc ref   | scSig     | scRef_lsb ,          */ \
    (0x0000 | (1u<<2u)  | (3u<<7u)  | (0u<<10u) | (0u<<14u)),             \
    /* dummy|  scRef_msb| HWtrig    | scGain    | scRefDel  | scSetDel */ \
    (0x0000 | (0u<<0u)  | (2u<<1u)  |  (0u<<6u) | (0u<<8u)  | (0u<<11u)), \
};

/* LED order table during ADC measurements -> used for fast access during ADC ISR */
/* LED 1r,4r,1g,4g,1b,4b,2r,5r,2g,5g,2b,5b,3r,6r,3g,6g,3b,6b - channel 18-VS, channel 19-Temp*/
const uint16_t LEDADCINDEX[20] = {0, 9, 1, 10, 2, 11, 3, 12, 4, 13, 5, 14, 6, 15, 7, 16, 8, 17, 18, 19};

/* storage for ADC results -> handled by ADC-DMA */
volatile uint16_t adcResults[2] = {0,0}; /* single channel + CRC */
/* storage for ADC channel list -> handlde by ADC-DMA */
volatile uint16_t adcData[5] = {                                                                              \
    (uint16_t)(&adcResults[0]),(uint16_t)(0x0000 | (1u<<2u) | (3u<<7u) | (0u<<10u) | (0u<<14u)),              \
    (uint16_t)(0x0000 | (0u<<0u) | (2u<<1u) | (0u<<6u) | (0u<<8u) | (0u<<11u)),0x0003,(uint16_t)(&adcData[0]) \
};

/* signalize valid ADC channels -> forward voltage measurement is not always allowed! */
volatile uint8_t ledMeasEna[2][20] = {         \
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1}, \
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1}  \
};  /* enable channel VS and Temp by default */
/* flag to show new PWM duty-cycles for LED forward measurement allowance */
volatile uint16_t ledMeasChgReq = 0;
/* points into valid array for LED forward voltage measurement allowance */
volatile uint16_t ledMeasValBuf = 0;

/* flag to signalize new data from ADC conversion */
volatile uint16_t adcNewData = 0;

void moduleAdcInit(void)
{
  IO_SET(PORT_ADC_CTRL,ADC_EN,ADCENABLE); /* enable ADC */

  /* create ADC init structure */
  Adc_Control_t adc_ctrl = {{ .START=1u,
                              .STOP=0u,
                              .SOS_SOURCE=AdcSOS_FIRST_HARDWARE_TRIGGER,
                              .SOC_SOURCE=AdcSOC_HARDWARE,
                              .NoInterleave=1u,
                              .Saturate=1u,
                              .IntScheme=AdcInt_EOS,
                              .ASB=AdsAsb_NEVER,
                              .AdcWidth=AdcWdt_16Bit}};

  /* initialize ADC - divider 15 -> 32MHz/(15+1) -> 2MHz, sbase and adc ctrl */
  AdcInit(ADCCLOCKDIVIDER, (void*)&adcData[0], adc_ctrl);
  /* start ADC conversion */
  AdcStart();

  /* configure ADC Interrupt*/
  Itc_Clear(ADC_SAR);
  Itc_SetPrio(ADC_SAR, 3u);
  Itc_Enable(ADC_SAR);
}

uint8_t getLedAdcSampleState(uint16_t led_index, ESingleLedChannel led_chanel)
{
	return ledMeasEna[(ledMeasValBuf^1)][(led_index*RGBLEDCHANNELS)+led_chanel];
}

void moduleAdcLedEnable(uint16_t led_index, ESingleLedChannel led_chanel, uint16_t enable)
{
  if (enable == 0)
    ledMeasEna[(ledMeasValBuf^1)][(led_index*RGBLEDCHANNELS)+led_chanel] = 0;
  else
    ledMeasEna[(ledMeasValBuf^1)][(led_index*RGBLEDCHANNELS)+led_chanel] = 1;
}

void moduleAdcUpdate(void)
{
  /* ADC has to be made aware of new PWM duty-cycles -> ATOMIC operation required */
  ENTER_SECTION(ATOMIC_KEEP_MODE);
  ledMeasChgReq = 1; /* signalize to ADC to check for new Data */
  EXIT_SECTION();
}

uint16_t moduleAdcDataReady(void)
{
  if (adcNewData == 0)
    return 0;
  else
    return 1;
}

void moduleAdcDataClear(void)
{
  /* reset flag in atomic operation */
  ENTER_SECTION(ATOMIC_KEEP_MODE);
  adcNewData = 0;
  EXIT_SECTION();
}

volatile uint16_t temperature_Cache = 0;
int16_t moduleAdcGetTemperature(void)
{
  uint16_t result = temperature_Cache;
  return result;
}

volatile uint16_t input_Voltage_Cache = 0;
uint16_t moduleAdcGetVs(void)
{
  uint16_t result = input_Voltage_Cache;
  return result;
}

/* ----------------------------------------------------------------------------
 * convert ADC value into mV for LED forward voltage
 */
static uint16_t calcLEDmVolt(uint16_t adcval){
    uint16_t result = 0;

    /* forward_voltage [mV] = (ADC_Val * Gain(5) * ADC_Ref_Volt(2.5) / ADC_Resolution (1023)) * 1000 (mV)
     * -> 2.5 = 5/2 -> avoids float operations */
    result = adcval * GAINDIFF2;
    result = (uint16_t)((uint32_t)((uint32_t)result * (uint32_t)5000) / (uint32_t)2046);

    return result;
}

volatile uint16_t led_voltage[NUMBOFSINGLELEDS];

void moduleAdcGetLedVoltage(SLEDVoltages *result)
{
  uint16_t cntLeds = 0;
  uint16_t index = 0;

  for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
  {
    for(index = 0; index < RGBLEDCHANNELS; index++)
    {
	  result->LedVoltage[cntLeds].ledChannelData[index] =
		  calcLEDmVolt(led_voltage[cntLeds*RGBLEDCHANNELS + index]);
    }
  }
}

void moduleLedVoltageUpdate(void)
{
	moduleAdcGetLedVoltage(&currentLedVoltage);
}

/* ADC interrupt */
__attribute__((interrupt)) void _ADC_SAR_INT(void) {
    /* handle ADC values and start new conversion */

    /* store actual ADC channel */
    static uint16_t currentADCchannel = 0;

    /* check if all LED channels have been measured */
    if(currentADCchannel>=NUMBOFSINGLELEDS) {
        /* process VS and temperature channels */
        if (currentADCchannel == NUMBOFSINGLELEDS)
          input_Voltage_Cache = adcResults[0];
        else
          temperature_Cache = adcResults[0];

        /* check if all ADC channels have been measured */
        if(currentADCchannel>=(NUMBOFSINGLELEDS+1)) {
            /* reset channel */
            currentADCchannel = 0;
            /* set flag to signalize new data from ADC */
            adcNewData = 1;
        }
        else {
            /* increase ADC channel */
            currentADCchannel++;
        }
    }
    else {
        /* process LED forward voltage channel if valid channel */
        if(ledMeasEna[ledMeasValBuf][LEDADCINDEX[currentADCchannel]] == 1) {
            led_voltage[LEDADCINDEX[currentADCchannel]] = adcResults[0];
        }
        /* increase ADC channel */
        currentADCchannel++;
    }

    /* check if differential measurement of LED is allowed */
    if(ledMeasEna[ledMeasValBuf][LEDADCINDEX[currentADCchannel]] == 1) {
        /* keep channel sequence */
        adcData[1] = ADCCHANNELLIST[(currentADCchannel*2)];
        adcData[2] = ADCCHANNELLIST[(currentADCchannel*2)+1];
    }
    else {
        /* LED measurement not allowed -> set dummy channel -> temperature */
        adcData[1] = ADCCHANNELLIST[38];
        adcData[2] = ADCCHANNELLIST[39];
    }

    /* signalize ADC to be ready for next conversion */
    IO_SET(ADC_SAR, SBASE_0, (uint16_t )(&adcData[0]));
    /* start ADC */
    AdcStart();

    /* check if new PWM duty-cycles are available */
    if(ledMeasChgReq == 1) {
        /* reset flag */
        ledMeasChgReq = 0;
        /* switch buffer to consider new LED forward voltage measurement constraints acc. PWM duty-cycle */
        ledMeasValBuf ^= 1;
    }
}

