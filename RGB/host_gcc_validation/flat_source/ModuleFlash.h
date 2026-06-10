#ifndef _HOST_MODULEFLASH_H_
#define _HOST_MODULEFLASH_H_

#include <stdint.h>
#include "SystemType.h"

typedef struct WhiteRgbMixParams
{
    uint16_t rgbPointX;
    uint16_t rgbPointY;
    uint16_t whiteRatio;
    uint16_t rgbRatio;
} SWhiteRgbMixParams;

typedef struct FlashContent
{
    uint16_t factor;
    uint16_t eolAddr;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint16_t whitex;
    uint16_t whitey;
    uint32_t whiteY;
    uint8_t brightness_factor;
    uint8_t Utemp;
    uint8_t Vtemp;
    SWhiteRgbMixParams whiteRgbMixParams;
} SFlashContent;

extern SFlashContent savedConfig;

#endif