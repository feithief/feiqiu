/***************************************************************************//**
 * @file			LinSNPD_Types.h
 *
 * @creator		rpy
 * @created		9.9.2015
 *
 * @brief  		LIN SNPD layer (Autoadressing) data types.
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

#ifndef LINSNPD_TYPES_H_
#define LINSNPD_TYPES_H_

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
typedef Lin_pvoid_t      LinSNPDIf_pGenericEnvData_t;           /**< SNPD environment data type **/
typedef Lin_EnvDataSze_t LinSNPDIf_EnvDataSze_t;                /**< LIN SNPD layer data type for the environment data length. */
typedef Lin_pvoid_t      LinSNPDIf_pGenericImpCfgData_t;        /**< Generic pointer to configuration parameter of the specific SNPD implementation */

typedef Lin_uint16_t LinSNPDIf_MeasCount_t; /**< SNPD measurement counter */

struct         LinSNPDIf_sInterfaceFunctions;                                      /**< Forward declaration of LIN SNPD layer interface functions. */
typedef struct LinSNPDIf_sInterfaceFunctions     LinSNPDIf_sInterfaceFunctions_t;  /**< Typedef of LinSNPDIf_sInterfaceFunctions. */
typedef        LinSNPDIf_sInterfaceFunctions_t*  LinSNPDIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinSNPDIf_sInterfaceFunctions. */
typedef const  LinSNPDIf_sInterfaceFunctions_t*  LinSNPDIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinSNPDIf_sInterfaceFunctions. */

struct         LinSNPDIf_sThis;                      /**< Forward declaration of LIN SNPD layer This-Pointer. */
typedef struct LinSNPDIf_sThis    LinSNPDIf_sThis_t; /**< Typedef for LinSNPDIf_sThis. */
typedef        LinSNPDIf_sThis_t* LinSNPDIf_pThis_t; /**< Typedef of pointer to LinSNPDIf_sThis. */

struct         LinSNPDIf_sInitParam;                           /**< Forward declaration of LIN SNPD layer initialization parameter struct. */
typedef struct LinSNPDIf_sInitParam    LinSNPDIf_sInitParam_t; /**< Typedef of LinSNPDIf_sInitParam */
typedef        LinSNPDIf_sInitParam_t* LinSNPDIf_pInitParam_t; /**< Typedef of pointer to LinSNPDIf_sInitParam */

/***************************************************************************//**
 * Enumerator for SNPD states.
 ******************************************************************************/
enum LinSNPDIf_eState
{
  LinSNPDIf_DISABLED          = 0,  /**<  SNPD state machine is disable call LinBusImp_AutoAddrInitialize() to enable. */
  LinSNPDIf_WAITING           = 1,  /**<  no auto addressing sequence has been performed since last LinBusImp_AutoAddrNext() call */
  LinSNPDIf_DONE_NOT_LAST     = 2,  /**<  AA sequence successfully performed, but this node was not the last */
  LinSNPDIf_DONE_LAST         = 3,  /**<  AA sequence successful, this node is the last one. Store provided NAD*/
  LinSNPDIf_SKIPED            = 4,  /**<  AA sequence successful, this node has already been adressed, so just skip any action. */
  LinSNPDIf_FAILED            = 5,  /**<  i.e. ADC sampling did not finish on time (i.e. oversampling to high, sample extension too long )*/   
};

typedef enum LinSNPDIf_eState LinSNPDIf_eState_t; /**< Typedef of LinSNPDIf_eState */


/***************************************************************************//**
 * @brief 
 * Enumerator for SNPD modes.
 ******************************************************************************/
enum LinSNPDIf_eSNPDMode
{
  LinSNPDIf_eSNPDMode_TYPE1           = 0,  /**<  @brief SNPD auto addressing mode for Type1 behaviour. */
  LinSNPDIf_eSNPDMode_TYPE2           = 1,  /**<  @brief SNPD auto addressing mode for Type2 behaviour. */
  LinSNPDIf_eSNPDMode_TYPE3           = 2,  /**<  @brief SNPD auto addressing mode for Type3 behaviour. */
};

typedef enum LinSNPDIf_eSNPDMode LinSNPDIf_SNPDMode_t; /**< Typedef of LinSNPDIf_eMode */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINSNPD_TYPES_H_ */

/*! @} */ /* SNPD */
/*! @} */ /* ELMOS_LIN_DRIVER */

