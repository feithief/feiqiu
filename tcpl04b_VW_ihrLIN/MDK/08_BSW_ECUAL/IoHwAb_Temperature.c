#include "IoHwAb_Temperature.h"

#include "IoHwAb_Adc.h"
#include "IoHwAb_Battery.h"
#include "IoHwAb_Led.h"
#include "IoHwAb_Time.h"

#define DTEMP_UPDATE_CYCLE_PERIOD       (250U)
#define DTEMP_MCU_FILTER_LEVEL          (2)
#define DTEMP_LED_FILTER_LEVEL          (16)
#define DTEMP_LED_FILTER2_LEVEL         (20)
#define DLED_CURRENT_MAX                (21)

/* 521.39 production defaults from ModuleFlash.c. */
#define DTHERMAL_RATIO_RED              (48)
#define DTHERMAL_RATIO_GREEN            (76)
#define DTHERMAL_RATIO_BLUE             (34)
#define DTHERMAL_RATIO_MCU              (76)
#define DRATING_LOWER_VOLTAGE           (8)
#define DRATING_HIGHER_VOLTAGE          (1)

static int16_t s_led_temp_deci_c;
static int16_t s_mcu_temp_deci_c;
static int16_t s_raw_mcu_temp_deci_c = 250;
static int16_t s_junction_filter;
static int16_t s_led_filter;
static int16_t s_led_filter2;
static bool s_initialized;
static uint16_t s_update_tick;

void IoHwAb_Temperature_Init(void)
{
    s_led_temp_deci_c = 0;
    s_mcu_temp_deci_c = 0;
    s_raw_mcu_temp_deci_c = 250;
    s_junction_filter = 0;
    s_led_filter = 0;
    s_led_filter2 = 0;
    s_initialized = false;
    s_update_tick = 0U;
    IoHwAb_Temperature_Update();
}

void IoHwAb_Temperature_Update(void)
{
    int16_t measured;
    int16_t tempTemperature;
    int16_t ratioAdjusting;
    int32_t pMcu = 0;
    int32_t pRed;
    int32_t pGreen;
    int32_t pBlue;
    int32_t batteryMv = (int32_t)IoHwAb_Battery_GetVoltage();
    uint16_t now = IoHwAb_Time_Get();

    if (IoHwAb_Adc_GetMcuTempDeciC(&measured))
    {
        s_raw_mcu_temp_deci_c = measured;
    }

    if (!s_initialized)
    {
        s_junction_filter = s_raw_mcu_temp_deci_c;
    }
    else
    {
        s_junction_filter = (int16_t)(
            ((int32_t)s_junction_filter * (DTEMP_MCU_FILTER_LEVEL - 1) +
             s_raw_mcu_temp_deci_c) / DTEMP_MCU_FILTER_LEVEL);
    }

    pRed = (int32_t)IoHwAb_Led_GetPWM(ELedChannelRed) *
           (int32_t)IoHwAb_Led_GetVoltage(ELedChannelRed) /
           (int32_t)SWSC_PWM_PERIOD;
    pGreen = (int32_t)IoHwAb_Led_GetPWM(ELedChannelGreen) *
             (int32_t)IoHwAb_Led_GetVoltage(ELedChannelGreen) /
             (int32_t)SWSC_PWM_PERIOD;
    pBlue = (int32_t)IoHwAb_Led_GetPWM(ELedChannelBlue) *
            (int32_t)IoHwAb_Led_GetVoltage(ELedChannelBlue) /
            (int32_t)SWSC_PWM_PERIOD;

    pRed = pRed * DTHERMAL_RATIO_RED * DLED_CURRENT_MAX;
    pGreen = pGreen * DTHERMAL_RATIO_GREEN * DLED_CURRENT_MAX;
    pBlue = pBlue * DTHERMAL_RATIO_BLUE * DLED_CURRENT_MAX;

    pMcu  = (int32_t)IoHwAb_Led_GetPWM(ELedChannelRed) *
            (batteryMv - IoHwAb_Led_GetVoltage(ELedChannelRed));
    pMcu += (int32_t)IoHwAb_Led_GetPWM(ELedChannelGreen) *
            (batteryMv - IoHwAb_Led_GetVoltage(ELedChannelGreen));
    pMcu += (int32_t)IoHwAb_Led_GetPWM(ELedChannelBlue) *
            (batteryMv - IoHwAb_Led_GetVoltage(ELedChannelBlue));

    if (batteryMv >= 12000)
    {
        ratioAdjusting = (int16_t)(((batteryMv - 12000) / 1000) *
                                   DRATING_HIGHER_VOLTAGE / 10);
    }
    else
    {
        ratioAdjusting = (int16_t)(((12000 - batteryMv) / 1000) *
                                   DRATING_LOWER_VOLTAGE / 10);
    }

    pMcu = pMcu / (int32_t)SWSC_PWM_PERIOD *
           (DTHERMAL_RATIO_MCU + ratioAdjusting);
    pMcu = pMcu * DLED_CURRENT_MAX +
           10 * batteryMv * DTHERMAL_RATIO_MCU;

    tempTemperature = (int16_t)((int32_t)s_junction_filter -
        ((pMcu - pRed - pGreen - pBlue) / 1000000L) *
        DTEMP_MULTIPLIED_RATIO);
    tempTemperature =
        (int16_t)(tempTemperature + 10 * DTEMP_MULTIPLIED_RATIO);

    if (!s_initialized)
    {
        s_led_filter = tempTemperature;
    }
    else
    {
        s_led_filter2 = (int16_t)(
            ((int32_t)s_led_filter * (DTEMP_LED_FILTER_LEVEL - 1) +
             tempTemperature) / DTEMP_LED_FILTER_LEVEL);
        s_led_filter = (int16_t)(
            ((int32_t)s_led_filter * (DTEMP_LED_FILTER2_LEVEL - 1) +
             s_led_filter2) / DTEMP_LED_FILTER2_LEVEL);
    }

    if (IoHwAb_Time_IsAfter(
            now, (uint16_t)(s_update_tick + DTEMP_UPDATE_CYCLE_PERIOD)) ||
        !s_initialized)
    {
        s_update_tick = now;
        s_led_temp_deci_c = s_led_filter;
        s_mcu_temp_deci_c = s_junction_filter;
    }
    s_initialized = true;
}

int16_t IoHwAb_Temperature_GetLed(void)
{
    /* ModuleTemperature.c updates once more when LED temperature is queried. */
    IoHwAb_Temperature_Update();
    return s_led_temp_deci_c;
}

int16_t IoHwAb_Temperature_GetMcu(void)
{
    return s_mcu_temp_deci_c;
}
