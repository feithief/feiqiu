#include <stdio.h>

#include "../Algorithm/Algorithm.h"
#include "../Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit.h"
#include "include/gcc_stub.h"

extern SSysLinStack sysLin_Stack;
extern volatile int16_t gAlgoSonderfunktion2DebugTempRawX10;
extern volatile uint16_t gAlgoSonderfunktion2DebugW25RawQ10000;
extern volatile uint16_t gAlgoSonderfunktion2DebugW25SafeQ10000;
extern volatile int32_t gAlgoSonderfunktion2DebugDeltaWQ10000;
extern volatile uint16_t gAlgoSonderfunktion2DebugWformulaQ10000;
extern volatile uint16_t gAlgoSonderfunktion2DebugWfinalQ10000;

static void init_t25_channels(void)
{
    algoUpdateRedCIE(7025, 2967, 29000);
    algoUpdateGreenCIE(1370, 7069, 47060);
    algoUpdateBlueCIE(1521, 284, 8733);
    algoUpdateWhiteCIE(55580);
    WitheBalanceTempInit();
}

int main(void)
{
    SColorParams inputColor;
    SPWMParams outputPWM;
    uint16_t whiteRatioQ10000 = 0u;
    uint8_t ratioValid = 0u;
    bool_t pwmValid = 0;

    init_t25_channels();

    inputColor.colorU = 200u;
    inputColor.colorV = 470u;
    inputColor.ledTemprature = 1000u;
    inputColor.factor = 100u;
    inputColor.intensity = 10000u;
    inputColor.tempAdjust = 10000u;
    inputColor.RGBFlag = 0;

    ratioValid = AlgoRatioK_GetWhiteRatioQ10000(inputColor.colorU,
                                                inputColor.colorV,
                                                &whiteRatioQ10000);

    printf("Input: U=%u, V=%u, ledTemp=%d (x10), mcuTempDebug=%d (x10)\n",
           (unsigned)inputColor.colorU,
           (unsigned)inputColor.colorV,
           (int)inputColor.ledTemprature,
           (int)moduleTempMcuGet_debug());

    printf("AlgoRatioK_GetWhiteRatioQ10000: valid=%u, W25_Q10000=%u\n",
           (unsigned)ratioValid,
           (unsigned)whiteRatioQ10000);

    sysLin_Stack.RGB.BCM_RGB_Sonderfunktion = 2u;
    pwmValid = algoGetPWMOutput(&inputColor, &outputPWM);

    printf("Temp correction debug: TempRawX10=%d, W25Raw=%u, W25Safe=%u, DeltaW_Q10000=%ld, Wformula=%u, Wfinal=%u\n",
           (int)gAlgoSonderfunktion2DebugTempRawX10,
           (unsigned)gAlgoSonderfunktion2DebugW25RawQ10000,
           (unsigned)gAlgoSonderfunktion2DebugW25SafeQ10000,
           (long)gAlgoSonderfunktion2DebugDeltaWQ10000,
           (unsigned)gAlgoSonderfunktion2DebugWformulaQ10000,
           (unsigned)gAlgoSonderfunktion2DebugWfinalQ10000);

    printf("algoGetPWMOutput: valid=%d, mode=%u, PWM_R=%u, PWM_G=%u, PWM_B=%u, PWM_W=%u\n",
           pwmValid,
           (unsigned)sysLin_Stack.RGB.BCM_RGB_Sonderfunktion,
           (unsigned)outputPWM.PWM_R,
           (unsigned)outputPWM.PWM_G,
           (unsigned)outputPWM.PWM_B,
           (unsigned)outputPWM.PWM_W);

    return 0;
}

