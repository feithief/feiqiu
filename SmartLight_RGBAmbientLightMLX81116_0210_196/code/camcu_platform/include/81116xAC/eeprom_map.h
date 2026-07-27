/**
 * @file
 * @brief CAMCU EEPROM map description concerning the startup code and the common parts of applications.
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git flash 137e5489
 *
 * Melexis N.V. is supplying this code for use with Melexis N.V. processor based microcontrollers only.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.  MELEXIS N.V. SHALL NOT IN ANY CIRCUMSTANCES,
 * BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * @endinternal
 *
 * @ingroup CAMCU_library
 *
 * @details
 */

#ifndef EEPROM_MAP_H
#define EEPROM_MAP_H

#include <stdint.h>
#include "memory_map.h"
#include <syslib.h>
#include "lib_miscio.h"

/* PATCHes area definition */
/** The PATCH area start offset */
#define EE_PATCH_AREA_START            (MEM_EEPROM_START + 0x00)                       /* Cell 0 in EEPROM map excel sheet */
/** The PATCH version offset */
#define EE_PATCH_VERSION               (*(uint16_t *)(MEM_EEPROM_START + 0x02))        /* Cell 1 in EEPROM map excel sheet, patch area size in words */
/** The PATCH area size offset */
#define EE_PATCH_AREA_SIZE             ((uint8_t)((*(uint16_t *)(MEM_EEPROM_START + 0x00))>>8))  /* Cell 2 in EEPROM map excel sheet. EEPROM access is 16bit, the addr 0x01 is used */
/** The PATCH area start data to copy offset */
#define EE_PATCH_DATA_TO_COPY          (MEM_EEPROM_START + 0x04)                       /* Cell 3 in EEPROM map excel sheet */

/** Sets the patch data structure in EEPROM */
typedef struct {
    uint16_t address;   /**< patch address has [15:0] address format (addr[19:16]=0). The least bit is used to enable/disable the patch. */
    uint16_t data;      /**< patch instruction */
} PatchContent_t;

/** PATCHes area maximum size in words */
#define EE_PATCH_MAX_AREA_SIZE  0x20
/** PATCHes quantity */
#define PATCH_COUNT     3

/* Adding this since it was changed in 48b338f718c72053a505ab033f1f3e6ce36452c3 */
#ifndef MEM_EEPROM_CS_START
#define MEM_EEPROM_CS_START MEM_EEPROM_START
#endif

#ifndef MEM_EEPROM_APP_TRIM_START
#define MEM_EEPROM_APP_TRIM_START (MEM_EEPROM_START + 0x01B0)
#endif

/* TRIMMING VALUES AREA DEFINITION */
#define EE_TRIM_AREA_START             (MEM_EEPROM_CS_START + 0x000U)                       /* Cell 256 in EEPROM map excel sheet */
#define EE_TRIM_AREA_END               (MEM_EEPROM_CS_START + 0x038U)                       /* Cell 284 in EEPROM map excel sheet */
#define EE_TRIM_AREA_SIZE              (EE_TRIM_AREA_END - EE_TRIM_AREA_START)              /* -- Calculated size -- */

#define EE_APP_TRIM_AREA_START         (MEM_EEPROM_APP_TRIM_START + 0x000U)
#define EE_APP_TRIM_AREA_END           (MEM_EEPROM_APP_TRIM_START + 0x050U)
#define EE_APP_TRIM_AREA_SIZE          (EE_APP_TRIM_AREA_END - EE_APP_TRIM_AREA_START)      /* -- Calculated size -- */

/* FLASH and EEPROM shell definitions in EEPROM area */

/* Enables the EEPROM-mapped EEPROM memory shell settings */
#define EEP_USE_DIRECT_SETTINGS

#define EE_ROM_CONFIGURATION           (*(RomConfiguration_t *)(MEM_EEPROM_CS_START + 0x001))   /**< Holds the project and HW-specific settings being used in the ROM or FLASH */
#define EE_MS_TRIM0_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x002))  /**< For platform (bandgap, bias) */
#define EE_MS_TRIM1_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x004))  /**< For platform (vddd, vdda) */
#define EE_MS_TRIM2_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x006))  /**< For platform (1MHz RCO (150d), LIN) */
#define EE_MS_TRIM3_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x008))  /**< For platform (ADC refs 1+2) */
#define EE_MS_TRIM4_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x00A))  /**< For platform (ADC ref 3) */
#define EE_MS_TRIM5_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x00C))  /**< For platform (second regulator vddio) */
#define EE_MS_TRIM6_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x00E))  /**< For platform (speed1 for fast RCO + EE/Fl wait states - 24/12 MHz (0d530=0x212) */
#define EE_MS_TRIM7_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x010))  /**< For platform (speed2 for fast RCO + EE/Fl wait states - 28/14 MHz (0d634=0x27A)) */
#define EE_MS_TRIM8_VALUE              (*(uint16_t *)(MEM_EEPROM_CS_START + 0x012))  /**< For platform (speed3 for fast RCO + EE/Fl wait states - 32/16 MHz (0d761=0x2F9)) */

#ifdef EEP_USE_DIRECT_SETTINGS

#define EE_FL_R2_EE                    (*(volatile uint16_t *)(MEM_EEPROM_CS_START + 0x014))  /**< R2_EE (default: 0x4000) */
#define EE_FL_R1_EE                    (*(volatile uint16_t *)(MEM_EEPROM_CS_START + 0x016))  /**< R1_EE (default: 0x5366) */

#endif /* EEP_USE_DIRECT_SETTINGS */

/* Enables the EEPROM-mapped FLASH memory shell settings */
#define FL_USE_DIRECT_SETTINGS

#ifdef FL_USE_DIRECT_SETTINGS

#define EE_FL_R3_FL                    (*(uint16_t *)(MEM_EEPROM_CS_START + 0x018))  /**< R3_FL (default: 0x6744) */
#define EE_FL_R2_FL                    (*(uint16_t *)(MEM_EEPROM_CS_START + 0x01A))  /**< R2_FL (default: 0x0010) */
#define EE_FL_R1_FL                    (*(uint16_t *)(MEM_EEPROM_CS_START + 0x01C))  /**< R1_FL (default: 0x0188) */

#define EE_FL_TIM_EE                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x01E))  /**< EEPROM read timing port */
#define EE_FL_PROG_EE                  (*(uint16_t *)(MEM_EEPROM_CS_START + 0x020))  /**< EEPROM program cycle port */
#define EE_FL_ERA_EE                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x022))  /**< EEPROM erase timing */
#define EE_FL_WR_EE                    (*(uint16_t *)(MEM_EEPROM_CS_START + 0x024))  /**< EEPROM write timing */

#define EE_FL_CFG_FL                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x026))  /**< NV_TRIM9.  Flash read configuration word */
#define EE_FL_ER_WR_FL                 (*(uint16_t *)(MEM_EEPROM_CS_START + 0x028))  /**< NV_TRIM10. Flash erase/write timing */
#define EE_TIM_PORT2                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x02A))  /**< NV_TRIM11. Timing port 2 */
#define EE_TIM_PORT1                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x02C))  /**< NV_TRIM12. Timing port 1 */
#define EE_NV_MEM_OPEN                 (*(uint16_t *)(MEM_EEPROM_CS_START + 0x02E))  /**< NV_TRIM13. NV memory open */

#endif /* FL_USE_DIRECT_SETTINGS */

/* Chip revision definition */
#define EE_CHIP_ID_0                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x030))         /**< IC ID0 */
#define EE_CHIP_ID_1                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x032))         /**< IC ID1 */
#define EE_CHIP_ID_2                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x034))         /**< IC ID2 */
#define EE_CHIP_ID_3                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x036))         /**< IC ID3 used as a PRODUCT ID cell */

/* Test chip revision definition */
#define EE_MLX_CHIP_ID_0                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x038))         /**< IC TEST ID0, Test-mode accessible */
#define EE_MLX_CHIP_ID_1                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x03A))         /**< IC TEST ID1, Test-mode accessible */
#define EE_MLX_CHIP_ID_2                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x03C))         /**< IC TEST ID2, Test-mode accessible */
#define EE_MLX_CHIP_ID_3                   (*(uint16_t *)(MEM_EEPROM_CS_START + 0x03E))         /**< IC TEST ID3, Test-mode accessible */

/** Trimming + Waitstates description/configuration part structure */
typedef struct __attribute__((packed)) {
    uint16_t rco_trim : 10;         /**< represents [9:0] of 16bit value. The RCO32M trimming value for the frequency expected */
    uint16_t ee_wait : 3;           /**< represents [12:10] of 16bit value. EE_WAITSTATES setting, includes the AC_SEL dependent option */
    uint16_t fl_wait : 3;           /**< represents [15:13] of 16bit value. FL_WAITSTATES setting, includes the AC_SEL dependent option */
} waitstate_cfg_t;

#define EE_CLK_TRIMS_4                (*(waitstate_cfg_t *)(MEM_EEPROM_START + 0x1F8))  /**<  placeholder for speed setting 4 */
#define EE_CLK_TRIMS_5                (*(waitstate_cfg_t *)(MEM_EEPROM_START + 0x1FA))  /**<  placeholder for speed setting 5 */
#define EE_CLK_TRIMS_6                (*(waitstate_cfg_t *)(MEM_EEPROM_START + 0x1FC))  /**<  placeholder for speed setting 6 */
#define EE_CLK_TRIMS_7                (*(waitstate_cfg_t *)(MEM_EEPROM_START + 0x1FE))  /**<  placeholder for speed setting 7 */

/** Application trimming area. Includes common (named) parts and custom project-specific cells */
#define EE_APP_CRC_TEMP_MID        (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0000u)) /**< CAMCU, {CRC[7:0], TempMid[15:8]}CRC over address EE_APP_TRIM1-EE_APP_TRIM39 ( sum with carry of the 16 bit data), Temp_MID copy of 0x09B6 */
#define EE_APP_TRIM1               (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0002u)) /**< CAMCU, reserved */
#define EE_APP_OTEMP_CAL           (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0004u)) /**< CAMCU, OTempCal_EE */
#define EE_APP_TEMP_MID            (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0006u)) /**< CAMCU, {TempMid[7:0], unused[15:8]}Temp_MID copy of 0x09B1 */
#define EE_APP_GAIN_TEMP_CAL       (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0008u)) /**< CAMCU, {GainTempLowCal[7:0], GainTempHighCal[15:8]} */
#define EE_APP_ADC_REF_OFFSET      (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x000Au)) /**< CAMCU, {ADC ref offset[7:0], unused[15:8]} */
#define EE_APP_ADC_GAIN            (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x000Cu)) /**< CAMCU, {ADC gain low[7:0], ADC gain high[15:8]} */
#define EE_APP_CLOCK_CAL_1         (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x000Eu)) /**< CAMCU, {SClock24LowCal _EE[7:0], SClock24HighCal _EE[15:8]} */
#define EE_APP_CLOCK_CAL_2         (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0010u)) /**< CAMCU, {OClock24Cal _EE[7:0], SClock28LowCal _EE[15:8]} */
#define EE_APP_CLOCK_CAL_3         (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0012u)) /**< CAMCU, {SClock28HighCal _EE[7:0], OClock28Cal _EE[15:8]} */
#define EE_APP_CLOCK_CAL_4         (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0014u)) /**< CAMCU, {SClock32LowCal _EE[7:0], SClock32HighCal _EE[15:8]} */
#define EE_APP_CLOCK_CAL_5         (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0016u)) /**< CAMCU, {OClock32Cal _EE[7:0], 10KHz cal[15:8]} */
#define EE_APP_TRIM12              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0018u)) /**< CAMCU, reserved */
#define EE_APP_TRIM13              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x001Au)) /**< CAMCU, reserved */
#define EE_APP_SAADMCM             (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x001Cu)) /**< CAMCU, {LINAA- SAADMCM} */
#define EE_APP_GAINAAADMCAL        (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x001Eu)) /**< CAMCU, {LINAA- GAINAADMCAL}*/
#define EE_APP_IAA_TRIM_112_205    (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0020u)) /**< CAMCU, {IAA-TRIM112mA_TRIM_MISC[5:0], IAA-TRIM112mA_PORT_LINAA2[7:6],IAA-TRIM205mA_TRIM_MISC[13:8], IAA-TRIM205mA_PORT_LINAA2[15:14]} */
#define EE_APP_LINAA_GAIN_DIV      (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0022u)) /**< CAMCU, {IAA-TRIM462mA_TRIM_MISC[5:0], IAA-TRIM462mA_PORT_LINAA2[7:6], LINAA_GAIN[11:8], LIN_AA_DIV[15:12]} */

/** Project specific settings */
#define EE_APP_TRIM18              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0024u)) /**< project specific */
#define EE_APP_TRIM19              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0026u)) /**< project specific */
#define EE_APP_TRIM20              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0028u)) /**< project specific */
#define EE_APP_TRIM21              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x002Au)) /**< project specific */
#define EE_APP_TRIM22              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x002Cu)) /**< project specific */
#define EE_APP_TRIM23              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x002Eu)) /**< project specific */
#define EE_APP_TRIM24              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0030u)) /**< project specific */
#define EE_APP_TRIM25              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0032u)) /**< project specific */
#define EE_APP_TRIM26              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0034u)) /**< project specific */
#define EE_APP_TRIM27              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0036u)) /**< project specific */
#define EE_APP_TRIM28              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0038u)) /**< project specific */
#define EE_APP_TRIM29              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x003Au)) /**< project specific */
#define EE_APP_TRIM30              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x003Cu)) /**< project specific */
#define EE_APP_TRIM31              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x003Eu)) /**< project specific */
#define EE_APP_TRIM32              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0040u)) /**< project specific */
#define EE_APP_TRIM33              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0042u)) /**< project specific */
#define EE_APP_TRIM34              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0044u)) /**< project specific */
#define EE_APP_TRIM35              (*(uint16_t *)(MEM_EEPROM_APP_TRIM_START + 0x0046u)) /**< project specific */

#define EE_APP_TRIM36              ((uint16_t)EE_CLK_TRIMS_4)                           /**< CAMCU, reserved for additional speed settings */
#define EE_APP_TRIM37              ((uint16_t)EE_CLK_TRIMS_5)                           /**< CAMCU, reserved for additional speed settings */
#define EE_APP_TRIM38              ((uint16_t)EE_CLK_TRIMS_6)                           /**< CAMCU, reserved for additional speed settings */
#define EE_APP_TRIM39              ((uint16_t)EE_CLK_TRIMS_7)                           /**< CAMCU, reserved for additional speed settings */

#endif /* EEPROM_MAP_H */
