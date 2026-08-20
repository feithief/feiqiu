#include "IoHwAb_Led.h"

#include "Adc.h"
#include "pal_led.h"
#include "pal_led_def.h"

#define LED_DETECTION_PWM_MIN              (480U)
#define LED_DETECTION_OPEN_MV              (6999)
#define LED_DETECTION_SHORT_MV             (400)
#define LED_DETECTION_CONFIRM_COUNT        (100)

#define LED_MASK_RED                       (1U << LED_R)
#define LED_MASK_GREEN                     (1U << LED_G)
#define LED_MASK_BLUE                      (1U << LED_B)

static uint16_t s_pwm[3];
static ELedStatus s_status[3] =
    {ELedStatusInit, ELedStatusInit, ELedStatusInit};
static int16_t s_voltage_mv[3] = {2300, 2400, 2400};
static int16_t s_status_counter[3] =
{
    LED_DETECTION_CONFIRM_COUNT,
    LED_DETECTION_CONFIRM_COUNT,
    LED_DETECTION_CONFIRM_COUNT
};
static uint8_t s_detection_mask;
static uint8_t s_calibrationControl;
uint8_t Led_Error_flag;

extern const ctrl_manager_instance_t led_ctrl_instance_def;

static uint8_t IoHwAb_Led_Index(ELedChannel channel)
{
    return (channel == ELedChannelRed) ? LED_R :
           (channel == ELedChannelGreen) ? LED_G : LED_B;
}

static void IoHwAb_Led_UpdateOneStatus(uint8_t channel)
{
    ELedStatus temporaryStatus = s_status[channel];

    if (s_voltage_mv[channel] >= LED_DETECTION_OPEN_MV)
    {
        temporaryStatus = ELedStatusOpen;
    }
    else if (s_voltage_mv[channel] < LED_DETECTION_SHORT_MV)
    {
        temporaryStatus = ELedStatusShort;
    }
    else
    {
        temporaryStatus = ELedStatusNormal;
        s_status[channel] = ELedStatusNormal;
        s_status_counter[channel] = LED_DETECTION_CONFIRM_COUNT;
    }

    if (((temporaryStatus & (ELedStatusOpen | ELedStatusShort)) != 0U) &&
        (s_status_counter[channel] > 0))
    {
        s_status_counter[channel]--;
    }
    else
    {
        s_status[channel] = temporaryStatus;
        s_status_counter[channel] = LED_DETECTION_CONFIRM_COUNT;
    }
}

void IoHwAb_Led_Init(void)
{
    uint8_t index;

    pal_led_init();
    led_ctrl_instance_def.ctrl_register(LED_CHANNEL_0);
    pal_led_duty_cycle_set(LED_CHANNEL_0, s_pwm);
    pal_led_enable(LED_CHANNEL_0, true);

    s_detection_mask = 0U;
    Led_Error_flag = 0U;
    for (index = 0U; index < 3U; index++)
    {
        s_status[index] = ELedStatusInit;
        s_status_counter[index] = LED_DETECTION_CONFIRM_COUNT;
    }
}

void IoHwAb_Led_UpdatePWM(uint16_t red, uint16_t green, uint16_t blue)
{
    uint16_t calibrationDuty = (uint16_t)((SWSC_PWM_PERIOD / 10U) * 4U);

    if (s_calibrationControl == 1U)
    {
        red = calibrationDuty;
        green = 0U;
        blue = 0U;
    }
    else if (s_calibrationControl == 2U)
    {
        red = 0U;
        green = calibrationDuty;
        blue = 0U;
    }
    else if (s_calibrationControl == 3U)
    {
        red = 0U;
        green = 0U;
        blue = calibrationDuty;
    }

    s_pwm[LED_R] = (red < SWSC_PWM_PERIOD) ?
                   red : (SWSC_PWM_PERIOD - 1U);
    s_pwm[LED_G] = (green < SWSC_PWM_PERIOD) ?
                   green : (SWSC_PWM_PERIOD - 1U);
    s_pwm[LED_B] = (blue < SWSC_PWM_PERIOD) ?
                   blue : (SWSC_PWM_PERIOD - 1U);

    s_detection_mask = 0U;
    if (s_pwm[LED_R] > LED_DETECTION_PWM_MIN)
    {
        s_detection_mask |= LED_MASK_RED;
    }
    if (s_pwm[LED_G] > LED_DETECTION_PWM_MIN)
    {
        s_detection_mask |= LED_MASK_GREEN;
    }
    if (s_pwm[LED_B] > LED_DETECTION_PWM_MIN)
    {
        s_detection_mask |= LED_MASK_BLUE;
    }

    pal_led_duty_cycle_set(LED_CHANNEL_0, s_pwm);
}

void IoHwAb_Led_PowerOff(void)
{
    IoHwAb_Led_UpdatePWM(0U, 0U, 0U);
}

void IoHwAb_Led_UpdateStatus(void)
{
    int16_t voltage;

    if (s_detection_mask == 0U)
    {
        (void)Adc_StartLedVoltageCycle(0U);
        return;
    }

    if (((s_detection_mask & LED_MASK_RED) != 0U) &&
        Adc_GetLedForwardVoltageMv(LED_R, &voltage))
    {
        s_voltage_mv[LED_R] = voltage;
    IoHwAb_Led_UpdateOneStatus(LED_R);
    }
    if (((s_detection_mask & LED_MASK_GREEN) != 0U) &&
        Adc_GetLedForwardVoltageMv(LED_G, &voltage))
    {
        s_voltage_mv[LED_G] = voltage;
    IoHwAb_Led_UpdateOneStatus(LED_G);
    }
    if (((s_detection_mask & LED_MASK_BLUE) != 0U) &&
        Adc_GetLedForwardVoltageMv(LED_B, &voltage))
    {
        s_voltage_mv[LED_B] = voltage;
    IoHwAb_Led_UpdateOneStatus(LED_B);
    }

    Led_Error_flag =
        (((s_status[LED_R] | s_status[LED_G] | s_status[LED_B]) &
          (ELedStatusOpen | ELedStatusShort)) != 0U) ? 1U : 0U;

    (void)Adc_StartLedVoltageCycle(s_detection_mask);
}

void IoHwAb_Led_Sleep(void)
{
    IoHwAb_Led_PowerOff();
    pal_led_enable(LED_CHANNEL_0, false);
    s_status[LED_R] = ELedStatusInit;
    s_status[LED_G] = ELedStatusInit;
    s_status[LED_B] = ELedStatusInit;
}

ELedStatus IoHwAb_Led_GetStatus(ELedChannel channel)
{
    return s_status[IoHwAb_Led_Index(channel)];
}

int16_t IoHwAb_Led_GetVoltage(ELedChannel channel)
{
    return s_voltage_mv[IoHwAb_Led_Index(channel)];
}

uint16_t IoHwAb_Led_GetPWM(ELedChannel channel)
{
    return s_pwm[IoHwAb_Led_Index(channel)];
}

void IoHwAb_Led_ResetDetection(void)
{
    uint8_t index;

    Led_Error_flag = 0U;
    for (index = 0U; index < 3U; index++)
    {
        s_status_counter[index] = LED_DETECTION_CONFIRM_COUNT;
        if ((s_status[index] & (ELedStatusOpen | ELedStatusShort)) != 0U)
        {
            s_status[index] = ELedStatusInit;
        }
    }
}

bool IoHwAb_Led_SetCalibrationControl(uint8_t Control)
{
    if (Control > 4U)
    {
        return false;
    }
    s_calibrationControl = Control;
    return true;
}

uint8_t IoHwAb_Led_GetCalibrationControl(void)
{
    return s_calibrationControl;
}
