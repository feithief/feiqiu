 /* ------------------------------------------------------------------- 
  * @file  lin_app.h
  * @brief lin application layer code (code)
  *
  * Purpose:
  *    
  *
  * @section history_sec Revision History
  * $Id: lin.c, v 1.0 2018/09/13 $ 
  * $Id: lin.c, v 1.1 2018/09/30 $ 
  *
  * @verbatim
  * ------------------------------------------------------------------- 
  * Date     B/F Who Comment
  * ------------------------------------------------------------------- 
  * 2018/09/13 (F) JEFFERY.WU initial version
  * 2018/09/30 (F) JEFFERY.WU add b4 diag service 
  * ------------------------------------------------------------------- 
  * 
  *
  * @creator      JEFFERY.WU
  * @created      2018/09/30
  * 
  * @project      521.36
  * @requested    -
  * @inspector    - 
  * @verification -
  * @version      $Revision: V1.0 $
  * @version      $Revision: V1.1 $
  * 
  *
  * ------------------------------------------------------------------- 
  */

/* ============================================================================ */
/* -------------------------------- INCLUDES ---------------------------------- */
/* ============================================================================ */

#include "lin_app.h"
#include "SystemStatus.h"
#include "SystemLinDiag.h"

/* ============================================================================ */
/* --------------------------- DEFINES AND MACROS ----------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* ----------------------------- USER DEFINES --------------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* ----------------------- STRUCTS, ENUMS AND TYPEDEFS ------------------------ */
/* ============================================================================ */


/* ============================================================================ */
/* ---------------------------- GLOBALE VARIABLES ----------------------------- */
/* ============================================================================ */

  l_u16 messageCounter;
  l_u16 BusIdleTimeOutCounter;
  
  #ifdef J2602_SLAVE
  static l_irqmask lin_irqState = 0u;
  static l_u16 bytes, i;
  static l_u8 *bc_data, x1[8];
  static l_u8 (*tmp_arr_p)[];
  static l_u8 reset_response = 0u;
  #endif /* ifdef J2602_SLAVE */
  
  #ifdef LIN_COOKED_API
  #define MasterReq_MaxLength 30 //orignal1 20, orignal2 30.
  unsigned char config_state;
  unsigned char diag_error_code;
  unsigned short MasterReq_length;
  unsigned short SlaveResp_length;
  //Master Request <= 20 Bytes, Slave Response <= 20 Bytes
  unsigned char MasterReq_data[MasterReq_MaxLength];
  unsigned char SlaveResp_data[MasterReq_MaxLength];
  #endif // ifdef LIN_COOKED_API
  
/* ============================================================================ */
/* ------------------------ MODULE GLOBALE VARIABLES -------------------------- */
/* ============================================================================ */


/* ============================================================================ */
/* -------------------------- FUNCTION DEFINITIONS ---------------------------- */
/* ============================================================================ */

void LIN_Task(void)
{
  BusIdleTimeOutCounter += LIN_TASK_CYCLE_MS;
  
  /* Execute diagnostic process if the message is diagnostic frame*/
  ld_task();
  
#ifdef LIN_COOKED_API
  /* Diagnostic handle */
  Diag_Handle();
#endif // end #ifdef LIN_COOKED_API
}

//every 10 ms ,this task should be called.
void LIN_Sleep_Status(void)
{
  unsigned int tmpStatus;
  
  l_sys_irq_disable(); /* disable UART interrupt */
  
  /* for more detailed information on LIN communication */
  tmpStatus = l_ifc_read_status();
  
  l_sys_irq_restore(1u); /* enable UART interrupt */
  
  /* If one of the status flags is set then there was something on the bus.
  * Reset counter and wake up. */
  if (tmpStatus & 0x0008u)
  {
#ifndef DLIN_COMM_NO_SLEEP_WHITE_LIGHT
    if (sysStatus.sleepFlag == ESystemSleepAwake)
      sysStatus.sleepFlag = ESystemSleepPrepare;
#endif
  }
  else
  {
    if (tmpStatus & 0x00FFu)
    {
      BusIdleTimeOutCounter = 0u;
    }
    else
    {
      /* After 4 seconds no communication or other disturb - go to sleep */
      if(BusIdleTimeOutCounter >= 4000u)//650 == 3.2s
      {
        BusIdleTimeOutCounter = 4000u; /* prevent overflow */
#ifndef DLIN_COMM_NO_SLEEP_WHITE_LIGHT
        if (sysStatus.sleepFlag == ESystemSleepAwake)
          sysStatus.sleepFlag = ESystemSleepPrepare;
#endif
      }
    }
  }
}


/**************************************
ISR for UART interrupts
**************************************/
void lin_uart_ISR(int sno)
{
  l_ifc_rx();
  l_cyclic_com_task();
}

l_irqmask l_sys_irq_disable (void)
{
  l_irqmask ret_val = 0u;
  
  /* Disable RX Interrupt */
  vic_DisableModule(vic_IRQ_SCI);
  return ret_val;
}

/* restore lin interrupts  */
void l_sys_irq_restore (l_irqmask mask)
{
  /* Enable RX Interrupt*/
  vic_EnableModule(vic_IRQ_SCI);
  return;
}

void LIN_Enable_Transceiver(void)
{
  LIN_CTRL_PHY_CONFIG |= E_LIN_CTRL_PHY_CONFIG_LIN_ON;                              // enable transmitter
  LIN_CTRL_PHY_CONFIG &= ~E_LIN_CTRL_PHY_CONFIG_LIN_HS;                             // setting transmitter be Normal speed mode
  
  /* The two register must be set as the following values according to Spec */
//  LIN_CTRL_LIN_AA_CONFIG_MODES0 |= 0x0001;
// LIN_CTRL_LIN_AA_CONFIG_MODES1 |= 0x0143;
}

void LIN_Disable_Transceiver(void)
{
  LIN_CTRL_PHY_CONFIG &= ~E_LIN_CTRL_PHY_CONFIG_LIN_ON;                             // disable transmitter
}

/********************
 * Example for user defined Callback functions from LIN driver
 ********************/

#ifdef LIN_ENABLE_RBI_RD_ECU_SN
/* Driver Callback to get Serial Number of ECU. */
/* Argument 1:  Output: pointer to data buffer for serial number  */
 void l_callback_RBI_ReadEcuSerialNumber(l_u8 SerialnumberResult[])
{
  /* If read ECU serial number (read by ID service - ID 1) is active,
  * fill 4 bytes of data of the array for transmitting the correct
  * data in the response.
  */
  SerialnumberResult[0] = 0x08u;
  SerialnumberResult[1] = 0x15u;
  SerialnumberResult[2] = 0x47u;
  SerialnumberResult[3] = 0x11u;
}
#endif /* ifdef LIN_ENABLE_RBI_RD_ECU_SN */

#ifdef LIN_ENABLE_RBI_USER_DEFINED
/* Driver Callback to get User defined answer for specific id. */
/* Argument 1: Input: ID from request */
/* Argument 2: Output: pointer to data buffer response (5 bytes of data) */
l_u8 l_callback_RBI_UserDefined(l_u8 id,l_u8 UserDefinedResult[])
{
  l_u8 retval = 0u;
  /* Assemble data and return data to LIN driver to transmit. Length of
  * response data different for different LIN standards:
  * LIN 2.0 / 2.1: Response data has to be filled with 5 bytes. Return value
  *                for positive response 1 to 5,
  *                for negative response = 0
  *                for no response > 5 (not recommended)
  * LIN 2.2: Allows response length to be configurable, so the return value
  *          is used as information how many databytes are transferred:
  *          for negative response = 0
  *          for positive response 1 to 5
  *                for no response > 5 (not recommended)
  *          This option can be activated by adding a define to genLinConfig.h:
  *          #define LIN_RBI_UDEF_2_2
  */
  switch (id)
  {
    case 32:
    /* define cell serial number here */
    UserDefinedResult[0] = id;
    UserDefinedResult[1] = id;
    UserDefinedResult[2] = 0x08u;
    UserDefinedResult[3] = 0x09u;
    UserDefinedResult[4] = 0xFFu;
    retval = 5u;
    break;
    case 33:
    /* define cell serial number here */
    UserDefinedResult[0] = id;
    UserDefinedResult[1] = id;
    UserDefinedResult[2] = 0x08u;
    UserDefinedResult[3] = 0x09u;
    UserDefinedResult[4] = 0xFFu;
    retval = 5u;
    break;
    case 34:
    /* define cell serial number here */
    UserDefinedResult[0] = id;
    UserDefinedResult[1] = id;
    UserDefinedResult[2] = 0x08u;
    UserDefinedResult[3] = 0x09u;
    UserDefinedResult[4] = 0xFFu;
    retval = 5u;
    break;
    case 35:
    /* define cell serial number here */
    UserDefinedResult[0] = id;
    UserDefinedResult[1] = id;
    UserDefinedResult[2] = 0x08u;
    UserDefinedResult[3] = 0x09u;
    UserDefinedResult[4] = 0xFFu;
    retval = 5u;
    break;
    default:
    break;
  }
  return retval;
}
#endif /* end #ifdef LIN_ENABLE_RBI_USER_DEFINED */

#ifdef    LIN_ENABLE_DATA_DUMP
/* Driver Callback for data dump request. */
/* Argument 1: Input: pointer to data buffer with data from request */
/* Argument 2: Output: pointer to data buffer to make response */
void l_callback_DataDump(const l_u8 ReceivedData[],l_u8 TransmitData[])
{
  /* ReceivedData delivers the request data (5 bytes) and expects
     * the response data in the TransmitData (also 5 bytes)
     * All data bytes needs to be filled correctly.
     */
    TransmitData[0] = ReceivedData[0];
    TransmitData[1] = ReceivedData[1];
    TransmitData[2] = ReceivedData[2];
    TransmitData[3] = ReceivedData[3];
    TransmitData[4] = ReceivedData[4];
}
#endif /* end #ifdef LIN_ENABLE_DATA_DUMP */

#ifndef LIN_INITIAL_NAD
l_u8 l_callback_Get_Initial_NAD(void)
{
  /* Here you can set the desired initial NAD.
  * Definition in genLinConfig.h has to be removed. */
  
  return SlaveNodeNAD;
}
#endif /* end ifndef LIN_INITIAL_NAD */


#ifdef LIN_USER_DEF_DIAG_ENABLE
/* (MISRA-C:2004 8.10/R, e765) external ... could be made static
 * Reason: Callback of API, configuration dependent usage, MISRA violation will be issued. */
void ld_user_define_diagnostic(void)
{
  /* delete previous response upon receiving a new response */
#ifdef LIN_PROTOCOL_VERSION_2_1
  if(l_u8_rd_MasterReq0() !=  0x7Eu)
#endif /* ifdef LIN_PROTOCOL_VERSION_2_1 */
  {
    g_lin_user_diag_active = 0u;
  }
  /* Example Assign NAD, ignore Suppl ID, Func ID */
  if((l_u8_rd_MasterReq0() ==  l_get_initial_NAD()) && (l_u8_rd_MasterReq1()==0x06u) && (l_u8_rd_MasterReq2() == 0xB0u))
  {
    /* positive response */
    l_u8_wr_SlaveResp0(l_get_initial_NAD());
    l_u8_wr_SlaveResp1(0x01u);
    l_u8_wr_SlaveResp2(0xF0u);
    l_u8_wr_SlaveResp3(0xFFu);
    l_u8_wr_SlaveResp4(0xFFu);
    l_u8_wr_SlaveResp5(0xFFu);
    l_u8_wr_SlaveResp6(0xFFu);
    l_u8_wr_SlaveResp7(0xFFu);
    l_set_current_NAD(l_u8_rd_MasterReq7());
    g_lin_user_diag_active = 1u;
  }
}
#endif /* ifdef LIN_USER_DEF_DIAG_ENABLE */

typedef enum RespError
{
  RespErrorNone = 0,
  RespLinIDInvalidRead,
  RespLinIDInvalidWrite,
  RespLinFrameInvalidRead,
  //RespLinFrameInvalidWrite,
  RespSIDInvalid
} RespError;


void diagErrorhandle(RespError errorCode, uint8_t* sendBuffer, uint16_t* respLength)
{
  if (RespErrorNone == errorCode)
    return;
  
  *respLength = 3;
  config_state = 2u;
  sendBuffer[0] = 0x7F;
  sendBuffer[3] = 0xAA;
  sendBuffer[4] = 0xAA;
  sendBuffer[5] = 0xAA;
  switch (errorCode)
  {
    case RespLinIDInvalidRead:  //request routing id is invalid while reading
      sendBuffer[1] = 0x22;
      sendBuffer[2] = 0x31;
     break;
    case RespLinIDInvalidWrite:  //request routing id is invalid while writing
      sendBuffer[1] = 0x2E;
      sendBuffer[2] = 0x31;
     break;
    case RespLinFrameInvalidRead:  //request length error
      sendBuffer[1] = 0x22;
      sendBuffer[2] = 0x13;
     break;
    case RespSIDInvalid:
      sendBuffer[1] = 0x2E;   //request sid is invalid
      sendBuffer[2] = 0x11;
     break;
    default:
     break;
  }
}

#ifdef LIN_COOKED_API
void Diag_Handle(void)
{
  unsigned char ld_Rx_State;
  unsigned char ld_Tx_State;
  ld_Rx_State = ld_rx_status();
  ld_Tx_State = ld_tx_status();
  
  RespError resp = RespErrorNone;

  if ((ld_Rx_State > LD_COMPLETED) || (ld_Tx_State > LD_COMPLETED))
  {
    (void)ld_init();
    config_state = 0u;
    diag_error_code = 0x01u; // Service was failure
  }
  else
  {
    switch (config_state)
    {
      case 0u:
        if (ld_Rx_State == LD_COMPLETED)
        {
          //MasterReq_length = 30u;
          MasterReq_length = MasterReq_MaxLength;
          ld_receive_message(&MasterReq_length, &MasterReq_data[0u]);
          config_state = 1u;
          diag_error_code = 0x00u; // Service was OK
        }
        break;

      case 1u:
        if (ld_Rx_State == LD_COMPLETED)
        {
          config_state = 0u;
          diag_error_code = 0x00u; // Service was OK
          
          // Write data by ID 0x2E
          if (MasterReq_data[0u] == 0x2Eu)
          {
            if (systemLinDiagHandleRecv(&MasterReq_data[0u], ((uint16_t)MasterReq_data[1u]<<8|(uint16_t)MasterReq_data[2u]), MasterReq_length) == 0)
            {
              resp = RespErrorNone;
            }
            else
            {
              resp = RespLinIDInvalidWrite;
            }
          }
          // Read data by ID 0x22
          else if (MasterReq_data[0u] == 0x22u)
          {
            uint16_t ret;
            
            if (MasterReq_length > 3)
              resp = RespLinFrameInvalidRead;
            else
            {
              ret = systemLinDiagHandleSend(&SlaveResp_data[0u],((uint16_t)MasterReq_data[1u]<<8|(uint16_t)MasterReq_data[2u]), MasterReq_length, (uint16_t*)&SlaveResp_length);
              SlaveResp_length += 3;
              
              if (ret == 0)
              {
                config_state = 2u;
                resp = RespErrorNone;
              }
              else
              {
                resp = RespLinIDInvalidRead;
              }
            }
          }
          else if (MasterReq_data[0u] == 0x27u)
          {
            if (systemLinDiag0x27Handle(&MasterReq_data[1u], MasterReq_data[1u], &SlaveResp_data[0u], (uint16_t*)&SlaveResp_length) == 0)
            {
              config_state = 2u;
              resp = RespErrorNone;
            }
          }
          else
          {
            resp = RespSIDInvalid;
          }
          
          //error handle
          diagErrorhandle(resp, &SlaveResp_data[0u], (uint16_t*)&SlaveResp_length);
        }
        break;
    
      case 2u:
        if (ld_Tx_State == LD_COMPLETED)
        {
          ld_send_message(SlaveResp_length, &SlaveResp_data[0u]);
          config_state = 0u;
        }
        break;

      default:
        config_state = 0u;
        break;
    }
  }
}
#endif // ifdef LIN_COOKED_API


