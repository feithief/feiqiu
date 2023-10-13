/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  bml.h
 * \date      04.11.2015
 * \brief     Master header file of Basic Math Library. Token: bml
 * \author    DX.cba
 **************************************************************************************/

/*!
   \page bml Basic Math Library
   The <b>B</b>asic <b>M</b>ath <b>L</b>ibrary (bml) provides basic mathematical operations for floating point numbers.
   \image latex BasicMath_Structure_Overview.pdf "Basic Math Library structure " width=1.0\textwidth
   The bml description chapter is divided into three subchapters:
   \li  \subpage bmlTypDesc
   \li  \subpage bmlFncDesc
   \li  \subpage bmlMacDesc

   \page bmlTypDesc Typedef Description
   This chapter describes following typedefs defined in the Basic Math Library:
   \li <tt> \subpage bmltpFloat </tt>

   \page bmlFncDesc Function Description
   This chapter describes following functions defined in the Basic Math Library:
   \li <tt> \subpage bmlfnAbs </tt>
   \li <tt> \subpage bmlfnAdd </tt>
   \li <tt> \subpage bmlfnCheckFloatSaturation </tt>
   \li <tt> \subpage bmlfnCompare </tt>
   \li <tt> \subpage bmlfnDiv </tt>
   \li <tt> \subpage bmlfnFloatToUint </tt>
   \li <tt> \subpage bmlfnIntToFloat </tt>
   \li <tt> \subpage bmlfnMul </tt>
   \li <tt> \subpage bmlfnNeg </tt>
   \li <tt> \subpage bmlfnNormalize </tt>
   \li <tt> \subpage bmlfnSqrt </tt>
   \li <tt> \subpage bmlfnSub </tt>

   \page bmlMacDesc Macro Description
   This chapter describes defines used for basic math library functions and floating point numbers. It is divided in the
   following subchapters:
   \li  \subpage bmlGeneralDefs
   \li  \subpage bmlPosFloats
   \li  \subpage bmlNegFloats

   \page bmlGeneralDefs General defines and macros
   This chapter describes following defines:
   \li <tt> \subpage bmldefFloatSignPos </tt>
   \li <tt> \subpage bmldefFloatSignNeg </tt>
   \li <tt> \subpage bmldefFloatMaxExp </tt>
   \li <tt> \subpage bmldefFloatMinExp </tt>
   \li <tt> \subpage bmldefFloatMaxMan </tt>
   \li <tt> \subpage bmldefFloatMinMan </tt>

   \page bmlPosFloats Positive floating point numbers
   This chapter describes following defines:
   \li <tt> \subpage bmldefFloat0Sign </tt>
   \li <tt> \subpage bmldefFloat0Exp </tt>
   \li <tt> \subpage bmldefFloat0Man </tt>
   \li <tt> \subpage bmldefFloat0 </tt>
   \li <tt> \subpage bmldefFloatMinPosSign </tt>
   \li <tt> \subpage bmldefFloatMinPosExp </tt>
   \li <tt> \subpage bmldefFloatMinPosMan </tt>
   \li <tt> \subpage bmldefFloatMinPos </tt>
   \li <tt> \subpage bmldefFloatSqrtMinPosSign </tt>
   \li <tt> \subpage bmldefFloatSqrtMinPosExp </tt>
   \li <tt> \subpage bmldefFloatSqrtMinPosMan </tt>
   \li <tt> \subpage bmldefFloatSqrtMinPos </tt>
   \li <tt> \subpage bmldefFloatMinPosMul2Sign </tt>
   \li <tt> \subpage bmldefFloatMinPosMul2Exp </tt>
   \li <tt> \subpage bmldefFloatMinPosMul2Man </tt>
   \li <tt> \subpage bmldefFloatMinPosMul2 </tt>
   \li <tt> \subpage bmldefFloatMinPosMul3Sign </tt>
   \li <tt> \subpage bmldefFloatMinPosMul3Exp </tt>
   \li <tt> \subpage bmldefFloatMinPosMul3Man </tt>
   \li <tt> \subpage bmldefFloatMinPosMul3 </tt>
   \li <tt> \subpage bmldefFloat0_001_Sign </tt>
   \li <tt> \subpage bmldefFloat0_001_Exp </tt>
   \li <tt> \subpage bmldefFloat0_001_Man </tt>
   \li <tt> \subpage bmldefFloat0_001 </tt>
   \li <tt> \subpage bmldefFloat0_01_Sign </tt>
   \li <tt> \subpage bmldefFloat0_01_Exp </tt>
   \li <tt> \subpage bmldefFloat0_01_Man </tt>
   \li <tt> \subpage bmldefFloat0_01 </tt>
   \li <tt> \subpage bmldefFloat0_1_Sign </tt>
   \li <tt> \subpage bmldefFloat0_1_Exp </tt>
   \li <tt> \subpage bmldefFloat0_1_Man </tt>
   \li <tt> \subpage bmldefFloat0_1 </tt>
   \li <tt> \subpage bmldefFloat0_2_Sign </tt>
   \li <tt> \subpage bmldefFloat0_2_Exp </tt>
   \li <tt> \subpage bmldefFloat0_2_Man </tt>
   \li <tt> \subpage bmldefFloat0_2 </tt>
   \li <tt> \subpage bmldefFloat0_25_Sign </tt>
   \li <tt> \subpage bmldefFloat0_25_Exp </tt>
   \li <tt> \subpage bmldefFloat0_25_Man </tt>
   \li <tt> \subpage bmldefFloat0_25 </tt>
   \li <tt> \subpage bmldefFloat0_3_Sign </tt>
   \li <tt> \subpage bmldefFloat0_3_Exp </tt>
   \li <tt> \subpage bmldefFloat0_3_Man </tt>
   \li <tt> \subpage bmldefFloat0_3 </tt>
   \li <tt> \subpage bmldefFloat0_33_Sign </tt>
   \li <tt> \subpage bmldefFloat0_33_Exp </tt>
   \li <tt> \subpage bmldefFloat0_33_Man </tt>
   \li <tt> \subpage bmldefFloat0_33 </tt>
   \li <tt> \subpage bmldefFloat0_5_Sign </tt>
   \li <tt> \subpage bmldefFloat0_5_Exp </tt>
   \li <tt> \subpage bmldefFloat0_5_Man </tt>
   \li <tt> \subpage bmldefFloat0_5 </tt>
   \li <tt> \subpage bmldefFloat0_6_Sign </tt>
   \li <tt> \subpage bmldefFloat0_6_Exp </tt>
   \li <tt> \subpage bmldefFloat0_6_Man </tt>
   \li <tt> \subpage bmldefFloat0_6 </tt>
   \li <tt> \subpage bmldefFloat0_66_Sign </tt>
   \li <tt> \subpage bmldefFloat0_66_Exp </tt>
   \li <tt> \subpage bmldefFloat0_66_Man </tt>
   \li <tt> \subpage bmldefFloat0_66 </tt>
   \li <tt> \subpage bmldefFloat0_8_Sign </tt>
   \li <tt> \subpage bmldefFloat0_8_Exp </tt>
   \li <tt> \subpage bmldefFloat0_8_Man </tt>
   \li <tt> \subpage bmldefFloat0_8 </tt>
   \li <tt> \subpage bmldefFloat1_Sign </tt>
   \li <tt> \subpage bmldefFloat1_Exp </tt>
   \li <tt> \subpage bmldefFloat1_Man </tt>
   \li <tt> \subpage bmldefFloat1 </tt>
   \li <tt> \subpage bmldefFloat1_5_Sign </tt>
   \li <tt> \subpage bmldefFloat1_5_Exp </tt>
   \li <tt> \subpage bmldefFloat1_5_Man </tt>
   \li <tt> \subpage bmldefFloat1_5 </tt>
   \li <tt> \subpage bmldefFloat2_Sign </tt>
   \li <tt> \subpage bmldefFloat2_Exp </tt>
   \li <tt> \subpage bmldefFloat2_Man </tt>
   \li <tt> \subpage bmldefFloat2 </tt>
   \li <tt> \subpage bmldefFloat3_Sign </tt>
   \li <tt> \subpage bmldefFloat3_Exp </tt>
   \li <tt> \subpage bmldefFloat3_Man </tt>
   \li <tt> \subpage bmldefFloat3 </tt>
   \li <tt> \subpage bmldefFloat4_Sign </tt>
   \li <tt> \subpage bmldefFloat4_Exp </tt>
   \li <tt> \subpage bmldefFloat4_Man </tt>
   \li <tt> \subpage bmldefFloat4 </tt>
   \li <tt> \subpage bmldefFloat4_8_Sign </tt>
   \li <tt> \subpage bmldefFloat4_8_Exp </tt>
   \li <tt> \subpage bmldefFloat4_8_Man </tt>
   \li <tt> \subpage bmldefFloat4_8 </tt>
   \li <tt> \subpage bmldefFloat5_Sign </tt>
   \li <tt> \subpage bmldefFloat5_Exp </tt>
   \li <tt> \subpage bmldefFloat5_Man </tt>
   \li <tt> \subpage bmldefFloat5 </tt>
   \li <tt> \subpage bmldefFloat6_Sign </tt>
   \li <tt> \subpage bmldefFloat6_Exp </tt>
   \li <tt> \subpage bmldefFloat6_Man </tt>
   \li <tt> \subpage bmldefFloat6 </tt>
   \li <tt> \subpage bmldefFloat7_Sign </tt>
   \li <tt> \subpage bmldefFloat7_Exp </tt>
   \li <tt> \subpage bmldefFloat7_Man </tt>
   \li <tt> \subpage bmldefFloat7 </tt>
   \li <tt> \subpage bmldefFloat8_Sign </tt>
   \li <tt> \subpage bmldefFloat8_Exp </tt>
   \li <tt> \subpage bmldefFloat8_Man </tt>
   \li <tt> \subpage bmldefFloat8 </tt>
   \li <tt> \subpage bmldefFloat9_Sign </tt>
   \li <tt> \subpage bmldefFloat9_Exp </tt>
   \li <tt> \subpage bmldefFloat9_Man </tt>
   \li <tt> \subpage bmldefFloat9 </tt>
   \li <tt> \subpage bmldefFloat9_99_Sign </tt>
   \li <tt> \subpage bmldefFloat9_99_Exp </tt>
   \li <tt> \subpage bmldefFloat9_99_Man </tt>
   \li <tt> \subpage bmldefFloat9_99 </tt>
   \li <tt> \subpage bmldefFloat10_Sign </tt>
   \li <tt> \subpage bmldefFloat10_Exp </tt>
   \li <tt> \subpage bmldefFloat10_Man </tt>
   \li <tt> \subpage bmldefFloat10 </tt>
   \li <tt> \subpage bmldefFloat20_Sign </tt>
   \li <tt> \subpage bmldefFloat20_Exp </tt>
   \li <tt> \subpage bmldefFloat20_Man </tt>
   \li <tt> \subpage bmldefFloat20 </tt>
   \li <tt> \subpage bmldefFloat40_Sign </tt>
   \li <tt> \subpage bmldefFloat40_Exp </tt>
   \li <tt> \subpage bmldefFloat40_Man </tt>
   \li <tt> \subpage bmldefFloat40 </tt>
   \li <tt> \subpage bmldefFloat50_Sign </tt>
   \li <tt> \subpage bmldefFloat50_Exp </tt>
   \li <tt> \subpage bmldefFloat50_Man </tt>
   \li <tt> \subpage bmldefFloat50 </tt>
   \li <tt> \subpage bmldefFloat100_Sign </tt>
   \li <tt> \subpage bmldefFloat100_Exp </tt>
   \li <tt> \subpage bmldefFloat100_Man </tt>
   \li <tt> \subpage bmldefFloat100 </tt>
   \li <tt> \subpage bmldefFloat150_Sign </tt>
   \li <tt> \subpage bmldefFloat150_Exp </tt>
   \li <tt> \subpage bmldefFloat150_Man </tt>
   \li <tt> \subpage bmldefFloat150 </tt>
   \li <tt> \subpage bmldefFloat200_Sign </tt>
   \li <tt> \subpage bmldefFloat200_Exp </tt>
   \li <tt> \subpage bmldefFloat200_Man </tt>
   \li <tt> \subpage bmldefFloat200 </tt>
   \li <tt> \subpage bmldefFloat254_Sign </tt>
   \li <tt> \subpage bmldefFloat254_Exp </tt>
   \li <tt> \subpage bmldefFloat254_Man </tt>
   \li <tt> \subpage bmldefFloat254 </tt>
   \li <tt> \subpage bmldefFloat255_Sign </tt>
   \li <tt> \subpage bmldefFloat255_Exp </tt>
   \li <tt> \subpage bmldefFloat255_Man </tt>
   \li <tt> \subpage bmldefFloat255 </tt>
   \li <tt> \subpage bmldefFloat1000_Sign </tt>
   \li <tt> \subpage bmldefFloat1000_Exp </tt>
   \li <tt> \subpage bmldefFloat1000_Man </tt>
   \li <tt> \subpage bmldefFloat1000 </tt>
   \li <tt> \subpage bmldefFloat4095_Sign </tt>
   \li <tt> \subpage bmldefFloat4095_Exp </tt>
   \li <tt> \subpage bmldefFloat4095_Man </tt>
   \li <tt> \subpage bmldefFloat4095 </tt>
   \li <tt> \subpage bmldefFloat32767_Sign </tt>
   \li <tt> \subpage bmldefFloat32767_Exp </tt>
   \li <tt> \subpage bmldefFloat32767_Man </tt>
   \li <tt> \subpage bmldefFloat32767 </tt>
   \li <tt> \subpage bmldefFloat65535_Sign </tt>
   \li <tt> \subpage bmldefFloat65535_Exp </tt>
   \li <tt> \subpage bmldefFloat65535_Man </tt>
   \li <tt> \subpage bmldefFloat65535 </tt>
   \li <tt> \subpage bmldefFloat65536_Sign </tt>
   \li <tt> \subpage bmldefFloat65536_Exp </tt>
   \li <tt> \subpage bmldefFloat65536_Man </tt>
   \li <tt> \subpage bmldefFloat65536 </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_0075_Sign </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_0075_Exp </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_0075_Man </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_0075 </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_75_Sign </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_75_Exp </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_75_Man </tt>
   \li <tt> \subpage bmldefFloatMaxPosMul0_75 </tt>
   \li <tt> \subpage bmldefFloatSqrtMaxPosSign </tt>
   \li <tt> \subpage bmldefFloatSqrtMaxPosExp </tt>
   \li <tt> \subpage bmldefFloatSqrtMaxPosMan </tt>
   \li <tt> \subpage bmldefFloatSqrtMaxPos </tt>
   \li <tt> \subpage bmldefFloatMaxPosSign </tt>
   \li <tt> \subpage bmldefFloatMaxPosExp </tt>
   \li <tt> \subpage bmldefFloatMaxPosMan </tt>
   \li <tt> \subpage bmldefFloatMaxPos </tt>

   \page bmlNegFloats Negative floating point numbers
   This chapter describes following defines:
   \li <tt> \subpage bmldefFloatMinNegSign </tt>
   \li <tt> \subpage bmldefFloatMinNegExp </tt>
   \li <tt> \subpage bmldefFloatMinNegMan </tt>
   \li <tt> \subpage bmldefFloatMinNeg </tt>
   \li <tt> \subpage bmldefFloatMinNegMul2Sign </tt>
   \li <tt> \subpage bmldefFloatMinNegMul2Exp </tt>
   \li <tt> \subpage bmldefFloatMinNegMul2Man </tt>
   \li <tt> \subpage bmldefFloatMinNegMul2 </tt>
   \li <tt> \subpage bmldefFloatMinNegMul3Sign </tt>
   \li <tt> \subpage bmldefFloatMinNegMul3Exp </tt>
   \li <tt> \subpage bmldefFloatMinNegMul3Man </tt>
   \li <tt> \subpage bmldefFloatMinNegMul3 </tt>
   \li <tt> \subpage bmldefFloatNeg0_01_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_01_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_01_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_01 </tt>
   \li <tt> \subpage bmldefFloatNeg0_1_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_1_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_1_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_1 </tt>
   \li <tt> \subpage bmldefFloatNeg0_2_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_2_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_2_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_2 </tt>
   \li <tt> \subpage bmldefFloatNeg0_25_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_25_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_25_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_25 </tt>
   \li <tt> \subpage bmldefFloatNeg0_3_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_3_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_3_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_3 </tt>
   \li <tt> \subpage bmldefFloatNeg0_33_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_33_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_33_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_33 </tt>
   \li <tt> \subpage bmldefFloatNeg0_5_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_5_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_5_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_5 </tt>
   \li <tt> \subpage bmldefFloatNeg0_6_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_6_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_6_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_6 </tt>
   \li <tt> \subpage bmldefFloatNeg0_66_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg0_66_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg0_66_Man </tt>
   \li <tt> \subpage bmldefFloatNeg0_66 </tt>
   \li <tt> \subpage bmldefFloatNeg1_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg1_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg1_Man </tt>
   \li <tt> \subpage bmldefFloatNeg1 </tt>
   \li <tt> \subpage bmldefFloatNeg1_5_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg1_5_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg1_5_Man </tt>
   \li <tt> \subpage bmldefFloatNeg1_5 </tt>
   \li <tt> \subpage bmldefFloatNeg2_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg2_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg2_Man </tt>
   \li <tt> \subpage bmldefFloatNeg2 </tt>
   \li <tt> \subpage bmldefFloatNeg3_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg3_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg3_Man </tt>
   \li <tt> \subpage bmldefFloatNeg3 </tt>
   \li <tt> \subpage bmldefFloatNeg4_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg4_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg4_Man </tt>
   \li <tt> \subpage bmldefFloatNeg4 </tt>
   \li <tt> \subpage bmldefFloatNeg5_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg5_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg5_Man </tt>
   \li <tt> \subpage bmldefFloatNeg5 </tt>
   \li <tt> \subpage bmldefFloatNeg6_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg6_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg6_Man </tt>
   \li <tt> \subpage bmldefFloatNeg6 </tt>
   \li <tt> \subpage bmldefFloatNeg7_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg7_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg7_Man </tt>
   \li <tt> \subpage bmldefFloatNeg7 </tt>
   \li <tt> \subpage bmldefFloatNeg8_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg8_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg8_Man </tt>
   \li <tt> \subpage bmldefFloatNeg8 </tt>
   \li <tt> \subpage bmldefFloatNeg9_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg9_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg9_Man </tt>
   \li <tt> \subpage bmldefFloatNeg9 </tt>
   \li <tt> \subpage bmldefFloatNeg9_99_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg9_99_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg9_99_Man </tt>
   \li <tt> \subpage bmldefFloatNeg9_99 </tt>
   \li <tt> \subpage bmldefFloatNeg10_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg10_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg10_Man </tt>
   \li <tt> \subpage bmldefFloatNeg10 </tt>
   \li <tt> \subpage bmldefFloatNeg20_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg20_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg20_Man </tt>
   \li <tt> \subpage bmldefFloatNeg20 </tt>
   \li <tt> \subpage bmldefFloatNeg40_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg40_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg40_Man </tt>
   \li <tt> \subpage bmldefFloatNeg40 </tt>
   \li <tt> \subpage bmldefFloatNeg50_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg50_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg50_Man </tt>
   \li <tt> \subpage bmldefFloatNeg50 </tt>
   \li <tt> \subpage bmldefFloatNeg100_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg100_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg100_Man </tt>
   \li <tt> \subpage bmldefFloatNeg100 </tt>
   \li <tt> \subpage bmldefFloatNeg150_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg150_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg150_Man </tt>
   \li <tt> \subpage bmldefFloatNeg150 </tt>
   \li <tt> \subpage bmldefFloatNeg200_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg200_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg200_Man </tt>
   \li <tt> \subpage bmldefFloatNeg200 </tt>
   \li <tt> \subpage bmldefFloatNeg254_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg254_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg254_Man </tt>
   \li <tt> \subpage bmldefFloatNeg254 </tt>
   \li <tt> \subpage bmldefFloatNeg255_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg255_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg255_Man </tt>
   \li <tt> \subpage bmldefFloatNeg255 </tt>
   \li <tt> \subpage bmldefFloatNeg1000_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg1000_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg1000_Man </tt>
   \li <tt> \subpage bmldefFloatNeg1000 </tt>
   \li <tt> \subpage bmldefFloatNeg32767_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg32767_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg32767_Man </tt>
   \li <tt> \subpage bmldefFloatNeg32767 </tt>
   \li <tt> \subpage bmldefFloatNeg32768_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg32768_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg32768_Man </tt>
   \li <tt> \subpage bmldefFloatNeg32768 </tt>
   \li <tt> \subpage bmldefFloatNeg65535_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg65535_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg65535_Man </tt>
   \li <tt> \subpage bmldefFloatNeg65535 </tt>
   \li <tt> \subpage bmldefFloatNeg65536_Sign </tt>
   \li <tt> \subpage bmldefFloatNeg65536_Exp </tt>
   \li <tt> \subpage bmldefFloatNeg65536_Man </tt>
   \li <tt> \subpage bmldefFloatNeg65536 </tt>
   \li <tt> \subpage bmldefFloatMaxNegMul_0_75_Sign </tt>
   \li <tt> \subpage bmldefFloatMaxNegMul_0_75_Exp </tt>
   \li <tt> \subpage bmldefFloatMaxNegMul_0_75_Man </tt>
   \li <tt> \subpage bmldefFloatMaxNegMul_0_75 </tt>
   \li <tt> \subpage bmldefFloatMaxNeg_Sign </tt>
   \li <tt> \subpage bmldefFloatMaxNeg_Exp </tt>
   \li <tt> \subpage bmldefFloatMaxNeg_Man </tt>
   \li <tt> \subpage bmldefFloatMaxNeg </tt>
 */


#ifndef BML_H_
#define BML_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "bml_Base_Defines.h"
#include "bml_Base_Typedefs.h"

#include "bml_Abs.h"
#include "bml_Add.h"
#include "bml_CheckFloatSaturation.h"
#include "bml_Compare.h"
#include "bml_Div.h"
#include "bml_FloatToUint.h"
#include "bml_IntToFloat.h"
#include "bml_Mul.h"
#include "bml_Neg.h"
#include "bml_Sqrt.h"
#include "bml_Sub.h"

#endif /* BML_H_ */
/* EOF */
