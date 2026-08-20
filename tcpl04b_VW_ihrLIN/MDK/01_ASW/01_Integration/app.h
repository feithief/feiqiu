#ifndef SWSC_APP_H
#define SWSC_APP_H

#include "prj_board_cfg.h"

/* One place for build-time features. Disabled features must not enter the image. */
#define CFG_SUPPORT_LOG                 (0)
#define CFG_SUPPORT_COMM_LIN            (1)
#define CFG_SUPPORT_LIN_MASTER          (0)
#define CFG_SUPPORT_LIN_SNPD            (0)
#define CFG_SUPPORT_LIN_SNPD_LED        (0)
#define CFG_SUPPORT_LIN_SNPD_TIMEOUT    (0)
#define CFG_SUPPORT_LIN_FID_LOAD        (0)
#define CFG_SUPPORT_LOW_POWER           (0)
#define CFG_SUPPORT_FAULT_DET           (0)
#define CFG_SUPPORT_HW_OTP              (0)
#define CFG_SUPPORT_SW_OTP              (1)
#define CFG_SUPPORT_WDG_EN              (1)
#define CFG_SUPPORT_SINGLE_BIN          (0)
#define CFG_SUPPORT_DFU_MULT            (0)

#define CFG_LED_DRIVING_STEP            (3)

#endif
