#include "Rte_Bsw.h"

#include "genLinConfig.h"
#include "Lin.h"
#include "lin_main.h"

static uint8_t s_lastDiagnosticNad;

static uint8_t Rte_ProtectFrameId(uint8_t FrameId)
{
    uint8_t id = (uint8_t)(FrameId & 0x3FU);
    uint8_t p0 = (uint8_t)(((id >> 0U) ^ (id >> 1U) ^
                            (id >> 2U) ^ (id >> 4U)) & 1U);
    uint8_t p1 = (uint8_t)(~((id >> 1U) ^ (id >> 3U) ^
                              (id >> 4U) ^ (id >> 5U)) & 1U);
    return (uint8_t)(id | (uint8_t)(p0 << 6U) | (uint8_t)(p1 << 7U));
}

void Rte_Call_Diagnostic_SetNad(uint8_t Nad)
{
    l_set_current_NAD(Nad);
    SlaveNodeNAD = Nad;
}

bool Rte_Call_Diagnostic_TakeNadChange(uint8_t *NadPtr)
{
    uint8_t currentNad = l_get_current_NAD();

    if ((NadPtr == NULL) || (currentNad == s_lastDiagnosticNad))
    {
        return false;
    }
    s_lastDiagnosticNad = currentNad;
    *NadPtr = currentNad;
    return true;
}

uint8_t Rte_Call_Diagnostic_GetNad(void)
{
    return l_get_current_NAD();
}

void Rte_Call_Lin_GoToSleep(void)
{
    Lin_GoToSleepInternal(LIN_CHANNEL_0);
}

//Std_ReturnType Rte_Call_Lin_SetApplicationAddress(uint8_t Address)
//{
//    if ((Address == 0U) || (Address > 0x3BU))
//    {
//        return E_NOT_OK;
//    }
//    g_lin_frame_ctrl[FRAME_RGBs_Slave_08a].frame.msg_id.lo = Address;
//    g_lin_frame_ctrl[FRAME_RGBs_Slave_08a].frame.pid =
//        Rte_ProtectFrameId(Address);
//    return E_OK;
//}
Std_ReturnType Rte_Call_Lin_SetApplicationAddress(uint8_t Address)
{
    if ((Address == 0U) || (Address > 16U))
    {
        return E_NOT_OK;
    }

    /* Stop current LIN/IHR stack */
    Lin_DeInit();

    /* New address becomes the initialization NAD */
    SlaveNodeNAD = Address;

    /* Full LIN/IHR re-initialization */
    Lin_Init(NULL);

    return E_OK;
}
