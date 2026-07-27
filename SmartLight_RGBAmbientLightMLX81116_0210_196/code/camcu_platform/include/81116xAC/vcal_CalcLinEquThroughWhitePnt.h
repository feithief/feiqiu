/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	 vcal_CalcLinEquThroughWhitePnt.h
 * \date     05.11.2015
 * \brief    Header file for vcal_CalcLinEquThroughWhitePnt function
 * \author   DX.cba
 **************************************************************************************/

#ifndef VCAL_CALCLINEQUTHROUGHWHITEPNT_H_
#define VCAL_CALCLINEQUTHROUGHWHITEPNT_H_

#include "vcal_Base_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/

/* PRQA S 292 26 */ /* Character $ is needed for Doxygen documentation. */
/*!
   \anchor t_CalcLinEqu2DResults
   \addindex t_CalcLinEqu2DResults
   \page vcalfnCalcLinEquThroughWhitePnt_tCalcLinEqu2DResults t_CalcLinEqu2DResults

   \details
   \b Description \n
   \n
   The calculation of \ref vcalfnCalcLinEquThroughWhitePnt returns a struct described below.

   \n
   \b Data \b Fields \n
   \latexonly
   \begin{tabularx}{\textwidth}{|l|l|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{structure / data field}} & \multicolumn{1}{c|}{\textbf{type}} & \multicolumn{1}{c|}{\textbf{description}}\\
   \hline
   {\textit{struct}} \{        & \nameref{vcalfnCalcLinEquThroughWhitePnt_tCalcLinEqu2DResults}  & results for the calculation\\
   \hspace{5mm}isLinEqu2DValid & \nameref{colorLibtBool}                                         & validation state of the calculation \newline CL\_FALSE - results invalid \newline CL\_TRUE - results valid\\
   \hspace{5mm}slope           & \nameref{bmltpFloat}                                            & slope $m$\\
   \hspace{5mm}offset          & \nameref{bmltpFloat}                                            & offset $n$\\
 \}							 &                                                                 & \\
   \hline
   \end{tabularx}
   \endlatexonly
 */
typedef struct {
    t_bool isLinEqu2DValid;
    t_pFloat slope;
    t_pFloat offset;
}t_CalcLinEqu2DResults;

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
t_CalcLinEqu2DResults vcal_CalcLinEquThroughWhitePnt(t_vec2D point);

#endif /* VCAL_CALCLINEQUTHROUGHWHITEPNT_H_ */
/* EOF */
