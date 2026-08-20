#include "SystemLinComm.h"

#include <string.h>

#include "IoHwAb_Led.h"
#include "Rte_Bsw.h"
#include "Rte_Com.h"
#include "Swc_Communication.h"
#include "Swc_PersistentData.h"
#include "SystemDerating.h"
#include "SystemStatus.h"
#include "genLinConfig.h"

static SLinSignals oldLinData;
static uint8_t s_lastBrightness;
static uint8_t s_brightnessDifference;

static uint8_t Swc_Communication_OpenStatus(ELedChannel Channel)
{
    ELedStatus status = IoHwAb_Led_GetStatus(Channel);
    if (status == ELedStatusInit)
    {
        return init;
    }
    return ((status & ELedStatusOpen) != 0U) ? Fehler : kein_Fehler;
}

static uint8_t Swc_Communication_ShortStatus(ELedChannel Channel)
{
    ELedStatus status = IoHwAb_Led_GetStatus(Channel);
    if (status == ELedStatusInit)
    {
        return init;
    }
    return ((status & ELedStatusShort) != 0U) ? Fehler : kein_Fehler;
}

void systemLinCommInit(void)
{
    Rte_LightStatusType status;

    (void)memset(&oldLinData, 0, sizeof(oldLinData));
    (void)memset(&status, 0, sizeof(status));
    status.RedShort = init;
    status.GreenShort = init;
    status.BlueShort = init;
    status.RedOpen = init;
    status.GreenOpen = init;
    status.BlueOpen = init;
    status.TemperatureState = init;
    status.RamError = init;
    status.RomError = init;
    status.NvMError = init;
    (void)Rte_Write_LightStatus(&status);
}

void systemLinCommChangeNad(uint8_t newNad)
{
    (void)Rte_Call_Lin_SetApplicationAddress(newNad);
		Rte_Call_Diagnostic_SetNad(newNad);
}

void systemLinCommHandler(void)
{
    /* Lin/LinIf/Com are scheduled by BswM before the SWC runnable. */
}

bool_t systemLinCommSignalUpdate(void)
{
    uint8_t changedNad;
    Rte_LightCommandType command;

    if (Rte_Call_Diagnostic_TakeNadChange(&changedNad))
    {
        Swc_PersistentData_StoreDiagnosticNad(changedNad);
    }
    if (Rte_Read_LightCommand(&command) != E_OK)
    {
        return bfalse;
    }

    sysLin.BCM_RGB_Slave_Adresse = command.slave_address;
    sysLin.BCM_RGB_Adressierungsart = command.group_addressing;

    if (((sysLin.BCM_RGB_Adressierungsart == Einzeladresse) &&
         (sysLin.BCM_RGB_Slave_Adresse == savedConfig.singleAddr)) ||
        ((sysLin.BCM_RGB_Adressierungsart == Gruppenadresse) &&
         ((sysLin.BCM_RGB_Slave_Adresse & savedConfig.groupAddr) ==
          savedConfig.groupAddr)))
    {
        if ((oldLinData.BCM_RGB_Rot != command.red) ||
            (oldLinData.BCM_RGB_Gruen != command.green) ||
            (oldLinData.BCM_RGB_Blau != command.blue) ||
            (oldLinData.BCM_RGB_Intensitaet != command.brightness) ||
            (oldLinData.BCM_RGB_Normierung_Intensitaet !=
             command.brightness_normalized) ||
            (oldLinData.BCM_RGB_Dimmrampe != command.dimming_time) ||
            (oldLinData.BCM_RGB_Sonderfunktion != command.work_mode) ||
            (oldLinData.BCM_RGB_Bewertung_Dimmzeit !=
             command.dimming_time_relative) ||
            (oldLinData.BCM_RGB_Dimmzeit_Zeitbasis !=
             command.dimming_time_base_250ms) ||
            (oldLinData.BCM_RGB_Gueltigkeit != command.valid))
        {
            if (savedConfig.platform == EPlatformTypeMQB)
            {
                sysLin.BCM_RGB_Bewertung_Dimmzeit =
                    command.dimming_time_relative;
                sysLin.BCM_RGB_Gueltigkeit = command.valid;
            }
            else if (savedConfig.platform == EPlatformTypeMLBevo)
            {
                sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_absolut;
                sysLin.BCM_RGB_Gueltigkeit = 1U;
            }
            else if (savedConfig.platform == EPlatformTypeMLBevoVW)
            {
                sysLin.BCM_RGB_Bewertung_Dimmzeit =
                    command.dimming_time_relative;
                sysLin.BCM_RGB_Gueltigkeit = 1U;
            }

            if (sysLin.BCM_RGB_Gueltigkeit == 1U)
            {
                if ((savedConfig.ledSupplier & 0xFF00U) == 0x0000U)
                {
                    if ((command.red != 0U) || (command.green != 0U) ||
                        (command.blue != 0U))
                    {
                        sysLin.BCM_RGB_Rot = command.red;
                        sysLin.BCM_RGB_Gruen = command.green;
                        sysLin.BCM_RGB_Blau = command.blue;
                    }
                    sysLin.BCM_RGB_Sonderfunktion =
                        (command.work_mode <= Stellgliedtest) ?
                        command.work_mode : keine_Sonderfunktion;
                }
                else if ((savedConfig.ledSupplier & 0xFF00U) == 0x0200U)
                {
                    sysLin.BCM_RGB_Rot = 183U;
                    sysLin.BCM_RGB_Gruen = 255U;
                    sysLin.BCM_RGB_Blau = 57U;
                    if (command.work_mode != Showroommodus)
                    {
                        sysLin.BCM_RGB_Sonderfunktion =
                            (command.work_mode <= Stellgliedtest) ?
                            command.work_mode : keine_Sonderfunktion;
                    }
                }

                if (command.brightness <= 125U)
                {
                    uint8_t brightness = (command.brightness > 100U) ?
                                         100U : command.brightness;
                    s_brightnessDifference =
                        (brightness >= s_lastBrightness) ?
                        (uint8_t)(brightness - s_lastBrightness) :
                        (uint8_t)(s_lastBrightness - brightness);
                    s_lastBrightness = brightness;
                    sysLin.BCM_RGB_Intensitaet = brightness;
                }
                sysLin.BCM_RGB_Normierung_Intensitaet =
                    command.brightness_normalized;
                sysLin.BCM_RGB_Dimmrampe = command.dimming_time;
                sysLin.BCM_RGB_Dimmzeit_Zeitbasis =
                    command.dimming_time_base_250ms;

                if (sysLin.BCM_RGB_Sonderfunktion == Showroommodus)
                {
                    sysLin.BCM_RGB_Bewertung_Dimmzeit = Dimmzeit_relativ;
                    sysLin.BCM_RGB_Dimmrampe = 1U;
                }

                oldLinData.BCM_RGB_Rot = command.red;
                oldLinData.BCM_RGB_Gruen = command.green;
                oldLinData.BCM_RGB_Blau = command.blue;
                oldLinData.BCM_RGB_Intensitaet = command.brightness;
                oldLinData.BCM_RGB_Normierung_Intensitaet =
                    command.brightness_normalized;
                oldLinData.BCM_RGB_Dimmrampe = command.dimming_time;
                oldLinData.BCM_RGB_Sonderfunktion = command.work_mode;
                oldLinData.BCM_RGB_Bewertung_Dimmzeit =
                    command.dimming_time_relative;
                oldLinData.BCM_RGB_Dimmzeit_Zeitbasis =
                    command.dimming_time_base_250ms;
                oldLinData.BCM_RGB_Gueltigkeit = command.valid;
                sysStatus.newLinSignal = btrue;
            }
            return btrue;
        }
    }
    return bfalse;
}

void systemLinCommSleep(void)
{
    (void)memset(&oldLinData, 0, sizeof(oldLinData));
    Rte_Call_Lin_GoToSleep();
}

void systemLinCommRespUpdate(void)
{
    Rte_LightStatusType status;

    status.ResponseError = 0U;
    status.NvMError = kein_Fehler;
    status.RamError = kein_Fehler;
    status.RomError =
        ((sysStatus.errFlag & ESystemErrorLedError) != 0U) ?
        Fehler : kein_Fehler;
    status.TemperatureState = systemDeratingGetTempstate();
    status.RedOpen = Swc_Communication_OpenStatus(ELedChannelRed);
    status.RedShort = Swc_Communication_ShortStatus(ELedChannelRed);
    status.GreenOpen = Swc_Communication_OpenStatus(ELedChannelGreen);
    status.GreenShort = Swc_Communication_ShortStatus(ELedChannelGreen);
    status.BlueOpen = Swc_Communication_OpenStatus(ELedChannelBlue);
    status.BlueShort = Swc_Communication_ShortStatus(ELedChannelBlue);
    (void)Rte_Write_LightStatus(&status);
}

void Swc_Communication_Init(void)
{
    systemLinCommInit();
}

void Swc_Communication_Handler(void)
{
    systemLinCommHandler();
}

bool_t Swc_Communication_SignalUpdate(void)
{
    return systemLinCommSignalUpdate();
}

void Swc_Communication_RespUpdate(void)
{
    systemLinCommRespUpdate();
}

void Swc_Communication_ChangeNad(uint8_t new_nad)
{
    systemLinCommChangeNad(new_nad);
}

void Swc_Communication_Sleep(void)
{
    systemLinCommSleep();
}

uint8_t GetBrightnessDiffForSpeedBasedDimming(void)
{
    return s_brightnessDifference;
}
