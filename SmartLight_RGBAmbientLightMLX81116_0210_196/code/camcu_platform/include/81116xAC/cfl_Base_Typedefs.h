/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  cfl_Base_Typedefs.h
 * \date      04.11.2015
 * \brief     Color Functional Library type definition file.
 * \author    DX.cba
 **************************************************************************************/

#ifndef CFL_BASE_TYPEDEFS_H_
#define CFL_BASE_TYPEDEFS_H_

/***************************************************************************************
 * Includes					            (scope: global)
 **************************************************************************************/
#include "vcal_Base_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_Fading
   \addindex t_Fading
   \page cfltFading t_Fading
   \details
   \b Description \n
   \n
   Fading data type structure\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{                        & \nameref{cfltFading} & parameters for fading\\
   \hspace{5mm}{\textit{struct}} actualxyY \{  & \nameref{vcaltvec3D} & actual values for xyY\\
   \hspace{10mm}vec3D[\nameref{cfldefxInd}]    & \nameref{bmltpFloat} & x-component\\
   \hspace{10mm}vec3D[\nameref{cfldefyInd}]    & \nameref{bmltpFloat} & y-component\\
   \hspace{10mm}vec3D[\nameref{cfldefYInd}]    & \nameref{bmltpFloat} & Y-component\\
   \hspace{5mm}\}                              &                      & \\
   \hspace{5mm}actualYlinear                   & \nameref{bmltpFloat} & linear intensity value\\
 \}                                          &                      & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_vec3D actualxyY;
    t_pFloat actualYlinear;
} t_Fading;

/*!
   \anchor t_FadingParamsColor
   \addindex t_FadingParamsColor
   \page cfltFadingParamsColor t_FadingParamsColor
   \details
   \b Description \n
   \n
   Color fading parameters data type structure\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{  & \nameref{cfltFadingParamsColor} & color fading parameters\\
   \hspace{5mm}deltaFadx & \nameref{bmltpFloat}            & step size of x-component\\
   \hspace{5mm}deltaFady & \nameref{bmltpFloat}            & step size of y-component\\
 \}                    &                                 & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_pFloat deltaFadx;
    t_pFloat deltaFady;
}t_FadingParamsColor;

/*!
   \anchor t_FadingParamsIntensity
   \addindex t_FadingParamsIntensity
   \page cfltFadingParamsIntensity t_FadingParamsIntensity
   \details
   \b Description \n
   \n
   Intensity fading parameters data type structure\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{     & \nameref{cfltFadingParamsIntensity} & intensity fading parameters\\
   \hspace{5mm}deltaYlinear & \nameref{bmltpFloat}                & step size of linear intensity\\
   \hspace{5mm}Ylinear      & \nameref{bmltpFloat}                & distance between start-intensity and target-intensity\\
 \}                       &                                     & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_pFloat deltaYlinear;
    t_pFloat Ylinear;
}t_FadingParamsIntensity;

/*!
   \anchor t_FadingParams
   \addindex t_FadingParams
   \page cfltFadingParams t_FadingParams
   \details
   \b Description \n
   \n
   Fading parameters data type structure\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{                       & \nameref{cfltFadingParams}          & fading parameters\\
   \hspace{5mm}{\textit{struct}} fadParCol \{ & \nameref{cfltFadingParamsColor}     & color fading parameters\\
   \hspace{10mm}deltaFadx                     & \nameref{bmltpFloat}                & step size of x-component\\
   \hspace{10mm}deltaFady                     & \nameref{bmltpFloat}                & step size of y-component\\
   \hspace{5mm}\}                             &                                     & \\
   \hspace{5mm}{\textit{struct}} fadParInt \{ & \nameref{cfltFadingParamsIntensity} & intensity fading parameters\\
   \hspace{10mm}deltaYlinear                  & \nameref{bmltpFloat}                & step size of linear intensity\\
   \hspace{10mm}Ylinear                       & \nameref{bmltpFloat}                & distance between start-intensity and target-intensity\\
   \hspace{5mm}\}                             &                                     & \\
 \}                                         &                                     & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_FadingParamsColor fadParCol;
    t_FadingParamsIntensity fadParInt;
} t_FadingParams;

/*!
   \anchor t_CurSourceRange
   \addindex t_CurSourceRange
   \page cfltCurSourceRange t_CurSourceRange
   \details
   \b Description \n
   \n
   Current source strength.\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{enum name}} & \multicolumn{1}{c|}{\textbf{enum value}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   CFL\_CURRENT\_SOURCE\_6mA  & 0 & 6mA current source\\
   CFL\_CURRENT\_SOURCE\_30mA & 1 & 30mA current source\\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef enum {
    CFL_CURRENT_SOURCE_6mA = 0,
    CFL_CURRENT_SOURCE_30mA = 1
}t_CurSourceRange;

/*!
   \anchor t_CurrentSources
   \addindex t_CurrentSources
   \page cfltCurrentSources t_CurrentSources
   \details
   \b Description \n
   \n
   Current source array for RGB channels\n
   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6.9cm}|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{                                                          & \nameref{cfltCurrentSources} & current sources\\
   \hspace{5mm}curSource \newline \hspace*{5mm} [\nameref{cfldefCurSrcIndRed}]   & \nameref{cfltCurSourceRange} & current source red LED\\
   \hspace{5mm}curSource \newline \hspace*{5mm} [\nameref{cfldefCurSrcIndGreen}] & \nameref{cfltCurSourceRange} & current source green LED\\
   \hspace{5mm}curSource \newline \hspace*{5mm} [\nameref{cfldefCurSrcIndBlue}]  & \nameref{cfltCurSourceRange} & current source blue LED\\
 \}                                                                            &                              & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_CurSourceRange curSource[3];
}t_CurrentSources;

#endif /* CFL_BASE_TYPEDEFS_H_ */
/* EOF */
