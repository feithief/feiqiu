/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	 cfl_CalcDutyCycleFromXYZSingCurrMultLED.h
 * \date     06.08.2018
 * \brief    Header file for cfl_CalcDutyCycleFromXYZSingCurrMultLED function
 * \author   rfo
 **************************************************************************************/

#ifndef CFL_CALCDUTYCYCLEFROMXYZSINGCURRMULTLED_H_
#define CFL_CALCDUTYCYCLEFROMXYZSINGCURRMULTLED_H_

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
extern t_vec3D cfl_CalcDutyCycleFromXYZSingCurrMultLED(t_uint16 ledIndex, t_vec3D vecXYZ, t_int16 temperature);

#endif /* CFL_CALCDUTYCYCLEFROMXYZSINGCURRMULTLED_H_ */
/* EOF */
