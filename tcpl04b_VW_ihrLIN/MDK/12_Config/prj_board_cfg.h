#ifndef SWSC_PRJ_BOARD_CFG_H
#define SWSC_PRJ_BOARD_CFG_H

#define BOARD_DEMO_EVK                  (1)
#define BOARD_DEMO_TYPE                 BOARD_DEMO_EVK

/* Bootloader/application hand-off word defined by the TCPL04B SDK layout. */
#define BOOT_APP_VECTOR_WORD_ADDR       (0x00001E1CUL)

/* TCPL04B final 1 KB is reserved by both scatter files as UserData. */
#define SWSC_CONFIG_FLASH_ADDRESS       (0x0000BC00UL)

#include "func_cfg_evk.h"

#endif
