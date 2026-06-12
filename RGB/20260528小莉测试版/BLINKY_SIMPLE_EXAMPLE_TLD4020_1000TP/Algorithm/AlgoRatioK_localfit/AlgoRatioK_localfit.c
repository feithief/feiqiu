/*********************************************************************************
 * FileName:    AlgoRatioK_localfit.c
 * Description: Wrapper entry for localfit white-ratio lookup implementations.
 *
 * Switch:
 *   ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE = 1 -> runtime-grid + uint16 ratio LUT
 *   ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE = 0 -> previous packed32-compatible LUT
 **********************************************************************************/

#include "AlgoRatioK_localfit.h"

#if (ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE != 0u)
#include "AlgoRatioK_localfit_ratio16_runtime.c"
#else
#include "AlgoRatioK_localfit_packed32_compat.c"
#endif