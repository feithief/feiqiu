#include "Led.h"

#include "pal_led.h"
#include "pal_led_def.h"

extern const ctrl_manager_instance_t led_ctrl_instance_def;

void Led_Init(void)
{
    pal_led_init();
    led_ctrl_instance_def.ctrl_register(LED_CHANNEL_0);
}

void Led_SetDuty(const uint16_t Duty[3])
{
    pal_led_duty_cycle_set(LED_CHANNEL_0, (uint16_t *)Duty);
}

void Led_Enable(bool Enable)
{
    pal_led_enable(LED_CHANNEL_0, Enable);
}
