/*********************************************************************************
 * FileName:    AlgoMinAngle.h
 * Description: Minimum-angle white-ratio lookup interface.
 *
 * Unit:
 *   targetU / targetV : u'v' * 1000, same as inputColor->colorU/colorV
 *   whiteU  / whiteV  : single-white u'v' * 1000
 *   whiteRatioQ10000  : white ratio * 10000
 **********************************************************************************/
#ifndef ALGO_MIN_ANGLE_H
#define ALGO_MIN_ANGLE_H

#include <stdint.h>

#ifndef ALGO_MIN_ANGLE_DELTA_UV_LIMIT_Q1000
#define ALGO_MIN_ANGLE_DELTA_UV_LIMIT_Q1000    (10u) /* default Delta uv limit = 0.020 */
#endif

uint8_t AlgoMinAngle_GetWhiteRatioQ10000(uint16_t targetU,
                                         uint16_t targetV,
                                         uint16_t whiteU,
                                         uint16_t whiteV,
                                         uint16_t *whiteRatioQ10000);

#endif
