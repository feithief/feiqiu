/*********************************************************************************
  *Copyright(C), KEBODA TECHNOLOGY CO.,LTD.
  *FileName:    Algorithm.c
  *Author:      Leo Song
  *Version:     0.0.1
  *Date:        2017.10.23
  *Description: This file contains algorithms that used to generate PWM
                and get fixed LIN signal.
  *Others:      the intensity adjust parameters may combined to one. Using a 32 bit
                integer instead of two parameters.
  *History:
     1.Date:         2017.10.23
       Author:       Leo song
       Modification: Initial version.
**********************************************************************************/

#include "Algorithm.h"
#include "FloatLib.h"
#include "ModuleLed.h"
#include "ModuleTemperature.h"
#include "SystemStatus.h"
#include "SystemControl.h"
#include "ModuleFlash.h"
#include "SystemLinComm.h"
#include "string.h"

#include "AlgoRatioK_localfit/AlgoRatioK_localfit.h"
extern uint16_t Debug[12];
/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
static const fix16_t fix16Const75 = F16(7.5);
static const fix16_t fix16Const10000 = 10000*0x00010000;
static const fix16_t fix16Const1M = F16(0.0001);
static const fix16_t fix16Int4 = 4 * 0x00010000;
static const fix16_t fix16Int6 = 6 * 0x00010000;
static const fix16_t fix16Int9 = 9 * 0x00010000;
static const fix16_t fix16Int12 = 12 * 0x00010000;
static const fix16_t fix16Int16 = 16 * 0x00010000;
static const fix16_t fix16Int10 = 10 * 0x00010000;
static const fix16_t fix16Int100 = 100 * 0x00010000;
static const fix16_t fix16Int1000 = 1000 * 0x00010000;
static uint8_t gAlgoUseRuntimeChannelBasis = 0u;
static int16_t gAlgoSonderfunktion2CurrentTemperatureDegC_X10 = 214;
static uint8_t gAlgoSonderfunktion2HasCurrentTemperature = 0u;
static ST_ChannelXyY gAlgoSonderfunktion2CurrentChannelXyY;
static uint8_t gAlgoSonderfunktion2HasCurrentChannelXyY = 0u;

#ifndef ALGO_SONDERFUNKTION2_TEMP_CORRECTION_ENABLE
#define ALGO_SONDERFUNKTION2_TEMP_CORRECTION_ENABLE  (1u)
#endif

#ifndef ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE
#define ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE       (1u)
#endif

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
volatile uint16_t gAlgoSonderfunktion2DebugTargetU = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugTargetV = 0u;
volatile int32_t gAlgoSonderfunktion2DebugTargetXQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugTargetYQ10000 = 0;
volatile int16_t gAlgoSonderfunktion2DebugTempRawX10 = 0;
volatile int16_t gAlgoSonderfunktion2DebugTempSafeX10 = 0;
volatile int32_t gAlgoSonderfunktion2DebugTQ10000 = 0;
volatile uint16_t gAlgoSonderfunktion2DebugW25RawQ10000 = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugW25SafeQ10000 = 0u;
volatile uint8_t gAlgoSonderfunktion2DebugSegmentIndex = 0u;
volatile int32_t gAlgoSonderfunktion2DebugF1Q10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugF2Q10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugDeltaWQ10000 = 0;
volatile uint16_t gAlgoSonderfunktion2DebugWformulaQ10000 = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugWmaxBoundaryQ10000 = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugWfinalQ10000 = 0u;
volatile uint8_t gAlgoSonderfunktion2DebugUseRuntimeChannel = 0u;
volatile int32_t gAlgoSonderfunktion2DebugCurrentRedXQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentRedYQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentGreenXQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentGreenYQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentBlueXQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentBlueYQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentWhiteXQ10000 = 0;
volatile int32_t gAlgoSonderfunktion2DebugCurrentWhiteYQ10000 = 0;
volatile uint16_t gAlgoSonderfunktion2DebugPWM_R = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugPWM_G = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugPWM_B = 0u;
volatile uint16_t gAlgoSonderfunktion2DebugPWM_W = 0u;

static void __algoSyncSonderfunktion2DebugArray(void)
{
    /*
     * Debug[0] = targetU
     * Debug[1] = targetV
     * Debug[2] = TempRawX10
     * Debug[3] = W25RawQ10000
     * Debug[4] = WformulaQ10000
     * Debug[5] = WfinalQ10000
     * Debug[6] = TempSafeX10
     * Debug[7] = SegmentIndex
     */
    Debug[0] = gAlgoSonderfunktion2DebugTargetU;
    Debug[1] = gAlgoSonderfunktion2DebugTargetV;
    Debug[2] = (uint16_t)gAlgoSonderfunktion2DebugTempRawX10;
    Debug[3] = gAlgoSonderfunktion2DebugW25RawQ10000;
    Debug[4] = gAlgoSonderfunktion2DebugWformulaQ10000;
    Debug[5] = gAlgoSonderfunktion2DebugWfinalQ10000;
    Debug[6] = (uint16_t)gAlgoSonderfunktion2DebugTempSafeX10;
    Debug[7] = (uint16_t)gAlgoSonderfunktion2DebugSegmentIndex;
}
#endif

#if (ALGO_SONDERFUNKTION2_TEMP_CORRECTION_ENABLE != 0u)
#define ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT      (3u)
#define ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT        (10u)
static const fix16_t gAlgoSonderfunktion2TempBaseC = F16(21.399999999999999);
static const fix16_t gAlgoSonderfunktion2TempScaleC = F16(78.599999999999994);
static const fix16_t gAlgoSonderfunktion2TempCoefF1[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT][ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT] =
{
    { F16(-0.062908395937713746), F16(0.46540428057720884), F16(0.013168433771137959), F16(0.24773798631542077), F16(-0.62906875075111557), F16(-0.1507447534143207), F16(-0.41473252725841903), F16(-0.41515998921298364), F16(-0.22023855342157164), F16(0.14402502618191421) },
    { F16(-0.22374203699409886), F16(0.46536043860076226), F16(1.2405996914282105), F16(-0.023722059509511648), F16(-3.3966619275817935), F16(0.33057905189497355), F16(-0.60412378816444079), F16(0.056288926808187839), F16(0.65854953007563211), F16(-0.11763963275258804) },
    { F16(3.9810563361488964), F16(-30.844899466572777), F16(20.166669784776083), F16(-5.3031206854300903), F16(-76.598508028686737), F16(24.255678990670969), F16(-13.044988091807113), F16(1.1891652786297808), F16(51.455151234680486), F16(24.8804473950639) }
};
static const fix16_t gAlgoSonderfunktion2TempCoefF2[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT][ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT] =
{
    { F16(0.060997372249708422), F16(-0.29460293306383584), F16(-0.15245132452691115), F16(-0.30532544869560357), F16(0.15045747220444899), F16(0.03766345646106279), F16(0.21796179896752851), F16(0.79785621136978657), F16(0.35269999743516989), F16(0.14799982600085532) },
    { F16(-0.16051504679538733), F16(0.45450657796232186), F16(0.30005085341958532), F16(0.17232274827027069), F16(0.40901655988621527), F16(-0.35856034142853771), F16(-0.039689348493071958), F16(-0.063098046905078142), F16(-0.60957468888636834), F16(-0.64743354537085085) },
    { F16(-2.6057463222430659), F16(14.84639274782614), F16(-7.5182256010158515), F16(3.6556625517521155), F16(29.250726598295557), F16(-13.020334378391167), F16(7.4003173679611001), F16(-1.1960887573337249), F16(-19.947837323948541), F16(-13.52865554401809) }
};
#endif


static fix16_t __algoWhiteRgbParamToFix16(uint16_t value)
{
    return fix16_div(fix16_from_int((int32_t)value), fix16Const10000);
}


/*This LED normal intensity may too high.*/
/*we use this parameter to adjust it to same range as other chip did.*/
static const fix16_t intensityAdjustConst = F16(1.0);
static const int32_t s32_plane_a[] = { 370, -387, -249 };
static const int32_t s32_plane_b[] = { 484, 487, -326 };
static const int32_t s32_plane_d[] = { -37502, 110603, 463879 };
static const int32_t s32_vectorNor[] = { 100691, -189723, 573 };
static const int32_t s32_pointK1[] = { 258, 395, 168290 };
static const int32_t s32_R_cornerPoint_large[] =  {135502234, 136970240, 165054 };
static const int32_t s32_G_cornerPoint_large[] = { 15781069, 150339584, 262144 };
static const int32_t s32_B_cornerPoint_large[] = { 51406438, 23121101, 77672 };
static const int32_t s32_R_cornerPoint_real[] = {517, 523, 630};
static const int32_t s32_G_cornerPoint_real[] = {60, 574, 1000};
static const int32_t s32_B_cornerPoint_real[] = {196, 88, 296};
static const int32_t s32_LineDirectionvec_GBm1n1p1[] = {138, -493};
static const int32_t s32_LineDirectionvec_BRm1n1p1[] = {304, 412};
static const int32_t s32_LineDirectionvec_GRm1n1p1[] = {509, -57};

static const fix16_t RC_UPART = F16(1.73439025052479);
static const fix16_t RC_VPART = F16(0.485686451774817);
static const fix16_t RC_CPART = F16(-382.951473174450);
static const fix16_t GC_UPART = F16(-1.70362297797533);
static const fix16_t GC_VPART = F16(1.25839799984915);
static const fix16_t GC_CPART = F16(223.089762394268);
static const fix16_t BC_UPART = F16(-0.0307672725494539);
static const fix16_t BC_VPART = F16(-0.275517909281090);
static const fix16_t BC_CPART = F16(159.861710780182);
static const fix16_t Color_Scale = F16(0.01);
static const fix16_t RxDivRy = F16(2.22588516746412);
static const fix16_t GxDivGy = F16(0.236181342632955);
static const fix16_t BxDivBy = F16(5.00255102040816);
static const fix16_t inv_Ry = F16(3.22555023923445);
static const fix16_t inv_Gy = F16(1.38849171752398);
static const fix16_t inv_By = F16(33.3486394557823);

fix16_t WB_L = F16(0), WB_M = F16(0), WB_N = F16(0);


#define WHITE_BALANCE_U_MAX 60
#define WHITE_BALANCE_U_MIN 20
#define WHITE_BALANCE_V_MAX 114
#define WHITE_BALANCE_V_MIN 74

#define BITSHIFT_PYRAMID (18u)
#define PLANECOUNT (3u)
#define SINT32_MAX (0x7fffffff)
#define CORNERPOINTU_IDX (0u)
#define CORNERPOINTV_IDX (1u)
#define CORNERPOINTL_IDX (2u)
#define DMAX_TEMP_SHIFT_RECORD_AMOUNT 8
typedef enum InputState {
    Cx00_INSIDE_GAMUT = 0,
    Cx02_BESIDE_EDGE_GB = 2,                 // == 0
    Cx03_BESIDE_EDGE_BR,      //1 minus
    Cx04_BESIDE_EDGE_GR,     //2 minus
    Cx05_BESIDE_CORNER_B,
    Cx06_BESIDE_CORNER_G,
    Cx07_BESIDE_CORNER_R
}InputState;

/*The input parameters of intensity only has 40% of normal value.*/
/*We need to multiply 2.5. The reason is that because too big value */
/*will cause our floating LIB over flow.*/
/*@brief CIE color multiplied by 10000.*/
typedef struct CIExyY
{
 int16_t x,y;
 int32_t Y;
}SCIExyY;

/*@brief CIE color converted to fix16 floating.*/
typedef struct CIEColor
{
 fix16_t x,y;
 fix16_t Y;
}SCIEColor;
/*Original R, G, B colors CIE coordinates.*/
static SCIExyY redxyY, greenxyY, bluexyY, whitexyY;
/*After temperature compensation's  R, G, B colors CIE coordinates.*/
static SCIEColor currentRedCIE, currentGreenCIE, currentBlueCIE, currentWhiteCIE;
volatile SCIEColor targetColor;
volatile SCIEColor splitWhiteCIE, splitPointCIE;
volatile bool_t splitTargetColorValid;
volatile fix16_t splitLambda, splitOneMinusLambda, splitRayT;

static fix16_t __algoGetWhitePointX(void)
{
    if ((gAlgoUseRuntimeChannelBasis != 0u) &&
        (gAlgoSonderfunktion2HasCurrentChannelXyY != 0u))
    {
        return __algoWhiteRgbParamToFix16((uint16_t)gAlgoSonderfunktion2CurrentChannelXyY.whitex);
    }

    return __algoWhiteRgbParamToFix16(savedConfig.whitex);
}

static fix16_t __algoGetWhitePointY(void)
{
    if ((gAlgoUseRuntimeChannelBasis != 0u) &&
        (gAlgoSonderfunktion2HasCurrentChannelXyY != 0u))
    {
        return __algoWhiteRgbParamToFix16((uint16_t)gAlgoSonderfunktion2CurrentChannelXyY.whitey);
    }

    return __algoWhiteRgbParamToFix16(savedConfig.whitey);
}
void AlgoSonderfunktion2_SetCurrentTemperatureX10(int16_t temperatureDegC_x10)
{
    gAlgoSonderfunktion2CurrentTemperatureDegC_X10 = temperatureDegC_x10;
    gAlgoSonderfunktion2HasCurrentTemperature = 1u;
}

void AlgoSonderfunktion2_SetCurrentTemperature(int16_t temperatureDegC)
{
    AlgoSonderfunktion2_SetCurrentTemperatureX10((int16_t)(temperatureDegC * 10));
}

void AlgoSonderfunktion2_SetCurrentChannelXyY(const ST_ChannelXyY *channelXyY)
{
    if (channelXyY == (void *)0)
    {
        gAlgoSonderfunktion2HasCurrentChannelXyY = 0u;
        memset((void *)&gAlgoSonderfunktion2CurrentChannelXyY, 0, sizeof(gAlgoSonderfunktion2CurrentChannelXyY));
        return;
    }

    gAlgoSonderfunktion2CurrentChannelXyY = *channelXyY;
    gAlgoSonderfunktion2HasCurrentChannelXyY = 1u;
}

static fix16_t __algoChannelXyToFix16(int16_t value)
{
    return fix16_div(fix16_from_int((int32_t)value), fix16Const10000);
}

static fix16_t __algoChannelYToFix16(int32_t value)
{
    return fix16_mul(fix16_div(fix16_from_int((int32_t)(value / 20)), fix16Const10000), fix16Const75);
}

static void __algoSaveRuntimeChannelBasis(SCIEColor *savedRed,
                                          SCIEColor *savedGreen,
                                          SCIEColor *savedBlue,
                                          SCIEColor *savedWhite)
{
    *savedRed = currentRedCIE;
    *savedGreen = currentGreenCIE;
    *savedBlue = currentBlueCIE;
    *savedWhite = currentWhiteCIE;
}

static void __algoApplyRuntimeChannelBasis(void)
{
    currentRedCIE.x = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.redx);
    currentRedCIE.y = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.redy);
    currentRedCIE.Y = __algoChannelYToFix16(gAlgoSonderfunktion2CurrentChannelXyY.redY);

    currentGreenCIE.x = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.greenx);
    currentGreenCIE.y = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.greeny);
    currentGreenCIE.Y = __algoChannelYToFix16(gAlgoSonderfunktion2CurrentChannelXyY.greenY);

    currentBlueCIE.x = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.bluex);
    currentBlueCIE.y = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.bluey);
    currentBlueCIE.Y = __algoChannelYToFix16(gAlgoSonderfunktion2CurrentChannelXyY.blueY);

    currentWhiteCIE.x = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.whitex);
    currentWhiteCIE.y = __algoChannelXyToFix16(gAlgoSonderfunktion2CurrentChannelXyY.whitey);
    currentWhiteCIE.Y = __algoChannelYToFix16(gAlgoSonderfunktion2CurrentChannelXyY.whiteY);

    gAlgoUseRuntimeChannelBasis = 1u;
}

static void __algoRestoreRuntimeChannelBasis(const SCIEColor *savedRed,
                                             const SCIEColor *savedGreen,
                                             const SCIEColor *savedBlue,
                                             const SCIEColor *savedWhite)
{
    currentRedCIE = *savedRed;
    currentGreenCIE = *savedGreen;
    currentBlueCIE = *savedBlue;
    currentWhiteCIE = *savedWhite;
    gAlgoUseRuntimeChannelBasis = 0u;
}

static int16_t __algoGetSonderfunktion2CorrectionTemperatureDegC_X10(const SColorParams *inputColor)
{
    (void)inputColor;

    /*
     * Current test path: always use MCU runtime temperature.
     * Keep AlgoSonderfunktion2_SetCurrentTemperatureX10() for later,
     * but do not let it override the live source for now.
     */
    return moduleTempMcuGet_debug();
}

static int32_t __algoFix16ToSignedQ10000(fix16_t value)
{
    return fix16_to_int(fix16_mul(value, fix16Const10000));
}

static uint16_t __algoClampPositiveFix16ToQ10000(fix16_t value)
{
    int32_t q10000;

    if (value < 0)
    {
        value = 0;
    }

    q10000 = __algoFix16ToSignedQ10000(value);
    if (q10000 < 0)
    {
        q10000 = 0;
    }
    if (q10000 > 65535)
    {
        q10000 = 65535;
    }

    return (uint16_t)q10000;
}

static bool_t __algoIsSonderfunktion2TempBypassTarget(uint16_t targetU,
                                                       uint16_t targetV)
{
    if (((targetU == 40u) && (targetV == 94u)) ||
        ((targetU == 200u) && (targetV == 470u)))
    {
        return btrue;
    }

    if (((targetU % 5u) == 0u) && ((targetV % 5u) == 0u))
    {
        if (((targetU / 5u) == 40u) && ((targetV / 5u) == 94u))
        {
            return btrue;
        }
    }

    return bfalse;
}

static bool_t __algoShouldBypassSonderfunktion2TempCorrection(uint16_t originalTargetU,
                                                              uint16_t originalTargetV,
                                                              uint16_t currentTargetU,
                                                              uint16_t currentTargetV)
{
    if (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion != 2u)
    {
        return bfalse;
    }

    if ((btrue == __algoIsSonderfunktion2TempBypassTarget(originalTargetU, originalTargetV)) ||
        (btrue == __algoIsSonderfunktion2TempBypassTarget(currentTargetU, currentTargetV)))
    {
        return btrue;
    }

    return bfalse;
}

static uint16_t __algoClampWhiteRatioFix16ToQ10000(fix16_t whiteRatio)
{
    fix16_t clampedValue = whiteRatio;
    int32_t q10000;

    if (clampedValue < 0)
    {
        clampedValue = 0;
    }
    if (clampedValue > fix16_one)
    {
        clampedValue = fix16_one;
    }

    q10000 = __algoFix16ToSignedQ10000(clampedValue);
    if (q10000 < 0)
    {
        q10000 = 0;
    }
    if (q10000 > 10000)
    {
        q10000 = 10000;
    }

    return (uint16_t)q10000;
}

static fix16_t __algoClampFix16(fix16_t value, fix16_t minValue, fix16_t maxValue)
{
    fix16_t ret = value;

    if (ret < minValue)
    {
        ret = minValue;
    }
    else if (ret > maxValue)
    {
        ret = maxValue;
    }
    else
    {
        /* Keep current value */
    }

    return ret;
}

#if (ALGO_SONDERFUNKTION2_TEMP_CORRECTION_ENABLE != 0u)
static void __algoUv1000ToXy(uint16_t targetU,
                             uint16_t targetV,
                             fix16_t *x,
                             fix16_t *y)
{
    fix16_t u;
    fix16_t v;
    fix16_t denominator;

    if ((x == (void *)0) || (y == (void *)0))
    {
        return;
    }

    u = fix16_div(fix16_from_int((int32_t)targetU), fix16Int1000);
    v = fix16_div(fix16_from_int((int32_t)targetV), fix16Int1000);
    denominator = fix16_add(fix16_sub(fix16_mul(fix16Int6, u), fix16_mul(fix16Int16, v)), fix16Int12);

    if ((denominator > -fix16Const1M) && (denominator < fix16Const1M))
    {
        *x = 0;
        *y = 0;
        return;
    }

    *x = fix16_div(fix16_mul(fix16Int9, u), denominator);
    *y = fix16_div(fix16_mul(fix16Int4, v), denominator);
}

static uint8_t __algoGetTemperatureCorrectionSegment(fix16_t whiteRatio)
{
    fix16_t whiteRatioSafe = __algoClampFix16(whiteRatio, 0, fix16_one);

    if (whiteRatioSafe < F16(0.25))
    {
        return 0u;
    }
    else if (whiteRatioSafe < F16(0.80))
    {
        return 1u;
    }
    else
    {
        return 2u;
    }
}

static fix16_t __algoCalcTemperatureDeltaWhiteRatio(uint8_t segmentIndex,
                                                    fix16_t x,
                                                    fix16_t y,
                                                    fix16_t whiteRatio,
                                                    int16_t temperatureDegC_X10)
{
    fix16_t basis[ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT];
    fix16_t limitedTemperature;
    fix16_t whiteRatioSafe;
    fix16_t t;
    fix16_t t2;
    fix16_t f1;
    fix16_t f2;
    uint8_t i;

    whiteRatioSafe = __algoClampFix16(whiteRatio, 0, fix16_one);
    limitedTemperature = fix16_div(fix16_from_int((int32_t)temperatureDegC_X10), fix16Int10);
    if (limitedTemperature < gAlgoSonderfunktion2TempBaseC)
    {
        limitedTemperature = gAlgoSonderfunktion2TempBaseC;
    }
    if (limitedTemperature > fix16Int100)
    {
        limitedTemperature = fix16Int100;
    }

    t = fix16_div(fix16_sub(limitedTemperature, gAlgoSonderfunktion2TempBaseC),
                  gAlgoSonderfunktion2TempScaleC);
    t2 = fix16_mul(t, t);

    basis[0] = fix16_one;
    basis[1] = x;
    basis[2] = y;
    basis[3] = whiteRatioSafe;
    basis[4] = fix16_mul(x, y);
    basis[5] = fix16_mul(x, whiteRatioSafe);
    basis[6] = fix16_mul(y, whiteRatioSafe);
    basis[7] = fix16_mul(whiteRatioSafe, whiteRatioSafe);
    basis[8] = fix16_mul(x, x);
    basis[9] = fix16_mul(y, y);

    f1 = 0;
    f2 = 0;
    for (i = 0u; i < ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT; i++)
    {
        f1 = fix16_add(f1, fix16_mul(gAlgoSonderfunktion2TempCoefF1[segmentIndex][i], basis[i]));
        f2 = fix16_add(f2, fix16_mul(gAlgoSonderfunktion2TempCoefF2[segmentIndex][i], basis[i]));
    }

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
    gAlgoSonderfunktion2DebugTempSafeX10 = (int16_t)fix16_to_int(fix16_mul(limitedTemperature, fix16Int10));
    gAlgoSonderfunktion2DebugTQ10000 = __algoFix16ToSignedQ10000(t);
    gAlgoSonderfunktion2DebugSegmentIndex = segmentIndex;
    gAlgoSonderfunktion2DebugF1Q10000 = __algoFix16ToSignedQ10000(f1);
    gAlgoSonderfunktion2DebugF2Q10000 = __algoFix16ToSignedQ10000(f2);
#endif

    return fix16_add(fix16_mul(t, f1), fix16_mul(t2, f2));
}
#endif

static uint16_t __algoApplyTemperatureWhiteRatioCorrection(uint16_t targetU,
                                                           uint16_t targetV,
                                                           uint16_t whiteRatioQ10000,
                                                           int16_t temperatureDegC_X10)
{
#if (ALGO_SONDERFUNKTION2_TEMP_CORRECTION_ENABLE == 0u)
    return whiteRatioQ10000;
#else
    fix16_t x;
    fix16_t y;
    fix16_t whiteRatio25;
    fix16_t whiteRatio25Safe;
    fix16_t deltaWhiteRatio;
    fix16_t whiteRatioFormula;
    uint8_t segmentIndex;


    __algoUv1000ToXy(targetU, targetV, &x, &y);
    whiteRatio25 = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
    whiteRatio25Safe = __algoClampFix16(whiteRatio25, 0, fix16_one);
    segmentIndex = __algoGetTemperatureCorrectionSegment(whiteRatio25Safe);
    deltaWhiteRatio = __algoCalcTemperatureDeltaWhiteRatio(segmentIndex,
                                                           x,
                                                           y,
                                                           whiteRatio25Safe,
                                                           temperatureDegC_X10);
    whiteRatioFormula = __algoClampFix16(fix16_add(whiteRatio25Safe, deltaWhiteRatio), 0, fix16_one);

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
    gAlgoSonderfunktion2DebugTargetU = targetU;
    gAlgoSonderfunktion2DebugTargetV = targetV;
    gAlgoSonderfunktion2DebugTargetXQ10000 = __algoFix16ToSignedQ10000(x);
    gAlgoSonderfunktion2DebugTargetYQ10000 = __algoFix16ToSignedQ10000(y);
    gAlgoSonderfunktion2DebugTempRawX10 = temperatureDegC_X10;
    gAlgoSonderfunktion2DebugW25RawQ10000 = whiteRatioQ10000;
    gAlgoSonderfunktion2DebugW25SafeQ10000 = __algoClampWhiteRatioFix16ToQ10000(whiteRatio25Safe);
    gAlgoSonderfunktion2DebugSegmentIndex = segmentIndex;
    gAlgoSonderfunktion2DebugDeltaWQ10000 = __algoFix16ToSignedQ10000(deltaWhiteRatio);
    gAlgoSonderfunktion2DebugWformulaQ10000 = __algoClampWhiteRatioFix16ToQ10000(whiteRatioFormula);
#endif

    if (deltaWhiteRatio == 0)
    {
        return __algoClampWhiteRatioFix16ToQ10000(whiteRatio25Safe);
    }

    return __algoClampWhiteRatioFix16ToQ10000(whiteRatioFormula);
#endif
}
volatile int16_t splitEdgeIndex;
static int32_t CtApSwcLedAlgorithm_AUDIspecBreachOnEdge(const uint8_t u8_inputState, int32_t* s32_intended_u, int32_t* s32_intended_v)
{
	int32_t s32_s, s32_u, s32_v;
    const int32_t* p_dirVecArray, * p_underPointArray_large, * p_upperPointArray_large, * p_linePoint_real;
    int32_t s32_intensity;
    int8_t u8_plane_idx;
    s32_u = *s32_intended_u;
    s32_v = *s32_intended_v;
    /*LineDirectionvec(1)*(normPoint(1) - LinePoint(1)) + LineDirectionvec(2)*(normPoint(2) - LinePoint(2));*/
    if (u8_inputState == Cx02_BESIDE_EDGE_GB)
    {
        p_upperPointArray_large = &s32_B_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_G_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_GBm1n1p1[0];
        p_linePoint_real = s32_B_cornerPoint_real;
        u8_plane_idx = 0u;
    }
    else if (u8_inputState == Cx03_BESIDE_EDGE_BR)
    {
        p_upperPointArray_large = &s32_R_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_B_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_BRm1n1p1[0];
        p_linePoint_real = s32_R_cornerPoint_real;
        u8_plane_idx = 1u;
    }
    else if (u8_inputState == Cx04_BESIDE_EDGE_GR)
    {
        p_upperPointArray_large = &s32_R_cornerPoint_large[CORNERPOINTU_IDX];
        p_underPointArray_large = &s32_G_cornerPoint_large[CORNERPOINTU_IDX];
        p_dirVecArray = &s32_LineDirectionvec_GRm1n1p1[0];
        p_linePoint_real = s32_R_cornerPoint_real;
        u8_plane_idx = 2u;
    }
    s32_s = p_dirVecArray[0] * (s32_u - p_linePoint_real[CORNERPOINTU_IDX]) + p_dirVecArray[1] * (s32_v - p_linePoint_real[CORNERPOINTV_IDX]);
    /*x = LinePoint(1) + s * LineDirectionvec(1);*/
    s32_u = p_upperPointArray_large[0] + s32_s * p_dirVecArray[0];
    if (s32_u >= p_upperPointArray_large[0])
    {
        /*upper corner*/
        *s32_intended_u = p_upperPointArray_large[0] >> BITSHIFT_PYRAMID;
        *s32_intended_v = p_upperPointArray_large[1] >> BITSHIFT_PYRAMID;
        s32_intensity = p_upperPointArray_large[2];
    }
    else if (s32_u <= p_underPointArray_large[0])
    {
        *s32_intended_u = p_underPointArray_large[0] >> BITSHIFT_PYRAMID;
        *s32_intended_v = p_underPointArray_large[1] >> BITSHIFT_PYRAMID;
        s32_intensity = p_underPointArray_large[2];
    }
    else
    {
        *s32_intended_u = s32_u >> BITSHIFT_PYRAMID;
        s32_v = p_upperPointArray_large[1] + s32_s * p_dirVecArray[1];
        *s32_intended_v = s32_v >> BITSHIFT_PYRAMID;
        s32_intensity = s32_plane_a[u8_plane_idx] * *s32_intended_u + s32_plane_b[u8_plane_idx] * *s32_intended_v + s32_plane_d[u8_plane_idx];
    }
    return s32_intensity;
}
static void CtApSwcLedAlgorithm_intensityPyramid(SColorParams* pst_LEDColor)
{
    uint8_t inputState = Cx00_INSIDE_GAMUT;
    int32_t s32_u = (int32_t)pst_LEDColor->colorU, s32_v = (int32_t)pst_LEDColor->colorV;
    int32_t s32_selectedIntensity = SINT32_MAX, s32_min_intensity_rotated = SINT32_MAX;
    int32_t s32_intensity, s32_vectorRef2Intensity[PLANECOUNT], s32_intensity_Rotated;
    for (uint8_t u8_plane_idx = 0; u8_plane_idx < PLANECOUNT; u8_plane_idx++)
    {
        s32_intensity = s32_plane_a[u8_plane_idx] * pst_LEDColor->colorU + s32_plane_b[u8_plane_idx] * pst_LEDColor->colorV + s32_plane_d[u8_plane_idx];
        s32_vectorRef2Intensity[0] = (int32_t)pst_LEDColor->colorU - s32_pointK1[0];
        s32_vectorRef2Intensity[1] = (int32_t)pst_LEDColor->colorV - s32_pointK1[1];
        s32_vectorRef2Intensity[2] = s32_intensity - s32_pointK1[2];
        s32_intensity_Rotated = s32_vectorRef2Intensity[0] * s32_vectorNor[0] + s32_vectorRef2Intensity[1] * s32_vectorNor[1] + s32_vectorRef2Intensity[2] * s32_vectorNor[2];
        /*if s32_intensity_Rotated == 0, Cx02_ONEDGE_GAMUT
        if 1 s32_intensity_Rotated < 0, Cx03_NOT_INGAMUT_BESIDE_EDGE
        if 2 s32_intensity_Rotated < 0, Cx04_NOT_INGAMUT_BESIDE_CORNER
        if 3 s32_intensity_Rotated < 0, mistake*/
        if (s32_intensity_Rotated < 0)
        {
            inputState += u8_plane_idx + 2;
        }
        if (s32_intensity_Rotated < s32_min_intensity_rotated)
        {
            s32_min_intensity_rotated = s32_intensity_Rotated;
            s32_selectedIntensity = s32_intensity;
        }
    }
    if (inputState == Cx00_INSIDE_GAMUT)
    {
       // s32_selectedIntensity = s32_selectedIntensity;
    }
    else if (inputState <= Cx04_BESIDE_EDGE_GR && inputState >= Cx02_BESIDE_EDGE_GB)
    {
        /*Beside edge*/
        s32_selectedIntensity = CtApSwcLedAlgorithm_AUDIspecBreachOnEdge(inputState, &s32_u, &s32_v);
    }
    else
    {
        /*Beside corner*/
        if (inputState == Cx05_BESIDE_CORNER_B)
        {
            s32_u = s32_B_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_B_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_B_cornerPoint_large[CORNERPOINTL_IDX];
        }
        else if (inputState == Cx06_BESIDE_CORNER_G)
        {
            s32_u = s32_G_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_G_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_G_cornerPoint_large[CORNERPOINTL_IDX];
        }
        else
        {
            s32_u = s32_R_cornerPoint_real[CORNERPOINTU_IDX];
            s32_v = s32_R_cornerPoint_real[CORNERPOINTV_IDX];
            s32_selectedIntensity = s32_R_cornerPoint_large[CORNERPOINTL_IDX];
        }
    }
    pst_LEDColor->intensity = (int32_t)(((int32_t)pst_LEDColor->intensity * (int32_t)s32_selectedIntensity) >> BITSHIFT_PYRAMID);
    pst_LEDColor->colorU = (int16_t)s32_u;
    pst_LEDColor->colorV = (int16_t)s32_v;
    return;
}
/**
*@details   Update red led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  red x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  red y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  red Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateRedCIE(int16_t x, int16_t y, int32_t Y)
{
  redxyY.x = x;
  redxyY.y = y;
  redxyY.Y = (Y/20);
}
/**
*@details   Update green led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  green x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  green y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  green Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateGreenCIE(int16_t x, int16_t y, int32_t Y)
{
  greenxyY.x = x;
  greenxyY.y = y;
  greenxyY.Y = (Y/20);
}
/**
*@details   Update blue led parameter.
*
*@note      The intensity(Y) value only have 40% of normal intensity for our float LIB has limitation.
*
*@param[in] x  blue x coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] y  blue y coordinate in CIE color. This value has been multiplied by 10000.
*
*@param[in] Y  blue Luminous in CIE color. This value has been multiplied by 10000.
*
*@retval    None.
*/
void algoUpdateBlueCIE(int16_t x, int16_t y, int32_t Y)
{
  bluexyY.x = x;
  bluexyY.y = y;
  bluexyY.Y = (Y/20);
}

void algoUpdateWhiteCIE(int32_t Y)
{
  whitexyY.Y = (Y/20);
}


void WitheBalanceTempInit()
{
		static CIE1976WhiteBalanceInfo Corolinit_temp;

	  uint16_t middleResult = 0u;
	  static uint16_t currentU, currentV;

//	  if(savedConfig.Utemp > WHITE_BALANCE_U_MAX )
//		  WhiteBalanceCalaU = WHITE_BALANCE_U_MAX;
//	  if(savedConfig.Utemp < WHITE_BALANCE_U_MIN )
//		  WhiteBalanceCalaU = WHITE_BALANCE_U_MIN;
//	  if(savedConfig.Vtemp > WHITE_BALANCE_V_MAX )
//		  WhiteBalanceCalaV = WHITE_BALANCE_V_MAX;
//	  if(savedConfig.Vtemp < WHITE_BALANCE_V_MIN )
//		  WhiteBalanceCalaV = WHITE_BALANCE_V_MIN;

	  Corolinit_temp.calibration_whitePointU = (int32_t)200;
	  Corolinit_temp.calibration_whitePointV = (int32_t)470;

	  middleResult = savedConfig.Utemp * 25;
	  if(middleResult % 10u >= 5u)
	  {
		  currentU = middleResult / 10 + 1;
	  }
	  else
	  {
		  currentU = middleResult / 10;
	  }


	  middleResult = savedConfig.Vtemp * 25;
	  if(middleResult % 10u >= 5u)
	  {
		  currentV = middleResult / 10 + 1;
	  }
	  else
	  {
		  currentV = middleResult / 10;
	  }

	 if((Corolinit_temp.target_whitePointU != currentU) ||
			 (Corolinit_temp.target_whitePointV != currentV))
	{
	    	  Corolinit_temp.target_whitePointU = currentU;
	    	  Corolinit_temp.target_whitePointV = currentV;
	    	  whiteBalanceInit(&Corolinit_temp);

	 }
//	  Corolinit_temp.target_whitePointU = (int32_t)WhiteBalanceCalaU*WHITE_BALANCE_UV_RATION;
//	  Corolinit_temp.target_whitePointV = (int32_t)WhiteBalanceCalaV*WHITE_BALANCE_UV_RATION;
//	  whiteBalanceInit(&Corolinit_temp);
}
void whiteBalanceInit(const CIE1976WhiteBalanceInfo* WB_info)
{
       fix16_t Div_midResult;
       WB_L = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), RC_UPART);
       WB_L = fix16_add(WB_L, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), RC_VPART));
       WB_L = fix16_add(WB_L, RC_CPART);
       WB_L = fix16_mul(WB_L, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), RC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), RC_VPART));
       Div_midResult = fix16_add(Div_midResult, RC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_L = fix16_div(Div_midResult, WB_L);//original / target

       WB_M = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), GC_UPART);
       WB_M = fix16_add(WB_M, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), GC_VPART));
       WB_M = fix16_add(WB_M, GC_CPART);
       WB_M = fix16_mul(WB_M, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), GC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), GC_VPART));
       Div_midResult = fix16_add(Div_midResult, GC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_M = fix16_div(Div_midResult, WB_M);//original / target

       WB_N = fix16_mul(fix16_from_int(WB_info->calibration_whitePointU), BC_UPART);
       WB_N = fix16_add(WB_N, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), BC_VPART));
       WB_N = fix16_add(WB_N, BC_CPART);
       WB_N = fix16_mul(WB_N, fix16_mul(fix16_from_int(WB_info->target_whitePointV), Color_Scale));
       Div_midResult = fix16_mul(fix16_from_int(WB_info->target_whitePointU), BC_UPART);
       Div_midResult = fix16_add(Div_midResult, fix16_mul(fix16_from_int(WB_info->target_whitePointV), BC_VPART));
       Div_midResult = fix16_add(Div_midResult, BC_CPART);
       Div_midResult = fix16_mul(Div_midResult, fix16_mul(fix16_from_int(WB_info->calibration_whitePointV), Color_Scale));
       WB_N = fix16_div(Div_midResult, WB_N);//original / target
}

void whiteBalance(const SColorParams *inputColor, SCIEColor *balancedColor)
{
       fix16_t f16_input_U, f16_input_V;
       //fix16_t Den = inputColor->colorV * DEN_CPART;
       fix16_t FluxR, FluxG, FluxB;
       fix16_t balanced_x, balanced_y, balanced_den;
       //FluxR = L * (RC_UPART * u_input + RC_VPART * v_input + RC_CPART) / Den;
       //FluxG = M * (GC_UPART * u_input + GC_VPART * v_input + GC_CPART) / Den;
       //FluxB = N * (BC_UPART * u_input + BC_VPART * v_input + BC_CPART) / Den;
       f16_input_U = fix16_from_int((int32_t)inputColor->colorU);
       f16_input_V = fix16_from_int((int32_t)inputColor->colorV);
       FluxR = fix16_mul(f16_input_U, RC_UPART);
       FluxR = fix16_add(FluxR, fix16_mul(RC_VPART, f16_input_V));
       FluxR = fix16_add(FluxR, RC_CPART);                                           //real fluxR mul Den * 1000
       FluxR = fix16_mul(WB_L, FluxR);

       FluxG = fix16_mul(f16_input_U, GC_UPART);
       FluxG = fix16_add(FluxG, fix16_mul(GC_VPART, f16_input_V));
       FluxG = fix16_add(FluxG, GC_CPART);                                          //real fluxG mul Den * 1000
       FluxG = fix16_mul(WB_M, FluxG);

       FluxB = fix16_mul(f16_input_U, BC_UPART);
       FluxB = fix16_add(FluxB, fix16_mul(BC_VPART, f16_input_V));
       FluxB = fix16_add(FluxB, BC_CPART);                                           //real fluxR mul Den * 1000
       FluxB = fix16_mul(WB_N, FluxB);

       balanced_x = fix16_mul(FluxR, RxDivRy);
       balanced_x = fix16_add(balanced_x, fix16_mul(FluxG, GxDivGy));
       balanced_x = fix16_add(balanced_x, fix16_mul(FluxB, BxDivBy));

       balanced_y = fix16_add(FluxR, FluxG);
       balanced_y = fix16_add(balanced_y, FluxB);

       balanced_den = fix16_mul(FluxR, inv_Ry);
       balanced_den = fix16_add(balanced_den, fix16_mul(FluxG, inv_Gy));
       balanced_den = fix16_add(balanced_den, fix16_mul(FluxB, inv_By));

       balanced_x = fix16_div(balanced_x, balanced_den);
       balanced_y = fix16_div(balanced_y, balanced_den);

       balancedColor->x = balanced_x;
       balancedColor->y = balanced_y;
// X_out_sym = (FluxR*RxDivRy + FluxG*GxDivGy + FluxB*BxDivBy) / (FluxR/Ry + FluxG/Gy + FluxB/By);
// Y_out_sym = (FluxR+FluxG+FluxB)/(FluxR/Ry + FluxG/Gy + FluxB/By);
}
/**@brief structure for temperature compensation.
 * @note more details about this structure, please refer to development document.*/
typedef struct LedTempShift{
 int16_t  Temperature;  /**<Tempetature.*/

 int16_t  whiteYShift;
 int16_t  redYShift;    /**<Red Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  greenYShift;  /**<Green Luminous shift of current temperature in percentage(multiplied by 1000).*/
 int16_t  blueYshift;   /**<Blue Luminous shift of current temperature in percentage(multiplied by 1000).*/

 int16_t  redxShift;    /**<Red x coordinate shift at current temperature.*/
 int16_t  redyShift;    /**<Red y coordinate shift at current temperature.*/

 int16_t  greenxShift;  /**<green x coordinate shift at current temperature.*/
 int16_t  greenyShift;  /**<green y coordinate shift at current temperature.*/

 int16_t  bluexShift;   /**<blue x coordinate shift at current temperature.*/
 int16_t  blueyShift;   /**<blue y coordinate shift at current temperature.*/
}SLedTempShift;



static const SLedTempShift TtoCIE[DMAX_TEMP_SHIFT_RECORD_AMOUNT]={
/*0*/{-40 ,1068 ,1383 ,1068 ,914  ,-60 , 63 ,-203 , 120  , 30 ,-40 },
/*1*/{-20 ,1045 ,1263 ,1045 ,934  ,-41 , 45 ,-139  , 82  , 21 ,-29 },
/*2*/{-10 ,1033 ,1202 ,1033 ,944  ,-32 , 35 ,-107  , 62  , 16 ,-23 },
/*3*/{0   ,1014 ,1130 ,1014 ,933  ,-25 , 30 ,-72  , 38   , 10 ,-20 },
/*4*/{25  ,1000 ,1000 ,1000 ,1000 , 0  , 0  , 0   , 0   , 0  , 0  },
/*5*/{50  ,970  ,900  ,970  ,1016 , 26 ,-24 , 85  ,-51  ,-13 , 10 },
/*6*/{85  ,922  ,680  ,922  ,1035 , 56 ,-53 , 196 ,-121  ,-31 , 28 },
/*7*/{120 ,884  ,417  ,884  ,1074 , 87 ,-85 , 308 ,-189 ,-47 , 49 }
};

/**
*@details   Referring to current led attribute calculate PWM duty cycle.
*
*@note more details about this function, please refer to development document.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[in]  MxyzY      Target color in CIE coordinate.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    None.
*/

static void __algoLedFixCIE(int16_t ledTemp)
{
	int16_t fixLevel = -1, i;
	  int16_t line_p, redp, greenp, bluep, whitep;
	  /*Temporary use*/
	  //SCIExyY redTempxyY, greenTempxyY, blueTempxyY;
	  int16_t tempShift;

	  for( i = 0 ; i < (DMAX_TEMP_SHIFT_RECORD_AMOUNT-1) ; i++)
	  {
	    if ((ledTemp >= TtoCIE[i].Temperature) &&
	        (ledTemp < TtoCIE[i+1].Temperature))
	    {
	      fixLevel = i;
	      break;
	    }
	  }

	  if (fixLevel == -1)
	    i = 0;

	  tempShift = (TtoCIE[i+1].Temperature-TtoCIE[i].Temperature);

      //fix white color
      line_p = ((TtoCIE[i+1].whiteYShift - TtoCIE[i].whiteYShift)<<4)/tempShift;
	  whitep = TtoCIE[i].whiteYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentWhiteCIE.Y = fix16_div(fix16_from_int(whitep*whitexyY.Y/1000), fix16Const10000);
	  currentWhiteCIE.Y = fix16_mul(currentWhiteCIE.Y, fix16Const75);



	//fix red color
	  line_p = ((TtoCIE[i+1].redYShift - TtoCIE[i].redYShift)<<4)/tempShift;
	  redp = TtoCIE[i].redYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentRedCIE.Y = fix16_div(fix16_from_int(redp*redxyY.Y/1000), fix16Const10000);
	  currentRedCIE.Y = fix16_mul(currentRedCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].redxShift - TtoCIE[i].redxShift)<<4)/tempShift;
	  redp = TtoCIE[i].redxShift + ((line_p * (ledTemp-TtoCIE[i].Temperature))>>4);
	  currentRedCIE.x = fix16_div(fix16_from_int(redxyY.x+redp), fix16Const10000);

	  line_p = ((TtoCIE[i+1].redyShift - TtoCIE[i].redyShift)<<4)/tempShift;
	  redp = TtoCIE[i].redyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentRedCIE.y = fix16_div(fix16_from_int(redxyY.y+redp), fix16Const10000);
	//fix red color

	//fix green color
	  line_p = ((TtoCIE[i+1].greenYShift - TtoCIE[i].greenYShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenYShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentGreenCIE.Y = fix16_div(fix16_from_int(greenp*greenxyY.Y/1000), fix16Const10000);
	  currentGreenCIE.Y = fix16_mul(currentGreenCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].greenxShift - TtoCIE[i].greenxShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenxShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4);
	  currentGreenCIE.x = fix16_div(fix16_from_int(greenxyY.x+greenp), fix16Const10000);

	  line_p = ((TtoCIE[i+1].greenyShift - TtoCIE[i].greenyShift)<<4)/tempShift;
	  greenp = TtoCIE[i].greenyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentGreenCIE.y = fix16_div(fix16_from_int(greenxyY.y+greenp), fix16Const10000);
	//fix green color

	//fix blue color
	  line_p = ((TtoCIE[i+1].blueYshift - TtoCIE[i].blueYshift)<<4)/tempShift;
	  bluep = TtoCIE[i].blueYshift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.Y = fix16_div(fix16_from_int(bluep*bluexyY.Y/1000), fix16Const10000);
	  currentBlueCIE.Y = fix16_mul(currentBlueCIE.Y, fix16Const75);

	  line_p = ((TtoCIE[i+1].bluexShift - TtoCIE[i].bluexShift)<<4)/tempShift;
	  bluep = TtoCIE[i].bluexShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.x = fix16_div(fix16_from_int(bluexyY.x+bluep), fix16Const10000);

	  line_p = ((TtoCIE[i+1].blueyShift - TtoCIE[i].blueyShift)<<4)/tempShift;
	  bluep = TtoCIE[i].blueyShift + ((line_p * ( ledTemp - TtoCIE[i].Temperature))>>4) ;
	  currentBlueCIE.y = fix16_div(fix16_from_int(bluexyY.y+bluep), fix16Const10000);


	//fix blue color
}

static bool_t __algoIsNearZero(fix16_t value)
{
    if ((value <= fix16Const1M) && (value >= -fix16Const1M))
    {
        return btrue;
    }
    return bfalse;
}

static fix16_t __algoCross2D(fix16_t ax, fix16_t ay, fix16_t bx, fix16_t by)
{
    return fix16_sub(fix16_mul(ax, by), fix16_mul(ay, bx));
}

static bool_t __algoSplitTargetColorByWhite(const SCIEColor *targetColor,
                                            volatile SCIEColor *whiteColor,
                                            volatile SCIEColor *pointColor)
{
    SCIEColor vertices[3];
    fix16_t dirX, dirY;
    fix16_t bestT, bestPX, bestPY;
    bool_t foundPoint;
    int16_t bestEdge;
    int16_t i;

    splitLambda = 0;
    splitOneMinusLambda = fix16_one;
    splitRayT = 0;
    splitEdgeIndex = -1;

    whiteColor->x = __algoGetWhitePointX();
    whiteColor->y = __algoGetWhitePointY();
    whiteColor->Y = 0;

    pointColor->x = targetColor->x;
    pointColor->y = targetColor->y;
    pointColor->Y = 0;

    if (targetColor->Y <= 0)
    {
        return btrue;
    }

    dirX = fix16_sub(targetColor->x, __algoGetWhitePointX());
    dirY = fix16_sub(targetColor->y, __algoGetWhitePointY());

    if ((__algoIsNearZero(dirX) == btrue) && (__algoIsNearZero(dirY) == btrue))
    {
        whiteColor->Y = targetColor->Y;
        splitLambda = 0;
        splitOneMinusLambda = fix16_one;
        splitRayT = fix16_one;
        splitEdgeIndex = -1;
        return btrue;
    }

    if (targetColor->y <= fix16Const1M)
    {
        return bfalse;
    }

    vertices[0].x = currentRedCIE.x;
    vertices[0].y = currentRedCIE.y;
    vertices[0].Y = 0;
    vertices[1].x = currentGreenCIE.x;
    vertices[1].y = currentGreenCIE.y;
    vertices[1].Y = 0;
    vertices[2].x = currentBlueCIE.x;
    vertices[2].y = currentBlueCIE.y;
    vertices[2].Y = 0;

    foundPoint = bfalse;
    bestT = 0;
    bestPX = 0;
    bestPY = 0;
    bestEdge = -1;

    for (i = 0; i < 3; i++)
    {
        SCIEColor startPoint, endPoint;
        fix16_t edgeX, edgeY;
        fix16_t relX, relY;
        fix16_t denominator;
        fix16_t rayT;
        fix16_t edgeU;

        startPoint = vertices[i];
        endPoint = vertices[(i + 1) % 3];

        edgeX = fix16_sub(endPoint.x, startPoint.x);
        edgeY = fix16_sub(endPoint.y, startPoint.y);
        relX = fix16_sub(startPoint.x, __algoGetWhitePointX());
        relY = fix16_sub(startPoint.y, __algoGetWhitePointY());

        denominator = __algoCross2D(dirX, dirY, edgeX, edgeY);
        if (__algoIsNearZero(denominator) == btrue)
        {
            continue;
        }

        rayT = fix16_div(__algoCross2D(relX, relY, edgeX, edgeY), denominator);
        edgeU = fix16_div(__algoCross2D(relX, relY, dirX, dirY), denominator);

        if ((rayT >= fix16_sub(fix16_one, fix16Const1M)) &&
            (edgeU >= -fix16Const1M) &&
            (edgeU <= fix16_add(fix16_one, fix16Const1M)))
        {
            if ((foundPoint == bfalse) || (rayT < bestT))
            {
                bestT = rayT;
                bestPX = fix16_add(__algoGetWhitePointX(), fix16_mul(rayT, dirX));
                bestPY = fix16_add(__algoGetWhitePointY(), fix16_mul(rayT, dirY));
                bestEdge = i;
                foundPoint = btrue;
            }
        }
    }

    if (foundPoint == bfalse)
    {
        return bfalse;
    }

    pointColor->x = bestPX;
    pointColor->y = bestPY;
    splitRayT = bestT;
    splitEdgeIndex = bestEdge;

    if (pointColor->y <= fix16Const1M)
    {
        return bfalse;
    }

    {
        fix16_t lambda;
        fix16_t oneMinusLambda;
        fix16_t whiteRatio;
        fix16_t pointRatio;

        lambda = fix16_div(fix16_one, bestT);
        oneMinusLambda = fix16_sub(fix16_one, lambda);

        splitLambda = lambda;
        splitOneMinusLambda = oneMinusLambda;

        whiteRatio = fix16_mul(oneMinusLambda, __algoGetWhitePointY());
        pointRatio = fix16_mul(lambda, pointColor->y);

        whiteColor->Y = fix16_mul(targetColor->Y, fix16_div(whiteRatio, targetColor->y));
        pointColor->Y = fix16_mul(targetColor->Y, fix16_div(pointRatio, targetColor->y));

        if (whiteColor->Y < 0)
        {
            whiteColor->Y = 0;
        }
        if (pointColor->Y < 0)
        {
            pointColor->Y = 0;
        }
    }

    return btrue;
}
static fix16_t __algoGetWhiteMixY(SColorParams * const inputColor,
                                  const SCIEColor *whiteColor)
{
    fix16_t mix_Y;

    /*
     * Same Y processing as RGB:
     * mix_Y = whiteColor->Y * tempAdjust / 6400
     */
    mix_Y = fix16_mul(whiteColor->Y,
                      fix16_from_int((int32_t)inputColor->tempAdjust / 2));

    mix_Y = fix16_mul(mix_Y, F16(0.02));
    mix_Y = fix16_mul(mix_Y, F16(0.015625));

    return mix_Y;
}
static fix16_t __algoConvertWhiteYToDutyRatio(fix16_t mixWhiteY)
{
    fix16_t duty_w_aa;

    if (mixWhiteY <= 0)
    {
        return 0;
    }

    if (currentWhiteCIE.Y <= 0)
    {
        return 0;
    }

    /*
     * White actual Y = currentWhiteCIE.Y * duty_w_aa
     * So:
     * duty_w_aa = target white Y / current white full-duty Y
     */
    duty_w_aa = fix16_div(mixWhiteY, currentWhiteCIE.Y);

    return duty_w_aa;
}
static uint16_t __algoGetCurrentBoundaryWhiteRatioQ10000(const SCIEColor *targetColorPtr)
{
    SCIEColor boundaryWhiteCIE;
    SCIEColor boundaryPointCIE;
    fix16_t boundaryRatio;
    bool_t boundaryValid;

    if ((targetColorPtr == (void *)0) || (targetColorPtr->Y <= fix16Const1M))
    {
        return 10000u;
    }

    boundaryValid = __algoSplitTargetColorByWhite(targetColorPtr, &boundaryWhiteCIE, &boundaryPointCIE);
    if (boundaryValid == bfalse)
    {
        return 10000u;
    }

    boundaryRatio = __algoClampFix16(fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y), 0, fix16_one);
    return __algoClampWhiteRatioFix16ToQ10000(boundaryRatio);
}

static void __algoGetWhiteDutyRatio(SColorParams * const inputColor,
                                    const SCIEColor *whiteColor,
                                    SPWMParams *outputPWM)
{
    fix16_t mixWhiteY;
    fix16_t duty_w_aa;
    int32_t duty_W;

    outputPWM->PWM_W = 0;

    mixWhiteY = __algoGetWhiteMixY(inputColor, whiteColor);

    duty_w_aa = __algoConvertWhiteYToDutyRatio(mixWhiteY);

    /* Clamp duty ratio to 0 ~ 1. */
    duty_w_aa = fix16_min(duty_w_aa, fix16_one);
    duty_w_aa = fix16_max(duty_w_aa, 0);

    /* Convert ratio to PWM count. */
    duty_w_aa = fix16_mul(duty_w_aa, F16(PWM_PERIOD));
    duty_W = fix16_to_int(duty_w_aa);

    /* Same factor scaling as RGB. */
    duty_W = ((duty_W * (inputColor->factor >> 7)) >> 8);

    /* Same brightness_factor scaling as RGB. */
    duty_W = (int32_t)duty_W *
             (int32_t)(savedConfig.brightness_factor & 0x7f) /
             (int32_t)100;

    if (duty_W > 32767)
    {
        duty_W = 32767;
    }
    else if (duty_W < 0)
    {
        duty_W = 0;
    }
    else
    {
        ;
    }

    outputPWM->PWM_W = (uint16_t)duty_W;
}
static void __algoGetDutyRatio(SColorParams * const inputColor, SCIEColor *MxyzY, SPWMParams *outputPWM)
{
	  fix16_t mix_Y;

	  fix16_t a,b,c,d,e,f;
	  fix16_t Rm;
	  fix16_t Gm;
	  fix16_t Bm;

	  fix16_t R_all;
	  fix16_t G_all;
	  fix16_t B_all;

	  //float tempBRESS;

	  int32_t duty_R ,duty_G ,duty_B;

	  a = fix16_sub(MxyzY->x, currentBlueCIE.x);
	  d = fix16_sub(MxyzY->y, currentBlueCIE.y);
	  Bm = fix16_div(currentBlueCIE.Y, currentBlueCIE.y);

	  b = fix16_sub(currentGreenCIE.y, MxyzY->y);
	  c = fix16_sub(currentGreenCIE.x, MxyzY->x);
	  Gm = fix16_div(currentGreenCIE.Y, currentGreenCIE.y);

	  e = fix16_sub(currentRedCIE.x, MxyzY->x);
	  f = fix16_sub(currentRedCIE.y, MxyzY->y);
	  Rm = fix16_div(currentRedCIE.Y, currentRedCIE.y);



	  R_all = fix16_mul(a, b);
	  B_all = fix16_mul(e, b);
	  G_all = fix16_mul(d, e);
	  R_all = fix16_sub(R_all, fix16_mul(c, d));
	  B_all = fix16_sub(B_all, fix16_mul(c, f));
	  G_all = fix16_sub(G_all, fix16_mul(f, a));

	  fix16_t duty_r_aa ,duty_g_aa ,duty_b_aa;

	  duty_r_aa = Bm;
	  duty_g_aa = Rm;
	  duty_b_aa = Rm;

	  duty_r_aa = fix16_mul(duty_r_aa, Gm);
	  duty_g_aa = fix16_mul(duty_g_aa, Bm);
	  duty_b_aa = fix16_mul(duty_b_aa, Gm);
	  duty_r_aa = fix16_mul(duty_r_aa, R_all);
	  duty_g_aa = fix16_mul(duty_g_aa, G_all);
	  duty_b_aa = fix16_mul(duty_b_aa, B_all);

	  /*temperature protection use this(tempAdjust) parameter to control LED intensity.*/
	  /*Minimum is 0, maximum is 6400, so we divide 6400.*/
	  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int((int32_t)inputColor->tempAdjust/2));
	  /*0.01 = 1/100, mix_Y =  mix_Y /100, for more calculating speed, we convert divide to multiply*/
	  mix_Y = fix16_mul(mix_Y, F16(0.02));
	  /*0.015625 = 1/64, mix_Y =  mix_Y /64, for more calculating speed, we convert divide to multiply*/
	  mix_Y = fix16_mul(mix_Y, F16(0.015625));
//	  mix_Y = fix16_mul(MxyzY->Y, fix16_from_int((int32_t)savedConfig.brightness_factor));
//	  mix_Y = fix16_mul(mix_Y, F16(0.01));

	  fix16_t temp = fix16_add(fix16_mul(currentRedCIE.Y, duty_r_aa),
	                           fix16_mul(currentGreenCIE.Y, duty_g_aa));

	  temp = fix16_add(temp, fix16_mul(currentBlueCIE.Y, duty_b_aa));
	  temp = fix16_div(mix_Y, temp);
#if 0
	  duty_r_aa = fix16_mul(duty_r_aa, temp);
	  duty_r_aa = fix16_mul(duty_r_aa, F16(32767));
	  //duty_r_aa = fix16_mul(duty_r_aa, fix16ConstPWM);
	  duty_g_aa = fix16_mul(duty_g_aa, temp);
	  duty_g_aa = fix16_mul(duty_g_aa, F16(32767));
	  //duty_g_aa = fix16_mul(duty_g_aa, fix16ConstPWM);
	  duty_b_aa = fix16_mul(duty_b_aa, temp);
	  duty_b_aa = fix16_mul(duty_b_aa, F16(32767));
	  //duty_b_aa = fix16_mul(duty_b_aa, fix16ConstPWM);
#endif

	  duty_r_aa = fix16_mul(duty_r_aa, temp);
	   duty_g_aa = fix16_mul(duty_g_aa, temp);
	   duty_b_aa = fix16_mul(duty_b_aa, temp);

	     /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	   duty_r_aa = fix16_min(duty_r_aa, fix16_one);
	   duty_g_aa = fix16_min(duty_g_aa, fix16_one);
	   duty_b_aa = fix16_min(duty_b_aa, fix16_one);
	   duty_r_aa = fix16_max(duty_r_aa, 0);
	   duty_g_aa = fix16_max(duty_g_aa, 0);
	   duty_b_aa = fix16_max(duty_b_aa, 0);

	   duty_r_aa = fix16_mul(duty_r_aa, F16(PWM_PERIOD));
	   duty_g_aa = fix16_mul(duty_g_aa, F16(PWM_PERIOD));
	   duty_b_aa = fix16_mul(duty_b_aa, F16(PWM_PERIOD));


	  duty_R = fix16_to_int(duty_r_aa);
	  duty_G = fix16_to_int(duty_g_aa);
	  duty_B = fix16_to_int(duty_b_aa);


	  /*factor is a parameter our customer using to adjust intensity.*/
	  duty_R = ((duty_R*(inputColor->factor>>7))>>8);
	  duty_G = ((duty_G*(inputColor->factor>>7))>>8);
	  duty_B = ((duty_B*(inputColor->factor>>7))>>8);


	  duty_R = (int32_t)duty_R *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  duty_G = (int32_t)duty_G *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  duty_B = (int32_t)duty_B *(int32_t)(savedConfig.brightness_factor & 0x7f)/(int32_t)100;
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_R > 32767)
		  duty_R = 32767;
	  else if (duty_R < 0)
	    duty_R = 0;
	  else
	  {;}
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_G > 32767)
		  duty_G = 32767;
	  else if (duty_G < 0)
	    duty_G = 0;
	  else
	  {;}
	  /*Algorithm may failed to generate PWM. We fix the abnormal value here*/
	  if (duty_B > 32767)
		  duty_B = 32767;
	  else if (duty_B < 0)
	    duty_B = 0;
	  else
	  {;}

	//  duty_R = duty_R<<1;
	//  duty_G = duty_G<<1;
	//  duty_B = duty_B<<1;

	  outputPWM->PWM_R = (uint16_t)duty_R;
	  outputPWM->PWM_G = (uint16_t)duty_G;
	  outputPWM->PWM_B = (uint16_t)duty_B;
}

int32_t tempv,tempu,templ,tempDenominator ,tempz,tempred,tempgreen,tempbule,tempmax;
fix16_t tempRED,tempGREEN,tempBULE,tempINY;
/*get target CIE color by input LIN and other signals.*/
static void __algoGetTargetColorFromCIE1976( SColorParams *inputColor, SCIEColor *targetColor)
{

	 CtApSwcLedAlgorithm_intensityPyramid(inputColor);
	 whiteBalance(inputColor,targetColor);
	  templ = (uint32_t)inputColor->intensity;
	  targetColor->Y = fix16_mul(fix16_from_int(templ), fix16Const1M);
	  targetColor->Y = fix16_mul(intensityAdjustConst, targetColor->Y);

}


/*
 * Lookup helper for BCM_RGB_Sonderfunktion == 2.
 *
 * The white-ratio lookup now uses the active localfit backend. The split
 * calculation stays in Algorithm.c because it uses private helpers and
 * state from this translation unit.
 */

static void __algoGetSonderfunktion2LookupPwmOutput(SColorParams * const inputColor,
                                     uint16_t originalTargetU,
                                     uint16_t originalTargetV,
                                     SCIEColor * const targetColorPtr,
                                     SPWMParams *outputPWM)
{
    uint16_t whiteRatioQ10000;
    uint8_t ratioValid;
    int16_t correctionTemperatureDegC_X10;
    uint8_t useRuntimeChannelBasis;

    SCIEColor boundaryWhiteCIE;
    SCIEColor boundaryPointCIE;
    SCIEColor dutyColor;
    SCIEColor savedRuntimeRedCIE;
    SCIEColor savedRuntimeGreenCIE;
    SCIEColor savedRuntimeBlueCIE;
    SCIEColor savedRuntimeWhiteCIE;

    fix16_t whiteRatio;
    fix16_t maxWhiteRatio;
    fix16_t ratioFactor;
    fix16_t lambda;
    fix16_t rgbRatio;
    fix16_t dirX;
    fix16_t dirY;

    useRuntimeChannelBasis = gAlgoSonderfunktion2HasCurrentChannelXyY;

    if (useRuntimeChannelBasis != 0u)
    {
        __algoSaveRuntimeChannelBasis(&savedRuntimeRedCIE,
                                      &savedRuntimeGreenCIE,
                                      &savedRuntimeBlueCIE,
                                      &savedRuntimeWhiteCIE);
        __algoApplyRuntimeChannelBasis();
    }

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
    gAlgoSonderfunktion2DebugUseRuntimeChannel = useRuntimeChannelBasis;
    gAlgoSonderfunktion2DebugCurrentRedXQ10000 = __algoFix16ToSignedQ10000(currentRedCIE.x);
    gAlgoSonderfunktion2DebugCurrentRedYQ10000 = __algoFix16ToSignedQ10000(currentRedCIE.y);
    gAlgoSonderfunktion2DebugCurrentGreenXQ10000 = __algoFix16ToSignedQ10000(currentGreenCIE.x);
    gAlgoSonderfunktion2DebugCurrentGreenYQ10000 = __algoFix16ToSignedQ10000(currentGreenCIE.y);
    gAlgoSonderfunktion2DebugCurrentBlueXQ10000 = __algoFix16ToSignedQ10000(currentBlueCIE.x);
    gAlgoSonderfunktion2DebugCurrentBlueYQ10000 = __algoFix16ToSignedQ10000(currentBlueCIE.y);
    gAlgoSonderfunktion2DebugCurrentWhiteXQ10000 = __algoFix16ToSignedQ10000(__algoGetWhitePointX());
    gAlgoSonderfunktion2DebugCurrentWhiteYQ10000 = __algoFix16ToSignedQ10000(__algoGetWhitePointY());
#endif

    ratioValid = AlgoRatioK_GetWhiteRatioQ10000((uint16_t)inputColor->colorU,
                                                (uint16_t)inputColor->colorV,
                                                &whiteRatioQ10000);

    if (ratioValid == 0u)
    {
        splitTargetColorValid = bfalse;
    }
    else
    {
        correctionTemperatureDegC_X10 = __algoGetSonderfunktion2CorrectionTemperatureDegC_X10(inputColor);
        if (__algoShouldBypassSonderfunktion2TempCorrection(originalTargetU,
                                                           originalTargetV,
                                                           (uint16_t)inputColor->colorU,
                                                           (uint16_t)inputColor->colorV))
        {
#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
            gAlgoSonderfunktion2DebugTargetU = originalTargetU;
            gAlgoSonderfunktion2DebugTargetV = originalTargetV;
            gAlgoSonderfunktion2DebugTempRawX10 = correctionTemperatureDegC_X10;
            gAlgoSonderfunktion2DebugW25RawQ10000 = whiteRatioQ10000;
            gAlgoSonderfunktion2DebugW25SafeQ10000 = whiteRatioQ10000;
            gAlgoSonderfunktion2DebugDeltaWQ10000 = 0;
            gAlgoSonderfunktion2DebugWformulaQ10000 = whiteRatioQ10000;
#endif
        }
        else
        {
            whiteRatioQ10000 = __algoApplyTemperatureWhiteRatioCorrection((uint16_t)inputColor->colorU,
                                                                          (uint16_t)inputColor->colorV,
                                                                          whiteRatioQ10000,
                                                                          correctionTemperatureDegC_X10);
        }

        /*
         * Use Sonderfunktion==0 boundary split result only to get the valid
         * max white ratio on the current white-to-target ray.
         */
        splitTargetColorValid = __algoSplitTargetColorByWhite(targetColorPtr,
                                                              &boundaryWhiteCIE,
                                                              &boundaryPointCIE);
        if ((splitTargetColorValid == btrue) && (targetColorPtr->Y > 0))
        {
            whiteRatio = __algoWhiteRgbParamToFix16(whiteRatioQ10000);
            maxWhiteRatio = fix16_div(boundaryWhiteCIE.Y, targetColorPtr->Y);

            if (whiteRatio > maxWhiteRatio)
            {
                whiteRatio = maxWhiteRatio;
            }

            if (whiteRatio < 0)
            {
                whiteRatio = 0;
            }

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
            gAlgoSonderfunktion2DebugWmaxBoundaryQ10000 = __algoClampWhiteRatioFix16ToQ10000(__algoClampFix16(maxWhiteRatio, 0, fix16_one));
            gAlgoSonderfunktion2DebugWfinalQ10000 = __algoClampWhiteRatioFix16ToQ10000(__algoClampFix16(whiteRatio, 0, fix16_one));
            __algoSyncSonderfunktion2DebugArray();
#endif

            if (__algoGetWhitePointY() <= fix16Const1M)
            {
                splitTargetColorValid = bfalse;
            }
            else
            {
                ratioFactor = fix16_div(fix16_mul(whiteRatio, targetColorPtr->y),
                                        __algoGetWhitePointY());

                lambda = fix16_sub(fix16_one, ratioFactor);

                if (lambda <= fix16Const1M)
                {
                    splitTargetColorValid = bfalse;
                }
                else
                {
                    splitRayT = fix16_div(fix16_one, lambda);

                    dirX = fix16_sub(targetColorPtr->x, __algoGetWhitePointX());
                    dirY = fix16_sub(targetColorPtr->y, __algoGetWhitePointY());

                    splitWhiteCIE.x = __algoGetWhitePointX();
                    splitWhiteCIE.y = __algoGetWhitePointY();
                    splitWhiteCIE.Y = fix16_mul(targetColorPtr->Y, whiteRatio);

                    splitPointCIE.x = fix16_add(__algoGetWhitePointX(), fix16_mul(splitRayT, dirX));
                    splitPointCIE.y = fix16_add(__algoGetWhitePointY(), fix16_mul(splitRayT, dirY));

                    rgbRatio = fix16_sub(fix16_one, whiteRatio);

                    if (rgbRatio < 0)
                    {
                        rgbRatio = 0;
                    }

                    splitPointCIE.Y = fix16_mul(targetColorPtr->Y, rgbRatio);

                    splitLambda = lambda;
                    splitOneMinusLambda = ratioFactor;
                    splitEdgeIndex = -3;
                    splitTargetColorValid = btrue;
                }
            }
        }
    }

    if (splitTargetColorValid == bfalse)
    {
        splitWhiteCIE.x = __algoGetWhitePointX();
        splitWhiteCIE.y = __algoGetWhitePointY();
        splitWhiteCIE.Y = 0;
        dutyColor = *targetColorPtr;
    }
    else
    {
        dutyColor = splitPointCIE;
    }

    __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
    __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);

#if (ALGO_SONDERFUNKTION2_TEMP_DEBUG_ENABLE != 0u)
    gAlgoSonderfunktion2DebugPWM_R = outputPWM->PWM_R;
    gAlgoSonderfunktion2DebugPWM_G = outputPWM->PWM_G;
    gAlgoSonderfunktion2DebugPWM_B = outputPWM->PWM_B;
    gAlgoSonderfunktion2DebugPWM_W = outputPWM->PWM_W;
    __algoSyncSonderfunktion2DebugArray();
#endif

    if (useRuntimeChannelBasis != 0u)
    {
        __algoRestoreRuntimeChannelBasis(&savedRuntimeRedCIE,
                                         &savedRuntimeGreenCIE,
                                         &savedRuntimeBlueCIE,
                                         &savedRuntimeWhiteCIE);
    }
}
/**
*@details   This is a function generates PWM by calling internal functions.
*           The whole process is this: 1. get target color by LIN signals.
*                                      2. adjust current LED attributes by temperature.
*                                      3. generate PWM.
*
*@param[in]  inputColor Color parameter.Only some of this structure will be used.
*
*@param[out] outputPWM  PWM duty cycle generated by this module.
*
*@retval    always true.
*/
bool_t algoGetPWMOutput(SColorParams * const inputColor, SPWMParams *outputPWM)
{
//	SCIEColor targetColor;
	SCIEColor dutyColor;
    uint16_t originalTargetU = (uint16_t)inputColor->colorU;
    uint16_t originalTargetV = (uint16_t)inputColor->colorV;
    /*Turn off lights by default.*/
    outputPWM->PWM_R = 0;
    outputPWM->PWM_G = 0;
    outputPWM->PWM_B = 0;
    outputPWM->PWM_W = 0;
    /*get target CIE color by input LIN and other signals.*/
    __algoGetTargetColorFromCIE1976(inputColor, (SCIEColor *)&targetColor);
    /*Compensate LED by temperature.*/
	__algoLedFixCIE(inputColor->ledTemprature/DTEMP_MULTIPLIED_RATIO);


    if (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion == 0)
    {
        splitTargetColorValid = __algoSplitTargetColorByWhite((const SCIEColor *)&targetColor, &splitWhiteCIE, &splitPointCIE);
        if (splitTargetColorValid == bfalse)
        {
            splitWhiteCIE.x = __algoGetWhitePointX();
            splitWhiteCIE.y = __algoGetWhitePointY();
            splitWhiteCIE.Y = 0;
            dutyColor = targetColor;
        }
        else
        {
            dutyColor = splitPointCIE;
        }
        /*Referring to target CIE-Color(targetColor), */
        /*some intensity control signals(inputColor)*/
        /*and current LED attributes, generate PWM*/
        __algoGetDutyRatio(inputColor, &dutyColor, outputPWM);
        /* White PWM from split white color. */
        __algoGetWhiteDutyRatio(inputColor, (SCIEColor *)&splitWhiteCIE, outputPWM);
    }
    else if ((sysLin_Stack.RGB.BCM_RGB_Sonderfunktion == 2) ||
             (sysLin_Stack.RGB.BCM_RGB_Sonderfunktion == 3))
    {
        __algoGetSonderfunktion2LookupPwmOutput(inputColor,
                                                originalTargetU,
                                                originalTargetV,
                                                (SCIEColor *)&targetColor,
                                                outputPWM);
    }
    else
    {
        __algoGetDutyRatio(inputColor, (SCIEColor *)&targetColor, outputPWM);
    }
  return btrue;
}







