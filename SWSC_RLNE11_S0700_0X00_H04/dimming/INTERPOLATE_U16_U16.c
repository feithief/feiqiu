/*
 * File: INTERPOLATE_U16_U16.c
 *
 * Code generated for Simulink model 'Dimming_Calculate'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri Mar  8 10:46:28 2024
 */

#include "rtwtypes.h"
#include "INTERPOLATE_U16_U16.h"

/* Lookup Interpolation INTERPOLATE_U16_U16 */
void INTERPOLATE_U16_U16(uint16_T *pY, uint16_T yL, uint16_T yR, uint16_T x,
    uint16_T xL, uint16_T xR)
{
    uint32_T bigProd;
    uint16_T yDiff;
    uint16_T xNum;
    uint16_T xDen;
    *pY = yL;

    /* If x is not strictly between xR and xL
     * then an interpolation calculation is not necessary x == xL
     * or not valid.  The invalid situation is expected when the input
     * is beyond the left or right end of the table.  The design is
     * that yL holds the correct value for *pY
     * in invalid situations.
     */
    if ((xR > xL) && (x > xL) ) {
        xDen = xR;
        xDen = (uint16_T)(uint32_T)((uint32_T)xDen - (uint32_T)xL);
        xNum = x;
        xNum = (uint16_T)(uint32_T)((uint32_T)xNum - (uint32_T)xL);
        if (yR >= yL ) {
            yDiff = yR;
            yDiff = (uint16_T)(uint32_T)((uint32_T)yDiff - (uint32_T)yL);
        } else {
            yDiff = yL;
            yDiff = (uint16_T)(uint32_T)((uint32_T)yDiff - (uint32_T)yR);
        }

        bigProd = (uint32_T)((uint32_T)yDiff * (uint32_T)xNum);
        yDiff = (uint16_T)(uint32_T)(bigProd / (uint32_T)xDen);
        if (yR >= yL ) {
            *pY = (uint16_T)(uint32_T)((uint32_T)*pY + (uint32_T)yDiff);
        } else {
            *pY = (uint16_T)(uint32_T)((uint32_T)*pY - (uint32_T)yDiff);
        }
    }
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
