/***************************************************************************//**
 * @file			LinLookup_Types.h
 *
 * @creator		sbai
 * @created		25.03.2015
 *
 * @brief  		Definitions of basic data types for the 'LIN LOOKUP Layer'.
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

#ifndef LINLOOKUP_TYPES_H_
#define LINLOOKUP_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

typedef Lin_pvoid_t      LinLookupIf_pGenericEnvData_t;           /**< Generic pointer to environment data of the LOOKUP layer module. */
typedef Lin_EnvDataSze_t LinLookupIf_EnvDataSze_t;                /**< LIN Lookup layer data type for the environment data length. */
typedef Lin_pvoid_t      LinLookupIf_pGenericFrameDescLst_t;      /**< Generic pointer to a 'Frame Description List'. **/
typedef Lin_cpvoid_t     LinLookupIf_cpGenericFrameDescLst_t;     /**< Generic constant pointer to a 'Frame Description List'. **/
typedef Lin_pvoid_t      LinLookupIf_pGenericImpCfgData_t;        /**< Generic pointer to configuration parameter of the specific LOOKUP layer implementation. */
typedef Lin_pvoid_t      LinLookupIf_pGenericCbCtxData_t;         /**< Pointer to LOOKUP callback context data. */
typedef Lin_pvoid_t      LinLookupIf_pGenericFrmDescLstEnvData_t; /**< Generic pointer to environment data per 'Frame Description List' of the LOOKUP layer module. */

typedef Lin_uint8_t          LinLookupIf_FrameIdx_t;                /**< LIN Frame Index @see LIN 2.2 Specification - Chapter 4.2.5.5 Assign frame ID range **/
typedef LinBusIf_pFrameID_t  LinLookupIf_pAssignFrameIDRangeLst_t;  /**< Pointer to a list of assigned 'Frame IDs'. @see LIN 2.2 Specification - Chapter 4.2.5.5 Assign frame ID range **/
typedef LinBusIf_cpFrameID_t LinLookupIf_cpAssignFrameIDRangeLst_t; /**< Pointer to a constant list of assigned 'Frame IDs'. @see LIN 2.2 Specification - Chapter 4.2.5.5 Assign frame ID range **/

typedef Lin_uint8_t LinLookupIf_FrameIdLstLength_t;   /**< Typedef for the length of a frame ID list.  */

typedef Lin_uint8_t* LinLookupIf_pAssignmentBuffer_t; /**< Pointer to an 'Assignment Buffer'. **/

struct         LinLookupIf_sInterfaceFunctions;                                        /* Forward declaration of LIN LOOKUP layer interface functions. */
typedef struct LinLookupIf_sInterfaceFunctions     LinLookupIf_sInterfaceFunctions_t;  /**< Typedef for LinLookupIf_sInterfaceFunctions. */
typedef        LinLookupIf_sInterfaceFunctions_t*  LinLookupIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinLookupIf_sInterfaceFunctions. */
typedef const  LinLookupIf_sInterfaceFunctions_t*  LinLookupIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinLookupIf_sInterfaceFunctions. */

struct         LinLookupIf_sThis;                        /* Forward declaration of LIN BUS layer This-Pointer. */
typedef struct LinLookupIf_sThis    LinLookupIf_sThis_t; /**< Typedef for LinLookupIf_sThis. */
typedef        LinLookupIf_sThis_t* LinLookupIf_pThis_t; /**< Typedef of pointer to LinLookupIf_sThis. */

struct         LinLookupIf_sInitParam;                             /* Forward declaration of LIN LOOKUP layer initialization parameter struct. */
typedef struct LinLookupIf_sInitParam    LinLookupIf_sInitParam_t; /**< Typedef for LinLookupIf_sInitParam */
typedef        LinLookupIf_sInitParam_t* LinLookupIf_pInitParam_t; /**< Typedef of pointer to LinLookupIf_sInitParam */

/***************************************************************************//**
 * @brief LIN LOOKUP assignments command types.
 ******************************************************************************/
enum LinLookupIf_eAssignFrameIDRangeCmds
{
  LinLookupIf_AssignFrameIDRangeCmd_UNASSIGN   = 0xFE, /**< Unassign the 'Frame Description' **/
  LinLookupIf_AssignFrameIDRangeCmd_DONOTCARE  = 0xFF  /**< "Do not care" about this 'Frame Description' **/
};

typedef enum LinLookupIf_eAssignFrameIDRangeCmds LinLookupIf_eAssignFrameIDRangeCmds_t; /**< Typedef of LinLookupIf_eAssignFrameIDRangeCmds */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINLOOKUP_TYPES_H_ */

/*! @} */ /* LOOKUP */
/*! @} */ /* ELMOS_LIN_DRIVER */
