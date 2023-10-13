/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  vcal_Base_Defines.h
 * \date      27.07.2015
 * \brief     Vector Algebra defines file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef VCAL_BASE_DEFINES_H_
#define VCAL_BASE_DEFINES_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "bml_Base_Defines.h"

/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/
/*!
   \anchor VCAL_VEC2D_DIM
   \addindex VCAL_VEC2D_DIM
   \page vcaldefVec2dDim   VCAL_VEC2D_DIM
   \details Constant representing the dimension of a two-dimensional vector, equal to 2.
 */
#define VCAL_VEC2D_DIM 2u

/*!
   \anchor VCAL_VEC3D_DIM
   \addindex VCAL_VEC3D_DIM
   \page vcaldefVec3dDim   VCAL_VEC3D_DIM
   \details Constant representing the dimension of a three-dimensional vector, equal to 3.
 */
#define VCAL_VEC3D_DIM 3u

/*!
   \anchor VCAL_VEC4D_DIM
   \addindex VCAL_VEC4D_DIM
   \page vcaldefVec4dDim   VCAL_VEC4D_DIM
   \details Constant representing the dimension of a four-dimensional vector, equal to 4.
 */
#define VCAL_VEC4D_DIM 4u

/*!
   \anchor VCAL_VEC2D_X_INDEX
   \addindex VCAL_VEC2D_X_INDEX
   \page vcaldefVec2dXindex   VCAL_VEC2D_X_INDEX
   \details Constant representing the array index from the x-component of a two-dimensional vector, equal to 0.
 */
#define VCAL_VEC2D_X_INDEX 0u

/*!
   \anchor VCAL_VEC2D_Y_INDEX
   \addindex VCAL_VEC2D_Y_INDEX
   \page vcaldefVec2dYindex   VCAL_VEC2D_Y_INDEX
   \details Constant representing the array index from the y-component of a two-dimensional vector, equal to 1.
 */
#define VCAL_VEC2D_Y_INDEX 1u

/*!
   \anchor VCAL_VEC3D_X_INDEX
   \addindex VCAL_VEC3D_X_INDEX
   \page vcaldefVec3dXindex   VCAL_VEC3D_X_INDEX
   \details Constant representing the array index from the x-component of a three-dimensional vector, equal to 0.
 */
#define VCAL_VEC3D_X_INDEX VCAL_VEC2D_X_INDEX

/*!
   \anchor VCAL_VEC3D_Y_INDEX
   \addindex VCAL_VEC3D_Y_INDEX
   \page vcaldefVec3dYindex   VCAL_VEC3D_Y_INDEX
   \details Constant representing the array index from the y-component of a three-dimensional vector, equal to 1.
 */
#define VCAL_VEC3D_Y_INDEX VCAL_VEC2D_Y_INDEX

/*!
   \anchor VCAL_VEC3D_Z_INDEX
   \addindex VCAL_VEC3D_Z_INDEX
   \page vcaldefVec3dZindex   VCAL_VEC3D_Z_INDEX
   \details Constant representing the array index from the z-component of a three-dimensional vector, equal to 2.
 */
#define VCAL_VEC3D_Z_INDEX 2u

/*!
   \anchor VCAL_LIN_EQU_A_INDEX
   \addindex VCAL_LIN_EQU_A_INDEX
   \page vcaldefLinEquAindex   VCAL_LIN_EQU_A_INDEX
   \details Constant representing the index from linear equation constant A, equal to 0.
 */
#define VCAL_LIN_EQU_A_INDEX 0u

/*!
   \anchor VCAL_LIN_EQU_B_INDEX
   \addindex VCAL_LIN_EQU_B_INDEX
   \page vcaldefLinEquBindex   VCAL_LIN_EQU_B_INDEX
   \details Constant representing the index from linear equation constant B, equal to 1.
 */
#define VCAL_LIN_EQU_B_INDEX 1u

/*!
   \anchor VCAL_LIN_EQU_C_INDEX
   \addindex VCAL_LIN_EQU_C_INDEX
   \page vcaldefLinEquCindex   VCAL_LIN_EQU_C_INDEX
   \details Constant representing the index from linear equation constant C, equal to 2.
 */
#define VCAL_LIN_EQU_C_INDEX 2u

/*!
   \anchor VCAL_LIN_EQU_D_INDEX
   \addindex VCAL_LIN_EQU_D_INDEX
   \page vcaldefLinEquDindex   VCAL_LIN_EQU_D_INDEX
   \details Constant representing the index from linear equation constant D, equal to 3.
 */
#define VCAL_LIN_EQU_D_INDEX 3u

/*!
   \anchor VCAL_INVALID_VEC_2D_X_SIGN
   \addindex VCAL_INVALID_VEC_2D_X_SIGN
   \page vcaldefInvVec2dXsign   VCAL_INVALID_VEC_2D_X_SIGN
   \details Struct element (sign) of constant representing the x-component of a two-dimensional vector. It represents an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_X_SIGN  FLOAT_10_SIGN

/*!
   \anchor VCAL_INVALID_VEC_2D_X_EXP
   \addindex VCAL_INVALID_VEC_2D_X_EXP
   \page vcaldefInvVec2dXexp   VCAL_INVALID_VEC_2D_X_EXP
   \details Struct element (exponent) of constant representing the x-component of a two-dimensional vector. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_X_EXP   FLOAT_10_EXP

/*!
   \anchor VCAL_INVALID_VEC_2D_X_MAN
   \addindex VCAL_INVALID_VEC_2D_X_MAN
   \page vcaldefInvVec2dXman   VCAL_INVALID_VEC_2D_X_MAN
   \details Struct element (mantissa) of constant representing the x-component of a two-dimensional vector. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_X_MAN   FLOAT_10_MAN

/*!
   \anchor VCAL_INVALID_VEC_2D_X
   \addindex VCAL_INVALID_VEC_2D_X
   \page vcaldefInvVec2dX   VCAL_INVALID_VEC_2D_X
   \details Constant representing the x-component of a two-dimensional vector, equal to \ref FLOAT_10. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_X       {VCAL_INVALID_VEC_2D_X_SIGN, VCAL_INVALID_VEC_2D_X_EXP, VCAL_INVALID_VEC_2D_X_MAN}

/*!
   \anchor VCAL_INVALID_VEC_2D_Y_SIGN
   \addindex VCAL_INVALID_VEC_2D_Y_SIGN
   \page vcaldefInvVec2dYsign   VCAL_INVALID_VEC_2D_Y_SIGN
   \details Struct element (sign) of constant representing the y-component of a two-dimensional vector. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_Y_SIGN  FLOAT_10_SIGN

/*!
   \anchor VCAL_INVALID_VEC_2D_Y_EXP
   \addindex VCAL_INVALID_VEC_2D_Y_EXP
   \page vcaldefInvVec2dYexp   VCAL_INVALID_VEC_2D_Y_EXP
   \details Struct element (exponent) of constant representing the y-component of a two-dimensional vector. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_Y_EXP   FLOAT_10_EXP

/*!
   \anchor VCAL_INVALID_VEC_2D_Y_MAN
   \addindex VCAL_INVALID_VEC_2D_Y_MAN
   \page vcaldefInvVec2dYman   VCAL_INVALID_VEC_2D_Y_MAN
   \details Struct element (mantissa) of constant representing the y-component of a two-dimensional vector. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_Y_MAN   FLOAT_10_MAN

/*!
   \anchor VCAL_INVALID_VEC_2D_Y
   \addindex VCAL_INVALID_VEC_2D_Y
   \page vcaldefInvVec2dY   VCAL_INVALID_VEC_2D_Y
   \details Constant representing the y-component of a two-dimensional vector, equal to \ref FLOAT_10. It represent an invalid input value.
 */
#define VCAL_INVALID_VEC_2D_Y       {VCAL_INVALID_VEC_2D_Y_SIGN, VCAL_INVALID_VEC_2D_Y_EXP, VCAL_INVALID_VEC_2D_Y_MAN}

/*!
   \anchor VCAL_ZERO_VEC_2D
   \addindex VCAL_ZERO_VEC_2D
   \page vcaldefZeroVec2d   VCAL_ZERO_VEC_2D
   \details  Constant representing two-dimensional zero vector.
 */
#define VCAL_ZERO_VEC_2D    {FLOAT_0, FLOAT_0}


#endif /* VCAL_BASE_DEFINES_H_ */
/* EOF */
