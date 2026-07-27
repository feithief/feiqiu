/***************************************************************************//**
 * @file   Lin_Basictypes.h
 *
 * @creator      RPY
 * @created      2013/10/16
 * @sdfv         Automotive Spice or Elmos Flow or Demo Flow
 *
 * @brief  Definitions of basic data types for the Elmos LIN Driver.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 *
 * $Id: Lin_Basictypes_H430_IAR.h 8455 2021-03-01 19:44:50Z poe $
 *
 * $Revision: 8455 $
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
#ifndef LIN_BASICTYPES_H430_IAR_H_
#define LIN_BASICTYPES_H430_IAR_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#define LIN_NULL  ((void*)(0UL)) /**< An internal definition of "NULL" */

#define LIN_TRUE  ((Lin_Bool_t)1U )           /**< Indicates a 'true' condition of a particular Lin_Bool_t */
#define LIN_FALSE ((Lin_Bool_t)0U )           /**< Indicates a 'false' condition of a particular Lin_Bool_t */


/* ****************************************************************************/
/* ********************STRUCTS, ENUMS AND TYPEDEFS ****************************/
/* ****************************************************************************/
typedef void *        Lin_pvoid_t;  /**< Void-pointer data type. */
typedef const void *  Lin_cpvoid_t; /**< Constant void-pointer data type. */

typedef intptr_t  Lin_intptr_t;     /**< Signed integer type capable of holding an object pointer. */
typedef uintptr_t Lin_uintptr_t;    /**< Unsigned integer type capable of holding an object pointer. */

typedef uint8_t   Lin_uint8_t;      /**< Unsigned 8 bit data type. */
typedef uint16_t  Lin_uint16_t;     /**< Unsigned 16 bit data type. */
typedef uint32_t  Lin_uint32_t;     /**< Unsigned 32 bit data type. */
#ifndef NO_STDINT
typedef uint64_t  Lin_uint64_t;     /**< Unsigned 64 bit data type. */
#endif
typedef int8_t  Lin_sint8_t;        /**< Signed 8 bit data type. */
typedef int16_t Lin_sint16_t;       /**< Signed 16 bit data type. */
typedef int32_t Lin_sint32_t;       /**< Signed 32 bit data type. */
#ifndef NO_STDINT
typedef int64_t Lin_sint64_t;       /**< Signed 64 bit data type. */
#endif
typedef _Bool Lin_Bool_t;	          /**< A boolean data type */

/* ****************************************************************************/
/* *** Derived data types */

typedef Lin_uintptr_t Lin_BufLength_t;                    /**< Basic length data type. */

typedef Lin_uint16_t  Lin_Version_t;                      /**< Version representation data type (major,minor)  */
typedef Lin_uint16_t  Lin_Build_t;                        /**< Build number representation  */

typedef Lin_pvoid_t   Lin_pGenericInterfaceFunctions_t;   /**< Generic pointer to interface functions structure */
typedef Lin_cpvoid_t  Lin_cpGenericInterfaceFunctions_t;  /**< Generic pointer to interface functions structure (const) */

typedef Lin_pvoid_t   Lin_pGenericEnvData_t;              /**< Generic pointer to environment data */
typedef Lin_uintptr_t Lin_EnvDataSze_t;                   /**< Generic type for the environment data size. */

typedef Lin_uint16_t  Lin_Error_t;                        /**< Common error code type */

/***************************************************************************//**
 * @brief A 'this' pointer representation consisting of function table and runtime data.
 ******************************************************************************/
/* PRQA S 3630 6  #This is an interface structure. We do not know when it will be dereferenced,
                   so we can not hide the implementation here. */
struct Lin_sThis
{
    Lin_cpGenericInterfaceFunctions_t  IfFunsTbl; /**< pointer to interface functions */
    Lin_pGenericEnvData_t              EnvData;   /**< pointer to runtime data */
};

typedef struct Lin_sThis      Lin_sThis_t;   /**< Typedef of Lin_sThis. */
typedef        Lin_sThis_t*   Lin_pThis_t;   /**< Typedef of pointer to Lin_sThis. */
typedef const  Lin_sThis_t*   Lin_cpThis_t;  /**< Typedef of pointer to constant Lin_sThis. */

/***************************************************************************//**
 * @brief Enumeration for LIN Driver interfaces.
 ******************************************************************************/
enum Lin_eInterfaceIds
{
  Lin_IfId_SNPD             = 0,  /**< SNPD Layer Interface ID. */
  Lin_IfId_BUS              = 1,  /**< BUS Layer Interface ID. */
  Lin_IfId_LOOKUP           = 2,  /**< Lookup Layer Interface ID. */
  Lin_IfId_PROTO            = 3,  /**< Protocol Layer Interface ID. */
  Lin_IfId_TRANS            = 4,  /**< Transport Layer Interface ID. */
  Lin_IfId_DIAG             = 5,  /**< Diagnose Layer Interface ID. */
  Lin_IfId_DATA_STORAGE     = 6,  /**< Data storage ID. */
  Lin_IfId_TRANS_RESP_BUILD = 7,  /**< Transport layer response contruction interface ID. */
  
  Lin_IfId_Count            = 8   /**< Actual count of available interfaces ID. */
};

typedef enum Lin_eInterfaceIds Lin_eInterfaceIds_t; /**< Typedef of Lin_eInterfaceIds. */

#define LIN_ERROR_AREA_SIZE        256u

/***************************************************************************//**
 * @brief
 *
 * BUS error codes:		0x100 - 0x1FF (256 - 511)
 * LOOKUP error codes:	0x200 - 0x2FF (512 - 767)
 * PROTO error codes:   0x300 - 0x3FF (768 - 1023)
 * TRANS error codes:   0x400 - 0x4FF (1024 - 1279)
 * DIAG error codes:    0x500 - 0x5FF (1280 - 1535)
 * DATASTG error codes: 0x600 - 0x6FF (1536 - 1791)
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 1840,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.,
 * Conflicts in signedness.,
 * Always make sure unsigned values ate defined and used.}
 ******************************************************************************/
// PRQA S 1840 ++
/* PRQA S 3630, 1755 2 #justification: this enum is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */   
enum Lin_eErrorCodeAreas
{
  Lin_NO_ERROR                     = 0,                                            /**< common value for all module to indicate an error free condition */  
    
  Lin_ERROR_AREA_BUS               = LIN_ERROR_AREA_SIZE,                          /**< Error codes related to bus start in this area */
  Lin_ERROR_AREA_LOOKUP            = Lin_ERROR_AREA_BUS      + LIN_ERROR_AREA_SIZE,  /**< Error codes related to lookup start in this area */
  Lin_ERROR_AREA_PROTO             = Lin_ERROR_AREA_LOOKUP   + LIN_ERROR_AREA_SIZE,  /**< Error codes related to protocol start in this area */
  Lin_ERROR_AREA_TRANS             = Lin_ERROR_AREA_PROTO    + LIN_ERROR_AREA_SIZE,  /**< Error codes related to transport start in this area */
  Lin_ERROR_AREA_DIAG_UDS          = Lin_ERROR_AREA_TRANS    + LIN_ERROR_AREA_SIZE,  /**< Error codes related to diag start in this area */
  Lin_ERROR_AREA_DATASTG           = Lin_ERROR_AREA_DIAG_UDS + LIN_ERROR_AREA_SIZE   /**< Error codes related to data storage start in this area */
  
  /* ... */
};
// PRQA S 1840 --

/* ****************************************************************************/
/* *************************** GLOBALE VARIABLES ******************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ******************** EXTERNAL FUNCTIONS / INTERFACE ************************/
/* ****************************************************************************/

#endif /* LIN_BASICTYPES_H430_IAR_H_ */
/*! @} */ /* ELMOS_LIN_DRIVER */
