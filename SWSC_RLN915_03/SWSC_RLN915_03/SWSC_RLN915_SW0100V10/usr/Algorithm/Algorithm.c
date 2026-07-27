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
#include "ModuleLed.h"
#include "ModuleTemperature.h"
//#include "api.h"
#include "ModuleAdc.h"
#include "ModuleClock.h" //rain 2021.06.16
#include "ModuleFlash.h"

/*****************/
volatile AlgorithmUpdate   UpdateFlag;


#if 0
/*Red color coordinate for calculating CIE color.*/
static const fix16_t FixedRedx = F16(0.7);
static const fix16_t FixedRedy = F16(0.27);
/*For faster calculating speed this is one divide FixedRedy.*/
static const fix16_t FixedRedyCB = F16(3.7037);
static const fix16_t FixedRedY = F16(1.40);

/*Green color coordinate for calculating CIE color.*/
static const fix16_t FixedGreenx = F16(0.2);
static const fix16_t FixedGreeny = F16(0.75);
/*For faster calculating speed this is one divide FixedGreeny.*/
static const fix16_t FixedGreenyCB = F16(1.3333);
static const fix16_t FixedGreenY = F16(2.40);

/*Blue color coordinate for calculating CIE color.*/
static const fix16_t FixedBluex = F16(0.15);
static const fix16_t FixedBluey = F16(0.03);
/*For faster calculating speed this is one divide FixedBluey.*/
static const fix16_t FixedBlueyCB = F16(33.3333);
static const fix16_t FixedBlueY = F16(0.5);

/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
static const fix16_t fix16Const2_5 = F16(2.5);
static const fix16_t fix16Const10000 = 10000*0x00010000;

/*This LED normal intensity may too high.*/
/*we use this parameter to adjust it to same range as other chip did.*/
static const fix16_t intensityAdjustConst = F16(0.8);//0.4
#endif

/*Red color coordinate for calculating CIE color.*/
static const fix16_t FixedRedx = F16(0.6911);
static const fix16_t FixedRedy = F16(0.3019);
/*For faster calculating speed this is one divide FixedRedy.*/
static const fix16_t FixedRedyCB = F16(3.3124);
static const fix16_t FixedRedY = F16(1.40);

/*Green color coordinate for calculating CIE color.*/
static const fix16_t FixedGreenx = F16(0.2059);
static const fix16_t FixedGreeny = F16(0.6753);
/*For faster calculating speed this is one divide FixedGreeny.*/
static const fix16_t FixedGreenyCB = F16(1.4804);
static const fix16_t FixedGreenY = F16(2.40);

/*Blue color coordinate for calculating CIE color.*/
static const fix16_t FixedBluex = F16(0.1552);
static const fix16_t FixedBluey = F16(0.0572);
/*For faster calculating speed this is one divide FixedBluey.*/
static const fix16_t FixedBlueyCB = F16(17.4825);
static const fix16_t FixedBlueY = F16(0.5);

/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
static const fix16_t fix16Const2_5 = F16(2.5);
static const fix16_t fix16Const10000 = 10000*0x00010000;

/*This LED normal intensity may too high.*/
/*we use this parameter to adjust it to same range as other chip did.*/
static const fix16_t intensityAdjustConst = F16(0.8);//0.4









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
static SCIExyY redxyY, greenxyY, bluexyY;
/*After temperature compensation's  R, G, B colors CIE coordinates.*/
static SCIEColor currentRedCIE, currentGreenCIE, currentBlueCIE;

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
void algoUpdateRedCIE(int16_t x, int16_t y, int32_t Y)
{
  redxyY.x = x;
  redxyY.y = y;
  redxyY.Y = Y;
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
void algoUpdateGreenCIE(int16_t x, int16_t y, int32_t Y)
{
  greenxyY.x = x;
  greenxyY.y = y;
  greenxyY.Y = Y;
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
void algoUpdateBlueCIE(int16_t x, int16_t y, int32_t Y)
{
  bluexyY.x = x;
  bluexyY.y = y;
  bluexyY.Y = Y;
}

/**
*@details   The raw R,G,B signals from LIN bus need to be fixed first. This function fix only R, G, B.
*
*@note      more details about this function, please refer to development document.
*
*@param[in&out] rawColor  The raw LIN signal.
*
*@retval    None.
*/
void algoGetFixedRGB(SColorParams *rawColor)
{
  int32_t max;
  int32_t maxk;

  max = (rawColor->colorR >= rawColor->colorG)?rawColor->colorR:rawColor->colorG;
  max = (max>=rawColor->colorB)?max:rawColor->colorB;

  /*0xff00 = 0xff << 8, multiplied by 256*/
  maxk = 0xff00 / max ;

  rawColor->colorR = (rawColor->colorR * maxk);
  rawColor->colorG = (rawColor->colorG * maxk);
  rawColor->colorB = (rawColor->colorB * maxk);
}

/**
*@details   Generate PWM pulse.
*
*@note more details about this function, please refer to development document.
*
*@param[in] inputColor  Algorithm input parameters.
*
*@param[out] MxyzY  target CIE color.
*
*@retval    None.
*/
static void __algoGetTargetColor(SColorParams * const inputColor, SCIEColor *MxyzY)
{
  fix16_t m1=0,m2=0,m3=0,sum_m=0;
  fix16_t mY1,mY2,mY3 ;
  fix16_t x1m1,x2m2,x3m3,sum_xm;
  fix16_t y1m1,y2m2,y3m3,sum_ym;
  fix16_t standard_Y;

  inputColor->colorR = (inputColor->colorR >> 8);
  inputColor->colorG = (inputColor->colorG >> 8);
  inputColor->colorB = (inputColor->colorB >> 8);

  mY1 = fix16_mul(FixedRedY, fix16_from_int(inputColor->colorR));
  mY2 = fix16_mul(FixedGreenY, fix16_from_int(inputColor->colorG));
  mY3 = fix16_mul(FixedBlueY, fix16_from_int(inputColor->colorB));

  m1 = fix16_mul(mY1, FixedRedyCB);
  m2 = fix16_mul(mY2, FixedGreenyCB);
  m3 = fix16_mul(mY3, FixedBlueyCB);

  sum_m = fix16_add(m1, m2);
  sum_m = fix16_add(sum_m, m3);

  x1m1 = fix16_mul(m1, FixedRedx);
  x2m2 = fix16_mul(m2, FixedGreenx);
  x3m3 = fix16_mul(m3, FixedBluex);

  sum_xm = fix16_add(x1m1, x2m2);
  sum_xm = fix16_add(sum_xm, x3m3);

  y1m1 = fix16_mul(m1, FixedRedy);
  y2m2 = fix16_mul(m2, FixedGreeny);
  y3m3 = fix16_mul(m3, FixedBluey);

  sum_ym = fix16_add(y1m1, y2m2);
  sum_ym = fix16_add(sum_ym, y3m3);

  MxyzY->x = fix16_div(sum_xm, sum_m);
  MxyzY->y = fix16_div(sum_ym, sum_m);

  standard_Y = fix16_add(mY1, mY2);
  standard_Y = fix16_add(standard_Y, mY3);
  /*0.00392 = 1/255, standard_Y = standard_Y/255, for more calculating speed, we convert divide to multiply*/
  standard_Y = fix16_mul(standard_Y, F16(0.00392));
  standard_Y = fix16_mul(standard_Y, intensityAdjustConst);

  MxyzY->Y = fix16_mul(standard_Y, fix16_from_int(inputColor->intensity));
#if 0
  /*0.01 = 1/100,  MxyzY->Y =  MxyzY->Y /100, for more calculating speed, we convert divide to multiply*/
  MxyzY->Y = fix16_mul(MxyzY->Y, F16(0.01));
#else
  /*100<<6 = 6400, intensity/6400 = intensity in percentage.*/
  /*0.01 = 1/100,  MxyzY->Y =  MxyzY->Y /100, for more calculating speed, we convert divide to multiply*/
  MxyzY->Y = fix16_mul( MxyzY->Y, F16(0.01));
  /*0.015625 = 1/64, MxyzY->Y =  MxyzY->Y /64, for more calculating speed, we convert divide to multiply*/
  MxyzY->Y = fix16_mul( MxyzY->Y, F16(0.015625));
#endif

  if(MxyzY->x < 0 ){MxyzY->x = 0;}  else{;}
  if(MxyzY->y < 0 ){MxyzY->y = 0;}  else{;}
  if(MxyzY->Y < 0 ){MxyzY->Y = 0;}  else{;}
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

#if 0//NoC
static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
	/*0*/{-40 ,1000 ,1000 ,1000  ,0 , 0 ,0 , 0  , 0 , 0},
	/*1*/{-20 ,1000 ,1000 ,1000  ,0 , 0 ,0  , 0  , 0 ,0 },
	/*2*/{-10 ,1000 ,1000 ,1000  ,0 , 0 ,0  , 0  , 0 ,0 },
	/*3*/{0   ,1000 ,1000 ,1000  ,0 , 0 ,0  , 0   , 0 ,0 },
	/*4*/{25  ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0 , 0 , 0 },
	/*5*/{50  ,1000  ,1000  ,1000 , 0 ,0 , 0  ,0  ,0 , 0 },
	/*6*/{85  ,1000  ,1000  ,1000 , 0 ,0 , 0 ,0  ,0 , 0 },
	/*7*/{120 ,1000  ,1000 ,1000 , 0 ,0 , 0 ,0 ,0 , 0 }
	  /*0*/{-40 ,1323  ,1128 ,1062  ,-61   ,59   ,-139   ,121  ,19    ,-29   },
	  /*1*/{-20 ,1272  ,1098 ,1041  ,-53   ,45   ,-105   ,91   ,15    ,-22   },
	  /*2*/{-10 ,1237  ,1076 ,1029  ,-31   ,39   ,-87    ,82   ,11    ,-17   },
	  /*3*/{0   ,1145  ,1041 ,1014  ,-25   ,35	 ,-49    ,37   ,8     ,-9    },
	  /*4*/{25  ,1000  ,1000 ,1000  ,0	   ,0	 ,0      ,0    ,0     ,0     },
	  /*5*/{50  ,850   ,928  ,989   ,25	  ,-25   ,41	 ,-48  ,-8    ,10	 },
	  /*6*/{85  ,676   ,860  ,923   ,52	  ,-57   ,104    ,-121  ,-18   ,27	 },
	  /*7*/{120 ,480   ,787  ,900   ,120  ,-82   ,198    ,-179 ,-26   ,42	 }
  /*0*/{-40 ,1323  ,1128 ,1062  ,-61   ,59   ,-139   ,121  ,19    ,-29   },
  /*1*/{-20 ,1272  ,1098 ,1041  ,-53   ,45   ,-105   ,91   ,15    ,-22   },
  /*2*/{-10 ,1237  ,1076 ,1029  ,-31   ,39   ,-87    ,82   ,11    ,-17   },
  /*3*/{0   ,1230  ,1041 ,1014  ,-25   ,35	 ,-49    ,37   ,8     ,-9    },
  /*4*/{25  ,1000  ,1000 ,1000  ,0	   ,0	 ,0      ,0    ,0     ,0     },
  /*5*/{50  ,854   ,953  ,989   ,25	  ,-25   ,41	 ,-48  ,-8    ,10	 },
  /*6*/{85  ,650   ,845  ,908   ,52	  ,-57   ,104   ,-121  ,-18   ,27	 },
  /*7*/{120 ,454   ,787  ,909   ,89	  ,-82   ,198    ,-179 ,-26   ,42	 }
};
#else
static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={


//  /*0*/{-40 ,1413,  1091, 940,  -11,  11,  -18,  6,  3,  -2   },
//  /*1*/{-20 ,1293 ,1058 ,953 ,-8 ,7 ,-13 ,4 ,2 ,-2   },
//  /*2*/{-10 ,1230 ,1045 ,963 ,-6 ,5 ,-11 ,3 ,2 ,-1},
//  /*3*/{0 ,1166 ,1032 ,973 ,-4 ,4 ,-8 ,2 ,1 ,-1},
//  /*4*/{25  ,1000  ,1000 ,1000  ,0     ,0  ,0      ,0    ,0     ,0     },
//  /*5*/{50 ,82 ,964 ,1034 ,4 ,-3 ,9 ,-4 ,-1 ,1},
//  /*6*/{85 ,556 ,913 ,1055 ,8 ,-7 ,21 ,-10 ,-3 ,3},		
//  /*7*/{100  ,447   ,889  ,1064   ,9   ,-9   ,27   ,-14  ,-4   ,4  },
//  /*8*/{120  ,319  ,858  ,1064  ,10  ,-9  ,35  ,-19  ,-5  ,5}
  
  /*0*/{-40 ,1323  ,1128 ,1062  ,-61   ,59   ,-139   ,121  ,19    ,-29   },
  /*1*/{-20 ,1272  ,1098 ,1041  ,-53   ,45   ,-105   ,91   ,15    ,-22   },
  /*2*/{-10 ,1237  ,1076 ,1029  ,-31   ,39   ,-87    ,82   ,11    ,-17   },
  /*3*/{0   ,1202  ,1028 ,1005  ,-37   ,32   ,-68    ,42   ,12     ,-13    },
  /*4*/{25  ,1000  ,1000 ,1000  ,0     ,0  ,0      ,0    ,0     ,0     },
  /*5*/{50  ,726   ,951  ,993   ,31   ,-33   ,74   ,-56  ,-13    ,16   },
  /*6*/{85  ,482   ,864  ,951   ,88   ,-76   ,232   ,-152  ,-37   ,41  },
  /*7*/{120 ,418   ,762  ,890   ,89   ,-82   ,198    ,-179 ,-26   ,42  }
};
#endif

/**
*@details   Do temperature compensate. At last multiply Y by 2.5.
*
*@note more details about this function, please refer to development document.
*
*@param[in] ledTemp  current led junction temperature.
*
*@retval    None.
*/
static void __algoLedFixCIE(int16_t ledTemp)
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
  currentRedCIE.Y = fix16_div(fix16_from_int(redp*redxyY.Y/1000), fix16Const10000);
  currentRedCIE.Y = fix16_mul(currentRedCIE.Y, fix16Const2_5);

  line_p = ((TtoCIE[i+1].redxShift - TtoCIE[i].redxShift)<<4)/tempShift;
  redp = TtoCIE[i].redxShift + ((line_p * (ledTemp-TtoCIE[i].Temperature))>>4);
  currentRedCIE.x = fix16_div(fix16_from_int(redxyY.x+redp), fix16Const10000);

  line_p = ((TtoCIE[i+1].redyShift - TtoCIE[i].redyShift)<<4)/tempShift;
  redp = TtoCIE[i].redyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
  currentRedCIE.y = fix16_div(fix16_from_int(redxyY.y+redp), fix16Const10000);
//fix red color

//fix green color
  line_p = ((TtoCIE[i+1].greenYShift - TtoCIE[i].greenYShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentGreenCIE.Y = fix16_div(fix16_from_int(greenp*greenxyY.Y/1000), fix16Const10000);
  currentGreenCIE.Y = fix16_mul(currentGreenCIE.Y, fix16Const2_5);

  line_p = ((TtoCIE[i+1].greenxShift - TtoCIE[i].greenxShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenxShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
  currentGreenCIE.x = fix16_div(fix16_from_int(greenxyY.x+greenp), fix16Const10000);

  line_p = ((TtoCIE[i+1].greenyShift - TtoCIE[i].greenyShift)<<4)/tempShift;
  greenp = TtoCIE[i].greenyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentGreenCIE.y = fix16_div(fix16_from_int(greenxyY.y+greenp), fix16Const10000);
//fix green color

//fix blue color
  line_p = ((TtoCIE[i+1].blueYshift - TtoCIE[i].blueYshift)<<4)/tempShift;
  bluep = TtoCIE[i].blueYshift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE.Y = fix16_div(fix16_from_int(bluep*bluexyY.Y/1000), fix16Const10000);
  currentBlueCIE.Y = fix16_mul(currentBlueCIE.Y, fix16Const2_5);

  line_p = ((TtoCIE[i+1].bluexShift - TtoCIE[i].bluexShift)<<4)/tempShift;
  bluep = TtoCIE[i].bluexShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE.x = fix16_div(fix16_from_int(bluexyY.x+bluep), fix16Const10000);

  line_p = ((TtoCIE[i+1].blueyShift - TtoCIE[i].blueyShift)<<4)/tempShift;
  bluep = TtoCIE[i].blueyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
  currentBlueCIE.y = fix16_div(fix16_from_int(bluexyY.y+bluep), fix16Const10000);

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
static void __algoGetDutyRatio(SColorParams * const inputColor, SCIEColor *MxyzY, SPWMParams *outputPWM)
{
  fix16_t mix_Y;

  fix16_t a,b,c,d,e,f;
  fix16_t Rm;
  fix16_t Gm;
  fix16_t Bm;

  a = fix16_sub(MxyzY->x, currentBlueCIE.x);
  d = fix16_sub(MxyzY->y, currentBlueCIE.y);
  Bm = fix16_div(currentBlueCIE.Y, currentBlueCIE.y);

  b = fix16_sub(currentGreenCIE.y, MxyzY->y);
  c = fix16_sub(currentGreenCIE.x, MxyzY->x);
  Gm = fix16_div(currentGreenCIE.Y, currentGreenCIE.y);

  e = fix16_sub(currentRedCIE.x, MxyzY->x);
  f = fix16_sub(currentRedCIE.y, MxyzY->y);
  Rm = fix16_div(currentRedCIE.Y, currentRedCIE.y);

  fix16_t R_all;
  fix16_t G_all;
  fix16_t B_all;

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
  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int(inputColor->tempAdjust));
  /*0.01 = 1/100, mix_Y =  mix_Y /100, for more calculating speed, we convert divide to multiply*/
  mix_Y = fix16_mul(mix_Y, F16(0.01));
  /*0.015625 = 1/64, mix_Y =  mix_Y /64, for more calculating speed, we convert divide to multiply*/
  mix_Y = fix16_mul(mix_Y, F16(0.015625));

  fix16_t temp = fix16_add(fix16_mul(currentRedCIE.Y, duty_r_aa),
                           fix16_mul(currentGreenCIE.Y, duty_g_aa));

  temp = fix16_add(temp, fix16_mul(currentBlueCIE.Y, duty_b_aa));
  temp = fix16_div(mix_Y, temp);


#if 0 
  duty_r_aa = fix16_mul(duty_r_aa, temp);
  duty_r_aa = fix16_mul(duty_r_aa, F16(PWM_PERIOD));
  duty_g_aa = fix16_mul(duty_g_aa, temp);
  duty_g_aa = fix16_mul(duty_g_aa, F16(PWM_PERIOD));
  duty_b_aa = fix16_mul(duty_b_aa, temp);
  duty_b_aa = fix16_mul(duty_b_aa, F16(PWM_PERIOD));

  int32_t duty_R ,duty_G ,duty_B;

  duty_R = fix16_to_int(duty_r_aa);
  duty_G = fix16_to_int(duty_g_aa);
  duty_B = fix16_to_int(duty_b_aa);
#else
  float f_temp,f_duty_r_aa,f_duty_g_aa,f_duty_b_aa;
  int32_t duty_R ,duty_G ,duty_B;
  f_temp = F16ToFloat(temp);

  f_duty_r_aa = F16ToFloat(duty_r_aa);

  f_duty_g_aa = F16ToFloat(duty_g_aa);

  f_duty_b_aa = F16ToFloat(duty_b_aa);
  
  f_duty_r_aa = f_duty_r_aa * f_temp;
  f_duty_r_aa = f_duty_r_aa * PWM_PERIOD;
  
  f_duty_g_aa = f_duty_g_aa * f_temp;
  f_duty_g_aa = f_duty_g_aa * PWM_PERIOD;
  
  f_duty_b_aa = f_duty_b_aa * f_temp;
  f_duty_b_aa = f_duty_b_aa * PWM_PERIOD;
  
  
  duty_R = (int)f_duty_r_aa;
  duty_G = (int)f_duty_g_aa;
  duty_B = (int)f_duty_b_aa;
#endif

  /*factor is a parameter our customer using to adjust intensity.*/
  duty_R = ((duty_R*(inputColor->factor>>7))>>8);
  duty_G = ((duty_G*(inputColor->factor>>7))>>8);
  duty_B = ((duty_B*(inputColor->factor>>7))>>8);

  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_R > (int32_t)PWM_VALUE_MAX)
    duty_R = PWM_VALUE_MAX-1;
  else if (duty_R < 0)
    duty_R = 0;
  else
  {;}
  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_G > (int32_t)PWM_VALUE_MAX)
    duty_G = PWM_VALUE_MAX-1;
  else if (duty_G < 0)
    duty_G = 0;
  else
  {;}
  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
  if (duty_B > (int32_t)PWM_VALUE_MAX)
    duty_B = PWM_VALUE_MAX-1;
  else if (duty_B < 0)
    duty_B = 0;
  else
  {;}

  outputPWM->PWM_R = (uint16_t)duty_R;
  outputPWM->PWM_G = (uint16_t)duty_G;
  outputPWM->PWM_B = (uint16_t)duty_B;
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
  SCIEColor targetColor;

  /*if intensity is 0, then turn off lights.*/
  if (inputColor->intensity == 0) 
  {
    outputPWM->PWM_R = 0;
    outputPWM->PWM_G = 0;
    outputPWM->PWM_B = 0;
    return btrue;
  }
  /*Invalid parameters, turn off lights.*/
  if ((inputColor->colorR ==0) &&
      (inputColor->colorG ==0) &&
      (inputColor->colorB ==0))
  {
    outputPWM->PWM_R = 0;
    outputPWM->PWM_G = 0;
    outputPWM->PWM_B = 0;
    return btrue;
  }
  /*get target CIE color by input LIN and other signals.*/
  __algoGetTargetColor(inputColor, &targetColor);
  /*Compensate LED by temperature.*/
  __algoLedFixCIE(inputColor->ledTemprature/DTEMP_MULTIPLIED_RATIO);
  /*Referring to target CIE-Color(targetColor), */
  /*some intensity control signals(inputColor)*/
  /*and current LED attributes, generate PWM*/
  __algoGetDutyRatio(inputColor, &targetColor, outputPWM);

  return btrue;
}
