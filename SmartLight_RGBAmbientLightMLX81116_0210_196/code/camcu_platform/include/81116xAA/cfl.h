/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  cfl.h
 * \date      04.11.2015
 * \brief     Master header file of Color Functional Library. Token: cfl
 * \author    DX.cba
 **************************************************************************************/

/*!
   \page cfl Color Functional Library
   The <b>C</b>olor <b>F</b>unctional <b>L</b>ibrary (cfl) provides color functional operations for Mlx811xy.
   \image latex ColorFnMath_Structure_Overview.pdf "Color Functional Library overview " width=1.0\textwidth
   The cfl description chapter is divided into four subchapters:
   \li \subpage  cflTypDesc
   \li \subpage  cflFncDesc
   \li \subpage  cflMacDesc
   \li \subpage  cflConsDesc

   \page cflTypDesc Typedef Description
   This chapter describes following typedefs which are defined in the cfl:
   \li <tt> \subpage  cfltFading </tt>
   \li <tt> \subpage  cfltFadingParamsColor </tt>
   \li <tt> \subpage  cfltFadingParamsIntensity</tt>
   \li <tt> \subpage  cfltFadingParams</tt>
   \li <tt> \subpage  cfltCurSourceRange </tt>
   \li <tt> \subpage  cfltCurrentSources </tt>

   \page cflFncDesc Function Description
   This chapter describes following functions defined in the Color Functional Library:
   \li <tt> \subpage  cflfnCalcDutyCycleFromXYZ </tt>
   \li <tt> \subpage  cflfnCalcDutyCycleFromXYZSingCurrMultLED </tt>
   \li <tt> \subpage  cflfnCalcFadingColor </tt>
   \li <tt> \subpage  cflfnCalcFadingIntensity</tt>
   \li <tt> \subpage  cflfnCalcFadingParamsCol </tt>
   \li <tt> \subpage  cflfnCalcFadingParamsInt </tt>
   \li <tt> \subpage  cflfnCalcFadingParams</tt>
   \li <tt> \subpage  cflfnCalcLedTargetxy </tt>
   \li <tt> \subpage  cflfnCalcMaxIntensityFromRGB </tt>
   \li <tt> \subpage  cflfnCalcTargetYIntensity </tt>
   \li <tt> \subpage  cflfnCalcTargetYIntensityFromMatrix </tt>
   \li <tt> \subpage  cflfnRgbIToxyY </tt>
   \li <tt> \subpage  cflfnRgbToxyY </tt>
   \li <tt> \subpage  cflfnScaleIntensity </tt>
   \li <tt> \subpage  cflfnxyYToXYZ </tt>
   \li <tt> \subpage  cflfnXyzToDutyCycle </tt>
   \li <tt> \subpage  cflfnXyzToDutyCycleSingCurrMultLED </tt>
   \li <tt> \subpage  cflfnXYZToxyY </tt>

   \page cflMacDesc Macro Description
   This chapter describes following macros defined in the Color Functional Library:
   \li <tt> \subpage  cfldefMaxNumCurSrc </tt>
   \li <tt> \subpage  cfldefBrightnessTableLength </tt>
   \li <tt> \subpage  cfldefMaxCurSrc </tt>
   \li <tt> \subpage  cfldefMinCurSrc </tt>
   \li <tt> \subpage  cfldefMaxDutyCycle </tt>
   \li <tt> \subpage  cfldefCurSrcIndRed </tt>
   \li <tt> \subpage  cfldefCurSrcIndGreen </tt>
   \li <tt> \subpage  cfldefCurSrcIndBlue </tt>
   \li <tt> \subpage  cfldefLedIndRed </tt>
   \li <tt> \subpage  cfldefLedIndGreen </tt>
   \li <tt> \subpage  cfldefLedIndBlue </tt>
   \li <tt> \subpage  cfldefxInd </tt>
   \li <tt> \subpage  cfldefyInd </tt>
   \li <tt> \subpage  cfldefYInd </tt>
   \li <tt> \subpage  cfldefMaxCurLimLowCs </tt>
   \li <tt> \subpage  cfldefRoomTmp </tt>
   \li <tt> \subpage  cfldefDeltaTempOff </tt>
   \li <tt> \subpage  cfldefWPxCoordSign </tt>
   \li <tt> \subpage  cfldefWPxCoordExp </tt>
   \li <tt> \subpage  cfldefWPxCoordMan </tt>
   \li <tt> \subpage  cfldefWPxCoord </tt>
   \li <tt> \subpage  cfldefWPyCoordSign </tt>
   \li <tt> \subpage  cfldefWPyCoordExp </tt>
   \li <tt> \subpage  cfldefWPyCoordMan </tt>
   \li <tt> \subpage  cfldefWPyCoord </tt>

   \page cflConsDesc Constants Description
   This chapter describes following constants defined in the Color Functional Library:
   \li <tt> \subpage  cflconREDTEMP </tt>
   \li <tt> \subpage  cflconGREENTEMP </tt>
   \li <tt> \subpage  cflconBLUETEMP </tt>
   \li <tt> \subpage  cflconSRGBtoXYZ </tt>
   \li <tt> \subpage  cflconXYZtoSRGB </tt>
   \li <tt> \subpage  cflconMaxIntensity </tt>
   \li <tt> \subpage  cflconSlopeColorgamut </tt>
   \li <tt> \subpage  cflconOffsetColorgamut </tt>
   \li <tt> \subpage  cflconBrightTab </tt>
   \li <tt> \subpage  cflconMSlope </tt>
   \li <tt> \subpage  cflconBOffset </tt>
   \li <tt> \subpage  cflconWhitePoint </tt>
   \li <tt> \subpage  cflconScaleRGB</tt>
   \li <tt> \subpage  cflconLimit_Max_Output</tt>
 */

#ifndef CFL_H_
#define CFL_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "cfl_Base_Constants.h"
#include "cfl_Base_Defines.h"
#include "cfl_Base_Typedefs.h"

#include "cfl_CalcDutyCycleFromXYZ.h"
#include "cfl_CalcDutyCycleFromXYZSingCurrMultLED.h"
#include "cfl_CalcFadingColor.h"
#include "cfl_CalcFadingIntensity.h"
#include "cfl_CalcFadingParams.h"
#include "cfl_CalcFadingParamsColor.h"
#include "cfl_CalcFadingParamsIntensity.h"
#include "cfl_CalcLedTargetxy.h"
#include "cfl_CalcMaxIntensityFromRGB.h"
#include "cfl_CalcTargetYIntensity.h"
#include "cfl_CalcTargetYIntensityFromMatrix.h"
#include "cfl_RgbIToxyY.h"
#include "cfl_RgbToxyY.h"
#include "cfl_ScaleIntensity.h"
#include "cfl_xyYToXYZ.h"
#include "cfl_XyzToDutyCycle.h"
#include "cfl_XyzToDutyCycleSingCurrMultLED.h"
#include "cfl_XYZToxyY.h"


#endif /* CFL_H_ */
/* EOF */
