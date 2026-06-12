/*
 * Copyright (c) 2025 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

/**
 * \file     types.h
 *
 * \brief    General type declarations
 *
 * \version  V1.0.4
 * \date     07. Jul 2025
 *
 */

/*******************************************************************************
**                             Author(s) Identity                             **
********************************************************************************
** Initials     Name                                                          **
** ---------------------------------------------------------------------------**
** CG           Christopher Gabriel                                           **
** SL           Stefano Lissandron                                            **
** JO           Julia Ott                                                     **
** VO           Vanessa Ongaro                                                **
*******************************************************************************/

/*******************************************************************************
**                          Revision Control History                          **
********************************************************************************
** V1.0.0: 2022-10-21, SL:   Initial version                                  **
** V1.0.1: 2023-02-13, SL:   Improved compatibility with C2x                  **
** V1.0.2: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             **
** V1.0.3: 2025-02-24, VO:   [ATVCOMETLL-165] Move error codes to             **
**                                            error_codes.h                   **
** V1.0.4: 2025-07-07, VO:   [ATVCOMETLL-202] Updated license text            **
*******************************************************************************/

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/*******************************************************************************
**                                  Includes                                  **
*******************************************************************************/

/*******************************************************************************
**                          Global Macro Definitions                          **
*******************************************************************************/
/** \brief Standard real type definitions to ease the use of different types
 */
#define STD_REAL_FIX_POINT_16 (1)
#define STD_REAL_FIX_POINT_32 (2)
#define STD_REAL_FLOAT_32     (3)

#ifndef STD_REAL_TYPE
  #define STD_REAL_TYPE (STD_REAL_FIX_POINT_16)
#endif


/** \brief Boolean definitions
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
  #include <stdbool.h>
#else
  #define bool unsigned int
  #define true 1
  #define false 0
#endif

/** \brief Shift definitions for fix point format
 */
#define STD_REAL_SHIFT_Qn8_y (-8  - 1)       /**< \brief Range=[-2^(-9), 2^(-9)[; Q1.15=>Step=1/2^24, Q1.31 =>Step=1/2^40 */
#define STD_REAL_SHIFT_Qn7_y (-7  - 1)       /**< \brief Range=[-2^(-8), 2^(-8)[; Q1.15=>Step=1/2^23, Q1.31 =>Step=1/2^39 */
#define STD_REAL_SHIFT_Qn6_y (-6  - 1)       /**< \brief Range=[-2^(-7), 2^(-7)[; Q1.15=>Step=1/2^22, Q1.31 =>Step=1/2^38 */
#define STD_REAL_SHIFT_Qn5_y (-5  - 1)       /**< \brief Range=[-2^(-6), 2^(-6)[; Q1.15=>Step=1/2^21, Q1.31 =>Step=1/2^37 */
#define STD_REAL_SHIFT_Qn4_y (-4  - 1)       /**< \brief Range=[-2^(-5), 2^(-5)[; Q1.15=>Step=1/2^20, Q1.31 =>Step=1/2^36 */
#define STD_REAL_SHIFT_Qn3_y (-3  - 1)       /**< \brief Range=[-2^(-4), 2^(-4)[; Q1.15=>Step=1/2^19, Q1.31 =>Step=1/2^35 */
#define STD_REAL_SHIFT_Qn2_y (-2  - 1)       /**< \brief Range=[-2^(-3), 2^(-3)[; Q1.15=>Step=1/2^18, Q1.31 =>Step=1/2^34 */
#define STD_REAL_SHIFT_Qn1_y (-1  - 1)       /**< \brief Range=[-2^(-2), 2^(-2)[; Q1.15=>Step=1/2^17, Q1.31 =>Step=1/2^33 */

#define STD_REAL_SHIFT_Q0_y  ( 0  - 1)       /**< \brief Range=[-2^(-1), 2^(-1)[; Q1.15=>Step=1/2^16, Q1.31 =>Step=1/2^32 */
#define STD_REAL_SHIFT_Q1_y  ( 1  - 1)       /**< \brief Range=[-2^0   , 2^0[;    Q1.15=>Step=1/2^15, Q1.31 =>Step=1/2^31 */
#define STD_REAL_SHIFT_Q2_y  ( 2  - 1)       /**< \brief Range=[-2^1   , 2^1[;    Q1.15=>Step=1/2^14, Q1.31 =>Step=1/2^30 */
#define STD_REAL_SHIFT_Q3_y  ( 3  - 1)       /**< \brief Range=[-2^2   , 2^2[;    Q1.15=>Step=1/2^13, Q1.31 =>Step=1/2^29 */
#define STD_REAL_SHIFT_Q4_y  ( 4  - 1)       /**< \brief Range=[-2^3   , 2^3[;    Q1.15=>Step=1/2^12, Q1.31 =>Step=1/2^28 */
#define STD_REAL_SHIFT_Q5_y  ( 5  - 1)       /**< \brief Range=[-2^4   , 2^4[;    Q1.15=>Step=1/2^11, Q1.31 =>Step=1/2^27 */
#define STD_REAL_SHIFT_Q6_y  ( 6  - 1)       /**< \brief Range=[-2^5   , 2^5[;    Q1.15=>Step=1/2^10, Q1.31 =>Step=1/2^26 */
#define STD_REAL_SHIFT_Q7_y  ( 7  - 1)       /**< \brief Range=[-2^6   , 2^6[;    Q7.9 =>Step=1/2^9,  Q8.24 =>Step=1/2^25 */
#define STD_REAL_SHIFT_Q8_y  ( 8  - 1)       /**< \brief Range=[-2^7   , 2^7[;    Q8.8 =>Step=1/2^8,  Q8.24 =>Step=1/2^24 */
#define STD_REAL_SHIFT_Q9_y  ( 9  - 1)       /**< \brief Range=[-2^8   , 2^8[;    Q9.7 =>Step=1/2^7,  Q9.23 =>Step=1/2^23 */
#define STD_REAL_SHIFT_Q10_y ( 10 - 1)       /**< \brief Range=[-2^9   , 2^9[;    Q10.6=>Step=1/2^6,  Q10.22=>Step=1/2^22 */
#define STD_REAL_SHIFT_Q11_y ( 11 - 1)       /**< \brief Range=[-2^10  , 2^10[;   Q11.5=>Step=1/2^5,  Q11.21=>Step=1/2^21 */
#define STD_REAL_SHIFT_Q12_y ( 12 - 1)       /**< \brief Range=[-2^11  , 2^11[;   Q12.4=>Step=1/2^4,  Q12.20=>Step=1/2^20 */
#define STD_REAL_SHIFT_Q13_y ( 13 - 1)       /**< \brief Range=[-2^12  , 2^12[;   Q13.3=>Step=1/2^3,  Q13.19=>Step=1/2^19 */
#define STD_REAL_SHIFT_Q14_y ( 14 - 1)       /**< \brief Range=[-2^13  , 2^13[;   Q14.2=>Step=1/2^2,  Q14.18=>Step=1/2^18 */
#define STD_REAL_SHIFT_Q15_y ( 15 - 1)       /**< \brief Range=[-2^14  , 2^14[;   Q15.1=>Step=1/2^1,  Q15.17=>Step=1/2^17 */
#define STD_REAL_SHIFT_Q16_y ( 16 - 1)       /**< \brief Range=[-2^15  , 2^15[;   Q16.0=>Step=1,      Q16.16=>Step=1/2^16 */
#define STD_REAL_SHIFT_Q32_y ( 32 - 1)       /**< \brief Range=[-2^31  , 2^31[;   Q16.0=>             Q32.0 =>Step=1      */

#define NULL_PTR ((void*)0)

#define INLINE __STATIC_INLINE

/*******************************************************************************
**                           Global Type Definitions                          **
*******************************************************************************/
typedef uint8_t uint8;                       /**< \brief 8 bit unsigned value  */
typedef uint16_t uint16;                     /**< \brief 16 bit unsigned value */
typedef uint32_t uint32;                     /**< \brief 32 bit unsigned value */
typedef uint64_t uint64;                     /**< \brief 64 bit unsigned value */

typedef int8_t sint8;                        /**< \brief 8 bit signed value  */
typedef int16_t sint16;                      /**< \brief 16 bit signed value */
typedef int32_t sint32;                      /**< \brief 32 bit signed value */
typedef int64_t sint64;                      /**< \brief 64 bit signed value */

typedef float   float32;                     /**< \brief 32 bit float value */
typedef double  float64;                     /**< \brief 64 bit float value */

typedef short FixPoint16;                    /**< \brief 16 bit fix point value */
typedef long  FixPoint32;                    /**< \brief 32 bit fix point value */


/** \brief Standard real type definitions to ease the use of different types
 */
#if (STD_REAL_TYPE == STD_REAL_FIX_POINT_16)
  typedef FixPoint16 TStdReal;
  typedef FixPoint32 TLongStdReal;
#elif (STD_REAL_TYPE == STD_REAL_FIX_POINT_32)
  typedef FixPoint32 TStdReal;
  typedef FixPoint32 TLongStdReal;
#elif (STD_REAL_TYPE == STD_REAL_FLOAT_32)
  typedef float32 TStdReal;
  typedef float32 TLongStdReal;
#endif


/** \brief Complex type definition based on TStdReal
 */
typedef struct StdRealComplex
{
  TStdReal imag;                             /**< \brief Imaginary part */
  TStdReal real;                             /**< \brief Real part */
} TStdRealComplex;

/** \brief Complex type definition
 */
typedef struct Complex
{
  sint16 Real;                               /**< \brief Real part */
  sint16 Imag;                               /**< \brief Imaginary part */
} TComplex;

/** \brief 2 phase currents type definition
 */
typedef struct PhaseCurr
{
  sint16 A;                                  /**< \brief Phase A current */
  sint16 B;                                  /**< \brief Phase B current */
} TPhaseCurr;

#endif /* TYPES_H */

