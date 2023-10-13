/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  bml_Base_Typedefs.h
 * \date      04.11.2015
 * \brief     Basic Math Library type definition file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef BML_BASE_TYPEDEFS_H_
#define BML_BASE_TYPEDEFS_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "colorLib_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_pFloat
   \addindex t_pFloat
   \page bmltpFloat t_pFloat
   \details
   \b Description \n
   \n
   t_pFloat is a structure for floating point operations and describes a number with
   sign, exponent and mantissa.

   \f[
   number_{float}  = \left( { - 1} \right)^{sign} *2^{exponent}*\left( {\frac{{mantissa}}{{2^{15} }}} \right)
   \f]

   The fraction part consists of 15 bits for the mantissa and additionally the hidden
   bit which is also stored in the memory. Because of that, 16 bits are used for the
   fraction part, 8 bits are used for the exponent and 8 bits are used for the sign
   -> 4 bytes.\n
   \n
   \b Data \b Fields
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{3cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   sign     & \nameref{colorLibtUInt8}  & +, sign == 0x00 \newline -, otherwise\\
   \hline
   exponent & \nameref{colorLibtInt8}   & Exponent \newline range: \nameref{bmldefFloatMinExp}...\nameref{bmldefFloatMaxExp}\\
   \hline
   mantissa & \nameref{colorLibtUInt16} & Fraction -> (1.xxx -> xxxx = mantissa).\\
   & & Mantissa is 15 bit where the hidden bit (16th) is also stored in the memory.\\
   & & range: \nameref{bmldefFloatMinMan}...\nameref{bmldefFloatMaxMan} \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_uint8 sign;
    t_int8 exponent;
    t_uint16 mantissa;
} t_pFloat;

#endif /* BML_BASE_TYPEDEFS_H_ */

/* EOF */
