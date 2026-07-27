/***************************************************************************//**
 * @file      LinDataStg_Types.h
 *
 * @creator   sbai
 * @created   25.11.2015
 *
 * @brief     Definitions of basic data types for the 'LIN Data Storage Layer'.
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

#ifndef LINDATASTG_TYPES_H_
#define LINDATASTG_TYPES_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "Lin_Basictypes.h"
#include "LinUDS_Types.h"
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define LINDATASTG_DVID_AREA_SIZE 0x1000u /**< Size of DVID areas/ranges. I.e. interface and implementation. */

#define LINDATASTGIF_DVID_INVALID       (0xFFFFu)
#define LINDATASTGIF_DVT_INVALID        (0xFFFFu)
#define LINDATASTGIF_FLASHTGT_INVALID   (0xFFFFu)
#define LINDATASTGIF_MT_INVALID         (0xFFFFu)
#define LinDataStgIf_TOKEN_INVALID      (0x00000000U)
#define LinDataStgIf_TOKEN_VALID        (0xFFFFFFFFU)
#define LinDataStgIf_TOKEN_PROTECTED    (0xA5A5A5A5U)

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 *
 ******************************************************************************/

typedef       Lin_uint16_t             LinDataStgIf_DataValueID_t;             /**< Type for 'Data Value ID'. Placeholder for implementation depended enum 'LinDataStgImp_eDataValueID_t'. */
typedef       Lin_uint16_t             LinDataStgIf_DataValueType_t;
typedef       Lin_uint32_t             LinDataStgIf_ValidToken_t;
typedef       Lin_uint8_t              LinDataStgIf_Data_t;                    /**< Basic LIN Data Storage data type */
typedef       LinDataStgIf_Data_t*     LinDataStgIf_pData_t;                   /**< Pointer to LinDataStgIf_Data_t */
typedef const LinDataStgIf_Data_t*     LinDataStgIf_cpData_t;
typedef       LinDataStgIf_Data_t**    LinDataStgIf_ppData_t;                  /**< Pointer-Pointer to LinDataStgIf_Data_t */
typedef       Lin_uint32_t             LinDataStgIf_IndexCounter_t;
typedef       Lin_uint16_t             LinDataStgIF_DataLen_t;
typedef       LinDataStgIF_DataLen_t*  LinDataStgIF_pDataLen_t;
typedef       Lin_pvoid_t              LinDataStgIf_pGenericImpCfgData_t;      /**< Generic pointer to configuration parameter of the specific LIN Data Storage layer implementation. */
typedef       Lin_cpvoid_t             LinDataStgIf_cpGenericImpCfgData_t;     /**< Generic const pointer to configuration parameter of the specific LIN Data Storage layer implementation. */
typedef       Lin_cpvoid_t             LinDataStgIf_cpAccessContextData_t;

struct LinDataStgIf_sDataEntryHeader
{
	LinDataStgIf_DataValueID_t   DataValueId;
	LinDataStgIf_DataValueType_t DataValueType;
	LinDataStgIf_ValidToken_t    ValidToken;
	LinDataStgIF_DataLen_t       PaddedDataLen;
	LinDataStgIF_DataLen_t       DataLen;
};

typedef struct LinDataStgIf_sDataEntryHeader    LinDataStgIf_DataEntryHeader_t;
typedef        LinDataStgIf_DataEntryHeader_t*  LinDataStgIf_pDataEntryHeader_t;

struct LinDataStgIf_sDataEntry
{
	LinDataStgIf_DataEntryHeader_t header;
    // PRQA S 1060 1 # flexible arra member is used to th specail structur of the data store
	LinDataStgIf_Data_t            Data[];
};

typedef struct LinDataStgIf_sDataEntry   LinDataStgIf_DataEntry_t;
typedef        LinDataStgIf_DataEntry_t* LinDataStgIf_pDataEntry_t;
typedef const  LinDataStgIf_DataEntry_t* LinDataStgIf_cpDataEntry_t;
/***************************************************************************//**
 *
 ******************************************************************************/
enum LinDataStgIf_eDataStorageMemType
{
	LinDataStgIf_MT_READONLY       = 0u,
	LinDataStgIf_MT_READWRITE      = 1u,
	LinDataStgIf_MT_READWRITEERASE = 2u,
	LinDataStgIf_MT_INVALID        = LINDATASTGIF_MT_INVALID
};

typedef enum LinDataStgIf_eDataStorageMemType LinDataStgIf_DataStorageMemType_t;
/***************************************************************************//**
 *
 ******************************************************************************/

enum LinDataStgIf_eDataStorageFlashTarget
{
	LinDataStgIf_FLASHTGT_MAIN    = 0u,
	LinDataStgIf_FLASHTGT_INFO    = 1u,
	LinDataStgIf_FLASHTGT_INVALID = LINDATASTGIF_FLASHTGT_INVALID
};

typedef enum LinDataStgIf_eDataStorageFlashTarget LinDataStgIf_DataStorageFlashTarget_t;
/***************************************************************************//**
 *
 ******************************************************************************/
// PRQA S 3630 1 # This rule is advisory
struct LinDataStgIf_sPage
  {
    LinDataStgIf_IndexCounter_t IndexCounter;
    // PRQA S 1060 1 # flexible arra member is used to th specail structur of the data store
    LinDataStgIf_pData_t DataArea[];
  };

typedef struct LinDataStgIf_sPage   LinDataStgIf_Page_t;
typedef        LinDataStgIf_Page_t* LinDataStgIf_pPage_t;
typedef const  LinDataStgIf_Page_t* LinDataStgIf_cpPage_t;

struct LinDataStgIf_sDataStorageEntry
{
	LinDataStgIf_DataStorageFlashTarget_t FlashTarget;
	LinDataStgIf_DataStorageMemType_t     MemType;
	LinDataStgIf_pPage_t                  StartAddressArea0;
	LinDataStgIf_pPage_t                  StartAddressArea1;
	Lin_uint16_t                          Length;
};

typedef struct LinDataStgIf_sDataStorageEntry   LinDataStgIf_DataStorageEntry_t;
typedef        LinDataStgIf_DataStorageEntry_t* LinDataStgIf_pDataStorageEntry_t;
typedef const LinDataStgIf_DataStorageEntry_t*  LinDataStgIf_cpDataStorageEntry_t;

/***************************************************************************//**
 *
 ******************************************************************************/

/* ****************************************************************************/
/*                            Simple data types                               */
/* ****************************************************************************/
typedef Lin_Error_t            LinDataStgIf_Error_t;              /**< Generic data types for DATASTG layer error codes. */

typedef Lin_pvoid_t            LinDataStgIf_pGenericEnvData_t;    /**< Generic pointer to environment data of the LIN Data Storage layer module. */
typedef Lin_EnvDataSze_t       LinDataStgIf_EnvDataSze_t;         /**< LIN Data Storage layer data type for the environment data length. */
/*
 *typedef Lin_pvoid_t            LinDataStgIf_pGenericCbCtxData_t;*//**< Pointer to LIN Data Storage callback context data. */

typedef Lin_BufLength_t        LinDataStgIf_Length_t;             /**< LIN Data Storage layer data type of length values */
typedef LinDataStgIf_Length_t* LinDataStgIf_pLength_t;            /**< Pointer to LinDataStgIf_Length_t */

typedef Lin_uint16_t           LinDataStgIf_DataStorageID_t;      /**< Type for 'Data Storage ID'. Placeholder for implementation depended enum 'LinDataStgIf_DataValueID_DATASTG_t'. */

struct LinDataStgIf_sInterfaceFunctions;                          /**< Forward declaration of LIN Data Storage layer interface functions. */
typedef struct LinDataStgIf_sInterfaceFunctions    LinDataStgIf_sInterfaceFunctions_t;  /**< Typedef for LinDataStgIf_sInterfaceFunctions. */
typedef        LinDataStgIf_sInterfaceFunctions_t* LinDataStgIf_pInterfaceFunctions_t;  /**< Typedef of pointer to LinDataStgIf_sInterfaceFunctions. */
typedef const  LinDataStgIf_sInterfaceFunctions_t* LinDataStgIf_cpInterfaceFunctions_t; /**< Typedef of constant pointer to LinDataStgIf_sInterfaceFunctions. */

/***************************************************************************//**
 * @brief LIN Data Storage layer data value data type enumerator
 ******************************************************************************/
enum LinDataStgIf_eValueDataType
{
	LinDataStgIf_DVT_DATASTG_DATA 	= 0u, /**< @brief Datastorage type. */
	LinDataStgIf_DVT_UDS_DID_DATA 	= 1u, /**< @brief UDS data identifier type. */
	LinDataStgIf_DVT_UDS_CFG_DATA 	= 2u, /**< @brief UDS related configuration data type. */
	LinDataStgIf_DVT_BLCFG_DATA 	= 3u, /**< @brief Bootloader configuration data type. */
	LinDataStgIf_DVT_LIN_DATA 		= 4u, /**< @brief LIN data type. */
	LinDataStgIf_DVT_CODE_DATA 		= 5u, /**< @brief Executable code data type. */
	LinDataStgIf_DVT_CUSTOM_DATA 	= 6u, /**< @brief Custom defined data type. */
	LinDataStgIf_DVT_UNDEFINED 		= LINDATASTGIF_DVT_INVALID
};

typedef enum LinDataStgIf_eValueDataType LinDataStgIf_ValueDataType_t;

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for UDS CFG data.
 ******************************************************************************/
enum LinDataStgIf_eDataValueID_UDS_CFG
{
	LinDataStgIf_DVID_UDS_CFG_LogSwBlksData              = 0u, 							/**< @brief Data about the logical SW Blocks. */
	LinDataStgIf_DVID_UDS_CFG_ProgrammingRequest         = 1u, 							/**< @brief Programming request marker. */
	LinDataStgIf_DVID_UDS_CFG_InvalidKeyCtr              = 2u, 							/**< @brief VW InvalidKeyCounter (KeyCtr) @see VW80124 */
	LinDataStgIf_DVID_UDS_CFG_AppHasBeenProgrammed       = 3u, 							/**< @brief Application has been programmed flag @see VW80124 */
	LinDataStgIf_DVID_UDS_CFG_ApplicationSoftwareVersion = 4u, 							/**< @brief Application version number. */
	LinDataStgIf_DVID_UDS_CFG_INVALID                    = LINDATASTGIF_DVID_INVALID
};

typedef enum LinDataStgIf_eDataValueID_UDS_CFG LinDataStgIf_DataValueID_UDS_CFG_t; /**< @brief Typedef of LinDataStgIf_eDataValueID_UDS_CFG. */

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for CODE types.
 ******************************************************************************/
enum LinDataStgIf_eDataValueID_CODE
{
	LinDataStgIf_DVID_CODE_KaS_GetSeedFun_CodeBlob 		= 0u, /**< @brief KaS seed generation code blob. */
	LinDataStgIf_DVID_CODE_KaS_CompareKeyFun_CodeBlob 	= 1u, /**< @brief KaS key compare code blob. */
	LinDataStgIf_DVID_CODE_INVALID 						= LINDATASTGIF_DVID_INVALID
};

typedef enum LinDataStgIf_eDataValueID_CODE LinDataStgIf_DataValueID_CODE_t; /**< @brief Typedef of LinDataStgIf_eDataValueID_CODE. */

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for BLCFG types.
 ******************************************************************************/
enum LinDataStgIf_eDataValueID_BLCFG
{
	LinDataStgIf_DVID_BLCFG_ApplicationInfo 				= 0u, /**< @brief Application info type. */
	LinDataStgIf_DVID_BLCFG_CheckApplicationCRCOnStartup 	= 1u, /**< @brief Flag to check application area CRC on bootloader startup. */
	LinDataStgIf_DVID_BLCFG_EnableJTAGAccess 				= 2u, /**< @brief Flag to enable JTAG access */
	LinDataStgIf_DVID_BLCFG_52436_v100 						= 3u,
	LinDataStgIf_DVID_BLCFG_INVALID = LINDATASTGIF_DVID_INVALID
};

typedef enum LinDataStgIf_eDataValueID_BLCFG LinDataStgIf_DataValueID_BLCFG_t; /**< @brief Typedef of LinDataStgIf_eDataValueID_BLCFG. */

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for LIN types.
 ******************************************************************************/
enum LinDataStgIf_eDataValueID_LIN
{
	LinDataStgIf_DVID_LIN_NAD 					= 0u, /**< @brief The LIN NAD. @see LIN 2.2a Specification - Chapter 4.2.3.2 NAD */
	LinDataStgIf_DVID_LIN_SupplierID 			= 1u, /**< @brief The LIN Supplier ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
	LinDataStgIf_DVID_LIN_FunctionID 			= 2u, /**< @brief The LIN Function ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
	LinDataStgIf_DVID_LIN_VariantID 			= 3u, /**< @brief The LIN Variant ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
	LinDataStgIf_DVID_LIN_ProductIdentification = 4u, /**< @brief The LIN Product Identification (Supplier ID + Function ID + Variant ID). @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
	LinDataStgIf_DVID_LIN_SerialNumber 			= 5u, /**< @brief The LIN Serial Number @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
	LinDataStgIf_DVID_LIN_INVALID 				= LINDATASTGIF_DVID_INVALID
};

typedef enum LinDataStgIf_eDataValueID_LIN LinDataStgIf_DataValueID_LIN_t; /**< @brief Typedef of LinDataStgIf_eDataValueID_LIN. */

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for DID types.
 ******************************************************************************/

enum LinDataStgIf_eDataValueID_UDS_DID
{
 LinDataStgIf_DVID_UDS_DID_LogicalSwBlkCntOfProgAttempts      = LinUDSIf_eDID_LogicalSwBlkCntOfProgAttempts,      /**< @brief VW Logical Software Block Counter Of Programming Attempts  (0x0407) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_LogicalSwBlkLockValue              = LinUDSIf_eDID_LogicalSwBlkLockValue,              /**< @brief VW Logical Software Block Lock Value  (0x040F) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_Fingerprint                        = LinUDSIf_eDID_Fingerprint,                        /**< @brief Fingerprint (0xF15A) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_FingerprintAndProgDateOfLogSwBlcks = LinUDSIf_eDID_FingerprintAndProgDateOfLogSwBlcks, /**< @brief Fingerprint And Programming Date Of Logical Software Blocks  (0xF15B) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_ECUSupplierNumber                  = LinUDSIf_eDID_ECUSupplierNumber,                  /**< @brief ECU Supplier Number (0xF179) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_VWFAZITIdentificationString        = LinUDSIf_eDID_VWFAZITIdentificationString,        /**< @brief VW FAZIT Identification String (0xF17C) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_ECUProductionTestSystemNumber      = LinUDSIf_eDID_ECUProductionTestSystemNumber,      /**< @brief ECU Production Test System Number (0xF17D) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_VWSupplierNumber                   = LinUDSIf_eDID_VWSupplierNumber,                   /**< @brief VW Supplier Number (0xF17F) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_VWECUSerialNumber                  = LinUDSIf_eDID_ECUSerialNumber,                    /**< @brief VW ECU Serial Number (0xF18C) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_DiagnosticSession                  = LinUDSIf_eDID_DiagnosticSession,                  /**< @brief Active Diagnostic Session (0xF186 - activeDiagnosticSessionDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_VehManSparePartNumber              = LinUDSIf_eDID_VehManSparePartNumber,              /**< @brief Vehicle Manufacturer Spare Part Number (0xF187 - vehicleManufacturerSparePartNumberDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_VehManECUSoftwareNumber            = LinUDSIf_eDID_VehManECUSoftwareNumber,            /**< @brief Vehicle Manufacturer ECU Software Number (0xF188 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_ECUManufacturingDate               = LinUDSIf_eDID_ECUManufacturingDate,               /**< @brief ECU Manufacturing Date (0xF18B) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_VehManECUSoftwareVersionNumber     = LinUDSIf_eDID_VehManECUSoftwareVersionNumber,     /**< @brief Vehicle Manufacturer ECU Software Version Number (0xF189 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier ) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_ECUSerialNumber                    = LinUDSIf_eDID_ECUSerialNumber,                    /**< @brief ECU Serial Number (0xF18C - ECUSerialNumberDataIdentifier ) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_VIN                                = LinUDSIf_eDID_VIN,                                /**< @brief Vehicle Identification Number  (0xF190 - VINDataIdentifier ) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_VehManECUHardwareNumber            = LinUDSIf_eDID_VehManECUHardwareNumber,            /**< @brief Vehicle Manufacturer ECU Hardware Number (0xF191 - vehicleManufacturerECUHardwareNumberDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_SysSupplECUHardwareNumber          = LinUDSIf_eDID_SysSupplECUHardwareNumber,          /**< @brief System Supplier ECU Hardware Number (0xF192 - systemSupplierECUHardwareNumberDataIdentifier ) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_SysSupplECUHWVersionNumber         = LinUDSIf_eDID_SysSupplECUHWVersionNumber,         /**< @brief System Supplier ECU Hardware Version Number (0xF193 - systemSupplierECUHardwareVersionNumberDataIdentifier) */
 LinDataStgIf_DVID_UDS_DID_SysSuppECUSoftwareNumber           = LinUDSIf_eDID_SysSuppECUSoftwareNumber,           /**< @brief System Supplier ECU Software Number (0xF194 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_SysSuppECUSoftwareVersionNumber    = LinUDSIf_eDID_SysSuppECUSoftwareVersionNumber,    /**< @brief System Supplier ECU Software Version Number (0xF195 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier ) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_SystemNameOrEngineType             = LinUDSIf_eDID_SystemNameOrEngineType,             /**< @brief System Name or Engine Type (0xF197 - systemNameOrEngineTypeDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_ODXFileId                          = LinUDSIf_eDID_ODXFileId,                          /**< @brief ODX File ID (0xF19E - ODXFileDataIdentifier) @see ISO 14229-1 */
 LinDataStgIf_DVID_UDS_DID_ODXFileVersion                     = LinUDSIf_eDID_ODXFileVersion,                     /**< @brief ODX File Version (0xF1A2) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_ECUHWVersionNumber                 = LinUDSIf_eDID_ECUHWVersionNumber,                 /**< @brief VW ECU Hardware Version Number (0xF1A3 - VWECUHardwareVersionNumber) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_LogicalSwBlkVersion                = LinUDSIf_eDID_LogicalSwBlkVersion,                /**< @brief VW Logical Software Block Version  (0xF1AB) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_ECUProgInfo                        = LinUDSIf_eDID_ECUProgInfo,                        /**< @brief ECU Programming Information (0xF1DF) @see VW 80125 */
 LinDataStgIf_DVID_UDS_DID_INVALID                            = LINDATASTGIF_DVID_INVALID
};
typedef enum LinDataStgIf_eDataValueID_UDS_DID LinDataStgIf_DataValueID_UDS_DID_t; /**< @brief Typedef of LinDataStgIf_DataValueID_UDS_DID_t. */

/***************************************************************************//**
 * @brief LIN Data Storage layer implementation Data Value ID enumerator for DATASTG types.
 ******************************************************************************/
enum LinDataStgIf_eDataValueID_DATASTG
{
	LinDataStgIf_DVID_DATASTG_UDSCONSTANT_DATA = 0u,
	LinDataStgIf_DVID_DATASTG_UDSWRITEABLE_DATA = 1u,
	LinDataStgIf_DVID_DATASTG_UDSVARIABLE_DATA = 2u,
	LinDataStgIf_DVID_DATASTG_APP_DATA = 3u,
	LinDataStgIf_DVID_DATASTG_CUSTOM00_DATA = 4u,
	LinDataStgIf_DVID_DATASTG_INVALID = LINDATASTGIF_DVID_INVALID
};

typedef enum LinDataStgIf_eDataValueID_DATASTG LinDataStgIf_DataValueID_DATASTG_t;

/***************************************************************************//**
 * @brief LIN Data Storage layer error enumerator
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * The Elmos LIN Driver defines areas of error codes for every module and sub-areas between
 * general defined interface error codes and implementation specific error codes. To link 
 * between this areas some arithmetic offset calculation has to be done between different
 * enum types.,
 * Conflicts in  signedness.,
 * Always make sure unsigned values are defined and used.}
 *
 ******************************************************************************/
// PRQA S 4521 ++
enum LinDataStgIf_eErrorCodes
{
  LinDataStgIf_ERR_NO_ERROR                 = Lin_NO_ERROR,                      /**< No error at all. */
  LinDataStgIf_ERR_READ_ONLY                = Lin_ERROR_AREA_DATASTG       + 1u, /**< 'Data Value' is read-only. */
  LinDataStgIf_ERR_UNKOWN_DATA_VALUE_ID     = Lin_ERROR_AREA_DATASTG       + 2u, /**< Unknown 'Data Value ID'*/
  LinDataStgIf_ERR_DATA_VALUE_ACCESS_FAILED = Lin_ERROR_AREA_DATASTG       + 3u, /**< Set/Get of 'Data Value' failed. */
  LinDataStgIf_ERR_DATA_VALUE_LEN_MISMATCH  = Lin_ERROR_AREA_DATASTG       + 4u, /**< 'Data Value' length mismatches. */
  LinDataStgIf_ERR_DATA_VAL_DEF_MISS        = Lin_ERROR_AREA_DATASTG       + 5u, /**< Missing Data Value ID definition. */
  LinDataStgIf_ERR_DATASTG_NO_FREE_STG_SLOT = Lin_ERROR_AREA_DATASTG       + 6u, /**< No free slot for additional data storages available. */
  LinDataStgIf_ERR_DATASTG_FULL             = Lin_ERROR_AREA_DATASTG       + 7u, /**< Data could not be written because of not enough space. */
  LinDataStgIf_ERR_DATASTG_UNKNOWN_ID       = Lin_ERROR_AREA_DATASTG       + 8u, /**< No data storage could be indentified for given id. */
  LinDataStgIf_ERR_DATASTG_ENTYR_PROTECTED  = Lin_ERROR_AREA_DATASTG       + 9u, /**< No data storage could be indentified for given id. */
  LinDataStgIf_ERR_IMPL_ERROR_AREA          = Lin_ERROR_AREA_DATASTG
			                                  + (LIN_ERROR_AREA_SIZE / 2u),       /**< Any additional implementation specific error codes start here. */
  LinDataStgIf_ERR_ACCESS_DENIED            = Lin_ERROR_AREA_DATASTG
			                                  + (LIN_ERROR_AREA_SIZE / 2u) + 1u,  /**< Access denied error froam flash occured */
  LinDataStgIf_ERR_FLASH                    = Lin_ERROR_AREA_DATASTG 
                                              + (LIN_ERROR_AREA_SIZE / 2u) + 2u,  /**< Other flash error than Access denied occured */
  LinDataStgIf_ERROR_UNKWON_ACTIVE_PAGE     = Lin_ERROR_AREA_DATASTG
			                                  + (LIN_ERROR_AREA_SIZE / 2u) + 3u,  /**< The active page could not be determinated (shouldn't occur) */
  LinDataStgIf_NULL_POINTER_REFERENCE       = Lin_ERROR_AREA_DATASTG
			                                  + (LIN_ERROR_AREA_SIZE / 2u) + 4u,  /**< a NULL Pointer was given as reference */
	LinDataStgIf_ERR_UNKWON_MEM_TYPE        = Lin_ERROR_AREA_DATASTG
			                                  + (LIN_ERROR_AREA_SIZE / 2u) + 5u   /**< a NULL Pointer was given as reference */
};
// PRQA S 4521 --

typedef enum LinDataStgIf_eErrorCodes LinDataStgIf_eErrorCodes_t; /**< Typedef of LinDataStgIf_eErrorCodes. */

/*
 *struct         LinDataStgIf_sCallbackFunctions;                                        *//**< Forward declaration of LIN Data Storage layer callback functions. */
/*
 *typedef struct LinDataStgIf_sCallbackFunctions     LinDataStgIf_sCallbackFunctions_t;  *//**< Typedef for LinDataStgIf_sCallbackFunctions. */
/*
 *typedef        LinDataStgIf_sCallbackFunctions_t*  LinDataStgIf_pCallbackFunctions_t;  *//**< Typedef of pointer to LinDataStgIf_sCallbackFunctions. */
/*
 *typedef const  LinDataStgIf_sCallbackFunctions_t*  LinDataStgIf_cpCallbackFunctions_t; *//**< Typedef of constant pointer to LinDataStgIf_sCallbackFunctions. */

struct         LinDataStgIf_sThis;                         /**< Forward declaration of LIN Data Storage layer This-Pointer. */
typedef struct LinDataStgIf_sThis    LinDataStgIf_sThis_t; /**< Typedef for LinDataStgIf_sThis. */
typedef        LinDataStgIf_sThis_t* LinDataStgIf_pThis_t; /**< Typedef of pointer to LinDataStgIf_sThis. */

/*
 *struct         LinDataStgIf_sInitParam;                             *//**< Forward declaration of LIN Data Storage layer initialization parameter struct. */
/*
 *typedef struct LinDataStgIf_sInitParam    LinDataStgIf_sInitParam_t;*//**< Typedef for LinDataStgIf_sInitParam */
/*
 *typedef        LinDataStgIf_sInitParam_t* LinDataStgIf_pInitParam_t;*//**< Typedef of pointer to LinDataStgIf_sInitParam */

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINDATASTG_TYPES_H_ */

/*! @} *//* DATASTG */
/*! @} *//* ELMOS_LIN_DRIVER */

