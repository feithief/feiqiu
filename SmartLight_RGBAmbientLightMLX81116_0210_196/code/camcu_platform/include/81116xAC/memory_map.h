/**
 * @file
 * @brief Memory map description
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 6d04ab96
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
 * @ingroup CAMCU_platform
 *
 * @details
 */

#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#ifndef UNITTEST
/* MLX82100 Chip FLASH START definition, according the mlx82100, spec. v.1.19 */
#ifndef MEM_FLASH_START
#define MEM_FLASH_START             (0x5800)
#define MEM_FLASH_SIZE              (0x8000)
#define MEM_FLASH_PAGE_SIZE         (0x0080)
#endif


#define MEM_FLASH_CS_START          (0x5700)
#define MEM_FLASH_CS_SIZE           (0x0080)

#ifndef  MEM_ROM_START
#define  MEM_ROM_START              (0x2000)
#define  MEM_ROM_SIZE               (0x2000)
#endif

#ifndef  MEM_EEPROM_START
#define  MEM_EEPROM_START           (0x0800)
#ifdef SHIFT_EE_CS_DATA /* SHIFTING DATA workaround. Uses the shift of CS EEPROM into the main-area to avoid EEPROM CS accessing */
#define  MEM_EEPROM_CS_START        (0x0980)
#warning "EEPROM CS DATA is shifted to the MA area!!!"
#else
#define  MEM_EEPROM_CS_START        (0x0A00)
#endif /* SHIFT_EE_CS_DATA */
#define  MEM_MAIN_EEPROM_SIZE       (0x0200)
#define  MEM_EEPROM_SIZE            (0x0240)
#define  MEM_EEPROM_CS_LAST_PAGE    (MEM_EEPROM_START + MEM_EEPROM_SIZE - 8u)
#endif

#define  MEM_EEPROM_PAGE_SIZE       (0x0008)

#ifndef  MEM_RAM_START
#define  MEM_RAM_START              (0x1000)
#define  MEM_RAM_SIZE               (0x0800)
#endif

#else

#define  MEM_EEPROM_START           (0x0800)
#define  MEM_EEPROM_CS_START        (0x0A00)
#define  MEM_MAIN_EEPROM_SIZE       (0x0200)
#define  MEM_EEPROM_SIZE            (0x0240)
#define  MEM_EEPROM_CS_LAST_PAGE    (MEM_EEPROM_START + MEM_EEPROM_SIZE - 8u)

#define MEM_FLASH_CS_START          (0x3700)
#define MEM_FLASH_CS_SIZE           (0x0080)

#define MEM_FLASH_START             (0x3800)
#define MEM_FLASH_SIZE              (0x0800)
#define MEM_FLASH_PAGE_SIZE         (0x0080)

#define  MEM_RAM_START              (0x1000)
#define  MEM_RAM_SIZE               (0x0C00)

#define  MEM_ROM_START              (0x2000)
#define  MEM_ROM_SIZE               (0x2000)
#endif /* UNITTEST */

#define SIGN_OFFSET                 (0x0028) /* Offset inside the FLASH is used to measure the FW signature */


/* MLX4 memories map definition */
#ifndef  MEM_COLIN_RAM_START
#define  MEM_COLIN_RAM_START        (0x0E00)
#define  MEM_COLIN_RAM_SIZE         (0x0200)
#endif

#ifndef  MEM_COLIN_ROM_START
#define  MEM_COLIN_ROM_START        (0x15800)
#define  MEM_COLIN_ROM_SIZE         (0x02000)
#endif

/* FLASH shell definition */
#ifndef  MEM_FLASH_LATCH_START
#define  MEM_FLASH_LATCH_START      (0x6070)
#define  MEM_FLASH_LATCH_SIZE       (0x0080)
#endif

/* MLX81130 Custom EEPROM MA definitions */
#define APP_TRIM35              (*(uint16_t *)(MEM_EEPROM_START + 0x01F6u)) /* Source for HC24; trim13_hc;         default 0x0000 */
#define APP_TRIM34              (*(uint16_t *)(MEM_EEPROM_START + 0x01F4u)) /* Source for HC22, HC23; trim12_hc;   default 0x0000 */
#define APP_TRIM33              (*(uint16_t *)(MEM_EEPROM_START + 0x01F2u)) /* Source for HC20, HC21; trim11_hc;   default 0x0000 */
#define APP_TRIM32              (*(uint16_t *)(MEM_EEPROM_START + 0x01F0u)) /* Source for HC18, HC19; trim10_hc;   default 0x0000 */
#define APP_TRIM31              (*(uint16_t *)(MEM_EEPROM_START + 0x01EEu)) /* Source for HC16, HC17; trim9_hc;    default 0x0000 */
#define APP_TRIM30              (*(uint16_t *)(MEM_EEPROM_START + 0x01ECu)) /* Source for HC14, HC15; trim8_hc;    default 0x0000 */
#define APP_TRIM29              (*(uint16_t *)(MEM_EEPROM_START + 0x01EAu)) /* Source for HC12, HC13; trim7_hc;    default 0x0000 */
#define APP_TRIM28              (*(uint16_t *)(MEM_EEPROM_START + 0x01E8u)) /* Source for HC10, HC11; trim6_hc;    default 0x0000 */
#define APP_TRIM27              (*(uint16_t *)(MEM_EEPROM_START + 0x01E6u)) /* Source for HC8, HC9; trim5_hc;  default 0x0000 */
#define APP_TRIM26              (*(uint16_t *)(MEM_EEPROM_START + 0x01E4u)) /* Source for HC6, HC7; trim4_hc;  default 0x0000 */
#define APP_TRIM25              (*(uint16_t *)(MEM_EEPROM_START + 0x01E2u)) /* Source for HC4, HC5; trim3_hc;  default 0x0000 */
#define APP_TRIM24              (*(uint16_t *)(MEM_EEPROM_START + 0x01E0u)) /* Source for HC2, HC3; trim2_hc;  default 0x0000 */
#define APP_TRIM23              (*(uint16_t *)(MEM_EEPROM_START + 0x01DEu)) /* Source for HC0, HC1; trim1_hc;  default 0x0000 */
#define APP_TRIM22              (*(uint16_t *)(MEM_EEPROM_START + 0x01DCu)) /* Source for TRIM_MISC;           default 0x0000 */
#define APP_TRIM21              (*(uint16_t *)(MEM_EEPROM_START + 0x01DAu)) /* Source for melibus_ctrl;        default 0x0500 */
#define APP_TRIM20              (*(uint16_t *)(MEM_EEPROM_START + 0x01D8u)) /* unused; default 0x0000 */
#define APP_TRIM19              (*(uint16_t *)(MEM_EEPROM_START + 0x01D6u)) /* unused; default 0x0000 */
#define APP_TRIM18              (*(uint16_t *)(MEM_EEPROM_START + 0x01D4u)) /* unused; default 0x0000 */


#endif /* MEMORY_MAP_H */
