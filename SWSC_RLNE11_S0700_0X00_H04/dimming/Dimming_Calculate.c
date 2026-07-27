/*
 * File: Dimming_Calculate.c
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

#include "Dimming_Calculate.h"
#include "LookUp_U16_U16.h"

/* Exported data definition */

/* Const memory section */
/* Definition for custom storage class: Const */
const uint16_T ac_u16_X[11] = { 0U, 40U, 600U, 1200U, 1600U, 2000U, 2400U, 2800U,
    3200U, 3600U, 4000U } ;            /* Referenced by: '<S1>/Constant' */

const uint16_T ac_u16_Y[11] = { 0U, 40U, 68U, 200U, 400U, 720U, 1120U, 1640U,
    2280U, 3080U, 4000U } ;            /* Referenced by: '<S1>/Constant1' */

/* Model step function */
void Dimming_Calculate_Fun(uint16_T u16_In, uint16_T *u16_Out)
{
    /* local block i/o variables */
    uint16_T rtb_LookupTableDynamic;
    uint16_T rtb_TmpSignalConversionAtu16_InOutport1;

    /* Outputs for Function Call SubSystem: '<Root>/Dimming_Calculate_Fun' */
    /* SignalConversion generated from: '<S1>/u16_In' */
    rtb_TmpSignalConversionAtu16_InOutport1 = u16_In;

    /* S-Function (sfix_look1_dyn): '<S1>/Lookup Table Dynamic' incorporates:
     *  Constant: '<S1>/Constant'
     *  Constant: '<S1>/Constant1'
     */
    /* Dynamic Look-Up Table Block: '<S1>/Lookup Table Dynamic'
     * Input0  Data Type:  Integer        U16
     * Input1  Data Type:  Integer        U16
     * Input2  Data Type:  Integer        U16
     * Output0 Data Type:  Integer        U16
     * Lookup Method: Linear_Endpoint
     *
     */
    LookUp_U16_U16( &(rtb_LookupTableDynamic), (&(ac_u16_Y[0])),
                   rtb_TmpSignalConversionAtu16_InOutport1, (&(ac_u16_X[0])),
                   10U);

    /* SignalConversion generated from: '<S1>/u16_Out' */
    *u16_Out = rtb_LookupTableDynamic;

    /* End of Outputs for SubSystem: '<Root>/Dimming_Calculate_Fun' */
}

/* Model initialize function */
void Dimming_Calculate_initialize(void)
{
    /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
