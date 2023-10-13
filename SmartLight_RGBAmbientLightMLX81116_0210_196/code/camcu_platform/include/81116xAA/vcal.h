/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  vcal.h
 * \date      05.11.2015
 * \brief     Master header file of Vector Algebra Library. Token: vcal
 * \author    DX.cba
 **************************************************************************************/

/*!
 *  \page vcal Vector Algebra Library
   The <b>V</b>e<b>c</b>tor <b>A</b>lgebra <b>L</b>ibrary (vcal) provides vector algebraic operations for Mlx811xy.
   \image latex VectorMath_Structure_Overview.pdf "Vector Algebra Library overview " width=1.0\textwidth
   The vcal description chapter is divided into four subchapters:
   \li \subpage vcalTypDesc
   \li \subpage vcalFncDesc
   \li \subpage vcalMacDesc
   \li \subpage vcalConsDesc

   \page vcalTypDesc Typedef Description
   This chapter describes following typedefs defined in the Vector Algebra Library:
   \li <tt> \subpage vcaltvec2D </tt>
   \li <tt> \subpage vcaltvec3D </tt>
   \li <tt> \subpage vcaltmat2x3 </tt>
   \li <tt> \subpage vcaltmat3x3 </tt>
   \li <tt> \subpage vcaltmat3x4 </tt>

   \page vcalFncDesc Function Description
   This chapter describes following functions defined in the Vector Algebra Library:
   \li <tt> \subpage vcalfnCalcDistVec2D </tt>
   \li <tt> \subpage vcalfnCalcIntersectionLineTriangle </tt>
   \li <tt> \subpage vcalfnCalcLinEquThroughWhitePnt </tt>
   \li <tt> \subpage vcalfnM3x3MulByVec3D </tt>
   \li <tt> \subpage vcalfnMagVec2D </tt>
   \li <tt> \subpage vcalfnMaxVec3D </tt>
   \li <tt> \subpage vcalfnMinVec3D </tt>
   \li <tt> \subpage vcalfnLimitVec3D </tt>
   \li <tt> \subpage vcalfnScaleVec3D </tt>
   \li <tt> \subpage vcalfnSolEquSys2 </tt>
   \li <tt> \subpage vcalfnSolEquSys3 </tt>
   \li <tt> \subpage vcalfnSubVec2D </tt>

   \page vcalMacDesc Macro Description
   This chapter describes following macros defined in the Vector Algebra Library:
   \li <tt> \subpage vcaldefVec2dDim </tt>
   \li <tt> \subpage vcaldefVec3dDim </tt>
   \li <tt> \subpage vcaldefVec4dDim </tt>
   \li <tt> \subpage vcaldefVec2dXindex </tt>
   \li <tt> \subpage vcaldefVec2dYindex </tt>
   \li <tt> \subpage vcaldefVec3dXindex </tt>
   \li <tt> \subpage vcaldefVec3dYindex </tt>
   \li <tt> \subpage vcaldefVec3dZindex </tt>
   \li <tt> \subpage vcaldefLinEquAindex </tt>
   \li <tt> \subpage vcaldefLinEquBindex </tt>
   \li <tt> \subpage vcaldefLinEquCindex </tt>
   \li <tt> \subpage vcaldefLinEquDindex </tt>
   \li <tt> \subpage vcaldefInvVec2dXsign </tt>
   \li <tt> \subpage vcaldefInvVec2dXexp </tt>
   \li <tt> \subpage vcaldefInvVec2dXman </tt>
   \li <tt> \subpage vcaldefInvVec2dX </tt>
   \li <tt> \subpage vcaldefInvVec2dYsign </tt>
   \li <tt> \subpage vcaldefInvVec2dYexp </tt>
   \li <tt> \subpage vcaldefInvVec2dYman </tt>
   \li <tt> \subpage vcaldefInvVec2dY </tt>
   \li <tt> \subpage vcaldefZeroVec2d </tt>

   \page vcalConsDesc Constants Description
   This chapter describes following constants defined in the Color Functional Library:
   \li <tt> \subpage vcalconInvVec2d </tt>
 */

#ifndef VCAL_H_
#define VCAL_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "vcal_Base_Constants.h"
#include "vcal_Base_Defines.h"
#include "vcal_Base_Typedefs.h"

#include "vcal_CalcDistVec2D.h"
#include "vcal_CalcIntersectionLineTriangle.h"
#include "vcal_CalcLinEquThroughWhitePnt.h"
#include "vcal_M3x3MulByVec3D.h"
#include "vcal_MagVec2D.h"
#include "vcal_MaxVec3D.h"
#include "vcal_MinVec3D.h"
#include "vcal_LimitVec3D.h"
#include "vcal_ScaleVec3D.h"
#include "vcal_SolEquSys2.h"
#include "vcal_SolEquSys3.h"
#include "vcal_SubVec2D.h"

#endif /* VCAL_H_ */
/* EOF */
