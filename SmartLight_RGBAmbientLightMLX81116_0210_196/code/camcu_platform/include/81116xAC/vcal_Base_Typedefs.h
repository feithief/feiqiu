/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  vcal_Base_Typedefs.h
 * \date      04.11.2015
 * \brief     Vector Algebra Library type definition file.
 * \author    DX.pri
 **************************************************************************************/

#ifndef VCAL_BASE_TYPEDEFS_H_
#define VCAL_BASE_TYPEDEFS_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "bml_Base_Typedefs.h"
#include "vcal_Base_Defines.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_vec2D
   \addindex t_vec2D
   \page vcaltvec2D t_vec2D
   \details
   \b Description \n
   \n
   Struct encapsulated one-dimensional array representing two-dimensional vector type in a
   general cartesian coordinate system (x,y).\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   vec2D[\nameref{vcaldefVec2dXindex}] & \nameref{bmltpFloat} & x-component of a two-dimensional vector\\
   \hline
   vec2D[\nameref{vcaldefVec2dYindex}] & \nameref{bmltpFloat} & y-component of a two-dimensional vector\\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_pFloat vec2D[VCAL_VEC2D_DIM]; /**< array including x- and y-component of a two-dimensional vector */
}t_vec2D;

/*!
   \anchor t_vec3D
   \addindex t_vec3D
   \page vcaltvec3D t_vec3D
   \details
   \b Description \n
   \n
   Struct encapsulated one-dimensional array representing three-dimensional vector type in a
   general cartesian coordinate system (x,y,z).\n
   \n
   \b Data \b Fields
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   vec3D[\nameref{vcaldefVec3dXindex}] & \nameref{bmltpFloat} & x-component of a three-dimensional vector\\
   \hline
   vec3D[\nameref{vcaldefVec3dYindex}] & \nameref{bmltpFloat} & y-component of a three-dimensional vector\\
   \hline
   vec3D[\nameref{vcaldefVec3dZindex}] & \nameref{bmltpFloat} & z-component of a three-dimensional vector\\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_pFloat vec3D[VCAL_VEC3D_DIM]; /**< array including x-, y- and z-component of a three-dimensional vector */
}t_vec3D;

/*!
   \anchor t_mat2x3
   \addindex t_mat2x3
   \page vcaltmat2x3 t_mat2x3
   \details
   \b Description \n
   \n
   Struct encapsulated two-dimensional array representing matrix of type 2x3.\n
   \n
   \b Data \b Fields
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6.5cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   mat2x3[0][0] & \nameref{bmltpFloat} & (0,0)-component of a 2x3 matrix\\
   \hline
   mat2x3[0][1] & \nameref{bmltpFloat} & (0,1)-component of a 2x3 matrix\\
   \hline
   mat2x3[0][2] & \nameref{bmltpFloat} & (0,2)-component of a 2x3 matrix\\
   \hline
   mat2x3[1][0] & \nameref{bmltpFloat} & (1,0)-component of a 2x3 matrix\\
   \hline
   mat2x3[1][1] & \nameref{bmltpFloat} & (1,1)-component of a 2x3 matrix\\
   \hline
   mat2x3[1][2] & \nameref{bmltpFloat} & (1,2)-component of a 2x3 matrix\\
   \hline
   \end{tabularx}
   \endlatexonly
   \n
   <b>note:</b> access to the array in following way: mat2x3[row index][column index]
 */
typedef struct {
    t_pFloat mat2x3[VCAL_VEC2D_DIM][VCAL_VEC3D_DIM];
}t_mat2x3;

/*!
   \anchor t_mat3x3
   \addindex t_mat3x3
   \page vcaltmat3x3 t_mat3x3
   \details
   \b Description \n
   \n
   Struct encapsulated two-dimensional array representing matrix of type 3x3.\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6.5cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   mat3x3[0][0] & \nameref{bmltpFloat} & (0,0)-component of a 3x3 matrix\\
   \hline
   mat3x3[0][1] & \nameref{bmltpFloat} & (0,1)-component of a 3x3 matrix\\
   \hline
   mat3x3[0][2] & \nameref{bmltpFloat} & (0,2)-component of a 3x3 matrix\\
   \hline
   mat3x3[1][0] & \nameref{bmltpFloat} & (1,0)-component of a 3x3 matrix\\
   \hline
   mat3x3[1][1] & \nameref{bmltpFloat} & (1,1)-component of a 3x3 matrix\\
   \hline
   mat3x3[1][2] & \nameref{bmltpFloat} & (1,2)-component of a 3x3 matrix\\
   \hline
   mat3x3[2][0] & \nameref{bmltpFloat} & (2,0)-component of a 3x3 matrix\\
   \hline
   mat3x3[2][1] & \nameref{bmltpFloat} & (2,1)-component of a 3x3 matrix\\
   \hline
   mat3x3[2][2] & \nameref{bmltpFloat} & (2,2)-component of a 3x3 matrix\\
   \hline
   \end{tabularx}
   \endlatexonly
   \n
   <b>note:</b> access to the array in following way: mat3x3[row index][column index]
 */
typedef struct {
    t_pFloat mat3x3[VCAL_VEC3D_DIM][VCAL_VEC3D_DIM];
}t_mat3x3;

/*!
   \anchor t_mat3x4
   \addindex t_mat3x4
   \page vcaltmat3x4 t_mat3x4
   \details
   \b Description \n
   Struct encapsulated two-dimensional array representing matrix of type 3x4.\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6.5cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   mat3x4[0][0] & \nameref{bmltpFloat} & (0,0)-component of a 3x4 matrix\\
   \hline
   mat3x4[0][1] & \nameref{bmltpFloat} & (0,1)-component of a 3x4 matrix\\
   \hline
   mat3x4[0][2] & \nameref{bmltpFloat} & (0,2)-component of a 3x4 matrix\\
   \hline
   mat3x4[0][3] & \nameref{bmltpFloat} & (0,3)-component of a 3x4 matrix\\
   \hline
   mat3x4[1][0] & \nameref{bmltpFloat} & (1,0)-component of a 3x4 matrix\\
   \hline
   mat3x4[1][1] & \nameref{bmltpFloat} & (1,1)-component of a 3x4 matrix\\
   \hline
   mat3x4[1][2] & \nameref{bmltpFloat} & (1,2)-component of a 3x4 matrix\\
   \hline
   mat3x4[1][3] & \nameref{bmltpFloat} & (1,3)-component of a 3x4 matrix\\
   \hline
   mat3x4[2][0] & \nameref{bmltpFloat} & (2,0)-component of a 3x4 matrix\\
   \hline
   mat3x4[2][1] & \nameref{bmltpFloat} & (2,1)-component of a 3x4 matrix\\
   \hline
   mat3x4[2][2] & \nameref{bmltpFloat} & (2,2)-component of a 3x4 matrix\\
   \hline
   mat3x4[2][3] & \nameref{bmltpFloat} & (2,3)-component of a 3x4 matrix\\
   \hline
   \end{tabularx}
   \endlatexonly
   \n
   <b>note:</b> access to the array in following way: mat3x4[row index][column index]
 */
typedef struct {
    t_pFloat mat3x4[VCAL_VEC3D_DIM][VCAL_VEC4D_DIM];
}t_mat3x4;

#endif /* VCAL_BASE_TYPEDEFS_H_ */
/* EOF */
