/***************************************************************************//**
 * @file		LinSNPD_Interface.h
 *
 * @creator		rpy
 * @created		9.9.2015
 *
 * @brief  		LIN SNPD layer (Autoadressing) interface.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup SNPD
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

#ifndef LINSNPD_INTERFACE_H_
#define LINSNPD_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinSNPD_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define LINSNPD_INTERFACE_MODULE_API_VERSION          0x0102u /**< LIN SNPD implementation version **/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/

/** @addtogroup LinSNPDIfIfFunDefs */
/** @{*/

/***************************************************************************//**
 * @brief Typedef of SNPD layer 'Initialization' interface function.
 *
 * @param[in] genericSNPDEnvData     Pointer to reserved SNPD Layer environment data.
 * @param[in] SNPDEnvDataSze         Size of the reserved RAM space for SNPD environment
 *                                   data.
 * @param[in] genericSNPDImpCfgData  DIAG layer configuration data depending of the
 *                                   implementation.
 *
 * @return                           LIN_TRUE if the initialization was successfully
 *                                   LIN_FALSE if not.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinSNPDIf_InitializationIfFun_t) (LinSNPDIf_pGenericEnvData_t    genericSNPDEnvData, LinSNPDIf_EnvDataSze_t SNPDEnvDataSze,
                                                       LinSNPDIf_pGenericImpCfgData_t genericSNPDImpCfgData);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'Get Sub-Interface' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 * @param[in]  interfaceId        Sub-Interface ID.
 * @param[out] ifThisPtr          Address (pointer) of an instance of the
 *                                LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinSNPDIf_GetSubInterfaceIfFun_t) (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'Start Measurement' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 *
 * @return  LIN_TRUE if the measurement successfully started.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinSNPDIf_StartMeasurementIfFun_t)  (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'End Measurement' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 *
 ******************************************************************************/
typedef void (*LinSNPDIf_EndMeasurementIfFun_t) (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'Next Measurement' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 * @param[in]  addressed          Device is already addressed?
 *
 * @return  LIN_TRUE if the next measurement successfully started.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinSNPDIf_NextMeasurementIfFun_t) (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_Bool_t addressed);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'Get State' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 *
 * @return  The SNPD module state. (LinSNPDIf_eState_t)
 *
 ******************************************************************************/
typedef LinSNPDIf_eState_t (*LinSNPDIf_GetStateIfFun_t)  (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * @brief Typedef of LIN SNPD layer 'Get Measurement Count' interface function.
 *
 * @param[in]  genericSNPDEnvData Pointer to reserved SNPD Layer environment data.
 *
 * @return  The measurement count.
 *
 ******************************************************************************/
typedef LinSNPDIf_MeasCount_t (*LinSNPDIf_GetMeasurementCountIfFun_t)  (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);


/***************************************************************************//**
 * LIN driver interface function pointer
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This interface function can be dereference outside of this project scope. It should not be hidden. */
struct LinSNPDIf_sInterfaceFunctions
{
  Lin_Version_t                        InterfaceVersion;    /**< @brief SNPD Layer interface version.*/

  LinSNPDIf_InitializationIfFun_t      Initialization;      /**< @brief Pointer to SNPD Layer 'Initialization' interface function. (@ref LinSNPDIf_InitializationIfFun_t) @copydetails LinSNPDIf_InitializationIfFun_t */
  LinSNPDIf_GetSubInterfaceIfFun_t     GetSubInterface;     /**< @brief Pointer to SNPD Layer 'Get Sub-Interface' interface function. (@ref LinSNPDIf_GetSubInterfaceIfFun_t) @copydetails LinSNPDIf_GetSubInterfaceIfFun_t */
  LinSNPDIf_StartMeasurementIfFun_t    StartMeasurement;    /**< @brief Pointer to SNPD Layer 'Start Measurement' interface function. (@ref LinSNPDIf_StartMeasurementIfFun_t) @copydetails LinSNPDIf_StartMeasurementIfFun_t */
  LinSNPDIf_EndMeasurementIfFun_t      EndMeasurement;      /**< @brief Pointer to SNPD Layer 'End Measurement' interface function. (@ref LinSNPDIf_EndMeasurementIfFun_t) @copydetails LinSNPDIf_EndMeasurementIfFun_t */
  LinSNPDIf_NextMeasurementIfFun_t     NextMeasurement;     /**< @brief Pointer to SNPD Layer 'Next Measurement' interface function. (@ref LinSNPDIf_NextMeasurementIfFun_t) @copydetails LinSNPDIf_NextMeasurementIfFun_t */
  LinSNPDIf_GetStateIfFun_t            GetState;            /**< @brief Pointer to SNPD Layer 'Get State' interface function. (@ref LinSNPDIf_GetStateIfFun_t) @copydetails LinSNPDIf_GetStateIfFun_t */
  LinSNPDIf_GetMeasurementCountIfFun_t GetMeasurementCount; /**< @brief Pointer to SNPD Layer 'Get Measurement Count' interface function. (@ref LinSNPDIf_GetMeasurementCountIfFun_t) @copydetails LinSNPDIf_GetMeasurementCountIfFun_t */
};
/** @}*/ /* LinSNPDIfIfFunDefs*/

/***************************************************************************//**
 * @brief Object-like This-pointer to connect the DIAG Layer to other layers.
 ******************************************************************************/
struct LinSNPDIf_sThis
{
  LinSNPDIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the SNPD Layer interface function struct. (@ref LinSNPDIf_sInterfaceFunctions) */
  LinSNPDIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved SNPD Layer environment data. */
};

/***************************************************************************//**
 * @brief LIN SNPD Layer interface configuration parameter.
 *
 * Data needed for initialization of the SNPD Layer.
 ******************************************************************************/
struct LinSNPDIf_sInitParam
{
  LinSNPDIf_cpInterfaceFunctions_t IfFunsTbl;  /**< @brief Pointer to the constant SNPD Layer interface function struct. */
  LinSNPDIf_pGenericEnvData_t      envData;    /**< @brief Pointer to reserved SNPD Layer environment data. */
  LinSNPDIf_EnvDataSze_t           EnvDataLen; /**< @brief Size of the reserved RAM for SNPD Layer environment data. */
  LinSNPDIf_pGenericImpCfgData_t   ImpCfgData; /**< @brief Pointer to implementation dependent configuration data for the SNPD Layer. (LinSNPDImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINSNPD_INTERFACE_H_ */

/*! @} */ /* SNPD */
/*! @} */ /* ELMOS_LIN_DRIVER */

