/*
 * File: LookUp_U16_U16.c
 *
 * Code generated for Simulink model 'Dimming_Calculate'.
 *
 * Model version                  : 1.6
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri Mar  8 10:46:28 2024
 */

#include "rtwtypes.h"
#include "BINARYSEARCH_U16.h"
#include "INTERPOLATE_U16_U16.h"
#include "LookUp_U16_U16.h"

/* Lookup Utility LookUp_U16_U16 */
void LookUp_U16_U16(uint16_T *pY, const uint16_T *pYData, uint16_T u, const
                    uint16_T *pUData, uint32_T iHi)
{
    uint32_T iLeft;
    uint32_T iRght;
    BINARYSEARCH_U16( &(iLeft), &(iRght), u, pUData, iHi);
    INTERPOLATE_U16_U16( pY, pYData[iLeft], pYData[iRght], u, pUData[iLeft],
                        pUData[iRght]);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
