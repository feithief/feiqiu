/***************************************************************************//**
 * @file		LinBus_Interface.h
 *
 * @creator	 sbai
 * @created	14.01.2015
 *
 * @brief Definition of the interface for the 'LIN BUS Layer'.
 *
 * An abstraction layer which encapsulates all the low-level
 * transport effort. Basically an event based API is exposed, which
 * can be used by some higher-level protocol layer to implement a LIN
 * stack.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup BUS BUS Layer
 * @{
 *
 * $Id$
 *
 * $Revision$
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor SE. To the extend permitted by law 
 * Elmos Semiconductor SE hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 ******************************************************************************/

#ifndef LINBUS_INTERFACE_H_
#define LINBUS_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinProto_FrameDescription.h"
#include "LinBus_Types.h"

#include "LinSNPD_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* LINBUS_INTERFACE_MODULE_API_VERSION history:
 *
 * Version  | Date (YYYY-MM-DD) | Who            | Info
 * ---------+-------------------+----------------+-----------------------------
 * 0x0103   | 2019-01-21        | RPY            | Added error code Lin_BusIf_ERR_CONCURRENT_MESSAGE
 *          |                   |                | Corrected some error code defines. 
 * 0x0102   | 2019-01-21        | RPY            | History started  
 * 
 * ****************************************************************************/
#define LINBUS_INTERFACE_MODULE_API_VERSION   0x0103u /**< @brief BUS Layer interface version. */

#define LIN_PID_MASK           ((Lin_uint8_t) 0x3Fu)  /**< @brief Mask for selecting ID part of a PID. */
#define LIN_INVALID_PID        (0xFFu)                /**< @brief A PID value which indicated a known invalid PID. */
#define LIN_INVALID_FRAMEID    (0xFFu)                /**< @brief A frame ID value which indicated a known invalid frame ID. */
#define LIN_DEFAULT_DATA_VALUE (0xFFu)                /**< @brief Initialization value for any LIN Message frame data. */
#define LIN_MAX_FRAMEID        63u                    /**< @brief Maximal value for the Frame ID. */
#define LIN_MAX_FRAMEID_CNT    (LIN_MAX_FRAMEID + 1u) /**< @brief Maximal count of Frame IDs. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* *****************************************************************************
 * Interface callback function types
 * ****************************************************************************/

/** @addtogroup LinBusIfIfFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Initialization' interface function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busEnvDataSze       Size of the reserved RAM for BUS environment
 *                                data.
 * @param[in] busCbFuns           Pointer to BUS Layer callback function.
 *                                struct. Implemented in the higher layer or
 *                                user application.
 *                                (LinBusTrIf_sCallbackFunctions).
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 * @param[in] init_baudrate       Initial baudrate.
 * @param[in] genericImpCfgData   Pointer to implementation dependent
 *                                configuration data for the BUS Layer
 *                                (LinBusImp_sCfgData).
 *
 * @return  LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the BUS Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinBusIf_InitializationIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                     LinBusIf_EnvDataSze_t busEnvDataSze,
                                                     LinBusIf_cpCallbackFunctions_t busCbFuns,
                                                     LinBusIf_pGenericCbCtxData_t genericBusCbCtxData,
                                                     LinBusIf_Baudrate_t init_baudrate,
                                                     LinBusIf_pGenericImpCfgData_t genericImpCfgData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Get Sub Interface' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 * @param[in] interfaceId       Sub-Interface ID.
 * @param[out] ifThisPtr        Address (pointer) of an instance of the
 *                              LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          @p ifThisPtr could be set.
 *
 * Acquires the desired sub-interface (@p interfaceId) and writes the address
 * of its This-Pointer to @p ifThisPtr.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinBusIf_GetSubInterfaceIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                      Lin_eInterfaceIds_t interfaceId, 
                                                      Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Task' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * The Task function has to be called periodically to process scheduled task of
 * the BUS Layer.
 *
 ******************************************************************************/
typedef void (*LinBusIf_TaskIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Get milliseconds' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * @return The milliseconds counter value.
 *
 * Get the  milliseconds counter value.
 *
 ******************************************************************************/
typedef LinBusIf_Tick_t (*LinBusIf_GetMillisecondsIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Shutdown' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * Shutdown the LIN communication. It disconnects the LIN driver from the
 * physical bus.
 *
 ******************************************************************************/
typedef void (*LinBusIf_ShutdownIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Go to sleep' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * Set's the LIN driver to the Sleep-Mode. It doesn't respond to LIN frames
 * anymore. The same mode will be set if the idle timeout has been reached.
 *
 ******************************************************************************/
typedef void (*LinBusIf_GoToSleepIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Get state' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * @return  The state of the BUS Layer.
 *
 * Get the state of the BUS Layer.
 *
 ******************************************************************************/
typedef LinBusIf_eState_t (*LinBusIf_GetStateIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Change baudrate' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 * @param[in] new_baudrate      Desired baud rate.
 * @param[in] try_only          Set to LIN_TRUE to try to set the new baudrate
 *                              first.
 *
 * @return Returns LIN_TRUE if the baud rate has been set successfully.
 *
 * Set the LIN bus baud rate.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinBusIf_ChangeBaudrateIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                     LinBusIf_Baudrate_t new_baudrate, Lin_Bool_t try_only);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Change idle timeout' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 * @param[in] timeout           Desired idle timeout.
 *
 * Set the LIN idle timeout.
 *
 * @see LIN 2.2 Specification - 2.6.1 SLAVE COMMUNICATION STATE DIAGRAM
 *
 ******************************************************************************/
typedef void (*LinBusIf_ChangeIdleTimeoutIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_TimeOut_t timeout);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Restart' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * Restarts the BUS Layer.
 *
 ******************************************************************************/
typedef void (*LinBusIf_RestartIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Wakeup cluster' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 * Sends a wakeup signal for the LIN cluster.
 *
 ******************************************************************************/
typedef void (*LinBusIf_WakeupClusterIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Send header' interface function.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 * @param[in] frameID           Frame ID' for frame header.
 *
 * @return LIN_TRUE, if the header has been send to the LIN bus.
 *
 * Sends a LIN frame header to the bus. (Normally only for Master-functionality)
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinBusIf_SendHeaderIfFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * @brief BUS Layer interface functions struct.
 *
 * Collective struct for the BUS Layer interface functions.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This interface functions can be dereference outside of this project scope. It should not be hidden. */
struct LinBusIf_sInterfaceFunctions                        
{
    Lin_Version_t                     InterfaceVersion;   /**< @brief BUS Layer interface version.*/

    LinBusIf_InitializationIfFun_t    Initialization;     /**< @brief Pointer to BUS Layer 'Initialization' interface function. (@ref LinBusIf_InitializationIfFun_t) @copydetails LinBusIf_InitializationIfFun_t */
    LinBusIf_GetSubInterfaceIfFun_t   GetSubInterface;    /**< @brief Pointer to BUS Layer 'Get Sub-Interface' interface function. (@ref LinBusIf_GetSubInterfaceIfFun_t) @copydetails LinBusIf_GetSubInterfaceIfFun_t */
    LinBusIf_TaskIfFun_t              Task;               /**< @brief Pointer to BUS Layer 'Task' interface function. (@ref LinBusIf_TaskIfFun_t) @copydetails LinBusIf_TaskIfFun_t */
    LinBusIf_GetMillisecondsIfFun_t   GetMilliseconds;    /**< @brief Pointer to BUS Layer 'Get milliseconds' interface function. (@ref LinBusIf_GetMillisecondsIfFun_t) @copydetails LinBusIf_GetMillisecondsIfFun_t */
    LinBusIf_ShutdownIfFun_t          Shutdown;           /**< @brief Pointer to BUS Layer 'Shutdown' interface function. (@ref LinBusIf_ShutdownIfFun_t) @copydetails LinBusIf_ShutdownIfFun_t */
    LinBusIf_GoToSleepIfFun_t         GoToSleep;          /**< @brief Pointer to BUS Layer 'Go to sleep' interface function. (@ref LinBusIf_GoToSleepIfFun_t) @copydetails LinBusIf_GoToSleepIfFun_t */
    LinBusIf_GetStateIfFun_t          GetState;           /**< @brief Pointer to BUS Layer 'Get state' interface function. (@ref LinBusIf_GetStateIfFun_t) @copydetails LinBusIf_GetStateIfFun_t */
    LinBusIf_ChangeBaudrateIfFun_t    ChangeBaudrate;     /**< @brief Pointer to BUS Layer 'Change baudrate' interface function. (@ref LinBusIf_ChangeBaudrateIfFun_t) @copydetails LinBusIf_ChangeBaudrateIfFun_t */
    LinBusIf_ChangeIdleTimeoutIfFun_t ChangeIdleTimeout;  /**< @brief Pointer to BUS Layer 'Change idle timeout' interface function. (@ref LinBusIf_ChangeIdleTimeoutIfFun_t) @copydetails LinBusIf_ChangeIdleTimeoutIfFun_t */
    LinBusIf_RestartIfFun_t           Restart;            /**< @brief Pointer to BUS Layer 'Restart' interface function. (@ref LinBusIf_RestartIfFun_t) @copydetails LinBusIf_RestartIfFun_t */
    LinBusIf_WakeupClusterIfFun_t     WakeupCluster;      /**< @brief Pointer to BUS Layer 'Wakeup cluster' interface function. (@ref LinBusIf_WakeupClusterIfFun_t) @copydetails LinBusIf_WakeupClusterIfFun_t */
    LinBusIf_SendHeaderIfFun_t        SendHeader;         /**< @brief Pointer to BUS Layer 'Send header interface function. (@ref LinBusIf_SendHeaderIfFun_t) @copydetails LinBusIf_SendHeaderIfFun_t */
};

/** @} LinBusIfIfFunDefs */

/* *****************************************************************************
 * BUS Layer callback function types
 ******************************************************************************/
/** @addtogroup LinBusIfCbFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Error' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] error               Occurred error.
 * @param[in] frameID             Related Frame ID if known, otherwise
 *                                "LINDRV_INVALID_FRAMEID".
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * Indicates an error in the BUS Layer and underlying layers of the LIN
 * Driver.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_errorcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinBusIf_ErrorCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                      LinBusIf_cpInterfaceFunctions_t busIfFuns, 
                                      LinBusIf_Error_t error, 
                                      LinBusIf_FrameID_t frameID, 
                                      LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/****************************************************************************//**
 * @brief Typedef of BUS Layer 'Restart' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] genericBusCbCtxDatv Pointer to BUS Layer callback context data.
 *
 * This callback is invoked once the LIN driver performs a restart.
 * (i.e. typically after an error, or wakeup )
 *
 * @par "Call Description 1:"
 * @mscfile msc_bus_restartcbcalled_1.dox
 * @n
 * @par "Call Description 2:"
 * @mscfile msc_bus_restartcbcalled_2.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinBusIf_RestartCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                        LinBusIf_cpInterfaceFunctions_t busIfFuns,
                                        LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Baud Rate Measurement Done' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] exp_baudrate        The nominal (configured) baud rate.
 * @param[in] prev_baudrate       The baud rate used for previous message, and as the
 *                                initial baud rate for sync measurement.
 * @param[in] meas_baudrate       The actual measured baud rate.
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * @return How the BUS Layer should proceed after the reception of the LIN frame.
 *
 * This callback event indicates successful measurement of a sync character.
 * The measured baud rate (in terms of divider value) is provided as argument.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_measdonecbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eMeasAction_t (*LinBusIf_MeasDoneCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                           LinBusIf_cpInterfaceFunctions_t busIfFuns, 
                                                           LinBusIf_Baudrate_t exp_baudrate,
                                                           LinBusIf_Baudrate_t prev_baudrate, 
                                                           LinBusIf_Baudrate_t meas_baudrate,
                                                           LinBusIf_BreakLen_t meas_break_len,
                                                           LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Frame ID Received' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] frameID             Received Frame ID.
 * @param[in,out] rxtx_buf        Pointer-Pointer to Receive-/Transmitbuffer were
 *                                the data shall be stored.
 * @param[in] buf_len             Length of the message buffer.
 * @param[in] crcType             Expected CRC type of the received frame
 *                                (LinBusIf_eCrcType).
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * @return  The treatment for this frame ID. Transmit data, receive data or ignore
 *          the frame ID? (LinBusIf_eFrameIdAction)
 *
 * This callback event indicates successful reception of a Frame ID.
 *
 * Pass the address of the pointer to the message buffer @p rxtx_buf. The passed
 * buffer has to stay valid until a LinBusIf_ReceiveDoneCbFun_t or
 * LinBusIf_TransmitDoneCbFun_t event has occurred with corresponding buffer
 * pointer. Returned buffers should incorporate the entire message including
 * checksum.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_receivelinmsg_no_err.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eFrameIdAction_t (*LinBusIf_FrameIdReceivedCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                                     LinBusIf_cpInterfaceFunctions_t busIfFuns, 
                                                                     LinBusIf_FrameID_t frameID,
                                                                     LinBusIf_ppData_t rxtx_buf, 
                                                                     LinBusIf_pBufLength_t buf_len,
                                                                     LinBusIf_pCrcType_t crcType,
                                                                     LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Frame Receive Done' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] frameID             Received Frame ID.
 * @param[in] rx_buf              Pointer to received data.
 * @param[in] buf_len             The total buffer length.
 * @param[in] error               Error code, if an error occurred.
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * @return  The behaviour of the BUS Layer after the processing of this message
 *          body. Set the BUS to Sleep-Mode or continue normal operation.
 *          (@ref LinBusIf_eMsgAction)
 *
 * This callback indicates reception of message body (incl. CRC).
 *
 * If @p frameID is LINDRV_INVALID_FRAMEID reception has failed and no known PID exists.
 * Likely an error callback has occurred before, indicating the error condition. The
 * callback is invoked in this case only to give the protocol layer the chance to
 * appropriate release 'rx_buf'.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_receivelinmsg_no_err.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eMsgAction_t (*LinBusIf_ReceiveDoneCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                             LinBusIf_cpInterfaceFunctions_t busIfFuns, 
                                                             LinBusIf_FrameID_t frameID,
                                                             LinBusIf_pData_t rx_buf, 
                                                             LinBusIf_BufLength_t buf_len,
                                                             LinBusIf_Error_t error, 
                                                             LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Message Transmit Done' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] frameID             Frame ID of message that has been transmitted.
 * @param[out] tx_buf             Pointer to transmit buffer.
 * @param[in] buf_len             The total buffer length.
 * @param[in] error               Parameter which holds the error code, if an error occurred.
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * @return  The behavior of the BUS Layer after the processing of this message body.
 *          Set the BUS to Sleep-Mode or continue normal operation. (LinBusIf_eMsgAction)
 *
 * This callback indicates completed transmission of message body (incl. CRC).
 *
 * If @p frameID is LINDRV_INVALID_FRAMEID transmission has failed the PID is
 * not known anymore. Likely an error callback has occurred before, indicating
 * the error condition. The callback is therefore invoked to give the higher
 * layers the chance to appropriate release 'tx_buf'.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_transmitlinmsg_no_err.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eMsgAction_t (*LinBusIf_TransmitDoneCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                              LinBusIf_cpInterfaceFunctions_t busIfFuns, 
                                                              LinBusIf_FrameID_t frameID,
                                                              LinBusIf_pData_t tx_buf, 
                                                              LinBusIf_BufLength_t buf_len,
                                                              LinBusIf_Error_t error, 
                                                              LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Idle' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * @return  How the BUS Layer should process after the occurrence of the Idle-event.
 *          (@ref LinBusIf_eIdleAction)
 *
 * This event indicates that there was no bus action for specified idle duration.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_idlecbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef LinBusIf_eIdleAction_t (*LinBusIf_IdleCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                                       LinBusIf_cpInterfaceFunctions_t busIfFuns,
                                                       LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief Typedef of BUS Layer 'Wakeup' callback function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busIfFuns           Pointer to the BUS Layer interface function struct.
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 *
 * A wakeup event (basically a break start while the bus drive is in sleep mode)
 * occurred.
 *
 * @par "Call Description:"
 * @mscfile msc_bus_wakupcbcalled.dox
 * @n
 *
 ******************************************************************************/
typedef void (*LinBusIf_WakeupCbFun_t)(LinBusIf_pGenericEnvData_t genericBusEnvData,
                                       LinBusIf_cpInterfaceFunctions_t busIfFuns,
                                       LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * @brief LIN BUS Layer callback functions struct.
 *
 * The set of callbacks which should be invoked in the higher layers.
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This interface functions can be dereference outside of this project scope. It should not be hidden. */
struct LinBusIf_sCallbackFunctions
{
  Lin_Version_t                   CallbackVersion;  /**< @brief Callback Version */

  LinBusIf_ErrorCbFun_t           Error;            /**< @brief Pointer to BUS Layer 'Error' callback function. (@ref LinBusIf_ErrorCbFun_t) @copydetails LinBusIf_ErrorCbFun_t */
  LinBusIf_RestartCbFun_t         Restart;          /**< @brief Pointer to BUS Layer 'Restart' callback function. (@ref LinBusIf_RestartCbFun_t) @copydetails LinBusIf_RestartCbFun_t */
  LinBusIf_MeasDoneCbFun_t        MeasDone;         /**< @brief Pointer to BUS Layer 'MeasDone' callback function. (@ref LinBusIf_MeasDoneCbFun_t) @copydetails LinBusIf_MeasDoneCbFun_t */
  LinBusIf_FrameIdReceivedCbFun_t FrameIdReceived;  /**< @brief Pointer to BUS Layer 'FrameIdReceived' callback function. (@ref LinBusIf_FrameIdReceivedCbFun_t) @copydetails LinBusIf_FrameIdReceivedCbFun_t */
  LinBusIf_ReceiveDoneCbFun_t     ReceiveDone;      /**< @brief Pointer to BUS Layer 'ReceiveDone' callback function. (@ref LinBusIf_ReceiveDoneCbFun_t) @copydetails LinBusIf_ReceiveDoneCbFun_t */
  LinBusIf_TransmitDoneCbFun_t    TransmitDone;     /**< @brief Pointer to BUS Layer 'TransmitDone' callback function. (@ref LinBusIf_TransmitDoneCbFun_t) @copydetails LinBusIf_TransmitDoneCbFun_t */
  LinBusIf_IdleCbFun_t            Idle;             /**< @brief Pointer to BUS Layer 'Idle' callback function. (@ref LinBusIf_IdleCbFun_t) @copydetails LinBusIf_IdleCbFun_t */
  LinBusIf_WakeupCbFun_t          Wakeup;           /**< @brief Pointer to BUS Layer 'Wakeup' callback function. (@ref LinBusIf_WakeupCbFun_t) @copydetails LinBusIf_WakeupCbFun_t */
};

/** @} LinBusIfCbFunDefs */

/***************************************************************************//**
 * @brief Object-like This-pointer to connect the BUS Layer to higher layers.
 ******************************************************************************/
struct LinBusIf_sThis
{
    LinBusIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the BUS Layer interface function struct. (LinBusIf_sInterfaceFunctions) */
    LinBusIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved BUS Layer environment data. */
};

/***************************************************************************//**
 * @brief LIN BUS Layer interface configuration parameter.
 *
 * Data needed for initialization of the BUS Layer.
 ******************************************************************************/
struct LinBusIf_sInitParam
{
    LinBusIf_cpInterfaceFunctions_t IfFunsTbl;   /**< @brief Pointer to the constant BUS Layer interface function struct. */
    LinBusIf_pGenericEnvData_t      envData;     /**< @brief Pointer to reserved BUS Layer environment data. */
    LinBusIf_BufLength_t            EnvDataLen;  /**< @brief Size of the reserved RAM for BUS Layer environment data. */
    LinBusIf_Baudrate_t             baudrate;    /**< @brief Initial baudrate */
    LinBusIf_pGenericImpCfgData_t   ImpCfgData;  /**< @brief Pointer to implementation dependent configuration data for the TRANS Layer. (LinTransImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINBUS_INTERFACE_H_ */

/*! @} *//* BUS */
/*! @} *//* ELMOS_LIN_DRIVER */
