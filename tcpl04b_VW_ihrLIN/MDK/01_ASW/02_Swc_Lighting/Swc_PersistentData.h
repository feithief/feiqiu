#ifndef SWC_PERSISTENT_DATA_H
#define SWC_PERSISTENT_DATA_H

#include <stdint.h>
#include "SystemType.h"

#define DMAX_SERIAL_NUMBER_LENGTH  (20U)
#define DMAX_PART_NUMBER_LENGTH    (11U)
#define DHARDWARE_VERSION_LENGTH   (3U)

typedef enum
{
    EPlatformTypeMQB = 0x0000,
    EPlatformTypeMLBevo = 0x0001,
    EPlatformTypeMLBevoVW = 0x0002,
    EPlatformTypeMax = 0xFFFF
} EPlatformType;

typedef enum
{
    EFlashUpdateFree = 0x00,
    EFlashUpdateProcessing = 0x01,
    EFlashAutoAddressStart = 0x02,
    EFlashUpdateLocked = 0x80
} EFlashUpdate;

typedef enum
{
    EHardwareTypeNoCap = 0x11,
    EHardwareTypeCap = 0x1A
} EHardwareType;

typedef struct
{
    uint8_t buf[18];
} SLineContent;

/*
 * SWC private persistent-data mirror.  The layout is intentionally kept
 * identical to the RLNE1A application because several production DIDs access
 * contiguous byte ranges inside this object.  NvM itself only sees the stable
 * Rte_SwscConfigType through Swc_PersistentData_Init/Save.
 */
typedef struct
{
    uint16_t crc;
    uint16_t magicNO;
    EPlatformType platform;
    uint16_t ledSupplier;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint16_t factor;
    uint8_t partNO[DMAX_PART_NUMBER_LENGTH + 1U];
    uint8_t hardwareVersion[DHARDWARE_VERSION_LENGTH + 1U];
    uint16_t groupAddr;
    uint16_t singleAddr;
    EHardwareType hardwareType;
    uint8_t serialNO[DMAX_SERIAL_NUMBER_LENGTH];
    int16_t RthermolRatio;
    int16_t GthermolRatio;
    int16_t BthermolRatio;
    int16_t MCUthermolRatio;
    int8_t RatingLowerVoltage;
    int8_t RatingHigherVoltage;
    uint8_t RCapCompensate;
    uint8_t GCapCompensate;
    uint8_t BCapCompensate;
    uint8_t ProductID[5];
    uint16_t Cali_Volt[3];
    uint16_t Cali_Temp;
    uint8_t Volt_Califlag;
} SFlashContent;

extern volatile EFlashUpdate flashFlag;
extern SFlashContent savedConfig;
extern SLineContent savedProduct;
extern uint16_t lastSavedSingalAdress;

void Swc_PersistentData_Init(void);
void Swc_PersistentData_Save(void);
void Swc_PersistentData_StoreDiagnosticNad(uint8_t Nad);

#endif
