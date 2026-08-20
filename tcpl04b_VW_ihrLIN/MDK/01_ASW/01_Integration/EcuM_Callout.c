#include "app.h"
#include "BswM.h"
#include "EcuM.h"
#include "Swc_Lighting.h"
#include "pal_systick.h"

#if defined(__ICCARM__)
int main(void)
#else
int TcMain(void)
#endif
{
    uint32_t tick_1ms;
    uint32_t tick_10ms;
    uint32_t tick_100ms;

    interrupt_disable();
#if defined(APP_MATCH_BOOT)
    system_remap_config(REG_READ32(BOOT_APP_VECTOR_WORD_ADDR), true);
#endif

    EcuM_Init();
    Swc_Lighting_Init();

    tick_1ms = systick_count_get();
    tick_10ms = tick_1ms;
    tick_100ms = tick_1ms;
    interrupt_enable();

    for (;;)
    {
        if (systick_diff(tick_1ms) >= 1U)
        {
            tick_1ms = systick_count_get();
            BswM_MainFunction();
            Swc_Lighting_MainFunction1ms();
        }
        if (systick_diff(tick_10ms) >= 10U)
        {
            tick_10ms = systick_count_get();
            Swc_Lighting_MainFunction10ms();
        }
        if (systick_diff(tick_100ms) >= 100U)
        {
            tick_100ms = systick_count_get();
            Swc_Lighting_MainFunction100ms();
        }
    }
}
