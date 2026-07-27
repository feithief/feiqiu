/***************************************************************************//**
 * @file			LinLookup_Implementation_Fixed.h
 *
 * @creator		sbai
 * @created		13.01.2015
 *
 * @brief  		LIN 'Fixed' LOOKUP Layer implementation.
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

#ifndef LINLOOKUP_IMPLEMENTATION_FIXED_H_
#define LINLOOKUP_IMPLEMENTATION_FIXED_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinLookup_Interface.h"
#include "LinProto_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINLOOKUPIMP_FIX_VERSION               0x0110u  /**< LIN LOOKUP FIXED implementation version */

#define LINLOOKUPIMP_FIX_CONFIG_DATA_VERSION   0x0100u  /**< Expected config data version. */

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINLOOKUPIMP_FIXED_EXT_IFFUN_STRCT_ACCESS == 1
extern const LinLookupIf_sInterfaceFunctions_t LinLookupImp_InterfaceFunctions_Fixed;
#endif /* LINLOOKUPIMP_EXT_IFFUN_STRCT_ACCESS == 1 */

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/** @addtogroup LinLookupImpInterfaceFunctions */
/** @{*/

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Initialization'
 *        function.
 *
 * @copydetails LinLookupIf_InitializationIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_Initialization_Fixed(LinLookupIf_pGenericEnvData_t    genericLookupEnvData, LinLookupIf_EnvDataSze_t lookupEnvDataSze,
                                             LinLookupIf_pGenericImpCfgData_t genericLookupImpCfgData);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Get Sub-Interface'
 *        function.
 *
 * @copydetails LinLookupIf_GetSubInterfaceIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_GetSubInterface_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Assign Frame ID'
 *        function.
 *
 * @copydetails LinLookupIf_AssignFrameIDIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AssignFrameID_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinProtoIf_MsgID_t msgID, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Assign Frame ID
 *        Range' function.
 *
 * @copydetails LinLookupIf_AssignFrameIDRangeIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AssignFrameIDRange_Fixed(LinLookupIf_pGenericEnvData_t         genericLookupEnvData, LinLookupIf_FrameIdx_t         frameIdx,
                                                 LinLookupIf_cpAssignFrameIDRangeLst_t frameIdLst,           LinLookupIf_FrameIdLstLength_t frameIdLstLen);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Get Frame
 *        Description' function.
 *
 * @copydetails LinLookupIf_GetFrameDescriptionIfFun_t
 *
 ******************************************************************************/
LinProtoIf_cpFrameDescription_t LinLookupImp_GetFrameDescription_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinBusIf_FrameID_t frameID);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Get Frame ID
 *        Assignment' function.
 *
 * @copydetails LinLookupIf_GetFrameIdAssignmentIfFun_t
 *
 ******************************************************************************/
LinBusIf_cpFrameID_t LinLookupImp_GetFrameIdAssignment_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Add Frame
 *        Description List' function.
 *
 * @copydetails LinLookupIf_AddFrameDescriptionListIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_AddFrameDescLst_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t     genericFrmDscLst,
                                              Lin_Bool_t                    ldfRelevance,         LinLookupIf_pGenericFrmDescLstEnvData_t genericFrmDescLstEnvData,
                                              LinLookupIf_EnvDataSze_t      frmDescLstEnvDataSze, LinLookupIf_pGenericCbCtxData_t         perFrmDescLstCbCtxData);

/***************************************************************************//**
 * @brief 'Fixed Lookup'Implementation of LIN LOOKUP layer 'Remove Frame
 *        Description List function.
 *
 * @copydetails LinLookupIf_RemoveFrameDescriptionListIfFun_t
 *
 ******************************************************************************/
Lin_Bool_t LinLookupImp_RmvFrameDescLst_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/***************************************************************************//**
 * @brief 'Fixed Lookup' Implementation of LIN LOOKUP layer 'Get per Frame
 *        Description List Context Data' function.
 *
 * @copydetails LinLookupIf_GetPerFrameDescLstCbCtxDataIfFun_t
 *
 ******************************************************************************/
LinLookupIf_pGenericCbCtxData_t LinLookupImp_GetPerFrameDescLstCbCtxData_Fixed(LinLookupIf_pGenericEnvData_t genericLookupEnvData, LinLookupIf_cpGenericFrameDescLst_t genericFrmDscLst);

/** @} LinLookupImpInterfaceFunctions */

#endif /* LINLOOKUP_IMPLEMENTATION_FIXED_H_ */

/*! @} */ /* LOOKUP */
/*! @} */ /* ELMOS_LIN_DRIVER */
