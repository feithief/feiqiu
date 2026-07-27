/***************************************************************************//**
 * @file			LinLookup_Implementation_Sequential.h
 *
 * @creator		sbai
 * @created		13.01.2015
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief       Implementation of LIN Lookup Layer Sequential
 *
 * @purpose     Implements functionality of diagnostic services 'Assign Frame ID',
 *              'Assign Frame ID Range' and handles the Frame Description List.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LOOKUP LOOKUP Layer
 * @{
 *
 * $Id: $
 *
 * $Revision: $
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

#ifndef LINLOOKUP_IMPLEMENTATION_Sequential_H_
#define LINLOOKUP_IMPLEMENTATION_Sequential_H_

/* PRQA S 1534 EOF #justification: This header file is a common file for many projects.
   So it is ok that some types and macros are defined that are not used in this project scope. */
/* Rule: 1534 MISRA a Rule-2.5
   Circumstances: The macro '%1s' is declared but not used within this project.
   Potential consequences: none because optimizer and compiler ignore unused macros
   Alternative safety assurance: none */

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinLookup_Interface.h"
#include "LinProto_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINLOOKUPIMP_SEQ_VERSION               0x0110u  /**< LIN LOOKUP SEQ implementation version. */

#define LINLOOKUPIMP_SEQ_CONFIG_DATA_VERSION   0x0100u  /**< Expected config data version. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINLOOKUPIMP_SEQUENTIAL_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinLookupIf_sInterfaceFunctions_t LinLookupImp_InterfaceFunctions_Sequential;
#endif /* LINLOOKUPIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/** @addtogroup LinLookupImpInterfaceFunctions */
/**@{*/

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Initialization'
 *        function.
 *
 * @param[in] genericLookupEnvData    Pointer to reserved LOOKUP Layer environment data.
 * @param[in] lookupEnvDataSze        Size of the reserved RAM for LOOKUP environment
 *                                    data.
 * @param[in] genericLookupImpCfgData Pointer to implementation dependent
 *                                    configuration data for the LOOKUP Layer.
 *                                    LinLookuImp_sCfgData
 *
 * @return LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * @details Initializes the LOOKUP Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_Initialization_Sequential(LinLookupIf_pGenericEnvData_t    genericLookupEnvData, LinLookupIf_EnvDataSze_t lookupEnvDataSze,
                                                  LinLookupIf_pGenericImpCfgData_t genericLookupImpCfgData);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Get Sub-Interface'
 *  function.
 *
 * @param[in]  genericLookupEnvData Pointer to reserved LOOKUP Layer environment
 *                                  data.
 * @param[in]  interfaceId          Sub-Interface ID.
 * @param[out] ifThisPtr            Address (pointer) of an instance of the
 *                                  LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          ifThisPtr could be set.
 *
 * @details Acquires the desired sub-interface (interfaceId) and writes the address
 *          of its This-Pointer to ifThisPtr.
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_GetSubInterface_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Assign Frame ID'
 *  function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] msgID                 'Message ID' which shall be assigned.
 * @param[in] frameID               'Frame ID' the message will be assigned to.
 *
 * @details Assigns a new 'Frame ID' to a 'Frame Description' with the desired
 *          'Message ID'. This function is necessary to support the 'Assign Frame ID'
 *          service described in LIN Specification 2.0.
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AssignFrameID_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinProtoIf_MsgID_t msgID, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of
 * LIN LOOKUP layer 'Assign Frame ID Sequential'.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] frameIdx              'Frame Index' to start from.
 * @param[in] frameIdLst            List with 'Frame IDs' to assign.
 * @param[in] frameIdLstLen         Length of the 'Frame ID' list.
 *
 * @return LIN_TRUE if the 'Frame IDs' have been successfully assigned.
 *
 * @details Assigns a new 'Frame IDs' from the given list (frameIdLst) starting at the
 *          passed list length (frameIdx) until  the end of the list is reached
 *          (frameIdLstLen). This function is necessary to support the
 *          'Assign Frame ID Range' service from the LIN 2.2 specification
 *          (Chapter 4.2.5.5).
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AssignFrameIDRange_Sequential(LinLookupIf_pGenericEnvData_t         genericLookupEnvData, LinLookupIf_FrameIdx_t         frameIdx,
                                                      LinLookupIf_cpAssignFrameIDRangeLst_t frameIdLst,           LinLookupIf_FrameIdLstLength_t frameIdLstLen);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Get Frame
 * Description' function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] frameID               'Frame ID' of desired 'Frame Description'.
 *
 * @return Pointer to the desired 'Frame Description'.
 *
 * @details Acquire the 'Frame Description' to which the desired 'Frame ID' is assigned.
 *
 ******************************************************************************/
LinProtoIf_cpFrameDescription_t LinLookupImp_GetFrameDescription_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer
 * 'Get Frame ID assignment' function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to the 'Frame Description List' of
 *                                  which the assignments should be returned.
 *
 * @return A pointer to a list of 'Frame IDs'. Can return LIN_NULL.
 *
 * @details This interface function returns a list of 'Frame IDs' to which the
 *          'Frame Descriptions' of the given 'Frame Description List' are assigned to.
 *
 ******************************************************************************/
LinBusIf_cpFrameID_t LinLookupImp_GetFrameIdAssignment_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Add Frame
 *  Description List' function.
 *
 * @param[in] genericLookupEnvData      Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst          Pointer to 'Frame Description List' to add.
 * @param[in] ldfRelevance              TRUE if the 'Frame IDs' in this list can be
 *                                      assigned/unassigned.
 * @param[in] genericFrmDescLstEnvData  Environment data per 'Frame Description List'.
 * @param[in] frmDescLstEnvDataSze      Size of reserved environment data.
 * @param[in] perFrmDescLstCbCtxData    Pointer to 'Frame Description List' context data.
 *
 * @return TRUE, if the list has been successfully added.
 *
 * @details Adds a new list of 'Frame Descriptions' to the LOOKUP Layer, which
 *          the other layers can access.
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AddFrameDescLst_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t     genericFrmDscLst,
                                                   Lin_Bool_t                    ldfRelevance,         LinLookupIf_pGenericFrmDescLstEnvData_t genericFrmDescLstEnvData,
                                                   LinLookupIf_EnvDataSze_t      frmDescLstEnvDataSze, LinLookupIf_pGenericCbCtxData_t         perFrmDescLstCbCtxData);

/***************************************************************************//**
 * 'Sequential Lookup'Implementation of LIN LOOKUP layer 'Remove Frame
 * Description List function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to 'Frame Description List' to remove.
 *
 * @return TRUE, if the list has been successfully removed,
 *         FALSE if not.
 *
 * @details Removes a list of 'Frame Descriptions' from the LOOKUP Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_RmvFrameDescLst_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * 'Sequential Lookup' Implementation of LIN LOOKUP layer 'Get per Frame
 *  Description List Context Data' function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to 'Frame Description List' of which
 *                                  the callback context data should be acquired.
 *
 * @return Pointer to the 'Frame Description List' callback context data.
 *
 * @details Get the pointer to 'Callback Context Data' of a 'Frame Description List',
 *          added at the LinLookupIf_AddFrameDescriptionListIfFun_t interface function.
 *
 ******************************************************************************/
LinLookupIf_pGenericCbCtxData_t LinLookupImp_GetPerFrameDescLstCbCtxData_Sequential(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/**@} LinLookupImpInterfaceFunctions */

#endif /* LINLOOKUP_IMPLEMENTATION_Sequential_H_ */

/*! @} */ /* LOOKUP */
/*! @} */ /* ELMOS_LIN_DRIVER */
