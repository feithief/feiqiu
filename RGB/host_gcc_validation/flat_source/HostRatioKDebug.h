#ifndef _HOST_RATIO_K_DEBUG_H_
#define _HOST_RATIO_K_DEBUG_H_

#include <stdint.h>
#include "SystemType.h"

typedef struct HostRatioKDebug
{
    uint16_t lookupColorU;
    uint16_t lookupColorV;
    uint16_t inputRgbPointX;
    uint16_t inputRgbPointY;
    uint16_t inputWhitePointX;
    uint16_t inputWhitePointY;
    uint16_t whiteRatioQ10000;
    uint8_t  ratioValid;
    uint8_t  splitValid;
    int32_t  targetColor_x;
    int32_t  targetColor_y;
    int32_t  targetColor_Y;
    int32_t  whiteRatioBeforeClamp;
    int32_t  maxWhiteRatio;
    int32_t  whiteRatioAfterClamp;
    int32_t  boundaryWhiteY;
    int32_t  targetColorY;
    int32_t  whiteRatio;
    int32_t  rgbRatio;
    int32_t  splitPointCIE_x;
    int32_t  splitPointCIE_y;
    int32_t  splitPointCIE_Y;
    int32_t  splitWhiteCIE_x;
    int32_t  splitWhiteCIE_y;
    int32_t  splitWhiteCIE_Y;
    int32_t  dutyColor_x;
    int32_t  dutyColor_y;
    int32_t  dutyColor_Y;
    int32_t  verifyRgbPointErrX;
    int32_t  verifyRgbPointErrY;
    int32_t  verifyRatioSumErr;
    int32_t  verifyTarget_x;
    int32_t  verifyTarget_y;
    int32_t  verifyTarget_Y;
    int32_t  verifyTargetErrX;
    int32_t  verifyTargetErrY;
    int32_t  verifyTargetErrYValue;
    uint16_t PWM_R;
    uint16_t PWM_G;
    uint16_t PWM_B;
    uint16_t PWM_W;
} SHostRatioKDebug;

extern volatile SHostRatioKDebug g_hostRatioKDebug;

void HostRatioKDebug_Reset(void);

#endif