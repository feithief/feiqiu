/*********************************************************************************
 * FileName:    AlgoRatioK_localfit.h
 * Description: Runtime-grid local weighted least-squares white-ratio lookup interface.
 *
 * Unit:
 *   targetU / targetV : u'v' * 1000
 *   whiteRatioQ10000  : white ratio * 10000
 *
 * Algorithm:
 *   1. Generate nearest ALGO_RATIO_K_AMOUNT grid points online.
 *   2. Use each point's LUT index to read ratioQ10000 from a uint16 table.
 *   3. Fit ratio = a * (U - targetU) + b * (V - targetV) + c.
 *   4. Return c as predicted target white ratio.
 *   5. Fall back to inverse-distance weighted average if fit is singular.
 **********************************************************************************/
#ifndef ALGO_RATIO_K_LOCALFIT_H
#define ALGO_RATIO_K_LOCALFIT_H

#include <stdint.h>

#ifndef ALGO_RATIO_K_AMOUNT
#define ALGO_RATIO_K_AMOUNT              (9u)
#endif

#ifndef ALGO_RATIO_K_FAST_NEAREST_ENABLE
#define ALGO_RATIO_K_FAST_NEAREST_ENABLE (1u)
#endif
#ifndef ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE
#define ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE (1u)
#endif

uint8_t AlgoRatioK_GetWhiteRatioQ10000(uint16_t targetU,
                                       uint16_t targetV,
                                       uint16_t *whiteRatioQ10000);

#endif
