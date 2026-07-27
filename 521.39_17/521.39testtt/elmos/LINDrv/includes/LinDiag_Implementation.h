/***************************************************************************//**
 * @file			LinDiag_Implementation.h
 *
 * @creator		sbai
 * @created		12.02.2015
 * @sdfv      TODO: Automotive Spice or Elmos Flow or Demo Flow
 *
 * @brief  		TODO: Short description of this module
 *
 * @purpose
 *
 * TODO: A detailed description of this module
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

#ifndef LINDIAG_IMPLEMENTATION_H_
#define LINDIAG_IMPLEMENTATION_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDrvImp_CompilationConfig.h"
#include "LinDiag_Interface.h"
#if LINDIAG_SUP_DATASTG == 1
#include "LinDataStg_Interface.h"
#endif

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

// Version number depends on supported features. 
// If new version should be generated, increment high nibble of first byte by one (i.e. from 0x0110 to 0x0120)
#define LINDIAGIMP_VERSION_BASE        			0x0120u  /**< LIN DIAG implementation version. */

#define LINDIAGIMP_VERSION_WITH_SNPD   			(LINDIAGIMP_VERSION_BASE + 1u)
#define LINDIAGIMP_VERSION_WITH_SNPD_DATASTG	(LINDIAGIMP_VERSION_BASE + 2u)
// add more features here

#define LINDIAGIMP_VERSION_WITH_DATASTG   		(LINDIAGIMP_VERSION_BASE + 5u)
// add more features here

#if (LINDIAG_SUP_DATASTG == 1) && (LINDIAG_SUP_SNPD == 1)
	#define LINDIAGIMP_VERSION             		LINDIAGIMP_VERSION_WITH_SNPD_DATASTG

#elif (LINDIAG_SUP_DATASTG == 1)
	#define LINDIAGIMP_VERSION             		LINDIAGIMP_VERSION_WITH_DATASTG
		
#elif (LINDIAG_SUP_SNPD == 1)
	#define LINDIAGIMP_VERSION             		LINDIAGIMP_VERSION_WITH_SNPD

#else
	#define LINDIAGIMP_VERSION             		LINDIAGIMP_VERSION_BASE
	
#endif


#define LINDIAGIMP_CONFIG_DATA_VERSION 0x0100u /**< Expected config data version. */

#define LINDIAG_SUP_CLASS_1            1 /**< Define for using the LIN diagnostic class 1. */
#define LINDIAG_SUP_CLASS_2            0 /**< Define for using the LIN diagnostic class 2. */
#define LINDIAG_SUP_CLASS_3            0 /**< Define for using the LIN diagnostic class 3. */

#define ELMOS_BLTEST                   0 /**< @internal Define to enable Elmos Bootloader Test services. */

#define LINDIAG_SNPD_REQ_MSG_LEN       5 /**< Define for the data length of an SNPD request message. */

#define LINDIAG_SNPD_NAD_POS           4 /**< Define for the poistion of th NAD in a SNPD request. */

#define LINDIAG_SNPD_CMD_POS           2 /**< Position of SNPD command within the LIN request*/
#define LINDIAG_SNPD_METHOD_ID_POS     3 /**< Position of SNPD method id within the LIN request*/

#define LINDIAG_SNPD_CMD_MEASURE       1 /**< Define for the SNPD Measure command. */
#define LINDIAG_SNPD_CMD_KEEP_NAD      2 /**< Define for the SNPD Keep command. */
#define LINDIAG_SNPD_CMD_STORE_NAD     3 /**< Define for the SNPD Store command. */
#define LINDIAG_SNPD_CMD_DISABLE       4 /**< Define for the SNPD Disable command. */


/* ************************************ ************************************/
/* *********************** STRUCTS, ENU ND TYPEDEFS ************************/
/* ****************************************************************************/
/***************************************************************************//**
 * TODO: Concrete description.
 *
 ******************************************************************************/
struct LinDiagImp_sCfgData
{
    Lin_Version_t                      Version;                 /**< Config data struct version */

    LinTransIf_sInitParam_t            LinTransIfInitParam;     /**< @copydoc LinTransIf_sInitParam */
    LinBusIf_sThis_t                   LinBusIfThisPointer;     /**< @copydoc LinBusIf_sThis */
    LinLookupIf_sThis_t                LinLookupIfThisPointer;  /**< @copydoc LinLookupIf_sThis */
#if LINDIAG_SUP_DATASTG == 1
    LinDataStgIf_sThis_t               LinDataStgIfThisPointer; /**< @copydoc LinDataStgIf_sThis */
#else
    LinDiagIf_sProductIdentification_t InitialProdIdent;        /**< LIN Product Identification. Supplier ID, Function ID and Variant ID.  */
    LinDiagIf_NAD_t                    InitialNad;              /**< Initial NAD. (Node Address) */
    LinDiagIf_SerialNumber_t           InitialSerialNumber;     /**< Initial Serial Number. */
#endif
#if LINDIAG_SUP_SNPD == 1
    LinSNPDIf_sThis_t                  LinSNPDIfThisPointer;    /**< @copydoc LinSNPDIf_sThis */
    Lin_Bool_t                         ImmediateNADUpdate;      /**< Immediate NAD Update flag. */
    Lin_Bool_t                         UseOldSNPDServiceID;     /**< If set, SNPD is bound to SID 0xB5 (TargetedReset will not be available). If cleared the SNPD is assigned to 0xB8 */
#endif    
};

typedef struct LinDiagImp_sCfgData    LinDiagImp_sCfgData_t; /**< Typedef of struct LinDiagImp_sCfgData. */
typedef        LinDiagImp_sCfgData_t* LinDiagImp_pCfgData_t; /**< Typedef of pointer to struct LinDiagImp_sCfgData. */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinDiagIf_sInterfaceFunctions_t LinDiagImp_InterfaceFunctions;
#endif /* LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Initialization' function.
 *
 * @copydetails LinDiagIf_InitializationIfFun_t
 *
 ******************************************************************************/
 Lin_Bool_t LinDiagImp_Initialization(LinDiagIf_pGenericEnvData_t     genericDiagEnvData,      LinDiagIf_EnvDataSze_t         diagEnvDataSze,
                                      LinDiagIf_cpCallbackFunctions_t diagCbFuns,              LinDiagIf_pGenericCbCtxData_t  genericDiagCbCtxData,
                                      Lin_Bool_t                      invalidReadByIDAnswered, LinDiagIf_pGenericImpCfgData_t genericDiagImpCfgData);

/***************************************************************************//**
  * @brief Implementation of LIN DIAG layer 'Task' function.
  *
  * @copydetails LinDiagIf_TaskIfFun_t
 *
 ******************************************************************************/
void LinDiagImp_Task(LinDiagIf_pGenericEnvData_t genericDiagEnvData);

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Get Sub-Interface' function.
 *
 * @copydetails LinDiagIf_GetSubInterfaceIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinDiagImp_GetSubInterface(LinDiagIf_pGenericEnvData_t genericDiagEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Get NAD' function.
 *
 * @copydetails LinDiagIf_GetNADIfFun_t
 *
 ******************************************************************************/
LinTransIf_NAD_t LinDiagImp_GetNad(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_eNADType_t nadtype);

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Set NAD' function.
 *
 * @copydetails LinDiagIf_SetNADIfFun_t
 *
 ******************************************************************************/
void LinDiagImp_SetNad(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinTransIf_NAD_t nad);

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Add RBI Table' function.
 *
 * @copydetails LinDiagIf_AddRbiTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinDiagImp_AddRbiTable(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl);

/***************************************************************************//**
 * @brief Implementation of LIN DIAG layer 'Remove RBI Table' function.
 *
 * @copydetails LinDiagIf_RmvRbiTableIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinDiagImp_RmvRbiTable(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl);

#endif /* LINDIAG_IMPLEMENTATION_H_ */

extern uint16_t Flag_AssignFrameID;
/*! @} */ /* DIAG */
/*! @} */ /* ELMOS_LIN_DRIVER */
