/***************************************************************************//**
 * @file		LinLookup_Interface.h
 *
 * @creator		sbai
 * @created		15.01.2015
 *
 * @brief The LOOKUP Layer implements the organization and mapping between
 *        PIDs and Frame Descriptions. It is possible to implement different
 *        variants of this module, to save RAM or code size., like a sequential
 *        or tree lookup. Currently only a direct lookup is supported.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LOOKUP LOOKUP Layer
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

#ifndef LINLOOKUP_INTERFACE_H_
#define LINLOOKUP_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinBus_Interface.h"
#include "LinProto_FrameDescription.h"
#include "LinLookup_Types.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

#define LINLOOKUP_INTERFACE_MODULE_API_VERSION          0x0102u /**< LIN LOOKUP interface version */

#define LINLOOKUP_INTERFACE_MAX_FRAME_DESCIPTION_LISTS  4u      /**< Maximal supported 'Frame Description Lists'. @see LinProtoIf_sFrameDescription_t **/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* *****************************************************************************
 * Interface callback function types
 ******************************************************************************/

/** @addtogroup LinLookupIfIfFunDefs */
/** @{*/


/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Initialization' interface function.
 *
 * @param[in] genericLookupEnvData    Pointer to reserved LOOKUP Layer environment data.
 * @param[in] lookupEnvDataSze        Size of the reserved RAM for LOOKUP environment
 *                                    data.
 * @param[in] genericLookupImpCfgData Pointer to implementation dependent
 *                                    configuration data for the LOOKUP Layer
 *                                    (LinLookuImp_sCfgData).
 *
 * @return LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the LOOKUP Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_InitializationIfFun_t) (LinLookupIf_pGenericEnvData_t    genericLookupEnvData, LinLookupIf_EnvDataSze_t lookupEnvDataSze,
                                                         LinLookupIf_pGenericImpCfgData_t genericLookupImpCfgData);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Get Sub Interface' interface function.
 *
 * @param[in]  genericLookupEnvData Pointer to reserved LOOKUP Layer environment
 *                                  data.
 * @param[in]  interfaceId          Sub-Interface ID.
 * @param[out] ifThisPtr            Address (pointer) of an instance of the
 *                                  LIN-This-Pointer struct (@ref Lin_sThis).
 *
 * @return  LIN_TRUE if the desired interface is available and the parameter
 *          @p ifThisPtr could be set.
 *
 * Acquires the desired sub-interface (@p interfaceId) and writes the address
 * of its This-Pointer to @p ifThisPtr.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_GetSubInterfaceIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Assign Frame ID' interface function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] msgID                 'Message ID' which shall be assigned.
 * @param[in] frameID               'Frame ID' the message will be assigned to.
 *
 * Assigns a new 'Frame ID' to a 'Frame Description' with the desired
 * 'Message ID'. This function is necessary to support the 'Assign Frame ID'
 * service described in LIN Specification 2.0.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_AssignFrameIDIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinProtoIf_MsgID_t msgID, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Assign Frame ID Range' interface function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] frameIdx              'Frame Index' to start from.
 * @param[in] frameIdLst            List with 'Frame IDs' to assign.
 * @param[in] frameIdLstLen         Length of the 'Frame ID' list.
 *
 * @return LIN_TRUE if the 'Frame IDs' have been successfully assigned.
 *
 * Assigns a new 'Frame IDs' from the given list (frameIdLst) starting at the
 * passed list length (frameIdx) until  the end of the list is reached
 * (frameIdLstLen). This function is necessary to support the
 * 'Assign Frame ID Range' service from the LIN 2.2 specification
 * (Chapter 4.2.5.5).
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_AssignFrameIDRangeIfFun_t) (LinLookupIf_pGenericEnvData_t         genericLookupEnvData, LinLookupIf_FrameIdx_t         frameIdx,
                                                             LinLookupIf_cpAssignFrameIDRangeLst_t frameIdLst,           LinLookupIf_FrameIdLstLength_t frameIdLstLen);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Get Frame Description' interface function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] frameID               'Frame ID' of desired 'Frame Description'.
 *
 * @return Pointer to the desired 'Frame Description'.
 *
 * Acquire the 'Frame Description' to which the desired 'Frame ID' is assigned.
 *
 ******************************************************************************/
typedef LinProtoIf_cpFrameDescription_t (*LinLookupIf_GetFrameDescriptionIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Get Frame ID Assignment' interface function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to the 'Frame Description List' of
 *                                  which the assignments should be returned.
 *
 * @return A pointer to a list of 'Frame IDs'. Can return LIN_NULL.
 *
 * This interface function returns a list of 'Frame IDs' to which the
 * 'Frame Descriptions' of the given 'Frame Description List' are assigned to.
 *
 ******************************************************************************/
typedef LinBusIf_cpFrameID_t (*LinLookupIf_GetFrameIdAssignmentIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Add Frame Description List' interface function.
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
 * Adds a new list of 'Frame Descriptions' to the LOOKUP Layer, which
 * the other layers can access.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_AddFrameDescriptionListIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t     genericFrmDscLst,
                                                                  Lin_Bool_t                    ldfRelevance,         LinLookupIf_pGenericFrmDescLstEnvData_t genericFrmDescLstEnvData,
                                                                  LinLookupIf_EnvDataSze_t      frmDescLstEnvDataSze, LinLookupIf_pGenericCbCtxData_t         perFrmDescLstCbCtxData);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer 'Remove Frame Description List' interface function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to 'Frame Description List' to remove.
 *
 * @return TRUE, if the list has been successfully removed.
 *
 * Removes a list of 'Frame Descriptions' from the LOOKUP Layer.
 *
 ******************************************************************************/
typedef Lin_Bool_t (*LinLookupIf_RemoveFrameDescriptionListIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * @brief Typedef of LOOKUP Layer interface 'Get per Frame Description List
 *        Context Data' function.
 *
 * @param[in] genericLookupEnvData  Pointer to reserved LOOKUP Layer environment data.
 * @param[in] genericFrmDscLst      Pointer to 'Frame Description List' of which
 *                                  the callback context data should be acquired.
 *
 * @return Pointer to the 'Frame Description List' callback context data.
 *
 * Get the pointer to 'Callback Context Data' of a 'Frame Description List',
 * added at the LinLookupIf_AddFrameDescriptionListIfFun_t interface function.
 *
 ******************************************************************************/
typedef  LinLookupIf_pGenericCbCtxData_t (*LinLookupIf_GetPerFrameDescLstCbCtxDataIfFun_t) (LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);


/***************************************************************************//**
 * @brief LIN LOOKUP Layer interface functions struct.
 *
 * Collective struct for the LOOKUP Layer interface functions.
 *
 ******************************************************************************/
struct LinLookupIf_sInterfaceFunctions
{
    Lin_Version_t                                  InterfaceVersion;           /**< @brief LOOKUP Layer interface version.*/

    LinLookupIf_InitializationIfFun_t              Initialization;             /**< @brief Pointer to LOOKUP Layer 'Initialization' interface function. (@ref LinLookupIf_InitializationIfFun_t) @copydetails LinLookupIf_InitializationIfFun_t */
    LinLookupIf_GetSubInterfaceIfFun_t             GetSubInterface;            /**< @brief Pointer to LOOKUP Layer 'Get Sub-Interface' interface function. (@ref LinLookupIf_GetSubInterfaceIfFun_t) @copydetails LinLookupIf_GetSubInterfaceIfFun_t */
    LinLookupIf_AssignFrameIDIfFun_t               AssignFrameID;              /**< @brief Pointer to LOOKUP Layer 'Assign Frame ID' interface function. (@ref LinLookupIf_AssignFrameIDIfFun_t) @copydetails LinLookupIf_AssignFrameIDIfFun_t */
    LinLookupIf_AssignFrameIDRangeIfFun_t          AssignFrameIDRange;         /**< @brief Pointer to LOOKUP Layer 'Assign Frame ID Range' interface function. (@ref LinLookupIf_AssignFrameIDRangeIfFun_t) @copydetails LinLookupIf_AssignFrameIDRangeIfFun_t */
    LinLookupIf_GetFrameDescriptionIfFun_t         GetFrameDescription;        /**< @brief Pointer to LOOKUP Layer 'Get Frame Description' interface function. (@ref LinLookupIf_GetFrameDescriptionIfFun_t) @copydetails LinLookupIf_GetFrameDescriptionIfFun_t */
    LinLookupIf_GetFrameIdAssignmentIfFun_t        GetFrameIdAssignment;       /**< @brief Pointer to LOOKUP Layer 'Get Frame ID Assignment' interface function. (@ref LinLookupIf_GetFrameIdAssignmentIfFun_t) @copydetails LinLookupIf_GetFrameIdAssignmentIfFun_t */
    LinLookupIf_AddFrameDescriptionListIfFun_t     AddFrameDescriptionList;    /**< @brief Pointer to LOOKUP Layer 'Add Frame Description List' interface function. (@ref LinLookupIf_AddFrameDescriptionListIfFun_t) @copydetails LinLookupIf_AddFrameDescriptionListIfFun_t */
    LinLookupIf_RemoveFrameDescriptionListIfFun_t  RemoveFrameDescriptionList; /**< @brief Pointer to LOOKUP Layer 'Remove Frame Description List' interface function. (@ref LinLookupIf_RemoveFrameDescriptionListIfFun_t) @copydetails LinLookupIf_RemoveFrameDescriptionListIfFun_t */
    LinLookupIf_GetPerFrameDescLstCbCtxDataIfFun_t GetPerFrmDescLstCbCtxData;  /**< @brief Pointer to LOOKUP Layer 'Get per Frame Description List Callback Context Data' interface function. (@ref LinLookupIf_GetPerFrameDescLstCbCtxDataIfFun_t) @copydetails LinLookupIf_GetPerFrameDescLstCbCtxDataIfFun_t */
};
/** @} LinLookupIfIfFunDefs */

/***************************************************************************//**
 * @brief  Object-like This-pointer to connect the LOOKUP Layer to higher layers
 *         or application.
 ******************************************************************************/
struct LinLookupIf_sThis
{
    LinLookupIf_cpInterfaceFunctions_t IfFunsTbl; /**< @brief Pointer to the LOOKUP Layer interface function struct. (LinLookupIf_sInterfaceFunctions) */
    LinLookupIf_pGenericEnvData_t      envData;   /**< @brief Pointer to reserved LOOKUP Layer environment data. */
};

/***************************************************************************//**
 * @brief LOOKUP Layer interface configuration parameter.
 *
 * Data needed for initialization of the LOOKUP Layer.
 ******************************************************************************/
struct LinLookupIf_sInitParam
{
    LinLookupIf_cpInterfaceFunctions_t IfFunsTbl;   /**< @brief Pointer to the constant LOOKUP Layer interface function struct. */
    LinLookupIf_pGenericEnvData_t      envData;     /**< @brief Pointer to reserved LOOKUP Layer environment data. */
    LinLookupIf_EnvDataSze_t           EnvDataLen;  /**< @brief Size of the reserved RAM for LOOKUP Layer environment data. */
    LinLookupIf_pGenericImpCfgData_t   ImpCfgData;  /**< @brief Pointer to implementation dependent configuration data for the LOOKUP Layer. (LinLookupImp_sCfgData) */
};

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINLOOKUP_INTERFACE_H_ */

/*! @} */ /* LOOKUP */
/*! @} */ /* ELMOS_LIN_DRIVER */
