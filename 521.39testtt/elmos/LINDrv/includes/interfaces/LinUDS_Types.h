/***************************************************************************//**
 * @file      LinUDS_Types.h
 *
 * @creator   sbai
 * @created   25.03.2015
 *
 * @brief     Definitions of basic data types for the 'LIN UDS Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG_UDS DIAG_UDS Layer
 * @{
 *
 * $Id: LinUDS_Types.h 8305 2021-02-03 13:29:35Z poe $
 *
 * $Revision: 8305 $
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

#ifndef LINUDS_TYPES_H_
#define LINUDS_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinTrans_Types.h"

/* ****************************************************************************/
/* *********** GENERAL DEFINES, ENUMS, STRUCTS, DATA TYPES, ETC. **************/
/* ****************************************************************************/
typedef Lin_pvoid_t      LinUDSIf_pGenericEnvData_t;    /**< Generic pointer to environment data of the UDS layer module. */
typedef Lin_EnvDataSze_t LinUDSIf_EnvDataSze_t;         /**< LIN UDS layer data type for the environment data length. */
typedef Lin_pvoid_t      LinUDSIf_pGenericImpCfgData_t; /**< Generic pointer to configuration parameter of the specific UDS Layer implementation */
typedef Lin_pvoid_t      LinUDSIf_pGenericCbCtxData_t;  /**< Pointer to UDS callback context data. */

typedef Lin_Error_t  LinUDSIf_Error_t;                  /**< Generic data types for UDS layer error codes. */

typedef Lin_uint8_t       LinUDSIf_Data_t;   /**< Basic UDS data type. */
typedef LinUDSIf_Data_t*  LinUDSIf_pData_t;  /**< Pointer to LinUDSIf_Data_t. */
typedef LinUDSIf_Data_t** LinUDSIf_ppData_t; /**< Pointer-Pointer to LinUDSIf_Data_t. */

typedef Lin_BufLength_t        LinUDSIf_BufLength_t;   /**< UDS layer data type of length values. */
typedef LinUDSIf_BufLength_t*  LinUDSIf_pBufLength_t;  /**< Pointer to LinUDSIf_BufLength_t. */
typedef LinUDSIf_BufLength_t** LinUDSIf_ppBufLength_t; /**< Pointer-Pointer to LinUDSIf_BufLength_t. */

struct         LinUDSIf_sInterfaceFunctions;                                     /**< Forward declaration of LIN UDS layer interface functions. */
typedef struct LinUDSIf_sInterfaceFunctions     LinUDSIf_sInterfaceFunctions_t;  /**< Typedef of LinUDSIf_sInterfaceFunctions. */
typedef        LinUDSIf_sInterfaceFunctions_t*  LinUDSIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinUDSIf_sInterfaceFunctions. */
typedef const  LinUDSIf_sInterfaceFunctions_t*  LinUDSIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinUDSIf_sInterfaceFunctions. */

struct         LinUDSIf_sCallbackFunctions;                                    /**< Forward declaration of LIN UDS layer callback functions. */
typedef struct LinUDSIf_sCallbackFunctions     LinUDSIf_sCallbackFunctions_t;  /**< Typedef of LinUDSIf_sCallbackFunctions. */
typedef        LinUDSIf_sCallbackFunctions_t*  LinUDSIf_pCallbackFunctions_t;  /**< Typedef of pointer to LinUDSIf_sCallbackFunctions. */
typedef const  LinUDSIf_sCallbackFunctions_t*  LinUDSIf_cpCallbackFunctions_t; /**< Typedef of constant pointer to LinUDSIf_sCallbackFunctions. */

struct         LinUDSIf_sThis;                     /**< Forward declaration of LIN UDS layer This-Pointer. */
typedef struct LinUDSIf_sThis    LinUDSIf_sThis_t; /**< Typedef for LinUDSIf_sThis. */
typedef        LinUDSIf_sThis_t* LinUDSIf_pThis_t; /**< Typedef of pointer to LinUDSIf_sThis. */

struct         LinUDSIf_sInitParam;                          /**< Forward declaration of LIN UDS layer initialization parameter struct. */
typedef struct LinUDSIf_sInitParam    LinUDSIf_sInitParam_t; /**< Typedef of LinUDSIf_sInitParam. */
typedef        LinUDSIf_sInitParam_t* LinUDSIf_pInitParam_t; /**< Typedef of pointer to LinUDSIf_sInitParam. */

typedef LinTransIf_NAD_t LinUDSIf_NAD_t; /**< Redefinition of the data type for the 'Node Address' of the TRANS layer (NAD)
                                              (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */
typedef LinTransIf_SID_t LinUDSIf_SID_t; /**< Redefinition of the data type for SID of the TRANS layer (Service Identifier)
                                              (See also: LIN 2.2a Specification - Chapter 3.2.1.5 SID) */

typedef Lin_uint32_t LinUDSIf_Timeout_t; /**< Data type for timeout values of the UDS layer. */

typedef Lin_uint8_t LinUDSIf_TimerNum_t; /**< Data type for timer number */

#define LinUDSIf_TIMER_NUM_INVALID 0xff  /**< Define for an invalid timer */

/***************************************************************************//**
 * Enumerator for the timer type
 ******************************************************************************/
enum LinUDSIf_eTimerType
{
  LinUDSIf_TimeType_DISABLED      = 0,   /**< This type can be used to disable an previously setup timer  */
  
  LinUDSIf_TimeType_SINGLE_SHOT   = 1,   /**< This kind of timer will start running on setup and will trigger once  */
  LinUDSIf_TimeType_PERIODIC      = 2,   /**< This kind of timer will start running on setup and will be restarted on each trigger */
  
  LinUDSIf_TimeType_S3_TIMER      = 3,   /**< This kind of timer will be retriggered on arrival of each service request (regardles if valid or not) */ 
  
};

typedef enum LinUDSIf_eTimerType LinUDSIf_eTimerType_t;

/***************************************************************************//**
 * Enumerator for the two types of NAD of the UDS layer.
 ******************************************************************************/
enum LinUDSIf_eNADType
{
  LinUDSIf_NADType_NORMAL  = 0, /**< Normal/Actually NAD. */
  LinUDSIf_NADType_INITIAL = 1  /**< Initial/Stored NAD. */
};

typedef enum LinUDSIf_eNADType LinUDSIf_eNADType_t; /**< Typedef of LinUDSIf_eNADType. */

/***************************************************************************//**
 * @brief Defines the callback context data which shall be passed to the
 *        'Frame ID processed' callback.
 ******************************************************************************/
typedef enum LinUDSIf_eLookupIdProcCbCtxDataType
{
  LinUDSIf_LookupIdPrcCbCtxDataType_NONE         = 0, /**< None (LIN_NULL), will be passed to the callback function. */
  LinUDSIf_LookupIdPrcCbCtxDataType_PER_CALLBACK = 1, /**< The callback context attached to the 'Frame Description' will be passed to the callback function. @see LinUDSIf_sFrameDescription */
  LinUDSIf_LookupIdPrcCbCtxDataType_PER_DESCLST  = 2, /**< The callback context attached to the 'Frame Description List' will be passed to the callback function. @see LinProtoIf_AddFrameDescLstIfFun_t */
  LinUDSIf_LookupIdPrcCbCtxDataType_PER_ENVDATA  = 3  /**< The callback context attached to the 'UDS Environment Data' will be passed to the callback function. @see */    
}LinUDSIf_LookupIdProcCbCtxDataType_t;

/***************************************************************************//**
 * @brief Typedef of special UDS Layer 'Append Data To Message Buffer' interface
 *        function.
 *
 * @param genericUdsEnvData[in] Pointer to reserved UDS Layer environment data.
 * @param data[in]              Pointer to data which shall be appened to the
 *                              message buffer.
 * @param dataLen[in]           Data length to append.
 *
 * This interface function will be passed as parameter in callback function
 * which needs the possibility for he user to pass specific data to the UDS
 * message buffer.
 *
 * @return LIN_TRUE if data has been written successfully to the message buffer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_AppendDataToMsgBuffer_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_pData_t data, LinUDSIf_BufLength_t dataLen);

/***************************************************************************//**
 * LIN UDS Layer error enumerator
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.
 * Conflicts in signedness. 
 * Always make sure unsigned values are defined and used.
 *
 ******************************************************************************/
// PRQA S 4521 ++
typedef enum LinUDSIf_eErrorCodes
{
  LinUDSIf_ERR_NO_ERROR                                 = Lin_NO_ERROR,                  /**< No error at all. */
  LinUDSIf_ERR_INIT                                     = Lin_ERROR_AREA_DIAG_UDS + 0u,  /**< Initialization error */
  LinUDSIf_ERR_INVALID_NAD                              = Lin_ERROR_AREA_DIAG_UDS + 1u,  /**< invalid NAD specified */
  LinUDSIf_ERR_IN_CORRECT_MESSAGE_LENGTH_INVALID_FORMAT = Lin_ERROR_AREA_DIAG_UDS + 2u,  /**< The data length mismatch for the service. */
  LinUDSIf_ERR_RBI_DEF                                  = Lin_ERROR_AREA_DIAG_UDS + 3u,  /**< Error at "Read by Identifier" definition. */
  LinUDSIf_ERR_RBI_NOT_FOUND                            = Lin_ERROR_AREA_DIAG_UDS + 4u,  /**< No entry for this identifier is found. */
  LinUDSIf_ERR_RBI_DATA_LEN                             = Lin_ERROR_AREA_DIAG_UDS + 5u,  /**< Mismatch between initialized and appended data length for a response. */
  LinUDSIf_ERR_RDI_DEF                                  = Lin_ERROR_AREA_DIAG_UDS + 6u,  /**< Error at "Record Data Identifier" definition. */
  LinUDSIf_ERR_RDI_NOT_FOUND                            = Lin_ERROR_AREA_DIAG_UDS + 7u,  /**< No entry for this "Record Data Identifier" is found. */
  LinUDSIf_ERR_RDI_DATA_LEN                             = Lin_ERROR_AREA_DIAG_UDS + 8u,  /**< Record Data Identifier length mismatch. */
  LinUDSIf_ERR_SEED_LEN_MISMATCH                        = Lin_ERROR_AREA_DIAG_UDS + 9u,  /**< Mismatch between configured and appended seed Length. */
  LinUDSIf_ERR_TRANS_LAYER                              = Lin_ERROR_AREA_DIAG_UDS + 10u, /**< Error using Transport Layer. */
  LinUDSIf_ERR_ROUTINEID_DEF                            = Lin_ERROR_AREA_DIAG_UDS + 11u, /**< Error at "Routine Identifier" definition. */
  LinUDSIf_ERR_ROUTINEID_DATA_LEN                       = Lin_ERROR_AREA_DIAG_UDS + 12u, /**< Routine Identifier length mismatch. */
  LinUDSIf_ERR_IMPL_ERROR_AREA                          = Lin_ERROR_AREA_DIAG_UDS + (LIN_ERROR_AREA_SIZE/2u) /**< Any additional implementation specific error codes start here. */
} LinUDSIf_eErrorCodes_t;
// PRQA S 4521 --

#define LINUDSIF_RSID_OFFSET         0x40u     /**< RSID offset. Added up to the SID for the response. */

#define LINUDSIF_SUPPLIER_ID_WILDCARD  0x7FFFu /**< Supplier ID wildcard. */
#define LINUDSIF_FUNCTION_ID_WILDCARD  0xFFFFu /**< Function ID wildcard. */

/***************************************************************************//**
 * Enumerator for the Service IDs of the UDS layer.
 ******************************************************************************/
enum LinUDSIf_ServiceID
{
  /* LIN 2.2 specification services. */
  LinUDSIf_SID_AssignNAD                      = 0xB0u, /**< Assign NAD LIN Diagnostic Service */
  LinUDSIf_SID_AssignFrameID                  = 0xB1u, /**< Assign Frame ID Diagnostic Service */
  LinUDSIf_SID_ReadByIdentifier               = 0xB2u, /**< Read By Identifier LIN Diagnostic Service */
  LinUDSIf_SID_ConditionalChangeNAD           = 0xB3u, /**< Conditional Change NAD LIN Diagnostic Service */
  LinUDSIf_SID_TargetedReset                  = 0xB5u, /**< Targeted Reset (SAE-J2602 only) */
  LinUDSIf_SID_SaveConfiguration              = 0xB6u, /**< Save Configuration LIN Diagnostic Service */
  LinUDSIf_SID_AssignFrameIDRange             = 0xB7u, /**< Assign Frame ID Range LIN Diagnostic Service */
  LinUDSIf_SID_SNPD                           = 0xB8u, /**< SNPD Service */
  /* Diagnostic and Communication Management functional unit  */
  LinUDSIf_SID_DiagnosticSessionControl       = 0x10u, /**< Diagnostic Session Control UDS Service */
  LinUDSIf_SID_ECUReset                       = 0x11u, /**< ECU Reset UDS Service */
  LinUDSIf_SID_SecurityAccess                 = 0x27u, /**< Security Access UDS Service */
  LinUDSIf_SID_CommunicationControl           = 0x28u, /**< Communication Control UDS Service */
  LinUDSIf_SID_TesterPresent                  = 0x3Eu, /**< Tester Present UDS Service */
  LinUDSIf_SID_ControlDTCSetting              = 0x85u, /**< Response On Event UDS Service */
  LinUDSIf_SID_ResponseOnEvent                = 0x86u, /**< Control DTC Setting UDS Service */
  /* Data Transmission functional unit */
  LinUDSIf_SID_ReadDataByIdentifier           = 0x22u, /**< Read Data By Identifier UDS Service */
  LinUDSIf_SID_WriteDataByIdentifier          = 0x2Eu, /**< Write Data By Identifier UDS Service */
  LinUDSIf_SID_ReadMemoryByAddress            = 0x23u,
  LinUDSIf_SID_WriteMemoryByAddress           = 0x3Du,
  /* Stored data transmission functional unit */
  LinUDSIf_SID_ClearDiagnosticInformation     = 0x14u, /**< Clear Diagnostic Information UDS Service */
  LinUDSIf_SID_ReadDTCInformation             = 0x19u, /**< Read DTC Information UDS Service */
  /* Input/Output control functional unit */
  LinUDSIf_SID_InputOutputControlByIdentifier = 0x2Fu, /**< Input Output Control By Identifier UDS Service */
  /* Remote activation of routine functional unit */
  LinUDSIf_SID_RoutineControl                 = 0x31u, /**< Routine Control UDS Service */
  /* Upload/Download functional unit */
  LinUDSIf_SID_RequestDownload                = 0x34u, /**< Request Download UDS Service */
  LinUDSIf_SID_RequestUpload                  = 0x35u, /**< Request Upload UDS Service */
  LinUDSIf_SID_TransferData                   = 0x36u, /**< Transfer Data UDS Service */
  LinUDSIf_SID_RequestTransferExit            = 0x37u, /**< Request Transfer Exit UDS Service */
};

typedef enum LinUDSIf_ServiceID LinUDSIf_ServiceID_t; /**< Typedef of LinUDSIf_ServiceID. */

#define LINUDSIF_NEG_RESP_RSID  0x7Fu /**< RSID of negative responses. */
#define LINUDSIF_NEG_RESP_LEN   2u    /**< Data length of negative responses. */

typedef Lin_uint16_t    LinUDSIf_NRC_t;  /**< Generic NRC (Negative Response Code) data type. */
typedef LinUDSIf_NRC_t* LinUDSIf_pNRC_t; /**< Pointer to generic NRC (Negative Response Code) data type. */

/***************************************************************************//**
 * Negative Response Codes
 ******************************************************************************/
enum LinUDSIf_eNegativeResponseCode
{
  LinUDIf_NRC_General_Reject                                           = 0x10u,
  LinUDIf_NRC_Service_Not_Supported                                    = 0x11u,
  LinUDIf_NRC_Sub_Function_Not_Supported                               = 0x12u,
  LinUDIf_NRC_In_Correct_Message_Length_Invalid_Format                 = 0x13u,
  LinUDIf_NRC_Response_Too_Long                                        = 0x14u,
  LinUDIf_NRC_Busy_Repeat_Request                                      = 0x21u,
  LinUDIf_NRC_Conditions_Not_Correct                                   = 0x22u,
  LinUDIf_NRC_Request_Sequence_Error                                   = 0x24u,
  LinUDIf_NRC_No_Response_From_Subnet_Component                        = 0x25u,
  LinUDIf_NRC_Failure_Prevents_Execution_Of_Requested_Action           = 0x26u,
  LinUDIf_NRC_Request_Out_Of_Range                                     = 0x31u,
  LinUDIf_NRC_Security_Access_Denied                                   = 0x33u,
  LinUDIf_NRC_Invalid_Key                                              = 0x35u,
  LinUDIf_NRC_Exceed_Number_Of_Attempts                                = 0x36u,
  LinUDIf_NRC_Required_Time_Delay_Not_Expired                          = 0x37u,
  LinUDIf_NRC_Upload_Download_Not_Accepted                             = 0x70u,
  LinUDIf_NRC_Transfer_Data_Suspended                                  = 0x71u,
  LinUDIf_NRC_General_Programming_Failure                              = 0x72u,
  LinUDIf_NRC_Wrong_Block_Sequence_Counter                             = 0x73u,
  LinUDIf_NRC_Request_Correctly_Received_Response_Pending              = 0x78u,
  LinUDIf_NRC_Sub_Function_Not_Supported_In_Active_Diagnostic_Session  = 0x7Eu,
  LinUDIf_NRC_Service_Not_Supported_In_Active_Diagnostic_Session       = 0x7Fu,
  LinUDIf_NRC_Rpm_Too_High                                             = 0x81u,
  LinUDIf_NRC_Rpm_Too_Low                                              = 0x82u,
  LinUDIf_NRC_Engine_Is_Running                                        = 0x83u,
  LinUDIf_NRC_Engine_Is_Not_Running                                    = 0x84u,
  LinUDIf_NRC_Engine_Run_Time_Too_Low                                  = 0x85u,
  LinUDIf_NRC_Temperature_Too_High                                     = 0x86u,
  LinUDIf_NRC_Temperature_Too_Low                                      = 0x87u,
  LinUDIf_NRC_Vehicle_Speed_Too_High                                   = 0x88u,
  LinUDIf_NRC_Vehicle_Speed_Too_Low                                    = 0x89u,
  LinUDIf_NRC_Throttle_Pedal_Too_High                                  = 0x8Au,
  LinUDIf_NRC_Throttle_Pedal_Too_Low                                   = 0x8Bu,
  LinUDIf_NRC_Transmission_Range_Not_In_Neutral                        = 0x8Cu,
  LinUDIf_NRC_Transmission_Rang_Not_In_Gear                            = 0x8Du,
  LinUDIf_NRC_Brake_Switches_Not_Closed                                = 0x8Fu,
  LinUDIf_NRC_Shifter_Lever_Not_In_Park                                = 0x90u,
  LinUDIf_NRC_Torque_Converter_Clutch_Locked                           = 0x91u,
  LinUDIf_NRC_Voltage_Too_High                                         = 0x92u,
  LinUDIf_NRC_Voltage_Too_Low                                          = 0x93u,
  LinUDIf_NRC_Basic_Setting_Not_Started                                = 0xF0u,
  LinUDIf_NRC_Not_Configured                                           = 0xF1u,
  LinUDIf_NRC_Ecu_Specific_Conditions_Not_Correct                      = 0xFAu
};

typedef enum LinUDSIf_eNegativeResponseCode LinUDSIf_eNegativeResponseCode_t; /**< Typedef of LinUDSIf_eNegativeResponseCode. */

/* ****************************************************************************/
/* ****************************** LIN SERVICES ********************************/
/* ****************************************************************************/

#define LINUDSIF_ASSIGNNAD_REQLEN             5u /**< Length of a 'Assign NAD' request. */
#define LINUDSIF_ASSIGNFRAMEID_REQLEN         5u /**< Length of a 'Assign Frame ID' request. */
#define LINUDSIF_READBYIDENTIFIER_REQLEN      5u /**< Length of a 'Read By Identifier' request. */
#define LINUDSIF_CONDITIONALCHANGENAD_REQLEN  5u /**< Length of a 'Conditional Change NAD' request. */
#define LINUDSIF_ASSIGNFRAMEIDRANGE_REQLEN    5u /**< Length of a 'Assign Frame ID Range' request. */
#define LINUDSIF_SNPD_REQLEN                  5u /**< Length of a 'SNPD' request. */

/* LIN 2.2 Read by identifier - identifier */

#define LINUDSIF_RB_IDENTIFIER_PROD_IDENT      0x00u /**< Read By Identifier for 'Product Identification'. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_RB_IDENTIFIER_PROD_IDENT_LEN  5u    /**< Data length of the 'Product Identification'. */

#define LINUDSIF_RB_IDENTIFIER_SERIAL_NUM      0x01u /**< Read By Identifier for 'Serial number'. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_RB_IDENTIFIER_SERIAL_NUM_LEN  4u    /**< Data length of the 'Serial number' */

typedef Lin_uint16_t LinUDSIf_SupplierID_t;   /**< Data type for the LIN Supplier ID. */
typedef Lin_uint16_t LinUDSIf_FunctionID_t;   /**< Data type for the LIN Function ID. */
typedef Lin_uint8_t  LinUDSIf_VariantID_t;    /**< Data type for the LIN Variant ID. */
typedef Lin_uint32_t LinUDSIf_SerialNumber_t; /**< Data type for the LIN Serial Number. */

typedef Lin_uint8_t     LinUDSIf_RbIdentifier_t;     /**< Data type for the LIN Diagnostic 'Read By Identifier' ID. */
typedef Lin_BufLength_t LinUDSIf_RbiLength_t;        /**< Data type for the data length of an LIN Diagnostic 'Read By Identifier' ID.  */
typedef Lin_pvoid_t     LinUDSIf_GenericRbiLength_t; /**< Generic void pointer data type for the 'Read By Identifier' length configuration parameter.  */

/***************************************************************************//**
 * Length-Callback function for the LIN diagnostic service "Read By Identifier"
 * if the "Read By Identifier" ID table entry is configured to use a length-
 * callback. (@ref LinUDSIf_RbiLenType_Callback)
 *
 * @param genericUdsEnvData[in] Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]         Pointer to the UDS Layer interface function struct.
 * @param rbIdentifier          'Read By Identifier' ID.
 *
 * @return Length of an 'Read By Identifier' ID.
 *
 ******************************************************************************/
typedef LinUDSIf_RbiLength_t (*LinUDSIf_RbILenCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                        LinUDSIf_RbIdentifier_t    rbIdentifier);

/***************************************************************************//**
 * 'Read By Identifier' callback function, which is called on reception of the
 * response Frame ID belonging to a 'Read By Identifier' request.
 *
 * @param genericUdsEnvData[in]     Pointer to reserved UDS Layer environment
 *                                  data.
 * @param udsIfFuns[in]             Pointer to the UDS Layer interface function
 *                                  struct.
 * @param rbIdentifier[in]          'Read By Identifier' ID.
 * @param appendDataToMsgBufFun[in] Pointer to the UDS 'Append Data To Msg
 *                                  Buffer' function.
 * @param genericUdsCbCtxData[in]   Pointer to UDS Layer callback context data.
 *
 * @return LIN_TRUE if the user has successfully filled the transmit buffer
 *         with the requested data.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RbIdentifierCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData, LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                    LinUDSIf_RbIdentifier_t      rbIdentifier,      LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                    LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * Enumerator for the length parameter types of a 'Read By Identifier' table
 * entry.
 ******************************************************************************/
enum LinUDSIf_eRbiLenType
{
  LinUDSIf_RbiLenType_Value    = 0,  /**< Integer value for the 'Read By Identifier' data length. */
  LinUDSIf_RbiLenType_Callback = 1   /**< Callback for the 'Read By Identifier' data length. */
};

typedef enum LinUDSIf_eRbiLenType LinUDSIf_eRbiLenType_t; /**< Typedef of LinUDSIf_eRbiLenType. */

/***************************************************************************//**
 * 'Read By Identifier' lookup table entry struct.
 ******************************************************************************/
struct LinUDSIf_sRbiLookupEntry
{
  LinUDSIf_RbIdentifier_t               FirstRbIdentifier; /**< Start of the 'Read By Identifier' ID range. */
  LinUDSIf_RbIdentifier_t               LastRbIdentifier;  /**< End of the 'Read By Identifier' ID range. */
  LinUDSIf_RbIdentifierCbFun_t          Callback;          /**< Pointer to the 'Read By Identifier' callback function. */
  LinUDSIf_eRbiLenType_t                LengthType;        /**< Type of the 'Read By Identifier' length value. */
  LinUDSIf_GenericRbiLength_t           Length;            /**< 'Read By Identifier' length value or callback. */
  LinUDSIf_LookupIdProcCbCtxDataType_t  cbCtxType;         /**< Callback context pointer type. */
  LinUDSIf_pGenericCbCtxData_t          CbCtxData;         /**< Pointer to the callback data for the 'Read By Identifier' callback function. */
};

typedef struct LinUDSIf_sRbiLookupEntry LinUDSIf_sRbiLookupEntry_t; /**< Typedef of LinUDSIf_sRbiLookupEntry. */
typedef LinUDSIf_sRbiLookupEntry_t*     LinUDSIf_pRbiLookupEntry_t; /**< Typedef of pointer to LinUDSIf_sRbiLookupEntry. */

/***************************************************************************//**
 * Struct for the LIN Product Identification.
 ******************************************************************************/
struct LinUDSIf_sProductIdentification
{
  LinUDSIf_SupplierID_t SupplierID; /**< Supplier ID. */
  LinUDSIf_FunctionID_t FunctionID; /**< Function ID. */
  LinUDSIf_VariantID_t  VariantID;  /**< Variant ID. */
};

typedef struct LinUDSIf_sProductIdentification    LinUDSIf_sProductIdentification_t; /**< Typedef of LinUDSIf_sProductIdentification. */
typedef        LinUDSIf_sProductIdentification_t* LinUDSIf_pProductIdentification_t; /**< Typedef of pointer toLinUDSIf_sProductIdentification. */

/***************************************************************************//**
 * Struct for the LIN Conditional Change NAD Service data.
 ******************************************************************************/
struct LinUDSIf_sConditionalChangeNADData
{
  LinUDSIf_RbIdentifier_t Id;     /**< 'Read By Identifier' ID. */
  LinUDSIf_Data_t         Byte;   /**< The data byte which should be extracted out the data defined by 'Read By Identifier' ID.  */
  LinUDSIf_Data_t         Mask;   /**< Value to do a bitwise XOR with. */
  LinUDSIf_Data_t         Invert; /**< Mask to do a bitwise AND with. */
  LinUDSIf_NAD_t          NewNAD; /**< The new NAD to change to if the result is zero. */
};

typedef struct LinUDSIf_sConditionalChangeNADData    LinUDSIf_sConditionalChangeNADData_t;  /**< Typedef of LinUDSIf_sConditionalChangeNADData. */
typedef        LinUDSIf_sConditionalChangeNADData_t* LinUDSIf_pConditionalChangeNADData_t;  /**< Typedef of pointer to LinUDSIf_sConditionalChangeNADData. */

/* ****************************************************************************/
/* ****************************** UDS SERVICES ********************************/
/* ****************************************************************************/
#define LINUDSIF_DIAGNOSTICSESSIONCONTROL_REQLEN            1u /**< Data length of the 'Diagnostic Session Control' service request. */
#define LINUDSIF_ECURESET_REQLEN                            1u /**< Data length of the 'ECU Reset' service request. */
#define LINUDSIF_SECURITYACCESS_REQLEN_MIN                  1u /**< Minimal data length of the 'Security Access' service request. */
#define LINUDSIF_COMMUNICATIONCONTROL_REQLEN_MIN            2u /**< Minimal data length of the 'Communication Control' service request. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_COMMUNICATIONCONTROL_REQLEN_MAX            4u /**< Maximal data length of the 'Communication Control' service request. */
#define LINUDSIF_TESTERPRESENT_REQLEN                       1u /**< Data length of the 'Tester Present' service request. */
#define LINUDSIF_CONTROLDTCSETTING_REQLEN_MIN               1u /**< Minimal data length of the 'Control DTC service request. */
#define LINUDSIF_CLEARDIAGNOSTICINFORMATION_REQLEN_MIN      1u /**< Minimal data length of the 'Clear Diagnostic Information' service request. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_CLEARDIAGNOSTICINFORMATION_REQLEN          3u /**< Data length of the 'Write Data By Identifier' service request. */
#define LINUDSIF_RESPONSEONEVENT_REQLEN_MIN                 1u /**< Minimal data length of the 'Response On Event' service request. */
#define LINUDSIF_READDATABYIDENTIFIER_REQLEN_MAX            LINUDS_MAX_PHYADR_RDBI_LST_LEN * sizeof(LinUDSIf_DID_t) /**< Maximal data length of the 'Read Data By Identifier' service request. */
#define LINUDSIF_WRITEDATABYIDENTIFIER_REQLEN_MIN           3u /**< Minimal data length of the 'Write Data By Identifier' service request. */
#define LINUDSIF_READMEMORYBYADDRESS_REQLEN_MIN             3u /**< Minimal data length of the 'Read Memory By Address' service request. */
#define LINUDSIF_WRITEMEMORYBYADDRESS_REQLEN_MIN            3u /**< Minimal data length of the 'Write Memory By Address' service request. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_CLEARDIAGNOSTICINFORMATION_REQLEN          3u /**< Minimal data length of the 'Clear Diagnostic Information' service request. */
#define LINUDSIF_READDTCINFORMATION_REQLEN_MIN              2u /**< Minimal data length of the 'Read DTC Information' service request. */
#define LINUDSIF_INPUTOUTPUTCONTROLBYIDENTIFIER_REQLEN_MIN  3u /**< Minimal data length of the 'Input Output Control By Identifier' service request. */
#define LINUDSIF_ROUTINECONTROL_REQLEN_MIN                  3u /**< Minimal data length of the 'Routine Control' service request. */
#define LINUDSIF_REQUESTDOWNLOAD_REQLEN_MIN                 4u /**< Minimal data length of the 'Request Download' service request. */
#define LINUDSIF_REQUESTUPLOAD_REQLEN_MIN                   4u /**< Minimal data length of the 'Request Upload' service request. */
#define LINUDSIF_TRANSFERDATA_REQLEN_MIN                    2u /**< Minimal data length of the 'Transfer Data' service request. */
#define LINUDSIF_REQUESTTRANSFEREXIT_REQLEN_MIN             0u /**< Minimal data length of the 'Request Transfer Exit' service request. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINUDSIF_DIAGNOSTICSESSIONCONTROL_RESPLEN 5u /**< Data length of the 'Diagnostic Session Control' service response. */
#define LINUDSIF_ECURESET_RESPLEN_MIN             1u /**< Minimal data length of the 'ECU Reset' service response. */
#define LINUDSIF_SECURITYACCESS_RESPLEN_MIN       1u /**< Minimal data length of the 'Security Access' service response. */
#define LINUDSIF_SECURITYACCESS_SEED_LEN_MIN      2u /**< Minimal seed length of the 'Security Access' service response. */

/* ****************************************************************************/
/* ********* DIAGNOSTIC AND COMMUNICATION MANAGEMENT FUNCTIONAL UNIT **********/
/* ****************************************************************************/

/* ****************************************************************************/
/* ******************* DiagnosticSessionControl Service ***********************/
/* ****************************************************************************/
typedef Lin_uint16_t  LinUDSIf_P2SrvMax_t;      /**< Data type for the P2 Server Max value. */
typedef Lin_uint16_t* LinUDSIf_pP2SrvMax_t;     /**< Data type for the pointer to the P2 Server Max value. */
typedef Lin_uint16_t  LinUDSIf_P2StarSrvMax_t;  /**< Data type for the P2* Server Max value. */
typedef Lin_uint16_t* LinUDSIf_pP2StarSrvMax_t; /**< Data type for the pointer to the P2* Server Max value. */

typedef Lin_uint8_t LinUDSIf_DiagnosticSessionType_t; /**< Data type for UDS Diagnostic Session Type. */

/***************************************************************************//**
 * DiagnosticSessionControl (0x10) Service - Sub-Function 'diagnosticSessionType' - LEV_DS_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_diagnosticSessionType
{
  LinUDSIf_SUBFUNC_DS_ISOSAEReserved_0x00               = 0x00u, /**< ISOSAEReserved - LEV_DS_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DS_defaultSession                    = 0x01u, /**< defaultSession - LEV_DS_DS */
  LinUDSIf_SUBFUNC_DS_ProgrammingSession                = 0x02u, /**< ProgrammingSession - LEV_DS_PRGS */
  LinUDSIf_SUBFUNC_DS_extendedDiagnosticSession         = 0x03u, /**< extendedDiagnosticSession - LEV_DS_EXTDS */
  LinUDSIf_SUBFUNC_DS_safetySystemDiagnosticSession     = 0x04u, /**< safetySystemDiagnosticSession - LEV_DS_SSDS */
  LinUDSIf_SUBFUNC_DS_ISOSAEReserved_Start_0x05         = 0x05u, /**< ISOSAEReserved Area Start - LEV_DS_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DS_ISOSAEReserved_End_0x3F           = 0x3Fu, /**< ISOSAEReserved Area End - LEV_DS_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DS_vehicleManufacturerSpecific_Start = 0x40u, /**< vehicleManufacturerSpecific Area Start - LEV_DS_VMS  */
  LinUDSIf_SUBFUNC_DS_vehicleManufacturerSpecific_End   = 0x5Fu, /**< vehicleManufacturerSpecific Area End - LEV_DS_VMS */
  LinUDSIf_SUBFUNC_DS_systemSupplierSpecific_Start      = 0x60u, /**< systemSupplierSpecific Area Start - LEV_DS_SSS */
  LinUDSIf_SUBFUNC_DS_systemSupplierSpecific_End        = 0x7Eu, /**< systemSupplierSpecific Area End - LEV_DS_SSS */
  LinUDSIf_SUBFUNC_DS_ISOSAEReserved_0x7F               = 0x7Fu  /**< ISOSAEReserved - LEV_DS_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_diagnosticSessionType LinUDSIf_eSubFunc_diagnosticSessionType_t; /**< Typedef of enum LinUDSIf_eSubFunc_diagnosticSessionType. */

/* ****************************************************************************/
/* **************************** ECUReset Service ******************************/
/* ****************************************************************************/
typedef Lin_uint8_t               LinUDSIf_ResetType_t;      /**< Data type for the 'ECU Reset' service parameter 'Reset Type'. */
typedef Lin_uint8_t               LinUDSIf_PowerDownTime_t;  /**< Data type for the 'ECU Reset' service parameter 'Power Down Time'. */
typedef LinUDSIf_PowerDownTime_t* LinUDSIf_pPowerDownTime_t; /**< Pointer to the 'ECU Reset' service parameter 'Power Down Time'. */

/***************************************************************************//**
 * ECUReset (0x11) Service - Sub-Function 'resetType' - LEV_RT_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_resetType
{
  LinUDSIf_SUBFUNC_RT_ISOSAEReserved_0x00               = 0x00u, /**< ISOSAEReserved - LEV_RT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RT_hardReset                         = 0x01u, /**< hardReset - LEV_RT_HR */
  LinUDSIf_SUBFUNC_RT_keyOffOnReset                     = 0x02u, /**< keyOffOnReset - LEV_RT_KOFFONR */
  LinUDSIf_SUBFUNC_RT_softReset                         = 0x03u, /**< softReset - LEV_RT_SR */
  LinUDSIf_SUBFUNC_RT_enableRapidPowerShutDown          = 0x04u, /**< enableRapidPowerShutDown - LEV_RT_ERPSD */
  LinUDSIf_SUBFUNC_RT_disableRapidPowerShutDown         = 0x05u, /**< disableRapidPowerShutDown - LEV_RT_DRPSD */
  LinUDSIf_SUBFUNC_RT_ISOSAEReserved_Start_0x06         = 0x06u, /**< ISOSAEReserved Area Start - LEV_RT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RT_ISOSAEReserved_End_0x3F           = 0x3Fu, /**< ISOSAEReserved Area End - LEV_RT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RT_vehicleManufacturerSpecific_Start = 0x40u, /**< vehicleManufacturerSpecific Area Start - LEV_RT_VMS  */
  LinUDSIf_SUBFUNC_RT_vehicleManufacturerSpecific_End   = 0x5Fu, /**< vehicleManufacturerSpecific Area End - LEV_RT_VMS */
  LinUDSIf_SUBFUNC_RT_systemSupplierSpecific_Start      = 0x60u, /**< systemSupplierSpecific Area Start - LEV_RT_SSS */
  LinUDSIf_SUBFUNC_RT_systemSupplierSpecific_End        = 0x7Eu, /**< systemSupplierSpecific Area End - LEV_RT_SSS */
  LinUDSIf_SUBFUNC_RT_ISOSAEReserved_0x7F               = 0x7Fu  /**< ISOSAEReserved - LEV_RT_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_resetType LinUDSIf_eSubFunc_resetType_t; /**< Typedef of enum LinUDSIf_eSubFunc_resetType. */

/* ****************************************************************************/
/* ************************ SecurityAccess Service ****************************/
/* ****************************************************************************/
typedef Lin_uint8_t                     LinUDSIf_SecurityAccessType_t;        /**< Data type for the 'Security Access' service parameter 'Security Access Type'. */
typedef LinUDSIf_SecurityAccessType_t*  LinUDSIf_pSecurityAccessType_t;       /**< Pointer to the 'Security Access' service parameter 'Security Access Type'. */
typedef Lin_uint8_t*                    LinUDSIf_pSecurityAccessDataRecord_t; /**< Pointer to the 'Security Access' service parameter 'Security Access Data Record'. */
typedef Lin_uint8_t*                    LinUDSIf_pSeed_t;                     /**< Pointer to the 'Security Access' service parameter 'Seed'. */
typedef Lin_uint8_t*                    LinUDSIf_pKey_t;                      /**< Pointer to the 'Security Access' service parameter 'Key'. */

/***************************************************************************//**
 * SecurityAccess (0x27) - Sub-Function 'securityAccessType' - LEV_SAT_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_securityAccessType
{
  LinUDSIf_SUBFUNC_SAT_ISOSAEReserved_0x00           = 0x00u, /**< ISOSAEReserved - LEV_SAT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_SAT_requestSeed                   = 0x01u, /**< requestSeed - LEV_SAT_RSD */
  LinUDSIf_SUBFUNC_SAT_sendKey                       = 0x02u, /**< sendKey - LEV_SAT_SK */
  LinUDSIf_SUBFUNC_SAT_requestSeed_Start             = 0x03u, /**< requestSeed - LEV_SAT_RSD - Uneven numbers are for 'requestSeed' */
  LinUDSIf_SUBFUNC_SAT_requestSeed_End               = 0x41u, /**< requestSeed - LEV_SAT_RSD - Uneven numbers are for 'requestSeed' */
  LinUDSIf_SUBFUNC_SAT_sendKey_Start                 = 0x04u, /**< sendKey - LEV_SAT_SK - Even numbers are for 'sendKey' */
  LinUDSIf_SUBFUNC_SAT_sendKey_End                   = 0x42u, /**< sendKey - LEV_SAT_SK - Even numbers are for 'sendKey' */
  LinUDSIf_SUBFUNC_SAT_ISOSAEReserved_Start_0x05     = 0x43u, /**< ISOSAEReserved Area Start - LEV_SAT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_SAT_ISOSAEReserved_End_0x3F       = 0x5Eu, /**< ISOSAEReserved Area End - LEV_SAT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_SAT_ISO26021_2_requestSeed_values = 0x5Fu, /**< ISO26021-2 requestSeed values - LEV_SAT_SK */
  LinUDSIf_SUBFUNC_SAT_ISO26021_2_sendKey_values     = 0x60u, /**< ISO26021-2 sendKey values - LEV_SAT_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_SAT_systemSupplierSpecific_Start  = 0x60u, /**< systemSupplierSpecific Area Start - LEV_SAT_SSS */
  LinUDSIf_SUBFUNC_SAT_systemSupplierSpecific_End    = 0x7Eu  /**< systemSupplierSpecific Area End - LEV_SAT_SSS */
};

typedef enum LinUDSIf_eSubFunc_securityAccessType LinUDSIf_eSubFunc_securityAccessType_t; /**< Typedef of enum LinUDSIf_eSubFunc_securityAccessType. */

/* ****************************************************************************/
/* ********************* CommunicationControl Service *************************/
/* ****************************************************************************/
typedef Lin_uint8_t  LinUDSIf_ComCtrlType_t;        /**< Data type for the 'Communication Control' service parameter 'Communication Control Type'. */
typedef Lin_uint8_t  LinUDSIf_CommunicationType_t;  /**< Data type for the 'Communication Control' service parameter 'Communication Type'. */
typedef Lin_uint16_t LinUDSIf_NIN_t;                /**< Data type for the 'Communication Control' service parameter 'NIN (Node Identification Number)'. */

/***************************************************************************//**
 * CommunicationControl (0x28) - communicationType parameter - CTP
 ******************************************************************************/

/***************************************************************************//**
 * Communication Type parameter struct
 ******************************************************************************/
struct LinUDSIf_sCommunicationType
{
    Lin_uint8_t Bit_0_1  :2;  /**< Bit 0 and Bit 1. */
    Lin_uint8_t reserved :2;  /**< Reserved. */
    Lin_uint8_t Bit_4_7  :4;  /**< Bit 4 to Bit 7. */
};

typedef struct LinUDSIf_sCommunicationType    LinUDSIf_sCommunicationType_t;  /**< Typedef of struct LinUDSIf_sCommunicationType. */
typedef        LinUDSIf_sCommunicationType_t* LinUDSIf_pCommunicationType_t;  /**< Typedef of pointer to struct LinUDSIf_sCommunicationType. */

#define LINUDSIF_CTP_BIT_0_1_MSK     0x03u  /**< communicationType - mask for bit 0 to 1 */
#define LINUDSIF_CTP_BIT_4_7_MSK     0xF0u  /**< communicationType - mask for bit 4 to 7 */

#define LINUDSIF_CTP_EXTRACT_BIT_0_1(CTP) ((CTP) & LINUDSIF_CTP_BIT_0_1_MSK)          /**< communicationType - bit 0 to 1 extraction macro. */
#define LINUDSIF_CTP_EXTRACT_BIT_4_7(CTP) (((CTP) & LINUDSIF_CTP_BIT_4_7_MSK) >> 4u)  /**< communicationType - bit 4 to 7 extraction macro. */

/***************************************************************************//**
 * Communication Type parameter enum for bit 0 - 1
 ******************************************************************************/
enum LinUDSIf_eCommunicationType_Bit_0_1
{
  LinUDSIf_CTP_Bit_0_1_ISOSAEReserved                                                         = 0x0u, /**< ISOSAEReserved */
  LinUDSIf_CTP_Bit_0_1_normalCommunicationMessages                                            = 0x1u, /**< normalCommunicationMessages - NCM */
  LinUDSIf_CTP_Bit_0_1_networkManagementCommunicationMessages                                 = 0x2u, /**< networkManagementCommunicationMessages - NWMCM */
  LinUDSIf_CTP_Bit_0_1_networkManagementCommunicationMessages_and_normalCommunicationMessages = 0x3u  /**< networkManagementCommunicationMessages and normalCommunicationMessages  - NWMCM-NCM */
};

typedef enum LinUDSIf_eCommunicationType_Bit_0_1 LinUDSIf_eCommunicationType_Bit_0_1_t; /**< Typedef of enum LinUDSIf_communicationType_Bit_0_1. */

/***************************************************************************//**
 * Communication Type parameter enum for bit 4 - 7
 ******************************************************************************/
enum LinUDSIf_eCommunicationType_Bit_4_7
{
  LinUDSIf_CTP_Bit_4_7_Dis_En_specified_communicationType                     = 0x00u, /**< Disable / Enable specified communicationType - DISENSCT */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_1   = 0x01u, /**< Disable / Enable specific subnet identified by subnet number - subnet 1 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_2   = 0x02u, /**< Disable / Enable specific subnet identified by subnet number - subnet 2 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_3   = 0x03u, /**< Disable / Enable specific subnet identified by subnet number - subnet 3 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_4   = 0x04u, /**< Disable / Enable specific subnet identified by subnet number - subnet 4 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_5   = 0x05u, /**< Disable / Enable specific subnet identified by subnet number - subnet 5 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_6   = 0x06u, /**< Disable / Enable specific subnet identified by subnet number - subnet 6 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_7   = 0x07u, /**< Disable / Enable specific subnet identified by subnet number - subnet 7 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_8   = 0x08u, /**< Disable / Enable specific subnet identified by subnet number - subnet 8 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_9   = 0x09u, /**< Disable / Enable specific subnet identified by subnet number - subnet 9 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_10  = 0x0Au, /**< Disable / Enable specific subnet identified by subnet number - subnet 10 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_11  = 0x0Bu, /**< Disable / Enable specific subnet identified by subnet number - subnet 11 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_12  = 0x0Cu, /**< Disable / Enable specific subnet identified by subnet number - subnet 12 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_13  = 0x0Du, /**< Disable / Enable specific subnet identified by subnet number - subnet 13 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_specific_subnet_id_by_subnet_nb_14  = 0x0Eu, /**< Disable / Enable specific subnet identified by subnet number - subnet 14 - DISENSSIVSN */
  LinUDSIf_CTP_Bit_4_7_Dis_En_network_which_request_is_received_on            = 0x0Fu, /**< Disable/Enable  network  which  request  is  received  on  (Receiving node (server)) - DENWRIRO */
};

typedef enum LinUDSIf_eCommunicationType_Bit_4_7 LinUDSIf_eCommunicationType_Bit_4_7_t; /**< Typedef of enum LinUDSIf_eCommunicationType_Bit_4_7. */

/***************************************************************************//**
 * CommunicationControl (0x28) - Sub-Function 'controlType' - LEV_CTRLTP_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_controlType
{
 LinUDSIf_SUBFUNC_CTRLTP_enableRxAndTx                                      = 0x00u, /**< enableRxAndTx - LEV_CTRLTP_ERXTX */
 LinUDSIf_SUBFUNC_CTRLTP_enableRxAndDisableTx                               = 0x01u, /**< enableRxAndDisableTx - LEV_CTRLTP_ERXDTX */
 LinUDSIf_SUBFUNC_CTRLTP_disableRxAndEnableTx                               = 0x02u, /**< disableRxAndEnableTx - LEV_CTRLTP_DRXETX */
 LinUDSIf_SUBFUNC_CTRLTP_disableRxAndTx                                     = 0x03u, /**< disableRxAndTx - LEV_CTRLTP_DRXTX */
 LinUDSIf_SUBFUNC_CTRLTP_enableRxAndDisableTxWithEnhancedAddressInformation = 0x04u, /**< enableRxAndDisableTxWithEnhancedAddressInformation - LEV_CTRLTP_ERXDTXWEAI */
 LinUDSIf_SUBFUNC_CTRLTP_enableRxAndTxWithEnhancedAddressInformation        = 0x05u, /**< enableRxAndTxWithEnhancedAddressInformation - LEV_CTRLTP_ERXTXWEAI */
 LinUDSIf_SUBFUNC_CTRLTP_ISOSAEReserved_Start_0x05                          = 0x06u, /**< ISOSAEReserved Area Start - LEV_CTRLTP_ISOSAERESRVD */
 LinUDSIf_SUBFUNC_CTRLTP_ISOSAEReserved_End_0x3F                            = 0x3Fu, /**< ISOSAEReserved Area End - LEV_CTRLTP_ISOSAERESRVD */
 LinUDSIf_SUBFUNC_CTRLTP_vehicleManufacturerSpecific_Start                  = 0x40u, /**< vehicleManufacturerSpecific Area Start - LEV_CTRLTP_VMS  */
 LinUDSIf_SUBFUNC_CTRLTP_vehicleManufacturerSpecific_End                    = 0x5Fu, /**< vehicleManufacturerSpecific Area End - LEV_CTRLTP_VMS */
 LinUDSIf_SUBFUNC_CTRLTP_systemSupplierSpecific_Start                       = 0x60u, /**< systemSupplierSpecific Area Start - LEV_CTRLTP_SS */
 LinUDSIf_SUBFUNC_CTRLTP_systemSupplierSpecific_End                         = 0x7Eu, /**< systemSupplierSpecific Area End - LEV_CTRLTP_DS_SSS */
 LinUDSIf_SUBFUNC_CTRLTP_ISOSAEReserved_0x7F                                = 0x7Fu  /**< ISOSAEReserved - LEV_CTRLTP_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_controlType LinUDSIf_eSubFunc_controlType_t; /**< Typedef of enum LinUDSIf_eSubFunc_controlType. */

/* ****************************************************************************/
/* ************************* TesterPresent Service ****************************/
/* ****************************************************************************/
typedef Lin_uint8_t LinUDSIf_ZeroSubFunction_t;  /**< Data type for the 'Tester Present' service parameter 'Zero Sub-Function'. */

/* ****************************************************************************/
/* *********************** ControlDTCSetting Service **************************/
/* ****************************************************************************/
typedef Lin_uint8_t  LinUDSIf_DTCSettingType_t;     /**< Generic data type for the 'Control DTC Setting' service parameter 'DTC Setting Type'. */
typedef Lin_uint8_t* LinUDSIf_pDTCSetCtrlOptRcrd_t; /**< Pointer to the 'Control DTC Setting' service parameter 'DTC Setting Control Option Record'. */

typedef Lin_uint8_t  LinUDSIf_DtcReportType_t;      /**< Data type for the 'Control DTC Setting' service parameter 'DTC Report Type'. */

typedef Lin_uint8_t* LinUDSIf_pDtcParameter_t;      /**< Pointer to the 'Control DTC Setting' service parameter. */
typedef Lin_uint16_t LinUDSIf_DtcParamLen_t;        /**< Data type for the 'Control DTC Setting' service parameter length. */
typedef Lin_uint8_t  LinUDSIf_GroupOfDTC_t[3];      /**< Data type for the 'Control DTC Setting' service parameter 'Group of DTC'. */

/***************************************************************************//**
 * ControlDTCSetting (0x85) - Sub-Function 'DTCSettingType' - LEV_ETP_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_DTCSettingType
{
  LinUDSIf_SUBFUNC_DTCSTP_ISOSAEReserved_0x00               = 0x00u, /**< ISOSAEReserved - LEV_DTCSTP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DTCSTP_on                                = 0x01u, /**< on - LEV_DTCSTP_ON */
  LinUDSIf_SUBFUNC_DTCSTP_off                               = 0x02u, /**< off - LEV_DTCSTP_OFF */
  LinUDSIf_SUBFUNC_DTCSTP_ISOSAEReserved_Start_0x03         = 0x03u, /**< ISOSAEReserved Area Start - LEV_DTCSTP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DTCSTP_ISOSAEReserved_End_0x3F           = 0x3Fu, /**< ISOSAEReserved Area End - LEV_DTCSTP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_DTCSTP_vehicleManufacturerSpecific_Start = 0x40u, /**< vehicleManufacturerSpecific Area Start - LEV_DTCSTP_VMS  */
  LinUDSIf_SUBFUNC_DTCSTP_vehicleManufacturerSpecific_End   = 0x5Fu, /**< vehicleManufacturerSpecific Area End - LEV_DTCSTP_VMS */
  LinUDSIf_SUBFUNC_DTCSTP_systemSupplierSpecific_Start      = 0x60u, /**< systemSupplierSpecific Area Start - LEV_DTCSTP_SSS */
  LinUDSIf_SUBFUNC_DTCSTP_systemSupplierSpecific_End        = 0x7Eu, /**< systemSupplierSpecific Area End - LEV_DTCSTP_SSS */
  LinUDSIf_SUBFUNC_DTCSTP_ISOSAEReserved_0x7F               = 0x7Fu  /**< ISOSAEReserved - LEV_DTCSTP_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_DTCSettingType LinUDSIf_eSubFunc_DTCSettingType_t; /**< Typedef of enum LinUDSIf_eSubFunc_DTCSettingType. */

/* ****************************************************************************/
/* ************************ ResponseOnEvent Service ***************************/
/* ****************************************************************************/
typedef Lin_uint8_t            LinUDSIf_EventType_t;        /**< Generic data type for the 'Response On Event' service parameter 'Event Type'. */
typedef LinUDSIf_EventType_t*  LinUDSIf_pEventType_t;       /**< Generic Pointer to the 'Response On Event' service parameter 'Event Type'. */

typedef Lin_uint8_t   LinUDSIf_EventWindowTime_t;           /**< Data type for the 'Response On Event' service parameter 'Event Window Time'. */
typedef Lin_uint8_t*  LinUDSIf_pEventTypeRecord_t;          /**< Pointer to the 'Response On Event' service parameter 'Event Window Time'. */
typedef Lin_uint8_t*  LinUDSIf_pServiceToRespondToRecord_t; /**< Pointer to the 'Response On Event' service parameter 'Service To Respond To Record'. */

/***************************************************************************//**
 * ResponseOnEvent (0x86) - Sub-Function 'eventType' - LEV_ETP_
 ******************************************************************************/

/***************************************************************************//**
 * Event Type parameter struct
 ******************************************************************************/
struct LinUDSIf_sEventType
{
    Lin_uint8_t Bit_0_5           :6; /**< Bit 0 o bit 5. */
    Lin_uint8_t Bit_6             :1; /**< Bit 6. */
    Lin_uint8_t SuppressPosRspMsg :1; /**< 'Suppress Positive Message' bit. */
};

typedef struct LinUDSIf_sEventType    LinUDSIf_sEventType_t;  /**< Typedef of struct LinUDSIf_sEventType. */
typedef        LinUDSIf_sEventType_t* LinUDSIf_psEventType_t; /**< Typedef of pointer to struct LinUDSIf_sEventType. */

#define LINUDSIF_ET_BIT_0_5_MSK   0x3Fu /**< eventType - mask for bit 0 to 5 */
#define LINUDSIF_ET_BIT_6_MSK     0x40u /**< eventType - mask for bit 6 */

#define LINUDSIF_ET_EXTRACT_BIT_0_5(ET) ((ET) & LINUDSIF_ET_BIT_0_5_MSK)     /**< eventType - bit 0 to 5 extraction macro. */
#define LINUDSIF_ET_EXTRACT_BIT_6(ET)   ((ET) & LINUDSIF_ET_BIT_6_MSK) >> 6u /**< eventType - bit 6 extraction macro. */

/***************************************************************************//**
 * Enumerator for 'Event Type' bits 0 - 5.
 ******************************************************************************/
enum LinUDSIf_eSubFunc_eventType_Bit_0_5
{
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_stopResponseOnEvent               = 0x00u, /**< stopResponseOnEvent - LEV_ETP_STPROE */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_onDTCStatusChange                 = 0x01u, /**< onDTCStatusChange - LEV_ETP_ONDTCS */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_onTimerInterrupt                  = 0x02u, /**< onTimerInterrupt - LEV_ETP_OTI */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_onChangeOfDataIdentifier          = 0x03u, /**< onChangeOfDataIdentifier - LEV_ETP_OCODID */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_reportActivatedEvents             = 0x04u, /**< reportActivatedEvents - LEV_ETP_RAE */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_startResponseOnEvent              = 0x05u, /**< startResponseOnEvent - LEV_ETP_STRTROE */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_clearResponseOnEvent              = 0x06u, /**< clearResponseOnEvent - LEV_ETP_CLRROE */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_onComparisonOfValues              = 0x07u, /**< onComparisonOfValues - LEV_ETP_OCOV */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_ISOSAEReserved_Start_0x08         = 0x08u, /**< ISOSAEReserved Area Start - LEV_ETP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_ISOSAEReserved_End_0x1F           = 0x1Fu, /**< ISOSAEReserved Area End - LEV_ETP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_vehicleManufacturerSpecific_Start = 0x20u, /**< vehicleManufacturerSpecific Area Start - LEV_ETP_VMS  */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_vehicleManufacturerSpecific_End   = 0x2Fu, /**< vehicleManufacturerSpecific Area End - LEV_ETP_VMS */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_systemSupplierSpecific_Start      = 0x30u, /**< systemSupplierSpecific Area Start - LEV_ETP_SSS */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_systemSupplierSpecific_End        = 0x3Eu, /**< systemSupplierSpecific Area End - LEV_ETP_SSS */
  LinUDSIf_SUBFUNC_ETP_Bit_0_5_ISOSAEReserved_0x3F               = 0x3Fu  /**< ISOSAEReserved - LEV_ETP_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_eventType_Bit_0_5 LinUDSIf_eSubFunc_eventType_Bit_0_5_t; /**< Typedef of enum LinUDSIf_eSubFunc_eventType_Bit_0_5 */

/***************************************************************************//**
 * Enumerator for 'Event Type' bit 6.
 ******************************************************************************/
enum LinUDSIf_eSubFunc_eventType_Bit_6
{
  LinUDSIf_SUBFUNC_ETP_Bit_6_doNotStoreEvent = 0x00u, /**< doNotStoreEvent - LEV_ETP_DNSE */
  LinUDSIf_SUBFUNC_ETP_Bit_6_storeEvent      = 0x01u, /**< storeEvent - LEV_ETP_SE */
};

typedef enum LinUDSIf_eSubFunc_eventType_Bit_6 LinUDSIf_eSubFunc_eventType_Bit_6_t; /**< Typedef of enum LinUDSIf_eSubFunc_eventType_Bit_6 */

/***************************************************************************//**
 * ETR Localization parameter struct for 'Event Type' 'onComparisonOfValues'.
 ******************************************************************************/
struct LinUDSIf_sETR_Localization
{
  Lin_uint16_t Offset  :10; /**< Offset on the positve response message from where to extract the data identifier value. Bit#0 (LSB) - Bit#9 (MSB) contain the start bit number offset. */
  Lin_uint16_t Length  :5;  /**< Bit#10 (LSB) - Bit#14 (MSB) contain the length of data identifier value to be compared. */
  Lin_uint16_t Sign    :1;  /**< (MSB), bit = 0 means comparison without sign, bit = 1 comparison with sign. */
};

typedef struct LinUDSIf_sETR_Localization     LinUDSIf_sETR_Localization_t; /**< Typedef of struct LinUDSIf_sETR_Localization. */
typedef        LinUDSIf_sETR_Localization_t*  LinUDSIf_pETR_Localization_t; /**< Typedef of pointer to struct LinUDSIf_sETR_Localization. */

/***************************************************************************//**
 * Struct for the 'Event Type' 'onComparisonOfValues' sub-function
 ******************************************************************************/
struct LinUDSIf_sETR_OCOV
{
    Lin_uint8_t                  DID[2];          /**< DataIdentifier (DID) */
    Lin_uint8_t                  ComparisonLogic; /**< Comparison logic */
    Lin_uint8_t                  RawReference[3]; /**< Raw reference comparison value. */
    Lin_uint8_t                  HysteresisValue; /**< hysteresis value . */
    LinUDSIf_sETR_Localization_t Localization;    /**< Localization. */
};

typedef struct LinUDSIf_sETR_OCOV LinUDSIf_sETR_OCOV_t; /**< Typedef of struct LinUDSIf_sETR_OCOV. */
typedef LinUDSIf_sETR_OCOV_t* LinUDSIf_pETR_OCOV_t;     /**< Typedef of pointer to struct LinUDSIf_sETR_OCOV. */

/***************************************************************************//**
 * ResponseOnEvent (0x86) - eventWindowTime parameter - EWT
 ******************************************************************************/
enum LinUDSIf_eEventWindowTime
{
  LinUDSIf_EWT_ISOSAEReserved_0x00               = 0x00u, /**< ISOSAEReserved - ISOSAERESRVD*/
  LinUDSIf_EWT_ISOSAEReserved_0x01               = 0x01u, /**< ISOSAEReserved - ISOSAERESRVD*/
  LinUDSIf_EWT_infiniteTimeToResponse            = 0x02u, /**< infiniteTimeToResponse - ITTR */
  LinUDSIf_EWT_vehicleManufacturerSpecific_Start = 0x03u, /**< vehicleManufacturerSpecific - VMS */
  LinUDSIf_EWT_vehicleManufacturerSpecific_End   = 0x7Fu, /**< vehicleManufacturerSpecific - VMS */
  LinUDSIf_EWT_ISOSAEReserved_Start_0x80         = 0x80u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_EWT_ISOSAEReserved_Start_0xFF         = 0xFFu  /**< ISOSAEReserved - ISOSAERESRVD */
};

typedef enum LinUDSIf_eEventWindowTime LinUDSIf_eEventWindowTime_t; /**< Typedef of struct LinUDSIf_eEventWindowTime. */

/* ****************************************************************************/
/* ****************** DATA TRANSMISSION FUNCTIONAL UNIT ***********************/
/* ****************************************************************************/
typedef Lin_uint16_t    LinUDSIf_DID_t;  /**< Data type for the UDS Data Identifier (DID). */
typedef LinUDSIf_DID_t* LinUDSIf_pDID_t; /**< Pointer to the UDS Data Identifier (DID). */

typedef Lin_uint16_t                      LinUDSIf_InputOutputIdentifier_t;  /**< Data type for the UDS Input/Output Identifier (IOID). */
typedef LinUDSIf_InputOutputIdentifier_t* LinUDSIf_pInputOutputIdentifier_t; /**< Pointer to the UDS Input/Output Identifier (IOID). */

typedef Lin_pvoid_t  LinUDSIf_DidLength_t; /**< Data type for data length behind a DID. */

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Read Data By Identifier' callback function.
 *
 * @param genericUdsEnvData[in]     Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]             Pointer to the UDS Layer interface function struct.
 * @param did[in]                   Data Identifier (DID).
 * @param appendDataToMsgBufFun[in] Pointer to function to append data to the
 *                                  message buffer.
 * @param pNcr[out]                 Pointer to NCR variable in case of an error.
 * @param genericUdsCbCtxData[in]   Pointer to UDS Layer callback context data.
 *
 * The callback is called after reception of a frame header for the response
 * to a 'ReadDataByIdentifier' (0x22) service request.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_rddatabyidcbcalled.dox
 * @n
 *
 * @return LIN_TRUE if the requested data has been appended to the
 *         message buffer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RdDataByIdCbFun_t) (LinUDSIf_pGenericEnvData_t genericUdsEnvData,  LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                  LinUDSIf_DID_t             did,                LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                  LinUDSIf_pNRC_t            pNcr,               LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Write Data By Identifier' callback function.
 *
 * @param genericUdsEnvData[in]   Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]           Pointer to the UDS Layer interface function struct.
 * @param did[in]                 Data Identifier (DID).
 * @param data[in]                Pointer to received data.
 * @param dataLen[in]             Length of the received data.
 * @param pNcr[out]               Pointer to NCR variable in case of an error.
 * @param genericUdsCbCtxData[in] Pointer to UDS Layer callback context data.
 *
 * The callback is called after reception of 'WriteDataByIdentifier' (0x23)
 * service request.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_wrtdatabyidcbcalled.dox
 * @n
 *
 * @return LIN_TRUE if data has been written successfully to the memory.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_WrtDataByIdCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData,  LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                   LinUDSIf_DID_t               did,                LinUDSIf_pData_t                data,
                                                   LinUDSIf_BufLength_t         dataLen,            LinUDSIf_pNRC_t                 pNcr,
                                                   LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/* ****************************************************************************/
/* ******************** ReadDataByIdentifier Service **************************/
/* ****************************************************************************/

/***************************************************************************//**
 * ReadDataByIdentifier (0x22) - DID
 ******************************************************************************/

enum LinUDSIf_eDataIdentifier
{
 LinUDSIf_eDID_LogicalSwBlkCntOfProgAttempts      = 0x0407u,  /**< @brief VW Logical Software Block Counter Of Programming Attempts  (0x0407) @see VW 80125 */
 LinUDSIf_eDID_LogicalSwBlkLockValue              = 0x040Fu,  /**< @brief VW Logical Software Block Lock Value  (0x040F) @see VW 80125 */
 LinUDSIf_eDID_Fingerprint                        = 0xF15Au,  /**< @brief Fingerprint (0xF15A) @see VW 80125 */
 LinUDSIf_eDID_FingerprintAndProgDateOfLogSwBlcks = 0xF15Bu,  /**< @brief Fingerprint And Programming Date Of Logical Software Blocks  (0xF15B) @see VW 80125 */
 LinUDSIf_eDID_ECUSupplierNumber                  = 0xF179u,  /**< @brief ECU Supplier Number (0xF179) @see VW 80125 */
 LinUDSIf_eDID_VWFAZITIdentificationString        = 0xF17Cu,  /**< @brief VW FAZIT Identification String (0xF17C) @see VW 80125 */
 LinUDSIf_eDID_ECUProductionTestSystemNumber      = 0xF17Du,  /**< @brief ECU Production Test System Number (0xF17D) @see VW 80125 */
 LinUDSIf_eDID_VWSupplierNumber                   = 0xF17Fu,  /**< @brief VW Supplier Number (0xF17F) @see VW 80125 */
 LinUDSIf_eDID_DiagnosticSession                  = 0xF186u,  /**< @brief Active Diagnostic Session (0xF186 - activeDiagnosticSessionDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_VehManSparePartNumber              = 0xF187u,  /**< @brief Vehicle Manufacturer Spare Part Number (0xF187 - vehicleManufacturerSparePartNumberDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_VehManECUSoftwareNumber            = 0xF188u,  /**< @brief Vehicle Manufacturer ECU Software Number (0xF188 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_VehManECUSoftwareVersionNumber     = 0xF189u,  /**< @brief Vehicle Manufacturer ECU Software Version Number (0xF188 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_ECUManufacturingDate               = 0xF18Bu,  /**< @brief ECU Manufacturing Date (0xF18B) @see VW 80125 */
 LinUDSIf_eDID_ECUSerialNumber                    = 0xF18Cu,  /**< @brief ECU Serial Number (0xF18C - ECUSerialNumberDataIdentifier ) @see ISO 14229-1 */
 LinUDSIf_eDID_VIN                                = 0xF190u,  /**< @brief Vehicle Identification Number  (0xF190 - VINDataIdentifier ) @see ISO 14229-1 */
 LinUDSIf_eDID_VehManECUHardwareNumber            = 0xF191u,  /**< @brief Vehicle Manufacturer ECU Hardware Number (0xF191 - vehicleManufacturerECUHardwareNumberDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_SysSupplECUHardwareNumber          = 0xF192u,  /**< @brief System Supplier ECU Hardware Number (0xF192 - systemSupplierECUHardwareNumberDataIdentifier ) @see ISO 14229-1 */
 LinUDSIf_eDID_SysSupplECUHWVersionNumber         = 0xF193u,  /**< @brief System Supplier ECU Hardware Version Number (0xF193 - systemSupplierECUHardwareVersionNumberDataIdentifier) */
 LinUDSIf_eDID_SysSuppECUSoftwareNumber           = 0xF194u,  /**< @brief System Supplier ECU Software Number (0xF194 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_SysSuppECUSoftwareVersionNumber    = 0xF195u,  /**< @brief System Supplier ECU Software Version Number (0xF195 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier ) @see ISO 14229-1 */
 LinUDSIf_eDID_SystemNameOrEngineType             = 0xF197u,  /**< @brief System Name or Engine Type (0xF197 - systemNameOrEngineTypeDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_ODXFileId                          = 0xF19Eu,  /**< @brief ODX File ID (0xF19E - ODXFileDataIdentifier) @see ISO 14229-1 */
 LinUDSIf_eDID_ODXFileVersion                     = 0xF1A2u,  /**< @brief ODX File Version (0xF1A2) @see VW 80125 */
 LinUDSIf_eDID_ECUHWVersionNumber                 = 0xF1A3u,  /**< @brief VW ECU Hardware Version Number (0xF1A3 - VWECUHardwareVersionNumber) @see VW 80125 */
 LinUDSIf_eDID_LogicalSwBlkVersion                = 0xF1ABu,  /**< @brief VW Logical Software Block Version  (0xF1AB) @see VW 80125 */
 LinUDSIf_eDID_ECUProgInfo                        = 0xF1DFu,  /**< @brief ECU Programming Information (0xF1DF) @see VW 80125 */
 LinUDSIf_eDID_INVALID                            = 0xFFFFu
};

typedef enum LinUDSIf_eDataIdentifier LinUDSIf_eDataIdentifier_t; /**< @brief Typedef of LinUDSIf_DID_t. */



/* ****************************************************************************/
/* ************** STORED DATA TRANSMISSION FUNCTIONAL UNIT ********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ***************** ClearDiagnosticInformation Service ***********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************** ReadDTCInformation Service **************************/
/* ****************************************************************************/

/***************************************************************************//**
 * ReadDTCInformation (0x19) - Enumerator for DTCStatusMask - DTCSM - DTC status bit definitions
 ******************************************************************************/
enum LinUDSIf_eDTCStatusMask
{
  LinUDSIf_DTCSM_testFailed                         = 0x01u, /**< Bit 0 - testFailed - DTCSM_TF **/
  LinUDSIf_DTCSM_testFailedThisOperationCycle       = 0x02u, /**< Bit 1 - testFailedThisOperationCycle - DTCSM_TFTOC **/
  LinUDSIf_DTCSM_pendingDTC                         = 0x04u, /**< Bit 2 - pendingDTC - DTCSM_PDTC **/
  LinUDSIf_DTCSM_confirmedDTC                       = 0x08u, /**< Bit 3 - confirmedDTC - DTCSM_CDTC **/
  LinUDSIf_DTCSM_testNotCompletedSinceLastClear     = 0x10u, /**< Bit 4 - testNotCompletedSinceLastClear - DTCSM_TNCSLC **/
  LinUDSIf_DTCSM_testFailedSinceLastClear           = 0x20u, /**< Bit 5 - testFailedSinceLastClear - DTCSM_TFSLC **/
  LinUDSIf_DTCSM_testNotCompletedThisOperationCycle = 0x40u, /**< Bit 6 - testNotCompletedThisOperationCycle - DTCSM_TNCTOC **/
  LinUDSIf_DTCSM_warningIndicatorRequested          = 0x80u  /**< Bit 7 - warningIndicatorRequested - DTCSM_WIR **/
};

typedef enum LinUDSIf_eDTCStatusMask LinUDSIf_eDTCStatusMask_t; /**< Typedef of struct LinUDSIf_eDTCStatusMask. */

/***************************************************************************//**
 *  ReadDTCInformation (0x19) - Struct for DTCStatusMask - DTCSM
 ******************************************************************************/
struct LinUDSIf_sDTCStatusMask
{
    Lin_uint8_t TF     :1; /**< Bit 0 - testFailed                         **/
    Lin_uint8_t TFTOC  :1; /**< Bit 1 - testFailedThisOperationCycle       **/
    Lin_uint8_t PDTC   :1; /**< Bit 2 - pendingDTC                         **/
    Lin_uint8_t CDTC   :1; /**< Bit 3 - confirmedDTC                       **/
    Lin_uint8_t TNCSLC :1; /**< Bit 4 - testNotCompletedSinceLastClear     **/
    Lin_uint8_t TFSLC  :1; /**< Bit 5 - testFailedSinceLastClear           **/
    Lin_uint8_t TNCTOC :1; /**< Bit 6 - testNotCompletedThisOperationCycle **/
    Lin_uint8_t WIR    :1; /**< Bit 7 - warningIndicatorRequested          **/
};

/***************************************************************************//**
 * ReadDTCInformation (0x19) - Enumerator for Sub-Function 'reportType'
 ******************************************************************************/
enum LinUDSIf_eSubFunc_reportType
{
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_0x00                             = 0x00u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_reportNumberOfDTCByStatusMask                   = 0x01u, /**< reportNumberOfDTCByStatusMask - RNODTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportDTCByStatusMask                           = 0x02u, /**< reportDTCByStatusMask - RDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportDTCSnapshotIdentification                 = 0x03u, /**< reportDTCSnapshotIdentification - RDTCSSI */
  LinUDSIf_SUBFUNC_RPT_reportDTCSnapshotRecordByDTCNumber              = 0x04u, /**< reportDTCSnapshotRecordByDTCNumber - RDTCSSBDTC */
  LinUDSIf_SUBFUNC_RPT_reportDTCStoredDataByRecordNumber               = 0x05u, /**< reportDTCStoredDataByRecordNumber - RDTCSDBRN */
  LinUDSIf_SUBFUNC_RPT_reportDTCExtDataRecordByDTCNumber               = 0x06u, /**< reportDTCExtDataRecordByDTCNumber - RDTCEDRBDN */
  LinUDSIf_SUBFUNC_RPT_reportNumberOfDTCBySeverityMaskRecord           = 0x07u, /**< reportNumberOfDTCBySeverityMaskRecord - RNODTCBSMR */
  LinUDSIf_SUBFUNC_RPT_reportDTCBySeverityMaskRecord                   = 0x08u, /**< reportDTCBySeverityMaskRecord - RDTCBSMR */
  LinUDSIf_SUBFUNC_RPT_reportSeverityInformationOfDTC                  = 0x09u, /**< reportSeverityInformationOfDTC - RSIODTC */
  LinUDSIf_SUBFUNC_RPT_reportSupportedDTC                              = 0x0Au, /**< reportSupportedDTC - RSUPDTC */
  LinUDSIf_SUBFUNC_RPT_reportFirstTestFailedDTC                        = 0x0Bu, /**< reportFirstTestFailedDTC - RFTFDTC */
  LinUDSIf_SUBFUNC_RPT_reportFirstConfirmedDTC                         = 0x0Cu, /**< reportFirstConfirmedDTC - RFCDTC */
  LinUDSIf_SUBFUNC_RPT_reportMostRecentTestFailedDTC                   = 0x0Du, /**< reportMostRecentTestFailedDTC - RMRTFDTC */
  LinUDSIf_SUBFUNC_RPT_reportMostRecentConfirmedDTC                    = 0x0Eu, /**< reportMostRecentConfirmedDTC - RMRCDTC */
  LinUDSIf_SUBFUNC_RPT_reportMirrorMemoryDTCByStatusMask               = 0x0Fu, /**< reportMirrorMemoryDTCByStatusMask - RMMDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportMirrorMemoryDTCExtDataRecordByDTCNumber   = 0x10u, /**< reportMirrorMemoryDTCExtDataRecordByDTCNumber - RMMDEDRBDN */
  LinUDSIf_SUBFUNC_RPT_reportNumberOfMirrorMemoryDTCByStatusMask       = 0x11u, /**< reportNumberOfMirrorMemoryDTCByStatusMask - RNOMMDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportNumberOfEmissionsOBDDTCByStatusMask       = 0x12u, /**< reportNumberOfEmissionsOBDDTCByStatusMask - RNOOEOBDDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportEmissionsOBDDTCByStatusMask               = 0x13u, /**< reportEmissionsOBDDTCByStatusMask - ROBDDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportDTCFaultDetectionCounter                  = 0x14u, /**< reportDTCFaultDetectionCounter - RDTCFDC */
  LinUDSIf_SUBFUNC_RPT_reportDTCWithPermanentStatus                    = 0x15u, /**< reportDTCWithPermanentStatus - RDTCWPS */
  LinUDSIf_SUBFUNC_RPT_reportDTCExtDataRecordByRecordNumber            = 0x16u, /**< reportDTCExtDataRecordByRecordNumber - RDTCEDBR */
  LinUDSIf_SUBFUNC_RPT_reportUserDefMemoryDTCByStatusMask              = 0x17u, /**< reportUserDefMemoryDTCByStatusMask - RUDMDTCBSM */
  LinUDSIf_SUBFUNC_RPT_reportUserDefMemoryDTCSnapshotRecordByDTCNumber = 0x18u, /**< reportUserDefMemoryDTCSnapshotRecordByDTCNumber - RUDMDTCSSBDTC */
  LinUDSIf_SUBFUNC_RPT_reportUserDefMemoryDTCExtDataRecordByDTCNumber  = 0x19u, /**< reportUserDefMemoryDTCExtDataRecordByDTCNumber - RUDMDTCEDRBDN */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_Start_0x1A                       = 0x1Au, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_End_0x41                         = 0x41u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_reportWWHOBDDTCByMaskRecord                     = 0x42u, /**< reportWWHOBDDTCByMaskRecord - RWWHOBDDTCBMR */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_Start_0x43                       = 0x43u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_End_0x54                         = 0x54u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_reportWWHOBDDTCWithPermanentStatus              = 0x55u, /**< reportWWHOBDDTCWithPermanentStatus - RWWHOBDDTCWPS */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_Start_0x56                       = 0x56u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RPT_ISOSAEReserved_End_0x7F                         = 0x7Fu, /**< ISOSAEReserved - ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_reportType LinUDSIf_SubFunc_reportType_t; /**< Typedef of struct LinUDSIf_eSubFunc_reportType. */

/* ****************************************************************************/
/* ****************** INPUT/OUTPUT CONTROL FUNCTIONAL UNIT ********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ***************** InputOutputControlByIdentifier Service *******************/
/* ****************************************************************************/
typedef Lin_uint8_t LinUDSIf_IOCtrlParameter_t; /**< Data type for 'IOCtrlParameter' parameter of 'InputOutputControlByIdentifier' service. */

/***************************************************************************//**
 * InputOutputControlByIdentifier (0x2F) - Enumerator for inputOutputControlParameter parameter - IOCP
 ******************************************************************************/
enum LinUDSIf_eInputOutputControlParameter
{
  LinUDSIf_IOCP_returnControlToECU        = 0x00u, /**< returnControlToECU - RCTECU*/
  LinUDSIf_IOCP_resetToDefault            = 0x01u, /**< resetToDefault - RTD*/
  LinUDSIf_IOCP_freezeCurrentState        = 0x02u, /**< freezeCurrentState - FCS */
  LinUDSIf_IOCP_shortTermAdjustment       = 0x03u, /**< shortTermAdjustment - STA */
  LinUDSIf_IOCP_ISOSAEReserved_Start_0x04 = 0x80u, /**< ISOSAEReserved - ISOSAERESRVD */
  LinUDSIf_IOCP_ISOSAEReserved_Start_0xFF = 0xFFu  /**< ISOSAEReserved - ISOSAERESRVD */
};

typedef enum LinUDSIf_eInputOutputControlParameter LinUDSIf_eInputOutputControlParameter_t; /**< Typedef of struct LinUDSIf_eInputOutputControlParameter. */

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Input Output Control By Identifier' callback
 *        function.
 *
 * @param genericUdsEnvData[in]     Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]             Pointer to the UDS Layer interface function struct.
 * @param did[in]                   Data Identifier (DID).
 * @param ioCtrlParam[in]           The 'Input Output Control Parameter'
 *                                  (@see LinUDSIf_eInputOutputControlParameter)
 * @param ctrlStateData[in]         Pointer to the received 'controlState' data.
 * @param ctrlStateDataLen[in]      Length of the received 'controlState' data.
 * @param ctrlEnMsk[in]             Pointer to the received
 *                                  'controlEnableMaskRecord' data.
 * @param ctrlEnMskLen[in]          Length of the received
 *                                  'controlEnableMaskRecord' data.
 * @param appendDataToMsgBufFun[in] Pointer to function to append data to the
 *                                  message buffer.
 * @param pNcr[out]                 Pointer to NCR variable in case of an error.
 * @param genericUdsCbCtxData[in]   Pointer to UDS Layer callback context data.
 *
 * The callback is called after reception of 'InputOutputControlByIdentifier'
 * (0x2F) service request.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_ioctrlcbcalled.dox
 * @n
 *
 * @return LIN_TRUE if data has been written successfully to the memory.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_IOCtrlCbFun_t) (LinUDSIf_pGenericEnvData_t       genericUdsEnvData,     LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                              LinUDSIf_DID_t                   did,                   LinUDSIf_IOCtrlParameter_t      ioCtrlParam,
                                              LinUDSIf_pData_t                 ctrlStateData,         LinUDSIf_BufLength_t            ctrlStateDataLen,
                                              LinUDSIf_pData_t                 ctrlEnMsk,             LinUDSIf_BufLength_t            ctrlEnMskLen,
                                              LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun, LinUDSIf_pNRC_t                 pNcr,
                                              LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/* ****************************************************************************/
/* ************* REMOTE ACTIVATION OF ROUTINE FUNCTIONAL UNIT *****************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ RoutineControl Service ****************************/
/* ****************************************************************************/
typedef Lin_uint8_t  LinUDSIf_RoutineCtrlType_t;      /**< Data type for the 'Routine Control' service parameter 'Routine Control Type'. */
typedef Lin_uint16_t LinUDSIf_RoutineIdentifier_t;    /**< Data type for the 'Routine Control' service parameter 'Routine Identifier'. */
typedef Lin_uint8_t* LinUDSIf_pRoutineCtrlOptRcrd_t;  /**< Pointer to 'Routine Control' service parameter 'Routine Control Option Record'. */

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Routine ID Length' callback function.
 *
 * @param genericUdsEnvData[in]     Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]             Pointer to the UDS Layer interface function struct.
 * @param routineId[in]             The ID of the routine to control.
 * @param routineCtrlType[in]       Sub-function of the 'Routine ID' service.
 *                                  (@see LinUDSIf_eSubFunc_SubroutineControlType)
 * @param genericUdsCbCtxData[in]   Pointer to UDS Layer callback context data.
 *
 * The callback is called after reception of 'Routine Control' (0x31) service
 * request to determine the length of the data send back at the response.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_routinectrllencbcalled.dox
 * @n
 *
 * @return The data length for this 'Routine Control' service request.
 *
 ******************************************************************************/
typedef LinUDSIf_BufLength_t (*LinUDSIf_RoutineIdLenCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData,  LinUDSIf_cpInterfaceFunctions_t udsIfFuns,
                                                              LinUDSIf_RoutineIdentifier_t routineId,          LinUDSIf_RoutineCtrlType_t      routineCtrlType,
                                                              LinUDSIf_pGenericCbCtxData_t genericUdsCbCtxData);

/***************************************************************************//**
 * @brief Typedef of UDS Layer 'Routine Control' callback function.
 *
 * @param genericUdsEnvData[in]     Pointer to reserved UDS Layer environment data.
 * @param udsIfFuns[in]             Pointer to the UDS Layer interface function struct.
 * @param suppressPosRspMsg[in]
 * @param routineId[in]             The ID of the routine to control.
 * @param pRoutineCtrlOptRcrd[in]
 * @param routineCtrlOptRcrdLen[in]
 * @param appendDataToMsgBufFun[in] Pointer to function to append data to the
 *                                  message buffer.
 * @param pNcr[out]                 Pointer to NCR variable in case of an error.
 * @param genericUdsCbCtxData[in]   Pointer to UDS Layer callback context data.
 *
 * The callback is called after reception of 'Routine Control' (0x31) service
 * request to determine the length of the data send back at the response.
 *
 * @par "Call Description:"
 * @mscfile msc_uds_routinectrllencbcalled.dox
 * @n
 *
 * @return The data length for this 'Routine Control' service request.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinUDSIf_RoutineControlCbFun_t) (LinUDSIf_pGenericEnvData_t   genericUdsEnvData,     LinUDSIf_cpInterfaceFunctions_t  udsIfFuns,
                                                      Lin_Bool_t                   suppressPosRspMsg,     LinUDSIf_RoutineCtrlType_t       routineCtrlType,
                                                      LinUDSIf_RoutineIdentifier_t routineId,             LinUDSIf_pRoutineCtrlOptRcrd_t   pRoutineCtrlOptRcrd,
                                                      LinUDSIf_BufLength_t         routineCtrlOptRcrdLen, LinUDSIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                      LinUDSIf_pNRC_t              pNrc,                  LinUDSIf_pGenericCbCtxData_t     genericUdsCbCtxData);

/***************************************************************************//**
 * Routine ID Lookup Entry struct
 ******************************************************************************/
struct LinUDSIf_sRoutineIdLookupEntry
{
  LinUDSIf_RoutineIdentifier_t          FirstRoutineId;  /**< Start of Routine ID range to map to one callback function. */
  LinUDSIf_RoutineIdentifier_t          LastRoutineId;   /**< End of Routine ID range to map to one callback function. */
  LinUDSIf_RoutineControlCbFun_t        RoutineIdCbFun;  /**< Pointer to Routine ID callback function. */
  LinUDSIf_RoutineIdLenCbFun_t          LengthCbFun;     /**< Pointer to Routine ID Length callback function. */
  LinUDSIf_LookupIdProcCbCtxDataType_t  CbCtxType;       /**< Callback context type. */
  LinUDSIf_pGenericCbCtxData_t          CbCtxData;       /**< Pointer to callback context data. */
};

typedef struct LinUDSIf_sRoutineIdLookupEntry    LinUDSIf_sRoutineIdLookupEntry_t;  /**< Typedef of struct LinUDSIf_sRoutineIdLookupEntry. */
typedef LinUDSIf_sRoutineIdLookupEntry_t*        LinUDSIf_pRoutineIdLookupEntry_t;  /**< Pointer to struct LinUDSIf_sRoutineIdLookupEntry. */
typedef const LinUDSIf_sRoutineIdLookupEntry_t*  LinUDSIf_cpRoutineIdLookupEntry_t; /**< Pointer to constant struct LinUDSIf_sRoutineIdLookupEntry. */

/***************************************************************************//**
 *  RoutineControl (0x31) - Sub-Function 'routineControlType' - LEV_RCTP_
 ******************************************************************************/
enum LinUDSIf_eSubFunc_SubroutineControlType
{
  LinUDSIf_SUBFUNC_RCTP_ISOSAEReserved_0x00       = 0x00u, /**< ISOSAEReserved - LEV_RCTP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RCTP_startRoutine              = 0x01u, /**< startRoutine - LEV_RCTP_STR */
  LinUDSIf_SUBFUNC_RCTP_stopRoutine               = 0x02u, /**< stopRoutine - LEV_RCTP_STPR */
  LinUDSIf_SUBFUNC_RCTP_requestRoutineResults     = 0x03u, /**< requestRoutineResults - LEV_RCTP_RRR */
  LinUDSIf_SUBFUNC_RCTP_ISOSAEReserved_Start_0x04 = 0x04u, /**< ISOSAEReserved - LEV_RCTP_ISOSAERESRVD */
  LinUDSIf_SUBFUNC_RCTP_ISOSAEReserved_End_0x7F   = 0x7Fu, /**< ISOSAEReserved - LEV_RCTP_ISOSAERESRVD */
};

typedef enum LinUDSIf_eSubFunc_SubroutineControlType LinUDSIf_eSubFunc_SubroutineControlType_t; /**< Typedef of struct LinUDSIf_eSubFunc_SubroutineControlType. */

/* ****************************************************************************/
/* ******************* UPLOAD/DOWNLOAD FUNCTIONAL UNIT ************************/
/* ****************************************************************************/
typedef Lin_uint8_t  LinUDSIf_CompressionMethod_t;                  /**< Data type for the 'Upload/Download Functional Unit' parameter 'Compression Method'. */
typedef Lin_uint8_t  LinUDSIf_EncryptionMethod_t;                   /**< Data type for the 'Upload/Download Functional Unit' parameter 'Encryption Method'. */
typedef Lin_uint8_t  LinUDSIf_MemSizeParamLen_t;                    /**< Data type for the Length of the 'Upload/Download Functional Unit' parameter 'Memory Size Parameter'. */
typedef Lin_uint8_t  LinUDSIf_MemAddressParamLen_t;                 /**< Data type for the Length of the 'Upload/Download Functional Unit' parameter 'Memory Address Parameter'. */
typedef Lin_uint8_t* LinUDSIf_pMemoryAddress_t;                     /**< Pointer to the 'Upload/Download Functional Unit' parameter 'Memory Address Parameter'. */
typedef Lin_uint8_t* LinUDSIf_pMemorySize_t;                        /**< Pointer to the 'Upload/Download Functional Unit' parameter 'Memory Size Parameter'. */
typedef Lin_uint8_t  LinUDSIf_MaxNumOfBlockLenParamLen_t;           /**< Data type for the Length of the 'Upload/Download Functional Unit' parameter 'Maximal Block Length Parameter'. */
typedef Lin_uint8_t* LinUDSIf_pMaxNumberOfBlockLength_t;            /**< Pointer to the 'Upload/Download Functional Unit' parameter 'Maximal Block Length Parameter'. */
typedef Lin_uint8_t  LinUDSIf_BlkSeqCnt_t;                          /**< Data type for the 'Upload/Download Functional Unit' parameter 'Block Sequence Count'. */
typedef Lin_uint8_t* LinUDSIf_TransParamRec_t;                      /**< Pointer to the 'Upload/Download Functional Unit' parameter 'Transfer Parameter Record'. */
typedef Lin_uint16_t LinUDSIf_TransParamRecLen_t;                   /**< Data type for the Length of the 'Upload/Download Functional Unit' parameter 'Transfer Parameter Record'. */
typedef LinUDSIf_TransParamRecLen_t* LinUDSIf_pTransParamRecLen_t;  /**< Pointer to the 'Upload/Download Functional Unit' parameter 'Transfer Parameter Record Length'. */


/***************************************************************************//**
 * Enumerator for the request type
 ******************************************************************************/
enum LinUDSIf_eTransReqType
{
  LinUDSIf_TRT_TransferData        = 0u,
  LinUDSIf_TRT_RequestTransferExit = 1u
};

typedef enum LinUDSIf_eTransReqType LinUDSIf_eTransReqType_t;

/***************************************************************************//**
 * Enumerator for the direction of a data transfer.
 ******************************************************************************/
enum LinUDSIf_eTransDirection
{
  LinUDSIf_TD_Upload   = 0u, /**< Data upload. */
  LinUDSIf_TD_Download = 1u  /**< Data download. */
};

typedef enum LinUDSIf_eTransDirection LinUDSIf_eTransDirection_t; /**< Typedef of LinUDSIf_eTransDirection. */

/***************************************************************************//**
 * Struct for the 'Address And Length Format Identifier'.
 ******************************************************************************/
struct LinUDSIf_sAddressAndLengthFormatIdentifier
{
  LinUDSIf_MemAddressParamLen_t MemAddressParamLen :4; /**< Length of the 'Memory Address' parameter. */
  LinUDSIf_MemSizeParamLen_t    MemSizeParamLen    :4; /**< Length of the 'Memory Size' parameter. */
};

typedef struct LinUDSIf_sAddressAndLengthFormatIdentifier LinUDSIf_sAddressAndLengthFormatIdentifier_t; /**< Typedef of struct LinUDSIf_sAddressAndLengthFormatIdentifier. */
typedef LinUDSIf_sAddressAndLengthFormatIdentifier_t* LinUDSIf_pAddressAndLengthFormatIdentifier_t;     /**< Typedef of pointer to struct  LinUDSIf_sAddressAndLengthFormatIdentifier. */

/***************************************************************************//**
 * Struct for all parameter in a Upload/Download request.
 ******************************************************************************/
struct LinUDSIf_sReqDataTransRecParam
{
  LinUDSIf_EncryptionMethod_t   EncryptionMethod;   /**< Encryption Method */
  LinUDSIf_CompressionMethod_t  CompressionMethod;  /**< Compression Method */
  LinUDSIf_MemAddressParamLen_t MemAdrParamLen;     /**< Length of the 'Memory Address' parameter. */
  LinUDSIf_MemSizeParamLen_t    MemSzeParamLen;     /**< Length of the 'Memory Size' parameter. */
  LinUDSIf_pMemoryAddress_t     MemoryAddressPtr;   /**< Pointer to the 'Memory Address' parameter. */
  LinUDSIf_pMemorySize_t        MemorySizePtr;      /**< Pointer to the 'Memory Size' parameter. */
};

typedef struct LinUDSIf_sReqDataTransRecParam LinUDSIf_sReqDataTransRecParam_t; /**< Typedef of struct LinUDSIf_sReqDataTransRecParam. */
typedef LinUDSIf_sReqDataTransRecParam_t*     LinUDSIf_pReqDataTransRecParam_t; /**< Typedef of pointer to struct LinUDSIf_sReqDataTransRecParam. */

/***************************************************************************//**
 * Struct for all parameter in a response to a Upload/Download request.
 ******************************************************************************/
struct LinUDSIf_sReqDataTransRetParam
{
  LinUDSIf_MaxNumOfBlockLenParamLen_t MaxNumOfBlockLenParamLen;   /**< Length of the 'maxNumberOfBlockLength' parameter. */
  LinUDSIf_pMaxNumberOfBlockLength_t  MaxNumberOfBlockLengthPtr;  /**< Pointer to the parameter holding the maximal length of a data block received/transmitted by the server. */
  Lin_uint8_t                         MaxNumOfBlkLenBufLen;       /**< Length of the buffer of the 'MaxNumOfBlockLenParamLen'. */
};

typedef struct LinUDSIf_sReqDataTransRetParam LinUDSIf_sReqDataTransRetParam_t; /**< Typedef of struct LinUDSIf_sReqDataTransRetParam. */
typedef LinUDSIf_sReqDataTransRetParam_t*     LinUDSIf_pReqDataTransRetParam_t; /**< Typedef of pointer to struct LinUDSIf_sReqDataTransRetParam. */

/***************************************************************************//**
 * Struct for the 'transferRequestParameterRecord' and 'transferResponseParameterRecord'.
 ******************************************************************************/
struct LinUDSIf_sTransParamRec
{
    LinUDSIf_TransParamRec_t    TransParamRec;    /**< Pointer to the 'transferRequestParameterRecord' and 'transferResponseParameterRecord' */
    LinUDSIf_TransParamRecLen_t TransParamRecLen; /**< Length of the 'transferRequestParameterRecord' and 'transferResponseParameterRecord' */
};

typedef struct LinUDSIf_sTransParamRec LinUDSIf_sTransParamRec_t; /**< Typedef of struct LinUDSIf_sTransParamRec. */
typedef LinUDSIf_sTransParamRec_t*     LinUDSIf_pTransParamRec_t; /**< Typedef of pointer to struct LinUDSIf_sTransParamRec. */

/* ****************************************************************************/
/* ***************************** DID LOOKUP ***********************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Enumerator for the two types to determine the length of data defined by a DID.
 ******************************************************************************/
enum LinUDSIf_eDidLenType
{
  LinUDSIf_DidLenType_Value    = 0,  /**< Defined by a constant value in a lookup entry. */
  LinUDSIf_DidLenType_Callback = 1   /**< Defined by a length callback function. */
};

typedef enum LinUDSIf_eDidLenType LinUDSIf_eDidLenType_t; /**< Typedef of struct LinUDSIf_eDidLenType. */

/***************************************************************************//**
 * Struct for a DID lookup entry type.
 ******************************************************************************/
typedef enum LinUDSIf_eLookupEntryType
{
  LinUDSIf_LookupEntryType_ReadDataFun  = 0u,
  LinUDSIf_LookupEntryType_WrtCbFun     = 1u,
  LinUDSIf_LookupEntryType_IOCtrlCbFun  = 2u,
  LinUDSIf_LookupEntryType_RbiFun       = 3u,
  LinUDSIf_LookupEntryType_RCtrlFun     = 4u
}LinUDSIf_eLookupEntryType_t;


/***************************************************************************//**
 * Struct for a DID lookup entry.
 ******************************************************************************/
struct LinUDSIf_sDidLookupEntry
{
  LinUDSIf_DID_t                        FirstDid;    /**< Start of the DID mapping range. */
  LinUDSIf_DID_t                        LastDid;     /**< End of the DID mapping range. */
  LinUDSIf_RdDataByIdCbFun_t            RdCbFun;     /**< Pointer to the 'Read' callback function. */
  LinUDSIf_WrtDataByIdCbFun_t           WrtCbFun;    /**< Pointer to the 'Write' callback function. */
  LinUDSIf_IOCtrlCbFun_t                IOCtrlCbFun; /**< Pointer to the 'IO Control' callback function. */
  LinUDSIf_eDidLenType_t                LengthType;  /**< Type of the length value. */
  LinUDSIf_DidLength_t                  Length;      /**< Data length value or pointer to length callback function. */
  LinUDSIf_LookupIdProcCbCtxDataType_t  CbCtxType;   /**< Callback context type. */
  LinUDSIf_pGenericCbCtxData_t          CbCtxData;   /**< Pointer to callback context data. */
};

typedef struct LinUDSIf_sDidLookupEntry    LinUDSIf_sDidLookupEntry_t;  /**< Typedef of struct LinUDSIf_sDidLookupEntry. */
typedef LinUDSIf_sDidLookupEntry_t*        LinUDSIf_pDidLookupEntry_t;  /**< Typedef of pointer to struct LinUDSIf_sDidLookupEntry. */
typedef const LinUDSIf_sDidLookupEntry_t*  LinUDSIf_cpDidLookupEntry_t; /**< Typedef of pointer to constant struct LinUDSIf_sDidLookupEntry. */


/***************************************************************************//**
 * Enumerator for the method id check (part of SNPD)
 ******************************************************************************/
enum LinUDSIf_MethodIdCheckResult
{
  LinUDSIf_MethodId_Accept     = 0x00u, /**< Accept the requested method id */
  LinUDSIf_MethodId_Ignore     = 0x01u, /**< Ignore the requested method id */
};

typedef enum LinUDSIf_MethodIdCheckResult LinUDSIf_MethodIdCheckResult_t; /**< Typedef of LinUDSIf_MethodIdCheckResult. */
typedef      Lin_uint8_t                  LinUDSIf_MethodId_t;            /**< Typedef for the method-id (SNPD). */


/***************************************************************************//**
 * Callback function for the LIN diagnostic service "Check SNPD method id"
 * if the configuration flag "LINUDS_SUP_SNPD" is set.
 *
 * @param genericUDSEnvData[in]    Pointer to reserved DIAG Layer environment data.
 * @param ifFunctions[in]           Pointer to the DIAG Layer interface function struct.
 * @param methodId[in]              The method ID to check.
 * @param genericUDSCbCtxData[in]  Pointer to DIAG Layer callback context data.
 *
 * @return (@ref LinUDSIf_MethodId_Accept)
 *
 ******************************************************************************/
typedef LinUDSIf_MethodIdCheckResult_t (*LinUDSIf_CheckSNPDMethodIdCbFun_t) (LinUDSIf_pGenericEnvData_t genericUDSEnvData, LinUDSIf_cpInterfaceFunctions_t ifFunctions,
                                                       LinUDSIf_MethodId_t methodId, LinUDSIf_pGenericCbCtxData_t genericUDSCbCtxData);


/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINUDS_TYPES_H_ */

/*! @} */ /* DIAG_UDS */
/*! @} */ /* ELMOS_LIN_DRIVER */
