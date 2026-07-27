/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  colorLib.h
 * \date      27.01.2016
 * \brief     Master header file of Mlx8110c Color Library. Token: cfl
 * \author    DX.cba
 **************************************************************************************/

/*!
   \page colorLib Color Library
   The Mlx811xy <b>Color Lib</b>rary (colorLib) contains mathematical operations and functions to handle
   RGB applications with MLX81106/07/08/09, MLX81112/15/20 and MLX80110. As shown in the figure below, it
   is divided in four separate mathematical libraries.
   \image latex ColorLib_Structure_Overview.pdf "MLX811xy Color Library structure " width=1.1\textwidth
   The colorLib handles RGB control values in the following steps:
   <ul>
   <li> Conversion of RGB control values into XYZ (CIE1931)
   <li> Calculate maximum intensity according maximum allowed current (XYZ &rarr; xyY &rarr; xyY<SUB>max</SUB> &rarr; X'Y'Z')
   <li> Conversion X'Y'Z' into PWM values
   <li> Temperature compensation (optional)
   </ul>
   \n
   The colorLib description chapter is divided in the following parts:
   \li \subpage colorLibTypDesc
   \li \subpage colorLibFncDesc

   \page colorLibTypDesc Typedef Description
   This chapter describes following typedefs defined in the colorLib:
   \li <tt> \subpage colorLibtInt8 </tt>
   \li <tt> \subpage colorLibtUInt8 </tt>
   \li <tt> \subpage colorLibtInt16 </tt>
   \li <tt> \subpage colorLibtUInt16 </tt>
   \li <tt> \subpage colorLibtInt32 </tt>
   \li <tt> \subpage colorLibtUInt32 </tt>
   \li <tt> \subpage colorLibtBool </tt>

   \page colorLibFncDesc Function Description
   This chapter describes following functions defined in Color Library:
   \li <tt> \subpage colorLibfnGetVersion </tt>
 */

#ifndef COLORLIB_H_
#define COLORLIB_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "colorLib_Defines.h"
#include "colorLib_Typedefs.h"
#include "colorLib_Version.h"

#include "bml.h"
#include "aml.h"
#include "vcal.h"
#include "cfl.h"


#endif /* COLORLIB_H_ */
/* EOF */
