#ifndef IHR_TCPL04B_LIN_HAL_H
#define IHR_TCPL04B_LIN_HAL_H

#include "genLinConfig.h"
#include "lin_type.h"

#ifndef LIN_FRAME_TIMEOUT
#define LIN_FRAME_TIMEOUT LIN_AUTOBAUD_TIMEOUT
#endif

#define lin_hal_set_rx_pid(pid)       (lin_temp_buffer.frame.PID = (pid))
#define lin_hal_set_tx_pid(pid)       (lin_temp_buffer.frame.PID = (pid))
#define lin_hal_is_byte_received()    (lin_hal_status_g.byte_received == L_SET)
#define lin_hal_reset_byte_received() (lin_hal_status_g.byte_received = L_RESET)

void lin_hal_init(void);
void lin_hal_rx_response(l_u8 Type, l_u8 Length);
void lin_hal_tx_response(l_u8 Type, const l_u8 Data[], l_u8 Length);
void lin_hal_wait_for_break(void);
void lin_hal_tx_wake_up(void);
void lin_hal_set_resp_error(void);
void l_hal_cyclic_call(void);
void l_frame_timeout_control(void);
void l_autobaud_control(void);
void l_hal_baudrate_control(void);

extern LIN_hal_stat_reg_t lin_hal_status_g;
extern t_lin_temp_frame_buffer lin_temp_buffer;
extern LIN_SLAVE_STATE_t lin_slave_state_g;
extern l_u8 lin_ab_timeout_timer;
extern l_u8 lin_frm_timeout_timer;
extern t_lin_timeout_ctrl lin_timeout_ctrl;
extern l_u8 l_txchk_byte_nr;
extern volatile l_u8 lin_ab_t2_state;

#endif
