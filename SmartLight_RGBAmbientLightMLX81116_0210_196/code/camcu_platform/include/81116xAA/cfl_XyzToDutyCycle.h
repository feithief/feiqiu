/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	 cfl_XyzToDutyCycle.h
 * \date     16.07.2015
 * \brief    Header file for cfl_XyzToDutyCycle function
 * \author   DX.cba
 **************************************************************************************/

#ifndef CFL_XYZTODUTYCYCLE_H_
#define CFL_XYZTODUTYCYCLE_H_

#include "vcal_Base_Typedefs.h"
#include "cfl_Base_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_LedTarget
   \addindex t_LedTarget
   \page cflXyzToDutyCycletLedTarget t_LedTarget

   \details
   \b Description \n
   \n
   This complex data type contains of the three target PWM duty cycle values for red,
   green and blue and the corresponding setting of the three current sources.

   \n
   \b Data \b Fields \n
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{7.5cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{                                                           & \nameref{cflXyzToDutyCycletLedTarget} & parameters for LED control \\
   \hspace{5mm}{\textit{struct}} rgbDutyCycles \{                                 & \nameref{vcaltvec3D}                  & duty cycles RGB\\
   \hspace{10mm}vec3D \newline \hspace*{10mm}[\nameref{cfldefLedIndRed}]          & \nameref{bmltpFloat}                  & duty cycle red LED\\
   \hspace{10mm}vec3D \newline \hspace*{10mm}[\nameref{cfldefLedIndGreen}]        & \nameref{bmltpFloat}                  & duty cycle green LED\\
   \hspace{10mm}vec3D \newline \hspace*{10mm}[\nameref{cfldefLedIndBlue}]         & \nameref{bmltpFloat}                  & duty cycle blue LED\\
   \hspace{5mm}\}                                                                 &                                       & \\
   \hspace{5mm}{\textit{struct}} curSrc \{                                        & \nameref{cfltCurrentSources}          & current sources\\
   \hspace{10mm}curSource \newline \hspace*{10mm}[\nameref{cfldefCurSrcIndRed}]   & \nameref{cfltCurSourceRange}          & current source red LED\\
   \hspace{10mm}curSource \newline \hspace*{10mm}[\nameref{cfldefCurSrcIndGreen}] & \nameref{cfltCurSourceRange}          & current source green LED\\
   \hspace{10mm}curSource \newline \hspace*{10mm}[\nameref{cfldefCurSrcIndBlue}]  & \nameref{cfltCurSourceRange}          & current source blue LED\\
   \hspace{5mm}\}                                                                 &                                       & \\
 \}                                                                             &                                       & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_vec3D rgbDutyCycles;
    t_CurrentSources curSrc;
}t_LedTarget;

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
extern t_LedTarget cfl_XyzToDutyCycle(t_vec3D vecInXyz, t_int16 temperature);

#endif /* CFL_XYZTODUTYCYCLE_H_ */
/* EOF */
