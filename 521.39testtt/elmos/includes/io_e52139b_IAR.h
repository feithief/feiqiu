
#ifndef __IO_E52139B_H
#define __IO_E52139B_H

#ifdef __IAR_SYSTEMS_ICC__
  #include "io_e52139b_macros.h"

  #ifndef _SYSTEM_BUILD
    #pragma system_include
  #endif
  #pragma language=save
  #pragma language=extended

  #ifdef __ICCARM__
    #if __LITTLE_ENDIAN__ == 0
      #error This file should only be compiled in little endian mode
    #endif
  #endif
#endif

/* ============================================================================== */
/* VIC module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* TABLE_BASE register ... */

/* TABLE_BASE bitfield type ... */

typedef struct {
  __REG16 base : 16;       /* [15:0] */
} vic_table_base_bf_t;

/* TABLE_BASE address mapping ... */

__IO_REG16_BIT(VIC_TABLE_BASE, 0x00000040u, __READ_WRITE, vic_table_base_bf_t);

/* ------------------------------------------------------------------------------ */
/* TABLE_TYPE register ... */

/* TABLE_TYPE bitfield type ... */

typedef struct {
  __REG16 type :  1;       /* [0] */
  __REG16      : 15;
} vic_table_type_bf_t;

/* TABLE_TYPE single bit enum ... */

enum {
  E_VIC_TABLE_TYPE_TYPE = 0x0001u
};

/* TABLE_TYPE address mapping ... */

__IO_REG16_BIT(VIC_TABLE_TYPE, 0x00000042u, __READ_WRITE, vic_table_type_bf_t);

/* ------------------------------------------------------------------------------ */
/* MAIN_ENABLE register ... */

/* MAIN_ENABLE bitfield type ... */

typedef struct {
  __REG16 enable :  1;       /* [0] */
  __REG16        : 15;
} vic_main_enable_bf_t;

/* MAIN_ENABLE single bit enum ... */

enum {
  E_VIC_MAIN_ENABLE_ENABLE = 0x0001u
};

/* MAIN_ENABLE address mapping ... */

__IO_REG16_BIT(VIC_MAIN_ENABLE, 0x00000044u, __READ_WRITE, vic_main_enable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 sys_state   :  1;       /* [0] */
  __REG16 divider     :  1;       /* [1] */
  __REG16 adc_ctrl    :  1;       /* [2] */
  __REG16 pwm         :  1;       /* [3] */
  __REG16 sci         :  1;       /* [4] */
  __REG16 swtimer     :  1;       /* [5] */
  __REG16 gpio        :  1;       /* [6] */
  __REG16 lin_ctrl    :  1;       /* [7] */
  __REG16 eeprom_ctrl :  1;       /* [8] */
  __REG16             :  7;
} vic_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_VIC_IRQ_STATUS_SYS_STATE   = 0x0001u,
  E_VIC_IRQ_STATUS_DIVIDER     = 0x0002u,
  E_VIC_IRQ_STATUS_ADC_CTRL    = 0x0004u,
  E_VIC_IRQ_STATUS_PWM         = 0x0008u,
  E_VIC_IRQ_STATUS_SCI         = 0x0010u,
  E_VIC_IRQ_STATUS_SWTIMER     = 0x0020u,
  E_VIC_IRQ_STATUS_GPIO        = 0x0040u,
  E_VIC_IRQ_STATUS_LIN_CTRL    = 0x0080u,
  E_VIC_IRQ_STATUS_EEPROM_CTRL = 0x0100u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(VIC_IRQ_STATUS, 0x00000070u, __READ_WRITE, vic_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask :  9;       /* [8:0] */
  __REG16      :  7;
} vic_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(VIC_IRQ_MASK, 0x00000074u, __READ_WRITE, vic_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} vic_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(VIC_IRQ_VENABLE, 0x00000078u, __WRITE, vic_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} vic_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(VIC_IRQ_VDISABLE, 0x0000007Au, __WRITE, vic_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} vic_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(VIC_IRQ_VMAX, 0x0000007Cu, __READ_WRITE, vic_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} vic_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(VIC_IRQ_VNO, 0x0000007Eu, __READ_WRITE, vic_irq_vno_bf_t);

/* ============================================================================== */
/* WDOG module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* CONTROL register ... */

/* CONTROL bitfield type ... */

typedef struct {
  __REG16 run_enable :  1;       /* [0] */
  __REG16 restart    :  1;       /* [1] */
  __REG16            :  6;
  __REG16 password   :  8;       /* [15:8] */
} wdog_control_bf_t;

/* CONTROL single bit enum ... */

enum {
  E_WDOG_CONTROL_RUN_ENABLE = 0x0001u,
  E_WDOG_CONTROL_RESTART    = 0x0002u
};

/* CONTROL address mapping ... */

__IO_REG16_BIT(WDOG_CONTROL, 0x00000080u, __READ_WRITE, wdog_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* WINDOW register ... */

/* WINDOW bitfield type ... */

typedef struct {
  __REG16 size   :  4;       /* [3:0] */
  __REG16 enable :  1;       /* [4] */
  __REG16        : 11;
} wdog_window_bf_t;

/* WINDOW single bit enum ... */

enum {
  E_WDOG_WINDOW_ENABLE = 0x0010u
};

/* WINDOW address mapping ... */

__IO_REG16_BIT(WDOG_WINDOW, 0x00000082u, __READ_WRITE, wdog_window_bf_t);

/* ------------------------------------------------------------------------------ */
/* PRESCALER register ... */

/* PRESCALER bitfield type ... */

typedef struct {
  __REG16 val :  8;       /* [7:0] */
  __REG16     :  8;
} wdog_prescaler_bf_t;

/* PRESCALER address mapping ... */

__IO_REG16_BIT(WDOG_PRESCALER, 0x00000084u, __READ_WRITE, wdog_prescaler_bf_t);

/* ------------------------------------------------------------------------------ */
/* RELOAD register ... */

/* RELOAD bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} wdog_reload_bf_t;

/* RELOAD address mapping ... */

__IO_REG16_BIT(WDOG_RELOAD, 0x00000086u, __READ_WRITE, wdog_reload_bf_t);

/* ------------------------------------------------------------------------------ */
/* COUNTER register ... */

/* COUNTER bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} wdog_counter_bf_t;

/* COUNTER address mapping ... */

__IO_REG16_BIT(WDOG_COUNTER, 0x00000088u, __READ, wdog_counter_bf_t);

/* ============================================================================== */
/* SWTIMER module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* CONFIG register ... */

/* CONFIG bitfield type ... */

typedef struct {
  __REG16 enables          :  2;       /* [1:0] */
  __REG16 cnt_auto_reloads :  2;       /* [3:2] */
  __REG16                  : 12;
} swtimer_config_bf_t;

/* CONFIG address mapping ... */

__IO_REG16_BIT(SWTIMER_CONFIG, 0x000000C0u, __READ_WRITE, swtimer_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* COMMAND register ... */

/* COMMAND bitfield type ... */

typedef struct {
  __REG16 cnt_reload_cmd :  2;       /* [1:0] */
  __REG16 div_reload_cmd :  2;       /* [3:2] */
  __REG16                : 12;
} swtimer_command_bf_t;

/* COMMAND address mapping ... */

__IO_REG16_BIT(SWTIMER_COMMAND, 0x000000C2u, __WRITE, swtimer_command_bf_t);

/* ------------------------------------------------------------------------------ */
/* CNT_RELOAD registers ... */

/* CNT_RELOAD bitfield type ... */

typedef struct {
  __REG16 cnt_reload : 16;       /* [15:0] */
} swtimer_cnt_reload_bf_t;

/* CNT_RELOAD address mapping ... */

__IO_REG16_BIT(SWTIMER_CNT0_RELOAD, 0x000000C4u, __READ_WRITE, swtimer_cnt_reload_bf_t);
__IO_REG16_BIT(SWTIMER_CNT1_RELOAD, 0x000000C6u, __READ_WRITE, swtimer_cnt_reload_bf_t);

/* ------------------------------------------------------------------------------ */
/* DIV_RELOAD registers ... */

/* DIV_RELOAD bitfield type ... */

typedef struct {
  __REG16 div_reload :  8;       /* [7:0] */
  __REG16            :  8;
} swtimer_div_reload_bf_t;

/* DIV_RELOAD address mapping ... */

__IO_REG16_BIT(SWTIMER_DIV0_RELOAD, 0x000000C8u, __READ_WRITE, swtimer_div_reload_bf_t);
__IO_REG16_BIT(SWTIMER_DIV1_RELOAD, 0x000000CAu, __READ_WRITE, swtimer_div_reload_bf_t);

/* ------------------------------------------------------------------------------ */
/* CNT_VALUE registers ... */

/* CNT_VALUE bitfield type ... */

typedef struct {
  __REG16 cnt_val : 16;       /* [15:0] */
} swtimer_cnt_value_bf_t;

/* CNT_VALUE address mapping ... */

__IO_REG16_BIT(SWTIMER_CNT0_VALUE, 0x000000CCu, __READ, swtimer_cnt_value_bf_t);
__IO_REG16_BIT(SWTIMER_CNT1_VALUE, 0x000000CEu, __READ, swtimer_cnt_value_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 evt_cnt_zero :  2;       /* [1:0] */
  __REG16 evt_div_zero :  2;       /* [3:2] */
  __REG16              : 12;
} swtimer_irq_status_bf_t;

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_STATUS, 0x000000F0u, __READ_WRITE, swtimer_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask :  4;       /* [3:0] */
  __REG16      : 12;
} swtimer_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_MASK, 0x000000F4u, __READ_WRITE, swtimer_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  2;       /* [1:0] */
  __REG16     : 14;
} swtimer_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_VENABLE, 0x000000F8u, __WRITE, swtimer_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  2;       /* [1:0] */
  __REG16     : 14;
} swtimer_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_VDISABLE, 0x000000FAu, __WRITE, swtimer_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  3;       /* [2:0] */
  __REG16      : 13;
} swtimer_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_VMAX, 0x000000FCu, __READ_WRITE, swtimer_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  3;       /* [2:0] */
  __REG16     : 13;
} swtimer_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(SWTIMER_IRQ_VNO, 0x000000FEu, __READ_WRITE, swtimer_irq_vno_bf_t);

/* ============================================================================== */
/* H430_MUL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* LAST_MODE register ... */

/* LAST_MODE bitfield type ... */

typedef struct {
  __REG16 last_mode :  2;       /* [1:0] */
  __REG16           : 14;
} h430_mul_last_mode_bf_t;

/* LAST_MODE address mapping ... */

__IO_REG16_BIT(H430_MUL_LAST_MODE, 0x00000100u, __READ, h430_mul_last_mode_bf_t);

/* ------------------------------------------------------------------------------ */
/* MPY register ... */

/* MPY bitfield type ... */

typedef struct {
  __REG16 op1 : 16;       /* [15:0] */
} h430_mul_mpy_bf_t;

/* MPY address mapping ... */

__IO_REG16_BIT(H430_MUL_MPY, 0x00000130u, __READ_WRITE, h430_mul_mpy_bf_t);

/* ------------------------------------------------------------------------------ */
/* MPYS register ... */

/* MPYS bitfield type ... */

typedef struct {
  __REG16 op1 : 16;       /* [15:0] */
} h430_mul_mpys_bf_t;

/* MPYS address mapping ... */

__IO_REG16_BIT(H430_MUL_MPYS, 0x00000132u, __READ_WRITE, h430_mul_mpys_bf_t);

/* ------------------------------------------------------------------------------ */
/* MAC register ... */

/* MAC bitfield type ... */

typedef struct {
  __REG16 op1 : 16;       /* [15:0] */
} h430_mul_mac_bf_t;

/* MAC address mapping ... */

__IO_REG16_BIT(H430_MUL_MAC, 0x00000134u, __READ_WRITE, h430_mul_mac_bf_t);

/* ------------------------------------------------------------------------------ */
/* MACS register ... */

/* MACS bitfield type ... */

typedef struct {
  __REG16 op1 : 16;       /* [15:0] */
} h430_mul_macs_bf_t;

/* MACS address mapping ... */

__IO_REG16_BIT(H430_MUL_MACS, 0x00000136u, __READ_WRITE, h430_mul_macs_bf_t);

/* ------------------------------------------------------------------------------ */
/* OP2 register ... */

/* OP2 bitfield type ... */

typedef struct {
  __REG16 op2 : 16;       /* [15:0] */
} h430_mul_op2_bf_t;

/* OP2 address mapping ... */

__IO_REG16_BIT(H430_MUL_OP2, 0x00000138u, __READ_WRITE, h430_mul_op2_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESLO register ... */

/* RESLO bitfield type ... */

typedef struct {
  __REG16 sum_lo : 16;       /* [15:0] */
} h430_mul_reslo_bf_t;

/* RESLO address mapping ... */

__IO_REG16_BIT(H430_MUL_RESLO, 0x0000013Au, __READ_WRITE, h430_mul_reslo_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESHI register ... */

/* RESHI bitfield type ... */

typedef struct {
  __REG16 sum_hi : 16;       /* [15:0] */
} h430_mul_reshi_bf_t;

/* RESHI address mapping ... */

__IO_REG16_BIT(H430_MUL_RESHI, 0x0000013Cu, __READ_WRITE, h430_mul_reshi_bf_t);

/* ------------------------------------------------------------------------------ */
/* SUMEXT register ... */

/* SUMEXT bitfield type ... */

typedef struct {
  __REG16 sum_ext : 16;       /* [15:0] */
} h430_mul_sumext_bf_t;

/* SUMEXT address mapping ... */

__IO_REG16_BIT(H430_MUL_SUMEXT, 0x0000013Eu, __READ, h430_mul_sumext_bf_t);

/* ============================================================================== */
/* DIVIDER module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* OP1 registers ... */

/* OP1 bitfield type ... */

typedef struct {
  __REG16 op1 : 16;       /* [15:0] */
} divider_op1_bf_t;

/* OP1 address mapping ... */

__IO_REG16_BIT(DIVIDER_OP1LO, 0x00000140u, __READ_WRITE, divider_op1_bf_t);
__IO_REG16_BIT(DIVIDER_OP1HI, 0x00000142u, __READ_WRITE, divider_op1_bf_t);

/* ------------------------------------------------------------------------------ */
/* OP2 register ... */

/* OP2 bitfield type ... */

typedef struct {
  __REG16 op2 : 16;       /* [15:0] */
} divider_op2_bf_t;

/* OP2 address mapping ... */

__IO_REG16_BIT(DIVIDER_OP2, 0x00000144u, __READ_WRITE, divider_op2_bf_t);

/* ------------------------------------------------------------------------------ */
/* OP2S register ... */

/* OP2S bitfield type ... */

typedef struct {
  __REG16 op2 : 16;       /* [15:0] */
} divider_op2s_bf_t;

/* OP2S address mapping ... */

__IO_REG16_BIT(DIVIDER_OP2S, 0x00000146u, __READ_WRITE, divider_op2s_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESULT registers ... */

/* RESULT bitfield type ... */

typedef struct {
  __REG16 result : 16;       /* [15:0] */
} divider_result_bf_t;

/* RESULT address mapping ... */

__IO_REG16_BIT(DIVIDER_RESULTLO, 0x00000148u, __READ, divider_result_bf_t);
__IO_REG16_BIT(DIVIDER_RESULTHI, 0x0000014Au, __READ, divider_result_bf_t);

/* ------------------------------------------------------------------------------ */
/* REMAINDER register ... */

/* REMAINDER bitfield type ... */

typedef struct {
  __REG16 remainder : 16;       /* [15:0] */
} divider_remainder_bf_t;

/* REMAINDER address mapping ... */

__IO_REG16_BIT(DIVIDER_REMAINDER, 0x0000014Cu, __READ, divider_remainder_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 evt_div_by_zero :  1;       /* [0] */
  __REG16                 : 15;
} divider_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_DIVIDER_IRQ_STATUS_EVT_DIV_BY_ZERO = 0x0001u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_STATUS, 0x00000170u, __READ_WRITE, divider_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask :  1;       /* [0] */
  __REG16      : 15;
} divider_irq_mask_bf_t;

/* IRQ_MASK single bit enum ... */

enum {
  E_DIVIDER_IRQ_MASK_MASK = 0x0001u
};

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_MASK, 0x00000174u, __READ_WRITE, divider_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  1;       /* [0] */
  __REG16     : 15;
} divider_irq_venable_bf_t;

/* IRQ_VENABLE single bit enum ... */

enum {
  E_DIVIDER_IRQ_VENABLE_VNO = 0x0001u
};

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_VENABLE, 0x00000178u, __WRITE, divider_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  1;       /* [0] */
  __REG16     : 15;
} divider_irq_vdisable_bf_t;

/* IRQ_VDISABLE single bit enum ... */

enum {
  E_DIVIDER_IRQ_VDISABLE_VNO = 0x0001u
};

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_VDISABLE, 0x0000017Au, __WRITE, divider_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  1;       /* [0] */
  __REG16      : 15;
} divider_irq_vmax_bf_t;

/* IRQ_VMAX single bit enum ... */

enum {
  E_DIVIDER_IRQ_VMAX_VMAX = 0x0001u
};

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_VMAX, 0x0000017Cu, __READ_WRITE, divider_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  1;       /* [0] */
  __REG16     : 15;
} divider_irq_vno_bf_t;

/* IRQ_VNO single bit enum ... */

enum {
  E_DIVIDER_IRQ_VNO_VNO = 0x0001u
};

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(DIVIDER_IRQ_VNO, 0x0000017Eu, __READ_WRITE, divider_irq_vno_bf_t);

/* ============================================================================== */
/* SYS_STATE module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* POWER register ... */

/* POWER bitfield type ... */

typedef struct {
  __REG16 sleep        :  1;       /* [0] */
  __REG16 standby      :  1;       /* [1] */
  __REG16 wakeup_delay :  7;       /* [8:2] */
  __REG16              :  7;
} sys_state_power_bf_t;

/* POWER single bit enum ... */

enum {
  E_SYS_STATE_POWER_SLEEP        = 0x0001u,
  E_SYS_STATE_POWER_STANDBY      = 0x0002u
};

/* POWER address mapping ... */

__IO_REG16_BIT(SYS_STATE_POWER, 0x00000180u, __WRITE, sys_state_power_bf_t);

/* ------------------------------------------------------------------------------ */
/* CONTROL register ... */

/* CONTROL bitfield type ... */

typedef struct {
  __REG16 sys_clk_sel :  2;       /* [1:0] */
  __REG16 pwm_clk_sel :  1;       /* [2] */
  __REG16             : 13;
} sys_state_control_bf_t;

/* CONTROL single bit enum ... */

enum {
  E_SYS_STATE_CONTROL_PWM_CLK_SEL = 0x0004u
};

/* CONTROL address mapping ... */

__IO_REG16_BIT(SYS_STATE_CONTROL, 0x00000182u, __READ_WRITE, sys_state_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESET_STATUS register ... */

/* RESET_STATUS bitfield type ... */

typedef struct {
  __REG16 v1v5_ok         :  1;       /* [0] */
  __REG16 sys_clk_fail    :  1;       /* [1] */
  __REG16 software        :  1;       /* [2] */
  __REG16 sram_parity     :  1;       /* [3] */
  __REG16 watchdog_window :  1;       /* [4] */
  __REG16 watchdog        :  1;       /* [5] */
  __REG16 watchdog_zero   :  1;       /* [6] */
  __REG16 crc_mismatch    :  1;       /* [7] */
  __REG16 cal_parity      :  1;       /* [8] */
  __REG16 imem_ecc_error  :  1;       /* [9] */
  __REG16                 :  6;
} sys_state_reset_status_bf_t;

/* RESET_STATUS single bit enum ... */

enum {
  E_SYS_STATE_RESET_STATUS_V1V5_OK         = 0x0001u,
  E_SYS_STATE_RESET_STATUS_SYS_CLK_FAIL    = 0x0002u,
  E_SYS_STATE_RESET_STATUS_SOFTWARE        = 0x0004u,
  E_SYS_STATE_RESET_STATUS_SRAM_PARITY     = 0x0008u,
  E_SYS_STATE_RESET_STATUS_WATCHDOG_WINDOW = 0x0010u,
  E_SYS_STATE_RESET_STATUS_WATCHDOG        = 0x0020u,
  E_SYS_STATE_RESET_STATUS_WATCHDOG_ZERO   = 0x0040u,
  E_SYS_STATE_RESET_STATUS_CRC_MISMATCH    = 0x0080u,
  E_SYS_STATE_RESET_STATUS_CAL_PARITY      = 0x0100u,
  E_SYS_STATE_RESET_STATUS_IMEM_ECC_ERROR  = 0x0200u
};

/* RESET_STATUS address mapping ... */

__IO_REG16_BIT(SYS_STATE_RESET_STATUS, 0x00000184u, __READ, sys_state_reset_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESET_STATUS_CLEAR register ... */

/* RESET_STATUS_CLEAR bitfield type ... */

typedef struct {
  __REG16 clear : 10;       /* [9:0] */
  __REG16       :  6;
} sys_state_reset_status_clear_bf_t;

/* RESET_STATUS_CLEAR address mapping ... */

__IO_REG16_BIT(SYS_STATE_RESET_STATUS_CLEAR, 0x00000186u, __WRITE, sys_state_reset_status_clear_bf_t);

/* ------------------------------------------------------------------------------ */
/* RESET_ENABLE register ... */

/* RESET_ENABLE bitfield type ... */

typedef struct {
  __REG16 software        :  1;       /* [0] */
  __REG16 sram_parity     :  1;       /* [1] */
  __REG16 watchdog_window :  1;       /* [2] */
  __REG16 watchdog        :  1;       /* [3] */
  __REG16 watchdog_zero   :  1;       /* [4] */
  __REG16 crc_mismatch    :  1;       /* [5] */
  __REG16 cal_parity      :  1;       /* [6] */
  __REG16 imem_ecc_error  :  1;       /* [7] */
  __REG16                 :  8;
} sys_state_reset_enable_bf_t;

/* RESET_ENABLE single bit enum ... */

enum {
  E_SYS_STATE_RESET_ENABLE_SOFTWARE        = 0x0001u,
  E_SYS_STATE_RESET_ENABLE_SRAM_PARITY     = 0x0002u,
  E_SYS_STATE_RESET_ENABLE_WATCHDOG_WINDOW = 0x0004u,
  E_SYS_STATE_RESET_ENABLE_WATCHDOG        = 0x0008u,
  E_SYS_STATE_RESET_ENABLE_WATCHDOG_ZERO   = 0x0010u,
  E_SYS_STATE_RESET_ENABLE_CRC_MISMATCH    = 0x0020u,
  E_SYS_STATE_RESET_ENABLE_CAL_PARITY      = 0x0040u,
  E_SYS_STATE_RESET_ENABLE_IMEM_ECC_ERROR  = 0x0080u
};

/* RESET_ENABLE address mapping ... */

__IO_REG16_BIT(SYS_STATE_RESET_ENABLE, 0x00000188u, __READ_WRITE, sys_state_reset_enable_bf_t);

/* ------------------------------------------------------------------------------ */
/* SW_RESET register ... */

/* SW_RESET bitfield type ... */

typedef struct {
  __REG16 por_flag          :  1;       /* [0] */
  __REG16 sw_reset_flag     :  3;       /* [3:1] */
  __REG16 sw_reset          :  1;       /* [4] */
  __REG16 set_sw_reset_flag :  3;       /* [7:5] */
  __REG16                   :  8;
} sys_state_sw_reset_bf_t;

/* SW_RESET single bit enum ... */

enum {
  E_SYS_STATE_SW_RESET_POR_FLAG          = 0x0001u,
  E_SYS_STATE_SW_RESET_SW_RESET          = 0x0010u
};

/* SW_RESET address mapping ... */

__IO_REG16_BIT(SYS_STATE_SW_RESET, 0x0000018Au, __READ_WRITE, sys_state_sw_reset_bf_t);

/* ------------------------------------------------------------------------------ */
/* SLEEP_ENABLE register ... */

/* SLEEP_ENABLE bitfield type ... */

typedef struct {
  __REG16 software        :  1;       /* [0] */
  __REG16 sram_parity     :  1;       /* [1] */
  __REG16 watchdog_window :  1;       /* [2] */
  __REG16 watchdog        :  1;       /* [3] */
  __REG16 watchdog_zero   :  1;       /* [4] */
  __REG16 crc_mismatch    :  1;       /* [5] */
  __REG16 cal_parity      :  1;       /* [6] */
  __REG16 imem_ecc_error  :  1;       /* [7] */
  __REG16                 :  8;
} sys_state_sleep_enable_bf_t;

/* SLEEP_ENABLE single bit enum ... */

enum {
  E_SYS_STATE_SLEEP_ENABLE_SOFTWARE        = 0x0001u,
  E_SYS_STATE_SLEEP_ENABLE_SRAM_PARITY     = 0x0002u,
  E_SYS_STATE_SLEEP_ENABLE_WATCHDOG_WINDOW = 0x0004u,
  E_SYS_STATE_SLEEP_ENABLE_WATCHDOG        = 0x0008u,
  E_SYS_STATE_SLEEP_ENABLE_WATCHDOG_ZERO   = 0x0010u,
  E_SYS_STATE_SLEEP_ENABLE_CRC_MISMATCH    = 0x0020u,
  E_SYS_STATE_SLEEP_ENABLE_CAL_PARITY      = 0x0040u,
  E_SYS_STATE_SLEEP_ENABLE_IMEM_ECC_ERROR  = 0x0080u
};

/* SLEEP_ENABLE address mapping ... */

__IO_REG16_BIT(SYS_STATE_SLEEP_ENABLE, 0x0000018Cu, __READ_WRITE, sys_state_sleep_enable_bf_t);

/* ------------------------------------------------------------------------------ */
/* TEST_MODE register ... */

/* TEST_MODE bitfield type ... */

typedef struct {
  __REG16 mode :  8;       /* [7:0] */
  __REG16 pass :  8;       /* [15:8] */
} sys_state_test_mode_bf_t;

/* TEST_MODE address mapping ... */

__IO_REG16_BIT(SYS_STATE_TEST_MODE, 0x0000018Eu, __READ_WRITE, sys_state_test_mode_bf_t);

/* ------------------------------------------------------------------------------ */
/* SIGNATURE registers ... */

/* SIGNATURE bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} sys_state_signature_bf_t;

/* SIGNATURE address mapping ... */

__IO_REG16_BIT(SYS_STATE_SIGNATURE_0, 0x00000190u, __READ_WRITE, sys_state_signature_bf_t);
__IO_REG16_BIT(SYS_STATE_SIGNATURE_1, 0x00000192u, __READ_WRITE, sys_state_signature_bf_t);
__IO_REG16_BIT(SYS_STATE_SIGNATURE_2, 0x00000194u, __READ_WRITE, sys_state_signature_bf_t);
__IO_REG16_BIT(SYS_STATE_SIGNATURE_3, 0x00000196u, __READ_WRITE, sys_state_signature_bf_t);

/* ------------------------------------------------------------------------------ */
/* OSC_5V_CONTROL register ... */

/* OSC_5V_CONTROL bitfield type ... */

typedef struct {
  __REG16 do_meas :  1;       /* [0] */
  __REG16         : 15;
} sys_state_osc_5v_control_bf_t;

/* OSC_5V_CONTROL single bit enum ... */

enum {
  E_SYS_STATE_OSC_5V_CONTROL_DO_MEAS = 0x0001u
};

/* OSC_5V_CONTROL address mapping ... */

__IO_REG16_BIT(SYS_STATE_OSC_5V_CONTROL, 0x00000198u, __WRITE, sys_state_osc_5v_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* OSC_5V_STATUS register ... */

/* OSC_5V_STATUS bitfield type ... */

typedef struct {
  __REG16 meas_val :  8;       /* [7:0] */
  __REG16 busy     :  1;       /* [8] */
  __REG16          :  7;
} sys_state_osc_5v_status_bf_t;

/* OSC_5V_STATUS single bit enum ... */

enum {
  E_SYS_STATE_OSC_5V_STATUS_BUSY     = 0x0100u
};

/* OSC_5V_STATUS address mapping ... */

__IO_REG16_BIT(SYS_STATE_OSC_5V_STATUS, 0x0000019Au, __READ, sys_state_osc_5v_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* TEST_MODE_COUNTER register ... */

/* TEST_MODE_COUNTER bitfield type ... */

typedef struct {
  __REG16 val : 15;       /* [14:0] */
  __REG16     :  1;
} sys_state_test_mode_counter_bf_t;

/* TEST_MODE_COUNTER address mapping ... */

__IO_REG16_BIT(SYS_STATE_TEST_MODE_COUNTER, 0x0000019Cu, __READ, sys_state_test_mode_counter_bf_t);

/* ------------------------------------------------------------------------------ */
/* VERSION register ... */

/* VERSION bitfield type ... */

typedef struct {
  __REG16 mask    :  3;       /* [2:0] */
  __REG16 version :  2;       /* [4:3] */
  __REG16         : 11;
} sys_state_version_bf_t;

/* VERSION address mapping ... */

__IO_REG16_BIT(SYS_STATE_VERSION, 0x0000019Eu, __READ, sys_state_version_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION_LOCK register ... */

/* CALIBRATION_LOCK bitfield type ... */

typedef struct {
  __REG16 lock :  1;       /* [0] */
  __REG16      : 15;
} sys_state_calibration_lock_bf_t;

/* CALIBRATION_LOCK single bit enum ... */

enum {
  E_SYS_STATE_CALIBRATION_LOCK_LOCK = 0x0001u
};

/* CALIBRATION_LOCK address mapping ... */

__IO_REG16_BIT(SYS_STATE_CALIBRATION_LOCK, 0x000001A0u, __READ_WRITE, sys_state_calibration_lock_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION0 register ... */

/* CALIBRATION0 bitfield type ... */

typedef struct {
  __REG16 trim_osc_48m      :  5;       /* [4:0] */
  __REG16 trim_osc_48m_temp :  3;       /* [7:5] */
  __REG16 trim_iref_temp    :  3;       /* [10:8] */
  __REG16 trim_iref         :  5;       /* [15:11] */
} sys_state_calibration0_bf_t;

/* CALIBRATION0 address mapping ... */

__IO_REG16_BIT(SYS_STATE_CALIBRATION0, 0x000001A2u, __READ_WRITE, sys_state_calibration0_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION1 register ... */

/* CALIBRATION1 bitfield type ... */

typedef struct {
  __REG16 trim_led0     :  6;       /* [5:0] */
  __REG16 trim_led1     :  6;       /* [11:6] */
  __REG16 led_top_disop :  1;       /* [12] */
  __REG16               :  3;
} sys_state_calibration1_bf_t;

/* CALIBRATION1 single bit enum ... */

enum {
  E_SYS_STATE_CALIBRATION1_LED_TOP_DISOP = 0x1000u
};

/* CALIBRATION1 address mapping ... */

__IO_REG16_BIT(SYS_STATE_CALIBRATION1, 0x000001A4u, __READ_WRITE, sys_state_calibration1_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION2 register ... */

/* CALIBRATION2 bitfield type ... */

typedef struct {
  __REG16 trim_led2      :  6;       /* [5:0] */
  __REG16 trim_led3      :  6;       /* [11:6] */
  __REG16 trim_bg        :  2;       /* [13:12] */
  __REG16 trim_v1v5_load :  2;       /* [15:14] */
} sys_state_calibration2_bf_t;

/* CALIBRATION2 address mapping ... */

__IO_REG16_BIT(SYS_STATE_CALIBRATION2, 0x000001A6u, __READ_WRITE, sys_state_calibration2_bf_t);

/* ------------------------------------------------------------------------------ */
/* SW_RESET_FLAG_CLEAR register ... */

/* SW_RESET_FLAG_CLEAR bitfield type ... */

typedef struct {
  __REG16       :  1;
  __REG16 clear :  3;       /* [3:1] */
  __REG16       : 12;
} sys_state_sw_reset_flag_clear_bf_t;

/* SW_RESET_FLAG_CLEAR address mapping ... */

__IO_REG16_BIT(SYS_STATE_SW_RESET_FLAG_CLEAR, 0x000001A8u, __WRITE, sys_state_sw_reset_flag_clear_bf_t);

/* ------------------------------------------------------------------------------ */
/* ENABLE_JTAG register ... */

/* ENABLE_JTAG bitfield type ... */

typedef struct {
  __REG16 enable             :  1;       /* [0] */
  __REG16 exit_boot_loader   :  1;       /* [1] */
  __REG16                    :  2;
  __REG16 skip_fuse_reflect  :  1;       /* [4] */
  __REG16 prevent_write_lock :  1;       /* [5] */
  __REG16                    : 10;
} sys_state_enable_jtag_bf_t;

/* ENABLE_JTAG single bit enum ... */

enum {
  E_SYS_STATE_ENABLE_JTAG_ENABLE             = 0x0001u,
  E_SYS_STATE_ENABLE_JTAG_EXIT_BOOT_LOADER   = 0x0002u,
  E_SYS_STATE_ENABLE_JTAG_SKIP_FUSE_REFLECT  = 0x0010u,
  E_SYS_STATE_ENABLE_JTAG_PREVENT_WRITE_LOCK = 0x0020u
};

/* ENABLE_JTAG address mapping ... */

__IO_REG16_BIT(SYS_STATE_ENABLE_JTAG, 0x000001ACu, __READ_WRITE, sys_state_enable_jtag_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 por5v_wake_up   :  1;       /* [0] */
  __REG16 lin_wake_up     :  1;       /* [1] */
  __REG16 timer_wake_up   :  1;       /* [2] */
  __REG16 temp_wake_up    :  1;       /* [3] */
  __REG16 jtag_wake_up    :  1;       /* [4] */
  __REG16 software        :  1;       /* [5] */
  __REG16 sram_parity     :  1;       /* [6] */
  __REG16 watchdog_window :  1;       /* [7] */
  __REG16 watchdog        :  1;       /* [8] */
  __REG16 watchdog_zero   :  1;       /* [9] */
  __REG16 crc_mismatch    :  1;       /* [10] */
  __REG16 cal_parity      :  1;       /* [11] */
  __REG16 imem_ecc_error  :  1;       /* [12] */
  __REG16                 :  3;
} sys_state_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_SYS_STATE_IRQ_STATUS_POR5V_WAKE_UP   = 0x0001u,
  E_SYS_STATE_IRQ_STATUS_LIN_WAKE_UP     = 0x0002u,
  E_SYS_STATE_IRQ_STATUS_TIMER_WAKE_UP   = 0x0004u,
  E_SYS_STATE_IRQ_STATUS_TEMP_WAKE_UP    = 0x0008u,
  E_SYS_STATE_IRQ_STATUS_JTAG_WAKE_UP    = 0x0010u,
  E_SYS_STATE_IRQ_STATUS_SOFTWARE        = 0x0020u,
  E_SYS_STATE_IRQ_STATUS_SRAM_PARITY     = 0x0040u,
  E_SYS_STATE_IRQ_STATUS_WATCHDOG_WINDOW = 0x0080u,
  E_SYS_STATE_IRQ_STATUS_WATCHDOG        = 0x0100u,
  E_SYS_STATE_IRQ_STATUS_WATCHDOG_ZERO   = 0x0200u,
  E_SYS_STATE_IRQ_STATUS_CRC_MISMATCH    = 0x0400u,
  E_SYS_STATE_IRQ_STATUS_CAL_PARITY      = 0x0800u,
  E_SYS_STATE_IRQ_STATUS_IMEM_ECC_ERROR  = 0x1000u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_STATUS, 0x000001B0u, __READ_WRITE, sys_state_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask : 13;       /* [12:0] */
  __REG16      :  3;
} sys_state_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_MASK, 0x000001B4u, __READ_WRITE, sys_state_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} sys_state_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_VENABLE, 0x000001B8u, __WRITE, sys_state_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} sys_state_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_VDISABLE, 0x000001BAu, __WRITE, sys_state_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} sys_state_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_VMAX, 0x000001BCu, __READ_WRITE, sys_state_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} sys_state_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(SYS_STATE_IRQ_VNO, 0x000001BEu, __READ_WRITE, sys_state_irq_vno_bf_t);

/* ============================================================================== */
/* LINSCI module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* BAUD_RATE register ... */

/* BAUD_RATE bitfield type ... */

typedef struct {
  __REG16 frac :  5;       /* [4:0] */
  __REG16 div  : 11;       /* [15:5] */
} linsci_baud_rate_bf_t;

/* BAUD_RATE address mapping ... */

__IO_REG16_BIT(LINSCI_BAUD_RATE, 0x000001C0u, __READ_WRITE, linsci_baud_rate_bf_t);

/* ------------------------------------------------------------------------------ */
/* UART_CONFIG register ... */

/* UART_CONFIG bitfield type ... */

typedef struct {
  __REG16 re           :  1;       /* [0] */
  __REG16 te           :  1;       /* [1] */
  __REG16 parity       :  2;       /* [3:2] */
  __REG16 stop         :  1;       /* [4] */
  __REG16 mask_brk_err :  1;       /* [5] */
  __REG16 clk_src      :  1;       /* [6] */
  __REG16              :  1;
  __REG16 txd_mask     :  1;       /* [8] */
  __REG16 txd_val      :  1;       /* [9] */
  __REG16 rxd_mask     :  1;       /* [10] */
  __REG16 rxd_val      :  1;       /* [11] */
  __REG16 debounce     :  3;       /* [14:12] */
  __REG16              :  1;
} linsci_uart_config_bf_t;

/* UART_CONFIG single bit enum ... */

enum {
  E_LINSCI_UART_CONFIG_RE           = 0x0001u,
  E_LINSCI_UART_CONFIG_TE           = 0x0002u,
  E_LINSCI_UART_CONFIG_STOP         = 0x0010u,
  E_LINSCI_UART_CONFIG_MASK_BRK_ERR = 0x0020u,
  E_LINSCI_UART_CONFIG_CLK_SRC      = 0x0040u,
  E_LINSCI_UART_CONFIG_TXD_MASK     = 0x0100u,
  E_LINSCI_UART_CONFIG_TXD_VAL      = 0x0200u,
  E_LINSCI_UART_CONFIG_RXD_MASK     = 0x0400u,
  E_LINSCI_UART_CONFIG_RXD_VAL      = 0x0800u
};

/* UART_CONFIG address mapping ... */

__IO_REG16_BIT(LINSCI_UART_CONFIG, 0x000001C2u, __READ_WRITE, linsci_uart_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_CONFIG register ... */

/* LIN_CONFIG bitfield type ... */

typedef struct {
  __REG16 autobaud           :  1;       /* [0] */
  __REG16 collision          :  1;       /* [1] */
  __REG16 break_thd          :  1;       /* [2] */
  __REG16 header_processing  :  1;       /* [3] */
  __REG16 filter_pid         :  1;       /* [4] */
  __REG16 chksum_enable      :  1;       /* [5] */
  __REG16 chksum_type        :  1;       /* [6] */
  __REG16 chksum_insert      :  1;       /* [7] */
  __REG16 suppress_tx_fb     :  1;       /* [8] */
  __REG16 sync_validation    :  1;       /* [9] */
  __REG16 dma_rx_skip_last   :  1;       /* [10] */
  __REG16 short_detect_e     :  1;       /* [11] */
  __REG16 ppid_edge_detect_e :  1;       /* [12] */
  __REG16 sync_byte_ext      :  1;       /* [13] */
  __REG16                    :  2;
} linsci_lin_config_bf_t;

/* LIN_CONFIG single bit enum ... */

enum {
  E_LINSCI_LIN_CONFIG_AUTOBAUD           = 0x0001u,
  E_LINSCI_LIN_CONFIG_COLLISION          = 0x0002u,
  E_LINSCI_LIN_CONFIG_BREAK_THD          = 0x0004u,
  E_LINSCI_LIN_CONFIG_HEADER_PROCESSING  = 0x0008u,
  E_LINSCI_LIN_CONFIG_FILTER_PID         = 0x0010u,
  E_LINSCI_LIN_CONFIG_CHKSUM_ENABLE      = 0x0020u,
  E_LINSCI_LIN_CONFIG_CHKSUM_TYPE        = 0x0040u,
  E_LINSCI_LIN_CONFIG_CHKSUM_INSERT      = 0x0080u,
  E_LINSCI_LIN_CONFIG_SUPPRESS_TX_FB     = 0x0100u,
  E_LINSCI_LIN_CONFIG_SYNC_VALIDATION    = 0x0200u,
  E_LINSCI_LIN_CONFIG_DMA_RX_SKIP_LAST   = 0x0400u,
  E_LINSCI_LIN_CONFIG_SHORT_DETECT_E     = 0x0800u,
  E_LINSCI_LIN_CONFIG_PPID_EDGE_DETECT_E = 0x1000u,
  E_LINSCI_LIN_CONFIG_SYNC_BYTE_EXT      = 0x2000u
};

/* LIN_CONFIG address mapping ... */

__IO_REG16_BIT(LINSCI_LIN_CONFIG, 0x000001C4u, __READ_WRITE, linsci_lin_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_CONTROL register ... */

/* LIN_CONTROL bitfield type ... */

typedef struct {
  __REG16 abort_rx  :  1;       /* [0] */
  __REG16 abort_tx  :  1;       /* [1] */
  __REG16 tx_chksum :  1;       /* [2] */
  __REG16 rx_sleep  :  1;       /* [3] */
  __REG16           : 12;
} linsci_lin_control_bf_t;

/* LIN_CONTROL single bit enum ... */

enum {
  E_LINSCI_LIN_CONTROL_ABORT_RX  = 0x0001u,
  E_LINSCI_LIN_CONTROL_ABORT_TX  = 0x0002u,
  E_LINSCI_LIN_CONTROL_TX_CHKSUM = 0x0004u,
  E_LINSCI_LIN_CONTROL_RX_SLEEP  = 0x0008u
};

/* LIN_CONTROL address mapping ... */

__IO_REG16_BIT(LINSCI_LIN_CONTROL, 0x000001C6u, __READ_WRITE, linsci_lin_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* STATUS register ... */

/* STATUS bitfield type ... */

typedef struct {
  __REG16 rx_idle          :  1;       /* [0] */
  __REG16 tx_idle          :  1;       /* [1] */
  __REG16 rx_fifo_full     :  1;       /* [2] */
  __REG16 tx_fifo_full     :  1;       /* [3] */
  __REG16 rx_header_state  :  2;       /* [5:4] */
  __REG16 rx_chksum_valid  :  1;       /* [6] */
  __REG16 rx_post_pid_edge :  1;       /* [7] */
  __REG16                  :  8;
} linsci_status_bf_t;

/* STATUS single bit enum ... */

enum {
  E_LINSCI_STATUS_RX_IDLE          = 0x0001u,
  E_LINSCI_STATUS_TX_IDLE          = 0x0002u,
  E_LINSCI_STATUS_RX_FIFO_FULL     = 0x0004u,
  E_LINSCI_STATUS_TX_FIFO_FULL     = 0x0008u,
  E_LINSCI_STATUS_RX_CHKSUM_VALID  = 0x0040u,
  E_LINSCI_STATUS_RX_POST_PID_EDGE = 0x0080u
};

/* STATUS address mapping ... */

__IO_REG16_BIT(LINSCI_STATUS, 0x000001C8u, __READ, linsci_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* ERROR register ... */

/* ERROR bitfield type ... */

typedef struct {
  __REG16 frame_err      :  1;       /* [0] */
  __REG16 parity_err     :  1;       /* [1] */
  __REG16 sync_err       :  1;       /* [2] */
  __REG16 sync_ov        :  1;       /* [3] */
  __REG16 sync_invalid   :  1;       /* [4] */
  __REG16 pid_parity_err :  1;       /* [5] */
  __REG16 bus_collision  :  1;       /* [6] */
  __REG16 concurrent_brk :  1;       /* [7] */
  __REG16 txd_timeout    :  1;       /* [8] */
  __REG16 rx_overflow    :  1;       /* [9] */
  __REG16 ppid_edge_err  :  1;       /* [10] */
  __REG16                :  5;
} linsci_error_bf_t;

/* ERROR single bit enum ... */

enum {
  E_LINSCI_ERROR_FRAME_ERR      = 0x0001u,
  E_LINSCI_ERROR_PARITY_ERR     = 0x0002u,
  E_LINSCI_ERROR_SYNC_ERR       = 0x0004u,
  E_LINSCI_ERROR_SYNC_OV        = 0x0008u,
  E_LINSCI_ERROR_SYNC_INVALID   = 0x0010u,
  E_LINSCI_ERROR_PID_PARITY_ERR = 0x0020u,
  E_LINSCI_ERROR_BUS_COLLISION  = 0x0040u,
  E_LINSCI_ERROR_CONCURRENT_BRK = 0x0080u,
  E_LINSCI_ERROR_TXD_TIMEOUT    = 0x0100u,
  E_LINSCI_ERROR_RX_OVERFLOW    = 0x0200u,
  E_LINSCI_ERROR_PPID_EDGE_ERR  = 0x0400u
};

/* ERROR address mapping ... */

__IO_REG16_BIT(LINSCI_ERROR, 0x000001CAu, __READ_WRITE, linsci_error_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_CONFIGURATION register ... */

/* LIN_CONFIGURATION bitfield type ... */

typedef struct {
  __REG16 cbm             :  1;       /* [0] */
  __REG16 txd_timeout_reg :  1;       /* [1] */
  __REG16 timer           :  1;       /* [2] */
  __REG16 dma             :  1;       /* [3] */
  __REG16 version         :  4;       /* [7:4] */
  __REG16                 :  8;
} linsci_lin_configuration_bf_t;

/* LIN_CONFIGURATION single bit enum ... */

enum {
  E_LINSCI_LIN_CONFIGURATION_CBM             = 0x0001u,
  E_LINSCI_LIN_CONFIGURATION_TXD_TIMEOUT_REG = 0x0002u,
  E_LINSCI_LIN_CONFIGURATION_TIMER           = 0x0004u,
  E_LINSCI_LIN_CONFIGURATION_DMA             = 0x0008u
};

/* LIN_CONFIGURATION address mapping ... */

__IO_REG16_BIT(LINSCI_LIN_CONFIGURATION, 0x000001CCu, __READ, linsci_lin_configuration_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA register ... */

/* DATA bitfield type ... */

typedef struct {
  __REG16 data       :  8;       /* [7:0] */
  __REG16 send_break :  1;       /* [8] */
  __REG16            :  7;
} linsci_data_bf_t;

/* DATA single bit enum ... */

enum {
  E_LINSCI_DATA_SEND_BREAK = 0x0100u
};

/* DATA address mapping ... */

__IO_REG16_BIT(LINSCI_DATA, 0x000001CEu, __READ_WRITE, linsci_data_bf_t);

/* ------------------------------------------------------------------------------ */
/* TBIT2_LENGTH register ... */

/* TBIT2_LENGTH bitfield type ... */

typedef struct {
  __REG16 data : 16;       /* [15:0] */
} linsci_tbit2_length_bf_t;

/* TBIT2_LENGTH address mapping ... */

__IO_REG16_BIT(LINSCI_TBIT2_LENGTH, 0x000001D0u, __READ, linsci_tbit2_length_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_PID register ... */

/* LIN_PID bitfield type ... */

typedef struct {
  __REG16 data :  8;       /* [7:0] */
  __REG16      :  8;
} linsci_lin_pid_bf_t;

/* LIN_PID address mapping ... */

__IO_REG16_BIT(LINSCI_LIN_PID, 0x000001D2u, __READ, linsci_lin_pid_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_CHECKSUM register ... */

/* LIN_CHECKSUM bitfield type ... */

typedef struct {
  __REG16 chksum_val :  8;       /* [7:0] */
  __REG16 initialize :  1;       /* [8] */
  __REG16            :  7;
} linsci_lin_checksum_bf_t;

/* LIN_CHECKSUM single bit enum ... */

enum {
  E_LINSCI_LIN_CHECKSUM_INITIALIZE = 0x0100u
};

/* LIN_CHECKSUM address mapping ... */

__IO_REG16_BIT(LINSCI_LIN_CHECKSUM, 0x000001D4u, __READ_WRITE, linsci_lin_checksum_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMER register ... */

/* TIMER bitfield type ... */

typedef struct {
  __REG16 enable          :  1;       /* [0] */
  __REG16 clk_src         :  1;       /* [1] */
  __REG16 prepare         :  2;       /* [3:2] */
  __REG16 break_restart   :  1;       /* [4] */
  __REG16 txd_timeout_e   :  1;       /* [5] */
  __REG16 capture_e       :  1;       /* [6] */
  __REG16 pid_cmp_val_e   :  1;       /* [7] */
  __REG16 txd_timeout_sel :  3;       /* [10:8] */
  __REG16                 :  5;
} linsci_timer_bf_t;

/* TIMER single bit enum ... */

enum {
  E_LINSCI_TIMER_ENABLE          = 0x0001u,
  E_LINSCI_TIMER_CLK_SRC         = 0x0002u,
  E_LINSCI_TIMER_BREAK_RESTART   = 0x0010u,
  E_LINSCI_TIMER_TXD_TIMEOUT_E   = 0x0020u,
  E_LINSCI_TIMER_CAPTURE_E       = 0x0040u,
  E_LINSCI_TIMER_PID_CMP_VAL_E   = 0x0080u
};

/* TIMER address mapping ... */

__IO_REG16_BIT(LINSCI_TIMER, 0x000001D6u, __READ_WRITE, linsci_timer_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMER_COUNTER register ... */

/* TIMER_COUNTER bitfield type ... */

typedef struct {
  __REG16 timer_counter : 16;       /* [15:0] */
} linsci_timer_counter_bf_t;

/* TIMER_COUNTER address mapping ... */

__IO_REG16_BIT(LINSCI_TIMER_COUNTER, 0x000001D8u, __READ, linsci_timer_counter_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMER_COMPARE register ... */

/* TIMER_COMPARE bitfield type ... */

typedef struct {
  __REG16 timer_compare : 16;       /* [15:0] */
} linsci_timer_compare_bf_t;

/* TIMER_COMPARE address mapping ... */

__IO_REG16_BIT(LINSCI_TIMER_COMPARE, 0x000001DAu, __READ_WRITE, linsci_timer_compare_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMER_CAPTURE register ... */

/* TIMER_CAPTURE bitfield type ... */

typedef struct {
  __REG16 timer_capture : 16;       /* [15:0] */
} linsci_timer_capture_bf_t;

/* TIMER_CAPTURE address mapping ... */

__IO_REG16_BIT(LINSCI_TIMER_CAPTURE, 0x000001DCu, __READ, linsci_timer_capture_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_TX_ADDRESS register ... */

/* DMA_TX_ADDRESS bitfield type ... */

typedef struct {
  __REG16 dma_tx_addr : 16;       /* [15:0] */
} linsci_dma_tx_address_bf_t;

/* DMA_TX_ADDRESS address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_TX_ADDRESS, 0x000001E0u, __READ_WRITE, linsci_dma_tx_address_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_TX_ADDRESS_HIGH register ... */

/* DMA_TX_ADDRESS_HIGH bitfield type ... */

typedef struct {
  __REG16 dma_tx_addr : 16;       /* [15:0] */
} linsci_dma_tx_address_high_bf_t;

/* DMA_TX_ADDRESS_HIGH address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_TX_ADDRESS_HIGH, 0x000001E2u, __READ_WRITE, linsci_dma_tx_address_high_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_TX_LENGTH register ... */

/* DMA_TX_LENGTH bitfield type ... */

typedef struct {
  __REG16 dma_tx_length :  8;       /* [7:0] */
  __REG16               :  8;
} linsci_dma_tx_length_bf_t;

/* DMA_TX_LENGTH address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_TX_LENGTH, 0x000001E4u, __READ_WRITE, linsci_dma_tx_length_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_RX_ADDRESS register ... */

/* DMA_RX_ADDRESS bitfield type ... */

typedef struct {
  __REG16 dma_rx_addr : 16;       /* [15:0] */
} linsci_dma_rx_address_bf_t;

/* DMA_RX_ADDRESS address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_RX_ADDRESS, 0x000001E8u, __READ_WRITE, linsci_dma_rx_address_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_RX_ADDRESS_HIGH register ... */

/* DMA_RX_ADDRESS_HIGH bitfield type ... */

typedef struct {
  __REG16 dma_rx_addr : 16;       /* [15:0] */
} linsci_dma_rx_address_high_bf_t;

/* DMA_RX_ADDRESS_HIGH address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_RX_ADDRESS_HIGH, 0x000001EAu, __READ_WRITE, linsci_dma_rx_address_high_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_RX_LENGTH register ... */

/* DMA_RX_LENGTH bitfield type ... */

typedef struct {
  __REG16 dma_rx_length :  8;       /* [7:0] */
  __REG16               :  8;
} linsci_dma_rx_length_bf_t;

/* DMA_RX_LENGTH address mapping ... */

__IO_REG16_BIT(LINSCI_DMA_RX_LENGTH, 0x000001ECu, __READ_WRITE, linsci_dma_rx_length_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 rxd_falling     :  1;       /* [0] */
  __REG16 rxd_rising      :  1;       /* [1] */
  __REG16 sci_timer_cmp   :  1;       /* [2] */
  __REG16 sci_timer_ov    :  1;       /* [3] */
  __REG16 bus_err         :  1;       /* [4] */
  __REG16 receiver_err    :  1;       /* [5] */
  __REG16 header_err      :  1;       /* [6] */
  __REG16 break_evt       :  1;       /* [7] */
  __REG16 sync_evt        :  1;       /* [8] */
  __REG16 pid_evt         :  1;       /* [9] */
  __REG16 rx_fifo_full    :  1;       /* [10] */
  __REG16 rx_dma_finished :  1;       /* [11] */
  __REG16 tx_fifo_empty   :  1;       /* [12] */
  __REG16 tx_dma_finished :  1;       /* [13] */
  __REG16 tx_finish_evt   :  1;       /* [14] */
  __REG16 tick_1ms        :  1;       /* [15] */
} linsci_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_LINSCI_IRQ_STATUS_RXD_FALLING     = 0x0001u,
  E_LINSCI_IRQ_STATUS_RXD_RISING      = 0x0002u,
  E_LINSCI_IRQ_STATUS_SCI_TIMER_CMP   = 0x0004u,
  E_LINSCI_IRQ_STATUS_SCI_TIMER_OV    = 0x0008u,
  E_LINSCI_IRQ_STATUS_BUS_ERR         = 0x0010u,
  E_LINSCI_IRQ_STATUS_RECEIVER_ERR    = 0x0020u,
  E_LINSCI_IRQ_STATUS_HEADER_ERR      = 0x0040u,
  E_LINSCI_IRQ_STATUS_BREAK_EVT       = 0x0080u,
  E_LINSCI_IRQ_STATUS_SYNC_EVT        = 0x0100u,
  E_LINSCI_IRQ_STATUS_PID_EVT         = 0x0200u,
  E_LINSCI_IRQ_STATUS_RX_FIFO_FULL    = 0x0400u,
  E_LINSCI_IRQ_STATUS_RX_DMA_FINISHED = 0x0800u,
  E_LINSCI_IRQ_STATUS_TX_FIFO_EMPTY   = 0x1000u,
  E_LINSCI_IRQ_STATUS_TX_DMA_FINISHED = 0x2000u,
  E_LINSCI_IRQ_STATUS_TX_FINISH_EVT   = 0x4000u,
  E_LINSCI_IRQ_STATUS_TICK_1MS        = 0x8000u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_STATUS, 0x000001F0u, __READ_WRITE, linsci_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask : 16;       /* [15:0] */
} linsci_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_MASK, 0x000001F4u, __READ_WRITE, linsci_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} linsci_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_VENABLE, 0x000001F8u, __WRITE, linsci_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} linsci_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_VDISABLE, 0x000001FAu, __WRITE, linsci_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  5;       /* [4:0] */
  __REG16      : 11;
} linsci_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_VMAX, 0x000001FCu, __READ_WRITE, linsci_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  5;       /* [4:0] */
  __REG16     : 11;
} linsci_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(LINSCI_IRQ_VNO, 0x000001FEu, __READ_WRITE, linsci_irq_vno_bf_t);

/* ============================================================================== */
/* GPIO module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* SELECT register ... */

/* SELECT bitfield type ... */

typedef struct {
  __REG16 select :  5;       /* [4:0] */
  __REG16        : 11;
} gpio_select_bf_t;

/* SELECT address mapping ... */

__IO_REG16_BIT(GPIO_SELECT, 0x00000200u, __READ_WRITE, gpio_select_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA_OE register ... */

/* DATA_OE bitfield type ... */

typedef struct {
  __REG16 enable  :  4;       /* [3:0] */
  __REG16 enable4 :  1;       /* [4] */
  __REG16         : 11;
} gpio_data_oe_bf_t;

/* DATA_OE single bit enum ... */

enum {
  E_GPIO_DATA_OE_ENABLE4 = 0x0010u
};

/* DATA_OE address mapping ... */

__IO_REG16_BIT(GPIO_DATA_OE, 0x00000202u, __READ_WRITE, gpio_data_oe_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA_IN register ... */

/* DATA_IN bitfield type ... */

typedef struct {
  __REG16 data :  5;       /* [4:0] */
  __REG16      : 11;
} gpio_data_in_bf_t;

/* DATA_IN address mapping ... */

__IO_REG16_BIT(GPIO_DATA_IN, 0x00000204u, __READ, gpio_data_in_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA_IE register ... */

/* DATA_IE bitfield type ... */

typedef struct {
  __REG16 enable :  5;       /* [4:0] */
  __REG16        : 11;
} gpio_data_ie_bf_t;

/* DATA_IE address mapping ... */

__IO_REG16_BIT(GPIO_DATA_IE, 0x00000206u, __READ_WRITE, gpio_data_ie_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA_OUT register ... */

/* DATA_OUT bitfield type ... */

typedef struct {
  __REG16      :  4;
  __REG16 data :  1;       /* [4] */
  __REG16      : 11;
} gpio_data_out_bf_t;

/* DATA_OUT single bit enum ... */

enum {
  E_GPIO_DATA_OUT_DATA = 0x0010u
};

/* DATA_OUT address mapping ... */

__IO_REG16_BIT(GPIO_DATA_OUT, 0x00000208u, __READ_WRITE, gpio_data_out_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 evt_pos_0 :  1;       /* [0] */
  __REG16 evt_neg_0 :  1;       /* [1] */
  __REG16 evt_pos_1 :  1;       /* [2] */
  __REG16 evt_neg_1 :  1;       /* [3] */
  __REG16 evt_pos_2 :  1;       /* [4] */
  __REG16 evt_neg_2 :  1;       /* [5] */
  __REG16 evt_pos_3 :  1;       /* [6] */
  __REG16 evt_neg_3 :  1;       /* [7] */
  __REG16 evt_pos_4 :  1;       /* [8] */
  __REG16 evt_neg_4 :  1;       /* [9] */
  __REG16           :  6;
} gpio_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_GPIO_IRQ_STATUS_EVT_POS_0 = 0x0001u,
  E_GPIO_IRQ_STATUS_EVT_NEG_0 = 0x0002u,
  E_GPIO_IRQ_STATUS_EVT_POS_1 = 0x0004u,
  E_GPIO_IRQ_STATUS_EVT_NEG_1 = 0x0008u,
  E_GPIO_IRQ_STATUS_EVT_POS_2 = 0x0010u,
  E_GPIO_IRQ_STATUS_EVT_NEG_2 = 0x0020u,
  E_GPIO_IRQ_STATUS_EVT_POS_3 = 0x0040u,
  E_GPIO_IRQ_STATUS_EVT_NEG_3 = 0x0080u,
  E_GPIO_IRQ_STATUS_EVT_POS_4 = 0x0100u,
  E_GPIO_IRQ_STATUS_EVT_NEG_4 = 0x0200u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_STATUS, 0x00000230u, __READ_WRITE, gpio_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask : 10;       /* [9:0] */
  __REG16      :  6;
} gpio_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_MASK, 0x00000234u, __READ_WRITE, gpio_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} gpio_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_VENABLE, 0x00000238u, __WRITE, gpio_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} gpio_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_VDISABLE, 0x0000023Au, __WRITE, gpio_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} gpio_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_VMAX, 0x0000023Cu, __READ_WRITE, gpio_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} gpio_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(GPIO_IRQ_VNO, 0x0000023Eu, __READ_WRITE, gpio_irq_vno_bf_t);

/* ============================================================================== */
/* CRC16 module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* START register ... */

/* START bitfield type ... */

typedef struct {
  __REG16 addr : 16;       /* [15:0] */
} crc16_start_bf_t;

/* START address mapping ... */

__IO_REG16_BIT(CRC16_START, 0x00000240u, __READ_WRITE, crc16_start_bf_t);

/* ------------------------------------------------------------------------------ */
/* LENGTH register ... */

/* LENGTH bitfield type ... */

typedef struct {
  __REG16 bytes : 16;       /* [15:0] */
} crc16_length_bf_t;

/* LENGTH address mapping ... */

__IO_REG16_BIT(CRC16_LENGTH, 0x00000242u, __READ_WRITE, crc16_length_bf_t);

/* ------------------------------------------------------------------------------ */
/* EXPECTED_CRC register ... */

/* EXPECTED_CRC bitfield type ... */

typedef struct {
  __REG16 crc : 16;       /* [15:0] */
} crc16_expected_crc_bf_t;

/* EXPECTED_CRC address mapping ... */

__IO_REG16_BIT(CRC16_EXPECTED_CRC, 0x00000244u, __READ_WRITE, crc16_expected_crc_bf_t);

/* ------------------------------------------------------------------------------ */
/* CONFIG register ... */

/* CONFIG bitfield type ... */

typedef struct {
  __REG16 timing   :  3;       /* [2:0] */
  __REG16 keep_sum :  1;       /* [3] */
  __REG16          : 12;
} crc16_config_bf_t;

/* CONFIG single bit enum ... */

enum {
  E_CRC16_CONFIG_KEEP_SUM = 0x0008u
};

/* CONFIG address mapping ... */

__IO_REG16_BIT(CRC16_CONFIG, 0x00000246u, __READ_WRITE, crc16_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* STATUS register ... */

/* STATUS bitfield type ... */

typedef struct {
  __REG16 state :  2;       /* [1:0] */
  __REG16       : 14;
} crc16_status_bf_t;

/* STATUS address mapping ... */

__IO_REG16_BIT(CRC16_STATUS, 0x00000248u, __READ, crc16_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* CRC_SUM register ... */

/* CRC_SUM bitfield type ... */

typedef struct {
  __REG16 crc : 16;       /* [15:0] */
} crc16_crc_sum_bf_t;

/* CRC_SUM address mapping ... */

__IO_REG16_BIT(CRC16_CRC_SUM, 0x0000024Au, __READ_WRITE, crc16_crc_sum_bf_t);

/* ============================================================================== */
/* LIN_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* PHY_CONFIG register ... */

/* PHY_CONFIG bitfield type ... */

typedef struct {
  __REG16 lin_on :  1;       /* [0] */
  __REG16 lin_hs :  1;       /* [1] */
  __REG16        : 14;
} lin_ctrl_phy_config_bf_t;

/* PHY_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_PHY_CONFIG_LIN_ON = 0x0001u,
  E_LIN_CTRL_PHY_CONFIG_LIN_HS = 0x0002u
};

/* PHY_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_PHY_CONFIG, 0x00000280u, __READ_WRITE, lin_ctrl_phy_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_PHY_CONFIG register ... */

/* AA_PHY_CONFIG bitfield type ... */

typedef struct {
  __REG16 meas_in_mux   :  1;       /* [0] */
  __REG16 gain          :  2;       /* [2:1] */
  __REG16               :  2;
  __REG16 amp_select    :  1;       /* [5] */
  __REG16 cm_meas       :  1;       /* [6] */
  __REG16 ipu_connect_m :  1;       /* [7] */
  __REG16               :  4;
  __REG16 amp_on        :  1;       /* [12] */
  __REG16               :  2;
  __REG16 lock_gain     :  1;       /* [15] */
} lin_ctrl_aa_phy_config_bf_t;

/* AA_PHY_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_AA_PHY_CONFIG_MEAS_IN_MUX   = 0x0001u,
  E_LIN_CTRL_AA_PHY_CONFIG_AMP_SELECT    = 0x0020u,
  E_LIN_CTRL_AA_PHY_CONFIG_CM_MEAS       = 0x0040u,
  E_LIN_CTRL_AA_PHY_CONFIG_IPU_CONNECT_M = 0x0080u,
  E_LIN_CTRL_AA_PHY_CONFIG_AMP_ON        = 0x1000u,
  E_LIN_CTRL_AA_PHY_CONFIG_LOCK_GAIN     = 0x8000u
};

/* AA_PHY_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_PHY_CONFIG, 0x00000284u, __READ_WRITE, lin_ctrl_aa_phy_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_FSM_CONFIG register ... */

/* AA_FSM_CONFIG bitfield type ... */

typedef struct {
  __REG16 fsm_enable       :  1;       /* [0] */
  __REG16 addressed        :  1;       /* [1] */
  __REG16 restore_eob      :  1;       /* [2] */
  __REG16 st5_mid_wait     :  1;       /* [3] */
  __REG16 st4_sw_wait      :  1;       /* [4] */
  __REG16 disable_adc_trig :  1;       /* [5] */
  __REG16 fsm_type3        :  1;       /* [6] */
  __REG16 fsm_diff         :  1;       /* [7] */
  __REG16 rc_stop          :  1;       /* [8] */
  __REG16                  :  7;
} lin_ctrl_aa_fsm_config_bf_t;

/* AA_FSM_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_AA_FSM_CONFIG_FSM_ENABLE       = 0x0001u,
  E_LIN_CTRL_AA_FSM_CONFIG_ADDRESSED        = 0x0002u,
  E_LIN_CTRL_AA_FSM_CONFIG_RESTORE_EOB      = 0x0004u,
  E_LIN_CTRL_AA_FSM_CONFIG_ST5_MID_WAIT     = 0x0008u,
  E_LIN_CTRL_AA_FSM_CONFIG_ST4_SW_WAIT      = 0x0010u,
  E_LIN_CTRL_AA_FSM_CONFIG_DISABLE_ADC_TRIG = 0x0020u,
  E_LIN_CTRL_AA_FSM_CONFIG_FSM_TYPE3        = 0x0040u,
  E_LIN_CTRL_AA_FSM_CONFIG_FSM_DIFF         = 0x0080u,
  E_LIN_CTRL_AA_FSM_CONFIG_RC_STOP          = 0x0100u
};

/* AA_FSM_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_FSM_CONFIG, 0x00000286u, __READ_WRITE, lin_ctrl_aa_fsm_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_FSM_CONTROL register ... */

/* AA_FSM_CONTROL bitfield type ... */

typedef struct {
  __REG16 st4_sw_proceed :  1;       /* [0] */
  __REG16                : 15;
} lin_ctrl_aa_fsm_control_bf_t;

/* AA_FSM_CONTROL single bit enum ... */

enum {
  E_LIN_CTRL_AA_FSM_CONTROL_ST4_SW_PROCEED = 0x0001u
};

/* AA_FSM_CONTROL address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_FSM_CONTROL, 0x00000288u, __WRITE, lin_ctrl_aa_fsm_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_FSM_STATUS register ... */

/* AA_FSM_STATUS bitfield type ... */

typedef struct {
  __REG16 tbit_cnt           :  4;       /* [3:0] */
  __REG16 fsm_state          :  4;       /* [7:4] */
  __REG16 aa_result          :  1;       /* [8] */
  __REG16 adc_idle           :  1;       /* [9] */
  __REG16 adc_valid          :  1;       /* [10] */
  __REG16 offset_cal         :  1;       /* [11] */
  __REG16 rc_detected        :  1;       /* [12] */
  __REG16 rc_detected_presel :  1;       /* [13] */
  __REG16                    :  2;
} lin_ctrl_aa_fsm_status_bf_t;

/* AA_FSM_STATUS single bit enum ... */

enum {
  E_LIN_CTRL_AA_FSM_STATUS_AA_RESULT          = 0x0100u,
  E_LIN_CTRL_AA_FSM_STATUS_ADC_IDLE           = 0x0200u,
  E_LIN_CTRL_AA_FSM_STATUS_ADC_VALID          = 0x0400u,
  E_LIN_CTRL_AA_FSM_STATUS_OFFSET_CAL         = 0x0800u,
  E_LIN_CTRL_AA_FSM_STATUS_RC_DETECTED        = 0x1000u,
  E_LIN_CTRL_AA_FSM_STATUS_RC_DETECTED_PRESEL = 0x2000u
};

/* AA_FSM_STATUS address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_FSM_STATUS, 0x0000028Au, __READ, lin_ctrl_aa_fsm_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_MODE_DEFAULT register ... */

/* AA_MODE_DEFAULT bitfield type ... */

typedef struct {
  __REG16 lin_pu_e     :  1;       /* [0] */
  __REG16 lin_ipu_e    :  1;       /* [1] */
  __REG16 lin_ipu_sel  :  7;       /* [8:2] */
  __REG16 lin_ipu_ramp :  1;       /* [9] */
  __REG16 oversampling :  2;       /* [11:10] */
  __REG16              :  3;
  __REG16 filter       :  1;       /* [15] */
} lin_ctrl_aa_mode_default_bf_t;

/* AA_MODE_DEFAULT single bit enum ... */

enum {
  E_LIN_CTRL_AA_MODE_DEFAULT_LIN_PU_E     = 0x0001u,
  E_LIN_CTRL_AA_MODE_DEFAULT_LIN_IPU_E    = 0x0002u,
  E_LIN_CTRL_AA_MODE_DEFAULT_LIN_IPU_RAMP = 0x0200u,
  E_LIN_CTRL_AA_MODE_DEFAULT_FILTER       = 0x8000u
};

/* AA_MODE_DEFAULT address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_MODE_DEFAULT, 0x0000028Cu, __READ_WRITE, lin_ctrl_aa_mode_default_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_MODE_OFFSET register ... */

/* AA_MODE_OFFSET bitfield type ... */

typedef struct {
  __REG16 lin_pu_e     :  1;       /* [0] */
  __REG16 lin_ipu_e    :  1;       /* [1] */
  __REG16 lin_ipu_sel  :  7;       /* [8:2] */
  __REG16 lin_ipu_ramp :  1;       /* [9] */
  __REG16 oversampling :  2;       /* [11:10] */
  __REG16              :  3;
  __REG16 filter       :  1;       /* [15] */
} lin_ctrl_aa_mode_offset_bf_t;

/* AA_MODE_OFFSET single bit enum ... */

enum {
  E_LIN_CTRL_AA_MODE_OFFSET_LIN_PU_E     = 0x0001u,
  E_LIN_CTRL_AA_MODE_OFFSET_LIN_IPU_E    = 0x0002u,
  E_LIN_CTRL_AA_MODE_OFFSET_LIN_IPU_RAMP = 0x0200u,
  E_LIN_CTRL_AA_MODE_OFFSET_FILTER       = 0x8000u
};

/* AA_MODE_OFFSET address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_MODE_OFFSET, 0x0000028Eu, __READ_WRITE, lin_ctrl_aa_mode_offset_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_MODE_PRESEL register ... */

/* AA_MODE_PRESEL bitfield type ... */

typedef struct {
  __REG16 lin_pu_e     :  1;       /* [0] */
  __REG16 lin_ipu_e    :  1;       /* [1] */
  __REG16 lin_ipu_sel  :  7;       /* [8:2] */
  __REG16 lin_ipu_ramp :  1;       /* [9] */
  __REG16 oversampling :  2;       /* [11:10] */
  __REG16              :  3;
  __REG16 filter       :  1;       /* [15] */
} lin_ctrl_aa_mode_presel_bf_t;

/* AA_MODE_PRESEL single bit enum ... */

enum {
  E_LIN_CTRL_AA_MODE_PRESEL_LIN_PU_E     = 0x0001u,
  E_LIN_CTRL_AA_MODE_PRESEL_LIN_IPU_E    = 0x0002u,
  E_LIN_CTRL_AA_MODE_PRESEL_LIN_IPU_RAMP = 0x0200u,
  E_LIN_CTRL_AA_MODE_PRESEL_FILTER       = 0x8000u
};

/* AA_MODE_PRESEL address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_MODE_PRESEL, 0x00000290u, __READ_WRITE, lin_ctrl_aa_mode_presel_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_MODE_SELECT register ... */

/* AA_MODE_SELECT bitfield type ... */

typedef struct {
  __REG16 lin_pu_e     :  1;       /* [0] */
  __REG16 lin_ipu_e    :  1;       /* [1] */
  __REG16 lin_ipu_sel  :  7;       /* [8:2] */
  __REG16 lin_ipu_ramp :  1;       /* [9] */
  __REG16 oversampling :  2;       /* [11:10] */
  __REG16              :  3;
  __REG16 filter       :  1;       /* [15] */
} lin_ctrl_aa_mode_select_bf_t;

/* AA_MODE_SELECT single bit enum ... */

enum {
  E_LIN_CTRL_AA_MODE_SELECT_LIN_PU_E     = 0x0001u,
  E_LIN_CTRL_AA_MODE_SELECT_LIN_IPU_E    = 0x0002u,
  E_LIN_CTRL_AA_MODE_SELECT_LIN_IPU_RAMP = 0x0200u,
  E_LIN_CTRL_AA_MODE_SELECT_FILTER       = 0x8000u
};

/* AA_MODE_SELECT address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_MODE_SELECT, 0x00000292u, __READ_WRITE, lin_ctrl_aa_mode_select_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_ADC_RESULT register ... */

/* AA_ADC_RESULT bitfield type ... */

typedef struct {
  __REG16 value : 12;       /* [11:0] */
  __REG16       :  4;
} lin_ctrl_aa_adc_result_bf_t;

/* AA_ADC_RESULT address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_ADC_RESULT, 0x00000294u, __READ_WRITE, lin_ctrl_aa_adc_result_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_I_DIFF_THD_1 register ... */

/* AA_I_DIFF_THD_1 bitfield type ... */

typedef struct {
  __REG16 i_diff_thd_1 : 12;       /* [11:0] */
  __REG16              :  4;
} lin_ctrl_aa_i_diff_thd_1_bf_t;

/* AA_I_DIFF_THD_1 address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_I_DIFF_THD_1, 0x00000296u, __READ_WRITE, lin_ctrl_aa_i_diff_thd_1_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_I_DIFF_THD_2 register ... */

/* AA_I_DIFF_THD_2 bitfield type ... */

typedef struct {
  __REG16 i_diff_thd_2 : 12;       /* [11:0] */
  __REG16              :  4;
} lin_ctrl_aa_i_diff_thd_2_bf_t;

/* AA_I_DIFF_THD_2 address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_I_DIFF_THD_2, 0x00000298u, __READ_WRITE, lin_ctrl_aa_i_diff_thd_2_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_I_SHUNT_1 register ... */

/* AA_I_SHUNT_1 bitfield type ... */

typedef struct {
  __REG16 i_shunt_1 : 12;       /* [11:0] */
  __REG16 password  :  4;       /* [15:12] */
} lin_ctrl_aa_i_shunt_1_bf_t;

/* AA_I_SHUNT_1 address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_I_SHUNT_1, 0x0000029Au, __READ_WRITE, lin_ctrl_aa_i_shunt_1_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_TYPE3_RAMP_CONFIG register ... */

/* AA_TYPE3_RAMP_CONFIG bitfield type ... */

typedef struct {
  __REG16 max_shunt_current :  7;       /* [6:0] */
  __REG16                   :  1;
  __REG16 max_steps         :  5;       /* [12:8] */
  __REG16                   :  2;
  __REG16 meas_only         :  1;       /* [15] */
} lin_ctrl_aa_type3_ramp_config_bf_t;

/* AA_TYPE3_RAMP_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_AA_TYPE3_RAMP_CONFIG_MEAS_ONLY         = 0x8000u
};

/* AA_TYPE3_RAMP_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_TYPE3_RAMP_CONFIG, 0x0000029Cu, __READ_WRITE, lin_ctrl_aa_type3_ramp_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_TYPE3_MEAS_CONFIG register ... */

/* AA_TYPE3_MEAS_CONFIG bitfield type ... */

typedef struct {
  __REG16 step_size : 12;       /* [11:0] */
  __REG16           :  3;
  __REG16 lock      :  1;       /* [15] */
} lin_ctrl_aa_type3_meas_config_bf_t;

/* AA_TYPE3_MEAS_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_AA_TYPE3_MEAS_CONFIG_LOCK      = 0x8000u
};

/* AA_TYPE3_MEAS_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_TYPE3_MEAS_CONFIG, 0x0000029Eu, __READ_WRITE, lin_ctrl_aa_type3_meas_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_TYPE3_STATUS register ... */

/* AA_TYPE3_STATUS bitfield type ... */

typedef struct {
  __REG16 current_i_sel :  7;       /* [6:0] */
  __REG16               :  1;
  __REG16 i_sel_stored  :  7;       /* [14:8] */
  __REG16               :  1;
} lin_ctrl_aa_type3_status_bf_t;

/* AA_TYPE3_STATUS address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_TYPE3_STATUS, 0x000002A0u, __READ, lin_ctrl_aa_type3_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_REVERSE_CURRENT register ... */

/* AA_REVERSE_CURRENT bitfield type ... */

typedef struct {
  __REG16 threshold : 12;       /* [11:0] */
  __REG16           :  3;
  __REG16 enable    :  1;       /* [15] */
} lin_ctrl_aa_reverse_current_bf_t;

/* AA_REVERSE_CURRENT single bit enum ... */

enum {
  E_LIN_CTRL_AA_REVERSE_CURRENT_ENABLE    = 0x8000u
};

/* AA_REVERSE_CURRENT address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_REVERSE_CURRENT, 0x000002A2u, __READ_WRITE, lin_ctrl_aa_reverse_current_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_OFFSET_GOAL register ... */

/* AA_OFFSET_GOAL bitfield type ... */

typedef struct {
  __REG16 goal : 12;       /* [11:0] */
  __REG16      :  3;
  __REG16 lock :  1;       /* [15] */
} lin_ctrl_aa_offset_goal_bf_t;

/* AA_OFFSET_GOAL single bit enum ... */

enum {
  E_LIN_CTRL_AA_OFFSET_GOAL_LOCK = 0x8000u
};

/* AA_OFFSET_GOAL address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_OFFSET_GOAL, 0x000002A4u, __READ_WRITE, lin_ctrl_aa_offset_goal_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_OFFSET_CONFIG register ... */

/* AA_OFFSET_CONFIG bitfield type ... */

typedef struct {
  __REG16 enable                     :  1;       /* [0] */
  __REG16 oversampling               :  2;       /* [2:1] */
  __REG16                            :  2;
  __REG16 filter                     :  1;       /* [5] */
  __REG16                            :  1;
  __REG16 trigger_offset_calibration :  8;       /* [14:7] */
  __REG16 lock                       :  1;       /* [15] */
} lin_ctrl_aa_offset_config_bf_t;

/* AA_OFFSET_CONFIG single bit enum ... */

enum {
  E_LIN_CTRL_AA_OFFSET_CONFIG_ENABLE                     = 0x0001u,
  E_LIN_CTRL_AA_OFFSET_CONFIG_FILTER                     = 0x0020u,
  E_LIN_CTRL_AA_OFFSET_CONFIG_LOCK                       = 0x8000u
};

/* AA_OFFSET_CONFIG address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_OFFSET_CONFIG, 0x000002A6u, __READ_WRITE, lin_ctrl_aa_offset_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* TRIM_AA_TYPE1 register ... */

/* TRIM_AA_TYPE1 bitfield type ... */

typedef struct {
  __REG16 cm_error     :  5;       /* [4:0] */
  __REG16              :  3;
  __REG16 offset_error :  5;       /* [12:8] */
  __REG16              :  2;
  __REG16 lock         :  1;       /* [15] */
} lin_ctrl_trim_aa_type1_bf_t;

/* TRIM_AA_TYPE1 single bit enum ... */

enum {
  E_LIN_CTRL_TRIM_AA_TYPE1_LOCK         = 0x8000u
};

/* TRIM_AA_TYPE1 address mapping ... */

__IO_REG16_BIT(LIN_CTRL_TRIM_AA_TYPE1, 0x000002A8u, __READ_WRITE, lin_ctrl_trim_aa_type1_bf_t);

/* ------------------------------------------------------------------------------ */
/* TRIM_AA_TYPE2_3 register ... */

/* TRIM_AA_TYPE2_3 bitfield type ... */

typedef struct {
  __REG16 cm_error     :  5;       /* [4:0] */
  __REG16              :  3;
  __REG16 offset_error :  5;       /* [12:8] */
  __REG16              :  2;
  __REG16 lock         :  1;       /* [15] */
} lin_ctrl_trim_aa_type2_3_bf_t;

/* TRIM_AA_TYPE2_3 single bit enum ... */

enum {
  E_LIN_CTRL_TRIM_AA_TYPE2_3_LOCK         = 0x8000u
};

/* TRIM_AA_TYPE2_3 address mapping ... */

__IO_REG16_BIT(LIN_CTRL_TRIM_AA_TYPE2_3, 0x000002AAu, __READ_WRITE, lin_ctrl_trim_aa_type2_3_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_REVERSE_CURRENT_PRESEL register ... */

/* AA_REVERSE_CURRENT_PRESEL bitfield type ... */

typedef struct {
  __REG16 threshold_presel : 12;       /* [11:0] */
  __REG16                  :  4;
} lin_ctrl_aa_reverse_current_presel_bf_t;

/* AA_REVERSE_CURRENT_PRESEL address mapping ... */

__IO_REG16_BIT(LIN_CTRL_AA_REVERSE_CURRENT_PRESEL, 0x000002ACu, __READ_WRITE, lin_ctrl_aa_reverse_current_presel_bf_t);

/* ------------------------------------------------------------------------------ */
/* LIN_CTRL_VERSION register ... */

/* LIN_CTRL_VERSION bitfield type ... */

typedef struct {
  __REG16 version         :  4;       /* [3:0] */
  __REG16                 :  4;
  __REG16 auto_addressing :  1;       /* [8] */
  __REG16                 :  7;
} lin_ctrl_lin_ctrl_version_bf_t;

/* LIN_CTRL_VERSION single bit enum ... */

enum {
  E_LIN_CTRL_LIN_CTRL_VERSION_AUTO_ADDRESSING = 0x0100u
};

/* LIN_CTRL_VERSION address mapping ... */

__IO_REG16_BIT(LIN_CTRL_LIN_CTRL_VERSION, 0x000002AEu, __READ, lin_ctrl_lin_ctrl_version_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16                                 :  1;
  __REG16 aa_finished                     :  1;       /* [1] */
  __REG16 aa_amp_timout_evt               :  1;       /* [2] */
  __REG16 aa_adc_timout_evt               :  1;       /* [3] */
  __REG16 aa_adc_valid_evt                :  1;       /* [4] */
  __REG16 aa_t_bit_evt                    :  1;       /* [5] */
  __REG16 aa_step_size_evt                :  1;       /* [6] */
  __REG16 aa_reverse_current_detected_evt :  1;       /* [7] */
  __REG16 aa_aborted                      :  1;       /* [8] */
  __REG16                                 :  7;
} lin_ctrl_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_LIN_CTRL_IRQ_STATUS_AA_FINISHED                     = 0x0002u,
  E_LIN_CTRL_IRQ_STATUS_AA_AMP_TIMOUT_EVT               = 0x0004u,
  E_LIN_CTRL_IRQ_STATUS_AA_ADC_TIMOUT_EVT               = 0x0008u,
  E_LIN_CTRL_IRQ_STATUS_AA_ADC_VALID_EVT                = 0x0010u,
  E_LIN_CTRL_IRQ_STATUS_AA_T_BIT_EVT                    = 0x0020u,
  E_LIN_CTRL_IRQ_STATUS_AA_STEP_SIZE_EVT                = 0x0040u,
  E_LIN_CTRL_IRQ_STATUS_AA_REVERSE_CURRENT_DETECTED_EVT = 0x0080u,
  E_LIN_CTRL_IRQ_STATUS_AA_ABORTED                      = 0x0100u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_STATUS, 0x000002B0u, __READ_WRITE, lin_ctrl_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask :  9;       /* [8:0] */
  __REG16      :  7;
} lin_ctrl_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_MASK, 0x000002B4u, __READ_WRITE, lin_ctrl_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} lin_ctrl_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_VENABLE, 0x000002B8u, __WRITE, lin_ctrl_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} lin_ctrl_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_VDISABLE, 0x000002BAu, __WRITE, lin_ctrl_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} lin_ctrl_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_VMAX, 0x000002BCu, __READ_WRITE, lin_ctrl_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} lin_ctrl_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(LIN_CTRL_IRQ_VNO, 0x000002BEu, __READ_WRITE, lin_ctrl_irq_vno_bf_t);

/* ============================================================================== */
/* JTAG_MASTER module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* IR register ... */

/* IR bitfield type ... */

typedef struct {
  __REG16 val :  8;       /* [7:0] */
  __REG16     :  8;
} jtag_master_ir_bf_t;

/* IR address mapping ... */

__IO_REG16_BIT(JTAG_MASTER_IR, 0x000002C0u, __READ_WRITE, jtag_master_ir_bf_t);

/* ------------------------------------------------------------------------------ */
/* DR register ... */

/* DR bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} jtag_master_dr_bf_t;

/* DR address mapping ... */

__IO_REG16_BIT(JTAG_MASTER_DR, 0x000002C2u, __READ_WRITE, jtag_master_dr_bf_t);

/* ============================================================================== */
/* ADC_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* REG_TRIGGER register ... */

/* REG_TRIGGER bitfield type ... */

typedef struct {
  __REG16 trigger : 12;       /* [11:0] */
  __REG16         :  4;
} adc_ctrl_reg_trigger_bf_t;

/* REG_TRIGGER address mapping ... */

__IO_REG16_BIT(ADC_CTRL_REG_TRIGGER, 0x00000300u, __WRITE, adc_ctrl_reg_trigger_bf_t);

/* ------------------------------------------------------------------------------ */
/* PWM0_CONFIG register ... */

/* PWM0_CONFIG bitfield type ... */

typedef struct {
  __REG16 config : 12;       /* [11:0] */
  __REG16        :  4;
} adc_ctrl_pwm0_config_bf_t;

/* PWM0_CONFIG address mapping ... */

__IO_REG16_BIT(ADC_CTRL_PWM0_CONFIG, 0x00000302u, __READ_WRITE, adc_ctrl_pwm0_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* PWM1_CONFIG register ... */

/* PWM1_CONFIG bitfield type ... */

typedef struct {
  __REG16 config : 12;       /* [11:0] */
  __REG16        :  4;
} adc_ctrl_pwm1_config_bf_t;

/* PWM1_CONFIG address mapping ... */

__IO_REG16_BIT(ADC_CTRL_PWM1_CONFIG, 0x00000304u, __READ_WRITE, adc_ctrl_pwm1_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* PWM2_CONFIG register ... */

/* PWM2_CONFIG bitfield type ... */

typedef struct {
  __REG16 config : 12;       /* [11:0] */
  __REG16        :  4;
} adc_ctrl_pwm2_config_bf_t;

/* PWM2_CONFIG address mapping ... */

__IO_REG16_BIT(ADC_CTRL_PWM2_CONFIG, 0x00000306u, __READ_WRITE, adc_ctrl_pwm2_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* PWM3_CONFIG register ... */

/* PWM3_CONFIG bitfield type ... */

typedef struct {
  __REG16 config : 12;       /* [11:0] */
  __REG16        :  4;
} adc_ctrl_pwm3_config_bf_t;

/* PWM3_CONFIG address mapping ... */

__IO_REG16_BIT(ADC_CTRL_PWM3_CONFIG, 0x00000308u, __READ_WRITE, adc_ctrl_pwm3_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* CONTROL register ... */

/* CONTROL bitfield type ... */

typedef struct {
  __REG16 cyclic       :  1;       /* [0] */
  __REG16 aa_block_dis :  1;       /* [1] */
  __REG16              : 14;
} adc_ctrl_control_bf_t;

/* CONTROL single bit enum ... */

enum {
  E_ADC_CTRL_CONTROL_CYCLIC       = 0x0001u,
  E_ADC_CTRL_CONTROL_AA_BLOCK_DIS = 0x0002u
};

/* CONTROL address mapping ... */

__IO_REG16_BIT(ADC_CTRL_CONTROL, 0x0000030Au, __READ_WRITE, adc_ctrl_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* STATUS register ... */

/* STATUS bitfield type ... */

typedef struct {
  __REG16 valid : 12;       /* [11:0] */
  __REG16       :  4;
} adc_ctrl_status_bf_t;

/* STATUS address mapping ... */

__IO_REG16_BIT(ADC_CTRL_STATUS, 0x0000030Cu, __READ, adc_ctrl_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADC_POWER register ... */

/* ADC_POWER bitfield type ... */

typedef struct {
  __REG16 power_always_on :  1;       /* [0] */
  __REG16                 :  7;
  __REG16 cdiv            :  2;       /* [9:8] */
  __REG16                 :  2;
  __REG16 tsamp           :  2;       /* [13:12] */
  __REG16                 :  2;
} adc_ctrl_adc_power_bf_t;

/* ADC_POWER single bit enum ... */

enum {
  E_ADC_CTRL_ADC_POWER_POWER_ALWAYS_ON = 0x0001u
};

/* ADC_POWER address mapping ... */

__IO_REG16_BIT(ADC_CTRL_ADCF12_CONFIG, 0x0000030Eu, __READ_WRITE, adc_ctrl_adc_power_bf_t);

/* ------------------------------------------------------------------------------ */
/* SAMPLE_EXT register ... */

/* SAMPLE_EXT bitfield type ... */

typedef struct {
  __REG16 cycles : 10;       /* [9:0] */
  __REG16        :  6;
} adc_ctrl_sample_ext_bf_t;

/* SAMPLE_EXT address mapping ... */

__IO_REG16_BIT(ADC_CTRL_SAMPLE_EXT, 0x00000310u, __READ_WRITE, adc_ctrl_sample_ext_bf_t);

/* ------------------------------------------------------------------------------ */
/* SAMPLE_EXT_VT register ... */

/* SAMPLE_EXT_VT bitfield type ... */

typedef struct {
  __REG16 cycles : 10;       /* [9:0] */
  __REG16        :  6;
} adc_ctrl_sample_ext_vt_bf_t;

/* SAMPLE_EXT_VT address mapping ... */

__IO_REG16_BIT(ADC_CTRL_SAMPLE_EXT_VT, 0x00000312u, __READ_WRITE, adc_ctrl_sample_ext_vt_bf_t);

/* ------------------------------------------------------------------------------ */
/* DMA_BASE_ADDR register ... */

/* DMA_BASE_ADDR bitfield type ... */

typedef struct {
  __REG16 base_addr : 16;       /* [15:0] */
} adc_ctrl_dma_base_addr_bf_t;

/* DMA_BASE_ADDR address mapping ... */

__IO_REG16_BIT(ADC_CTRL_DMA_BASE_ADDR, 0x00000314u, __READ_WRITE, adc_ctrl_dma_base_addr_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADC_OFFSET register ... */

/* ADC_OFFSET bitfield type ... */

typedef struct {
  __REG16 val : 13;       /* [12:0] */
  __REG16     :  3;
} adc_ctrl_adc_offset_bf_t;

/* ADC_OFFSET address mapping ... */

__IO_REG16_BIT(ADC_CTRL_ADC_OFFSET, 0x00000316u, __READ_WRITE, adc_ctrl_adc_offset_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADC_GAIN register ... */

/* ADC_GAIN bitfield type ... */

typedef struct {
  __REG16 val : 13;       /* [12:0] */
  __REG16     :  3;
} adc_ctrl_adc_gain_bf_t;

/* ADC_GAIN address mapping ... */

__IO_REG16_BIT(ADC_CTRL_ADC_GAIN, 0x00000318u, __READ_WRITE, adc_ctrl_adc_gain_bf_t);

/* ------------------------------------------------------------------------------ */
/* SAMPLE_EXT_AA register ... */

/* SAMPLE_EXT_AA bitfield type ... */

typedef struct {
  __REG16 cycles : 10;       /* [9:0] */
  __REG16        :  6;
} adc_ctrl_sample_ext_aa_bf_t;

/* SAMPLE_EXT_AA address mapping ... */

__IO_REG16_BIT(ADC_CTRL_SAMPLE_EXT_AA, 0x0000031Au, __READ_WRITE, adc_ctrl_sample_ext_aa_bf_t);

/* ------------------------------------------------------------------------------ */
/* MUX_OFFSET register ... */

/* MUX_OFFSET bitfield type ... */

typedef struct {
  __REG16 offset     :  5;       /* [4:0] */
  __REG16 static_sel :  1;       /* [5] */
  __REG16            : 10;
} adc_ctrl_mux_offset_bf_t;

/* MUX_OFFSET single bit enum ... */

enum {
  E_ADC_CTRL_MUX_OFFSET_STATIC_SEL = 0x0020u
};

/* MUX_OFFSET address mapping ... */

__IO_REG16_BIT(ADC_CTRL_MUX_OFFSET, 0x0000031Cu, __READ_WRITE, adc_ctrl_mux_offset_bf_t);

/* ------------------------------------------------------------------------------ */
/* TRIGGER_DELAY registers ... */

/* TRIGGER_DELAY bitfield type ... */

typedef struct {
  __REG16 delay : 16;       /* [15:0] */
} adc_ctrl_trigger_delay_bf_t;

/* TRIGGER_DELAY address mapping ... */

__IO_REG16_BIT(ADC_CTRL_TRIGGER_DELAY0, 0x00000320u, __READ_WRITE, adc_ctrl_trigger_delay_bf_t);
__IO_REG16_BIT(ADC_CTRL_TRIGGER_DELAY1, 0x00000322u, __READ_WRITE, adc_ctrl_trigger_delay_bf_t);
__IO_REG16_BIT(ADC_CTRL_TRIGGER_DELAY2, 0x00000324u, __READ_WRITE, adc_ctrl_trigger_delay_bf_t);
__IO_REG16_BIT(ADC_CTRL_TRIGGER_DELAY3, 0x00000326u, __READ_WRITE, adc_ctrl_trigger_delay_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 evt_update : 12;       /* [11:0] */
  __REG16 ready      :  1;       /* [12] */
  __REG16 blocked    :  1;       /* [13] */
  __REG16            :  2;
} adc_ctrl_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_ADC_CTRL_IRQ_STATUS_READY      = 0x1000u,
  E_ADC_CTRL_IRQ_STATUS_BLOCKED    = 0x2000u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_STATUS, 0x00000330u, __READ_WRITE, adc_ctrl_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask : 14;       /* [13:0] */
  __REG16      :  2;
} adc_ctrl_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_MASK, 0x00000334u, __READ_WRITE, adc_ctrl_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} adc_ctrl_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_VENABLE, 0x00000338u, __WRITE, adc_ctrl_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} adc_ctrl_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_VDISABLE, 0x0000033Au, __WRITE, adc_ctrl_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} adc_ctrl_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_VMAX, 0x0000033Cu, __READ_WRITE, adc_ctrl_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} adc_ctrl_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(ADC_CTRL_IRQ_VNO, 0x0000033Eu, __READ_WRITE, adc_ctrl_irq_vno_bf_t);

/* ============================================================================== */
/* PWM module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* CONTROL register ... */

/* CONTROL bitfield type ... */

typedef struct {
  __REG16 pwm_0_enable    :  1;       /* [0] */
  __REG16 pwm_1_enable    :  1;       /* [1] */
  __REG16 pwm_2_enable    :  1;       /* [2] */
  __REG16 pwm_3_enable    :  1;       /* [3] */
  __REG16 driver_0_enable :  1;       /* [4] */
  __REG16 driver_1_enable :  1;       /* [5] */
  __REG16 driver_2_enable :  1;       /* [6] */
  __REG16 driver_3_enable :  1;       /* [7] */
  __REG16 pull            :  5;       /* [12:8] */
  __REG16                 :  3;
} pwm_control_bf_t;

/* CONTROL single bit enum ... */

enum {
  E_PWM_CONTROL_PWM_0_ENABLE    = 0x0001u,
  E_PWM_CONTROL_PWM_1_ENABLE    = 0x0002u,
  E_PWM_CONTROL_PWM_2_ENABLE    = 0x0004u,
  E_PWM_CONTROL_PWM_3_ENABLE    = 0x0008u,
  E_PWM_CONTROL_DRIVER_0_ENABLE = 0x0010u,
  E_PWM_CONTROL_DRIVER_1_ENABLE = 0x0020u,
  E_PWM_CONTROL_DRIVER_2_ENABLE = 0x0040u,
  E_PWM_CONTROL_DRIVER_3_ENABLE = 0x0080u
};

/* CONTROL address mapping ... */

__IO_REG16_BIT(PWM_CONTROL, 0x00000340u, __READ_WRITE, pwm_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* DRIVER register ... */

/* DRIVER bitfield type ... */

typedef struct {
  __REG16 led0_current :  4;       /* [3:0] */
  __REG16 led1_current :  4;       /* [7:4] */
  __REG16 led2_current :  4;       /* [11:8] */
  __REG16 led3_current :  4;       /* [15:12] */
} pwm_driver_bf_t;

/* DRIVER address mapping ... */

__IO_REG16_BIT(PWM_DRIVER, 0x00000342u, __READ_WRITE, pwm_driver_bf_t);

/* ------------------------------------------------------------------------------ */
/* STATUS register ... */

/* STATUS bitfield type ... */

typedef struct {
  __REG16 pwm0    :  1;       /* [0] */
  __REG16 pwm1    :  1;       /* [1] */
  __REG16 pwm2    :  1;       /* [2] */
  __REG16 pwm3    :  1;       /* [3] */
  __REG16 running :  1;       /* [4] */
  __REG16         : 11;
} pwm_status_bf_t;

/* STATUS single bit enum ... */

enum {
  E_PWM_STATUS_PWM0    = 0x0001u,
  E_PWM_STATUS_PWM1    = 0x0002u,
  E_PWM_STATUS_PWM2    = 0x0004u,
  E_PWM_STATUS_PWM3    = 0x0008u,
  E_PWM_STATUS_RUNNING = 0x0010u
};

/* STATUS address mapping ... */

__IO_REG16_BIT(PWM_STATUS, 0x00000344u, __READ, pwm_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* POLARITY register ... */

/* POLARITY bitfield type ... */

typedef struct {
  __REG16 pwm_polarity :  4;       /* [3:0] */
  __REG16 pwm_term     :  4;       /* [7:4] */
  __REG16 pwm_zero     :  4;       /* [11:8] */
  __REG16              :  4;
} pwm_polarity_bf_t;

/* POLARITY address mapping ... */

__IO_REG16_BIT(PWM_POLARITY, 0x00000346u, __READ_WRITE, pwm_polarity_bf_t);

/* ------------------------------------------------------------------------------ */
/* PRESCALER registers ... */

/* PRESCALER bitfield type ... */

typedef struct {
  __REG16 val :  8;       /* [7:0] */
  __REG16     :  8;
} pwm_prescaler_bf_t;

/* PRESCALER address mapping ... */

__IO_REG16_BIT(PWM_PRESCALER0, 0x00000348u, __READ_WRITE, pwm_prescaler_bf_t);
__IO_REG16_BIT(PWM_PRESCALER1, 0x0000034Au, __READ_WRITE, pwm_prescaler_bf_t);
__IO_REG16_BIT(PWM_PRESCALER2, 0x0000034Cu, __READ_WRITE, pwm_prescaler_bf_t);
__IO_REG16_BIT(PWM_PRESCALER3, 0x0000034Eu, __READ_WRITE, pwm_prescaler_bf_t);

/* ------------------------------------------------------------------------------ */
/* PERIOD registers ... */

/* PERIOD bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} pwm_period_bf_t;

/* PERIOD address mapping ... */

__IO_REG16_BIT(PWM_PERIOD0, 0x00000350u, __READ_WRITE, pwm_period_bf_t);
__IO_REG16_BIT(PWM_PERIOD1, 0x00000358u, __READ_WRITE, pwm_period_bf_t);
__IO_REG16_BIT(PWM_PERIOD2, 0x00000360u, __READ_WRITE, pwm_period_bf_t);
__IO_REG16_BIT(PWM_PERIOD3, 0x00000368u, __READ_WRITE, pwm_period_bf_t);

/* ------------------------------------------------------------------------------ */
/* START registers ... */

/* START bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} pwm_start_bf_t;

/* START address mapping ... */

__IO_REG16_BIT(PWM_START0, 0x00000352u, __READ_WRITE, pwm_start_bf_t);
__IO_REG16_BIT(PWM_START1, 0x0000035Au, __READ_WRITE, pwm_start_bf_t);
__IO_REG16_BIT(PWM_START2, 0x00000362u, __READ_WRITE, pwm_start_bf_t);
__IO_REG16_BIT(PWM_START3, 0x0000036Au, __READ_WRITE, pwm_start_bf_t);

/* ------------------------------------------------------------------------------ */
/* STOP registers ... */

/* STOP bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} pwm_stop_bf_t;

/* STOP address mapping ... */

__IO_REG16_BIT(PWM_STOP0, 0x00000354u, __READ_WRITE, pwm_stop_bf_t);
__IO_REG16_BIT(PWM_STOP1, 0x0000035Cu, __READ_WRITE, pwm_stop_bf_t);
__IO_REG16_BIT(PWM_STOP2, 0x00000364u, __READ_WRITE, pwm_stop_bf_t);
__IO_REG16_BIT(PWM_STOP3, 0x0000036Cu, __READ_WRITE, pwm_stop_bf_t);

/* ------------------------------------------------------------------------------ */
/* COUNTER registers ... */

/* COUNTER bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} pwm_counter_bf_t;

/* COUNTER address mapping ... */

__IO_REG16_BIT(PWM_COUNTER0, 0x00000356u, __READ, pwm_counter_bf_t);
__IO_REG16_BIT(PWM_COUNTER1, 0x0000035Eu, __READ, pwm_counter_bf_t);
__IO_REG16_BIT(PWM_COUNTER2, 0x00000366u, __READ, pwm_counter_bf_t);
__IO_REG16_BIT(PWM_COUNTER3, 0x0000036Eu, __READ, pwm_counter_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 evt_period0 :  1;       /* [0] */
  __REG16 evt_start0  :  1;       /* [1] */
  __REG16 evt_stop0   :  1;       /* [2] */
  __REG16 evt_period1 :  1;       /* [3] */
  __REG16 evt_start1  :  1;       /* [4] */
  __REG16 evt_stop1   :  1;       /* [5] */
  __REG16 evt_period2 :  1;       /* [6] */
  __REG16 evt_start2  :  1;       /* [7] */
  __REG16 evt_stop2   :  1;       /* [8] */
  __REG16 evt_period3 :  1;       /* [9] */
  __REG16 evt_start3  :  1;       /* [10] */
  __REG16 evt_stop3   :  1;       /* [11] */
  __REG16             :  4;
} pwm_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_PWM_IRQ_STATUS_EVT_PERIOD0 = 0x0001u,
  E_PWM_IRQ_STATUS_EVT_START0  = 0x0002u,
  E_PWM_IRQ_STATUS_EVT_STOP0   = 0x0004u,
  E_PWM_IRQ_STATUS_EVT_PERIOD1 = 0x0008u,
  E_PWM_IRQ_STATUS_EVT_START1  = 0x0010u,
  E_PWM_IRQ_STATUS_EVT_STOP1   = 0x0020u,
  E_PWM_IRQ_STATUS_EVT_PERIOD2 = 0x0040u,
  E_PWM_IRQ_STATUS_EVT_START2  = 0x0080u,
  E_PWM_IRQ_STATUS_EVT_STOP2   = 0x0100u,
  E_PWM_IRQ_STATUS_EVT_PERIOD3 = 0x0200u,
  E_PWM_IRQ_STATUS_EVT_START3  = 0x0400u,
  E_PWM_IRQ_STATUS_EVT_STOP3   = 0x0800u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(PWM_IRQ_STATUS, 0x00000370u, __READ_WRITE, pwm_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 mask : 12;       /* [11:0] */
  __REG16      :  4;
} pwm_irq_mask_bf_t;

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(PWM_IRQ_MASK, 0x00000374u, __READ_WRITE, pwm_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} pwm_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(PWM_IRQ_VENABLE, 0x00000378u, __WRITE, pwm_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} pwm_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(PWM_IRQ_VDISABLE, 0x0000037Au, __WRITE, pwm_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  4;       /* [3:0] */
  __REG16      : 12;
} pwm_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(PWM_IRQ_VMAX, 0x0000037Cu, __READ_WRITE, pwm_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  4;       /* [3:0] */
  __REG16     : 12;
} pwm_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(PWM_IRQ_VNO, 0x0000037Eu, __READ_WRITE, pwm_irq_vno_bf_t);

/* ============================================================================== */
/* SFL_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* VERSION register ... */

/* VERSION bitfield type ... */

typedef struct {
  __REG16 version : 16;       /* [15:0] */
} sfl_ctrl_version_bf_t;

/* VERSION address mapping ... */

__IO_REG16_BIT(SFL_CTRL_VERSION, 0x00000380u, __READ, sfl_ctrl_version_bf_t);

/* ------------------------------------------------------------------------------ */
/* MODE_REQUEST register ... */

/* MODE_REQUEST bitfield type ... */

typedef struct {
  __REG16 mode           :  4;       /* [3:0] */
  __REG16 cookie_timeout :  3;       /* [6:4] */
  __REG16                :  9;
} sfl_ctrl_mode_request_bf_t;

/* MODE_REQUEST address mapping ... */

__IO_REG16_BIT(SFL_CTRL_MODE_REQUEST, 0x00000382u, __READ_WRITE, sfl_ctrl_mode_request_bf_t);

/* ------------------------------------------------------------------------------ */
/* MODE_STATUS register ... */

/* MODE_STATUS bitfield type ... */

typedef struct {
  __REG16 mode            :  4;       /* [3:0] */
  __REG16 busy            :  1;       /* [4] */
  __REG16 busy_error      :  1;       /* [5] */
  __REG16 cookie_error    :  1;       /* [6] */
  __REG16 rstbusy_timeout :  1;       /* [7] */
  __REG16 supply_error    :  1;       /* [8] */
  __REG16 cookie_timeout  :  1;       /* [9] */
  __REG16 active_prot_sel :  2;       /* [11:10] */
  __REG16                 :  4;
} sfl_ctrl_mode_status_bf_t;

/* MODE_STATUS single bit enum ... */

enum {
  E_SFL_CTRL_MODE_STATUS_BUSY            = 0x0010u,
  E_SFL_CTRL_MODE_STATUS_BUSY_ERROR      = 0x0020u,
  E_SFL_CTRL_MODE_STATUS_COOKIE_ERROR    = 0x0040u,
  E_SFL_CTRL_MODE_STATUS_RSTBUSY_TIMEOUT = 0x0080u,
  E_SFL_CTRL_MODE_STATUS_SUPPLY_ERROR    = 0x0100u,
  E_SFL_CTRL_MODE_STATUS_COOKIE_TIMEOUT  = 0x0200u
};

/* MODE_STATUS address mapping ... */

__IO_REG16_BIT(SFL_CTRL_MODE_STATUS, 0x00000384u, __READ, sfl_ctrl_mode_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* COOKIE register ... */

/* COOKIE bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} sfl_ctrl_cookie_bf_t;

/* COOKIE address mapping ... */

__IO_REG16_BIT(SFL_CTRL_COOKIE, 0x00000386u, __WRITE, sfl_ctrl_cookie_bf_t);

/* ------------------------------------------------------------------------------ */
/* READ_TIMING register ... */

/* READ_TIMING bitfield type ... */

typedef struct {
  __REG16 initial_fuse_read :  4;       /* [3:0] */
  __REG16 fuse_read         :  4;       /* [7:4] */
  __REG16 main_read         :  4;       /* [11:8] */
  __REG16                   :  4;
} sfl_ctrl_read_timing_bf_t;

/* READ_TIMING address mapping ... */

__IO_REG16_BIT(SFL_CTRL_READ_TIMING, 0x00000388u, __READ_WRITE, sfl_ctrl_read_timing_bf_t);

/* ------------------------------------------------------------------------------ */
/* VERIFY_CONFIG register ... */

/* VERIFY_CONFIG bitfield type ... */

typedef struct {
  __REG16 verify0_osvrd :  3;       /* [2:0] */
  __REG16 verify1_osvrd :  3;       /* [5:3] */
  __REG16 avref_trim    :  6;       /* [11:6] */
  __REG16               :  4;
} sfl_ctrl_verify_config_bf_t;

/* VERIFY_CONFIG address mapping ... */

__IO_REG16_BIT(SFL_CTRL_VERIFY_CONFIG, 0x0000038Au, __READ_WRITE, sfl_ctrl_verify_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* DCT_L register ... */

/* DCT_L bitfield type ... */

typedef struct {
  __REG16 os_vpp_ers :  4;       /* [3:0] */
  __REG16 os_vpp_pgm :  4;       /* [7:4] */
  __REG16 os_vrd     :  3;       /* [10:8] */
  __REG16 os_pgm_wl  :  2;       /* [12:11] */
  __REG16 os_sa_lc   :  3;       /* [15:13] */
} sfl_ctrl_dct_l_bf_t;

/* DCT_L address mapping ... */

__IO_REG16_BIT(SFL_CTRL_DCT_L, 0x0000038Cu, __READ_WRITE, sfl_ctrl_dct_l_bf_t);

/* ------------------------------------------------------------------------------ */
/* DCT_H register ... */

/* DCT_H bitfield type ... */

typedef struct {
  __REG16 os_sa_lat :  4;       /* [3:0] */
  __REG16 os_sa_pre :  3;       /* [6:4] */
  __REG16 os_ivc    :  2;       /* [8:7] */
  __REG16 os_ipgm   :  3;       /* [11:9] */
  __REG16 os_wt_osc :  4;       /* [15:12] */
} sfl_ctrl_dct_h_bf_t;

/* DCT_H address mapping ... */

__IO_REG16_BIT(SFL_CTRL_DCT_H, 0x0000038Eu, __READ_WRITE, sfl_ctrl_dct_h_bf_t);

/* ------------------------------------------------------------------------------ */
/* WADDR_L register ... */

/* WADDR_L bitfield type ... */

typedef struct {
  __REG16 addr : 16;       /* [15:0] */
} sfl_ctrl_waddr_l_bf_t;

/* WADDR_L address mapping ... */

__IO_REG16_BIT(SFL_CTRL_WADDR_L, 0x00000390u, __READ_WRITE, sfl_ctrl_waddr_l_bf_t);

/* ------------------------------------------------------------------------------ */
/* WADDR_H register ... */

/* WADDR_H bitfield type ... */

typedef struct {
  __REG16 addr :  4;       /* [3:0] */
  __REG16      : 12;
} sfl_ctrl_waddr_h_bf_t;

/* WADDR_H address mapping ... */

__IO_REG16_BIT(SFL_CTRL_WADDR_H, 0x00000392u, __READ_WRITE, sfl_ctrl_waddr_h_bf_t);

/* ------------------------------------------------------------------------------ */
/* WDATA registers ... */

/* WDATA bitfield type ... */

typedef struct {
  __REG16 data : 16;       /* [15:0] */
} sfl_ctrl_wdata_bf_t;

/* WDATA address mapping ... */

__IO_REG16_BIT(SFL_CTRL_WDATA_L, 0x00000394u, __READ_WRITE, sfl_ctrl_wdata_bf_t);
__IO_REG16_BIT(SFL_CTRL_WDATA_H, 0x00000396u, __READ_WRITE, sfl_ctrl_wdata_bf_t);

/* ------------------------------------------------------------------------------ */
/* COMMAND register ... */

/* COMMAND bitfield type ... */

typedef struct {
  __REG16 run :  1;       /* [0] */
  __REG16     : 15;
} sfl_ctrl_command_bf_t;

/* COMMAND single bit enum ... */

enum {
  E_SFL_CTRL_COMMAND_RUN = 0x0001u
};

/* COMMAND address mapping ... */

__IO_REG16_BIT(SFL_CTRL_COMMAND, 0x00000398u, __WRITE, sfl_ctrl_command_bf_t);

/* ------------------------------------------------------------------------------ */
/* COMMAND_STATUS register ... */

/* COMMAND_STATUS bitfield type ... */

typedef struct {
  __REG16 busy            :  1;       /* [0] */
  __REG16 wrbusy_timeout  :  1;       /* [1] */
  __REG16 protected_error :  1;       /* [2] */
  __REG16 mode_error      :  1;       /* [3] */
  __REG16 busy_error      :  1;       /* [4] */
  __REG16                 : 11;
} sfl_ctrl_command_status_bf_t;

/* COMMAND_STATUS single bit enum ... */

enum {
  E_SFL_CTRL_COMMAND_STATUS_BUSY            = 0x0001u,
  E_SFL_CTRL_COMMAND_STATUS_WRBUSY_TIMEOUT  = 0x0002u,
  E_SFL_CTRL_COMMAND_STATUS_PROTECTED_ERROR = 0x0004u,
  E_SFL_CTRL_COMMAND_STATUS_MODE_ERROR      = 0x0008u,
  E_SFL_CTRL_COMMAND_STATUS_BUSY_ERROR      = 0x0010u
};

/* COMMAND_STATUS address mapping ... */

__IO_REG16_BIT(SFL_CTRL_COMMAND_STATUS, 0x0000039Au, __READ, sfl_ctrl_command_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* BUSY_MEASUREMENT register ... */

/* BUSY_MEASUREMENT bitfield type ... */

typedef struct {
  __REG16 length : 16;       /* [15:0] */
} sfl_ctrl_busy_measurement_bf_t;

/* BUSY_MEASUREMENT address mapping ... */

__IO_REG16_BIT(SFL_CTRL_BUSY_MEASUREMENT, 0x0000039Cu, __READ, sfl_ctrl_busy_measurement_bf_t);

/* ------------------------------------------------------------------------------ */
/* PROTECT_ACCESS_SEL register ... */

/* PROTECT_ACCESS_SEL bitfield type ... */

typedef struct {
  __REG16 main_fine :  2;       /* [1:0] */
  __REG16 main      :  2;       /* [3:2] */
  __REG16           : 12;
} sfl_ctrl_protect_access_sel_bf_t;

/* PROTECT_ACCESS_SEL address mapping ... */

__IO_REG16_BIT(SFL_CTRL_PROTECT_ACCESS_SEL, 0x0000039Eu, __READ_WRITE, sfl_ctrl_protect_access_sel_bf_t);

/* ------------------------------------------------------------------------------ */
/* FUSE_PROTECT register ... */

/* FUSE_PROTECT bitfield type ... */

typedef struct {
  __REG16 dct   :  1;       /* [0] */
  __REG16 red   :  1;       /* [1] */
  __REG16 ldt0  :  1;       /* [2] */
  __REG16 ldt1  :  1;       /* [3] */
  __REG16 info0 :  1;       /* [4] */
  __REG16 info1 :  1;       /* [5] */
  __REG16 info2 :  1;       /* [6] */
  __REG16 info3 :  1;       /* [7] */
  __REG16       :  8;
} sfl_ctrl_fuse_protect_bf_t;

/* FUSE_PROTECT single bit enum ... */

enum {
  E_SFL_CTRL_FUSE_PROTECT_DCT   = 0x0001u,
  E_SFL_CTRL_FUSE_PROTECT_RED   = 0x0002u,
  E_SFL_CTRL_FUSE_PROTECT_LDT0  = 0x0004u,
  E_SFL_CTRL_FUSE_PROTECT_LDT1  = 0x0008u,
  E_SFL_CTRL_FUSE_PROTECT_INFO0 = 0x0010u,
  E_SFL_CTRL_FUSE_PROTECT_INFO1 = 0x0020u,
  E_SFL_CTRL_FUSE_PROTECT_INFO2 = 0x0040u,
  E_SFL_CTRL_FUSE_PROTECT_INFO3 = 0x0080u
};

/* FUSE_PROTECT address mapping ... */

__IO_REG16_BIT(SFL_CTRL_FUSE_PROTECT, 0x000003A0u, __READ_WRITE, sfl_ctrl_fuse_protect_bf_t);

/* ------------------------------------------------------------------------------ */
/* MAIN_PROTECT register ... */

/* MAIN_PROTECT bitfield type ... */

typedef struct {
  __REG16 lock : 16;       /* [15:0] */
} sfl_ctrl_main_protect_bf_t;

/* MAIN_PROTECT address mapping ... */

__IO_REG16_BIT(SFL_CTRL_MAIN_PROTECT, 0x000003A4u, __READ_WRITE, sfl_ctrl_main_protect_bf_t);

/* ------------------------------------------------------------------------------ */
/* MAIN_FINE_PROTECT register ... */

/* MAIN_FINE_PROTECT bitfield type ... */

typedef struct {
  __REG16 lock : 16;       /* [15:0] */
} sfl_ctrl_main_fine_protect_bf_t;

/* MAIN_FINE_PROTECT address mapping ... */

__IO_REG16_BIT(SFL_CTRL_MAIN_FINE_PROTECT, 0x000003A8u, __READ_WRITE, sfl_ctrl_main_fine_protect_bf_t);

/* ------------------------------------------------------------------------------ */
/* ECC_CONTROL register ... */

/* ECC_CONTROL bitfield type ... */

typedef struct {
  __REG16 swap_bus :  1;       /* [0] */
  __REG16          : 15;
} sfl_ctrl_ecc_control_bf_t;

/* ECC_CONTROL single bit enum ... */

enum {
  E_SFL_CTRL_ECC_CONTROL_SWAP_BUS = 0x0001u
};

/* ECC_CONTROL address mapping ... */

__IO_REG16_BIT(SFL_CTRL_ECC_CONTROL, 0x000003AAu, __READ_WRITE, sfl_ctrl_ecc_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* ECC_STATUS register ... */

/* ECC_STATUS bitfield type ... */

typedef struct {
  __REG16 ecc_sum        :  7;       /* [6:0] */
  __REG16                :  1;
  __REG16 corrected_accu :  1;       /* [8] */
  __REG16 failure_accu   :  1;       /* [9] */
  __REG16 corrected      :  1;       /* [10] */
  __REG16 failure        :  1;       /* [11] */
  __REG16                :  4;
} sfl_ctrl_ecc_status_bf_t;

/* ECC_STATUS single bit enum ... */

enum {
  E_SFL_CTRL_ECC_STATUS_CORRECTED_ACCU = 0x0100u,
  E_SFL_CTRL_ECC_STATUS_FAILURE_ACCU   = 0x0200u,
  E_SFL_CTRL_ECC_STATUS_CORRECTED      = 0x0400u,
  E_SFL_CTRL_ECC_STATUS_FAILURE        = 0x0800u
};

/* ECC_STATUS address mapping ... */

__IO_REG16_BIT(SFL_CTRL_ECC_STATUS, 0x000003ACu, __READ_WRITE, sfl_ctrl_ecc_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IP_STATUS register ... */

/* IP_STATUS bitfield type ... */

typedef struct {
  __REG16 rstbusy :  1;       /* [0] */
  __REG16 wrbusy  :  1;       /* [1] */
  __REG16 hvchk   :  1;       /* [2] */
  __REG16 hvenb   :  1;       /* [3] */
  __REG16 resetb  :  1;       /* [4] */
  __REG16         : 11;
} sfl_ctrl_ip_status_bf_t;

/* IP_STATUS single bit enum ... */

enum {
  E_SFL_CTRL_IP_STATUS_RSTBUSY = 0x0001u,
  E_SFL_CTRL_IP_STATUS_WRBUSY  = 0x0002u,
  E_SFL_CTRL_IP_STATUS_HVCHK   = 0x0004u,
  E_SFL_CTRL_IP_STATUS_HVENB   = 0x0008u,
  E_SFL_CTRL_IP_STATUS_RESETB  = 0x0010u
};

/* IP_STATUS address mapping ... */

__IO_REG16_BIT(SFL_CTRL_IP_STATUS, 0x000003AEu, __READ, sfl_ctrl_ip_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* ECC_TEST register ... */

/* ECC_TEST bitfield type ... */

typedef struct {
  __REG16 write_ecc_val  :  7;       /* [6:0] */
  __REG16 write_ecc_sel  :  1;       /* [7] */
  __REG16 correction_off :  1;       /* [8] */
  __REG16                :  7;
} sfl_ctrl_ecc_test_bf_t;

/* ECC_TEST single bit enum ... */

enum {
  E_SFL_CTRL_ECC_TEST_WRITE_ECC_SEL  = 0x0080u,
  E_SFL_CTRL_ECC_TEST_CORRECTION_OFF = 0x0100u
};

/* ECC_TEST address mapping ... */

__IO_REG16_BIT(SFL_CTRL_ECC_TEST, 0x000003B0u, __READ_WRITE, sfl_ctrl_ecc_test_bf_t);

/* ------------------------------------------------------------------------------ */
/* TEST_MODES register ... */

/* TEST_MODES bitfield type ... */

typedef struct {
  __REG16 tsmode    :  1;       /* [0] */
  __REG16 do_stop   :  1;       /* [1] */
  __REG16 use_waddr :  1;       /* [2] */
  __REG16 use_wdata :  1;       /* [3] */
  __REG16           : 12;
} sfl_ctrl_test_modes_bf_t;

/* TEST_MODES single bit enum ... */

enum {
  E_SFL_CTRL_TEST_MODES_TSMODE    = 0x0001u,
  E_SFL_CTRL_TEST_MODES_DO_STOP   = 0x0002u,
  E_SFL_CTRL_TEST_MODES_USE_WADDR = 0x0004u,
  E_SFL_CTRL_TEST_MODES_USE_WDATA = 0x0008u
};

/* TEST_MODES address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TEST_MODES, 0x000003B2u, __READ_WRITE, sfl_ctrl_test_modes_bf_t);

/* ------------------------------------------------------------------------------ */
/* TMR_OUT register ... */

/* TMR_OUT bitfield type ... */

typedef struct {
  __REG16 sel0 :  4;       /* [3:0] */
  __REG16 sel1 :  4;       /* [7:4] */
  __REG16      :  8;
} sfl_ctrl_tmr_out_bf_t;

/* TMR_OUT address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TMR_OUT, 0x000003B4u, __READ_WRITE, sfl_ctrl_tmr_out_bf_t);

/* ------------------------------------------------------------------------------ */
/* TMR_SEL register ... */

/* TMR_SEL bitfield type ... */

typedef struct {
  __REG16 resetb   :  1;       /* [0] */
  __REG16 prog     :  1;       /* [1] */
  __REG16 erase    :  1;       /* [2] */
  __REG16 sect     :  1;       /* [3] */
  __REG16 mas      :  1;       /* [4] */
  __REG16 hvenb    :  1;       /* [5] */
  __REG16 mcs      :  1;       /* [6] */
  __REG16 fuserd   :  1;       /* [7] */
  __REG16 ifen     :  1;       /* [8] */
  __REG16 ldten    :  2;       /* [10:9] */
  __REG16 reden    :  1;       /* [11] */
  __REG16 dcten    :  1;       /* [12] */
  __REG16 radckb   :  1;       /* [13] */
  __REG16 wadckb   :  1;       /* [14] */
  __REG16 tsregsel :  1;       /* [15] */
} sfl_ctrl_tmr_sel_bf_t;

/* TMR_SEL single bit enum ... */

enum {
  E_SFL_CTRL_TMR_SEL_RESETB   = 0x0001u,
  E_SFL_CTRL_TMR_SEL_PROG     = 0x0002u,
  E_SFL_CTRL_TMR_SEL_ERASE    = 0x0004u,
  E_SFL_CTRL_TMR_SEL_SECT     = 0x0008u,
  E_SFL_CTRL_TMR_SEL_MAS      = 0x0010u,
  E_SFL_CTRL_TMR_SEL_HVENB    = 0x0020u,
  E_SFL_CTRL_TMR_SEL_MCS      = 0x0040u,
  E_SFL_CTRL_TMR_SEL_FUSERD   = 0x0080u,
  E_SFL_CTRL_TMR_SEL_IFEN     = 0x0100u,
  E_SFL_CTRL_TMR_SEL_REDEN    = 0x0800u,
  E_SFL_CTRL_TMR_SEL_DCTEN    = 0x1000u,
  E_SFL_CTRL_TMR_SEL_RADCKB   = 0x2000u,
  E_SFL_CTRL_TMR_SEL_WADCKB   = 0x4000u,
  E_SFL_CTRL_TMR_SEL_TSREGSEL = 0x8000u
};

/* TMR_SEL address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TMR_SEL, 0x000003B6u, __READ_WRITE, sfl_ctrl_tmr_sel_bf_t);

/* ------------------------------------------------------------------------------ */
/* TMR_VAL register ... */

/* TMR_VAL bitfield type ... */

typedef struct {
  __REG16 resetb   :  1;       /* [0] */
  __REG16 prog     :  1;       /* [1] */
  __REG16 erase    :  1;       /* [2] */
  __REG16 sect     :  1;       /* [3] */
  __REG16 mas      :  1;       /* [4] */
  __REG16 hvenb    :  1;       /* [5] */
  __REG16 mcs      :  1;       /* [6] */
  __REG16 fuserd   :  1;       /* [7] */
  __REG16 ifen     :  1;       /* [8] */
  __REG16 ldten    :  2;       /* [10:9] */
  __REG16 reden    :  1;       /* [11] */
  __REG16 dcten    :  1;       /* [12] */
  __REG16 radckb   :  1;       /* [13] */
  __REG16 wadckb   :  1;       /* [14] */
  __REG16 tsregsel :  1;       /* [15] */
} sfl_ctrl_tmr_val_bf_t;

/* TMR_VAL single bit enum ... */

enum {
  E_SFL_CTRL_TMR_VAL_RESETB   = 0x0001u,
  E_SFL_CTRL_TMR_VAL_PROG     = 0x0002u,
  E_SFL_CTRL_TMR_VAL_ERASE    = 0x0004u,
  E_SFL_CTRL_TMR_VAL_SECT     = 0x0008u,
  E_SFL_CTRL_TMR_VAL_MAS      = 0x0010u,
  E_SFL_CTRL_TMR_VAL_HVENB    = 0x0020u,
  E_SFL_CTRL_TMR_VAL_MCS      = 0x0040u,
  E_SFL_CTRL_TMR_VAL_FUSERD   = 0x0080u,
  E_SFL_CTRL_TMR_VAL_IFEN     = 0x0100u,
  E_SFL_CTRL_TMR_VAL_REDEN    = 0x0800u,
  E_SFL_CTRL_TMR_VAL_DCTEN    = 0x1000u,
  E_SFL_CTRL_TMR_VAL_RADCKB   = 0x2000u,
  E_SFL_CTRL_TMR_VAL_WADCKB   = 0x4000u,
  E_SFL_CTRL_TMR_VAL_TSREGSEL = 0x8000u
};

/* TMR_VAL address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TMR_VAL, 0x000003B8u, __READ_WRITE, sfl_ctrl_tmr_val_bf_t);

/* ------------------------------------------------------------------------------ */
/* TMR_DIG register ... */

/* TMR_DIG bitfield type ... */

typedef struct {
  __REG16 avref_on_sel :  1;       /* [0] */
  __REG16 avref_on_val :  1;       /* [1] */
  __REG16              : 14;
} sfl_ctrl_tmr_dig_bf_t;

/* TMR_DIG single bit enum ... */

enum {
  E_SFL_CTRL_TMR_DIG_AVREF_ON_SEL = 0x0001u,
  E_SFL_CTRL_TMR_DIG_AVREF_ON_VAL = 0x0002u
};

/* TMR_DIG address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TMR_DIG, 0x000003BAu, __READ_WRITE, sfl_ctrl_tmr_dig_bf_t);

/* ------------------------------------------------------------------------------ */
/* TMR_ANA register ... */

/* TMR_ANA bitfield type ... */

typedef struct {
  __REG16 exvpp_to_pad   :  1;       /* [0] */
  __REG16 vddf_to_atbus  :  1;       /* [1] */
  __REG16 vddhf_to_atbus :  1;       /* [2] */
  __REG16 avref_to_atbus :  1;       /* [3] */
  __REG16 vddf_min       :  1;       /* [4] */
  __REG16 vddf_max       :  1;       /* [5] */
  __REG16 vddhf_min      :  1;       /* [6] */
  __REG16 vddhf_max      :  1;       /* [7] */
  __REG16                :  8;
} sfl_ctrl_tmr_ana_bf_t;

/* TMR_ANA single bit enum ... */

enum {
  E_SFL_CTRL_TMR_ANA_EXVPP_TO_PAD   = 0x0001u,
  E_SFL_CTRL_TMR_ANA_VDDF_TO_ATBUS  = 0x0002u,
  E_SFL_CTRL_TMR_ANA_VDDHF_TO_ATBUS = 0x0004u,
  E_SFL_CTRL_TMR_ANA_AVREF_TO_ATBUS = 0x0008u,
  E_SFL_CTRL_TMR_ANA_VDDF_MIN       = 0x0010u,
  E_SFL_CTRL_TMR_ANA_VDDF_MAX       = 0x0020u,
  E_SFL_CTRL_TMR_ANA_VDDHF_MIN      = 0x0040u,
  E_SFL_CTRL_TMR_ANA_VDDHF_MAX      = 0x0080u
};

/* TMR_ANA address mapping ... */

__IO_REG16_BIT(SFL_CTRL_TMR_ANA, 0x000003BCu, __READ_WRITE, sfl_ctrl_tmr_ana_bf_t);

/* ============================================================================== */
/* EEPROM_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* MODE register ... */

/* MODE bitfield type ... */

typedef struct {
  __REG16 erase       :  1;       /* [0] */
  __REG16 prog        :  1;       /* [1] */
  __REG16 margin_read :  1;       /* [2] */
  __REG16             :  5;
  __REG16 pass        :  8;       /* [15:8] */
} eeprom_ctrl_mode_bf_t;

/* MODE single bit enum ... */

enum {
  E_EEPROM_CTRL_MODE_ERASE       = 0x0001u,
  E_EEPROM_CTRL_MODE_PROG        = 0x0002u,
  E_EEPROM_CTRL_MODE_MARGIN_READ = 0x0004u
};

/* MODE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_MODE, 0x000003C0u, __READ_WRITE, eeprom_ctrl_mode_bf_t);

/* ------------------------------------------------------------------------------ */
/* STATUS register ... */

/* STATUS bitfield type ... */

typedef struct {
  __REG16 busy      :  1;       /* [0] */
  __REG16 mode_busy :  1;       /* [1] */
  __REG16           : 14;
} eeprom_ctrl_status_bf_t;

/* STATUS single bit enum ... */

enum {
  E_EEPROM_CTRL_STATUS_BUSY      = 0x0001u,
  E_EEPROM_CTRL_STATUS_MODE_BUSY = 0x0002u
};

/* STATUS address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_STATUS, 0x000003C2u, __READ, eeprom_ctrl_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* LOCK_L register ... */

/* LOCK_L bitfield type ... */

typedef struct {
  __REG16 lock :  8;       /* [7:0] */
  __REG16 pass :  8;       /* [15:8] */
} eeprom_ctrl_lock_l_bf_t;

/* LOCK_L address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_LOCK_L, 0x000003C4u, __READ_WRITE, eeprom_ctrl_lock_l_bf_t);

/* ------------------------------------------------------------------------------ */
/* LOCK_U register ... */

/* LOCK_U bitfield type ... */

typedef struct {
  __REG16 lock :  8;       /* [7:0] */
  __REG16 pass :  8;       /* [15:8] */
} eeprom_ctrl_lock_u_bf_t;

/* LOCK_U address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_LOCK_U, 0x000003C6u, __READ_WRITE, eeprom_ctrl_lock_u_bf_t);

/* ------------------------------------------------------------------------------ */
/* LOCK_CNT register ... */

/* LOCK_CNT bitfield type ... */

typedef struct {
  __REG16 lock_counter :  4;       /* [3:0] */
  __REG16              :  4;
  __REG16 pass         :  8;       /* [15:8] */
} eeprom_ctrl_lock_cnt_bf_t;

/* LOCK_CNT address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_LOCK_CNT, 0x000003CAu, __READ_WRITE, eeprom_ctrl_lock_cnt_bf_t);

/* ------------------------------------------------------------------------------ */
/* LOCK_L_FREEZE register ... */

/* LOCK_L_FREEZE bitfield type ... */

typedef struct {
  __REG16 freeze :  8;       /* [7:0] */
  __REG16 pass   :  8;       /* [15:8] */
} eeprom_ctrl_lock_l_freeze_bf_t;

/* LOCK_L_FREEZE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_LOCK_L_FREEZE, 0x000003CCu, __READ_WRITE, eeprom_ctrl_lock_l_freeze_bf_t);

/* ------------------------------------------------------------------------------ */
/* LOCK_U_FREEZE register ... */

/* LOCK_U_FREEZE bitfield type ... */

typedef struct {
  __REG16 freeze :  8;       /* [7:0] */
  __REG16 pass   :  8;       /* [15:8] */
} eeprom_ctrl_lock_u_freeze_bf_t;

/* LOCK_U_FREEZE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_LOCK_U_FREEZE, 0x000003CEu, __READ_WRITE, eeprom_ctrl_lock_u_freeze_bf_t);

/* ------------------------------------------------------------------------------ */
/* IP_ENABLE register ... */

/* IP_ENABLE bitfield type ... */

typedef struct {
  __REG16 enable :  1;       /* [0] */
  __REG16        :  7;
  __REG16 pass   :  8;       /* [15:8] */
} eeprom_ctrl_ip_enable_bf_t;

/* IP_ENABLE single bit enum ... */

enum {
  E_EEPROM_CTRL_IP_ENABLE_ENABLE = 0x0001u
};

/* IP_ENABLE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IP_ENABLE, 0x000003D0u, __READ_WRITE, eeprom_ctrl_ip_enable_bf_t);

/* ------------------------------------------------------------------------------ */
/* MARGIN_TRIM register ... */

/* MARGIN_TRIM bitfield type ... */

typedef struct {
  __REG16 trim     :  3;       /* [2:0] */
  __REG16 reg_trim :  2;       /* [4:3] */
  __REG16          : 11;
} eeprom_ctrl_margin_trim_bf_t;

/* MARGIN_TRIM address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_MARGIN_TRIM, 0x000003D2u, __READ_WRITE, eeprom_ctrl_margin_trim_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMEOUT_L register ... */

/* TIMEOUT_L bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} eeprom_ctrl_timeout_l_bf_t;

/* TIMEOUT_L address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_TIMEOUT_L, 0x000003D4u, __READ, eeprom_ctrl_timeout_l_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMEOUT_H register ... */

/* TIMEOUT_H bitfield type ... */

typedef struct {
  __REG16 val :  4;       /* [3:0] */
  __REG16     : 12;
} eeprom_ctrl_timeout_h_bf_t;

/* TIMEOUT_H address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_TIMEOUT_H, 0x000003D6u, __READ, eeprom_ctrl_timeout_h_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_STATUS register ... */

/* IRQ_STATUS bitfield type ... */

typedef struct {
  __REG16 timeout       :  1;       /* [0] */
  __REG16 invalid_addr  :  1;       /* [1] */
  __REG16 ip_off_access :  1;       /* [2] */
  __REG16               : 13;
} eeprom_ctrl_irq_status_bf_t;

/* IRQ_STATUS single bit enum ... */

enum {
  E_EEPROM_CTRL_IRQ_STATUS_TIMEOUT       = 0x0001u,
  E_EEPROM_CTRL_IRQ_STATUS_INVALID_ADDR  = 0x0002u,
  E_EEPROM_CTRL_IRQ_STATUS_IP_OFF_ACCESS = 0x0004u
};

/* IRQ_STATUS address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_STATUS, 0x000003F0u, __READ_WRITE, eeprom_ctrl_irq_status_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_MASK register ... */

/* IRQ_MASK bitfield type ... */

typedef struct {
  __REG16 timeout       :  1;       /* [0] */
  __REG16 invalid_addr  :  1;       /* [1] */
  __REG16 ip_off_access :  1;       /* [2] */
  __REG16               : 13;
} eeprom_ctrl_irq_mask_bf_t;

/* IRQ_MASK single bit enum ... */

enum {
  E_EEPROM_CTRL_IRQ_MASK_TIMEOUT       = 0x0001u,
  E_EEPROM_CTRL_IRQ_MASK_INVALID_ADDR  = 0x0002u,
  E_EEPROM_CTRL_IRQ_MASK_IP_OFF_ACCESS = 0x0004u
};

/* IRQ_MASK address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_MASK, 0x000003F4u, __READ_WRITE, eeprom_ctrl_irq_mask_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VENABLE register ... */

/* IRQ_VENABLE bitfield type ... */

typedef struct {
  __REG16 vno :  2;       /* [1:0] */
  __REG16     : 14;
} eeprom_ctrl_irq_venable_bf_t;

/* IRQ_VENABLE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_VENABLE, 0x000003F8u, __WRITE, eeprom_ctrl_irq_venable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VDISABLE register ... */

/* IRQ_VDISABLE bitfield type ... */

typedef struct {
  __REG16 vno :  2;       /* [1:0] */
  __REG16     : 14;
} eeprom_ctrl_irq_vdisable_bf_t;

/* IRQ_VDISABLE address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_VDISABLE, 0x000003FAu, __WRITE, eeprom_ctrl_irq_vdisable_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VMAX register ... */

/* IRQ_VMAX bitfield type ... */

typedef struct {
  __REG16 vmax :  2;       /* [1:0] */
  __REG16      : 14;
} eeprom_ctrl_irq_vmax_bf_t;

/* IRQ_VMAX address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_VMAX, 0x000003FCu, __READ_WRITE, eeprom_ctrl_irq_vmax_bf_t);

/* ------------------------------------------------------------------------------ */
/* IRQ_VNO register ... */

/* IRQ_VNO bitfield type ... */

typedef struct {
  __REG16 vno :  2;       /* [1:0] */
  __REG16     : 14;
} eeprom_ctrl_irq_vno_bf_t;

/* IRQ_VNO address mapping ... */

__IO_REG16_BIT(EEPROM_CTRL_IRQ_VNO, 0x000003FEu, __READ_WRITE, eeprom_ctrl_irq_vno_bf_t);

/* ============================================================================== */
/* CAL_SFL_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_0 register ... */

/* VALID_SIGNATURE_0 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_0_bf_t;

/* VALID_SIGNATURE_0 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_0, 0x00001000u, __READ, cal_sfl_ctrl_valid_signature_0_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_1 register ... */

/* VALID_SIGNATURE_1 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_1_bf_t;

/* VALID_SIGNATURE_1 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_1, 0x00001004u, __READ, cal_sfl_ctrl_valid_signature_1_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_2 register ... */

/* VALID_SIGNATURE_2 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_2_bf_t;

/* VALID_SIGNATURE_2 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_2, 0x00001008u, __READ, cal_sfl_ctrl_valid_signature_2_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_3 register ... */

/* VALID_SIGNATURE_3 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_3_bf_t;

/* VALID_SIGNATURE_3 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_3, 0x0000100Cu, __READ, cal_sfl_ctrl_valid_signature_3_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_4 register ... */

/* VALID_SIGNATURE_4 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_4_bf_t;

/* VALID_SIGNATURE_4 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_4, 0x00001010u, __READ, cal_sfl_ctrl_valid_signature_4_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_5 register ... */

/* VALID_SIGNATURE_5 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_5_bf_t;

/* VALID_SIGNATURE_5 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_5, 0x00001014u, __READ, cal_sfl_ctrl_valid_signature_5_bf_t);

/* ------------------------------------------------------------------------------ */
/* VALID_SIGNATURE_6 register ... */

/* VALID_SIGNATURE_6 bitfield type ... */

typedef struct {
  __REG32 sign : 32;       /* [31:0] */
} cal_sfl_ctrl_valid_signature_6_bf_t;

/* VALID_SIGNATURE_6 address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_VALID_SIGNATURE_6, 0x00001018u, __READ, cal_sfl_ctrl_valid_signature_6_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_DCT register ... */

/* SFL_DCT bitfield type ... */

typedef struct {
  __REG32 dct : 32;       /* [31:0] */
} cal_sfl_ctrl_sfl_dct_bf_t;

/* SFL_DCT address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_DCT, 0x0000101Cu, __READ, cal_sfl_ctrl_sfl_dct_bf_t);

/* ------------------------------------------------------------------------------ */
/* INIT_CODE_PTR register ... */

/* INIT_CODE_PTR bitfield type ... */

typedef struct {
  __REG32 addr : 32;       /* [31:0] */
} cal_sfl_ctrl_init_code_ptr_bf_t;

/* INIT_CODE_PTR address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_INIT_CODE_PTR, 0x00001020u, __READ, cal_sfl_ctrl_init_code_ptr_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_READ_TIMING register ... */

/* SFL_READ_TIMING bitfield type ... */

typedef struct {
  __REG32 initial_fuse_read :  4;       /* [3:0] */
  __REG32 fuse_read         :  4;       /* [7:4] */
  __REG32 main_read         :  4;       /* [11:8] */
  __REG32                   : 20;
} cal_sfl_ctrl_sfl_read_timing_bf_t;

/* SFL_READ_TIMING address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_READ_TIMING, 0x00001024u, __READ, cal_sfl_ctrl_sfl_read_timing_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_VERIFY_CONFIG register ... */

/* SFL_VERIFY_CONFIG bitfield type ... */

typedef struct {
  __REG32 verify0_osvrd :  3;       /* [2:0] */
  __REG32 verify1_osvrd :  3;       /* [5:3] */
  __REG32 avref_trim    :  6;       /* [11:6] */
  __REG32               : 20;
} cal_sfl_ctrl_sfl_verify_config_bf_t;

/* SFL_VERIFY_CONFIG address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_VERIFY_CONFIG, 0x00001028u, __READ, cal_sfl_ctrl_sfl_verify_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_FUSE_PROTECT register ... */

/* SFL_FUSE_PROTECT bitfield type ... */

typedef struct {
  __REG32 dct   :  1;       /* [0] */
  __REG32 red   :  1;       /* [1] */
  __REG32 ldt0  :  1;       /* [2] */
  __REG32 ldt1  :  1;       /* [3] */
  __REG32 info0 :  1;       /* [4] */
  __REG32 info1 :  1;       /* [5] */
  __REG32 info2 :  1;       /* [6] */
  __REG32 info3 :  1;       /* [7] */
  __REG32       : 24;
} cal_sfl_ctrl_sfl_fuse_protect_bf_t;

/* SFL_FUSE_PROTECT single bit enum ... */

enum {
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_DCT   = 0x00000001u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_RED   = 0x00000002u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_LDT0  = 0x00000004u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_LDT1  = 0x00000008u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_INFO0 = 0x00000010u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_INFO1 = 0x00000020u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_INFO2 = 0x00000040u,
  E_CAL_SFL_CTRL_SFL_FUSE_PROTECT_INFO3 = 0x00000080u
};

/* SFL_FUSE_PROTECT address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_FUSE_PROTECT, 0x0000102Cu, __READ, cal_sfl_ctrl_sfl_fuse_protect_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_MAIN_PROTECT register ... */

/* SFL_MAIN_PROTECT bitfield type ... */

typedef struct {
  __REG32 lock : 16;       /* [15:0] */
  __REG32      : 16;
} cal_sfl_ctrl_sfl_main_protect_bf_t;

/* SFL_MAIN_PROTECT address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_MAIN_PROTECT, 0x00001030u, __READ, cal_sfl_ctrl_sfl_main_protect_bf_t);

/* ------------------------------------------------------------------------------ */
/* SFL_MAIN_FINE_PROTECT registers ... */

/* SFL_MAIN_FINE_PROTECT bitfield type ... */

typedef struct {
  __REG32 lock : 16;       /* [15:0] */
  __REG32      : 16;
} cal_sfl_ctrl_sfl_main_fine_protect_bf_t;

/* SFL_MAIN_FINE_PROTECT address mapping ... */

__IO_REG32_BIT(CAL_SFL_CTRL_SFL_MAIN_FINE_PROTECT_0, 0x00001038u, __READ, cal_sfl_ctrl_sfl_main_fine_protect_bf_t);
__IO_REG32_BIT(CAL_SFL_CTRL_SFL_MAIN_FINE_PROTECT_1, 0x0000103Cu, __READ, cal_sfl_ctrl_sfl_main_fine_protect_bf_t);
__IO_REG32_BIT(CAL_SFL_CTRL_SFL_MAIN_FINE_PROTECT_2, 0x00001040u, __READ, cal_sfl_ctrl_sfl_main_fine_protect_bf_t);
__IO_REG32_BIT(CAL_SFL_CTRL_SFL_MAIN_FINE_PROTECT_3, 0x00001044u, __READ, cal_sfl_ctrl_sfl_main_fine_protect_bf_t);

/* ============================================================================== */
/* CAL_SYS_STATE module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* CALIBRATION0 register ... */

/* CALIBRATION0 bitfield type ... */

typedef struct {
  __REG32 trim_osc_48m      :  5;       /* [4:0] */
  __REG32 trim_osc_48m_temp :  3;       /* [7:5] */
  __REG32 trim_iref_temp    :  3;       /* [10:8] */
  __REG32 trim_iref         :  5;       /* [15:11] */
  __REG32                   : 16;
} cal_sys_state_calibration0_bf_t;

/* CALIBRATION0 address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_CALIBRATION0, 0x00001048u, __READ_WRITE, cal_sys_state_calibration0_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION1 register ... */

/* CALIBRATION1 bitfield type ... */

typedef struct {
  __REG32 trim_led0     :  6;       /* [5:0] */
  __REG32 trim_led1     :  6;       /* [11:6] */
  __REG32 led_top_disop :  1;       /* [12] */
  __REG32               : 19;
} cal_sys_state_calibration1_bf_t;

/* CALIBRATION1 single bit enum ... */

enum {
  E_CAL_SYS_STATE_CALIBRATION1_LED_TOP_DISOP = 0x00001000u
};

/* CALIBRATION1 address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_CALIBRATION1, 0x0000104Cu, __READ_WRITE, cal_sys_state_calibration1_bf_t);

/* ------------------------------------------------------------------------------ */
/* CALIBRATION2 register ... */

/* CALIBRATION2 bitfield type ... */

typedef struct {
  __REG32 trim_led2      :  6;       /* [5:0] */
  __REG32 trim_led3      :  6;       /* [11:6] */
  __REG32 trim_bg        :  2;       /* [13:12] */
  __REG32 trim_v1v5_load :  2;       /* [15:14] */
  __REG32                : 16;
} cal_sys_state_calibration2_bf_t;

/* CALIBRATION2 address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_CALIBRATION2, 0x00001050u, __READ_WRITE, cal_sys_state_calibration2_bf_t);

/* ------------------------------------------------------------------------------ */
/* TRIM_LIN_AA_TYPE1 register ... */

/* TRIM_LIN_AA_TYPE1 bitfield type ... */

typedef struct {
  __REG32 cm_error     :  5;       /* [4:0] */
  __REG32              :  3;
  __REG32 offset_error :  5;       /* [12:8] */
  __REG32              :  2;
  __REG32 lock         :  1;       /* [15] */
  __REG32              : 16;
} cal_sys_state_trim_lin_aa_type1_bf_t;

/* TRIM_LIN_AA_TYPE1 single bit enum ... */

enum {
  E_CAL_SYS_STATE_TRIM_LIN_AA_TYPE1_LOCK         = 0x00008000u
};

/* TRIM_LIN_AA_TYPE1 address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_TRIM_LIN_AA_TYPE1, 0x00001054u, __READ_WRITE, cal_sys_state_trim_lin_aa_type1_bf_t);

/* ------------------------------------------------------------------------------ */
/* TRIM_LIN_AA_TYPE2_3 register ... */

/* TRIM_LIN_AA_TYPE2_3 bitfield type ... */

typedef struct {
  __REG32 cm_error     :  5;       /* [4:0] */
  __REG32              :  3;
  __REG32 offset_error :  5;       /* [12:8] */
  __REG32              :  2;
  __REG32 lock         :  1;       /* [15] */
  __REG32              : 16;
} cal_sys_state_trim_lin_aa_type2_3_bf_t;

/* TRIM_LIN_AA_TYPE2_3 single bit enum ... */

enum {
  E_CAL_SYS_STATE_TRIM_LIN_AA_TYPE2_3_LOCK         = 0x00008000u
};

/* TRIM_LIN_AA_TYPE2_3 address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_TRIM_LIN_AA_TYPE2_3, 0x00001058u, __READ_WRITE, cal_sys_state_trim_lin_aa_type2_3_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADC_OFFSET register ... */

/* ADC_OFFSET bitfield type ... */

typedef struct {
  __REG32 val : 13;       /* [12:0] */
  __REG32     : 19;
} cal_sys_state_adc_offset_bf_t;

/* ADC_OFFSET address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_ADC_OFFSET, 0x0000105Cu, __READ_WRITE, cal_sys_state_adc_offset_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADC_GAIN register ... */

/* ADC_GAIN bitfield type ... */

typedef struct {
  __REG32 val : 13;       /* [12:0] */
  __REG32     : 19;
} cal_sys_state_adc_gain_bf_t;

/* ADC_GAIN address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_ADC_GAIN, 0x00001060u, __READ_WRITE, cal_sys_state_adc_gain_bf_t);

/* ------------------------------------------------------------------------------ */
/* ENABLE_JTAG register ... */

/* ENABLE_JTAG bitfield type ... */

typedef struct {
  __REG32 enable :  1;       /* [0] */
  __REG32        : 31;
} cal_sys_state_enable_jtag_bf_t;

/* ENABLE_JTAG single bit enum ... */

enum {
  E_CAL_SYS_STATE_ENABLE_JTAG_ENABLE = 0x00000001u
};

/* ENABLE_JTAG address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_ENABLE_JTAG, 0x00001064u, __READ_WRITE, cal_sys_state_enable_jtag_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_TYPE3_MEAS_CONFIG register ... */

/* AA_TYPE3_MEAS_CONFIG bitfield type ... */

typedef struct {
  __REG32 step_size : 12;       /* [11:0] */
  __REG32           : 20;
} cal_sys_state_aa_type3_meas_config_bf_t;

/* AA_TYPE3_MEAS_CONFIG address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_AA_TYPE3_MEAS_CONFIG, 0x00001068u, __READ_WRITE, cal_sys_state_aa_type3_meas_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_OFFSET_GOAL register ... */

/* AA_OFFSET_GOAL bitfield type ... */

typedef struct {
  __REG32 goal : 12;       /* [11:0] */
  __REG32      :  3;
  __REG32 lock :  1;       /* [15] */
  __REG32      : 16;
} cal_sys_state_aa_offset_goal_bf_t;

/* AA_OFFSET_GOAL single bit enum ... */

enum {
  E_CAL_SYS_STATE_AA_OFFSET_GOAL_LOCK = 0x00008000u
};

/* AA_OFFSET_GOAL address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_AA_OFFSET_GOAL, 0x00001074u, __READ_WRITE, cal_sys_state_aa_offset_goal_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_OFFSET_CONFIG register ... */

/* AA_OFFSET_CONFIG bitfield type ... */

typedef struct {
  __REG32 enable                     :  1;       /* [0] */
  __REG32 oversampling               :  2;       /* [2:1] */
  __REG32                            :  1;
  __REG32 algorithm                  :  1;       /* [4] */
  __REG32 filter                     :  1;       /* [5] */
  __REG32                            :  1;
  __REG32 trigger_offset_calibration :  8;       /* [14:7] */
  __REG32 lock                       :  1;       /* [15] */
  __REG32                            : 16;
} cal_sys_state_aa_offset_config_bf_t;

/* AA_OFFSET_CONFIG single bit enum ... */

enum {
  E_CAL_SYS_STATE_AA_OFFSET_CONFIG_ENABLE                     = 0x00000001u,
  E_CAL_SYS_STATE_AA_OFFSET_CONFIG_ALGORITHM                  = 0x00000010u,
  E_CAL_SYS_STATE_AA_OFFSET_CONFIG_FILTER                     = 0x00000020u,
  E_CAL_SYS_STATE_AA_OFFSET_CONFIG_LOCK                       = 0x00008000u
};

/* AA_OFFSET_CONFIG address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_AA_OFFSET_CONFIG, 0x00001076u, __READ_WRITE, cal_sys_state_aa_offset_config_bf_t);

/* ------------------------------------------------------------------------------ */
/* AA_PHY_CONFIG register ... */

/* AA_PHY_CONFIG bitfield type ... */

typedef struct {
  __REG32 meas_in_mux   :  1;       /* [0] */
  __REG32 gain          :  2;       /* [2:1] */
  __REG32               :  2;
  __REG32 amp_select    :  1;       /* [5] */
  __REG32 cm_meas       :  1;       /* [6] */
  __REG32 ipu_connect_m :  1;       /* [7] */
  __REG32               :  4;
  __REG32 amp_on        :  1;       /* [12] */
  __REG32               :  2;
  __REG32 lock_gain     :  1;       /* [15] */
  __REG32               : 16;
} cal_sys_state_aa_phy_config_bf_t;

/* AA_PHY_CONFIG single bit enum ... */

enum {
  E_CAL_SYS_STATE_AA_PHY_CONFIG_MEAS_IN_MUX   = 0x00000001u,
  E_CAL_SYS_STATE_AA_PHY_CONFIG_AMP_SELECT    = 0x00000020u,
  E_CAL_SYS_STATE_AA_PHY_CONFIG_CM_MEAS       = 0x00000040u,
  E_CAL_SYS_STATE_AA_PHY_CONFIG_IPU_CONNECT_M = 0x00000080u,
  E_CAL_SYS_STATE_AA_PHY_CONFIG_AMP_ON        = 0x00001000u,
  E_CAL_SYS_STATE_AA_PHY_CONFIG_LOCK_GAIN     = 0x00008000u
};

/* AA_PHY_CONFIG address mapping ... */

__IO_REG32_BIT(CAL_SYS_STATE_AA_PHY_CONFIG, 0x00001078u, __READ_WRITE, cal_sys_state_aa_phy_config_bf_t);

/* ============================================================================== */
/* TESTER_SFL_CTRL module register definitions */
/* ============================================================================== */

/* ------------------------------------------------------------------------------ */
/* RANDOMIZE_FLASH register ... */

/* RANDOMIZE_FLASH bitfield type ... */

typedef struct {
  __REG16 run      :  1;       /* [0] */
  __REG16 run_main :  1;       /* [1] */
  __REG16          : 14;
} tester_sfl_ctrl_randomize_flash_bf_t;

/* RANDOMIZE_FLASH single bit enum ... */

enum {
  E_TESTER_SFL_CTRL_RANDOMIZE_FLASH_RUN      = 0x0001u,
  E_TESTER_SFL_CTRL_RANDOMIZE_FLASH_RUN_MAIN = 0x0002u
};

/* RANDOMIZE_FLASH address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_RANDOMIZE_FLASH, 0x00001A40u, __WRITE, tester_sfl_ctrl_randomize_flash_bf_t);

/* ------------------------------------------------------------------------------ */
/* PROVOKE_ERROR register ... */

/* PROVOKE_ERROR bitfield type ... */

typedef struct {
  __REG16 supply_error   :  1;       /* [0] */
  __REG16 ecc_1bit_error :  1;       /* [1] */
  __REG16 ecc_2bit_error :  1;       /* [2] */
  __REG16                : 13;
} tester_sfl_ctrl_provoke_error_bf_t;

/* PROVOKE_ERROR single bit enum ... */

enum {
  E_TESTER_SFL_CTRL_PROVOKE_ERROR_SUPPLY_ERROR   = 0x0001u,
  E_TESTER_SFL_CTRL_PROVOKE_ERROR_ECC_1BIT_ERROR = 0x0002u,
  E_TESTER_SFL_CTRL_PROVOKE_ERROR_ECC_2BIT_ERROR = 0x0004u
};

/* PROVOKE_ERROR address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_PROVOKE_ERROR, 0x00001A42u, __WRITE, tester_sfl_ctrl_provoke_error_bf_t);

/* ------------------------------------------------------------------------------ */
/* ADDR registers ... */

/* ADDR bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} tester_sfl_ctrl_addr_bf_t;

/* ADDR address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_ADDR_L, 0x00001A44u, __WRITE, tester_sfl_ctrl_addr_bf_t);
__IO_REG16_BIT(TESTER_SFL_CTRL_ADDR_H, 0x00001A46u, __WRITE, tester_sfl_ctrl_addr_bf_t);

/* ------------------------------------------------------------------------------ */
/* DATA registers ... */

/* DATA bitfield type ... */

typedef struct {
  __REG16 val : 16;       /* [15:0] */
} tester_sfl_ctrl_data_bf_t;

/* DATA address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_DATA_L, 0x00001A48u, __READ_WRITE, tester_sfl_ctrl_data_bf_t);
__IO_REG16_BIT(TESTER_SFL_CTRL_DATA_H, 0x00001A4Au, __READ_WRITE, tester_sfl_ctrl_data_bf_t);

/* ------------------------------------------------------------------------------ */
/* ECC_SUM register ... */

/* ECC_SUM bitfield type ... */

typedef struct {
  __REG16 val :  7;       /* [6:0] */
  __REG16     :  9;
} tester_sfl_ctrl_ecc_sum_bf_t;

/* ECC_SUM address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_ECC_SUM, 0x00001A4Cu, __READ, tester_sfl_ctrl_ecc_sum_bf_t);

/* ------------------------------------------------------------------------------ */
/* CONTROL register ... */

/* CONTROL bitfield type ... */

typedef struct {
  __REG16 write :  1;       /* [0] */
  __REG16 read  :  1;       /* [1] */
  __REG16       : 14;
} tester_sfl_ctrl_control_bf_t;

/* CONTROL single bit enum ... */

enum {
  E_TESTER_SFL_CTRL_CONTROL_WRITE = 0x0001u,
  E_TESTER_SFL_CTRL_CONTROL_READ  = 0x0002u
};

/* CONTROL address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_CONTROL, 0x00001A4Eu, __WRITE, tester_sfl_ctrl_control_bf_t);

/* ------------------------------------------------------------------------------ */
/* TIMEOUT_CONFIG register ... */

/* TIMEOUT_CONFIG bitfield type ... */

typedef struct {
  __REG16 overwrite_value : 16;       /* [15:0] */
} tester_sfl_ctrl_timeout_config_bf_t;

/* TIMEOUT_CONFIG address mapping ... */

__IO_REG16_BIT(TESTER_SFL_CTRL_TIMEOUT_CONFIG, 0x00001A50u, __WRITE, tester_sfl_ctrl_timeout_config_bf_t);

/* ============================================================================== */

#ifdef __IAR_SYSTEMS_ICC__
  #pragma language=restore
#endif

#endif  /* __IO_E52139B__ */

