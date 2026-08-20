#include "pal_systick.h"

static volatile uint32_t s_milliseconds;

void SysTick_Handler(void)
{
    s_milliseconds++;
}

uint32_t systick_count_get(void)
{
    return s_milliseconds;
}

uint32_t systick_diff(uint32_t start_tick)
{
    /* Unsigned subtraction is defined modulo 2^32 and handles tick wrap. */
    return systick_count_get() - start_tick;
}

void delay_ms(uint32_t milliseconds)
{
    uint32_t start = systick_count_get();

    while (systick_diff(start) < milliseconds)
    {
    }
}

void delay_us(uint32_t microseconds)
{
    uint32_t start = SysTick->VAL;
    uint32_t reload = SysTick->LOAD + 1U;
    uint32_t required = microseconds * (DEFAULT_SYSTEM_CLOCK / 1000000UL);
    uint32_t elapsed = 0U;
    uint32_t current;

    while (elapsed < required)
    {
        current = SysTick->VAL;
        if (current <= start)
        {
            elapsed += start - current;
        }
        else
        {
            elapsed += start + reload - current;
        }
        start = current;
    }
}
