/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	 cfl_RgbToxyY.h
 * \date     28.12.2017
 * \brief    Header file for cfl_RgbIToxyY function
 * \author   rfo
 **************************************************************************************/

#ifndef CFL_RGBTOXYY_H_
#define CFL_RGBTOXYY_H_

#include "vcal_Base_Typedefs.h"

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
extern t_vec3D cfl_RgbToxyY(t_uint16 redPart, t_uint16 greenPart, t_uint16 bluePart);

#endif /* CFL_RGBTOXYY_H_ */
/* EOF */
