#include "Lin.h"

#include "genLinConfig.h"
#include "lin_driver_api.h"
#include "lin_main.h"

static bool s_initialized;
static uint8_t s_taskDivider;

void Lin_Init(const Lin_ConfigType *Config)
{
    (void)Config;
    s_taskDivider = 0U;
    s_initialized = (l_sys_init() == 0U);
}

void Lin_DeInit(void)
{
    lin_hal_wait_for_break();
    s_initialized = false;
}

void Lin_MainFunction(void)
{
    if (!s_initialized)
    {
        return;
    }
    s_taskDivider++;
    if (s_taskDivider >= LIN_TASK_CYCLE_MS)
    {
        s_taskDivider = 0U;
        /* Frozen IHR protocol kernel: only the AUTOSAR scheduling shell is
         * new.  Frame parsing, checksum, transport and node configuration
         * remain executed by the proven IHR functions. */
        ld_task();
    }
}

void Lin_GoToSleepInternal(uint8_t Channel)
{
    if (Channel == LIN_CHANNEL_0)
    {
        lin_hal_wait_for_break();
    }
}

void Lin_WakeupInternal(uint8_t Channel)
{
    if ((Channel == LIN_CHANNEL_0) && s_initialized)
    {
        lin_hal_tx_wake_up();
    }
}
