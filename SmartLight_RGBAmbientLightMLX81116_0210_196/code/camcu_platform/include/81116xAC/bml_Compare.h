/***************************************************************************************
 * Copyright (C) 2014-2016 Melexis N.V.
 * git flash 137e5489
 ***********************************************************************************//*!
 * \file	  bml_Compare.h
 * \date      05.11.2015
 * \brief     Header file for bml_Compare function
 * \author    DX.cba
 **************************************************************************************/

#ifndef BML_COMPARE_H_
#define BML_COMPARE_H_

#include "bml_Base_Typedefs.h"

/***************************************************************************************
* Typedefs					           (scope: global)
***************************************************************************************/
/*!
   \anchor t_BmlCompareResults
   \addindex t_BmlCompareResults
   \page bmlfnCompareBmlCompareResults t_BmlCompareResults
   \details
   \b Description \n
   \n
   This enumeration data type is used by the \ref bmlfnCompare operation of the Basic Math Library.
   It describes the three possible results from a comparison of two floating point numbers
   from type \ref t_pFloat.\n
   \n
   \n
   \b Enumeration \b details
   \n
   \latexonly
   \begin{tabularx}{\textwidth}{|p{6cm}|X|}
   \hline
   \rowcolor{lightgray}\multicolumn{1}{|c|}{\textbf{enum name}} & \multicolumn{1}{c|}{\textbf{enum value}}\\
   \hline
   BML\_COMPARE\_EQUAL   & 0\\
   \hline
   BML\_COMPARE\_GREATER & 1\\
   \hline
   BML\_COMPARE\_LESS    & 2\\
   \hline
   \end{tabularx}
   \endlatexonly
 */typedef enum {
    BML_COMPARE_EQUAL = 0,
    BML_COMPARE_GREATER = 1,
    BML_COMPARE_LESS = 2
}t_BmlCompareResults;

/***************************************************************************************
* Constants and macros                 (scope: global)
***************************************************************************************/

/***************************************************************************************
* Declaration of variables             (scope: global)
***************************************************************************************/

/***************************************************************************************
* Exported function prototypes         (scope: global)
***************************************************************************************/
extern t_BmlCompareResults bml_Compare(t_pFloat flnumber1,t_pFloat flnumber2);

#endif /* BML_COMPARE_H_ */
/* EOF */
