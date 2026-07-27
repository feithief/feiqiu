/**
 * @file
 * @brief Memory map description
 * @internal
 *
 * @copyright (C) 2015-2017 Melexis N.V.
 * git 5f2794fe
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
#define  MEM_RAM_SIZE               (0x0800)

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


#endif /* MEMORY_MAP_H */
