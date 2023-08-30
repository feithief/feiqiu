/***************************************************************************//**
 * @file		LinDiag_Types.h
 *
 * @creator		sbai
 * @created		25.03.2015
 *
 * @brief  		Definitions of basic data types for the 'LIN DIAG Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG DIAG Layer
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

#ifndef LINDIAG_TYPES_H_
#define LINDIAG_TYPES_H_

/* PRQA S 1534,1535 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types and macros are defined that are not used in this project scope. */
/* Rule: 1534 MISRA a Rule-2.5
   Circumstances: The macro '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */
/* Rule: 1535 MISRA a Rule-2.3
   Circumstances: The typedef '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */
/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinTrans_Types.h"

/* ****************************************************************************/
/* *********** GENERAL DEFINES, ENUMS, STRUCTS, DATA TYPES, ETC. **************/
/* ****************************************************************************/
typedef Lin_pvoid_t          LinDiagIf_pGenericEnvData_t;    /**< Generic pointer to environment data of the DIAG layer module. */
typedef Lin_EnvDataSze_t     LinDiagIf_EnvDataSze_t;         /**< LIN DIAG layer data type for the environment data length. */
typedef Lin_pvoid_t          LinDiagIf_pGenericImpCfgData_t; /**< Generic pointer to configuration parameter of the specific DIAG Layer implementation. */
typedef Lin_pvoid_t          LinDiagIf_pGenericCbCtxData_t;  /**< Pointer to DIAG callback context data. */

typedef Lin_Error_t          LinDiagIf_Error_t;        /**< Generic data types for DIAG layer error codes. */

typedef Lin_uint8_t          LinDiagIf_Data_t;   /**< Basic DIAG data type. */
typedef LinDiagIf_Data_t*    LinDiagIf_pData_t;  /**< Pointer to LinDiagIf_Data_t. */
typedef LinDiagIf_Data_t**   LinDiagIf_ppData_t; /**< Pointer-Pointer to LinDiagIf_Data_t. */

typedef Lin_BufLength_t      LinDiagIf_Length_t;   /**< DIAG layer data type of length values. */
typedef LinDiagIf_Length_t*  LinDiagIf_pLength_t;  /**< Pointer to LinDiagIf_BufLength_t. */
typedef LinDiagIf_Length_t** LinDiagIf_ppLength_t; /**< Pointer-Pointer to LinIf_DiagLength_t. */

struct         LinDiagIf_sInterfaceFunctions;                                      /**< Forward declaration of LIN Diag layer interface functions. */
typedef struct LinDiagIf_sInterfaceFunctions     LinDiagIf_sInterfaceFunctions_t;  /**< Typedef of LinDiagIf_sInterfaceFunctions. */
typedef        LinDiagIf_sInterfaceFunctions_t*  LinDiagIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinDiagIf_sInterfaceFunctions. */
typedef const  LinDiagIf_sInterfaceFunctions_t*  LinDiagIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinDiagIf_sInterfaceFunctions. */

struct         LinDiagIf_sCallbackFunctions;                                     /**< Forward declaration of LIN Diag layer callback functions. */
typedef struct LinDiagIf_sCallbackFunctions     LinDiagIf_sCallbackFunctions_t;  /**< Typedef of LinDiagIf_sCallbackFunctions. */
typedef        LinDiagIf_sCallbackFunctions_t*  LinDiagIf_pCallbackFunctions_t;  /**< Typedef of pointer to LinDiagIf_sCallbackFunctions. */
typedef const  LinDiagIf_sCallbackFunctions_t*  LinDiagIf_cpCallbackFunctions_t; /**< Typedef of constant pointer to LinDiagIf_sCallbackFunctions. */

struct         LinDiagIf_sThis;                      /**< Forward declaration of LIN Diag layer This-Pointer. */
typedef struct LinDiagIf_sThis    LinDiagIf_sThis_t; /**< Typedef for LinDiagIf_sThis. */
typedef        LinDiagIf_sThis_t* LinDiagIf_pThis_t; /**< Typedef of pointer to LinDiagIf_sThis. */

struct         LinDiagIf_sInitParam;                           /**< Forward declaration of LIN DIAG layer initialization parameter struct. */
typedef struct LinDiagIf_sInitParam    LinDiagIf_sInitParam_t; /**< Typedef of LinDiagIf_sInitParam. */
typedef        LinDiagIf_sInitParam_t* LinDiagIf_pInitParam_t; /**< Typedef of pointer to LinDiagIf_sInitParam. */

typedef LinTransIf_NAD_t LinDiagIf_NAD_t; /**< Redefinition of the data type for the 'Node Address' of the TRANS layer (NAD)
                                               (See also: LIN 2.2a Specification - Chapter 4.2.3.2 NAD) */
typedef LinTransIf_SID_t LinDiagIf_SID_t; /**< Redefinition of the data type for SID of the TRANS layer (Service Identifier)
                                               (See also: LIN 2.2a Specification - Chapter 3.2.1.5 SID) */

typedef Lin_uint32_t LinDiagIf_Timeout_t; /**< Data type for timeout values of the DIAG layer. */

/***************************************************************************//**
 * Enumerator for the two types of NAD of the DIAG layer.
 ******************************************************************************/
enum LinDiagIf_eNADType
{
  LinDiagIf_NADType_NORMAL  = 0, /**< Normal/Actually NAD. */
  LinDiagIf_NADType_INITIAL = 1  /**< Initial/Stored NAD. */
};

typedef enum LinDiagIf_eNADType LinDiagIf_eNADType_t; /**< Typedef of LinUDSIf_eNADType. */

/***************************************************************************//**
 * @brief Typedef of special DIAG Layer 'Append Data To Message Buffer' interface
 *        function.
 *
 * @param genericDiagEnvData[in] Pointer to reserved DIAG Layer environment data.
 * @param data[in]               Pointer to data which shall be appened to the
 *                               message buffer.
 * @param dataLen[in]            Data length to append.
 *
 * This interface function will be passed as parameter in callback function
 * which needs the possibility for he user to pass specific data to the DIAG
 * message buffer.
 *
 * @return LIN_TRUE if data has been written successfully to the message buffer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_AppendDataToMsgBuffer_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pData_t data, LinDiagIf_Length_t dataLen);

/***************************************************************************//**
 * LIN diagnostic layer error enumerator
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.,
 * Conflicts in  signedness.,
 * Always make sure unsigned values ate defined and used.}
 ******************************************************************************/
// PRQA S 4521 ++
typedef enum LinDiagIf_eErrorCodes
{
  LinDiagIf_ERR_NO_ERROR                                 = Lin_NO_ERROR,                  /**< No error at all. */
  LinDiagIf_ERR_INIT                                     = Lin_ERROR_AREA_DIAG_UDS + 0u,  /**< Initialization error */
  LinDiagIf_ERR_INVALID_NAD                              = Lin_ERROR_AREA_DIAG_UDS + 1u,  /**< invalid NAD specified */
  LinDiagIf_ERR_IN_CORRECT_MESSAGE_LENGHT_INVALID_FORMAT = Lin_ERROR_AREA_DIAG_UDS + 2u,  /**< The data length mismatch for the service. */
  LinDiagIf_ERR_RBI_DEF                                  = Lin_ERROR_AREA_DIAG_UDS + 3u,  /**< Error at "Read by Identifier" definition. */
  LinDiagIf_ERR_RBI_NOT_FOUND                            = Lin_ERROR_AREA_DIAG_UDS + 4u,  /**< No entry for this identifier is found. */
  LinDiagIf_ERR_RBI_DATA_LEN                             = Lin_ERROR_AREA_DIAG_UDS + 5u,  /**< Mismatch between initialized and appended data length for a response. */
  LinDiagIf_ERR_TRANS_LAYER                              = Lin_ERROR_AREA_DIAG_UDS + 10u, /**< Error using Transport Layer. */
  LinDiagIf_ERR_IMPL_ERROR_AREA                          = Lin_ERROR_AREA_DIAG_UDS + (LIN_ERROR_AREA_SIZE/2u) /**< Any additional implementation specific error codes start here. */
} LinDiagIf_eErrorCodes_t;
// PRQA S 4521 --

#define LINDIAGIF_RSID_OFFSET         0x40u     /**< RSID offset. Added up to the SID for the response. */

#define LINDIAGIF_SUPPLIER_ID_WILDCARD  0x7FFFu /**< Supplier ID wildcard. */
#define LINDIAGIF_FUNCTION_ID_WILDCARD  0xFFFFu /**< Function ID wildcard. */

/***************************************************************************//**
 * Enumerator for the Service IDs of the DIAG layer.
 ******************************************************************************/
enum LinDiagIf_ServiceID
{
  /* LIN 2.2 specification services. */
  KBDLinDiagIf_SID_0x22            = 0x22u, /**< KBD 22 sercvice */  
  KBDLinDiagIf_SID_0x2E            = 0x2Eu, /**< KBD 2E sercvice */  
  LinDiagIf_SID_AssignNAD            = 0xB0u, /**< Assign NAD LIN Diagnostic Service */
  LinDiagIf_SID_AssignFrameID        = 0xB1u, /**< Assign Frame ID Diagnostic Service */
  LinDiagIf_SID_ReadByIdentifier     = 0xB2u, /**< Read By Identifier LIN Diagnostic Service */
  LinDiagIf_SID_ConditionalChangeNAD = 0xB3u, /**< Conditional Change NAD LIN Diagnostic Service */
  LinDiagIf_SID_DataDump             = 0xB4u, /**< Data Dump LIN Diagnostic Service */
  LinDiagIf_SID_TargetedReset        = 0xB5u, /**< Targeted Reset (SAE-J2602 only)  */
  LinDiagIf_SID_Old_SNPD             = 0xB5u, /**< Old SNPD SID  */
  LinDiagIf_SID_SaveConfiguration    = 0xB6u, /**< Save Configuration LIN Diagnostic Service */
  LinDiagIf_SID_AssignFrameIDRange   = 0xB7u, /**< Assign Frame ID Range LIN Diagnostic Service */
  LinDiagIf_SID_SNPD                 = 0xB8u, /**< SNPD Service ID according to ISO */
};

typedef enum LinDiagIf_ServiceID LinDiagIf_ServiceID_t; /**< Typedef of LinDiagIf_ServiceID. */

/***************************************************************************//**
 * Negative Response Codes
 ******************************************************************************/
enum LinDiagIf_eNegativeResponseCode
{
  LinDiagIf_NRC_General_Reject             = 0x10u,
  LinDiagIf_NRC_Service_Not_Supported      = 0x11u,
  LinDiagIf_NRC_Sub_Function_Not_Supported = 0x12u,
};

typedef enum LinDiagIf_eNegativeResponseCode LinDiagIf_eNegativeResponseCode_t; /**< Typedef of LinDiagIf_eNegativeResponseCode. */

#define LINDIAGIF_NEG_RESP_RSID  0x7Fu /**< RSID of negative responses. */
#define LINDIAGIF_NEG_RESP_LEN   2u    /**< Data length of negative responses. */

typedef Lin_uint16_t     LinDiagIf_NRC_t;  /**< Generic NRC (Negative Response Code) data type. */
typedef LinDiagIf_NRC_t* LinDiagIf_pNRC_t; /**< Pointer to generic NRC (Negative Response Code) data type. */

typedef struct LinDiagIf_sProduktInformation    LinDiagIf_sProduktInformation_t;
typedef        LinDiagIf_sProduktInformation_t* LinDiagIf_pProduktInformation_t;
/* ****************************************************************************/
/* ****************************** LIN SERVICES ********************************/
/* ****************************************************************************/

#define LINDIAGIF_ASSIGNNAD_REQLEN             5u  /**< Length of a 'Assign NAD' request. */
#define LINDIAGIF_ASSIGNFRAMEID_REQLEN         5u  /**< Length of a 'Assign Frame ID' request. */
#define LINDIAGIF_READBYIDENTIFIER_REQLEN      5u  /**< Length of a 'Read By Identifier' request. */
#define LINDIAGIF_CONDITIONALCHANGENAD_REQLEN  5u  /**< Length of a 'Conditional Change NAD' request. */
#define LINDIAGIF_ASSIGNFRAMEIDRANGE_REQLEN    5u  /**< Length of a 'Assign Frame ID Range' request. */

#define LINDIAGIF_DATA_DUMP_REQLEN              5u  /**< Length of a 'Data Dump' request. */
#define LINDIAGIF_DATA_DUMP_RESPLEN             5u  /**< Length of a 'Data Dump' response. */

/* LIN 2.2 Read by identifier - identifier */

#define LINDIAGIF_RB_IDENTIFIER_PROD_IDENT      0x00u /**< Read By Identifier for 'Product Identification'. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINDIAGIF_RB_IDENTIFIER_PROD_IDENT_LEN  5u    /**< Data length of the 'Product Identification'. */

#define LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM      0x01u /**< Read By Identifier for 'Serial number'. */
//PRQA S 0791 1 #In C99 there are 63 significant characters supported and not 31 like required in this c90 rule
#define LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM_LEN  4u    /**< Data length of the 'Serial number' */

typedef Lin_uint16_t LinDiagIf_SupplierID_t;   /**< Data type for the LIN Supplier ID. */
typedef Lin_uint16_t LinDiagIf_FunctionID_t;   /**< Data type for the LIN Function ID. */
typedef Lin_uint8_t  LinDiagIf_VariantID_t;    /**< Data type for the LIN Variant ID. */
typedef Lin_uint32_t LinDiagIf_SerialNumber_t; /**< Data type for the LIN Serial Number. */

typedef Lin_uint8_t LinDiagIf_RbIdentifier_t; /**< Data type for the LIN Diagnostic 'Read By Identifier' ID. */
typedef Lin_pvoid_t LinDiagIf_RbiLenght_t;    /**< Data type for the data length of an LIN Diagnostic 'Read By Identifier' ID.  */

/***************************************************************************//**
 * Length-Callback function for the LIN diagnostic service "Read By Identifier"
 * if the "Read By Identifier" ID table entry is configured to use a length-
 * callback. (@ref LinDiagIf_RbiLenType_Callback)
 *
 * @param genericDiagEnvData[in] Pointer to reserved DIAG Layer environment data.
 * @param ifFunctions[in]        Pointer to the DIAG Layer interface function struct.
 * @param rbIdentifier          'Read By Identifier' ID.
 *
 * @return Length of an 'Read By Identifier' ID.
 *
 ******************************************************************************/
typedef LinDiagIf_Length_t (*LinDiagIf_RbILenCbFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                                       LinDiagIf_RbIdentifier_t    rbIdentifier);

/***************************************************************************//**
 * 'Read By Identifier' callback function, which is called on reception of the
 * response Frame ID belonging to a 'Read By Identifier' request.
 *
 * @param genericDiagEnvData[in]    Pointer to reserved DIAG Layer environment
 *                                  data.
 * @param ifFunctions[in]           Pointer to the DIAG Layer interface function
 *                                  struct.
 * @param rbIdentifier[in]          'Read By Identifier' ID.
 * @param appendDataToMsgBufFun[in] Pointer to the DIAG 'Append Data To Msg
 *                                  Buffer' function.
 * @param genericUdsCbCtxData[in]   Pointer to DIAG Layer callback context data.
 *
 * @return LIN_TRUE if the user has successfully filled the transmit buffer
 *         with the requested data.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDiagIf_RbIdentifierCbFun_t) (LinDiagIf_pGenericEnvData_t   genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t  ifFunctions,
                                                     LinDiagIf_RbIdentifier_t      rbIdentifier,       LinDiagIf_AppendDataToMsgBuffer_t appendDataToMsgBufFun,
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);

/***************************************************************************//**
 * Enumerator for the length parameter types of a 'Read By Identifier' table
 * entry.
 ******************************************************************************/
enum LinDiagIf_eRbiLenType
{
  LinDiagIf_RbiLenType_Value    = 0,  /**< Integer value for the 'Read By Identifier' data length. */
  LinDiagIf_RbiLenType_Callback = 1   /**< Callback for the 'Read By Identifier' data length. */
};

typedef enum LinDiagIf_eRbiLenType LinDiagIf_eRbiLenType_t; /**< Typedef of LinDiagIf_eRbiLenType. */

/***************************************************************************//**
 * 'Read By Identifier' lookup table entry struct.
 ******************************************************************************/
struct LinDiagIf_sRbiLookupEntry
{
  LinDiagIf_RbIdentifier_t      FirstRbIdentifier; /**< Start of the 'Read By Identifier' ID range. */
  LinDiagIf_RbIdentifier_t      LastRbIdentifier;  /**< End of the 'Read By Identifier' ID range. */
  LinDiagIf_RbIdentifierCbFun_t Callback;          /**< Pointer to the 'Read By Identifier' callback function. */
  LinDiagIf_eRbiLenType_t       LengthType;        /**< Type of the 'Read By Identifier' length value. */
  LinDiagIf_RbiLenght_t         Length;            /**< 'Read By Identifier' length value or callback. */
  LinDiagIf_pGenericCbCtxData_t CbCtxData;         /**< Pointer to the callback data for the 'Read By Identifier' callback function. */
};

typedef struct LinDiagIf_sRbiLookupEntry LinDiagIf_sRbiLookupEntry_t; /**< Typedef of LinDiagIf_sRbiLookupEntry. */
typedef LinDiagIf_sRbiLookupEntry_t*     LinDiagIf_pRbiLookupEntry_t; /**< Typedef of pointer to LinDiagIf_sRbiLookupEntry. */

/***************************************************************************//**
 * Struct for the LIN Product Identification.
 ******************************************************************************/
struct LinDiagIf_sProductIdentification
{
  LinDiagIf_SupplierID_t SupplierID; /**< Supplier ID. */
  LinDiagIf_FunctionID_t FunctionID; /**< Function ID. */
  LinDiagIf_VariantID_t  VariantID;  /**< Variant ID. */
};

typedef struct LinDiagIf_sProductIdentification    LinDiagIf_sProductIdentification_t; /**< Typedef of LinDiagIf_sProductIdentification. */
typedef        LinDiagIf_sProductIdentification_t* LinDiagIf_pProductIdentification_t; /**< Typedef of pointer to LinDiagIf_sProductIdentification. */

/***************************************************************************//**
 * Struct for the LIN Conditional Change NAD Service data.
 ******************************************************************************/
struct LinDiagIf_sConditionalChangeNADData
{
  LinDiagIf_RbIdentifier_t Id;     /**< 'Read By Identifier' ID. */
  LinDiagIf_Data_t         Byte;   /**< The data byte which should be extracted out the data defined by 'Read By Identifier' ID.  */
  LinDiagIf_Data_t         Mask;   /**< Value to do a bitwise XOR with. */
  LinDiagIf_Data_t         Invert; /**< Mask to do a bitwise AND with. */
  LinTransIf_NAD_t         NewNAD; /**< The new NAD to change to if the result is zero. */
};

typedef struct LinDiagIf_sConditionalChangeNADData    LinDiagIf_sConditionalChangeNADData_t;  /**< Typedef of LinDiagIf_sConditionalChangeNADData. */
typedef        LinDiagIf_sConditionalChangeNADData_t* LinDiagIf_pConditionalChangeNADData_t;  /**< Typedef of pointer to LinDiagIf_sConditionalChangeNADData. */


/***************************************************************************//**
 * Enumerator for the method id check (part of SNPD)
 ******************************************************************************/
enum LinDiagIf_MethodIdCheckResult
{
  LinDiagIf_MethodId_Accept     = 0x00u, /**< Accept the requested method id */
  LinDiagIf_MethodId_Ignore     = 0x01u, /**< Ignore the requested method id */
};

typedef enum LinDiagIf_MethodIdCheckResult LinDiagIf_MethodIdCheckResult_t; /**< Typedef of LinDiagIf_MethodIdCheckResult. */
typedef      Lin_uint8_t                   LinDiagIf_MethodId_t;            /**< Typedef for the method-id (SNPD). */

/***************************************************************************//**
 * Callback function for the LIN diagnostic service "Check SNPD method id"
 * if the configuration flag "LINDIAG_SUP_SNPD" is set
 *
 * @param genericDiagEnvData[in]    Pointer to reserved DIAG Layer environment data.
 * @param ifFunctions[in]           Pointer to the DIAG Layer interface function struct.
 * @param methodId[in]              The method ID to check.
 * @param genericDiagCbCtxData[in]  Pointer to DIAG Layer callback context data.
 *
 * @return (@ref LinDiagIf_MethodId_Accept)
 *
 ******************************************************************************/
typedef LinDiagIf_MethodIdCheckResult_t (*LinDiagIf_CheckSNPDMethodIdCbFun_t) (LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_cpInterfaceFunctions_t ifFunctions,
                                                       LinDiagIf_MethodId_t methodId, LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);



/***************************************************************************//**
 * Struct for the LIN Data Dump Service data.
 ******************************************************************************/
struct LinDiagIf_sDataDumpData
{
  LinDiagIf_Data_t D1;     /**< Custom Data byte. */
  LinDiagIf_Data_t D2;     /**< Custom Data byte. */
  LinDiagIf_Data_t D3;     /**< Custom Data byte. */
  LinDiagIf_Data_t D4;     /**< Custom Data byte. */
  LinDiagIf_Data_t D5;     /**< Custom Data byte. */
};

typedef struct LinDiagIf_sDataDumpData    LinDiagIf_sDataDumpData_t;  /**< Typedef of LinDiagIf_sDataDumpData. */
typedef        LinDiagIf_sDataDumpData_t* LinDiagIf_pDataDumpData_t;  /**< Typedef of pointer to LinDiagIf_sDataDumpData. */


#endif /* LINDIAG_TYPES_H_ */

/*! @} */ /* DIAG */
/*! @} */ /* ELMOS_LIN_DRIVER */
