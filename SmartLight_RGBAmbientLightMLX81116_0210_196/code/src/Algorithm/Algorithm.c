/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    Algorithm.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains algorithms that used to generate PWM
                and get fixed LIN signal.
  *Others:      the intensity adjust parameters may combined to one. Using a 32 bit
                integer instead of two parameters.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/
#include "Algorithm.h"
#include "FloatLib.h"

/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
//static const fix16_t fix16Const25 = F16(2.5);
static const fix16_t fix16Const75 = F16(7.5);
static const fix16_t fix16Const10000 = 10000*0x00010000;
//static const fix16_t fix16Const1000 = 1000*0x00010000;
static const fix16_t fix16ConstM = F16(0.001);
static const fix16_t fix16Const1M = F16(0.0001);

/*This LED normal intensity may too high.*/
/*we use this parameter to adjust it to same range as other chip did.*/
static const fix16_t intensityAdjustConst = F16(1.0);

/*@brief CIE color multiplied by 10000.*/
typedef struct CIExyY
{
 int16_t x,y;
 int32_t Y;
}SCIExyY;

/*@brief CIE color converted to fix16 floating.*/
typedef struct CIEColor
{
 fix16_t x,y;
 fix16_t Y;
}SCIEColor;

/*Original R, G, B colors CIE coordinates.*/
static SCIExyY redxyY[RGBLEDMAX], greenxyY[RGBLEDMAX], bluexyY[RGBLEDMAX];
/*After temperature compensation's  R, G, B colors CIE coordinates.*/
static SCIEColor currentRedCIE[RGBLEDMAX], currentGreenCIE[RGBLEDMAX], currentBlueCIE[RGBLEDMAX];

/**
*@details   Update red led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  red x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  red y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  red Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateRedCIE(uint16_t ledIndex, int16_t x, int16_t y, int32_t Y)
{
  redxyY[ledIndex].x = x;
  redxyY[ledIndex].y = y;
  redxyY[ledIndex].Y = Y/6;
}

/**
*@details   Update green led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  green x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  green y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  green Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateGreenCIE(uint16_t ledIndex, int16_t x, int16_t y, int32_t Y)
{
  greenxyY[ledIndex].x = x;
  greenxyY[ledIndex].y = y;
  greenxyY[ledIndex].Y = Y/6;
}

/**
*@details   Update blue led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  blue x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  blue y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  blue Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateBlueCIE(uint16_t ledIndex, int16_t x, int16_t y, int32_t Y)
{
  bluexyY[ledIndex].x = x;
  bluexyY[ledIndex].y = y;
  bluexyY[ledIndex].Y = Y/6;
}

uint32_t tempDenominator, tempu, tempv, templ;
/*get target CIE color by input LIN and other signals.*/
static void __algoGetTargetColorFromCIE1976(uint16_t ledIndex, SColorParams *inputColor, SCIEColor *targetColor)
{
  tempDenominator  = (uint32_t)inputColor->colorU[ledIndex]*6 + 12000;
  tempDenominator  = tempDenominator - (uint32_t)inputColor->colorV[ledIndex]*16;

  tempu = ((uint32_t)inputColor->colorU[ledIndex]*9)*1000;
  tempv = ((uint32_t)inputColor->colorV[ledIndex]*4)*1000;

  tempu = tempu/tempDenominator;
  tempv = tempv/tempDenominator;

  targetColor->x = fix16_mul(fix16_from_int(tempu), fix16ConstM);
  targetColor->y = fix16_mul(fix16_from_int(tempv), fix16ConstM);

  templ = (uint32_t)inputColor->intensity[ledIndex]*(uint32_t)15625/(uint32_t)4000;
  if (templ < 39)
	  templ = 39;
  //templ = (uint32_t)inputColor->intensity[ledIndex]*(uint32_t)23437/(uint32_t)4000;
  targetColor->Y = fix16_mul(fix16_from_int(templ), fix16Const1M);
  targetColor->Y = fix16_mul(intensityAdjustConst, targetColor->Y);
}

/**@brief structure for temperature compensation.
 * @note more details about this structure, please refer to development document.*/
typedef struct LedTempShift{
 int16_t  Temperature;  /**<Tempetature.*/

 int16_t  redYShift;    /**<Red Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  greenYShift;  /**<Green Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  blueYshift;   /**<Blue Luminous shift of current temperature in percentage(multiplied by 1000).*/

 int16_t  redxShift;    /**<Red x coordinate shift at current temperature.*/
 int16_t  redyShift;    /**<Red y coordinate shift at current temperature.*/

 int16_t  greenxShift;  /**<green x coordinate shift at current temperature.*/
 int16_t  greenyShift;  /**<green y coordinate shift at current temperature.*/

 int16_t  bluexShift;   /**<blue x coordinate shift at current temperature.*/
 int16_t  blueyShift;   /**<blue y coordinate shift at current temperature.*/
}SLedTempShift;

#define DMAX_TEMP_SHIFT_RECORD_AMOUNT 8

//static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
///*0*/{-40 ,1438 ,1067 ,950  ,-74 , 74 ,-101 , 93  , 18 ,-20 },
///*1*/{-20 ,1329 ,1054 ,964  ,-51 , 51 ,-70  , 64  , 12 ,-14 },
///*2*/{-10 ,1279 ,1043 ,977  ,-40 , 40 ,-54  , 50  , 10 ,-11 },
///*3*/{0   ,1186 ,1020 ,955  ,-35 , 38 ,-96  , 9   , 13 ,-15 },
///*4*/{25  ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0   , 0  , 0  },
///*5*/{50  ,825  ,970  ,1035 , 33 ,-29 , 84  ,-13  ,-12 , 14 },
///*6*/{85  ,576  ,921  ,1062 , 62 ,-62 , 204 ,-67  ,-30 , 39 },
///*7*/{120 ,342  ,861  ,1063 , 81 ,-81 , 335 ,-155 ,-52 , 72 }
//};

//static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
///*0*/{-40 ,676 ,547 ,503  ,-3 , 10 ,0 , 86  , 1 ,3 },
///*1*/{-20 ,827 ,726 ,687  ,-4 , 6 ,-1  , 18  , 1 ,0 },
///*2*/{-10 ,889 ,805 ,773  ,-3 , 5 ,-1  , -3  , 1 ,0 },
///*3*/{0   ,936 ,875 ,849  ,0 , 4 ,-1  , -12   , 0 ,0 },
///*4*/{25  ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0   , 0  , 0  },
///*5*/{50  ,931  ,990  ,993 , 0 ,3 , 2  ,33  ,-1 , 1 },
///*6*/{85  ,684  ,856  ,892 , 9 ,-6 , 10 ,133  ,0 , 9 },
///*7*/{120 ,342  ,861  ,1063 , 81 ,-81 , 335 ,-155 ,-52 , 72 }
//};

static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
/*0*/{-40 ,622 ,487 ,454  ,-2 , 10 ,-3 , -4  , 1 ,-4 },
/*1*/{-20 ,797 ,686 ,662  ,-2 , 7 ,-4  , -44  , 0 ,-5 },
/*2*/{-10 ,881 ,788 ,769  ,-2 , 5 ,-3  , -51  , 0 ,-4 },
/*3*/{0   ,944 ,878 ,866  ,4 , 5 ,-3  , -41   , 0 ,-4 },
/*4*/{25  ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0   , 0  , 0  },
/*5*/{50  ,961  ,1041  ,993 , 1 ,-3 , 2  ,26  ,0 , 0 },
/*6*/{85  ,812  ,980  ,892 , 9 ,-3 , 6 ,72  ,-1 , 4 },
/*7*/{120 ,342  ,861  ,1063 , 81 ,-81 , 335 ,-155 ,-52 , 72 }
};

/**
*@details   Do temperature compensate. At last multiply Y by 2.5.
*
*@note more details about this function, please refer to development document.
*
*@param[in] ledTemp  current led junction temperature.
*
*@retval    None.
*/
static void __algoLedFixCIE(uint16_t ledIndex, int16_t ledTemp)
{
  int16_t fixLevel = -1, i;
  int16_t line_p, redp, greenp, bluep;
  /*Temporary use*/
  //SCIExyY redTempxyY, greenTempxyY, blueTempxyY;
  int16_t tempShift;

  for( i = 0 ; i < (DMAX_TEMP_SHIFT_RECORD_AMOUNT-1) ; i++)
  {
    if ((ledTemp >= TtoCIE[i].Temperature) &&
        (ledTemp < TtoCIE[i+1].Temperature))
    {
      fixLevel = i;
      break;
    }
  }

  if (fixLevel == -1)
    i = 0;

  tempShift = (TtoCIE[i+1].Temperature-TtoCIE[i].Temperature);

//fix red color
  line_p = ((TtoCIE[i+1].redYShift - TtoCIE[i].redYShift)<<4)/tempShift;
  redp = TtoCIE[i].redYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
  currentRedCIE[ledIndex].Y = fix16_div(fix16_from_int(redp*redxyY[ledIndex].Y/1000), fix16Const10000);
  currentRedCIE[ledIndex].Y = fix16_mul(currentRedCIE[ledIndex].Y, fix16Const75);

  line_p = ((TtoCIE[i+1].redxShift - TtoCIE[i].redxShift)<<4)/tempShift;
  redp = TtoCIE[i].redxShift + ((line_p * (ledTemp-TtoCIE[i].Temperature))>>4);
  currentRedCIE[ledIndex].x = fix16_div(fix16_from_int(redxyY[ledIndex].x+redp), fix16Const10000);

  line_p = ((TtoCIE[i+1].redyShift - TtoCIE[i].redyShift)<<4)/tempShift;
  redp = TtoCIE[i].redyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
  currentRedCIE[ledIndex].y = fix16_div(fix16_from_int(redxyY[ledIndex].y+redp), fix16Const10000);
//fix red color

//fix green color
  line_p = ((TtoCIE[i+1].greenYShift - TtoCIE[i].greenYShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentGreenCIE[ledIndex].Y = fix16_div(fix16_from_int(greenp*greenxyY[ledIndex].Y/1000), fix16Const10000);
  currentGreenCIE[ledIndex].Y = fix16_mul(currentGreenCIE[ledIndex].Y, fix16Const75);

  line_p = ((TtoCIE[i+1].greenxShift - TtoCIE[i].greenxShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenxShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
  currentGreenCIE[ledIndex].x = fix16_div(fix16_from_int(greenxyY[ledIndex].x+greenp), fix16Const10000);

  line_p = ((TtoCIE[i+1].greenyShift - TtoCIE[i].greenyShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentGreenCIE[ledIndex].y = fix16_div(fix16_from_int(greenxyY[ledIndex].y+greenp), fix16Const10000);
//fix green color

//fix blue color
  line_p = ((TtoCIE[i+1].blueYshift - TtoCIE[i].blueYshift)<<4)/tempShift;
  bluep = TtoCIE[i].blueYshift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE[ledIndex].Y = fix16_div(fix16_from_int(bluep*bluexyY[ledIndex].Y/1000), fix16Const10000);
  currentBlueCIE[ledIndex].Y = fix16_mul(currentBlueCIE[ledIndex].Y, fix16Const75);

  line_p = ((TtoCIE[i+1].bluexShift - TtoCIE[i].bluexShift)<<4)/tempShift;
  bluep = TtoCIE[i].bluexShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE[ledIndex].x = fix16_div(fix16_from_int(bluexyY[ledIndex].x+bluep), fix16Const10000);

  line_p = ((TtoCIE[i+1].blueyShift - TtoCIE[i].blueyShift)<<4)/tempShift;
  bluep = TtoCIE[i].blueyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE[ledIndex].y = fix16_div(fix16_from_int(bluexyY[ledIndex].y+bluep), fix16Const10000);

//fix blue color
}

/**
*@details   Referring to current led attribute calculate PWM duty cycle.
*
*@note more details about this function, please refer to development document.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[in]  MxyzY      Target color in CIE coordinate.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    None.
*/

static void __algoGetDutyRatio(uint16_t ledIndex, SColorParams * const inputColor, SCIEColor *MxyzY, SPWMParams *outputPWM)
{
  fix16_t mix_Y;

  fix16_t a,b,c,d,e,f;
  fix16_t Rm;
  fix16_t Gm;
  fix16_t Bm;

  fix16_t R_all;
  fix16_t G_all;
  fix16_t B_all;

  int32_t duty_R ,duty_G ,duty_B;

  a = fix16_sub(MxyzY->x, currentBlueCIE[ledIndex].x);
  d = fix16_sub(MxyzY->y, currentBlueCIE[ledIndex].y);
  Bm = fix16_div(currentBlueCIE[ledIndex].Y, currentBlueCIE[ledIndex].y);

  b = fix16_sub(currentGreenCIE[ledIndex].y, MxyzY->y);
  c = fix16_sub(currentGreenCIE[ledIndex].x, MxyzY->x);
  Gm = fix16_div(currentGreenCIE[ledIndex].Y, currentGreenCIE[ledIndex].y);

  e = fix16_sub(currentRedCIE[ledIndex].x, MxyzY->x);
  f = fix16_sub(currentRedCIE[ledIndex].y, MxyzY->y);
  Rm = fix16_div(currentRedCIE[ledIndex].Y, currentRedCIE[ledIndex].y);



  R_all = fix16_mul(a, b);
  B_all = fix16_mul(e, b);
  G_all = fix16_mul(d, e);
  R_all = fix16_sub(R_all, fix16_mul(c, d));
  B_all = fix16_sub(B_all, fix16_mul(c, f));
  G_all = fix16_sub(G_all, fix16_mul(f, a));

  fix16_t duty_r_aa ,duty_g_aa ,duty_b_aa;

  duty_r_aa = Bm;
  duty_g_aa = Rm;
  duty_b_aa = Rm;

  duty_r_aa = fix16_mul(duty_r_aa, Gm);
  duty_g_aa = fix16_mul(duty_g_aa, Bm);
  duty_b_aa = fix16_mul(duty_b_aa, Gm);
  duty_r_aa = fix16_mul(duty_r_aa, R_all);
  duty_g_aa = fix16_mul(duty_g_aa, G_all);
  duty_b_aa = fix16_mul(duty_b_aa, B_all);

  /*temperature protection use this(tempAdjust) parameter to control LED intensity.*/
  /*Minimum is 0, maximum is 6400, so we divide 6400.*/
  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int((int32_t)inputColor->tempAdjust[ledIndex]/2));
  /*0.01 = 1/100, mix_Y =  mix_Y /100, for more calculating speed, we convert divide to multiply*/
  mix_Y = fix16_mul(mix_Y, F16(0.02));
  /*0.015625 = 1/64, mix_Y =  mix_Y /64, for more calculating speed, we convert divide to multiply*/
  mix_Y = fix16_mul(mix_Y, F16(0.015625));

  fix16_t temp = fix16_add(fix16_mul(currentRedCIE[ledIndex].Y, duty_r_aa),
                           fix16_mul(currentGreenCIE[ledIndex].Y, duty_g_aa));

  temp = fix16_add(temp, fix16_mul(currentBlueCIE[ledIndex].Y, duty_b_aa));
  temp = fix16_div(mix_Y, temp);

  duty_r_aa = fix16_mul(duty_r_aa, temp);
  duty_r_aa = fix16_mul(duty_r_aa, F16(32767));
  //duty_r_aa = fix16_mul(duty_r_aa, fix16ConstPWM);
  duty_g_aa = fix16_mul(duty_g_aa, temp);
  duty_g_aa = fix16_mul(duty_g_aa, F16(32767));
  //duty_g_aa = fix16_mul(duty_g_aa, fix16ConstPWM);
  duty_b_aa = fix16_mul(duty_b_aa, temp);
  duty_b_aa = fix16_mul(duty_b_aa, F16(32767));
  //duty_b_aa = fix16_mul(duty_b_aa, fix16ConstPWM);

  duty_R = fix16_to_int(duty_r_aa);
  duty_G = fix16_to_int(duty_g_aa);
  duty_B = fix16_to_int(duty_b_aa);

  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_R > 32767)
	  duty_R = 32767;
  else if (duty_R < 0)
    duty_R = 0;
  else
  {;}
  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_G > 32767)
	  duty_G = 32767;
  else if (duty_G < 0)
    duty_G = 0;
  else
  {;}
  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_B > 32767)
	  duty_B = 32767;
  else if (duty_B < 0)
    duty_B = 0;
  else
  {;}

//  duty_R = duty_R<<1;
//  duty_G = duty_G<<1;
//  duty_B = duty_B<<1;

  outputPWM->PWM_R[ledIndex] = (uint16_t)duty_R;
  outputPWM->PWM_G[ledIndex] = (uint16_t)duty_G;
  outputPWM->PWM_B[ledIndex] = (uint16_t)duty_B;
}

/**
*@details   This is a function generates PWM by calling internal functions.
*           The whole process is this: 1. get target color by LIN signals.
*                                      2. adjust current LED attributes by temperature.
*                                      3. generate PWM.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    always true.
*/
bool_t algoGetPWMOutput(SColorParams * const inputColor, SPWMParams *outputPWM)
{
  uint16_t index, VUpLimit,VDownLimit;
  SCIEColor targetColor;
  uint32_t tempValue;

  for (index = 0; index < RGBLEDMAX; index++)
  {
    /*Turn off lights by default.*/
    outputPWM->PWM_R[index] = 0;
    outputPWM->PWM_G[index] = 0;
    outputPWM->PWM_B[index] = 0;
  }

  for (index = 0; index < RGBLEDMAX; index++)
  {
    /*if intensity is 0, then turn off lights.*/
    if (inputColor->intensity[index] == 0)
      continue;

	if ((inputColor->colorU[index] > 529) || (inputColor->colorU[index] < 76))
		continue;

	if ((inputColor->colorV[index] > 565) || (inputColor->colorV[index] < 113))
		continue;

	tempValue = (uint32_t)(inputColor->colorU[index] - 76) * (565 - 517) / (uint32_t)(529 - 76);

	VUpLimit = 565 - (uint16_t)tempValue + 15;

	if (inputColor->colorU[index] <= 203)
	{
		tempValue = (uint32_t)((inputColor->colorU[index] - 76)) * (565 - 114) / (uint32_t)(203 - 76);
		VDownLimit = 565 - (uint16_t)tempValue - 15;
	}
	else
	{
		tempValue = (uint32_t)(inputColor->colorU[index] - 203) * (517 - 114) / (uint32_t)(529 - 203);
		VDownLimit = 114 + (uint16_t)tempValue - 15;
	}

	if ((inputColor->colorV[index] >= VUpLimit) || (inputColor->colorV[index] <= VDownLimit))
		continue;

    /*get target CIE color by input LIN and other signals.*/
    __algoGetTargetColorFromCIE1976(index, inputColor, &targetColor);
    /*Compensate LED by temperature.*/
    __algoLedFixCIE(index, inputColor->ledTemprature[index]/DTEMP_MULTIPLIED_RATIO);
//    __algoLedFixCIE(index, 25);
    /*Referring to target CIE-Color(targetColor), */
    /*some intensity control signals(inputColor)*/
    /*and current LED attributes, generate PWM*/
    __algoGetDutyRatio(index, inputColor, &targetColor, outputPWM);
  }

  return btrue;
}

