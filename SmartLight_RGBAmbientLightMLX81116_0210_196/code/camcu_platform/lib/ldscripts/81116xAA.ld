/*
 * Software Platform
 *
 * Copyright (C) 2017-2018 Melexis N.V.
 * Linker script file
 *
 * Project: MLX81116
 *          Flash :   32 Kbytes
 *          RAM :      2 KBytes
 *          EEPROM : 512 bytes
 */

OUTPUT_FORMAT("elf32-mlx16-fx")
OUTPUT_ARCH(mlx16fx)


/*
 * Default libraries which will be used for linking
 * Any additional libraries could be provided either through command
 * line option or in linker script
 */

/* Standard library */
GROUP(-lc -lgcc)

/* Force searching symbol in libraries if not defined */
EXTERN(__fw_vectors)
EXTERN(__fw_sys_services)

MEMORY
{
    /* EEPROM memory map */
    eeprom_patch     (rw!x) : ORIGIN = 0x0800, LENGTH = 0x0040  /* EEPROM patch area */
    eeprom_custom    (rw!x) : ORIGIN = 0x0840, LENGTH = 0x0170  /* EEPROM customer area */
    eeprom_mlx       (rw!x) : ORIGIN = 0x09B0, LENGTH = 0x0090  /* EEPROM melexis area */
    eeprom_reg       (rw!x) : ORIGIN = 0x0A40, LENGTH = 0x0006  /* EEPROM registers */

    /* MLX16 RAM memory map */
    ram              (rwx)  : ORIGIN = 0x1000, LENGTH = 0x07E0  /* First 256 bytes are dp: */
    bl_ram           (rw)   : ORIGIN = 0x17E0, LENGTH = 0x0018  /* Bootloader's RAM used as a flags/modes storage */
    bist_stat        (rw)   : ORIGIN = 0x17F8, LENGTH = 0x0008  /* Use the latest RAM area for the BIST-flags */

    /* MLX16 ROM memory map */
    rom              (rx)   : ORIGIN = 0x2000, LENGTH = 0x1FFC  /* ROM */
    rom_crc          (rx)   : ORIGIN = 0x3FFC, LENGTH = 0x0004  /* CRC of the ROM */

    /* MLX16 flash memory map */
    flash_control    (rw)   : ORIGIN = 0x5668, LENGTH = 0x0198  /* FLASH shell latches, and other configuration 'RAM'/ROM data */

    /* User flash regions */
    fw_vectors       (rx)   : ORIGIN = 0x5800, LENGTH = 0x01B0  /* Interrupt vector table in flash */
    flash            (rx)   : ORIGIN = 0x59B0, LENGTH = 0x7E34  /* Application flash area */
    fw_product_id    (r)    : ORIGIN = 0xD7E4, LENGTH = 0x0008  /* Product ID */
    fw_app_version   (r)    : ORIGIN = 0xD7EC, LENGTH = 0x0004  /* Application version */
    fw_pltf_version  (r)    : ORIGIN = 0xD7F0, LENGTH = 0x0004  /* Flash platform version */
    fw_prot_key      (r)    : ORIGIN = 0xD7F4, LENGTH = 0x0008  /* FW protection key (64 bits) */
    flash_crc        (r)    : ORIGIN = 0xD7FC, LENGTH = 0x0004  /* Flash CRC area */
}


/* direct, io and extra page addresses */
__dp__ = 0x1000;
__io__ = 0x0000;
__ep__ = 0x0800;

/* far page addresses */
__fp0__ = 0x2000;   /* interrupt tables */
__fp1__ = 0x2100;   /* interrupt tables */
__fp2__ = 0x2200;   /* interrupt tables */
__fp3__ = 0x2300;
__fp4__ = 0x2400;
__fp5__ = 0x2500;
__fp6__ = 0x2600;
__fp7__ = 0x0800;   /* points to nodp RAM */

SECTIONS
{
    .fw_vectors :
    {
        *(.fw_vectors)
    } > fw_vectors

    .fw_sys_services :
    {
        *(.fw_sys_services)
    } > flash

    /* Initialized variables (data) with .dp attribute
     * The initial values are copied by crt from Flash to RAM
     *
     * NOTES: Output section name shoud be '.dp.data' otherwise binutils
     * complain about "Dwarf Error"
     */
    .dp.data :
    {
        . = ALIGN (2);
        __ram_start = . ;
        __data_dp_start = . ;
        *(.dp.data)
        . = ALIGN (2);
        __data_dp_end = . ;
    } > ram AT > flash
    __data_dp_size = __data_dp_end - __data_dp_start;
    __data_load_start = LOADADDR(.dp.data);

    /* Uninitialized variables (bss) with .dp attribute
     * This memory area is zeroed by crt
     *
     * NOTES: Output section name shoud be '.dp' otherwise binutils
     * complain about "Dwarf Error"
     */
    .dp :
    {
        . = ALIGN (2);
        __bss_dp_start = .;
        *(.dp)
        . = ALIGN (2);
        __bss_dp_end = . ;
    } > ram
    __bss_dp_size = __bss_dp_end - __bss_dp_start;

    /* Global initialized variables with nodp attribute
     * The initial values are copied by crt from Flash
     */
    .data :
    {
        . = ALIGN (2);
        __data_start = . ;
        *(.ramfunc)
        *(.data)
        *(.data.*)
        . = ALIGN (2);
        __data_end = . ;
    } > ram AT > flash
    __data_size = __data_end - __data_start ;

    /* Global variables with nodp attribute
     * This memory area is zeroed by crt
     */
    .bss :
    {
        . = ALIGN (2);
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN (2);
        __bss_end = . ;
    } > ram
    __bss_size = __bss_end - __bss_start;

    /* Global data not cleared after reset  */
    .noinit (NOLOAD) :
    {
        . = ALIGN (2);
        __noinit_start = .;
        *(.noinit)
        . = ALIGN (2);
        __noinit_end = .;
    } > ram
    __noinit_size = __noinit_end - __noinit_start;

    /* The stack starts immediately after the global variables */
    PROVIDE (_fw_stack = .);

    .bist_stat (NOLOAD):
    {
        *(.bist_stat)
    } > bist_stat

    .flash_control (NOLOAD):
    {
        *(.flash_control)
    } > flash_control

    .eeprom_led_data :
    {
        . = ALIGN (2);
        __ledsect_start = . ;

        *(.led_data.CG_OFFSET)
        *(.led_data.CG_SLOPE)
        *(.led_data.B_OFFSET)
        *(.led_data.M_SLOPE)
        . = ALIGN (2);
        __ledsect_end = . ;
    } > eeprom_custom AT > flash

    __ledsect_size = __ledsect_end - __ledsect_start;
    __ledsect_load_start = LOADADDR(.eeprom_led_data);

    /* This is the starting address of the heap.
     * It is located at the end of RAM and grows downward to the end of stack.
     * In a typical embedded application the heap is not used, but this
     * symbol has to be defined since some library functions need it.
     */
    PROVIDE (__ram_end = __ram_start + 0x800);  /* for 2K RAM */

    .flash_data :
    {
        *(.rodata)          /* mostly jumptables */
        *(.rodata.*)
        *(.rodata._mlx_*)   /* Flash arrays */

        . = ALIGN (2);
        *(.led_data.WHITEPOINT)
        *(.led_data.INVPOINT)
        *(.led_data.BRIGHTNESSTAB)
        *(.led_data.RGBTOXYZ)
        *(.led_data.BLUETEMP)
        *(.led_data.GREENTEMP)
        *(.led_data.REDTEMP)
        *(.led_data.XYZTORGB)
        *(.led_data.MAXINTENSITY)
        *(.led_data.SCALERGB)
        *(.led_data.LIMITMAXOUTPUT)

        . = ALIGN (0x2);
        *(.text)
        *(.text.*)

        . = ALIGN (2);
        _etext = .;

        /*
         * this should be last linking to flash
         * the rest of the flash will be used to store load data
         * (see "AT > flash" below)
         */
    } > flash

    .flash_fill _etext :
    {
        . = ALIGN (2);
        FILL(0xFF07);       /* Fill not used Flash with jump to myself (swapped as used as pattern) */
        . = ALIGN(ABSOLUTE(ORIGIN(flash)) + LENGTH(flash) - 2);
        SHORT(0x07FF);      /* There should be something at the Flash section end */
    } > flash

    .fw_product_id :
    {
        __fw_product_id_start = . ;
        KEEP(*(.fw_product_id))
    } > fw_product_id

    .fw_app_version :
    {
        __fw_app_version_start = . ;
        KEEP(*(.fw_app_version))
    } > fw_app_version

    .fw_pltf_version :
    {
        __fw_pltf_version_start = . ;
        KEEP(*(.fw_pltf_version))
    } > fw_pltf_version

    .fw_prot_key :
    {
        __fw_prot_key_start = . ;
        KEEP(*(.fw_prot_key))
    } > fw_prot_key

    .flash_crc :
    {
        KEEP(*(.FL_SIGN))
        LONG(FL_SIGN);
    } > flash_crc

    /* Global variables in extra page with no values specified. */
    .ep :
    {
        . = ALIGN (2);
        __bss_ep_start = .;
        *(.ep)
        . = ALIGN (2);
        __bss_ep_end = . ;
    } > eeprom_custom

    .ep.data :
    {
        . = ALIGN (2);
        __data_ep_start = . ;
        *(.ep.data)
        . = ALIGN (2);
        __data_ep_end = . ;
    } > eeprom_custom

    .eeprom_patch (NOLOAD):
    {
        *(.eeprom_patch)
    } > eeprom_patch

    .eeprom_mlx (NOLOAD):
    {
        *(.eeprom_cs)
    } > eeprom_mlx

    .eeprom_reg (NOLOAD):
    {
        *(.eeprom_reg)
    } > eeprom_reg
}

/* EOF */
