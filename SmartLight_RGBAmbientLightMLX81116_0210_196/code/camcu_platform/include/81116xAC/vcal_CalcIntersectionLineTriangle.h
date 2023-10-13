/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  vcal_CalcIntersectionLineTriangle.h
 * \date      07.07.2015
 * \brief     Header file for vcal_CalcIntersectionLineTriangle function
 * \author    DX.pri
 **************************************************************************************/

#ifndef VCAL_INTERSECTIONLINETRIANGLE_H_
#define VCAL_INTERSECTIONLINETRIANGLE_H_

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
* Exported function prototypes         (scope: global)
***************************************************************************************/
extern t_mat2x3 vcal_CalcIntersectionLineTriangle(t_vec2D targetPoint, t_vec3D slope, t_vec3D offset);

#endif /* VCAL_INTERSECTIONLINETRIANGLE_H_ */

/* EOF */
