/***************************************************************************//**
 * @file			LinDataStg_DataValueIDs.h
 *
 * @creator		sbai
 * @created		25.11.2015
 *
 * @brief     TODO
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DATASTG
 * @{
 *
 * $Id: LinDataStg_DataValueIDs.h 8305 2021-02-03 13:29:35Z poe $
 *
 * $Revision: 8305 $
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

#ifndef LINDATASTG_DATAVALUEIDS_H_
#define LINDATASTG_DATAVALUEIDS_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDataStg_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief LIN Data Storage layer interface Data Value ID enumerator.
 *
 * @misra{M3CM Dir-10.1. - PRQA Msg 4521,
 * TODO
 * Conflicts in  signedness.,
 * Always make sure unsigned values ate defined and used.}
 ******************************************************************************/
// PRQA S 4521 ++
enum LinDataStgIf_eDataValueID
{
  LinDataStgIf_DVID_NAD                                = LinDataStg_DVID_AREA_INTERFACE + 0u,      /* The LIN NAD. @see LIN 2.2a Specification - Chapter 4.2.3.2 NAD */
  LinDataStgIf_DVID_SupplierID                         = LinDataStg_DVID_AREA_INTERFACE + 1u,      /* The LIN Supplier ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
  LinDataStgIf_DVID_FunctionID                         = LinDataStg_DVID_AREA_INTERFACE + 2u,      /* The LIN Function ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
  LinDataStgIf_DVID_VariantID                          = LinDataStg_DVID_AREA_INTERFACE + 3u,      /* The LIN Variant ID. @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
  LinDataStgIf_DVID_ProductIdentification              = LinDataStg_DVID_AREA_INTERFACE + 4u,      /* The LIN Product Identification (Supplier ID + Function ID + Variant ID). @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
  LinDataStgIf_DVID_SerialNumber                       = LinDataStg_DVID_AREA_INTERFACE + 5u,      /* The LIN Serial Number @see LIN 2.2a Specification - Chapter 4.2.1 LIN PRODUCT IDENTIFICATION */
  LinDataStgIf_DVID_FingerprintAndProgDateOfLogSwBlcks = LinDataStg_DVID_AREA_INTERFACE + 6u,      /* Fingerprint And Programming Date Of Logical Software Blocks  (0xF15B) @see VW 80125 */
  LinDataStgIf_DVID_DiagnosticSession                  = LinDataStg_DVID_AREA_INTERFACE + 7u,      /* Active Diagnostic Session (0xF186 - activeDiagnosticSessionDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_VehManSparePartNumber              = LinDataStg_DVID_AREA_INTERFACE + 8u,      /* Vehicle Manufacturer Spare Part Number (0xF187 - vehicleManufacturerSparePartNumberDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_VehManECUSoftwareNumber            = LinDataStg_DVID_AREA_INTERFACE + 9u,      /* Vehicle Manufacturer ECU Software Number (0xF188 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_VehManECUSoftwareVersionNumber     = LinDataStg_DVID_AREA_INTERFACE + 10u,     /* Vehicle Manufacturer ECU Software Version Number (0xF189 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier ) @see ISO 14229-1 */
  LinDataStgIf_DVID_ECUSerialNumber                    = LinDataStg_DVID_AREA_INTERFACE + 11u,     /* ECU Serial Number (0xF18C - ECUSerialNumberDataIdentifier ) @see ISO 14229-1 */
  LinDataStgIf_DVID_VIN                                = LinDataStg_DVID_AREA_INTERFACE + 12u,     /* Vehicle Identification Number  (0xF190 - VINDataIdentifier ) @see ISO 14229-1 */
  LinDataStgIf_DVID_VehManECUHardwareNumber            = LinDataStg_DVID_AREA_INTERFACE + 13u,     /* Vehicle Manufacturer ECU Hardware Number (0xF191 - vehicleManufacturerECUHardwareNumberDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_SysSupplECUHardwareNumber          = LinDataStg_DVID_AREA_INTERFACE + 14u,     /* System Supplier ECU Hardware Number (0xF192 - systemSupplierECUHardwareNumberDataIdentifier ) @see ISO 14229-1 */
  LinDataStgIf_DVID_SysSupplECUHWVersionNumber         = LinDataStg_DVID_AREA_INTERFACE + 15u,     /* System Supplier ECU Hardware Version Number (0xF193 - systemSupplierECUHardwareVersionNumberDataIdentifier) */
  LinDataStgIf_DVID_SysSuppECUSoftwareNumber           = LinDataStg_DVID_AREA_INTERFACE + 16u,     /* System Supplier ECU Software Number (0xF194 - vehicleManufacturerECUSoftwareNumberDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_SysSuppECUSoftwareVersionNumber    = LinDataStg_DVID_AREA_INTERFACE + 17u,     /* System Supplier ECU Software Version Number (0xF195 - vehicleManufacturerECUSoftwareVersionNumberDataIdentifier ) @see ISO 14229-1 */
  LinDataStgIf_DVID_SystemNameOrEngineType             = LinDataStg_DVID_AREA_INTERFACE + 18u,     /* System Name or Engine Type (0xF197 - systemNameOrEngineTypeDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_ODXFileId                          = LinDataStg_DVID_AREA_INTERFACE + 19u,     /* ODX File ID (0xF19E - ODXFileDataIdentifier) @see ISO 14229-1 */
  LinDataStgIf_DVID_LogicalSwBlkCntOfProgAttempts      = LinDataStg_DVID_AREA_INTERFACE + 20u,     /* VW Logical Software Block Counter Of Programming Attempts  (0x0407) @see VW 80125 */
  LinDataStgIf_DVID_LogicalSwBlkLockValue              = LinDataStg_DVID_AREA_INTERFACE + 21u,     /* VW Logical Software Block Lock Value  (0x040F) @see VW 80125 */
  LinDataStgIf_DVID_ODXFileVersion                     = LinDataStg_DVID_AREA_INTERFACE + 22u,     /* ODX File Version (0xF1A2) @see VW 80125 */
  LinDataStgIf_DVID_ECUHWVersionNumber                 = LinDataStg_DVID_AREA_INTERFACE + 23u,     /* VW ECU Hardware Version Number (0xF1A3 - VWECUHardwareVersionNumber) @see VW 80125 */
  LinDataStgIf_DVID_LogicalSwBlkVersion                = LinDataStg_DVID_AREA_INTERFACE + 24u,     /* VW Logical Software Block Version  (0xF1AB) @see VW 80125 */
  LinDataStgIf_DVID_ECUProgInfo                        = LinDataStg_DVID_AREA_INTERFACE + 25u,     /* ECU Programming Information (0xF1DF) @see VW 80125 */
  LinDataStgIf_DVID_LogSwBlksData                      = LinDataStg_DVID_AREA_INTERFACE + 26u,     /* Data about the logical SW Blocks. */
  LinDataStgIf_DVID_ProgrammingRequest                 = LinDataStg_DVID_AREA_INTERFACE + 27u,
  LinDataStgIf_DVID_VWSupplierNumber                   = LinDataStg_DVID_AREA_INTERFACE + 28u,     /* VW Supplier Number (0xF17F) @see VW 80125 */
  LinDataStgIf_DVID_ECUManufacturingDate               = LinDataStg_DVID_AREA_INTERFACE + 29u,     /* ECU Manufacturing Date (0xF18B) @see VW 80125 */
  LinDataStgIf_DVID_ECUProductionTestSystemNumber      = LinDataStg_DVID_AREA_INTERFACE + 30u,     /* ECU Production Test System Number (0xF17D) @see VW 80125 */
  LinDataStgIf_DVID_ECUSupplierNumber                  = LinDataStg_DVID_AREA_INTERFACE + 31u,     /* ECU Supplier Number (0xF179) @see VW 80125 */
  LinDataStgIf_DVID_VWFAZITIdentificationString        = LinDataStg_DVID_AREA_INTERFACE + 32u,     /* VW FAZIT Identification String (0xF17C) @see VW 80125 */
  LinDataStgIf_DVID_VWECUSerialNumber                  = LinDataStg_DVID_AREA_INTERFACE + 33u,     /* VW ECU Serial Number (0xF18C) @see VW 80125 */
  LinDataStgIf_DVID_SendECUResetServiceResponse        = LinDataStg_DVID_AREA_INTERFACE + 35u,
  LinDataStgIf_DVID_CNT                                = LinDataStg_DVID_AREA_INTERFACE + 36u,     /* Count of Interface Data Value IDs */
  LinDataStgIf_DVID_MAX                                = LinDataStg_DVID_AREA_INTERFACE + 0x0FFFu, /* Maximal value of Interface Data Value IDs */
  LinDataStgIf_DVID_INVALID                            = LinDataStg_DVID_INVALID,                  /* 0xFFFF */
};

typedef enum LinDataStgIf_eDataValueID LinDataStgIf_eDataValueID_t; /**< Typedef of LinDataStgIf_eDataValueID. */
// PRQA S 4521 --

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

#endif /* LINDATASTG_DATAVALUEIDS_H_ */

/*! @} */ /* DATASTG */
/*! @} */ /* ELMOS_LIN_DRIVER */

