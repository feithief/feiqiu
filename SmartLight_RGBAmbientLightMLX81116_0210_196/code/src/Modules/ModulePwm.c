#include "../Modules/ModulePwm.h"

/*
 * ledpwm.c
 *
 *  Created on: 2019Äê5ÔÂ19ÈÕ
 *      Author: SH1705016
 */

#include <plib.h>
#include <lib_pwm.h>
#include <atomic.h>
#include "../Modules/ModuleAdc.h"
#include "../Modules/ModuleFlash.h"

#define LCCURRENT30MA 5u    /* 30mA */
#define LCCURRENT40MA 7u    /* 40mA */
#define LCCURRENT50MA 13u   /* 50mA */
#define LCCURRENT60MA 15u   /* 60mA */

#define MAXLCDIAGCURRENT 7u /* 1mA */
#define MINLCDIAGCURRENT 0u /* 125uA */

/* disable PWM on HSWLC0,HSWLC1 and HSWLC2 */
#define DISAPWM_HSWLC0_HSWLC1_HSWLC2 0x0E00u

#define SWITCHON_HSWLC0_2_HSW1 0x002Au
#define SWITCHON_HSWLC1_HSW0_2 0x0015u

#define PORTADDRESSPWM1MASTERHT IO_ADDRESS(PWM_MASTER1, HT)

#define PORTADDRESSPWMHTGAP 0x0005u

/* PWM defines */

#define HSWLATENCY 0x40u
#define PWM_CHANNELS 9u
/* 320 * 1/32MHz => 10us */
//#define T_MINON 320u
#define T_MINON 1u

/* double buffer for pwm duty-cycle */
volatile uint16_t pwmDuty[2][18] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
/* points to actual PWM duty-cycle buffer array */
volatile uint16_t validPmwDuty = 0;
/* flag which points into valid PWM duty-cycle buffer */
volatile uint16_t pwmstat = 0;

//static SLEDPwmSet currentPwm;

void modulePwmInit(void)
{
  /* configure diagnostic current source */
  IO_SET(PORT_LC_DISPU_DIAG,LCDIAG_SEL,MAXLCDIAGCURRENT); /* max current */

  /* configure current sources LC0-LC8 to 60mA */
  IO_SET(PORT_LC_SEL1,LC0_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL1,LC1_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL1,LC2_SEL,LCCURRENT50MA);
  IO_SET(PORT_LC_SEL1,LC3_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL2,LC4_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL2,LC5_SEL,LCCURRENT50MA);
  IO_SET(PORT_LC_SEL2,LC6_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL2,LC7_SEL,LCCURRENT40MA);
  IO_SET(PORT_LC_SEL3,LC8_SEL,LCCURRENT50MA);

  IO_SET(PORT_SELP_LC,SELP_LC,DISAPWM_HSWLC0_HSWLC1_HSWLC2); /* disable PWM on HSWLC0,HSWLC1 and HSWLC2 */

  IO_SET(PORT_LC_DISPU_DIAG,LC_DISPU,0x0FFF); /* disable pull-up resistors on LCx and HSWLCx */

  /* configure PWM outputs for LC0-8 */
  /* PWM1 slave compare 2us delay (HSW switching) after Master CMP ISR for ADC HW-Trigger */
  PWM1_INIT(1,0,0x7FFF,0x0000,(PWMMAXPER+HSWLATENCY),PWM_MIRROR_MODE,0,0);
  PWM2_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);

  PWM3_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
  PWM4_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
  PWM5_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
  PWM6_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
  PWM7_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
  PWM8_INIT(1,0,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);

  /* min. 10us delay for PWM interrupt - set to 16us with tolerance -> used for HSW switching */
  PWM1_MASTER_INIT(1,0,0x7FFF,0x4000,0x0000,0x7E00,PWM_MIRROR_MODE,0,0);

//  PWM1_INIT(1,1,0x7FFF,0x0000,(PWMMAXPER+HSWLATENCY),PWM_MIRROR_MODE,0,0);
//  PWM2_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//
//  PWM3_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//  PWM4_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//  PWM5_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//  PWM6_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//  PWM7_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//  PWM8_INIT(1,1,0x4000,0x0000,0,PWM_MIRROR_MODE,0,0);
//
//    /* min. 10us delay for PWM interrupt - set to 16us with tolerance -> used for HSW switching */
//  PWM1_MASTER_INIT(1,1,0x7FFF,0x4000,0x0000,PWMMAXPER,PWM_MIRROR_MODE,0,0);

  PWM1_MASTER_ENABLE();
  PWM1_ENABLE();

  PWM2_ENABLE();
  PWM3_ENABLE();
  PWM4_ENABLE();
  PWM5_ENABLE();
  PWM6_ENABLE();
  PWM7_ENABLE();
  PWM8_ENABLE();

  /* configure PWM Interrupts*/
  Itc_Clear(PWM_MASTER1_CMP);
  Itc_SetPrio(PWM_MASTER1_CMP, 3u);
  Itc_Enable(PWM_MASTER1_CMP);
}

static volatile uint8_t ledMask = 0;
void modulePwmLedSpecialMask(uint8_t mask)
{
  ledMask = mask;
}

//static const uint16_t LEDCHANNELMAP[RGBLEDMAX*RGBLEDCHANNELS] = {
//    1, 0, 2,/*0*/
//    10, 9, 11,/*1*/
//    4, 3, 5,/*2*/
//    13, 12, 14,/*3*/
//    7, 6, 8,/*4*/
//    16, 15, 17,/*5*/
//};

const uint16_t LEDCHANNELMAP[RGBLEDMAX*RGBLEDCHANNELS] = {
    1, 0, 2,/*0*/
    10, 9, 11,/*1*/
    4, 3, 5,/*2*/
    13, 12, 14,/*3*/
    7, 6, 8,/*4*/
    16, 15, 17,/*5*/
};

SLEDPwmSet pwmParam;
void modulePwmSet(SPWMParams *ouputPwm)
{
  uint16_t cntLeds = 0;
  uint16_t index = 0;
  uint16_t temp = 0;
//  SLEDPwmSet pwmParam;

  if ((ledMask & 0xFC) == 0x0)
  {
    for (index = 0; index < RGBLEDMAX; index++)
    {
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]]
                        = (ouputPwm->PWM_R[index] == 0)?0:(ouputPwm->PWM_R[index]+savedConfig.RCapCompensate[index]);
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]]
                        = (ouputPwm->PWM_G[index] == 0)?0:(ouputPwm->PWM_G[index]+savedConfig.GCapCompensate[index]);
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]]
                        = (ouputPwm->PWM_B[index] == 0)?0:(ouputPwm->PWM_B[index]+savedConfig.BCapCompensate[index]);

        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]] =
        		pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]]>32767?32767:pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]];
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]] =
        		pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]]>32767?32767:pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]];
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]] =
        		pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]]>32767?32767:pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]];
    }
  }
  else
  {
    for (index = 0; index < RGBLEDMAX; index++)
    {
      if (ledMask&(0x1<<(index+2)))
      {
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]]
                        = ((ledMask&0x3)==0x00)?32000:0;
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]]
                        = ((ledMask&0x3)==0x01)?32000:0;
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]]
                        = ((ledMask&0x3)==0x02)?32000:0;
      }
      else
      {
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 0]]
                        = 0;
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 1]]
                        = 0;
        pwmParam.LedRGB[LEDCHANNELMAP[index*RGBLEDCHANNELS + 2]]
                        = 0;
      }
    }
  }

  for(cntLeds = 0; cntLeds < RGBLEDMAX; cntLeds++)
  {
    /* scale and set PWM duty-cycle for single RGB-LED */
    for(index = 0; index < RGBLEDCHANNELS; index++)
    {
      temp = pwmParam.LedRGB[cntLeds*RGBLEDCHANNELS + index];

//      if(index == 0)
//	  {
//		  if(temp < 36)//11468
//		  {
//			  temp = (temp == 0)?0:(temp + 10);//Green
//		  }
//	  }
//	  else if(index == 1)
//	  {
//		  if(temp < 25)//11468
//		  {
//			  temp = (temp == 0)?0:(temp + 6);//Red
//		  }
//	  }
//	  else if(index == 2)
//	  {
//		  if(temp < 18)//11468
//		  {
//			  temp = (temp == 0)?0:(temp + 6);//Blue
//		  }
//	  }
      /* scale to PWMMAXPER period and round */

      pwmDuty[(validPmwDuty^1)][(cntLeds*RGBLEDCHANNELS + index)] = temp;

      /* check if LED forward voltage can be measured */
      if (temp >= T_MINON) {
          /* signalize that channel can be measured */
          moduleAdcLedEnable(cntLeds, index, 1);
      }
      else {
          /* signalize that channel cannot be measured */
          moduleAdcLedEnable(cntLeds, index, 0);
      }
    }
//	  temp = 3500;
//	  pwmDuty[(validPmwDuty^1)][(cntLeds*RGBLEDCHANNELS + 0)] = temp;
//	  moduleAdcLedEnable(cntLeds, 0, 1);
//	  temp = 5200;
//	  pwmDuty[(validPmwDuty^1)][(cntLeds*RGBLEDCHANNELS + 1)] = temp;
//	  moduleAdcLedEnable(cntLeds, 1, 1);
//	  temp = 2200;
//	  pwmDuty[(validPmwDuty^1)][(cntLeds*RGBLEDCHANNELS + 2)] = temp;
//	  moduleAdcLedEnable(cntLeds, 2, 1);
  }

  /* signalize new PWM duty-cycle values are available */
  validPmwDuty ^= 1;
  moduleAdcUpdate();
}

//static uint16_t POWERCHANNELMAP[2] = {
//    0x0015,/*HS0_HS2_LC1*/
//    0x002A,/*HS1_LC0_LC2*/
//};

/* PWM1 Master compare interrupt */
__attribute__((interrupt)) void  _PWM_MASTER1_CMP_INT(void){
    /* set new PWM duty-cycle and set corresponding HSW switches */

    /* set pointer to PWM1 Master HT to set new PWM duty-cycle*/
    uint16_t* ptrIO = (uint16_t*)PORTADDRESSPWM1MASTERHT;
    /* loop variable */
    uint16_t temp = 0;

    /* check current PWM state -> currently driven RGB-LEDs */
    if(pwmstat == 0) {
        /* set new high side switches */
//        IO_SET(PORT_HSW_OUT,HSW_OUT,SWITCHON_HSWLC1_HSW0_2); /* HS1_LC0_LC2 */
        IO_SET(PORT_HSW_OUT,HSW_OUT,0x0015); /* HS1_LC0_LC2 */

        /* set new PWM duty-cycles for next 3 RGB-LEDs */
        for(temp = 0; temp < PWM_CHANNELS; temp++) {
//            ptrIO[(temp*PORTADDRESSPWMHTGAP)] = 0;
            ptrIO[((temp*PORTADDRESSPWMHTGAP)+1)] = (0x7FFF - pwmDuty[validPmwDuty][temp])/2;
        }
    }
    else {
        /* set new high side switches */
//        IO_SET(PORT_HSW_OUT,HSW_OUT,SWITCHON_HSWLC0_2_HSW1); /* HS0_HS2_LC1 */
        IO_SET(PORT_HSW_OUT,HSW_OUT,0x002A); /* HS0_HS2_LC1 */
//
//        /* set new PWM duty-cycles for next 3 RGB-LEDs */
        for(temp = 0; temp < PWM_CHANNELS; temp++) {
//            ptrIO[(temp*PORTADDRESSPWMHTGAP)] = 0;
            ptrIO[((temp*PORTADDRESSPWMHTGAP)+1)] = (0x7FFF - pwmDuty[validPmwDuty][temp+PWM_CHANNELS])/2;
        }
    }

    /* signalize that driven RGB-LEDs has changed */
    pwmstat ^= 1;
}
