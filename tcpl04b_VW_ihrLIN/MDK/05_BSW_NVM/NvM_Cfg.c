#include "NvM_Cfg.h"

#define NVM_SWSC_CONFIG_MAGIC          (0x53575343UL)
#define NVM_SWSC_CONFIG_VERSION        (4U)
#define NVM_SWSC_CONFIG_LEGACY_V3_VERSION (3U)
#define NVM_SWSC_CONFIG_LEGACY_VERSION (2U)

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint8_t nad;
    uint8_t brightness_factor;
    uint16_t application_address;
    uint16_t group_address_mask;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint8_t Utemp;
    uint8_t Vtemp;
    uint8_t x1, y1l, y1h;
    uint8_t x2, y2l, y2h;
    uint8_t x3, y3l, y3h;
    uint8_t x4, y4l, y4h;
    uint8_t x5, y5l, y5h;
    uint8_t x6, y6l, y6h;
    uint8_t x7, y7l, y7h;
    uint8_t x8, y8l, y8h;
    uint8_t x9, y9l, y9h;
} NvM_SwscConfigLegacyV2Type;

/* Exact version-3 layout, used only to migrate existing flash content. */
typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint8_t nad;
    uint8_t brightness_factor;
    uint16_t application_address;
    uint16_t group_address_mask;
    uint16_t redx;
    uint16_t redy;
    uint32_t redY;
    uint16_t greenx;
    uint16_t greeny;
    uint32_t greenY;
    uint16_t bluex;
    uint16_t bluey;
    uint32_t blueY;
    uint8_t Utemp;
    uint8_t Vtemp;
    uint8_t x1, y1l, y1h;
    uint8_t x2, y2l, y2h;
    uint8_t x3, y3l, y3h;
    uint8_t x4, y4l, y4h;
    uint8_t x5, y5l, y5h;
    uint8_t x6, y6l, y6h;
    uint8_t x7, y7l, y7h;
    uint8_t x8, y8l, y8h;
    uint8_t x9, y9l, y9h;
    uint16_t led_supplier;
    uint16_t platform;
    uint16_t intensity_factor;
    uint8_t part_number[11];
    uint8_t hardware_version[3];
    uint8_t serial_number[20];
    uint16_t calibration_voltage[3];
    uint16_t calibration_temperature;
} NvM_SwscConfigLegacyV3Type;

static Rte_SwscConfigType s_swscConfigRam;

static const Rte_SwscConfigType s_swscConfigRomDefault =
{
    .magic = NVM_SWSC_CONFIG_MAGIC,
    .version = NVM_SWSC_CONFIG_VERSION,
    .nad = 8U,
    .brightness_factor = 100U,
    .application_address = 0x0008U,
    .group_address_mask = 0x0001U,
    .redx = 7006U, .redy = 2967U, .redY = 19680U,
    .greenx = 1587U, .greeny = 7256U, .greenY = 29106U,
    .bluex = 1518U, .bluey = 294U, .blueY = 1968U,
    .Utemp = 80U, .Vtemp = 188U,
    .x1 = 10U, .y1l = 3U, .y1h = 0U,
    .x2 = 20U, .y2l = 17U, .y2h = 0U,
    .x3 = 30U, .y3l = 50U, .y3h = 0U,
    .x4 = 40U, .y4l = 100U, .y4h = 0U,
    .x5 = 50U, .y5l = 180U, .y5h = 0U,
    .x6 = 60U, .y6l = 24U, .y6h = 1U,
    .x7 = 70U, .y7l = 154U, .y7h = 1U,
    .x8 = 80U, .y8l = 58U, .y8h = 2U,
    .x9 = 90U, .y9l = 2U, .y9h = 3U,
    .led_supplier = 0x0002U,
    .platform = 0x0002U,
    .intensity_factor = 0x8000U,
    .part_number = { '1','2','3','4','5','6','7','8','9','0','1' },
    .hardware_version = { 'H','0','4' },
    .serial_number =
        { '1','2','3','4','5','6','7','8','9','0',
          '0','9','8','7','6','5','4','3','2','1' },
    .calibration_voltage = { 0U, 0U, 0U },
    .calibration_temperature = 0U,
    .ihr_current_nad = 8U,
    .ihr_auto_nad = 8U
};

static bool NvM_CfgValidateSwsc(const void *DataPtr)
{
    const Rte_SwscConfigType *config =
        (const Rte_SwscConfigType *)DataPtr;
    return ((config != NULL) &&
            (config->magic == NVM_SWSC_CONFIG_MAGIC) &&
            (config->version == NVM_SWSC_CONFIG_VERSION) &&
            (config->nad >= 1U) && (config->nad <= 16U) &&
            (config->ihr_current_nad != 0U) &&
            (config->ihr_auto_nad >= 1U) &&
            (config->ihr_auto_nad <= 16U));
}

static bool NvM_CfgValidateSwscLegacy(const void *DataPtr)
{
    const NvM_SwscConfigLegacyV2Type *config =
        (const NvM_SwscConfigLegacyV2Type *)DataPtr;
    return ((config != NULL) &&
            (config->magic == NVM_SWSC_CONFIG_MAGIC) &&
            ((config->version == NVM_SWSC_CONFIG_LEGACY_VERSION) ||
             (config->version == NVM_SWSC_CONFIG_LEGACY_V3_VERSION)) &&
            (config->nad >= 1U) && (config->nad <= 16U));
}

static void NvM_CfgMigrateSwscLegacy(void *DestinationPtr,
                                     const void *LegacyPtr)
{
    uint16_t index;
    uint16_t copyLength;
    uint8_t *destination = (uint8_t *)DestinationPtr;
    const uint8_t *legacy = (const uint8_t *)LegacyPtr;
    const NvM_SwscConfigLegacyV2Type *legacyHeader =
        (const NvM_SwscConfigLegacyV2Type *)LegacyPtr;
    s_swscConfigRam = s_swscConfigRomDefault;
    copyLength = (legacyHeader->version ==
                  NVM_SWSC_CONFIG_LEGACY_V3_VERSION) ?
                 (uint16_t)sizeof(NvM_SwscConfigLegacyV3Type) :
                 (uint16_t)sizeof(NvM_SwscConfigLegacyV2Type);
    for (index = 0U; index < copyLength; index++)
    {
        destination[index] = legacy[index];
    }
    s_swscConfigRam.magic = NVM_SWSC_CONFIG_MAGIC;
    s_swscConfigRam.version = NVM_SWSC_CONFIG_VERSION;
    s_swscConfigRam.ihr_current_nad = s_swscConfigRam.nad;
    s_swscConfigRam.ihr_auto_nad = s_swscConfigRam.nad;
}

const NvM_BlockDescriptorType NvM_BlockDescriptorTable[] =
{
    {
        NVM_BLOCK_SWSC_CONFIG,
        sizeof(Rte_SwscConfigType),
        sizeof(NvM_SwscConfigLegacyV3Type),
        &s_swscConfigRam,
        &s_swscConfigRomDefault,
        NvM_CfgValidateSwsc,
        NvM_CfgValidateSwscLegacy,
        NvM_CfgMigrateSwscLegacy
    }
};

const uint16_t NvM_BlockDescriptorCount =
    (uint16_t)(sizeof(NvM_BlockDescriptorTable) /
               sizeof(NvM_BlockDescriptorTable[0]));

const Rte_SwscConfigType *NvM_CfgGetSwscConfig(void)
{
    return &s_swscConfigRam;
}
