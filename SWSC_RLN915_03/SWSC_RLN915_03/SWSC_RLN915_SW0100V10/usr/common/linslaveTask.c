/**
 * @copyright 2021 Indie microcontroller.
 *
 * This file is proprietary to Indie microcontroller.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie microcontroller.
 *
 * @file linSlaveTask.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2021/07/06
 */

#include <linSlaveTask.h>
#include <linStackTask.h>
#include <applicationTask.h>
#include <linsNodeCfgIdentify.h>
#include <softTimerTask.h>
#include <taskManager.h>
#include <ColorMixingTask.h>
#include <isrfuncs.h>
#include <measureTask.h>
#include <pdsTask.h>
#include <safetyMonitorTask.h>

#include "ModuleFlash.h"
#include "SystemLinComm.h"
#include "ModulePower.h"
#include "SystemStatus.h"
#include "SystemLinDiag.h"

#if LIN_STACK_TYPE == LIN_STACK_TYPE_LIN2_2A

#define FID_COLOR_CTRL                  DEFAULT_LINS_FID0
#define FID_STATUS_FRAME                DEFAULT_LINS_FID1
#define USER_DATA_REPORT                DEFAULT_LINS_FID2
#define EVENT_TRIGGER_DATA_REPORT       DEFAULT_LINS_FID3

#define DIAG_DATA_BUFF_SIZE     (128U)

#define FID_TABLE_INDEX_0       (0)
#define FID_TABLE_INDEX_1       (1)
#define FID_TABLE_INDEX_2       (2)
#define FID_TABLE_INDEX_3       (3)

#define FID_COLOR_CTRL_INDEX        (0)    // cmd Frame    LJM
#define FID_STATUS_FRAME_INDEX      (1)    //state frame   LJM
#define USER_DATA_REPORT_INDEX      (2)
#define EVENT_TRIGGERED_INDEX       (3)

/* ******************internal function declarations****************************/
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t * const frameInfo);
void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame);
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame);
void UnconditionalPublishedCmdsTxFinishedISR(uint8_t fid, uint8_t resvd);
void DiagnosticSleepRequestHandle(SleepRequestType_t type);
void BusWakeUpRequestHandle(BusWakeUpRequestResult_t result);


/* *******************global variants declarations*****************************/
static TaskState_t        linsTaskState = TASK_STATE_INIT;
static uint8_t diagDataRspBuff[DIAG_DATA_BUFF_SIZE];
static uint8_t diagDataRecvBuff[DIAG_DATA_BUFF_SIZE];
static uint8_t busWakeupRetryCount = 0U;

void busWakeupRetryTimerExpired(SoftTimer_t *timer);

static SoftTimer_t busWakeupRetryTimer = {
    .mode     = TIMER_ONE_SHOT_MODE,
    .interval = 2250U,
    .handler  = busWakeupRetryTimerExpired
};

static uint8_t eventTriggeredLedOn = FALSE;

/*  LIN command handle callbacks declarations  */
static ls_LinsFramesCallback_t linsFramesCallback ={
  UnconditionalSubscribedCmdsHandle,            /* received data from master */
  UnconditionalPublishedCmdsISR,                /* send data to master, it's an interrupt function, please fill the data as fast as possible */
  UnconditionalPublishedCmdsTxFinishedISR,      /* send data to master finished, it's an interrupt function, please fill the data as fast as possible  */
  DiagnosticSubscribedCmdsHandle,               /* Diagnostic sigle PDU and MultiPDU received data from master */
  NULL,                                         /* special functional NAD (0x7E) handler*/
  DiagnosticSleepRequestHandle,                 /* Diagnostic sleep request from master */
  //modulePowerSleep,
  BusWakeUpRequestHandle,                       /* Get the bus wake up result this would take effects after call API: ls_send_wake_up_bus_signal()*/
  NULL,                                         /* SAE J2602 -1 Configuration Messages */
  NULL,                                         /*  5.7.2.5 DNN Based Broadcast Messages */
};

/* **FID table declarations****/
//static LIN_Device_Frame_t UnconditionalCmdsTable[DEFAULT_LINS_FID_SIZE] = 
static LIN_Device_Frame_t UnconditionalCmdsTable[2] = 
{
  [FID_COLOR_CTRL_INDEX] = {
      .frame_id = 0x10,
      .msg_type = LIN_MSG_TYPE_RX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
  },
  
  [FID_STATUS_FRAME_INDEX] = {
      .frame_id = FID_STATUS_FRAME,   /* status management frame */
      .msg_type = LIN_MSG_TYPE_TX,
      .checksum = LIN_CHECKSUM_ENHANCED,
      .length   = LIN_BUFF_SIZE,
      .frameIsValid = TRUE,
  }
};


/* please don't do any changes for it would be used by lin stack */
LIN_Device_Frame_t *LINS_GetUcndFramesTable(uint8_t *tableItemsCount)
{
    *tableItemsCount = (uint8_t)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t));
    return UnconditionalCmdsTable;
}

/* 
  Diagnostic subscribed frame received from LIN master
*/
#if 0
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const diagReq)
{
    DiagRspInfo_t diagRsp={
      .sid      = diagReq->sid,
      .type     = diagReq->type,
      .payload  = diagDataRspBuff,
    };
    if (diagReq->type == PDU_TYPE_SINGLE_PDU)
    {
        if (diagReq->sid >= LIN_SID_ASSIGN_NAD && diagReq->sid <= LIN_SID_ASSIGN_FRAME_ID_RANGE)
        {
            /* handle Node configuration and Identification commands*/
            diagRsp.type = PDU_TYPE_SINGLE_PDU;
            if (LNCI_HandleNodeCfgIdentifyRequest(diagReq,&diagRsp)== TRUE)
            {
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
        else
        {
            if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG)
            {
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
                {
                    diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
                }
                if(APPL_PrepareLedParamRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }
            else if (diagReq->sid == LIN_SID_APP_DIAG_SINGLE_PDU_RECV_MULTI_RSP)
            {
                /*  handle diagnostic single pdu, multi response, user defined diagnostic frame here for example: */
                for (uint16_t i = 0U; i < 15U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)i;
                }
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 15U;
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);
            }
#ifdef BOOTLOADER_EN
            else if (diagReq->sid == LIN_SID_APP_DIAG_BOOT_MODE && diagReq->packLength == sizeof(EnterBootloaderFrame_t))
            {
                  EnterBootloaderFrame_t *boot = (EnterBootloaderFrame_t *)((void*)diagReq->payload);
                  if ( boot->command == BOOT_MODE_HANDSHAKE && boot->magicKey == BOOT_MODE_MAGIC_KEY_HANDSHAKE)
                  {
                      /*  Reset Chip   */
                      SYSCTRLA_REG_RETAIN0 = 0x05U;
                      PMUA_REG_CTRL.UPDATE = 1U;
                      while(PMUA_REG_CTRL.UPDATE == 1U);
                      CRGA_REG_RESETCTRL.HARDRSTREQ = 1U;
                  }
            }
#endif
        }
    }  //if (diagReq->type == PDU_TYPE_SINGLE_PDU)
    else   //first frame or serial frame
    {
        /*  handle diagnostic multi pdu received sigle pdu response, user defined diagnostic frame here   */
        if (diagReq->sid == LIN_SID_APP_DIAG_LED_SET_CONFIG)
        {
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength)
            {
                /* handle finished data here  */
                if ( APPL_HandleLedParamConfigRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);  
                }
            }
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_LED_GET_CONFIG)
        {
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength)
            {
                /*  prepare diagnostic single pdu, user defined diagnostic frame, here is for LED param configuration*/
                if(APPL_PrepareLedParamRequest(diagReq->sid,diagDataRecvBuff,&diagRsp) == TRUE)
                {
                    ls_handle_diagnostic_response(&diagRsp);
                }
            }       
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_SINGLE_RSP)
        {
            /*  handle diagnostic multi pdu received single pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength)
            {
                /* handle finished data here  */
                for (uint16_t i = 0U; i < 6U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)i;
                }
                /* handle response data here  */
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 5U;/* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);
            }
        }
        else if (diagReq->sid == LIN_SID_APP_DIAG_MULTI_PDU_RECV_MULTI_RSP)
        {
            /*  handle diagnostic multi pdu received multi pdu response, user defined diagnostic frame here   */
            for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
            {
                diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
            }
            /* Package received finished */
            if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength)
            {
                /* handle finished data here  */
                /* handle response data here  */
                for (uint16_t i = 0U; i < 15U; i++)
                {
                    diagDataRspBuff[i] = (uint8_t)(DIAG_DATA_BUFF_SIZE - i);
                }
                diagRsp.sid = diagReq->sid;
                diagRsp.packLength = 15U; /* Valid data length not includes sid*/
                diagRsp.type = (diagRsp.packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
                ls_handle_diagnostic_response(&diagRsp);  
            }
        }else{
          
        }
    }
}


#else
#include "SystemLinDiag.h"
void DiagnosticSubscribedCmdsHandle(const DiagReqInfo_t *const diagReq)
{
  uint16_t ret_DiagHandle = 0xff;
  DiagRspInfo_t diagRsp={
    .sid      = diagReq->sid,
    .type     = diagReq->type,
    .payload  = diagDataRspBuff,
  };
  
  if ((diagReq->sid >= LIN_SID_ASSIGN_NAD) && (diagReq->sid <= LIN_SID_ASSIGN_FRAME_ID_RANGE) && (diagReq->type == PDU_TYPE_SINGLE_PDU))
  {
      /* handle Node configuration and Identification commands*/
      diagRsp.type = PDU_TYPE_SINGLE_PDU;
      if (LNCI_HandleNodeCfgIdentifyRequest(diagReq,&diagRsp)== TRUE)
      {
          ls_handle_diagnostic_response(&diagRsp);
      }
  }
  else  // not LIN STD Diag
  {
    if(ls_read_nad() == 0xa0)  //检查是否转a0
    {
      switch(diagReq->sid)
      {
      case 0x22:
        ret_DiagHandle = systemLinDiag0x22Handle(diagReq, &diagRsp);
        break;
      case 0x2E:
        for (uint16_t i = 0; i < diagReq->frameDataLength; i++)
        {
            diagDataRecvBuff[i+diagReq->packCurrIndex] = diagReq->payload[i];
        }
        if ((diagReq->frameDataLength + diagReq->packCurrIndex)>=diagReq->packLength)
        {
            ret_DiagHandle = systemLinDiag0x2EHandle(diagReq, diagDataRecvBuff, &diagRsp);
        }
        break;
      case 0x27:
        ret_DiagHandle = systemLinDiag0x27Handle(diagReq, diagDataRecvBuff, &diagRsp);
        moduleFlashSave();
        break;
      }

      if(ret_DiagHandle == 0)
      {
        ls_handle_diagnostic_response(&diagRsp);
      }
    }
  }
   
}
#endif


/* 
  Received sleep command from lin master or bus idle timeout occurs
*/
void DiagnosticSleepRequestHandle(SleepRequestType_t type)
{
    //(void)ls_clr_go_to_sleep_flag();
    /* Set system to hibranate mode*/
    //PMU_EnterDeepSleepMode();   //if the fun work, need lin message to wake up.    
    
    if (sysStatus.sleepFlag == ESystemSleepAwake)
      sysStatus.sleepFlag = ESystemSleepPrepare;
    
}

void busWakeupRetryTimerExpired(SoftTimer_t *timer)
{
    ls_send_wake_up_bus_signal();
}

/* Called timeout after bus wake up 3 tries, or called when bus recovery */
void BusWakeUpRequestHandle(BusWakeUpRequestResult_t result)
{
    if (result == BUS_WAKEUP_REQ_RESULT_REQ_TIMEOUT){
        if (busWakeupRetryCount == 1U){
            busWakeupRetryCount++;
            SoftTimer_Start(&busWakeupRetryTimer);
        }else{
            busWakeupRetryCount = 0U;
        }
    }else{
        busWakeupRetryCount = 0U;
        SoftTimer_Stop(&busWakeupRetryTimer);
    }
}


void UnconditionalPublishedCmdsTxFinishedISR(uint8_t fid, uint8_t resvd)
{
    if ( fid == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id){
        (void)ls_ifc_clear_error_status();
        (void)ls_clr_error_code_log();
        (void)ls_clr_overrun_flag();
    }
}
/*  
  User defined published frame callback function which from LINS ISR
  please fill frame data[0-7] only, please don't do any changes on the left info except data[x]!!!!
*/
#if 0
void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame)
{
    /*   feed buffer to frame   */
    if (frame->frame_id == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id){
        /* status management frame */
        lin_status_t status = ls_ifc_read_status();
        frame->data[0] = (uint8_t)status.payload;
        frame->data[1] = LINS_GetPIDFromFID(status.pid);
        frame->data[2] = ls_read_error_code_log();
        frame->data[3] = MES_DataIntegrityError() | ((uint8_t)SAFM_GetBatteryState() << 2) | ((uint8_t)SAFM_GetChipTemperatureState() << 4);
        /* MUST BE 4 bytes Aligned  */
        uint32_t *ledPNState = frame->data32 + 1U;
        *ledPNState = 0U;
        for (uint8_t i = 0U; i < LED_NUM; i++){
            (*ledPNState) |= (uint32_t)SAFM_GetLedPNStateState((LedNum_t)i) << (i*2U);
        }
    }else if (frame->frame_id == UnconditionalCmdsTable[USER_DATA_REPORT_INDEX].frame_id){
        /* user defined data report */
        frame->data[0] = (APPL_GetLEDState()) | (eventTriggeredLedOn << 1U) | 0xF8U;
        for (uint8_t i = 1U; i < LIN_BUFF_SIZE; i++){
            frame->data[i] = 0xFFU;
        }
    }else if (frame->frame_id == UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].frame_id){
        /* event trigger data report */
        if (frame->eventTriggered == TRUE){
            frame->data[0] = LINS_GetPIDFromFID(UnconditionalCmdsTable[UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].linkedEventTriggerFidTableIndex].frame_id);/* transfer FID to PID  */
            frame->data[1] = (APPL_GetLEDState()) | (eventTriggeredLedOn << 1U) | 0xF8U;
            for (uint8_t i = 2U; i < LIN_BUFF_SIZE; i++){
                frame->data[i] = 0xFFU;
            }
        }
    }else{
        /* intent to empty   */
    }
}
#endif

void UnconditionalPublishedCmdsISR(LIN_Device_Frame_t *const frame)
{
    /*   feed buffer to frame   */
    if (frame->frame_id == UnconditionalCmdsTable[FID_STATUS_FRAME_INDEX].frame_id)
    {
        memset(l_LinData_Response.data, 0xff, LIN_BUFF_SIZE);  //not used as 0xff      
        systemLinCommRespUpdate();
        memcpy(frame->data, l_LinData_Response.data, LIN_BUFF_SIZE);   
    }
}


void LINS_EventTriggered_ISR(void)
{
    if (eventTriggeredLedOn == TRUE){
        eventTriggeredLedOn = FALSE;
    }else{
        eventTriggeredLedOn = TRUE;
    }
//    UnconditionalCmdsTable[EVENT_TRIGGERED_INDEX].eventTriggered = TRUE;   //暂时不用
}

uint8_t LINS_GetPIDFromFID(uint8_t frameId)
{
    uint8_t id = frameId;
    uint8_t P0,P1;
    P0 = (uint8_t)(((id >> 0)&0x01U)^((id >> 1)&0x01U)^((id >> 2)&0x01U) ^ ((id>> 4)&0x01U));
    P1 = (uint8_t)(~(((id >> 1)&0x01U)^((id >> 3)&0x01U)^((id >> 4)&0x01U) ^ ((id>> 5)&0x01U))) & 0x01U;
    id = frameId | (P0 << 6) | (P1 << 7);
    return id;
}


void LINS_SendWakeUpSignal_ISR(void)
{
    if (busWakeupRetryCount == 0U){
        busWakeupRetryCount++;
        ls_send_wake_up_bus_signal();
    }
}

/* 
  User defined subscribed frame received from LIN master
*/

LIN_Device_Frame_t l_LinData_Receive;  //LJM added
LIN_Device_Frame_t l_LinData_Response;

void UnconditionalSubscribedCmdsHandle(LIN_Device_Frame_t const *frame)
{
#if 0
    if (frame->frame_id == UnconditionalCmdsTable[FID_COLOR_CTRL_INDEX].frame_id){
        APPL_HandleColorControlCommands(frame);
    }
#endif
    if (frame->frame_id == UnconditionalCmdsTable[FID_COLOR_CTRL_INDEX].frame_id)
    {
      memset(&l_LinData_Receive, 0, sizeof(LIN_Device_Frame_t));
      memcpy(&l_LinData_Receive, frame, sizeof(LIN_Device_Frame_t));
    } 
    
}


void LINS_TaskHandler(void)
{
    switch(linsTaskState){
    case TASK_STATE_ACTIVE:
      break;
    case TASK_STATE_INIT:
      ls_register_services(LIN_PROTOCOL_LIN2_2A,UnconditionalCmdsTable, (l_u8)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t)), DIAG_DATA_BUFF_SIZE,&linsFramesCallback);
      (void)ls_set_tp_timeout(N_AS, N_CR);
      /* Note that please don't change the following glitch configuration!!!!*/
      (void)ls_set_lins_rx_glitch_filter_1st(0x00U,0x08U);
      (void)ls_set_lins_rx_glitch_filter_2nd(0x0AU,0x10U);
      (void)ls_set_lins_rx_glitch_filter_3rd(0x30U,0x30U);

      (void)l_sys_init();
      (void)ls_disable_lin_auto_sleep(TRUE);
      if(GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_2].ACTDET != 0U){
          GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_2].CLR = 1U;
          LINS_SendWakeUpSignal_ISR();
      }
      GPIO_RegisterIRQ(GPIO_PORT_2, GPIO_EDGE_FALLING, LINS_SendWakeUpSignal_ISR);
      linsTaskState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}


void SystemLINInit(void)
{
  ls_register_services(LIN_PROTOCOL_LIN2_2A,UnconditionalCmdsTable, (l_u8)(sizeof(UnconditionalCmdsTable)/sizeof(LIN_Device_Frame_t)), DIAG_DATA_BUFF_SIZE,&linsFramesCallback);
  (void)ls_set_tp_timeout(N_AS, N_CR);
  /* Note that please don't change the following glitch configuration!!!!*/
  (void)ls_set_lins_rx_glitch_filter_1st(0x00U,0x08U);
  (void)ls_set_lins_rx_glitch_filter_2nd(0x0AU,0x10U);
  (void)ls_set_lins_rx_glitch_filter_3rd(0x30U,0x30U);

  (void)l_sys_init();
  (void)ls_disable_lin_auto_sleep(TRUE);
  if(GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_2].ACTDET != 0U){
      GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_2].CLR = 1U;
      LINS_SendWakeUpSignal_ISR();
  }
  GPIO_RegisterIRQ(GPIO_PORT_2, GPIO_EDGE_FALLING, LINS_SendWakeUpSignal_ISR);

      
  UnconditionalCmdsTable[0].frame_id = 0x10;
  UnconditionalCmdsTable[0].msg_type = LIN_MSG_TYPE_RX;
  UnconditionalCmdsTable[0].checksum = LIN_CHECKSUM_ENHANCED;
  UnconditionalCmdsTable[0].length   = LIN_BUFF_SIZE;
  UnconditionalCmdsTable[0].frameIsValid = TRUE;

  UnconditionalCmdsTable[1].frame_id = savedConfig.singleAddr;   /* status management frame */
  UnconditionalCmdsTable[1].msg_type = LIN_MSG_TYPE_TX;
  UnconditionalCmdsTable[1].checksum = LIN_CHECKSUM_ENHANCED;
  UnconditionalCmdsTable[1].length   = LIN_BUFF_SIZE;
  UnconditionalCmdsTable[1].frameIsValid = TRUE;
      
}


#endif