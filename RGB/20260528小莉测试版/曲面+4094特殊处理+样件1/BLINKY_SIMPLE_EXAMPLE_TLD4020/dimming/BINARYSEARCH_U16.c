/*
 * File: BINARYSEARCH_U16.c
 *
 * Code generated for Simulink model 'Dimming_Calculate'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri Mar  8 10:46:28 2024
 */

#include "rtwtypes.h"
#include "BINARYSEARCH_U16.h"

/* Lookup Binary Search Utility BINARYSEARCH_U16 */
void BINARYSEARCH_U16(uint32_T *piLeft, uint32_T *piRght, uint16_T u, const
                      uint16_T *pData, uint32_T iHi)
{
    /* Find the location of current input value in the data table. */
    *piLeft = 0U;
    *piRght = iHi;
    if (u <= pData[0] ) {
        /* Less than or equal to the smallest point in the table. */
        *piRght = 0U;
    } else if (u >= pData[iHi] ) {
        /* Greater than or equal to the largest point in the table. */
        *piLeft = iHi;
    } else {
        uint32_T i;

        /* Do a binary search. */
        while (( *piRght - *piLeft ) > 1U ) {
            /* Get the average of the left and right indices using to Floor rounding. */
            i = (*piLeft + *piRght) >> 1;

            /* Move either the right index or the left index so that */
            /*  LeftDataPoint <= CurrentValue < RightDataPoint */
            if (u < pData[i] ) {
                *piRght = i;
            } else {
                *piLeft = i;
            }
        }
    }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
