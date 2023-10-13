/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  cfl_Base_Defines.h
 * \date      14.07.2015
 * \brief     Color Functional Library defines file.
 * \author    DX.cba, DX.pri
 **************************************************************************************/

#ifndef CFL_BASE_DEFINES_H_
#define CFL_BASE_DEFINES_H_

/***************************************************************************************
* Includes                              (scope: global)
***************************************************************************************/
#include "bml_Base_Defines.h"
#include "bml_Base_Typedefs.h"

/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/
/*!
   \anchor CFL_MAX_NUMBER_CUR_SCRS
   \addindex CFL_MAX_NUMBER_CUR_SCRS
   \page cfldefMaxNumCurSrc CFL_MAX_NUMBER_CUR_SCRS
   \details Constant representing the maximum number of available current sources, equal to 2.
 */
#define CFL_MAX_NUMBER_CUR_SCRS  2u

/*!
   \anchor CFL_BRIGHTNESS_TABLE_LENGTH
   \addindex CFL_BRIGHTNESS_TABLE_LENGTH
   \page cfldefBrightnessTableLength CFL_BRIGHTNESS_TABLE_LENGTH
   \details Constant representing the length of the brightness table (number of elements), equal to 101.
 */
#define CFL_BRIGHTNESS_TABLE_LENGTH 101u

/*!
   \anchor CFL_MAX_CUR_SRC
   \addindex CFL_MAX_CUR_SRC
   \page cfldefMaxCurSrc CFL_MAX_CUR_SRC
   \details Constant representing maximum current source, equal to 30.0.
 */
#define CFL_MAX_CUR_SRC {0,4,61440} /* 30 mA */

/*!
   \anchor CFL_MIN_CUR_SRC
   \addindex CFL_MIN_CUR_SRC
   \page cfldefMinCurSrc CFL_MIN_CUR_SRC
   \details Constant representing minimum current source, equal to 6.0.
 */
#define CFL_MIN_CUR_SRC {0,2,49152} /* 6 mA */

/*!
   \anchor CFL_MAX_DUTY_CYCLE
   \addindex CFL_MAX_DUTY_CYCLE
   \page cfldefMaxDutyCycle CFL_MAX_DUTY_CYCLE
   \details Constant representing the maximum duty cycle, equal to 65535.0.
 */
#define CFL_MAX_DUTY_CYCLE {0,15,65535}  /* 65535 */

/*!
   \anchor CFL_CUR_SRC_INDEX_RED
   \addindex CFL_CUR_SRC_INDEX_RED
   \page cfldefCurSrcIndRed CFL_CUR_SRC_INDEX_RED
   \details Constant representing current source index red led, equal to 0.
 */
#define CFL_CUR_SRC_INDEX_RED    0

/*!
   \anchor CFL_CUR_SRC_INDEX_GREEN
   \addindex CFL_CUR_SRC_INDEX_GREEN
   \page cfldefCurSrcIndGreen CFL_CUR_SRC_INDEX_GREEN
   \details Constant representing current source index green led, equal to 1.
 */
#define CFL_CUR_SRC_INDEX_GREEN  1u

/*!
   \anchor CFL_CUR_SRC_INDEX_BLUE
   \addindex CFL_CUR_SRC_INDEX_BLUE
   \page cfldefCurSrcIndBlue CFL_CUR_SRC_INDEX_BLUE
   \details Constant representing current source index blue led, equal to 2.
 */
#define CFL_CUR_SRC_INDEX_BLUE   2u

/*!
   \anchor CFL_LED_INDEX_RED
   \addindex CFL_LED_INDEX_RED
   \page cfldefLedIndRed CFL_LED_INDEX_RED
   \details Constant representing index red led, equal to 0.
 */
#define CFL_LED_INDEX_RED    0

/*!
   \anchor CFL_LED_INDEX_GREEN
   \addindex CFL_LED_INDEX_GREEN
   \page cfldefLedIndGreen CFL_LED_INDEX_GREEN
   \details Constant representing index green led, equal to 1.
 */
#define CFL_LED_INDEX_GREEN    1u

/*!
   \anchor CFL_LED_INDEX_BLUE
   \addindex CFL_LED_INDEX_BLUE
   \page cfldefLedIndBlue CFL_LED_INDEX_BLUE
   \details Constant representing index blue led, equal to 2.
 */
#define CFL_LED_INDEX_BLUE    2u

/*!
   \anchor CFL_Cx_INDEX
   \addindex CFL_Cx_INDEX
   \page cfldefxInd CFL_Cx_INDEX
   \details Constant representing x index, equal to 0.
 */
#define CFL_Cx_INDEX 0

/*!
   \anchor CFL_Cy_INDEX
   \addindex CFL_Cy_INDEX
   \page cfldefyInd CFL_Cy_INDEX
   \details Constant representing y index, equal to 1.
 */
#define CFL_Cy_INDEX 1

/*!
   \anchor CFL_Y_INDEX
   \addindex CFL_Y_INDEX
   \page cfldefYInd CFL_Y_INDEX
   \details Constant representing Y index, equal to 2.
 */
#define CFL_Y_INDEX 2

/*!
   \anchor CFL_MAX_DUTY_CYCLE_LOW_CS
   \addindex CFL_MAX_DUTY_CYCLE_LOW_CS
   \page cfldefMaxCurLimLowCs CFL_MAX_DUTY_CYCLE_LOW_CS
   \details Maximum duty cycle for low current source. There is a buffer in case of changing another channel to high current source, equal to 32768.
 */
#define CFL_MAX_DUTY_CYCLE_LOW_CS 0x8000u

/*!
   \anchor CFL_DELTA_TEMP_OFFSET
   \addindex CFL_DELTA_TEMP_OFFSET
   \page cfldefDeltaTempOff CFL_DELTA_TEMP_OFFSET
   \details Constant representing temperature offset value to convert temperature values {-40...100} in positive number range, equal to 40.
 */
#define CFL_DELTA_TEMP_OFFSET 40

/*!
   \anchor CFL_ROOM_TEMP
   \addindex CFL_ROOM_TEMP
   \page cfldefRoomTmp CFL_ROOM_TEMP
   \details Constant representing room temperature in degree centigrade, equal to 25.
 */
#define CFL_ROOM_TEMP 25u

/*!
   \anchor CFL_WP_X_COORD_SIGN
   \addindex CFL_WP_X_COORD_SIGN
   \page cfldefWPxCoordSign CFL_WP_X_COORD_SIGN
   \details Constant representing floating point number sign component for x-coordinate of white point, equal to \ref FLOAT_0_33_SIGN.
 */
#define CFL_WP_X_COORD_SIGN FLOAT_0_33_SIGN

/*!
   \anchor CFL_WP_X_COORD_EXP
   \addindex CFL_WP_X_COORD_EXP
   \page cfldefWPxCoordExp CFL_WP_X_COORD_EXP
   \details Constant representing floating point number exponent component for x-coordinate of white point, equal to \ref FLOAT_0_33_EXP.
 */
#define CFL_WP_X_COORD_EXP FLOAT_0_33_EXP

/*!
   \anchor CFL_WP_X_COORD_MAN
   \addindex CFL_WP_X_COORD_MAN
   \page cfldefWPxCoordMan CFL_WP_X_COORD_MAN
   \details Constant representing floating point number mantissa component for x-coordinate of white point, equal to \ref FLOAT_0_33_MAN.
 */
#define CFL_WP_X_COORD_MAN FLOAT_0_33_MAN

/*!
   \anchor CFL_WP_X_COORD
   \addindex CFL_WP_X_COORD
   \page cfldefWPxCoord CFL_WP_X_COORD
   \details Constant representing floating point number x-coordinate for white point, equal to \ref FLOAT_0_33.
 */
#define CFL_WP_X_COORD {CFL_WP_X_COORD_SIGN, CFL_WP_X_COORD_EXP, CFL_WP_X_COORD_MAN}

/*!
   \anchor CFL_WP_Y_COORD_SIGN
   \addindex CFL_WP_Y_COORD_SIGN
   \page cfldefWPyCoordSign CFL_WP_Y_COORD_SIGN
   \details Constant representing floating point number sign component for y-coordinate of white point, equal to \ref FLOAT_0_33_SIGN.
 */
#define CFL_WP_Y_COORD_SIGN FLOAT_0_33_SIGN

/*!
   \anchor CFL_WP_Y_COORD_EXP
   \addindex CFL_WP_Y_COORD_EXP
   \page cfldefWPyCoordExp CFL_WP_Y_COORD_EXP
   \details Constant representing floating point number exponent component for y-coordinate of white point, equal to \ref FLOAT_0_33_EXP.
 */
#define CFL_WP_Y_COORD_EXP FLOAT_0_33_EXP

/*!
   \anchor CFL_WP_Y_COORD_MAN
   \addindex CFL_WP_Y_COORD_MAN
   \page cfldefWPyCoordMan CFL_WP_Y_COORD_MAN
   \details Constant representing floating point number mantissa component for y-coordinate of white point, equal to \ref FLOAT_0_33_MAN.
 */
#define CFL_WP_Y_COORD_MAN FLOAT_0_33_MAN

/*!
   \anchor CFL_WP_Y_COORD
   \addindex CFL_WP_Y_COORD
   \page cfldefWPyCoord CFL_WP_Y_COORD
   \details Constant representing floating point number y-coordinate for white point, equal to \ref FLOAT_0_33.
 */
#define CFL_WP_Y_COORD {CFL_WP_Y_COORD_SIGN, CFL_WP_Y_COORD_EXP, CFL_WP_Y_COORD_MAN}

#endif /* CFL_BASE_DEFINES_H_ */
/* EOF */
