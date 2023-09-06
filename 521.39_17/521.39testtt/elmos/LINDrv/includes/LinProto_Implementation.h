/***************************************************************************//**
 * @file			LinProto_Implementation.h
 *
 * @creator		sbai
 * @created		13.01.2015
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief     Contains implementation of the LIN Protocol Layer
 *
 * @purpose   -Evaluates and verifies incoming Frame ID
 *            -Gets Frame Description to corresponding Frame ID
 *            -Implements functions to handle and edit Frame Description List.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup PROTO PROTO Layer
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
 * -----------------------------------------------------------------------------
 * Date      Version    Who   Comment
 * YY/MM/DD  Ma.Mi.Bd
 * -----------------------------------------------------------------------------
 * 21/07/29  1.0.x      POE   ---
 * ----------------------------------------------------------------------------- 

 ******************************************************************************/

#ifndef LINPROTO_IMPLEMENTATION_H_
#define LINPROTO_IMPLEMENTATION_H_

/* PRQA S 1534 EOF #justification: This header file is a common file for many projects.
   So it is ok that some macros are defined that are not used in this project scope. */
/* Rule: 1534 MISRA a Rule-2.5
   Circumstances: The macro '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinLookup_Interface.h"
#include "LinBus_Interface.h"
#include "LinProto_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINPROTOIMP_VERSION             0x0110u /**< LIN PROTO implementation version. */

#define LINPROTOIMP_CONFIG_DATA_VERSION 0x0100u /**< Expected config data version. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Struct for PROTO layer implementation specific configuration data.
 ******************************************************************************/
struct LinProtoImp_sCfgData
{
    Lin_Version_t            Version;              /**< Config data struct version */

    LinBusIf_sInitParam_t    LinBusIfInitParam;    /**< @copydoc LinBusIf_sInitParam_t */
    LinLookupIf_sInitParam_t LinLookupIfInitParam; /**< @copydoc LinLookupIf_sInitParam_t */
};

/***************************************************************************//**
 * Typedef of LinProtoImp_sCfgData
 ******************************************************************************/
typedef struct LinProtoImp_sCfgData    LinProtoImp_sCfgData_t; /**< Typedef of LinProtoImp_sCfgData. */
/***************************************************************************//**
 * Typedef of pointer to LinProtoImp_sCfgData
 ******************************************************************************/
typedef        LinProtoImp_sCfgData_t* LinProtoImp_pCfgData_t; /**< Typedef of pointer to LinProtoImp_sCfgData. */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINPROTOIMP_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinProtoIf_sInterfaceFunctions_t LinProtoImp_InterfaceFunctions;
#endif /* LINPROTOIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/** @addtogroup LinProtoImpInterfaceFunctions */
/** @{*/

/***************************************************************************//**
 * LIN PROTO layer 'Initialization' function.
 *
 * @param[in] genericProtoEnvData     Pointer to reserved PROTO Layer environment data.
 * @param[in] protoEnvDataSze         Size of the reserved RAM for PROTO environment
 *                                    data.
 * @param[in] protoCbFuns             Pointer to PROTO Layer callback function.
 *                                    struct. Implemented in the higher layer or
 *                                    user application.
 * @param[in] genericProtoCbCtxData   Pointer to PROTO Layer callback context
 *                                    data.
 * @param[in] genericProtoImpCfgData  Pointer to implementation dependent
 *                                    configuration data for the PROTO Layer
 *                                    LinProtoImp_sCfgData.
 *
 * @return LIN_TRUE if the initialization was successfully,
 *         LIN_FALSE if not.
 *
 * @details Initializes the LIN PROTO Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_Initialization( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                       LinLookupIf_EnvDataSze_t protoEnvDataSze,
                                       LinProtoIf_cpCallbackFunctions_t protoCbFuns,
									   LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData,
									   LinProtoIf_pGenericImpCfgData_t genericProtoImpCfgData );

/***************************************************************************//**
 * LIN PROTO layer 'Get Sub-Interface' function.
 *
 * @param[in]  genericProtoEnvData  Pointer to reserved PROTO Layer environment data.
 * @param[in]  interfaceId          Sub-Interface ID.
 * @param[out] ifThisPtr            Address (pointer) of an instance of the
 *                                  LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          ifThisPtr could be set.
 *
 * @details Acquires the desired sub-interface (interfaceId) and
 *          writes the address of its This-Pointer to ifThisPtr.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_GetSubInterface( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                        Lin_eInterfaceIds_t interfaceId, 
										Lin_pThis_t ifThisPtr );

/***************************************************************************//**
 * LIN PROTO layer 'Task' function.
 *
 * @param[in] genericProtoEnvData Pointer to reserved PROTO Layer environment data.
 *
 * @details The 'Task' function has to be called periodically to process scheduled
 *          task of the PROTO Layer.
 *
 ******************************************************************************/
void LinProtoImp_Task(LinProtoIf_pGenericEnvData_t genericProtoEnvData);

/***************************************************************************//**
 * LIN PROTO layer 'Append Callbacks' function.
 *
 * @param[in] genericProtoEnvData   Pointer to reserved PROTO Layer environment data.
 * @param[in] protoCbFuns           Pointer to PROTO Layer callback function.
 *                                  struct, which shall be added.
 * @param[in] genericProtoCbCtxData Pointer to PROTO Layer callback context
 *                                  data.
 *
 * @return -LIN_TRUE, if the callbacks have been successfully added to the
 *          PROTO Layer.
 *         -LIN_FALSE if Protocol Layer callback version isn't equal to
 *          Protocol Layer Interface module API version
           -LIN_FALSE if pointer to Protocol Layer Callback function
            is a NULL pointer
           -Pointer to Protocol Environment data is a NULL pointer
           -Pointer to callback in callback entry table is a
            NULL pointer.
 *
 * @details The PROTO Layer supports more then one callback struct, to allow more
 *          then one layer to use it. This function adds a additional callback
 *          function struct (LinProtoIf_sCallbackFunctions) to the call queue.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_AppendCallbacks( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                        LinProtoIf_cpCallbackFunctions_t protoCbFuns,
                                        LinProtoIf_pGenericCbCtxData_t genericProtoCbCtxData);

/***************************************************************************//**
 * PROTO Layer 'Remove Callbacks' interface function.
 *
 * @param[in] genericProtoEnvData Pointer to reserved PROTO Layer environment data.
 * @param[in] protoCbFuns         Pointer to PROTO Layer callback function.
 *                                struct, which shall be removed.
 *
 * @return LIN_TRUE, if the callbacks have been successfully removed from the
 *         PROTO Layer.
 *         LIN_FALSE if Pointer to Protocol Layer environment data is a
 *         NULL pointer or callback entry was not found.
 *
 * @details Remove callbacks added via LinProtoIf_AppendCallbacksIfFun_t.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_RemoveCallbacks(LinProtoIf_pGenericEnvData_t genericProtoEnvData, LinProtoIf_cpCallbackFunctions_t protoCbFuns);

/***************************************************************************//**
 * Implementation of LIN PROTO layer 'Add Frame Description List' function.
 *
 * @param[in] genericProtoEnvData             Pointer to reserved PROTO Layer
 *                                            environment data.
 * @param[in] genericFrmDscLst                Pointer to 'Frame Description List'
 *                                            to add.
 * @param[in] ldfRelevance                    TRUE if the 'Frame IDs' in this list
 *                                            can be assigned/unassigned.
 * @param[in] genericFrmDescLstEnvData        Environment data per 'Frame
 *                                            Description List'.
 * @param[in] frmDescLstEnvDataSze            Size of reserved environment data
 *                                            per 'Frame Description List'.
 * @param[in] genericProtoPerDescLstCbCtxData Callback context data connected to
 *                                            this list.
 *                                            See also: LinProtoIf_eFrameIdProcCbCtxDataType
 *
 * @return TRUE, if the list has been successfully added.
 *
 * @details Adds a 'Frame Description List' with 'Frame Descriptions' to which the
 *          PROTO Layer reacts on.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_AddFrameDescriptionList( LinProtoIf_pGenericEnvData_t genericProtoEnvData,  
                                                LinProtoIf_cpGenericFrameDescriptionList_t genericFrmDscLst,
                                                Lin_Bool_t ldfRelevance,
											    LinProtoIf_pGenericFrmDescLstEnvData_t genericFrmDescLstEnvData,
                                                LinProtoIf_EnvDataSze_t frmDescLstEnvDataSze,
											    LinProtoIf_pGenericCbCtxData_t genericProtoPerDescLstCbCtxData );

/***************************************************************************//**
 * Implementation of LIN PROTO layer 'Remove Frame Description List' function.
 *
 * @param[in] genericProtoEnvData Pointer to reserved PROTO Layer environment data.
 * @param[in] genericFrmDscLst    Pointer to 'Frame Description List' to remove.
 *
 * @return TRUE, if the list has been successfully removed.
 *         FALSE if pointer to Protocol Layer Environment data
 *         is a NULL pointer or list couldn't removed.
 *
 * @detais Removes a list of 'Frame Descriptions' from the PROTO Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinProtoImp_RemoveFrameDescriptionList( LinProtoIf_pGenericEnvData_t genericProtoEnvData, 
                                                   LinProtoIf_cpGenericFrameDescriptionList_t genericFrmDscLst );

/** @} LinProtoImpInterfaceFunctions */

#endif /* LINPROTO_IMPLEMENTATION_H_ */

/*! @} */ /* PROTO */
/*! @} */ /* ELMOS_LIN_DRIVER */
