/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	 cfl_CalcFadingParamsIntensity.h
 * \date     13.06.2016
 * \brief    Header file for cfl_CalcFadingParamsIntensity function
 * \author   DX.cba, DX.bsc
 * \details  multi line additional description
 **************************************************************************************/

#ifndef CFL_CALCFADINGPARAMSINTENSITY_H_
#define CFL_CALCFADINGPARAMSINTENSITY_H_

#include "cfl_Base_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/

/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/

/***************************************************************************************
* Declaration of variables             (scope: global)
***************************************************************************************/

/***************************************************************************************
* Inline functions				       (scope: global)
***************************************************************************************/

/***************************************************************************************
* Exported function prototypes         (scope: global)
***************************************************************************************/
extern t_FadingParamsIntensity cfl_CalcFadingParamsIntensity(t_pFloat actualY,
                                                             t_vec3D targetxyY,
                                                             t_pFloat stepsizeIntensity);

#endif /* CFL_CALCFADINGPARAMSINTENSITY_H_ */
/* EOF */
