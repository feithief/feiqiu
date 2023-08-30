/***************************************************************************//**
 * @file			LinUDS_Implementation.h
 *
 * @creator		sbai
 * @created		12.02.2015
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief  		UDS on LIN bus implementation
 *
 * @purpose
 *
 * Implements Unified Diagnostic Services for LIN
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG_UDS DIAG_UDS Layer
 * @{
 *
 * $Id: LinUDS_Implementation.h 8305 2021-02-03 13:29:35Z poe $
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
 * Disclaimer:
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

#ifndef LINUDS_IMPLEMENTATION_H_
#define LINUDS_IMPLEMENTATION_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDrvImp_CompilationConfig.h"
#include "LinUDS_Interface.h"
#include "LinDataStg_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

// Version number depends on supported features. 
// If new version should be generated, increment high nibble of first byte by one (i.e. from 0x0110 to 0x0120)
#define LINUDSIMP_VERSION_BASE        			0x0118u /**< LIN DIAG implementation version. */

#define LINUDSIMP_VERSION_WITH_SNPD   			(LINUDSIMP_VERSION_BASE + 1u)
#define LINUDSIMP_VERSION_WITH_SNPD_DATASTG		(LINUDSIMP_VERSION_BASE + 2u)
// add more features here

#define LINUDSIMP_VERSION_WITH_DATASTG   		(LINUDSIMP_VERSION_BASE + 5u)
// add more features here

#if (LINUDS_SUP_DATASTG == 1) && (LINUDS_SUP_SNPD == 1)
	#define LINUDSIMP_VERSION             		LINUDSIMP_VERSION_WITH_SNPD_DATASTG

#elif (LINUDS_SUP_DATASTG == 1)
	#define LINUDSIMP_VERSION             		LINUDSIMP_VERSION_WITH_DATASTG
		
#elif (LINUDS_SUP_SNPD == 1)
	#define LINUDSIMP_VERSION             		LINUDSIMP_VERSION_WITH_SNPD_DATASTG

#else
	#define LINUDSIMP_VERSION             		LINUDSIMP_VERSION_BASE
	
#endif


#define LINUDSIMP_CONFIG_DATA_VERSION 0x0100  /**< Expected config data version. */

#define LINUDS_SUP_CLASS_1            1 /**< Define for using the LIN diagnostic class 1. */
#define LINUDS_SUP_CLASS_2            0 /**< Define for using the LIN diagnostic class 2. */
#define LINUDS_SUP_CLASS_3            0 /**< Define for using the LIN diagnostic class 3. */

#define ELMOS_BLTEST                  0 /**< Define to enable Elmso Bootloader Test services. */

#define LINUDS_SNPD_REQ_MSG_LEN       5 /**< Define for the data length of an SNPD request message. */

#define LINUDS_SNPD_NAD_POS           4 /**< Define for the postion of th NAD in a SNPD request. */
#define LINUDS_SNPD_METHOD_ID_POS     3 /**< Position of SNPD method id within the LIN request*/

#define LINUDS_SNPD_CMD_MEASURE       1 /**< Define for the SNPD Measure command. */
#define LINUDS_SNPD_CMD_KEEP_NAD      2 /**< Define for the SNPD Keep command. */
#define LINUDS_SNPD_CMD_STORE_NAD     3 /**< Define for the SNPD Store command. */
#define LINUDS_SNPD_CMD_DISABLE       4 /**< Define for the SNPD Disable command. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
/***************************************************************************//**
 * Struct for UDS layer implementation specific configuration data.
 *
 ******************************************************************************/
struct LinUDSImp_sCfgData
{
    Lin_Version_t           Version;                            /**< Config data struct version */

    LinTransIf_sInitParam_t LinTransIfInitParam;                /**< @copydoc LinTransIf_sInitParam */
    LinBusIf_sThis_t        LinBusIfThisPointer;                /**< @copydoc LinBusIf_sThis */
    LinLookupIf_sThis_t     LinLookupIfThisPointer;             /**< @copydoc LinLookupIf_sThis */
#if LINUDS_SUP_DATASTG == 1
    LinDataStgIf_sThis_t              LinDataStgIfThisPointer;  /**< @copydoc LinDataStgIf_sThis */
#else
    LinUDSIf_sProductIdentification_t InitialProdIdent;         /**< LIN Product Identification. Supplier ID, Function ID and Variant ID.  */
    LinUDSIf_NAD_t                    InitialNad;               /**< Initial NAD. (Node Address) */
    LinUDSIf_SerialNumber_t           InitialSerialNumber;      /**< Initial Serial Number. */
#endif
#if LINUDS_SUP_SNPD == 1    
    LinSNPDIf_sThis_t       LinSNPDIfThisPointer;               /**< @copydoc LinSNPDIf_sThis */
    Lin_Bool_t              ImmediateNADUpdate;                 /**< Immediate NAD Update flag. */
#endif    
};

typedef struct LinUDSImp_sCfgData    LinUDSImp_sCfgData_t;  /**< Typedef of struct LinUDSImp_sCfgData. */
typedef        LinUDSImp_sCfgData_t* LinUDSImp_pCfgData_t;  /**< Typedef of pointer to struct LinUDSImp_sCfgData. */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINUDSIMP_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinUDSIf_sInterfaceFunctions_t LinUDSImp_InterfaceFunctions;
#endif /* LINUDSIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Initialization' function.
 *
 * @copydetails LinUDSIf_InitializationIfFun_t
 *
 ******************************************************************************/
 Lin_Bool_t LinUDSImp_Initialization(LinUDSIf_pGenericEnvData_t     genericUdsEnvData,       LinUDSIf_EnvDataSze_t         udsEnvDataSze,
                                     LinUDSIf_cpCallbackFunctions_t udsCbFuns,               LinUDSIf_pGenericCbCtxData_t  genericUdsCbCtxData,
                                     Lin_Bool_t                     invalidReadByIDAnswered, LinUDSIf_pGenericImpCfgData_t genericUdsImpCfgData);

 /***************************************************************************//**
  * @brief Implementation of LIN UDS layer 'Task' function.
  *
  * @copydetails LinUDSIf_TaskIfFun_t
  *
  ******************************************************************************/
void LinUDSImp_Task(LinUDSIf_pGenericEnvData_t genericUdsEnvData);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Get Sub-Interface' function.
 *
 * @copydetails LinUDSIf_GetSubInterfaceIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_GetSubInterface(LinUDSIf_pGenericEnvData_t genericUdsEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Get NAD' function.
 *
 * @copydetails LinUDSIf_GetNADIfFun_t
 *
 ******************************************************************************/
LinTransIf_NAD_t LinUDSImp_GetNad(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_eNADType_t nadtype);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Set NAD' function.
 *
 * @copydetails LinUDSIf_SetNADIfFun_t
 *
 ******************************************************************************/
void LinUDSImp_SetNad(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinTransIf_NAD_t nad);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Add RBI Table' function.
 *
 * @copydetails LinUDSIf_AddRbiTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_AddRbiTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Remove RBI Table' function.
 *
 * @copydetails LinUDSIf_RmvRbiTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_RmvRbiTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Add DID Table' function.
 *
 * @copydetails LinUDSIf_AddDidTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_AddDidTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpDidLookupEntry_t didTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Remove DID Table' function.
 *
 * @copydetails LinUDSIf_RmvDidTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_RmvDidTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpDidLookupEntry_t didTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Add Routine ID Table' function.
 *
 * @copydetails LinUDSIf_AddRoutineIdTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_AddRoutineIdTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpRoutineIdLookupEntry_t routineIdTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Remove Routine ID Table' function.
 *
 * @copydetails LinUDSIf_RmvRoutineIdTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_RmvRoutineIdTable(LinUDSIf_pGenericEnvData_t genericUdsEnvData, LinUDSIf_cpRoutineIdLookupEntry_t routineIdTbl);

/***************************************************************************//**
 * @brief Implementation of LIN UDS layer 'Set Up Timer'  function.
 *
 * @copydetails LinUDSIf_SetUpTimerIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinUDSImp_SetUpTimer(LinUDSIf_pGenericEnvData_t genericDiagEnvData, LinUDSIf_TimerNum_t    timerNum,
                                 LinUDSIf_eTimerType_t type,                   LinUDSIf_Timeout_t     timeout,            
                                 LinUDSIf_pGenericCbCtxData_t genericDiagCbCtxData);

#endif /* LINUDS_IMPLEMENTATION_H_ */


/*! @} */ /* DIAG_UDS */
/*! @} */ /* ELMOS_LIN_DRIVER */
