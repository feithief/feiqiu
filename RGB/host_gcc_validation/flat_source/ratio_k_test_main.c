#include <stdio.h>
#include <stdlib.h>
#include "Algorithm.h"
#include "HostRatioKDebug.h"
#include "ModuleFlash.h"
#include "ModuleTemperature.h"
#include "SystemLinComm.h"

static void print_q10000_xyz(const char *name, int32_t x, int32_t y, int32_t Y)
{
    printf("%s = (%0.4f, %0.4f, %0.4f)\n",
           name,
           (double)x / 10000.0,
           (double)y / 10000.0,
           (double)Y / 10000.0);
}

static void write_csv(const char *path, unsigned int rawU, unsigned int rawV)
{
    FILE *fp = fopen(path, "w");
    if (fp == NULL)
    {
        return;
    }

    fprintf(fp, "requested_raw_u,requested_raw_v,algo_pwm_input_u,algo_pwm_input_v,ratioK_lookup_u,ratioK_lookup_v,input_rgb_x,input_rgb_y,input_white_x,input_white_y,");
    fprintf(fp, "whiteRatioQ10000,ratioValid,splitValid,whiteRatioBeforeClamp,maxWhiteRatio,whiteRatioAfterClamp,boundaryWhiteY,targetColorY,whiteRatioUsed,rgbRatio,");
    fprintf(fp, "target_x,target_y,target_Y,splitPoint_x,splitPoint_y,splitPoint_Y,");
    fprintf(fp, "splitWhite_x,splitWhite_y,splitWhite_Y,duty_x,duty_y,duty_Y,");
    fprintf(fp, "verifyRgbPointErrX,verifyRgbPointErrY,verifyRatioSumErr,");
    fprintf(fp, "verifyTarget_x,verifyTarget_y,verifyTarget_Y,");
    fprintf(fp, "verifyTargetErrX,verifyTargetErrY,verifyTargetErrYValue,");
    fprintf(fp, "pwm_r,pwm_g,pwm_b,pwm_w\n");

    fprintf(fp,
            "%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld,%u,%u,%u,%u\n",
            rawU,
            rawV,
            rawU * 5u,
            rawV * 5u,
            (unsigned int)g_hostRatioKDebug.lookupColorU,
            (unsigned int)g_hostRatioKDebug.lookupColorV,
            (unsigned int)g_hostRatioKDebug.inputRgbPointX,
            (unsigned int)g_hostRatioKDebug.inputRgbPointY,
            (unsigned int)g_hostRatioKDebug.inputWhitePointX,
            (unsigned int)g_hostRatioKDebug.inputWhitePointY,
            (unsigned int)g_hostRatioKDebug.whiteRatioQ10000,
            (unsigned int)g_hostRatioKDebug.ratioValid,
            (unsigned int)g_hostRatioKDebug.splitValid,
            (long)g_hostRatioKDebug.whiteRatioBeforeClamp,
            (long)g_hostRatioKDebug.maxWhiteRatio,
            (long)g_hostRatioKDebug.whiteRatioAfterClamp,
            (long)g_hostRatioKDebug.boundaryWhiteY,
            (long)g_hostRatioKDebug.targetColorY,
            (long)g_hostRatioKDebug.whiteRatio,
            (long)g_hostRatioKDebug.rgbRatio,
            (long)g_hostRatioKDebug.targetColor_x,
            (long)g_hostRatioKDebug.targetColor_y,
            (long)g_hostRatioKDebug.targetColor_Y,
            (long)g_hostRatioKDebug.splitPointCIE_x,
            (long)g_hostRatioKDebug.splitPointCIE_y,
            (long)g_hostRatioKDebug.splitPointCIE_Y,
            (long)g_hostRatioKDebug.splitWhiteCIE_x,
            (long)g_hostRatioKDebug.splitWhiteCIE_y,
            (long)g_hostRatioKDebug.splitWhiteCIE_Y,
            (long)g_hostRatioKDebug.dutyColor_x,
            (long)g_hostRatioKDebug.dutyColor_y,
            (long)g_hostRatioKDebug.dutyColor_Y,
            (long)g_hostRatioKDebug.verifyRgbPointErrX,
            (long)g_hostRatioKDebug.verifyRgbPointErrY,
            (long)g_hostRatioKDebug.verifyRatioSumErr,
            (long)g_hostRatioKDebug.verifyTarget_x,
            (long)g_hostRatioKDebug.verifyTarget_y,
            (long)g_hostRatioKDebug.verifyTarget_Y,
            (long)g_hostRatioKDebug.verifyTargetErrX,
            (long)g_hostRatioKDebug.verifyTargetErrY,
            (long)g_hostRatioKDebug.verifyTargetErrYValue,
            (unsigned int)g_hostRatioKDebug.PWM_R,
            (unsigned int)g_hostRatioKDebug.PWM_G,
            (unsigned int)g_hostRatioKDebug.PWM_B,
            (unsigned int)g_hostRatioKDebug.PWM_W);

    fclose(fp);
}

int main(int argc, char *argv[])
{
    SColorParams inputColor;
    SPWMParams outputPWM;
    unsigned int inputU = 40u;
    unsigned int inputV = 94u;

    if (argc >= 3)
    {
        inputU = (unsigned int)strtoul(argv[1], NULL, 0);
        inputV = (unsigned int)strtoul(argv[2], NULL, 0);
    }

    HostRatioKDebug_Reset();
    inputColor.colorU = (uint16_t)inputU*5;
    inputColor.colorV = (uint16_t)inputV*5;
    inputColor.ledTemprature = 25 * DTEMP_MULTIPLIED_RATIO;
    inputColor.factor = savedConfig.factor;
    inputColor.intensity = 6400u;
    inputColor.tempAdjust = 6400u;
    inputColor.RGBFlag = bfalse;

    outputPWM.PWM_R = 0u;
    outputPWM.PWM_G = 0u;
    outputPWM.PWM_B = 0u;
    outputPWM.PWM_W = 0u;

    algoUpdateRedCIE(savedConfig.redx, savedConfig.redy, savedConfig.redY);
    algoUpdateGreenCIE(savedConfig.greenx, savedConfig.greeny, savedConfig.greenY);
    algoUpdateBlueCIE(savedConfig.bluex, savedConfig.bluey, savedConfig.blueY);
    algoUpdateWhiteCIE(savedConfig.whiteY);
    WitheBalanceTempInit();

    sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = 2u;
    (void)algoGetPWMOutput(&inputColor, &outputPWM);

    printf("=== Ratio K host test ===\n");
    printf("requested UV(raw)        = %u, %u\n", inputU, inputV);
    printf("algoGetPWMOutput input   = %u, %u\n", (unsigned int)inputColor.colorU, (unsigned int)inputColor.colorV);
    printf("ratioK LUT input         = %u, %u\n",
           (unsigned int)g_hostRatioKDebug.lookupColorU,
           (unsigned int)g_hostRatioKDebug.lookupColorV);
    printf("RGB point input          = (%0.4f, %0.4f)\n",
           (double)g_hostRatioKDebug.inputRgbPointX / 10000.0,
           (double)g_hostRatioKDebug.inputRgbPointY / 10000.0);
    printf("White point input        = (%0.4f, %0.4f)\n",
           (double)g_hostRatioKDebug.inputWhitePointX / 10000.0,
           (double)g_hostRatioKDebug.inputWhitePointY / 10000.0);
    printf("whiteRatioQ10000         = %u\n", (unsigned int)g_hostRatioKDebug.whiteRatioQ10000);
    printf("whiteRatio used          = %0.4f\n", (double)g_hostRatioKDebug.whiteRatioAfterClamp / 10000.0);
    printf("rgbRatio used            = %0.4f\n", (double)g_hostRatioKDebug.rgbRatio / 10000.0);
    printf("ratioValid               = %u\n", (unsigned int)g_hostRatioKDebug.ratioValid);
    printf("splitValid               = %u\n", (unsigned int)g_hostRatioKDebug.splitValid);

    print_q10000_xyz("targetColor", g_hostRatioKDebug.targetColor_x, g_hostRatioKDebug.targetColor_y, g_hostRatioKDebug.targetColor_Y);
    print_q10000_xyz("splitPointCIE", g_hostRatioKDebug.splitPointCIE_x, g_hostRatioKDebug.splitPointCIE_y, g_hostRatioKDebug.splitPointCIE_Y);
    print_q10000_xyz("splitWhiteCIE", g_hostRatioKDebug.splitWhiteCIE_x, g_hostRatioKDebug.splitWhiteCIE_y, g_hostRatioKDebug.splitWhiteCIE_Y);
    print_q10000_xyz("dutyColor", g_hostRatioKDebug.dutyColor_x, g_hostRatioKDebug.dutyColor_y, g_hostRatioKDebug.dutyColor_Y);
    printf("PWM output          = R:%u G:%u B:%u W:%u\n",
           (unsigned int)g_hostRatioKDebug.PWM_R,
           (unsigned int)g_hostRatioKDebug.PWM_G,
           (unsigned int)g_hostRatioKDebug.PWM_B,
           (unsigned int)g_hostRatioKDebug.PWM_W);

    write_csv("ratio_k_test_output.csv", inputU, inputV);
    return 0;
}