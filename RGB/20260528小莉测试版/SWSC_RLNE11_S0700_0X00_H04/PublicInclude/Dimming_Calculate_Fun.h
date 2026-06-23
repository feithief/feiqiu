/*
 * File: Dimming_Calculate.h
 *
 * Code generated for Simulink model 'Dimming_Calculate'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri Mar  8 10:54:22 2024
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: NXP->Cortex-M4
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Dimming_Calculate_h_
#define RTW_HEADER_Dimming_Calculate_h_
#ifndef Dimming_Calculate_COMMON_INCLUDES_
#define Dimming_Calculate_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Dimming_Calculate_COMMON_INCLUDES_ */

/* Child system includes */
#include "Dimming_Calculate_Fun.h"

/* Model Code Variants */

/* Macros for accessing real-time model data structure */

/* Model entry point functions */
extern void Dimming_Calculate_initialize(void);

/* Exported data declaration */

/* Const memory section */
/* Declaration for custom storage class: Const */
extern const uint16_T ac_u16_X[11];    /* Referenced by: '<S1>/Constant' */
extern const uint16_T ac_u16_Y[11];    /* Referenced by: '<S1>/Constant1' */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'Dimming_Calculate'
 * '<S1>'   : 'Dimming_Calculate/Dimming_Calculate_Fun'
 */
#endif                                 /* RTW_HEADER_Dimming_Calculate_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
