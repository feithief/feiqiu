/***************************************************************************//**
 * @file      LinDataStg_Interface.h
 *
 * @creator   sbai
 * @created   25.11.2015
 *
 * @brief     Definition of the interface for the 'LIN Data Storage Layer'.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DATASTG
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

#ifndef LINDATASTG_INTERFACE_H_
#define LINDATASTG_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinDatastg_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define LINDATASTG_INTERFACE_MODULE_API_VERSION 0x0101u /**<  @brief DATASTG Layer interface version */

/** @addtogroup LinDataStgIfIfFunDefs */
/** @{*/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ******************************************************************************
 * DATASTG Layer interface function types
 *******************************************************************************/

/***************************************************************************//**
 * @brief Typedef of DATASTG Layer 'Initialization' interface function.
 *
 * @param genericDataStgEnvData[in]     Pointer to reserved DATASTG Layer
 *                                      environment data.
 * @param dataStgEnvDataSze[in]         Size of the reserved RAM for DATASTG Layer
 *                                      environment data.
 * @param dataStgCbFuns[in]             Pointer to DATASTG Layer callback function
 *                                      struct. Implemented in the higher layer or
 *                                      user application.
 *                                      LinDataStgIf_sCallbackFunctions
 * @param genericDataStgCbCtxData[in]   Pointer to DATASTG Layer callback context
 *                                      data.
 * @param genericDataStgImpCfgData[in]  Pointer to implementation dependent
 *                                      configuration data for the DATASTG Layer.
 *                                      LinDataStgImp_sCfgData
 *
 * @return LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the LIN LIN Data Storage layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDataStgIf_InitializationIfFun_t)(LinDataStgIf_pGenericEnvData_t     genericDataStgEnvData,
                                                         LinDataStgIf_EnvDataSze_t          dataStgEnvDataSze,
                                                         LinDataStgIf_cpGenericImpCfgData_t genericImpCfgData);

/***************************************************************************//**
 * @brief Typedef of DATASTG Layer 'Get Sub-Interface' interface function.
 *
 * @param genericDataStgEnvData[in] Pointer to reserved DATASTG Layer
 *                                  environment data.
 * @param interfaceId[in]           Sub-Interface ID.
 * @param ifThisPtr[out]            Variable (pointer) which will be set to the
 *                                  address of the This-Pointer of the desired
 *                                  sub-interface.
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          @p ifThisPtr could be set.
 *
 * Acquires the desired sub-interface (@p interfaceId) and writes the address
 * of its This-Pointer to @p ifThisPtr.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinDataStgIf_GetSubInterfaceIfFun_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                          Lin_eInterfaceIds_t interfaceId, 
                                                          LinDataStgIf_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of LIN Data Storage layer 'Append or Update Data Storage'
 *        interface function.
 *
 * @param genericDataStgEnvData[in] Pointer to reserved DATASTG Layer
 *                                  environment data.
 * @param dataStorageEntry[in]      Data storage entry pointer. (LinDataStgIf_cpDataStorageEntry_t)
 * @param dataStorageId[in]         Data storage identification id.
 *
 * @return Error code if something went wrong. LinDataStgIf_ERR_NO_ERROR if the
 *         everything was successful.
 *
 * Add the data storage entry defined by the Data Storage entry (@p dataStorageEntry) to the data storage pool.
 *
 ******************************************************************************/
typedef LinDataStgIf_eErrorCodes_t (*LinDataStgIf_AppendOrUpdateDataStorageEntry_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                                                    LinDataStgIf_cpDataStorageEntry_t pDataStorageEntry,
                                                                                    LinDataStgIf_DataStorageID_t dataStorageId);

/***************************************************************************//**
 * @brief Typedef of LIN Data Storage layer 'Get Data Value'
 *        interface function.
 *
 * @param genericDataStgEnvData[in] Pointer to reserved DATASTG Layer
 *                                  environment data.
 * @param dataValueType[in]         Data Value Type. (LinDataStgIf_DataValueType_t)
 * @param dataValueId[in]           Data Value ID. (LinDataStgIf_DataValueID_t)
 * @param buffer[out]               Pointer to buffer to copy the data to.
 * @param bufferLen[in]             Length of the provided buffer.
 * @param pDataValLen[in]           Pointer to variable to write length of the
 *                                  copied data to.
 *
 * @return Error code if something went wrong. LinDataStgIf_ERR_NO_ERROR if the
 *         everything was successful.
 *
 * Get the data defined by the Data Value ID (@p dataValueId).
 *
 * If parameter 'buffer' is LIN_NULL. Only the length of the 'Data Value' will be
 * copied to 'pDataValLen'.
 *
 ******************************************************************************/
typedef LinDataStgIf_eErrorCodes_t (*LinDataStgIf_GetDataValueIfFun_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                                       LinDataStgIf_DataValueType_t dataValueType,
                                                                       LinDataStgIf_DataValueID_t dataValueId, 
                                                                       LinDataStgIf_pData_t buffer,
                                                                       LinDataStgIf_Length_t bufferLen, 
                                                                       LinDataStgIF_pDataLen_t pDataValLen);

/***************************************************************************//**
 * Writes a given data value with given dataValueId and dataValueType to data storage.
 *
 * @param  genericDataStgEnvData      generic enviorment data
 * @param  AccessContextData          passphrase to unlock flash
 * @param  dataStorageId              ID of the data store
 * @param  dataValueType              data value type ID
 * @param  dataValueId                data value ID
 * @param  buffer                     buffer to store result
 * @param  bufferLen                  size of the buffer
 * @return error code
 ******************************************************************************/
typedef LinDataStgIf_eErrorCodes_t (*LinDataStgIf_SetDataValueIfFun_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                                       LinDataStgIf_cpAccessContextData_t AccessContextData,
                                                                       LinDataStgIf_DataStorageID_t dataStorageId,
                                                                       LinDataStgIf_DataValueType_t dataValueType,
                                                                       LinDataStgIf_DataValueID_t dataValueId, 
                                                                       LinDataStgIf_cpData_t buffer,
                                                                       LinDataStgIF_DataLen_t bufferLen);

/***************************************************************************//**
 * Writes a given data value with given dataValueId and dataValueType to data storage.
 *
 * @param  genericDataStgEnvData      generic enviorment data
 * @param  AccessContextData          passphrase to unlock flash
 * @param  dataStorageId              ID of the data store
 * @param  dataValueType              data value type ID
 * @param  dataValueId                data value ID
 * @return error code
 ******************************************************************************/
typedef LinDataStgIf_eErrorCodes_t (*LinDataStgIf_InvalidateDataValueIfFun_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                                              LinDataStgIf_cpAccessContextData_t AccessContextData,
                                                                              LinDataStgIf_DataStorageID_t dataStorageId,
                                                                              LinDataStgIf_DataValueType_t dataValueType,
                                                                              LinDataStgIf_DataValueID_t dataValueId);

/***************************************************************************//**
 * Prtotects a given data value with given dataValueId and dataValueType to data storage.
 *
 * @param  genericDataStgEnvData      generic enviorment data
 * @param  AccessContextData          passphrase to unlock flash
 * @param  dataStorageId              ID of the data store
 * @param  dataValueType              data value type ID
 * @param  dataValueId                data value ID
 * @return error code
 ******************************************************************************/
typedef LinDataStgIf_eErrorCodes_t (*LinDataStgImp_ProtectDataValueFun_t)(LinDataStgIf_pGenericEnvData_t genericDataStgEnvData,
                                                                          LinDataStgIf_cpAccessContextData_t AccessContextData,
                                                                          LinDataStgIf_DataStorageID_t dataStorageId,
                                                                          LinDataStgIf_DataValueType_t dataValueType,
                                                                          LinDataStgIf_DataValueID_t dataValueId);

/***************************************************************************//**
 * @brief DATASTG Layer interface functions struct.
 *
 * Collective struct for the LIN Data Storage layer interface functions.
 ******************************************************************************/
struct LinDataStgIf_sInterfaceFunctions
{
  Lin_Version_t                                 InterfaceVersion; /**< Interface version */

  LinDataStgIf_InitializationIfFun_t 			Initialization;   			/**< @brief Pointer to DATASTG Layer 'Initialization' interface function. (LinDataStgIf_InitializationIfFun_t) */
  LinDataStgIf_GetSubInterfaceIfFun_t 			GetSubInterface;  			/**< @brief Pointer to DATASTG Layer 'Get Sub-Interface' interface function. (LinDataStgIf_GetSubInterfaceIfFun_t) */
  LinDataStgIf_AppendOrUpdateDataStorageEntry_t	AppendOrUpdateDataStorage; 	/**< @brief Pointer to DATASTG Layer 'Update Data Storage' interface function. (LinDataStgIf_UpdateDataStorageEntry_t) */
  LinDataStgIf_GetDataValueIfFun_t    			GetDataValue;     			/**< @brief Pointer to DATASTG Layer 'Get Data Value' interface function. (LinDataStgIf_GetDataValueIfFun_t) */
  LinDataStgIf_SetDataValueIfFun_t    			SetDataValue;     			/**< @brief Pointer to DATASTG Layer 'Set Data Value' interface function. (LinDataStgIf_SetDataValueIfFun_t) */
  LinDataStgIf_InvalidateDataValueIfFun_t 		InvalidateDataValue; 		/**< @brief Pointer to DATASTG Layer 'Erase Data Value' interface function. */
  LinDataStgImp_ProtectDataValueFun_t 			ProtectDataValue; 			/**< @brief Pointer to DATASTG Layer 'Protecte Data Value' interface function */
};

/** @} LinDataStgIfIfFunDefs */

/* *****************************************************************************
 * LIN DataStg Layer Module callback function types
 ******************************************************************************/
/***************************************************************************//**
 * @brief  Object-like This-pointer to connect the DATASTG Layer to higher layers.
 ******************************************************************************/
/* PRQA S 3630 ++ #This is a struct definition that is API part of module
 LinDataStg. It can be used by used from application
 outside this rom project. It should not
 be hidden! */
struct LinDataStgIf_sThis
  {
    LinDataStgIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the DATASTG Layer interface function struct. (LinDataStgIf_sInterfaceFunctions) */
    LinDataStgIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved DATASTG Layer environment data. */
};
// PRQA S 3630 --
/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINDATASTG_INTERFACE_H_ */

/*! @} *//* DATASTG */
/*! @} *//* ELMOS_LIN_DRIVER */

