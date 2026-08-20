#include "lin_hal.h"

#include "lin_autoaddressing.h"
#include "lin_driver_api.h"
#include "lin_main.h"
#include "lin_slave_task.h"
#include "pal_lin_comm.h"
#include "tcpl04x.h"
#include "tcpl04x_ll_sci.h"

LIN_hal_stat_reg_t lin_hal_status_g;
t_lin_temp_frame_buffer lin_temp_buffer;
LIN_SLAVE_STATE_t lin_slave_state_g;
l_u8 lin_ab_timeout_timer;
l_u8 lin_frm_timeout_timer;
t_lin_timeout_ctrl lin_timeout_ctrl;
l_u8 l_txchk_byte_nr;
volatile l_u8 lin_ab_t2_state;
volatile l_u16 lin_temp_word;

static const l_u8 *s_txData;
static l_u8 s_txLength;
static l_u8 s_txOffset;
static l_u8 s_rxLength;
static l_u8 s_rxOffset;
static E_StateMachineStatus s_autoAddressState;
static AA_INITIAL_CALLBACK s_autoAddressStart;
static AA_INITIAL_CALLBACK s_autoAddressEnd;
static bool s_autoAddressSelected;
static uint32_t s_savedPrimask;
static uint8_t s_criticalNesting;

#if defined(SWSC_ENABLE_AUTO_ADDRESSING)
static bool Lin_HwIsB5Start(const l_u8 *Data)
{
    return ((Data[0] == 0x7FU) && (Data[1] == 0x06U) &&
            (Data[2] == 0xB5U) && (Data[3] == 0xFFU) &&
            (Data[4] == 0x7FU) &&
            (Data[5] == D_AUTO_ADDR_STEP1_START_AA) &&
            (Data[6] == 0x02U) && (Data[7] == 0xFFU));
}
#endif

static void Lin_HwAbort(void)
{
    pal_lin_abort_handle(LIN_BUS_0,
        (lin_abort_type_e)(LIN_ABORT_TYPE_RX | LIN_ABORT_TYPE_TX));
    s_txData = NULL;
    s_txLength = 0U;
    s_txOffset = 0U;
    s_rxLength = 0U;
    s_rxOffset = 0U;
}

static void Lin_HwReportError(void)
{
    lin_hal_set_resp_error();
    Lin_HwAbort();
    lin_hal_wait_for_break();
}

static void Lin_HwTcpl04bIsr(uint32_t Isr)
{
    const uint32_t rxErrorMask =
        LIN_INT_RX_CHKSUM_ERROR_FLAG | LIN_INT_STOP_BIT_ERROR_FLAG |
        LIN_INT_RX_FIFO_OVF_FLAG;
    const uint32_t txErrorMask =
        LIN_INT_STOP_BIT_ERROR_FLAG | LIN_INT_TX_RX_CONFLICT_FLAG;
    const uint32_t headerErrorMask =
        LIN_INT_RX_CHKPTY_ERROR_FLAG | LIN_INT_SYNC_VALUE_ERROR_FLAG;

    /* A new BREAK owns the bus state.  Process it before stale error bits
     * which may have accumulated while the CPU was halted by a debugger. */
    if ((Isr & LIN_INT_BREAK_DET_FLAG) != 0U)
    {
        Lin_HwAbort();
        lin_hal_status_g.isr_rec_break = L_SET;
        lin_slave_state_g = break_received;
        lin_timeout_ctrl.byte = 0U;
        lin_frm_timeout_timer = 0U;
    }

    /* Header errors are relevant only when they are not accompanied by a new
     * BREAK.  Response errors belong only to the active RX/TX transaction;
     * other LIN frames on the bus must not reset this slave. */
    if ((((Isr & headerErrorMask) != 0U) &&
         ((Isr & LIN_INT_BREAK_DET_FLAG) == 0U)) ||
        (((Isr & rxErrorMask) != 0U) &&
         (lin_slave_state_g == rx_data)) ||
        (((Isr & txErrorMask) != 0U) &&
         (lin_slave_state_g == tx_data)))
    {
        Lin_HwReportError();
        return;
    }

    if ((Isr & LIN_INT_TX_FIFO_EMPTY_FLAG) != 0U)
    {
        if ((s_txData != NULL) && (s_txOffset < s_txLength))
        {
            l_u8 remaining = (l_u8)(s_txLength - s_txOffset);
            l_u8 chunk = (remaining > 4U) ? 4U : remaining;
            pal_lin_tx_4byte(LIN_BUS_0, (uint8_t *)&s_txData[s_txOffset],
                             chunk);
            s_txOffset = (l_u8)(s_txOffset + chunk);
        }
    }

    if ((Isr & LIN_INT_SYNC_DET_FLAG) != 0U)
    {
        pal_lin_autobaudrate_check(LIN_BUS_0);
        lin_hal_status_g.isr_rec_sync = L_SET;
        lin_slave_state_g = sync_received;
    }

    if ((Isr & LIN_INT_RX_PID_DONE_FLAG) != 0U)
    {
        l_u8 pid = 0xFFU;
        pal_lin_read_byte(LIN_BUS_0, LIN_READ_TYPE_PID, &pid);
        lin_temp_buffer.frame.PID = pid;
        lin_slave_task_rx_pid(pid);
    }

    /* The TCPL04B vendor driver reads RX_DATA for every byte interrupt before
     * checking the protocol state.  Always drain the hardware FIFO; copy the
     * byte into the IHR buffer only for a frame selected by LinIf. */
    if (((Isr & LIN_INT_RX_1BYTE_FLAG) != 0U) &&
        ((Isr & LIN_INT_STOP_BIT_ERROR_FLAG) == 0U))
    {
        l_u8 rxByte;
        pal_lin_read_byte(
            LIN_BUS_0, LIN_READ_TYPE_FIFO, &rxByte);
        if ((lin_slave_state_g == rx_data) &&
            (s_rxOffset < s_rxLength))
        {
            lin_temp_buffer.frame.DataBuffer[s_rxOffset] = rxByte;
            s_rxOffset++;
        }
    }

    if (((Isr & LIN_INT_RX_DONE_FLAG) != 0U) &&
        (lin_slave_state_g == rx_data))
    {
        if (s_rxOffset != s_rxLength)
        {
            Lin_HwReportError();
            return;
        }
#if defined(SWSC_ENABLE_AUTO_ADDRESSING)
        if ((s_rxLength == 8U) &&
            Lin_HwIsB5Start(lin_temp_buffer.frame.DataBuffer))
        {
            LIN_AutoAddr_Start();
        }
#endif
        lin_slave_task_rx_data();
        lin_timeout_ctrl.byte = 0U;
        lin_frm_timeout_timer = 0U;
        s_rxLength = 0U;
        s_rxOffset = 0U;
        lin_slave_state_g = idle;
    }

    if ((Isr & LIN_INT_TX_DONE_FLAG) != 0U)
    {
        lin_slave_task_tx_data();
        lin_timeout_ctrl.byte = 0U;
        lin_frm_timeout_timer = 0U;
        s_txData = NULL;
        s_txLength = 0U;
        s_txOffset = 0U;
        lin_slave_state_g = idle;
    }

    if ((Isr & LIN_INT_SLV_SELECT_FLAG) != 0U)
    {
        s_autoAddressSelected = true;
        s_autoAddressState = E_StateMachineStatusAddressed;
    }
    if ((Isr & LIN_INT_AUTOADDR_DONE_FLAG) != 0U)
    {
        if (s_autoAddressState == E_StateMachineStatusStart)
        {
            s_autoAddressState = E_StateMachineStatusWaitting;
        }
    }

}

void lin_hal_init(void)
{
    lin_hal_status_g.byte_received = L_RESET;
    lin_hal_status_g.ferror = L_RESET;
    lin_hal_status_g.stopbit = L_RESET;
    lin_hal_status_g.isr_rec_break = L_RESET;
    lin_hal_status_g.isr_rec_sync = L_RESET;
    lin_slave_state_g = idle;
    lin_timeout_ctrl.byte = 0U;
    lin_ab_timeout_timer = 0U;
    lin_frm_timeout_timer = 0U;
    lin_ab_t2_state = 0U;
    l_txchk_byte_nr = 0U;
    s_savedPrimask = 0U;
    s_criticalNesting = 0U;
    s_txData = NULL;
    s_txLength = 0U;
    s_txOffset = 0U;
    s_rxLength = 0U;
    s_rxOffset = 0U;
    pal_lin_init(LIN_BUS_0, LIN_MODE_SLV, LIN_BAUDRATE,
                 Lin_HwTcpl04bIsr);
    LIN_SCI->CTRL_F.RX_NUM_MODE = 1U;
    LIN_SCI->ICR |= (SCI_INT_RX_1BYTE_DONE | SCI_INT_RX_DONE |
                     SCI_INT_RX_FIFO_FULL | SCI_INT_RX_CHKSUM_ERR |
                     SCI_INT_RX_FIFO_OVF_ERR);
    LIN_SCI->IMR |= SCI_INT_RX_FIFO_FULL;
    LIN_SCI->IMR &= ~(SCI_INT_RX_1BYTE_DONE | SCI_INT_RX_DONE |
                      SCI_INT_RX_CHKSUM_ERR | SCI_INT_RX_FIFO_OVF_ERR);
}

void lin_hal_rx_response(l_u8 Type, l_u8 Length)
{
    (void)Type;
    if ((Length == 0U) || (Length > 8U))
    {
        Lin_HwReportError();
        return;
    }
    /* IHR HAL contract: the protocol core uses this length when copying the
     * completed response from lin_temp_buffer into its application frame. */
    g_lin_frame_data_size = Length;
    s_rxLength = Length;
    s_rxOffset = 0U;
    LIN_SCI->CTRL_F.RX_NUM_MODE = 1U;
    LIN_SCI->CTRL_F.RX_NUM = Length;
    LIN_SCI->CTRL_F.CHKSUM_TYPE =
        ((lin_temp_buffer.frame.PID == 0x3CU) ||
         (lin_temp_buffer.frame.PID == 0x7DU)) ? 0U : 1U;
    pal_lin_rx_response(LIN_BUS_0, lin_temp_buffer.frame.PID,
                        lin_temp_buffer.frame.DataBuffer, Length);
    lin_slave_state_g = rx_data;
    lin_timeout_ctrl.flag.frame = 1U;
    lin_timeout_ctrl.flag.recept_started = 1U;
    lin_frm_timeout_timer = 0U;
}

void lin_hal_tx_response(l_u8 Type, const l_u8 Data[], l_u8 Length)
{
    (void)Type;
    if ((Data == NULL) || (Length == 0U) || (Length > 8U))
    {
        Lin_HwReportError();
        return;
    }
    /* Keep the production IHR HAL contract for both response directions. */
    g_lin_frame_data_size = Length;
    s_txData = Data;
    s_txLength = Length;
    s_txOffset = (Length > 4U) ? 4U : Length;
    lin_slave_state_g = tx_data;
    lin_timeout_ctrl.flag.frame = 1U;
    lin_timeout_ctrl.flag.transm_ongoing = 1U;
    lin_frm_timeout_timer = 0U;
    if (!pal_lin_tx_response(LIN_BUS_0, lin_temp_buffer.frame.PID,
                             (uint8_t *)Data, Length))
    {
        Lin_HwReportError();
    }
}

void lin_hal_wait_for_break(void)
{
    Lin_HwAbort();
    lin_slave_state_g = idle;
    lin_timeout_ctrl.flag.frame = 0U;
    lin_timeout_ctrl.flag.recept_started = 0U;
    lin_timeout_ctrl.flag.transm_ongoing = 0U;
    lin_frm_timeout_timer = 0U;
}

void lin_hal_tx_wake_up(void)
{
    lin_slave_state_g = idle;
}

void lin_hal_set_resp_error(void)
{
    if ((lin_timeout_ctrl.flag.recept_started != 0U) ||
        (lin_timeout_ctrl.flag.transm_ongoing != 0U))
    {
        g_lin_status_word.flag.last_pid = g_lin_active_pid;
        g_lin_status_word.flag.error_in_resp = L_SET;
    }
}

void l_frame_timeout_control(void)
{
    if (lin_timeout_ctrl.flag.frame != 0U)
    {
        lin_frm_timeout_timer++;
        if (lin_frm_timeout_timer >= LIN_FRAME_TIMEOUT)
        {
            Lin_HwReportError();
        }
    }
}

void l_autobaud_control(void)
{
    if (lin_ab_t2_state != 0U)
    {
        lin_ab_timeout_timer++;
        if (lin_ab_timeout_timer >= LIN_AUTOBAUD_TIMEOUT)
        {
            lin_ab_timeout_timer = 0U;
            lin_ab_t2_state = 0U;
            pal_lin_autobaudrate_check(LIN_BUS_0);
        }
    }
}

void l_hal_baudrate_control(void) {}
void l_hal_cyclic_call(void) {}

l_irqmask l_sys_irq_disable(void)
{
    l_irqmask mask = (l_irqmask)__get_PRIMASK();
    __disable_irq();
    return mask;
}

void l_sys_irq_restore(l_irqmask Mask)
{
    if (Mask == 0U)
    {
        __enable_irq();
    }
}

void Lin_Driver_EnterCritical(void)
{
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    if (s_criticalNesting == 0U)
    {
        s_savedPrimask = mask;
    }
    s_criticalNesting++;
}

void Lin_Driver_ExitCritical(void)
{
    if (s_criticalNesting > 0U)
    {
        s_criticalNesting--;
        if ((s_criticalNesting == 0U) && (s_savedPrimask == 0U))
        {
            __enable_irq();
        }
    }
}

void LIN_AutoAddr_Init(AA_INITIAL_CALLBACK Start, AA_INITIAL_CALLBACK End)
{
    s_autoAddressStart = Start;
    s_autoAddressEnd = End;
    s_autoAddressSelected = false;
    s_autoAddressState = E_StateMachineStatusIdle;
}

void LIN_AutoAddr_Start(void)
{
    static uint16_t thresholds[4] = {0x043U, 0x075U, 0x0B9U, 0x032U};
    s_autoAddressSelected = false;
    s_autoAddressState = E_StateMachineStatusStart;
    if (s_autoAddressStart != NULL)
    {
        s_autoAddressStart();
    }
    (void)ll_lin_snpd_enable(LL_LIN_SBC_0, LIN_SNPD_TYPE_STEPS_4,
                             false, thresholds);
}

E_AddrStatus LIN_AutoAddr_Assign_NAD(void)
{
    return s_autoAddressSelected ? AddressStatusSuccess : AddressStatusWatting;
}

void LIN_AutoAddr_End(void)
{
    (void)ll_lin_snpd_disable(LL_LIN_SBC_0);
    s_autoAddressSelected = false;
    s_autoAddressState = E_StateMachineStatusIdle;
    if (s_autoAddressEnd != NULL)
    {
        s_autoAddressEnd();
    }
}

E_StateMachineStatus LIN_AutoAddr_Get_Status(void)
{
    return s_autoAddressState;
}
