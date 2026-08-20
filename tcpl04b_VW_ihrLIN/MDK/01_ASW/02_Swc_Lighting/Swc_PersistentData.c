#include "Swc_PersistentData.h"

#include <string.h>

#include "Rte_NvM.h"

#define SWC_LEGACY_MAGIC_NUMBER  (550U)

SFlashContent savedConfig;
SLineContent savedProduct;
volatile EFlashUpdate flashFlag;
uint16_t lastSavedSingalAdress;

static void Swc_PersistentData_UpdateLockState(void)
{
    if ((savedConfig.ledSupplier & 0x00F0U) == 0x0080U)
    {
        flashFlag = (EFlashUpdate)(flashFlag | EFlashUpdateLocked);
    }
    else
    {
        flashFlag = (EFlashUpdate)(flashFlag &
                    (EFlashUpdate)(~EFlashUpdateLocked));
    }
}

static void Swc_PersistentData_FromRte(const Rte_SwscConfigType *SourcePtr)
{
    (void)memset(&savedConfig, 0, sizeof(savedConfig));
    savedConfig.magicNO = SWC_LEGACY_MAGIC_NUMBER;
    savedConfig.platform = (EPlatformType)SourcePtr->platform;
    savedConfig.ledSupplier = SourcePtr->led_supplier;
    savedConfig.redx = SourcePtr->redx;
    savedConfig.redy = SourcePtr->redy;
    savedConfig.redY = SourcePtr->redY;
    savedConfig.greenx = SourcePtr->greenx;
    savedConfig.greeny = SourcePtr->greeny;
    savedConfig.greenY = SourcePtr->greenY;
    savedConfig.bluex = SourcePtr->bluex;
    savedConfig.bluey = SourcePtr->bluey;
    savedConfig.blueY = SourcePtr->blueY;
    savedConfig.factor = SourcePtr->intensity_factor;
    (void)memcpy(savedConfig.partNO, SourcePtr->part_number,
                 DMAX_PART_NUMBER_LENGTH);
    (void)memcpy(savedConfig.hardwareVersion, SourcePtr->hardware_version,
                 DHARDWARE_VERSION_LENGTH);
    savedConfig.groupAddr = SourcePtr->group_address_mask;
    savedConfig.singleAddr = SourcePtr->application_address;
    savedConfig.hardwareType = EHardwareTypeNoCap;
    (void)memcpy(savedConfig.serialNO, SourcePtr->serial_number,
                 DMAX_SERIAL_NUMBER_LENGTH);
    savedConfig.Cali_Volt[0] = SourcePtr->calibration_voltage[0];
    savedConfig.Cali_Volt[1] = SourcePtr->calibration_voltage[1];
    savedConfig.Cali_Volt[2] = SourcePtr->calibration_voltage[2];
    savedConfig.Cali_Temp = SourcePtr->calibration_temperature;
}

static void Swc_PersistentData_ToRte(Rte_SwscConfigType *DestinationPtr)
{
    DestinationPtr->platform = (uint16_t)savedConfig.platform;
    DestinationPtr->led_supplier = savedConfig.ledSupplier;
    DestinationPtr->redx = savedConfig.redx;
    DestinationPtr->redy = savedConfig.redy;
    DestinationPtr->redY = savedConfig.redY;
    DestinationPtr->greenx = savedConfig.greenx;
    DestinationPtr->greeny = savedConfig.greeny;
    DestinationPtr->greenY = savedConfig.greenY;
    DestinationPtr->bluex = savedConfig.bluex;
    DestinationPtr->bluey = savedConfig.bluey;
    DestinationPtr->blueY = savedConfig.blueY;
    DestinationPtr->intensity_factor = savedConfig.factor;
    (void)memcpy(DestinationPtr->part_number, savedConfig.partNO,
                 DMAX_PART_NUMBER_LENGTH);
    (void)memcpy(DestinationPtr->hardware_version,
                 savedConfig.hardwareVersion,
                 DHARDWARE_VERSION_LENGTH);
    DestinationPtr->group_address_mask = savedConfig.groupAddr;
    DestinationPtr->application_address = savedConfig.singleAddr;
    (void)memcpy(DestinationPtr->serial_number, savedConfig.serialNO,
                 DMAX_SERIAL_NUMBER_LENGTH);
    DestinationPtr->calibration_voltage[0] = savedConfig.Cali_Volt[0];
    DestinationPtr->calibration_voltage[1] = savedConfig.Cali_Volt[1];
    DestinationPtr->calibration_voltage[2] = savedConfig.Cali_Volt[2];
    DestinationPtr->calibration_temperature = savedConfig.Cali_Temp;
}

void Swc_PersistentData_Init(void)
{
    Rte_SwscConfigType config;

    (void)Rte_Call_NvM_ReadSwscConfig(&config);
    Swc_PersistentData_FromRte(&config);
    (void)memset(&savedProduct, 0, sizeof(savedProduct));
    lastSavedSingalAdress = savedConfig.singleAddr;
    flashFlag = EFlashUpdateFree;
    Swc_PersistentData_UpdateLockState();
}

void Swc_PersistentData_Save(void)
{
    Rte_SwscConfigType config;
    bool addressChanged;

    (void)Rte_Call_NvM_ReadSwscConfig(&config);

    addressChanged =
        (config.application_address != savedConfig.singleAddr);

    Swc_PersistentData_ToRte(&config);

    if (addressChanged &&
        (savedConfig.singleAddr >= 1U) &&
        (savedConfig.singleAddr <= 16U))
    {
        config.nad =
            (uint8_t)savedConfig.singleAddr;

        config.ihr_current_nad =
            (uint8_t)savedConfig.singleAddr;

        config.ihr_auto_nad =
            (uint8_t)savedConfig.singleAddr;
    }

    (void)Rte_Call_NvM_WriteSwscConfig(&config);
    Swc_PersistentData_UpdateLockState();
}

void Swc_PersistentData_StoreDiagnosticNad(uint8_t Nad)
{
    Rte_SwscConfigType config;

    if ((Nad < 1U) || (Nad > 16U))
    {
        return;
    }
    (void)Rte_Call_NvM_ReadSwscConfig(&config);
    config.nad = Nad;
    config.ihr_current_nad = Nad;
    config.ihr_auto_nad = Nad;
    (void)Rte_Call_NvM_WriteSwscConfig(&config);
}
