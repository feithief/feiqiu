/*********************************************************************************
  * FileName:    AlgoRatioK_full.h
  * Description: K-point white-ratio lookup interface.
  *
  * Unit:
  *   targetU / targetV : u'v' * 1000, same as inputColor->colorU/colorV
  *   whiteRatioQ10000  : white ratio * 10000
**********************************************************************************/
#ifndef ALGO_RATIO_K_H
#define ALGO_RATIO_K_H

#include <stdint.h>

#ifndef ALGO_RATIO_K_AMOUNT
#define ALGO_RATIO_K_AMOUNT              (6u)
#endif

uint8_t AlgoRatioK_GetWhiteRatioQ10000(uint16_t targetU,
                                       uint16_t targetV,
                                       uint16_t *whiteRatioQ10000);

#endif
