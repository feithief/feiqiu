#ifndef GEN_LIN_CONFIG_H
#define GEN_LIN_CONFIG_H

#include <stdint.h>
#include "Com_Cfg.h"

/* IHR protocol feature selection.  Application frames/signals are configured
 * only in Com_Cfg.c; this header contains no LDF-specific signal definition. */
#define LIN_BAUDRATE                    (LIN_CONFIG_BAUDRATE)
#define LIN_SLAVE
#define LIN_PROTOCOL_VERSION_2_0
#define LIN_SUPPLIER_ID                 ((l_u16)0u)
#define LIN_SUPPLIER_ID_HI              ((l_u8)0u)
#define LIN_SUPPLIER_ID_LO              ((l_u8)0u)
#define LIN_FUNCTION_ID                 ((l_u16)0u)
#define LIN_FUNCTION_ID_HI              ((l_u8)0u)
#define LIN_FUNCTION_ID_LO              ((l_u8)0u)
#define LIN_VARIANT_ID                  ((l_u8)0u)

#define LIN_DIAG_ENABLE
#define LIN_COOKED_API
#define LIN_TASK_CYCLE_MS               (2u)
#define LIN_FRAME_TIMEOUT               (5u)
#define LIN_FRAME_TIMEOUT_DELAY         (1u)
#define LIN_AUTOBAUD_TIMEOUT            (2u)
#define LIN_ENABLE_ASSIGN_FRAME_ID_RANGE
#define LIN_ENABLE_AA_BSM_B5
#define LIN_ENABLE_ASSIGN_NAD
#define LIN_ENABLE_ASSIGN_FRAME_ID
#define LIN_ENABLE_RBI_RD_MSG_ID_PID

#ifndef LIN_TYPES
#define LIN_TYPES
typedef unsigned char  l_bool;
typedef unsigned char  l_u8;
typedef unsigned short l_u16;
typedef unsigned long  l_u32;
typedef unsigned short l_irqmask, l_ioctl_op;
typedef unsigned short l_signal_handle, l_flag_handle, l_ifc_handle,
                       l_schedule_handle;
#endif

#ifndef L_TRUE
#define L_TRUE (1 == 1)
#endif
#ifndef L_FALSE
#define L_FALSE (1 == 0)
#endif

#define BRG_PRE                        ((l_u8)0u)
#define BRG_VAL                        ((l_u16)130u)
#define BRG_FD_SEL                     ((l_u8)7u)
#define BG_SEL                         ((l_u8)0u)
#define LIN_UART1
#define AUTOBAUD_ENABLED
#define MAX_BDREG                      ((l_u16)(144u & 0x7FFu))
#define MIN_BDREG                      ((l_u16)(116u & 0x7FFu))
#define LIN_INTERRUPT_PRIORITY         ((l_u8)0u)

#include "lin_type.h"

/* LIN_NUMBER_OF_FRAMES comes from the LDF-generated Com_Cfg.h. Diagnostic
 * MRF/SRF stay separate in the unchanged IHR diagnostic state machine. */

#define l_get_current_NAD()            (g_lin_prod_id.NAD)
#define l_set_current_NAD(x)           (g_lin_prod_id.NAD = (x))
#define l_get_initial_NAD()            (g_lin_prod_id.Initial_NAD)
#define l_get_SupplierID_Low()         (g_lin_prod_id.Supplier_lo)
#define l_get_SupplierID_High()        (g_lin_prod_id.Supplier_hi)
#define l_get_FunctionID_Low()         (g_lin_prod_id.Function_lo)
#define l_get_FunctionID_High()        (g_lin_prod_id.Function_hi)
#define l_get_Variant()                (g_lin_prod_id.Variant)

#define l_get_msg_id_low_frame(x)      (g_lin_frame_ctrl[(x)].frame.msg_id.lo)
#define l_get_msg_id_high_frame(x)     (g_lin_frame_ctrl[(x)].frame.msg_id.hi)
#define l_get_pid_frame(x)             (g_lin_frame_ctrl[(x)].frame.pid)
#define l_set_pid_frame(x, y)          (g_lin_frame_ctrl[(x)].frame.pid = (y))

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif
{
    l_u8 frame_data[8];
    l_u8 *dataptr;
} t_l_FrmData;

#ifdef USE_PACKED
typedef @packed union
#else
typedef union
#endif
{
    l_u8 dataBytes[LIN_NUMBER_OF_FRAMES * 8u];
    t_l_FrmData Frame[LIN_NUMBER_OF_FRAMES];
} t_l_Lin_Data;

extern t_l_Lin_Data l_LinData;
extern const t_l_Lin_Data const_LinData;
extern unsigned char SlaveNodeNAD;

void lin_frame_control_init(void);
void lin_set_frame_enable(l_u8 index);
void lin_set_frame_disable(l_u8 index);
l_u8 lin_get_frame_status(l_u8 index);

l_bool l_bytes_rd_Frame(l_u8 FrameIdx, l_u8 Start, l_u8 Count,
                        l_u8 Destination[]);
l_bool l_bytes_wr_Frame(l_u8 FrameIdx, l_u8 Start, l_u8 Count,
                        l_u8 Destination[]);
void l_Update_Frame_Flags(l_u8 number);
void l_get_byte_array(l_u8 start[], l_u8 count, l_u8 destination[]);
void l_set_byte_array(l_u8 start[], l_u8 count, const l_u8 source[]);

#define l_Update_flags_frame(x)        do { (void)(x); } while (0)
#define l_Set_Response_Error_Flag()    do { } while (0)
#define l_Reset_Response_Error_Flag()  do { } while (0)
#if (LINIF_RESPONSE_ERROR_FRAME_INDEX < LIN_NUMBER_OF_FRAMES)
#define LIN_Response_Error_Frame_PID \
    (g_lin_frame_ctrl[LINIF_RESPONSE_ERROR_FRAME_INDEX].frame.pid)
#define LIN_RespError_Compare_PID(x) \
    (g_lin_frame_ctrl[LINIF_RESPONSE_ERROR_FRAME_INDEX].frame.pid == (x))
#else
#define LIN_Response_Error_Frame_PID   (0xFFU)
#define LIN_RespError_Compare_PID(x)   (L_FALSE)
#endif

#endif
