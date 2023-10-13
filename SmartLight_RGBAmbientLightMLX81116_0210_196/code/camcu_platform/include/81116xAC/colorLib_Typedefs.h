/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  colorLib_Typedefs.h
 * \date      04.11.2015
 * \brief     COLOR_LIB Basic types definition file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef COLORLIB_TYPEDEFS_H_
#define COLORLIB_TYPEDEFS_H_

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_int8
   \addindex t_int8
   \page colorLibtInt8 t_int8
   \details Signed 8 Bit integer data type
 */
typedef signed char t_int8;

/*!
   \anchor t_uint8
   \addindex t_uint8
   \page colorLibtUInt8 t_uint8
   \details Unsigned 8 Bit integer data type
 */
typedef unsigned char t_uint8;

/*!
   \anchor t_int16
   \addindex t_int16
   \page colorLibtInt16 t_int16
   \details Signed 16 Bit integer data type
 */
typedef signed int t_int16;

/*!
   \anchor t_uint16
   \addindex t_uint16
   \page colorLibtUInt16 t_uint16
   \details Unsigned 16 Bit integer data type
 */
typedef unsigned int t_uint16;

/*!
   \anchor t_int32
   \addindex t_int32
   \page colorLibtInt32 t_int32
   \details Signed 32 Bit integer data type
 */
typedef signed long int t_int32;

/*!
   \anchor t_uint32
   \addindex t_uint32
   \page colorLibtUInt32 t_uint32
   \details Unsigned 32 Bit integer data type
 */
typedef unsigned long int t_uint32;

/*!
   \anchor t_bool
   \addindex t_bool
   \page colorLibtBool t_bool
   \details
   \b Description \n
   \n
   Boolean data type enumeration\n
   \n
   \n
   \b Enumeration \b details
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6cm}|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{enum name}} & \multicolumn{1}{c|}{\textbf{enum value}}\\
   \hline
   CL\_FALSE & 0\\
   \hline
   CL\_TRUE & 1\\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef enum {
    CL_FALSE = 0,   /**< True */
    CL_TRUE = 1     /**< False */
}t_bool;

#endif /* COLORLIB_TYPEDEFS_H_ */
/* EOF */
