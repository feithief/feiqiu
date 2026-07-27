/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  bml_Base_Defines.h
 * \date      05.11.2015
 * \brief     Basic Math Library defines file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef BML_BASE_DEFINES_H_
#define BML_BASE_DEFINES_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/

/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/
/*!
   \anchor FLOAT_SIGN_POS
   \addindex FLOAT_SIGN_POS
   \page bmldefFloatSignPos         FLOAT_SIGN_POS
   \details Constant representing sign value for positive floating point numbers, equal to 0
 */
#define FLOAT_SIGN_POS 0

/*!
   \anchor FLOAT_SIGN_NEG
   \addindex FLOAT_SIGN_NEG
   \page bmldefFloatSignNeg         FLOAT_SIGN_NEG
   \details Constant representing sign value for negative floating point numbers, equal to 1
 */
#define FLOAT_SIGN_NEG 1

/*!
   \anchor FLOAT_MAX_EXP
   \addindex FLOAT_MAX_EXP
   \page bmldefFloatMaxExp         FLOAT_MAX_EXP
   \details Constant representing maximum exponent value for floating point numbers, equal to 127
 */
#define FLOAT_MAX_EXP 127

/*!
   \anchor FLOAT_MIN_EXP
   \addindex FLOAT_MIN_EXP
   \page bmldefFloatMinExp         FLOAT_MIN_EXP
   \details Constant representing minimum exponent value for floating point numbers, equal to -127
 */
#define FLOAT_MIN_EXP (-127)

/*!
   \anchor FLOAT_MAX_MAN
   \addindex FLOAT_MAX_MAN
   \page bmldefFloatMaxMan         FLOAT_MAX_MAN
   \details Constant representing maximum mantissa value for floating point numbers, equal to 65535
 */
#define FLOAT_MAX_MAN 65535u

/*!
   \anchor FLOAT_MIN_MAN
   \addindex FLOAT_MIN_MAN
   \page bmldefFloatMinMan         FLOAT_MIN_MAN
   \details Constant representing minimum mantissa value for floating point numbers, equal to 32768
 */
#define FLOAT_MIN_MAN 32768u


/***************************************************************************************
*                       positive pFloat numbers
***************************************************************************************/
/*!
   \anchor FLOAT_0_SIGN
   \addindex FLOAT_0_SIGN
   \page bmldefFloat0Sign      FLOAT_0_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.0
 */
#define FLOAT_0_SIGN        FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_EXP
   \addindex FLOAT_0_EXP
   \page bmldefFloat0Exp       FLOAT_0_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.0
 */
#define FLOAT_0_EXP         0

/*!
   \anchor FLOAT_0_MAN
   \addindex FLOAT_0_MAN
   \page bmldefFloat0Man       FLOAT_0_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.0
 */
#define FLOAT_0_MAN         0u

/*!
   \anchor FLOAT_0
   \addindex FLOAT_0
   \page bmldefFloat0          FLOAT_0
   \details Constant representing the value of a floating number, equal to 0.0
 */
#define FLOAT_0             {FLOAT_0_SIGN,FLOAT_0_EXP,FLOAT_0_MAN}

/*!
   \anchor FLOAT_MIN_POS_SIGN
   \addindex FLOAT_MIN_POS_SIGN
   \page bmldefFloatMinPosSign     FLOAT_MIN_POS_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 2^(-127)
 */
#define FLOAT_MIN_POS_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_MIN_POS_EXP
   \addindex FLOAT_MIN_POS_EXP
   \page bmldefFloatMinPosExp      FLOAT_MIN_POS_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 2^(-127)
 */
#define FLOAT_MIN_POS_EXP       (FLOAT_MIN_EXP)

/*!
   \anchor FLOAT_MIN_POS_MAN
   \addindex FLOAT_MIN_POS_MAN
   \page bmldefFloatMinPosMan      FLOAT_MIN_POS_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 2^(-127)
 */
#define FLOAT_MIN_POS_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MIN_POS
   \addindex FLOAT_MIN_POS
   \page bmldefFloatMinPos         FLOAT_MIN_POS
   \details Constant representing the value of a floating number, equal to 2^(-127)
 */
#define FLOAT_MIN_POS           {FLOAT_MIN_POS_SIGN, FLOAT_MIN_POS_EXP, FLOAT_MIN_POS_MAN}

/*!
   \anchor FLOAT_SQRT_MIN_POS_SIGN
   \addindex FLOAT_SQRT_MIN_POS_SIGN
   \page bmldefFloatSqrtMinPosSign      FLOAT_SQRT_MIN_POS_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to sqrt(2^(-127))
 */
#define FLOAT_SQRT_MIN_POS_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_SQRT_MIN_POS_EXP
   \addindex FLOAT_SQRT_MIN_POS_EXP
   \page bmldefFloatSqrtMinPosExp      FLOAT_SQRT_MIN_POS_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to sqrt(2^(-127))
 */
#define FLOAT_SQRT_MIN_POS_EXP      (-64)

/*!
   \anchor FLOAT_SQRT_MIN_POS_MAN
   \addindex FLOAT_SQRT_MIN_POS_MAN
   \page bmldefFloatSqrtMinPosMan      FLOAT_SQRT_MIN_POS_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to sqrt(2^(-127))
 */
#define FLOAT_SQRT_MIN_POS_MAN      46341

/*!
   \anchor FLOAT_SQRT_MIN_POS
   \addindex FLOAT_SQRT_MIN_POS
   \page bmldefFloatSqrtMinPos         FLOAT_SQRT_MIN_POS
   \details Constant representing the value of a floating number, equal to sqrt(2^(-127))
 */
#define FLOAT_SQRT_MIN_POS          {FLOAT_SQRT_MIN_POS_SIGN, FLOAT_SQRT_MIN_POS_EXP, FLOAT_SQRT_MIN_POS_MAN}

/*!
   \anchor FLOAT_MIN_POS_MUL2_SIGN
   \addindex FLOAT_MIN_POS_MUL2_SIGN
   \page bmldefFloatMinPosMul2Sign     FLOAT_MIN_POS_MUL2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 2^(-127)*2
 */
#define FLOAT_MIN_POS_MUL2_SIGN  FLOAT_MIN_POS_SIGN

/*!
   \anchor FLOAT_MIN_POS_MUL2_EXP
   \addindex FLOAT_MIN_POS_MUL2_EXP
   \page bmldefFloatMinPosMul2Exp      FLOAT_MIN_POS_MUL2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 2^(-127)*2
 */
#define FLOAT_MIN_POS_MUL2_EXP      (FLOAT_MIN_POS_EXP+1)

/*!
   \anchor FLOAT_MIN_POS_MUL2_MAN
   \addindex FLOAT_MIN_POS_MUL2_MAN
   \page bmldefFloatMinPosMul2Man      FLOAT_MIN_POS_MUL2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 2^(-127)*2
 */
#define FLOAT_MIN_POS_MUL2_MAN      FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MIN_POS_MUL2
   \addindex FLOAT_MIN_POS_MUL2
   \page bmldefFloatMinPosMul2         FLOAT_MIN_POS_MUL2
   \details Constant representing the value of a floating number, equal to 2^(-127)*2
 */
#define FLOAT_MIN_POS_MUL2  {FLOAT_MIN_POS_MUL2_SIGN, FLOAT_MIN_POS_MUL2_EXP, FLOAT_MIN_POS_MUL2_MAN}

/*!
   \anchor FLOAT_MIN_POS_MUL3_SIGN
   \addindex FLOAT_MIN_POS_MUL3_SIGN
   \page bmldefFloatMinPosMul3Sign     FLOAT_MIN_POS_MUL3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 2^(-127)*3
 */
#define FLOAT_MIN_POS_MUL3_SIGN  FLOAT_MIN_POS_SIGN

/*!
   \anchor FLOAT_MIN_POS_MUL3_EXP
   \addindex FLOAT_MIN_POS_MUL3_EXP
   \page bmldefFloatMinPosMul3Exp      FLOAT_MIN_POS_MUL3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 2^(-127)*3
 */
#define FLOAT_MIN_POS_MUL3_EXP      (-126)

/*!
   \anchor FLOAT_MIN_POS_MUL3_MAN
   \addindex FLOAT_MIN_POS_MUL3_MAN
   \page bmldefFloatMinPosMul3Man      FLOAT_MIN_POS_MUL3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 2^(-127)*3
 */
#define FLOAT_MIN_POS_MUL3_MAN      49152

/*!
   \anchor FLOAT_MIN_POS_MUL3
   \addindex FLOAT_MIN_POS_MUL3
   \page bmldefFloatMinPosMul3         FLOAT_MIN_POS_MUL3
   \details Constant representing the value of a floating number, equal to 2^(-127)*3
 */
#define FLOAT_MIN_POS_MUL3  {FLOAT_MIN_POS_MUL3_SIGN, FLOAT_MIN_POS_MUL3_EXP, FLOAT_MIN_POS_MUL3_MAN}

/*!
   \anchor FLOAT_0_001_SIGN
   \addindex FLOAT_0_001_SIGN
   \page bmldefFloat0_001_Sign     FLOAT_0_001_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.001
 */
#define FLOAT_0_001_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_001_EXP
   \addindex FLOAT_0_001_EXP
   \page bmldefFloat0_001_Exp     FLOAT_0_001_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.001
 */
#define FLOAT_0_001_EXP     (-10)

/*!
   \anchor FLOAT_0_001_MAN
   \addindex FLOAT_0_001_MAN
   \page bmldefFloat0_001_Man     FLOAT_0_001_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.001
 */
#define FLOAT_0_001_MAN     33554

/*!
   \anchor FLOAT_0_001
   \addindex FLOAT_0_001
   \page bmldefFloat0_001    FLOAT_0_001
   \details Constant representing the value of a floating number, equal to 0.001
 */
#define FLOAT_0_001         {FLOAT_0_001_SIGN, FLOAT_0_001_EXP,FLOAT_0_001_MAN}

/*!
   \anchor FLOAT_0_01_SIGN
   \addindex FLOAT_0_01_SIGN
   \page bmldefFloat0_01_Sign     FLOAT_0_01_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.01
 */
#define FLOAT_0_01_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_01_EXP
   \addindex FLOAT_0_01_EXP
   \page bmldefFloat0_01_Exp     FLOAT_0_01_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.01
 */
#define FLOAT_0_01_EXP      (-7)

/*!
   \anchor FLOAT_0_01_MAN
   \addindex FLOAT_0_01_MAN
   \page bmldefFloat0_01_Man     FLOAT_0_01_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.01
 */
#define FLOAT_0_01_MAN      41943

/*!
   \anchor FLOAT_0_01
   \addindex FLOAT_0_01
   \page bmldefFloat0_01    FLOAT_0_01
   \details Constant representing the value of a floating number, equal to 0.01
 */
#define FLOAT_0_01          {FLOAT_0_01_SIGN, FLOAT_0_01_EXP,FLOAT_0_01_MAN}

/*!
   \anchor FLOAT_0_1_SIGN
   \addindex FLOAT_0_1_SIGN
   \page bmldefFloat0_1_Sign     FLOAT_0_1_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.1
 */
#define FLOAT_0_1_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_1_EXP
   \addindex FLOAT_0_1_EXP
   \page bmldefFloat0_1_Exp     FLOAT_0_1_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.1
 */
#define FLOAT_0_1_EXP       (-4)

/*!
   \anchor FLOAT_0_1_MAN
   \addindex FLOAT_0_1_MAN
   \page bmldefFloat0_1_Man     FLOAT_0_1_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.1
 */
#define FLOAT_0_1_MAN       52428

/*!
   \anchor FLOAT_0_1
   \addindex FLOAT_0_1
   \page bmldefFloat0_1    FLOAT_0_1
   \details Constant representing the value of a floating number, equal to 0.1
 */
#define FLOAT_0_1           {FLOAT_0_1_SIGN, FLOAT_0_1_EXP,FLOAT_0_1_MAN}

/*!
   \anchor FLOAT_0_2_SIGN
   \addindex FLOAT_0_2_SIGN
   \page bmldefFloat0_2_Sign     FLOAT_0_2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.2
 */
#define FLOAT_0_2_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_2_EXP
   \addindex FLOAT_0_2_EXP
   \page bmldefFloat0_2_Exp     FLOAT_0_2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.2
 */
#define FLOAT_0_2_EXP       (-3)

/*!
   \anchor FLOAT_0_2_MAN
   \addindex FLOAT_0_2_MAN
   \page bmldefFloat0_2_Man     FLOAT_0_2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.2
 */
#define FLOAT_0_2_MAN       52428

/*!
   \anchor FLOAT_0_2
   \addindex FLOAT_0_2
   \page bmldefFloat0_2    FLOAT_0_2
   \details Constant representing the value of a floating number, equal to 0.2
 */
#define FLOAT_0_2           {FLOAT_0_2_SIGN, FLOAT_0_2_EXP, FLOAT_0_2_MAN}

/*!
   \anchor FLOAT_0_25_SIGN
   \addindex FLOAT_0_25_SIGN
   \page bmldefFloat0_25_Sign     FLOAT_0_25_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.25
 */
#define FLOAT_0_25_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_25_EXP
   \addindex FLOAT_0_25_EXP
   \page bmldefFloat0_25_Exp     FLOAT_0_25_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.25
 */
#define FLOAT_0_25_EXP      (-2)

/*!
   \anchor FLOAT_0_25_MAN
   \addindex FLOAT_0_25_MAN
   \page bmldefFloat0_25_Man     FLOAT_0_25_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.25
 */
#define FLOAT_0_25_MAN      32768

/*!
   \anchor FLOAT_0_25
   \addindex FLOAT_0_25
   \page bmldefFloat0_25   FLOAT_0_25
   \details Constant representing the value of a floating number, equal to 0.25
 */
#define FLOAT_0_25          {FLOAT_0_25_SIGN, FLOAT_0_25_EXP, FLOAT_0_25_MAN}

/*!
   \anchor FLOAT_0_3_SIGN
   \addindex FLOAT_0_3_SIGN
   \page bmldefFloat0_3_Sign     FLOAT_0_3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.3
 */
#define FLOAT_0_3_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_3_EXP
   \addindex FLOAT_0_3_EXP
   \page bmldefFloat0_3_Exp     FLOAT_0_3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.3
 */
#define FLOAT_0_3_EXP       (-2)

/*!
   \anchor FLOAT_0_3_MAN
   \addindex FLOAT_0_3_MAN
   \page bmldefFloat0_3_Man     FLOAT_0_3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.3
 */
#define FLOAT_0_3_MAN       39322

/*!
   \anchor FLOAT_0_3
   \addindex FLOAT_0_3
   \page bmldefFloat0_3   FLOAT_0_3
   \details Constant representing the value of a floating number, equal to 0.3
 */
#define FLOAT_0_3           {FLOAT_0_3_SIGN, FLOAT_0_3_EXP, FLOAT_0_3_MAN}

/*!
   \anchor FLOAT_0_33_SIGN
   \addindex FLOAT_0_33_SIGN
   \page bmldefFloat0_33_Sign     FLOAT_0_33_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.33
 */
#define FLOAT_0_33_SIGN FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_33_EXP
   \addindex FLOAT_0_33_EXP
   \page bmldefFloat0_33_Exp     FLOAT_0_33_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.33
 */
#define FLOAT_0_33_EXP (-2)

/*!
   \anchor FLOAT_0_33_MAN
   \addindex FLOAT_0_33_MAN
   \page bmldefFloat0_33_Man     FLOAT_0_33_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.33
 */
#define FLOAT_0_33_MAN 43691

/*!
   \anchor FLOAT_0_33
   \addindex FLOAT_0_33
   \page bmldefFloat0_33   FLOAT_0_33
   \details Constant representing the value of a floating number, equal to 0.33
 */
#define FLOAT_0_33 {FLOAT_0_33_SIGN, FLOAT_0_33_EXP, FLOAT_0_33_MAN}

/*!
   \anchor FLOAT_0_5_SIGN
   \addindex FLOAT_0_5_SIGN
   \page bmldefFloat0_5_Sign     FLOAT_0_5_SIGN
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.5
 */
#define FLOAT_0_5_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_5_EXP
   \addindex FLOAT_0_5_EXP
   \page bmldefFloat0_5_Exp     FLOAT_0_5_EXP
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.5
 */
#define FLOAT_0_5_EXP       (-1)

/*!
   \anchor FLOAT_0_5_MAN
   \addindex FLOAT_0_5_MAN
   \page bmldefFloat0_5_Man     FLOAT_0_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.5
 */
#define FLOAT_0_5_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_0_5
   \addindex FLOAT_0_5
   \page bmldefFloat0_5   FLOAT_0_5
   \details Constant representing the value of a floating number, equal to 0.5
 */
#define FLOAT_0_5           {FLOAT_0_5_SIGN, FLOAT_0_5_EXP, FLOAT_0_5_MAN}

/*!
   \anchor FLOAT_0_6_SIGN
   \addindex FLOAT_0_6_SIGN
   \page bmldefFloat0_6_Sign     FLOAT_0_6_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.6
 */
#define FLOAT_0_6_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_6_EXP
   \addindex FLOAT_0_6_EXP
   \page bmldefFloat0_6_Exp     FLOAT_0_6_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.6
 */
#define FLOAT_0_6_EXP       (-1)

/*!
   \anchor FLOAT_0_6_MAN
   \addindex FLOAT_0_6_MAN
   \page bmldefFloat0_6_Man     FLOAT_0_6_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.6
 */
#define FLOAT_0_6_MAN       39322

/*!
   \anchor FLOAT_0_6
   \addindex FLOAT_0_6
   \page bmldefFloat0_6   FLOAT_0_6
   \details Constant representing the value of a floating number, equal to 0.6
 */
#define FLOAT_0_6           {FLOAT_0_6_SIGN, FLOAT_0_6_EXP, FLOAT_0_6_MAN}

/*!
   \anchor FLOAT_0_66_SIGN
   \addindex FLOAT_0_66_SIGN
   \page bmldefFloat0_66_Sign     FLOAT_0_66_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.66
 */
#define FLOAT_0_66_SIGN FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_66_EXP
   \addindex FLOAT_0_66_EXP
   \page bmldefFloat0_66_Exp     FLOAT_0_66_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.66
 */
#define FLOAT_0_66_EXP (-1)

/*!
   \anchor FLOAT_0_66_MAN
   \addindex FLOAT_0_66_MAN
   \page bmldefFloat0_66_Man     FLOAT_0_66_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.66
 */
#define FLOAT_0_66_MAN 43691

/*!
   \anchor FLOAT_0_66
   \addindex FLOAT_0_66
   \page bmldefFloat0_66  FLOAT_0_6
   \details Constant representing the value of a floating number, equal to 0.66
 */
#define FLOAT_0_66 {FLOAT_0_66_SIGN, FLOAT_0_66_EXP, FLOAT_0_66_MAN}

/*!
   \anchor FLOAT_0_8_SIGN
   \addindex FLOAT_0_8_SIGN
   \page bmldefFloat0_8_Sign     FLOAT_0_8_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.8
 */
#define FLOAT_0_8_SIGN FLOAT_SIGN_POS

/*!
   \anchor FLOAT_0_8_EXP
   \addindex FLOAT_0_8_EXP
   \page bmldefFloat0_8_Exp     FLOAT_0_8_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.8
 */
#define FLOAT_0_8_EXP (-1)

/*!
   \anchor FLOAT_0_8_MAN
   \addindex FLOAT_0_8_MAN
   \page bmldefFloat0_8_Man     FLOAT_0_8_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.8
 */
#define FLOAT_0_8_MAN 52429

/*!
   \anchor FLOAT_0_8
   \addindex FLOAT_0_8
   \page bmldefFloat0_8   FLOAT_0_8
   \details Constant representing the value of a floating number, equal to 0.8
 */
#define FLOAT_0_8 {FLOAT_0_8_SIGN, FLOAT_0_8_EXP, FLOAT_0_8_MAN}

/*!
   \anchor FLOAT_1_SIGN
   \addindex FLOAT_1_SIGN
   \page bmldefFloat1_Sign     FLOAT_1_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 1.0
 */
#define FLOAT_1_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_1_EXP
   \addindex FLOAT_1_EXP
   \page bmldefFloat1_Exp     FLOAT_1_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 1.0
 */
#define FLOAT_1_EXP     0

/*!
   \anchor FLOAT_1_MAN
   \addindex FLOAT_1_MAN
   \page bmldefFloat1_Man     FLOAT_1_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 1.0
 */
#define FLOAT_1_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_1
   \addindex FLOAT_1
   \page bmldefFloat1   FLOAT_1
   \details Constant representing the value of a floating number, equal to 1.0
 */
#define FLOAT_1  {FLOAT_1_SIGN,  FLOAT_1_EXP, FLOAT_1_MAN}

/*!
   \anchor FLOAT_1_5_SIGN
   \addindex FLOAT_1_5_SIGN
   \page bmldefFloat1_5_Sign     FLOAT_1_5_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 1.5
 */
#define FLOAT_1_5_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_1_5_EXP
   \addindex FLOAT_1_5_EXP
   \page bmldefFloat1_5_Exp     FLOAT_1_5_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 1.5
 */
#define FLOAT_1_5_EXP   0

/*!
   \anchor FLOAT_1_5_MAN
   \addindex FLOAT_1_5_MAN
   \page bmldefFloat1_5_Man     FLOAT_1_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 1.5
 */
#define FLOAT_1_5_MAN   49152

/*!
   \anchor FLOAT_1_5
   \addindex FLOAT_1_5
   \page bmldefFloat1_5 FLOAT_1_5
   \details Constant representing the value of a floating number, equal to 1.5
 */
#define FLOAT_1_5       {FLOAT_1_5_SIGN,  FLOAT_1_5_EXP, FLOAT_1_5_MAN}

/*!
   \anchor FLOAT_2_SIGN
   \addindex FLOAT_2_SIGN
   \page bmldefFloat2_Sign     FLOAT_2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 2.0
 */
#define FLOAT_2_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_2_EXP
   \addindex FLOAT_2_EXP
   \page bmldefFloat2_Exp     FLOAT_2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 2.0
 */
#define FLOAT_2_EXP     1

/*!
   \anchor FLOAT_2_MAN
   \addindex FLOAT_2_MAN
   \page bmldefFloat2_Man     FLOAT_2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 2.0
 */
#define FLOAT_2_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_2
   \addindex FLOAT_2
   \page bmldefFloat2   FLOAT_2
   \details Constant representing the value of a floating number, equal to 2.0
 */
#define FLOAT_2         {FLOAT_2_SIGN, FLOAT_2_EXP, FLOAT_2_MAN}

/*!
   \anchor FLOAT_3_SIGN
   \addindex FLOAT_3_SIGN
   \page bmldefFloat3_Sign     FLOAT_3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 3.0
 */
#define FLOAT_3_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_3_EXP
   \addindex FLOAT_3_EXP
   \page bmldefFloat3_Exp     FLOAT_3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 3.0
 */
#define FLOAT_3_EXP     1

/*!
   \anchor FLOAT_3_MAN
   \addindex FLOAT_3_MAN
   \page bmldefFloat3_Man     FLOAT_3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 3.0
 */
#define FLOAT_3_MAN     49152

/*!
   \anchor FLOAT_3
   \addindex FLOAT_3
   \page bmldefFloat3   FLOAT_3
   \details Constant representing the value of a floating number, equal to 3.0
 */
#define FLOAT_3         {FLOAT_3_SIGN, FLOAT_3_EXP, FLOAT_3_MAN}

/*!
   \anchor FLOAT_4_SIGN
   \addindex FLOAT_4_SIGN
   \page bmldefFloat4_Sign     FLOAT_4_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 4.0
 */
#define FLOAT_4_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_4_EXP
   \addindex FLOAT_4_EXP
   \page bmldefFloat4_Exp     FLOAT_4_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 4.0
 */
#define FLOAT_4_EXP     2

/*!
   \anchor FLOAT_4_MAN
   \addindex FLOAT_4_MAN
   \page bmldefFloat4_Man     FLOAT_4_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 4.0
 */
#define FLOAT_4_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_4
   \addindex FLOAT_4
   \page bmldefFloat4   FLOAT_4
   \details Constant representing the value of a floating number, equal to 4.0
 */
#define FLOAT_4         {FLOAT_4_SIGN, FLOAT_4_EXP, FLOAT_4_MAN}

/*!
   \anchor FLOAT_4_8_SIGN
   \addindex FLOAT_4_8_SIGN
   \page bmldefFloat4_8_Sign     FLOAT_4_8_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 4.8
 */
#define FLOAT_4_8_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_4_8_EXP
   \addindex FLOAT_4_8_EXP
   \page bmldefFloat4_8_Exp     FLOAT_4_8_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 4.8
 */
#define FLOAT_4_8_EXP       2

/*!
   \anchor FLOAT_4_8_MAN
   \addindex FLOAT_4_8_MAN
   \page bmldefFloat4_8_Man     FLOAT_4_8_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 4.8
 */
#define FLOAT_4_8_MAN       39322

/*!
   \anchor FLOAT_4_8
   \addindex FLOAT_4_8
   \page bmldefFloat4_8 FLOAT_4_8
   \details Constant representing the value of a floating number, equal to 4.8
 */
#define FLOAT_4_8           {FLOAT_4_8_SIGN, FLOAT_4_8_EXP, FLOAT_4_8_MAN}

/*!
   \anchor FLOAT_5_SIGN
   \addindex FLOAT_5_SIGN
   \page bmldefFloat5_Sign     FLOAT_5_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 5.0
 */
#define FLOAT_5_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_5_EXP
   \addindex FLOAT_5_EXP
   \page bmldefFloat5_Exp     FLOAT_5_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 5.0
 */
#define FLOAT_5_EXP     2

/*!
   \anchor FLOAT_5_MAN
   \addindex FLOAT_5_MAN
   \page bmldefFloat5_Man     FLOAT_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 5.0
 */
#define FLOAT_5_MAN     40960

/*!
   \anchor FLOAT_5
   \addindex FLOAT_5
   \page bmldefFloat5   FLOAT_5
   \details Constant representing the value of a floating number, equal to 5.0
 */
#define FLOAT_5         {FLOAT_5_SIGN, FLOAT_5_EXP, FLOAT_5_MAN}

/*!
   \anchor FLOAT_6_SIGN
   \addindex FLOAT_6_SIGN
   \page bmldefFloat6_Sign     FLOAT_6_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 6.0
 */
#define FLOAT_6_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_6_EXP
   \addindex FLOAT_6_EXP
   \page bmldefFloat6_Exp     FLOAT_6_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 6.0
 */
#define FLOAT_6_EXP     2

/*!
   \anchor FLOAT_6_MAN
   \addindex FLOAT_6_MAN
   \page bmldefFloat6_Man     FLOAT_6_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 6.0
 */
#define FLOAT_6_MAN     49152

/*!
   \anchor FLOAT_6
   \addindex FLOAT_6
   \page bmldefFloat6   FLOAT_6
   \details Constant representing the value of a floating number, equal to 6.0
 */
#define FLOAT_6         {FLOAT_6_SIGN, FLOAT_6_EXP, FLOAT_6_MAN}

/*!
   \anchor FLOAT_7_SIGN
   \addindex FLOAT_7_SIGN
   \page bmldefFloat7_Sign     FLOAT_7_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 7.0
 */
#define FLOAT_7_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_7_EXP
   \addindex FLOAT_7_EXP
   \page bmldefFloat7_Exp     FLOAT_7_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 7.0
 */
#define FLOAT_7_EXP     2

/*!
   \anchor FLOAT_7_MAN
   \addindex FLOAT_7_MAN
   \page bmldefFloat7_Man     FLOAT_7_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 7.0
 */
#define FLOAT_7_MAN     57344

/*!
   \anchor FLOAT_7
   \addindex FLOAT_7
   \page bmldefFloat7   FLOAT_7
   \details Constant representing the value of a floating number, equal to 7.0
 */
#define FLOAT_7         {FLOAT_7_SIGN, FLOAT_7_EXP, FLOAT_7_MAN}

/*!
   \anchor FLOAT_8_SIGN
   \addindex FLOAT_8_SIGN
   \page bmldefFloat8_Sign     FLOAT_8_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 8.0
 */
#define FLOAT_8_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_8_EXP
   \addindex FLOAT_8_EXP
   \page bmldefFloat8_Exp     FLOAT_8_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 8.0
 */
#define FLOAT_8_EXP     3

/*!
   \anchor FLOAT_8_MAN
   \addindex FLOAT_8_MAN
   \page bmldefFloat8_Man     FLOAT_8_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 8.0
 */
#define FLOAT_8_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_8
   \addindex FLOAT_8
   \page bmldefFloat8   FLOAT_8
   \details Constant representing the value of a floating number, equal to 4.0
 */
#define FLOAT_8         {FLOAT_8_SIGN, FLOAT_8_EXP, FLOAT_8_MAN}

/*!
   \anchor FLOAT_9_SIGN
   \addindex FLOAT_9_SIGN
   \page bmldefFloat9_Sign     FLOAT_9_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 9.0
 */
#define FLOAT_9_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_9_EXP
   \addindex FLOAT_9_EXP
   \page bmldefFloat9_Exp     FLOAT_9_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 9.0
 */
#define FLOAT_9_EXP     3

/*!
   \anchor FLOAT_9_MAN
   \addindex FLOAT_9_MAN
   \page bmldefFloat9_Man     FLOAT_9_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 9.0
 */
#define FLOAT_9_MAN     36864

/*!
   \anchor FLOAT_9
   \addindex FLOAT_9
   \page bmldefFloat9   FLOAT_9
   \details Constant representing the value of a floating number, equal to 9.0
 */
#define FLOAT_9         {FLOAT_9_SIGN, FLOAT_9_EXP, FLOAT_9_MAN}

/*!
   \anchor FLOAT_9_99_SIGN
   \addindex FLOAT_9_99_SIGN
   \page bmldefFloat9_99_Sign     FLOAT_9_99_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 9.99
 */
#define FLOAT_9_99_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_9_99_EXP
   \addindex FLOAT_9_99_EXP
   \page bmldefFloat9_99_Exp     FLOAT_9_99_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 9.99
 */
#define FLOAT_9_99_EXP      3

/*!
   \anchor FLOAT_9_99_MAN
   \addindex FLOAT_9_99_MAN
   \page bmldefFloat9_99_Man     FLOAT_9_99_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 9.99
 */
#define FLOAT_9_99_MAN      40919

/*!
   \anchor FLOAT_9_99
   \addindex FLOAT_9_99
   \page bmldefFloat9_99 FLOAT_9_99
   \details Constant representing the value of a floating number, equal to 9.99
 */
#define FLOAT_9_99          {FLOAT_9_99_SIGN, FLOAT_9_99_EXP, FLOAT_9_99_MAN}

/*!
   \anchor FLOAT_10_SIGN
   \addindex FLOAT_10_SIGN
   \page bmldefFloat10_Sign     FLOAT_10_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 10.0
 */
#define FLOAT_10_SIGN   FLOAT_SIGN_POS

/*!
   \anchor FLOAT_10_EXP
   \addindex FLOAT_10_EXP
   \page bmldefFloat10_Exp     FLOAT_10_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 10.0
 */
#define FLOAT_10_EXP    3

/*!
   \anchor FLOAT_10_MAN
   \addindex FLOAT_10_MAN
   \page bmldefFloat10_Man     FLOAT_10_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 10.0
 */
#define FLOAT_10_MAN    40960

/*!
   \anchor FLOAT_10
   \addindex FLOAT_10
   \page bmldefFloat10   FLOAT_10
   \details Constant representing the value of a floating number, equal to 10.0
 */
#define FLOAT_10        {FLOAT_10_SIGN, FLOAT_10_EXP, FLOAT_10_MAN}

/*!
   \anchor FLOAT_20_SIGN
   \addindex FLOAT_20_SIGN
   \page bmldefFloat20_Sign     FLOAT_20_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 20.0
 */
#define FLOAT_20_SIGN       FLOAT_SIGN_POS

/*!
   \anchor FLOAT_20_EXP
   \addindex FLOAT_20_EXP
   \page bmldefFloat20_Exp     FLOAT_20_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 20.0
 */
#define FLOAT_20_EXP        4

/*!
   \anchor FLOAT_20_MAN
   \addindex FLOAT_20_MAN
   \page bmldefFloat20_Man     FLOAT_20_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 20.0
 */
#define FLOAT_20_MAN        40960

/*!
   \anchor FLOAT_20
   \addindex FLOAT_20
   \page bmldefFloat20   FLOAT_20
   \details Constant representing the value of a floating number, equal to 20.0
 */
#define FLOAT_20            {FLOAT_20_SIGN, FLOAT_20_EXP, FLOAT_20_MAN}

/*!
   \anchor FLOAT_40_SIGN
   \addindex FLOAT_40_SIGN
   \page bmldefFloat40_Sign     FLOAT_40_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 40.0
 */
#define FLOAT_40_SIGN   FLOAT_SIGN_POS

/*!
   \anchor FLOAT_40_EXP
   \addindex FLOAT_40_EXP
   \page bmldefFloat40_Exp     FLOAT_40_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 40.0
 */
#define FLOAT_40_EXP    5

/*!
   \anchor FLOAT_40_MAN
   \addindex FLOAT_40_MAN
   \page bmldefFloat40_Man     FLOAT_40_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 40.0
 */
#define FLOAT_40_MAN    40960

/*!
   \anchor FLOAT_40
   \addindex FLOAT_40
   \page bmldefFloat40   FLOAT_40
   \details Constant representing the value of a floating number, equal to 40.0
 */
#define FLOAT_40        {FLOAT_40_SIGN, FLOAT_40_EXP, FLOAT_40_MAN}

/*!
   \anchor FLOAT_50_SIGN
   \addindex FLOAT_50_SIGN
   \page bmldefFloat50_Sign     FLOAT_50_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 50.0
 */
#define FLOAT_50_SIGN   FLOAT_SIGN_POS

/*!
   \anchor FLOAT_50_EXP
   \addindex FLOAT_50_EXP
   \page bmldefFloat50_Exp     FLOAT_50_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 50.0
 */
#define FLOAT_50_EXP    5

/*!
   \anchor FLOAT_50_MAN
   \addindex FLOAT_50_MAN
   \page bmldefFloat50_Man     FLOAT_50_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 50.0
 */
#define FLOAT_50_MAN    51200

/*!
   \anchor FLOAT_50
   \addindex FLOAT_50
   \page bmldefFloat50   FLOAT_50
   \details Constant representing the value of a floating number, equal to 50.0
 */
#define FLOAT_50        {FLOAT_50_SIGN, FLOAT_50_EXP, FLOAT_50_MAN}

/*!
   \anchor FLOAT_100_SIGN
   \addindex FLOAT_100_SIGN
   \page bmldefFloat100_Sign     FLOAT_100_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 100.0
 */
#define FLOAT_100_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_100_EXP
   \addindex FLOAT_100_EXP
   \page bmldefFloat100_Exp     FLOAT_100_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 100.0
 */
#define FLOAT_100_EXP   6

/*!
   \anchor FLOAT_100_MAN
   \addindex FLOAT_100_MAN
   \page bmldefFloat100_Man     FLOAT_100_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 100.0
 */
#define FLOAT_100_MAN   51200

/*!
   \anchor FLOAT_100
   \addindex FLOAT_100
   \page bmldefFloat100   FLOAT_100
   \details Constant representing the value of a floating number, equal to 100.0
 */
#define FLOAT_100       {FLOAT_100_SIGN, FLOAT_100_EXP, FLOAT_100_MAN}

/*!
   \anchor FLOAT_150_SIGN
   \addindex FLOAT_150_SIGN
   \page bmldefFloat150_Sign     FLOAT_150_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 150.0
 */
#define FLOAT_150_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_150_EXP
   \addindex FLOAT_150_EXP
   \page bmldefFloat150_Exp     FLOAT_150_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 150.0
 */
#define FLOAT_150_EXP   7

/*!
   \anchor FLOAT_150_MAN
   \addindex FLOAT_150_MAN
   \page bmldefFloat150_Man     FLOAT_150_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 150.0
 */
#define FLOAT_150_MAN   38400

/*!
   \anchor FLOAT_150
   \addindex FLOAT_150
   \page bmldefFloat150   FLOAT_150
   \details Constant representing the value of a floating number, equal to 150.0
 */
#define FLOAT_150       {FLOAT_150_SIGN, FLOAT_150_EXP, FLOAT_150_MAN}

/*!
   \anchor FLOAT_200_SIGN
   \addindex FLOAT_200_SIGN
   \page bmldefFloat200_Sign     FLOAT_200_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 200.0
 */
#define FLOAT_200_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_200_EXP
   \addindex FLOAT_200_EXP
   \page bmldefFloat200_Exp     FLOAT_200_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 200.0
 */
#define FLOAT_200_EXP   7

/*!
   \anchor FLOAT_200_MAN
   \addindex FLOAT_200_MAN
   \page bmldefFloat200_Man     FLOAT_200_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 200.0
 */
#define FLOAT_200_MAN   51200

/*!
   \anchor FLOAT_200
   \addindex FLOAT_200
   \page bmldefFloat200   FLOAT_200
   \details Constant representing the value of a floating number, equal to 200.0
 */
#define FLOAT_200       {FLOAT_200_SIGN, FLOAT_200_EXP, FLOAT_200_MAN}

/*!
   \anchor FLOAT_254_SIGN
   \addindex FLOAT_254_SIGN
   \page bmldefFloat254_Sign     FLOAT_254_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 254.0
 */
#define FLOAT_254_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_254_EXP
   \addindex FLOAT_254_EXP
   \page bmldefFloat254_Exp     FLOAT_254_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 254.0
 */
#define FLOAT_254_EXP   7

/*!
   \anchor FLOAT_254_MAN
   \addindex FLOAT_254_MAN
   \page bmldefFloat254_Man     FLOAT_254_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 254.0
 */
#define FLOAT_254_MAN   65024

/*!
   \anchor FLOAT_254
   \addindex FLOAT_254
   \page bmldefFloat254   FLOAT_254
   \details Constant representing the value of a floating number, equal to 254.0
 */
#define FLOAT_254       {FLOAT_254_SIGN, FLOAT_254_EXP, FLOAT_254_MAN}

/*!
   \anchor FLOAT_255_SIGN
   \addindex FLOAT_255_SIGN
   \page bmldefFloat255_Sign     FLOAT_255_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 255.0
 */
#define FLOAT_255_SIGN  FLOAT_SIGN_POS

/*!
   \anchor FLOAT_255_EXP
   \addindex FLOAT_255_EXP
   \page bmldefFloat255_Exp     FLOAT_255_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 255.0
 */
#define FLOAT_255_EXP   7

/*!
   \anchor FLOAT_255_MAN
   \addindex FLOAT_255_MAN
   \page bmldefFloat255_Man     FLOAT_255_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 255.0
 */
#define FLOAT_255_MAN   65280

/*!
   \anchor FLOAT_255
   \addindex FLOAT_255
   \page bmldefFloat255   FLOAT_255
   \details Constant representing the value of a floating number, equal to 255.0
 */
#define FLOAT_255   {FLOAT_255_SIGN, FLOAT_255_EXP, FLOAT_255_MAN}

/*!
   \anchor FLOAT_1000_SIGN
   \addindex FLOAT_1000_SIGN
   \page bmldefFloat1000_Sign     FLOAT_1000_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 1000.0
 */
#define FLOAT_1000_SIGN FLOAT_SIGN_POS

/*!
   \anchor FLOAT_1000_EXP
   \addindex FLOAT_1000_EXP
   \page bmldefFloat1000_Exp     FLOAT_1000_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 1000.0
 */
#define FLOAT_1000_EXP  9

/*!
   \anchor FLOAT_1000_MAN
   \addindex FLOAT_1000_MAN
   \page bmldefFloat1000_Man     FLOAT_1000_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 1000.0
 */
#define FLOAT_1000_MAN  64000

/*!
   \anchor FLOAT_1000
   \addindex FLOAT_1000
   \page bmldefFloat1000  FLOAT_1000
   \details Constant representing the value of a floating number, equal to 1000.0
 */
#define FLOAT_1000      {FLOAT_1000_SIGN, FLOAT_1000_EXP, FLOAT_1000_MAN}

/*!
   \anchor FLOAT_4095_SIGN
   \addindex FLOAT_4095_SIGN
   \page bmldefFloat4095_Sign     FLOAT_4095_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 4095.0
 */
#define FLOAT_4095_SIGN FLOAT_SIGN_POS

/*!
   \anchor FLOAT_4095_EXP
   \addindex FLOAT_4095_EXP
   \page bmldefFloat4095_Exp     FLOAT_4095_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 4095.0
 */
#define FLOAT_4095_EXP  11

/*!
   \anchor FLOAT_4095_MAN
   \addindex FLOAT_4095_MAN
   \page bmldefFloat4095_Man     FLOAT_4095_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 4095.0
 */
#define FLOAT_4095_MAN  65520

/*!
   \anchor FLOAT_4095
   \addindex FLOAT_4095
   \page bmldefFloat4095   FLOAT_4095
   \details Constant representing the value of a floating number, equal to 4095.0
 */
#define FLOAT_4095      {FLOAT_4095_SIGN, FLOAT_4095_EXP, FLOAT_4095_MAN}

/*!
   \anchor FLOAT_32767_SIGN
   \addindex FLOAT_32767_SIGN
   \page bmldefFloat32767_Sign     FLOAT_32767_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 32767.0
 */
#define FLOAT_32767_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_32767_EXP
   \addindex FLOAT_32767_EXP
   \page bmldefFloat32767_Exp     FLOAT_32767_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 32767.0
 */
#define FLOAT_32767_EXP     14

/*!
   \anchor FLOAT_32767_MAN
   \addindex FLOAT_32767_MAN
   \page bmldefFloat32767_Man     FLOAT_32767_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 32767.0
 */
#define FLOAT_32767_MAN     65534

/*!
   \anchor FLOAT_32767
   \addindex FLOAT_32767
   \page bmldefFloat32767  FLOAT_32767
   \details Constant representing the value of a floating number, equal to 32767.0
 */
#define FLOAT_32767         {FLOAT_32767_SIGN, FLOAT_32767_EXP, FLOAT_32767_MAN}

/*!
   \anchor FLOAT_65535_SIGN
   \addindex FLOAT_65535_SIGN
   \page bmldefFloat65535_Sign FLOAT_65535_SIGN
   \details Struct element (sign) of Constant representing the value of a floating number, equal to 65535.0
 */
#define FLOAT_65535_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_65535_EXP
   \addindex FLOAT_65535_EXP
   \page bmldefFloat65535_Exp FLOAT_65535_EXP
   \details Struct element (exponent) of Constant representing the value of a floating number, equal to 65535.0
 */
#define FLOAT_65535_EXP      15

/*!
   \anchor FLOAT_65535_MAN
   \addindex FLOAT_65535_MAN
   \page bmldefFloat65535_Man FLOAT_65535_MAN
   \details Struct element (mantissa) of Constant representing the value of a floating number, equal to 65535.0
 */
#define FLOAT_65535_MAN      FLOAT_MAX_MAN

/*!
   \anchor FLOAT_65535
   \addindex FLOAT_65535
   \page bmldefFloat65535      FLOAT_65535
   \details Constant representing the value of a floating number, equal to 65535.0
 */
#define FLOAT_65535          {FLOAT_65535_SIGN, FLOAT_65535_EXP, FLOAT_65535_MAN}

/*!
   \anchor FLOAT_65536_SIGN
   \addindex FLOAT_65536_SIGN
   \page bmldefFloat65536_Sign FLOAT_65536_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 65536.0
 */
#define FLOAT_65536_SIGN    FLOAT_SIGN_POS

/*!
   \anchor FLOAT_65536_EXP
   \addindex FLOAT_65536_EXP
   \page bmldefFloat65536_Exp FLOAT_65536_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 65536.0
 */
#define FLOAT_65536_EXP     16

/*!
   \anchor FLOAT_65536_MAN
   \addindex FLOAT_65536_MAN
   \page bmldefFloat65536_Man FLOAT_65536_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 65536.0
 */
#define FLOAT_65536_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_65536
   \addindex FLOAT_65536
   \page bmldefFloat65536  FLOAT_65536
   \details Constant representing the value of a floating number, equal to 65536.0
 */
#define FLOAT_65536         {FLOAT_65536_SIGN, FLOAT_65536_EXP, FLOAT_65536_MAN}

/*!
   \anchor FLOAT_MAX_POS_MUL_0_0075_SIGN
   \addindex FLOAT_MAX_POS_MUL_0_0075_SIGN
   \page bmldefFloatMaxPosMul0_0075_Sign  FLOAT_MAX_POS_MUL_0_0075_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.0075*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_0075_SIGN   FLOAT_SIGN_POS

/*!
   \anchor FLOAT_MAX_POS_MUL_0_0075_EXP
   \addindex FLOAT_MAX_POS_MUL_0_0075_EXP
   \page bmldefFloatMaxPosMul0_0075_Exp  FLOAT_MAX_POS_MUL_0_0075_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.0075*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_0075_EXP    (120)

/*!
   \anchor FLOAT_MAX_POS_MUL_0_0075_MAN
   \addindex FLOAT_MAX_POS_MUL_0_0075_MAN
   \page bmldefFloatMaxPosMul0_0075_Man  FLOAT_MAX_POS_MUL_0_0075_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.0075*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_0075_MAN    62914

/*!
   \anchor FLOAT_MAX_POS_MUL_0_0075
   \addindex FLOAT_MAX_POS_MUL_0_0075
   \page bmldefFloatMaxPosMul0_0075  FLOAT_MAX_POS_MUL_0_0075
   \details Constant representing the value of a floating number, equal to 0.0075*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_0075        {FLOAT_MAX_POS_MUL_0_0075_SIGN, FLOAT_MAX_POS_MUL_0_0075_EXP, FLOAT_MAX_POS_MUL_0_0075_MAN}

/*!
   \anchor FLOAT_MAX_POS_MUL_0_75_SIGN
   \addindex FLOAT_MAX_POS_MUL_0_75_SIGN
   \page bmldefFloatMaxPosMul0_75_Sign  FLOAT_MAX_POS_MUL_0_75_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_75_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_MAX_POS_MUL_0_75_EXP
   \addindex FLOAT_MAX_POS_MUL_0_75_EXP
   \page bmldefFloatMaxPosMul0_75_Exp  FLOAT_MAX_POS_MUL_0_75_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_75_EXP      (FLOAT_MAX_EXP)

/*!
   \anchor FLOAT_MAX_POS_MUL_0_75_MAN
   \addindex FLOAT_MAX_POS_MUL_0_75_MAN
   \page bmldefFloatMaxPosMul0_75_Man  FLOAT_MAX_POS_MUL_0_75_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_75_MAN      49151

/*!
   \anchor FLOAT_MAX_POS_MUL_0_75
   \addindex FLOAT_MAX_POS_MUL_0_75
   \page bmldefFloatMaxPosMul0_75  FLOAT_MAX_POS_MUL_0_75
   \details Constant representing the value of a floating number, equal to 0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_POS_MUL_0_75          {FLOAT_MAX_POS_MUL_0_75_SIGN, FLOAT_MAX_POS_MUL_0_75_EXP, FLOAT_MAX_POS_MUL_0_75_MAN}

/*!
   \anchor FLOAT_SQRT_MAX_POS_SIGN
   \addindex FLOAT_SQRT_MAX_POS_SIGN
   \page bmldefFloatSqrtMaxPosSign     FLOAT_SQRT_MAX_POS_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to sqrt(2^(127)*65535/32768)
 */
#define FLOAT_SQRT_MAX_POS_SIGN     FLOAT_SIGN_POS

/*!
   \anchor FLOAT_SQRT_MAX_POS_EXP
   \addindex FLOAT_SQRT_MAX_POS_EXP
   \page bmldefFloatSqrtMaxPosExp      FLOAT_SQRT_MAX_POS_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to sqrt(2^(127)*65535/32768)
 */
#define FLOAT_SQRT_MAX_POS_EXP      (63)

/*!
   \anchor FLOAT_SQRT_MAX_POS_MAN
   \addindex FLOAT_SQRT_MAX_POS_MAN
   \page bmldefFloatSqrtMaxPosMan      FLOAT_SQRT_MAX_POS_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to sqrt(2^(127)*65535/32768)
 */
#define FLOAT_SQRT_MAX_POS_MAN      FLOAT_MAX_MAN

/*!
   \anchor FLOAT_SQRT_MAX_POS
   \addindex FLOAT_SQRT_MAX_POS
   \page bmldefFloatSqrtMaxPos         FLOAT_SQRT_MAX_POS
   \details Constant representing the value of a floating number, equal to sqrt(2^(127)*65535/32768)
 */
#define FLOAT_SQRT_MAX_POS          {FLOAT_SQRT_MAX_POS_SIGN, FLOAT_SQRT_MAX_POS_EXP, FLOAT_SQRT_MAX_POS_MAN}

/*!
   \anchor FLOAT_MAX_POS_SIGN
   \addindex FLOAT_MAX_POS_SIGN
   \page bmldefFloatMaxPosSign     FLOAT_MAX_POS_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to 2^(127)*65535/32768
 */
#define FLOAT_MAX_POS_SIGN      FLOAT_SIGN_POS

/*!
   \anchor FLOAT_MAX_POS_EXP
   \addindex FLOAT_MAX_POS_EXP
   \page bmldefFloatMaxPosExp      FLOAT_MAX_POS_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to 2^(127)*65535/32768
 */
#define FLOAT_MAX_POS_EXP       (FLOAT_MAX_EXP)

/*!
   \anchor FLOAT_MAX_POS_MAN
   \addindex FLOAT_MAX_POS_MAN
   \page bmldefFloatMaxPosMan      FLOAT_MAX_POS_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to 2^(127)*65535/32768
 */
#define FLOAT_MAX_POS_MAN       FLOAT_MAX_MAN

/*!
   \anchor FLOAT_MAX_POS
   \addindex FLOAT_MAX_POS
   \page bmldefFloatMaxPos         FLOAT_MAX_POS
   \details Constant representing the value of a floating number, equal to 2^(127)*65535/32768
 */
#define FLOAT_MAX_POS           {FLOAT_MAX_POS_SIGN, FLOAT_MAX_POS_EXP, FLOAT_MAX_POS_MAN}

/***************************************************************************************
*                       negative pFloat numbers
***************************************************************************************/
/*!
   \anchor FLOAT_MIN_NEG_SIGN
   \addindex FLOAT_MIN_NEG_SIGN
   \page bmldefFloatMinNegSign     FLOAT_MIN_NEG_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1*2^(-127)
 */
#define FLOAT_MIN_NEG_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MIN_NEG_EXP
   \addindex FLOAT_MIN_NEG_EXP
   \page bmldefFloatMinNegExp     FLOAT_MIN_NEG_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1*2^(-127)
 */
#define FLOAT_MIN_NEG_EXP       (FLOAT_MIN_EXP)

/*!
   \anchor FLOAT_MIN_NEG_MAN
   \addindex FLOAT_MIN_NEG_MAN
   \page bmldefFloatMinNegMan     FLOAT_MIN_NEG_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1*2^(-127)
 */
#define FLOAT_MIN_NEG_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MIN_NEG
   \addindex FLOAT_MIN_NEG
   \page bmldefFloatMinNeg     FLOAT_MIN_NEG
   \details Constant representing the value of a floating number, equal to -1*2^(-127)
 */
#define FLOAT_MIN_NEG           {FLOAT_MIN_NEG_SIGN, FLOAT_MIN_NEG_EXP, FLOAT_MIN_NEG_MAN}

/*!
   \anchor FLOAT_MIN_NEG_MUL2_SIGN
   \addindex FLOAT_MIN_NEG_MUL2_SIGN
   \page bmldefFloatMinNegMul2Sign     FLOAT_MIN_NEG_MUL2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1*2^(-127)*2
 */
#define FLOAT_MIN_NEG_MUL2_SIGN     FLOAT_MIN_NEG_SIGN

/*!
   \anchor FLOAT_MIN_NEG_MUL2_EXP
   \addindex FLOAT_MIN_NEG_MUL2_EXP
   \page bmldefFloatMinNegMul2Exp     FLOAT_MIN_NEG_MUL2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1*2^(-127)*2
 */
#define FLOAT_MIN_NEG_MUL2_EXP      (-126)

/*!
   \anchor FLOAT_MIN_NEG_MUL2_MAN
   \addindex FLOAT_MIN_NEG_MUL2_MAN
   \page bmldefFloatMinNegMul2Man     FLOAT_MIN_NEG_MUL2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1*2^(-127)*2
 */
#define FLOAT_MIN_NEG_MUL2_MAN      FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MIN_NEG_MUL2
   \addindex FLOAT_MIN_NEG_MUL2
   \page bmldefFloatMinNegMul2     FLOAT_MIN_NEG_MUL2
   \details Constant representing the value of a floating number, equal to -1*2^(-127)*2
 */
#define FLOAT_MIN_NEG_MUL2          {FLOAT_MIN_NEG_MUL2_SIGN, FLOAT_MIN_NEG_MUL2_EXP, FLOAT_MIN_NEG_MUL2_MAN}

/*!
   \anchor FLOAT_MIN_NEG_MUL3_SIGN
   \addindex FLOAT_MIN_NEG_MUL3_SIGN
   \page bmldefFloatMinNegMul3Sign     FLOAT_MIN_NEG_MUL3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1*2^(-127)*3
 */
#define FLOAT_MIN_NEG_MUL3_SIGN  FLOAT_MIN_NEG_SIGN

/*!
   \anchor FLOAT_MIN_NEG_MUL3_EXP
   \addindex FLOAT_MIN_NEG_MUL3_EXP
   \page bmldefFloatMinNegMul3Exp     FLOAT_MIN_NEG_MUL3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1*2^(-127)*3
 */
#define FLOAT_MIN_NEG_MUL3_EXP      (-126)

/*!
   \anchor FLOAT_MIN_NEG_MUL3_MAN
   \addindex FLOAT_MIN_NEG_MUL3_MAN
   \page bmldefFloatMinNegMul3Man     FLOAT_MIN_NEG_MUL3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1*2^(-127)*3
 */
#define FLOAT_MIN_NEG_MUL3_MAN      49152

/*!
   \anchor FLOAT_MIN_NEG_MUL3
   \addindex FLOAT_MIN_NEG_MUL3
   \page bmldefFloatMinNegMul3     FLOAT_MIN_NEG_MUL3
   \details Constant representing the value of a floating number, equal to -1*2^(-127)*3
 */
#define FLOAT_MIN_NEG_MUL3          {FLOAT_MIN_NEG_MUL3_SIGN, FLOAT_MIN_NEG_MUL3_EXP, FLOAT_MIN_NEG_MUL3_MAN}

/*!
   \anchor FLOAT_MINUS_0_01_SIGN
   \addindex FLOAT_MINUS_0_01_SIGN
   \page bmldefFloatNeg0_01_Sign     FLOAT_MINUS_0_01_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.01
 */
#define FLOAT_MINUS_0_01_SIGN   FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_01_EXP
   \addindex FLOAT_MINUS_0_01_EXP
   \page bmldefFloatNeg0_01_Exp     FLOAT_MINUS_0_01_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.01
 */
#define FLOAT_MINUS_0_01_EXP    (-7)

/*!
   \anchor FLOAT_MINUS_0_01_MAN
   \addindex FLOAT_MINUS_0_01_MAN
   \page bmldefFloatNeg0_01_Man     FLOAT_MINUS_0_01_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.01
 */
#define FLOAT_MINUS_0_01_MAN    41943

/*!
   \anchor FLOAT_MINUS_0_01
   \addindex FLOAT_MINUS_0_01
   \page bmldefFloatNeg0_01    FLOAT_MINUS_0_01
   \details Constant representing the value of a floating number, equal to -0.01
 */
#define FLOAT_MINUS_0_01        {FLOAT_MINUS_0_01_SIGN, FLOAT_MINUS_0_01_EXP,FLOAT_MINUS_0_01_MAN}

/*!
   \anchor FLOAT_MINUS_0_1_SIGN
   \addindex FLOAT_MINUS_0_1_SIGN
   \page bmldefFloatNeg0_1_Sign     FLOAT_MINUS_0_1_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.1
 */
#define FLOAT_MINUS_0_1_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_1_EXP
   \addindex FLOAT_MINUS_0_1_EXP
   \page bmldefFloatNeg0_1_Exp     FLOAT_MINUS_0_1_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.1
 */
#define FLOAT_MINUS_0_1_EXP     (-4)

/*!
   \anchor FLOAT_MINUS_0_1_MAN
   \addindex FLOAT_MINUS_0_1_MAN
   \page bmldefFloatNeg0_1_Man     FLOAT_MINUS_0_1_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.1
 */
#define FLOAT_MINUS_0_1_MAN     52428

/*!
   \anchor FLOAT_MINUS_0_1
   \addindex FLOAT_MINUS_0_1
   \page bmldefFloatNeg0_1    FLOAT_MINUS_0_1
   \details Constant representing the value of a floating number, equal to -0.1
 */
#define FLOAT_MINUS_0_1         {FLOAT_MINUS_0_1_SIGN, FLOAT_MINUS_0_1_EXP,FLOAT_MINUS_0_1_MAN}

/*!
   \anchor FLOAT_MINUS_0_2_SIGN
   \addindex FLOAT_MINUS_0_2_SIGN
   \page bmldefFloatNeg0_2_Sign     FLOAT_MINUS_0_2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.2
 */
#define FLOAT_MINUS_0_2_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_2_EXP
   \addindex FLOAT_MINUS_0_2_EXP
   \page bmldefFloatNeg0_2_Exp     FLOAT_MINUS_0_2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.2
 */
#define FLOAT_MINUS_0_2_EXP     (-3)

/*!
   \anchor FLOAT_MINUS_0_2_MAN
   \addindex FLOAT_MINUS_0_2_MAN
   \page bmldefFloatNeg0_2_Man     FLOAT_MINUS_0_2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.2
 */
#define FLOAT_MINUS_0_2_MAN     52428

/*!
   \anchor FLOAT_MINUS_0_2
   \addindex FLOAT_MINUS_0_2
   \page bmldefFloatNeg0_2    FLOAT_MINUS_0_2
   \details Constant representing the value of a floating number, equal to -0.2
 */
#define FLOAT_MINUS_0_2         {FLOAT_MINUS_0_2_SIGN, FLOAT_MINUS_0_2_EXP, FLOAT_MINUS_0_2_MAN}

/*!
   \anchor FLOAT_MINUS_0_25_SIGN
   \addindex FLOAT_MINUS_0_25_SIGN
   \page bmldefFloatNeg0_25_Sign     FLOAT_MINUS_0_25_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.25
 */
#define FLOAT_MINUS_0_25_SIGN       FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_25_EXP
   \addindex FLOAT_MINUS_0_25_EXP
   \page bmldefFloatNeg0_25_Exp     FLOAT_MINUS_0_25_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.25
 */
#define FLOAT_MINUS_0_25_EXP        (-2)

/*!
   \anchor FLOAT_MINUS_0_25_MAN
   \addindex FLOAT_MINUS_0_25_MAN
   \page bmldefFloatNeg0_25_Man     FLOAT_MINUS_0_25_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.25
 */
#define FLOAT_MINUS_0_25_MAN        32768

/*!
   \anchor FLOAT_MINUS_0_25
   \addindex FLOAT_MINUS_0_25
   \page bmldefFloatNeg0_25    FLOAT_MINUS_0_25
   \details Constant representing the value of a floating number, equal to -0.25
 */
#define FLOAT_MINUS_0_25            {FLOAT_MINUS_0_25_SIGN, FLOAT_MINUS_0_25_EXP, FLOAT_MINUS_0_25_MAN}

/*!
   \anchor FLOAT_MINUS_0_3_SIGN
   \addindex FLOAT_MINUS_0_3_SIGN
   \page bmldefFloatNeg0_3_Sign     FLOAT_MINUS_0_3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.3
 */
#define FLOAT_MINUS_0_3_SIGN        FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_3_EXP
   \addindex FLOAT_MINUS_0_3_EXP
   \page bmldefFloatNeg0_3_Exp     FLOAT_MINUS_0_3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.3
 */
#define FLOAT_MINUS_0_3_EXP     (-2)

/*!
   \anchor FLOAT_MINUS_0_3_MAN
   \addindex FLOAT_MINUS_0_3_MAN
   \page bmldefFloatNeg0_3_Man     FLOAT_MINUS_0_3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.3
 */
#define FLOAT_MINUS_0_3_MAN     39322

/*!
   \anchor FLOAT_MINUS_0_3
   \addindex FLOAT_MINUS_0_3
   \page bmldefFloatNeg0_3    FLOAT_MINUS_0_3
   \details Constant representing the value of a floating number, equal to -0.3
 */
#define FLOAT_MINUS_0_3         {FLOAT_MINUS_0_3_SIGN, FLOAT_MINUS_0_3_EXP, FLOAT_MINUS_0_3_MAN}

/*!
   \anchor FLOAT_MINUS_0_33_SIGN
   \addindex FLOAT_MINUS_0_33_SIGN
   \page bmldefFloatNeg0_33_Sign     FLOAT_MINUS_0_33_SIGN
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.33
 */
#define FLOAT_MINUS_0_33_SIGN   FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_33_EXP
   \addindex FLOAT_MINUS_0_33_EXP
   \page bmldefFloatNeg0_33_Exp     FLOAT_MINUS_0_33_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.33
 */
#define FLOAT_MINUS_0_33_EXP    (-2)

/*!
   \anchor FLOAT_MINUS_0_33_MAN
   \addindex FLOAT_MINUS_0_33_MAN
   \page bmldefFloatNeg0_33_Man     FLOAT_MINUS_0_33_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.33
 */
#define FLOAT_MINUS_0_33_MAN    43691

/*!
   \anchor FLOAT_MINUS_0_33
   \addindex FLOAT_MINUS_0_33
   \page bmldefFloatNeg0_33    FLOAT_MINUS_0_33
   \details Constant representing the value of a floating number, equal to -0.33
 */
#define FLOAT_MINUS_0_33        {FLOAT_MINUS_0_33_SIGN, FLOAT_MINUS_0_33_EXP, FLOAT_MINUS_0_33_MAN}

/*!
   \anchor FLOAT_MINUS_0_5_SIGN
   \addindex FLOAT_MINUS_0_5_SIGN
   \page bmldefFloatNeg0_5_Sign     FLOAT_MINUS_0_5_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.5
 */
#define FLOAT_MINUS_0_5_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_5_EXP
   \addindex FLOAT_MINUS_0_5_EXP
   \page bmldefFloatNeg0_5_Exp     FLOAT_MINUS_0_5_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.5
 */
#define FLOAT_MINUS_0_5_EXP     (-1)

/*!
   \anchor FLOAT_MINUS_0_5_MAN
   \addindex FLOAT_MINUS_0_5_MAN
   \page bmldefFloatNeg0_5_Man     FLOAT_MINUS_0_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.5
 */
#define FLOAT_MINUS_0_5_MAN     FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_0_5
   \addindex FLOAT_MINUS_0_5
   \page bmldefFloatNeg0_5    FLOAT_MINUS_0_5
   \details Constant representing the value of a floating number, equal to -0.5
 */
#define FLOAT_MINUS_0_5         {FLOAT_MINUS_0_5_SIGN, FLOAT_MINUS_0_5_EXP, FLOAT_MINUS_0_5_MAN}

/*!
   \anchor FLOAT_MINUS_0_6_SIGN
   \addindex FLOAT_MINUS_0_6_SIGN
   \page bmldefFloatNeg0_6_Sign     FLOAT_MINUS_0_6_SIGN
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.6
 */
#define FLOAT_MINUS_0_6_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_6_EXP
   \addindex FLOAT_MINUS_0_6_EXP
   \page bmldefFloatNeg0_6_Exp     FLOAT_MINUS_0_6_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.6
 */
#define FLOAT_MINUS_0_6_EXP     (-1)

/*!
   \anchor FLOAT_MINUS_0_6_MAN
   \addindex FLOAT_MINUS_0_6_MAN
   \page bmldefFloatNeg0_6_Man     FLOAT_MINUS_0_6_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.6
 */
#define FLOAT_MINUS_0_6_MAN     39322

/*!
   \anchor FLOAT_MINUS_0_6
   \addindex FLOAT_MINUS_0_6
   \page bmldefFloatNeg0_6    FLOAT_MINUS_0_6
   \details Constant representing the value of a floating number, equal to -0.6
 */
#define FLOAT_MINUS_0_6         {FLOAT_MINUS_0_6_SIGN, FLOAT_MINUS_0_6_EXP, FLOAT_MINUS_0_6_MAN}

/*!
   \anchor FLOAT_MINUS_0_66_SIGN
   \addindex FLOAT_MINUS_0_66_SIGN
   \page bmldefFloatNeg0_66_Sign     FLOAT_MINUS_0_66_SIGN
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.66
 */
#define FLOAT_MINUS_0_66_SIGN   FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_0_66_EXP
   \addindex FLOAT_MINUS_0_66_EXP
   \page bmldefFloatNeg0_66_Exp     FLOAT_MINUS_0_66_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.66
 */
#define FLOAT_MINUS_0_66_EXP    (-1)

/*!
   \anchor FLOAT_MINUS_0_66_MAN
   \addindex FLOAT_MINUS_0_66_MAN
   \page bmldefFloatNeg0_66_Man     FLOAT_MINUS_0_66_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.66
 */
#define FLOAT_MINUS_0_66_MAN    43691

/*!
   \anchor FLOAT_MINUS_0_66
   \addindex FLOAT_MINUS_0_66
   \page bmldefFloatNeg0_66   FLOAT_MINUS_0_66
   \details Constant representing the value of a floating number, equal to -0.66
 */
#define FLOAT_MINUS_0_66        {FLOAT_MINUS_0_66_SIGN, FLOAT_MINUS_0_66_EXP, FLOAT_MINUS_0_66_MAN}

/*!
   \anchor FLOAT_MINUS_1_SIGN
   \addindex FLOAT_MINUS_1_SIGN
   \page bmldefFloatNeg1_Sign     FLOAT_MINUS_1_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1.0
 */
#define FLOAT_MINUS_1_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_1_EXP
   \addindex FLOAT_MINUS_1_EXP
   \page bmldefFloatNeg1_Exp     FLOAT_MINUS_1_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1.0
 */
#define FLOAT_MINUS_1_EXP       0

/*!
   \anchor FLOAT_MINUS_1_MAN
   \addindex FLOAT_MINUS_1_MAN
   \page bmldefFloatNeg1_Man     FLOAT_MINUS_1_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1.0
 */
#define FLOAT_MINUS_1_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_1
   \addindex FLOAT_MINUS_1
   \page bmldefFloatNeg1   FLOAT_MINUS_1
   \details Constant representing the value of a floating number, equal to -1.0
 */
#define FLOAT_MINUS_1           {FLOAT_MINUS_1_SIGN, FLOAT_MINUS_1_EXP, FLOAT_MINUS_1_MAN}

/*!
   \anchor FLOAT_MINUS_1_5_SIGN
   \addindex FLOAT_MINUS_1_5_SIGN
   \page bmldefFloatNeg1_5_Sign     FLOAT_MINUS_1_5_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1.5
 */
#define FLOAT_MINUS_1_5_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_1_5_EXP
   \addindex FLOAT_MINUS_1_5_EXP
   \page bmldefFloatNeg1_5_Exp     FLOAT_MINUS_1_5_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1.5
 */
#define FLOAT_MINUS_1_5_EXP     0

/*!
   \anchor FLOAT_MINUS_1_5_MAN
   \addindex FLOAT_MINUS_1_5_MAN
   \page bmldefFloatNeg1_5_Man     FLOAT_MINUS_1_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1.5
 */
#define FLOAT_MINUS_1_5_MAN     49152

/*!
   \anchor FLOAT_MINUS_1_5
   \addindex FLOAT_MINUS_1_5
   \page bmldefFloatNeg1_5   FLOAT_MINUS_1_5
   \details Constant representing the value of a floating number, equal to -1.5
 */
#define FLOAT_MINUS_1_5         {FLOAT_MINUS_1_5_SIGN,  FLOAT_MINUS_1_5_EXP, FLOAT_MINUS_1_5_MAN}

/*!
   \anchor FLOAT_MINUS_2_SIGN
   \addindex FLOAT_MINUS_2_SIGN
   \page bmldefFloatNeg2_Sign     FLOAT_MINUS_2_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -2.0
 */
#define FLOAT_MINUS_2_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_2_EXP
   \addindex FLOAT_MINUS_2_EXP
   \page bmldefFloatNeg2_Exp     FLOAT_MINUS_2_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -2.0
 */
#define FLOAT_MINUS_2_EXP       1

/*!
   \anchor FLOAT_MINUS_2_MAN
   \addindex FLOAT_MINUS_2_MAN
   \page bmldefFloatNeg2_Man     FLOAT_MINUS_2_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -2.0
 */
#define FLOAT_MINUS_2_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_2
   \addindex FLOAT_MINUS_2
   \page bmldefFloatNeg2   FLOAT_MINUS_2
   \details Constant representing the value of a floating number, equal to -2.0
 */
#define FLOAT_MINUS_2           {FLOAT_MINUS_2_SIGN, FLOAT_MINUS_2_EXP, FLOAT_MINUS_2_MAN}

/*!
   \anchor FLOAT_MINUS_3_SIGN
   \addindex FLOAT_MINUS_3_SIGN
   \page bmldefFloatNeg3_Sign    FLOAT_MINUS_3_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -3.0
 */
#define FLOAT_MINUS_3_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_3_EXP
   \addindex FLOAT_MINUS_3_EXP
   \page bmldefFloatNeg3_Exp     FLOAT_MINUS_3_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -3.0
 */
#define FLOAT_MINUS_3_EXP       1

/*!
   \anchor FLOAT_MINUS_3_MAN
   \addindex FLOAT_MINUS_3_MAN
   \page bmldefFloatNeg3_Man     FLOAT_MINUS_3_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -3.0
 */
#define FLOAT_MINUS_3_MAN       49152

/*!
   \anchor FLOAT_MINUS_3
   \addindex FLOAT_MINUS_3
   \page bmldefFloatNeg3   FLOAT_MINUS_3
   \details Constant representing the value of a floating number, equal to -3.0
 */
#define FLOAT_MINUS_3           {FLOAT_MINUS_3_SIGN, FLOAT_MINUS_3_EXP, FLOAT_MINUS_3_MAN}

/*!
   \anchor FLOAT_MINUS_4_SIGN
   \addindex FLOAT_MINUS_4_SIGN
   \page bmldefFloatNeg4_Sign     FLOAT_MINUS_4_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -4.0
 */
#define FLOAT_MINUS_4_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_4_EXP
   \addindex FLOAT_MINUS_4_EXP
   \page bmldefFloatNeg4_Exp     FLOAT_MINUS_4_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -4.0
 */
#define FLOAT_MINUS_4_EXP       2

/*!
   \anchor FLOAT_MINUS_4_MAN
   \addindex FLOAT_MINUS_4_MAN
   \page bmldefFloatNeg4_Man     FLOAT_MINUS_4_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -4.0
 */
#define FLOAT_MINUS_4_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_4
   \addindex FLOAT_MINUS_4
   \page bmldefFloatNeg4   FLOAT_MINUS_4
   \details Constant representing the value of a floating number, equal to -4.0
 */
#define FLOAT_MINUS_4           {FLOAT_MINUS_4_SIGN, FLOAT_MINUS_4_EXP, FLOAT_MINUS_4_MAN}

/*!
   \anchor FLOAT_MINUS_5_SIGN
   \addindex FLOAT_MINUS_5_SIGN
   \page bmldefFloatNeg5_Sign     FLOAT_MINUS_5_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -5.0
 */
#define FLOAT_MINUS_5_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_5_EXP
   \addindex FLOAT_MINUS_5_EXP
   \page bmldefFloatNeg5_Exp     FLOAT_MINUS_5_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -5.0
 */
#define FLOAT_MINUS_5_EXP       2

/*!
   \anchor FLOAT_MINUS_5_MAN
   \addindex FLOAT_MINUS_5_MAN
   \page bmldefFloatNeg5_Man     FLOAT_MINUS_5_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -5.0
 */
#define FLOAT_MINUS_5_MAN       40960

/*!
   \anchor FLOAT_MINUS_5
   \addindex FLOAT_MINUS_5
   \page bmldefFloatNeg5   FLOAT_MINUS_5
   \details Constant representing the value of a floating number, equal to -5.0
 */
#define FLOAT_MINUS_5           {FLOAT_MINUS_5_SIGN, FLOAT_MINUS_5_EXP, FLOAT_MINUS_5_MAN}

/*!
   \anchor FLOAT_MINUS_6_SIGN
   \addindex FLOAT_MINUS_6_SIGN
   \page bmldefFloatNeg6_Sign     FLOAT_MINUS_6_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -6.0
 */
#define FLOAT_MINUS_6_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_6_EXP
   \addindex FLOAT_MINUS_6_EXP
   \page bmldefFloatNeg6_Exp     FLOAT_MINUS_6_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -6.0
 */
#define FLOAT_MINUS_6_EXP       2

/*!
   \anchor FLOAT_MINUS_6_MAN
   \addindex FLOAT_MINUS_6_MAN
   \page bmldefFloatNeg6_Man     FLOAT_MINUS_6_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -6.0
 */
#define FLOAT_MINUS_6_MAN       49152

/*!
   \anchor FLOAT_MINUS_6
   \addindex FLOAT_MINUS_6
   \page bmldefFloatNeg6   FLOAT_MINUS_6
   \details Constant representing the value of a floating number, equal to -6.0
 */
#define FLOAT_MINUS_6           {FLOAT_MINUS_6_SIGN, FLOAT_MINUS_6_EXP, FLOAT_MINUS_6_MAN}

/*!
   \anchor FLOAT_MINUS_7_SIGN
   \addindex FLOAT_MINUS_7_SIGN
   \page bmldefFloatNeg7_Sign     FLOAT_MINUS_7_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -7.0
 */
#define FLOAT_MINUS_7_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_7_EXP
   \addindex FLOAT_MINUS_7_EXP
   \page bmldefFloatNeg7_Exp     FLOAT_MINUS_7_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -7.0
 */
#define FLOAT_MINUS_7_EXP       2

/*!
   \anchor FLOAT_MINUS_7_MAN
   \addindex FLOAT_MINUS_7_MAN
   \page bmldefFloatNeg7_Man     FLOAT_MINUS_7_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -7.0
 */
#define FLOAT_MINUS_7_MAN       57344

/*!
   \anchor FLOAT_MINUS_7
   \addindex FLOAT_MINUS_7
   \page bmldefFloatNeg7   FLOAT_MINUS_7
   \details Constant representing the value of a floating number, equal to -7.0
 */
#define FLOAT_MINUS_7           {FLOAT_MINUS_7_SIGN, FLOAT_MINUS_7_EXP, FLOAT_MINUS_7_MAN}

/*!
   \anchor FLOAT_MINUS_8_SIGN
   \addindex FLOAT_MINUS_8_SIGN
   \page bmldefFloatNeg8_Sign     FLOAT_MINUS_8_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -8.0
 */
#define FLOAT_MINUS_8_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_8_EXP
   \addindex FLOAT_MINUS_8_EXP
   \page bmldefFloatNeg8_Exp     FLOAT_MINUS_8_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -8.0
 */
#define FLOAT_MINUS_8_EXP       3

/*!
   \anchor FLOAT_MINUS_8_MAN
   \addindex FLOAT_MINUS_8_MAN
   \page bmldefFloatNeg8_Man     FLOAT_MINUS_8_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -8.0
 */
#define FLOAT_MINUS_8_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_8
   \addindex FLOAT_MINUS_8
   \page bmldefFloatNeg8   FLOAT_MINUS_8
   \details Constant representing the value of a floating number, equal to -8.0
 */
#define FLOAT_MINUS_8           {FLOAT_MINUS_8_SIGN, FLOAT_MINUS_8_EXP, FLOAT_MINUS_8_MAN}

/*!
   \anchor FLOAT_MINUS_9_SIGN
   \addindex FLOAT_MINUS_9_SIGN
   \page bmldefFloatNeg9_Sign     FLOAT_MINUS_9_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -9.0
 */
#define FLOAT_MINUS_9_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_9_EXP
   \addindex FLOAT_MINUS_9_EXP
   \page bmldefFloatNeg9_Exp     FLOAT_MINUS_9_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -9.0
 */
#define FLOAT_MINUS_9_EXP       3

/*!
   \anchor FLOAT_MINUS_9_MAN
   \addindex FLOAT_MINUS_9_MAN
   \page bmldefFloatNeg9_Man     FLOAT_MINUS_9_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -9.0
 */
#define FLOAT_MINUS_9_MAN       36864

/*!
   \anchor FLOAT_MINUS_9
   \addindex FLOAT_MINUS_9
   \page bmldefFloatNeg9   FLOAT_MINUS_9
   \details Constant representing the value of a floating number, equal to -9.0
 */
#define FLOAT_MINUS_9           {FLOAT_MINUS_9_SIGN, FLOAT_MINUS_9_EXP, FLOAT_MINUS_9_MAN}

/*!
   \anchor FLOAT_MINUS_9_99_SIGN
   \addindex FLOAT_MINUS_9_99_SIGN
   \page bmldefFloatNeg9_99_Sign     FLOAT_MINUS_9_99_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -9.99
 */
#define FLOAT_MINUS_9_99_SIGN       FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_9_99_EXP
   \addindex FLOAT_MINUS_9_99_EXP
   \page bmldefFloatNeg9_99_Exp     FLOAT_MINUS_9_99_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -9.99
 */
#define FLOAT_MINUS_9_99_EXP        3

/*!
   \anchor FLOAT_MINUS_9_99_MAN
   \addindex FLOAT_MINUS_9_99_MAN
   \page bmldefFloatNeg9_99_Man     FLOAT_MINUS_9_99_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -9.99
 */
#define FLOAT_MINUS_9_99_MAN        40919

/*!
   \anchor FLOAT_MINUS_9_99
   \addindex FLOAT_MINUS_9_99
   \page bmldefFloatNeg9_99   FLOAT_MINUS_9_99
   \details Constant representing the value of a floating number, equal to -9.99
 */
#define FLOAT_MINUS_9_99            {FLOAT_MINUS_9_99_SIGN, FLOAT_MINUS_9_99_EXP, FLOAT_MINUS_9_99_MAN}

/*!
   \anchor FLOAT_MINUS_10_SIGN
   \addindex FLOAT_MINUS_10_SIGN
   \page bmldefFloatNeg10_Sign     FLOAT_MINUS_10_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -10.0
 */
#define FLOAT_MINUS_10_SIGN     FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_10_EXP
   \addindex FLOAT_MINUS_10_EXP
   \page bmldefFloatNeg10_Exp     FLOAT_MINUS_10_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -10.0
 */
#define FLOAT_MINUS_10_EXP      3

/*!
   \anchor FLOAT_MINUS_10_MAN
   \addindex FLOAT_MINUS_10_MAN
   \page bmldefFloatNeg10_Man     FLOAT_MINUS_10_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -10.0
 */
#define FLOAT_MINUS_10_MAN      40960

/*!
   \anchor FLOAT_MINUS_10
   \addindex FLOAT_MINUS_10
   \page bmldefFloatNeg10   FLOAT_MINUS_10
   \details Constant representing the value of a floating number, equal to -10.0
 */
#define FLOAT_MINUS_10          {FLOAT_MINUS_10_SIGN, FLOAT_MINUS_10_EXP, FLOAT_MINUS_10_MAN}

/*!
   \anchor FLOAT_MINUS_20_SIGN
   \addindex FLOAT_MINUS_20_SIGN
   \page bmldefFloatNeg20_Sign     FLOAT_MINUS_20_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -20.0
 */
#define FLOAT_MINUS_20_SIGN     FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_20_EXP
   \addindex FLOAT_MINUS_20_EXP
   \page bmldefFloatNeg20_Exp     FLOAT_MINUS_20_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -20.0
 */
#define FLOAT_MINUS_20_EXP      4

/*!
   \anchor FLOAT_MINUS_20_MAN
   \addindex FLOAT_MINUS_20_MAN
   \page bmldefFloatNeg20_Man     FLOAT_MINUS_20_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -20.0
 */
#define FLOAT_MINUS_20_MAN      40960

/*!
   \anchor FLOAT_MINUS_20
   \addindex FLOAT_MINUS_20
   \page bmldefFloatNeg20   FLOAT_MINUS_20
   \details Constant representing the value of a floating number, equal to -20.0
 */
#define FLOAT_MINUS_20          {FLOAT_MINUS_20_SIGN, FLOAT_MINUS_20_EXP, FLOAT_MINUS_20_MAN}

/*!
   \anchor FLOAT_MINUS_40_SIGN
   \addindex FLOAT_MINUS_40_SIGN
   \page bmldefFloatNeg40_Sign     FLOAT_MINUS_40_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -40.0
 */
#define FLOAT_MINUS_40_SIGN FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_40_EXP
   \addindex FLOAT_MINUS_40_EXP
   \page bmldefFloatNeg40_Exp     FLOAT_MINUS_40_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -40.0
 */
#define FLOAT_MINUS_40_EXP  5

/*!
   \anchor FLOAT_MINUS_40_MAN
   \addindex FLOAT_MINUS_40_MAN
   \page bmldefFloatNeg40_Man     FLOAT_MINUS_40_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -40.0
 */
#define FLOAT_MINUS_40_MAN  40960

/*!
   \anchor FLOAT_MINUS_40
   \addindex FLOAT_MINUS_40
   \page bmldefFloatNeg40   FLOAT_MINUS_40
   \details Constant representing the value of a floating number, equal to -40.0
 */
#define FLOAT_MINUS_40      {FLOAT_MINUS_40_SIGN, FLOAT_MINUS_40_EXP, FLOAT_MINUS_40_MAN}

/*!
   \anchor FLOAT_MINUS_50_SIGN
   \addindex FLOAT_MINUS_50_SIGN
   \page bmldefFloatNeg50_Sign     FLOAT_MINUS_50_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -50.0
 */
#define FLOAT_MINUS_50_SIGN     FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_50_EXP
   \addindex FLOAT_MINUS_50_EXP
   \page bmldefFloatNeg50_Exp     FLOAT_MINUS_50_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -50.0
 */
#define FLOAT_MINUS_50_EXP      5

/*!
   \anchor FLOAT_MINUS_50_MAN
   \addindex FLOAT_MINUS_50_MAN
   \page bmldefFloatNeg50_Man     FLOAT_MINUS_50_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -50.0
 */
#define FLOAT_MINUS_50_MAN      51200

/*!
   \anchor FLOAT_MINUS_50
   \addindex FLOAT_MINUS_50
   \page bmldefFloatNeg50   FLOAT_MINUS_50
   \details Constant representing the value of a floating number, equal to -50.0
 */
#define FLOAT_MINUS_50          {FLOAT_MINUS_50_SIGN, FLOAT_MINUS_50_EXP, FLOAT_MINUS_50_MAN}

/*!
   \anchor FLOAT_MINUS_100_SIGN
   \addindex FLOAT_MINUS_100_SIGN
   \page bmldefFloatNeg100_Sign     FLOAT_MINUS_100_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -100.0
 */
#define FLOAT_MINUS_100_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_100_EXP
   \addindex FLOAT_MINUS_100_EXP
   \page bmldefFloatNeg100_Exp     FLOAT_MINUS_100_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -100.0
 */
#define FLOAT_MINUS_100_EXP     6

/*!
   \anchor FLOAT_MINUS_100_MAN
   \addindex FLOAT_MINUS_100_MAN
   \page bmldefFloatNeg100_Man     FLOAT_MINUS_100_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -100.0
 */
#define FLOAT_MINUS_100_MAN     51200

/*!
   \anchor FLOAT_MINUS_100
   \addindex FLOAT_MINUS_100
   \page bmldefFloatNeg100   FLOAT_MINUS_100
   \details Constant representing the value of a floating number, equal to -100.0
 */
#define FLOAT_MINUS_100         {FLOAT_MINUS_100_SIGN, FLOAT_MINUS_100_EXP, FLOAT_MINUS_100_MAN}

/*!
   \anchor FLOAT_MINUS_150_SIGN
   \addindex FLOAT_MINUS_150_SIGN
   \page bmldefFloatNeg150_Sign     FLOAT_MINUS_150_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -150.0
 */
#define FLOAT_MINUS_150_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_150_EXP
   \addindex FLOAT_MINUS_150_EXP
   \page bmldefFloatNeg150_Exp     FLOAT_MINUS_150_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -150.0
 */
#define FLOAT_MINUS_150_EXP     7

/*!
   \anchor FLOAT_MINUS_150_MAN
   \addindex FLOAT_MINUS_150_MAN
   \page bmldefFloatNeg150_Man     FLOAT_MINUS_150_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -150.0
 */
#define FLOAT_MINUS_150_MAN     38400

/*!
   \anchor FLOAT_MINUS_150
   \addindex FLOAT_MINUS_150
   \page bmldefFloatNeg150   FLOAT_MINUS_150
   \details Constant representing the value of a floating number, equal to -150.0
 */
#define FLOAT_MINUS_150         {FLOAT_MINUS_150_SIGN, FLOAT_MINUS_150_EXP, FLOAT_MINUS_150_MAN}

/*!
   \anchor FLOAT_MINUS_200_SIGN
   \addindex FLOAT_MINUS_200_SIGN
   \page bmldefFloatNeg200_Sign     FLOAT_MINUS_200_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -200.0
 */
#define FLOAT_MINUS_200_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_200_EXP
   \addindex FLOAT_MINUS_200_EXP
   \page bmldefFloatNeg200_Exp     FLOAT_MINUS_200_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -200.0
 */
#define FLOAT_MINUS_200_EXP     7

/*!
   \anchor FLOAT_MINUS_200_MAN
   \addindex FLOAT_MINUS_200_MAN
   \page bmldefFloatNeg200_Man     FLOAT_MINUS_200_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -200.0
 */
#define FLOAT_MINUS_200_MAN     51200

/*!
   \anchor FLOAT_MINUS_200
   \addindex FLOAT_MINUS_200
   \page bmldefFloatNeg200   FLOAT_MINUS_200
   \details Constant representing the value of a floating number, equal to -200.0
 */
#define FLOAT_MINUS_200         {FLOAT_MINUS_200_SIGN, FLOAT_MINUS_200_EXP, FLOAT_MINUS_200_MAN}

/*!
   \anchor FLOAT_MINUS_254_SIGN
   \addindex FLOAT_MINUS_254_SIGN
   \page bmldefFloatNeg254_Sign     FLOAT_MINUS_254_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -254.0
 */
#define FLOAT_MINUS_254_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_254_EXP
   \addindex FLOAT_MINUS_254_EXP
   \page bmldefFloatNeg254_Exp     FLOAT_MINUS_254_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -254.0
 */
#define FLOAT_MINUS_254_EXP     7

/*!
   \anchor FLOAT_MINUS_254_MAN
   \addindex FLOAT_MINUS_254_MAN
   \page bmldefFloatNeg254_Man     FLOAT_MINUS_254_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -254.0
 */
#define FLOAT_MINUS_254_MAN     65024

/*!
   \anchor FLOAT_MINUS_254
   \addindex FLOAT_MINUS_254
   \page bmldefFloatNeg254   FLOAT_MINUS_254
   \details Constant representing the value of a floating number, equal to -254.0
 */
#define FLOAT_MINUS_254         {FLOAT_MINUS_254_SIGN, FLOAT_MINUS_254_EXP, FLOAT_MINUS_254_MAN}

/*!
   \anchor FLOAT_MINUS_255_SIGN
   \addindex FLOAT_MINUS_255_SIGN
   \page bmldefFloatNeg255_Sign     FLOAT_MINUS_255_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -255.0
 */
#define FLOAT_MINUS_255_SIGN    FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_255_EXP
   \addindex FLOAT_MINUS_255_EXP
   \page bmldefFloatNeg255_Exp     FLOAT_MINUS_255_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -255.0
 */
#define FLOAT_MINUS_255_EXP     7

/*!
   \anchor FLOAT_MINUS_255_MAN
   \addindex FLOAT_MINUS_255_MAN
   \page bmldefFloatNeg255_Man     FLOAT_MINUS_255_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -255.0
 */
#define FLOAT_MINUS_255_MAN     65280

/*!
   \anchor FLOAT_MINUS_255
   \addindex FLOAT_MINUS_255
   \page bmldefFloatNeg255   FLOAT_MINUS_255
   \details Constant representing the value of a floating number, equal to -255.0
 */
#define FLOAT_MINUS_255         {FLOAT_MINUS_255_SIGN, FLOAT_MINUS_255_EXP, FLOAT_MINUS_255_MAN}

/*!
   \anchor FLOAT_MINUS_1000_SIGN
   \addindex FLOAT_MINUS_1000_SIGN
   \page bmldefFloatNeg1000_Sign     FLOAT_MINUS_1000_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1000.0
 */
#define FLOAT_MINUS_1000_SIGN   FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_1000_EXP
   \addindex FLOAT_MINUS_1000_EXP
   \page bmldefFloatNeg1000_Exp     FLOAT_MINUS_1000_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1000.0
 */
#define FLOAT_MINUS_1000_EXP    9

/*!
   \anchor FLOAT_MINUS_1000_MAN
   \addindex FLOAT_MINUS_1000_MAN
   \page bmldefFloatNeg1000_Man     FLOAT_MINUS_1000_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1000.0
 */
#define FLOAT_MINUS_1000_MAN    64000

/*!
   \anchor FLOAT_MINUS_1000
   \addindex FLOAT_MINUS_1000
   \page bmldefFloatNeg1000   FLOAT_MINUS_1000
   \details Constant representing the value of a floating number, equal to -1000.0
 */
#define FLOAT_MINUS_1000        {FLOAT_MINUS_1000_SIGN, FLOAT_MINUS_1000_EXP, FLOAT_MINUS_1000_MAN}

/*!
   \anchor FLOAT_MINUS_32767_SIGN
   \addindex FLOAT_MINUS_32767_SIGN
   \page bmldefFloatNeg32767_Sign     FLOAT_MINUS_32767_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -32767.0
 */
#define FLOAT_MINUS_32767_SIGN  FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_32767_EXP
   \addindex FLOAT_MINUS_32767_EXP
   \page bmldefFloatNeg32767_Exp     FLOAT_MINUS_32767_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -32767.0
 */
#define FLOAT_MINUS_32767_EXP   14

/*!
   \anchor FLOAT_MINUS_32767_MAN
   \addindex FLOAT_MINUS_32767_MAN
   \page bmldefFloatNeg32767_Man     FLOAT_MINUS_32767_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -32767.0
 */
#define FLOAT_MINUS_32767_MAN   65534

/*!
   \anchor FLOAT_MINUS_32767
   \addindex FLOAT_MINUS_32767
   \page bmldefFloatNeg32767   FLOAT_MINUS_32767
   \details Constant representing the value of a floating number, equal to -32767.0
 */
#define FLOAT_MINUS_32767       {FLOAT_MINUS_32767_SIGN, FLOAT_MINUS_32767_EXP, FLOAT_MINUS_32767_MAN}

/*!
   \anchor FLOAT_MINUS_32768_SIGN
   \addindex FLOAT_MINUS_32768_SIGN
   \page bmldefFloatNeg32768_Sign     FLOAT_MINUS_32768_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -32768.0
 */
#define FLOAT_MINUS_32768_SIGN  FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_32768_EXP
   \addindex FLOAT_MINUS_32768_EXP
   \page bmldefFloatNeg32768_Exp     FLOAT_MINUS_32768_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -32768.0
 */
#define FLOAT_MINUS_32768_EXP   15

/*!
   \anchor FLOAT_MINUS_32768_MAN
   \addindex FLOAT_MINUS_32768_MAN
   \page bmldefFloatNeg32768_Man     FLOAT_MINUS_32768_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -32768.0
 */
#define FLOAT_MINUS_32768_MAN   FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_32768
   \addindex FLOAT_MINUS_32768
   \page bmldefFloatNeg32768   FLOAT_MINUS_32768
   \details Constant representing the value of a floating number, equal to -32768.0
 */
#define FLOAT_MINUS_32768       {FLOAT_MINUS_32768_SIGN, FLOAT_MINUS_32768_EXP, FLOAT_MINUS_32768_MAN}

/*!
   \anchor FLOAT_MINUS_65535_SIGN
   \addindex FLOAT_MINUS_65535_SIGN
   \page bmldefFloatNeg65535_Sign     FLOAT_MINUS_65535_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -65535.0
 */
#define FLOAT_MINUS_65535_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_65535_EXP
   \addindex FLOAT_MINUS_65535_EXP
   \page bmldefFloatNeg65535_Exp     FLOAT_MINUS_65535_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -65535.0
 */
#define FLOAT_MINUS_65535_EXP       15

/*!
   \anchor FLOAT_MINUS_65535_MAN
   \addindex FLOAT_MINUS_65535_MAN
   \page bmldefFloatNeg65535_Man     FLOAT_MINUS_65535_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -65535.0
 */
#define FLOAT_MINUS_65535_MAN       FLOAT_MAX_MAN

/*!
   \anchor FLOAT_MINUS_65535
   \addindex FLOAT_MINUS_65535
   \page bmldefFloatNeg65535   FLOAT_MINUS_65535
   \details Constant representing the value of a floating number, equal to -65535.0
 */
#define FLOAT_MINUS_65535           {FLOAT_MINUS_65535_SIGN, FLOAT_MINUS_65535_EXP, FLOAT_MINUS_65535_MAN}

/*!
   \anchor FLOAT_MINUS_65536_SIGN
   \addindex FLOAT_MINUS_65536_SIGN
   \page bmldefFloatNeg65536_Sign     FLOAT_MINUS_65536_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -65536.0
 */
#define FLOAT_MINUS_65536_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MINUS_65536_EXP
   \addindex FLOAT_MINUS_65536_EXP
   \page bmldefFloatNeg65536_Exp     FLOAT_MINUS_65536_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -65536.0
 */
#define FLOAT_MINUS_65536_EXP       16

/*!
   \anchor FLOAT_MINUS_65536_MAN
   \addindex FLOAT_MINUS_65536_MAN
   \page bmldefFloatNeg65536_Man     FLOAT_MINUS_65536_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -65536.0
 */
#define FLOAT_MINUS_65536_MAN       FLOAT_MIN_MAN

/*!
   \anchor FLOAT_MINUS_65536
   \addindex FLOAT_MINUS_65536
   \page bmldefFloatNeg65536   FLOAT_MINUS_65536
   \details Constant representing the value of a floating number, equal to -65536.0
 */
#define FLOAT_MINUS_65536           {FLOAT_MINUS_65536_SIGN, FLOAT_MINUS_65536_EXP, FLOAT_MINUS_65536_MAN}

/*!
   \anchor FLOAT_MAX_NEG_MUL_0_75_SIGN
   \addindex FLOAT_MAX_NEG_MUL_0_75_SIGN
   \page bmldefFloatMaxNegMul_0_75_Sign     FLOAT_MAX_NEG_MUL_0_75_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_NEG_MUL_0_75_SIGN     FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MAX_NEG_MUL_0_75_EXP
   \addindex FLOAT_MAX_NEG_MUL_0_75_EXP
   \page bmldefFloatMaxNegMul_0_75_Exp     FLOAT_MAX_NEG_MUL_0_75_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_NEG_MUL_0_75_EXP      FLOAT_MAX_EXP

/*!
   \anchor FLOAT_MAX_NEG_MUL_0_75_MAN
   \addindex FLOAT_MAX_NEG_MUL_0_75_MAN
   \page bmldefFloatMaxNegMul_0_75_Man     FLOAT_MAX_NEG_MUL_0_75_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_NEG_MUL_0_75_MAN      49151

/*!
   \anchor FLOAT_MAX_NEG_MUL_0_75
   \addindex FLOAT_MAX_NEG_MUL_0_75
   \page bmldefFloatMaxNegMul_0_75   FLOAT_MAX_NEG_MUL_0_75
   \details Constant representing the value of a floating number, equal to -0.75*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_NEG_MUL_0_75          {FLOAT_MAX_NEG_MUL_0_75_SIGN, FLOAT_MAX_NEG_MUL_0_75_EXP, FLOAT_MAX_NEG_MUL_0_75_MAN}

/*!
   \anchor FLOAT_MAX_NEG_SIGN
   \addindex FLOAT_MAX_NEG_SIGN
   \page bmldefFloatMaxNeg_Sign     FLOAT_MAX_NEG_SIGN
   \details Struct element (sign) of constant representing the value of a floating number, equal to -1*(2^(127)*65535/32768)
 */
#define FLOAT_MAX_NEG_SIGN      FLOAT_SIGN_NEG

/*!
   \anchor FLOAT_MAX_NEG_EXP
   \addindex FLOAT_MAX_NEG_EXP
   \page bmldefFloatMaxNeg_Exp     FLOAT_MAX_NEG_EXP
   \details Struct element (exponent) of constant representing the value of a floating number, equal to -1*2^(127)*65535/32768
 */
#define FLOAT_MAX_NEG_EXP       FLOAT_MAX_EXP

/*!
   \anchor FLOAT_MAX_NEG_MAN
   \addindex FLOAT_MAX_NEG_MAN
   \page bmldefFloatMaxNeg_Man   FLOAT_MAX_NEG_MAN
   \details Struct element (mantissa) of constant representing the value of a floating number, equal to -1*2^(127)*65535/32768
 */
#define FLOAT_MAX_NEG_MAN       FLOAT_MAX_MAN

/*!
   \anchor FLOAT_MAX_NEG
   \addindex FLOAT_MAX_NEG
   \page bmldefFloatMaxNeg         FLOAT_MAX_NEG
   \details Constant representing the value of a floating number, equal to -1*2^(127)*65535/32768
 */
#define FLOAT_MAX_NEG           {FLOAT_MAX_NEG_SIGN, FLOAT_MAX_NEG_EXP, FLOAT_MAX_NEG_MAN}

#endif /* BML_BASE_DEFINES_H_ */
/* EOF */
