/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  colorLib_Version.h
 * \date      01.07.2015
 * \brief     COLOR_LIB version header file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef COLORLIB_VERSION_H_
#define COLORLIB_VERSION_H_

#include "colorLib_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_ColorLibVersion
   \addindex t_ColorLibVersion
   \page colorLibfnGetVersion_t_ColorLibVersion t_ColorLibVersion

   \details
   \b Description \n
   \n
   The function call of \ref colorLibfnGetVersion returns a struct containing the Color
   Library version information.

   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{ & \nameref{colorLibfnGetVersion_t_ColorLibVersion} & Color Library version number\\
   \hspace{5mm}major    & \nameref{colorLibtUInt16}                        & major version number\\
   \hspace{5mm}minor    & \nameref{colorLibtUInt16}                        & minor version number\\
 \}					  &                                                  & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_uint16 major; /**< COLOR_LIB major version */
    t_uint16 minor; /**< COLOR_LIB minor version */
} t_ColorLibVersion;


/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/

/***************************************************************************************
 * Declaration of variables             (scope: global)
 **************************************************************************************/

/***************************************************************************************
 * Exported function prototypes         (scope: global)
 **************************************************************************************/
extern t_ColorLibVersion colorLib_GetVersion(void);

#endif /* COLORLIB_VERSION_H_ */
/* EOF */
