/***************************************************************************//**
 * @file     LinDrvImp_Config.h
 *
 * @creator  poe
 * @created  2021/02/23 
 *
 * @brief    LIN Driver Configuration and Intitialization
 *
 * @purpose  This module contains the LIN driver configuration data
 *           and handles the initialization process.
 *
 *
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
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
 * $Id: $
 *
 * $Revision: 1929 $
 *
 ******************************************************************************/

#ifndef LINDRVIMP_CONFIG_H_
#define LINDRVIMP_CONFIG_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinBus_Implementation.h"
#include "vic_InterruptHandler.h"
#include "LinLookup_Implementation_Sequential.h"
#include "LinProto_Implementation.h"
#include "LinTrans_Implementation.h"
#include "LinDiag_Implementation.h"
#include "Lin_EnvDataSizes.h"
#include "rom_API.h"
#include "bootloader_UserConfig.h"
#include "bootloader_interface.h"
#include "eeprom_access_interface.h"
#include "flash_access_interface.h"
/* ****************************************************************************/
/* ***************************** USER INCLUDES ********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define EXAMPLE_EE_NAD_LOC EE_START
/* ****************************************************************************/
/* **************************** INTERFACE OPTIONS *************************** */
/* ****************************************************************************/



/* ****************************************************************************/
/* ************************* BASIC LIN CONFIGURATION ************************ */
/* ****************************************************************************/


/* ****************************************************************************/
/*                          SNPD Configuration                                */
/* ****************************************************************************/
#define SNPD_PRESEL_PULLUPCURR ( 3u)
#define SNPD_SEL_PULLUPCURR    (19u)

/* ****************************************************************************/
/*                                  Bus Layer                                 */
/* ****************************************************************************/

  #define LINBUSIMP_CLK_FREQUENCY           24000000
  #define LINBUSIMP_BAUDRATE                19200
  #define LINBUSIMP_DEBOUNCER_VALUE         2  
  #define LINBUSIMP_SEND_HEADER_BREAK_LEN   13
  #define LINBUSIMP_DEFAULT_IGNORE_MSGTOUT  0x00u /**< Default ignore message timeout value. */

  #define VIC_IF_FUNS                   &vic_Implementation_InterfaceFunctions

/* ****************************************************************************/
/*                                Lookup Layer                                */
/* ****************************************************************************/

/* ****************************************************************************/
/*                               Protocol Layer                               */
/* ****************************************************************************/
  #define LINPROTOIMP_MSG_BUF_LEN           8u

/* ****************************************************************************/
/*                              Transport Layer                               */
/* ****************************************************************************/

  #define LINTRANSIMP_MSG_BUF_LEN         60u


/* ****************************************************************************/
/*                               Diagnose Layer                               */
/* ****************************************************************************/
  #define LINDIAGIMP_INITIAL_NAD          (1u)
  #define LINDIAGIMP_REQST_CH_FRM_ID      (60u)
  #define LINDIAGIMP_RESP_CH_FRM_ID       (61u)
  #define LINDIAGIMP_REQST_CH_MSG_SZE     (8u)
  #define LINDIAGIMP_RESP_CH_MSG_SZE      (8u)
  #define LINDIAGIMP_REQST_CH_CRC_TYPE    (LinProtoIf_CRC_Classic)
  #define LINDIAGIMP_RESP_CH_CRC_TYPE     (LinProtoIf_CRC_Classic)
  #define LINDIAGIMP_NAS_TIMEOUT          (1000u)
  #define LINDIAGIMP_NCR_TIMEOUT          (1000u)

  #define LINDIAGIMP_SUPPLIERID           0x0084u
  #define LINDIAGIMP_FUNCTIONID           0x0003u
  #define LINDIAGIMP_VARIANTID            0x39u
  #define LINDIAGIMP_SERIALNUMBER         (0x00000000ULL+LINDIAGIMP_INITIAL_NAD)

  #define LINDRVIMP_DIAG_USED_IF_VERSION     LINDIAG_INTERFACE_MODULE_API_VERSION

  #define LINDRVIMP_DIAG_CALLBACK_CTX_DATA  LIN_NULL
  
/* ****************************************************************************/
/* ************************* CALLBACK CONFIGURATION ************************* */
/* ****************************************************************************/

/* ****************************************************************************/
/*                                  Bus Layer                                 */
/* ****************************************************************************/

/* ****************************************************************************/
/*                                Lookup Layer                                */
/* ****************************************************************************/

/* ****************************************************************************/
/*                               Protocol Layer                               */
/* ****************************************************************************/

  #define LINDRVIMP_PROTO_USED_IF_VERSION    LINPROTO_INTERFACE_MODULE_API_VERSION
  #define LINDRVIMP_PROTO_CALLBACK_CTX_DATA  LIN_NULL


/* ****************************************************************************/
/*                              Transport Layer                               */
/* ****************************************************************************/

/* ****************************************************************************/
/*                               Diagnose Layer                               */
/* ****************************************************************************/



/* ****************************************************************************/
/* ********************** ADVANCED DRIVER CONFIGURATION ********************* */
/* ****************************************************************************/

/* ****************************************************************************/
/*                                  Bus Layer                                 */
/* ****************************************************************************/

/* ****************************************************************************/
/*                                Lookup Layer                                */
/* ****************************************************************************/

/* ****************************************************************************/
/*                               Protocol Layer                               */
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

extern LinBusIf_cpInterfaceFunctions_t    LinDrvImp_BusIfFuns;
extern LinLookupIf_cpInterfaceFunctions_t LinDrvImp_LookupIfFuns;
extern LinProtoIf_cpInterfaceFunctions_t  LinDrvImp_ProtoIfFuns;
extern LinTransIf_cpInterfaceFunctions_t  LinDrvImp_TransIfFuns;
extern eepromIf_cpInterfaceFunctions_t    EEImp_IfFuns;
extern LinDiagIf_cpInterfaceFunctions_t   LinDrvImp_DiagIfFuns;
extern flashIf_cpInterfaceFunctions_t     FlashImp_IfFuncs;

#pragma data_alignment=2
extern Lin_uint8_t LinDrvImp_BusEnvData[LINBUSIMP_ENVIRONMENT_DATA_SZE];

#pragma data_alignment=2
extern Lin_uint8_t LinDrvImp_LookupEnvData[LINDRV_LOOKUP_SEQ_ENVIRONMENT_DATA_SZE];

#pragma data_alignment=2
extern Lin_uint8_t LinDrvImp_ProtoEnvData[LINPROTOIMP_ENVIRONMENT_DATA_SZE(LINPROTOIMP_MSG_BUF_LEN)];

#pragma data_alignment=2
extern Lin_uint8_t LinDrvImp_TransEnvData[LINTRANSIMP_ENVIRONMENT_DATA_SZE(LINTRANSIMP_MSG_BUF_LEN,0,0)];

#pragma data_alignment=2
extern Lin_uint8_t LinDrvImp_DiagEnvData[LINDIAGIMP_ENVIRONMENT_DATA_SZE];

extern Lin_Bool_t LinDrvImp_Diag_triggerNADStorage;

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/* **************************************************************************//**
 * @implementation
 * Initialization of the LIN stack.
 *
 * @param   none
 * @return  TRUE        LIN driver initialization was successfull,
 *          FALSE       else
 *
 * This function has to be called once at the beginning of the main application.
 * *****************************************************************************/
Lin_Bool_t LinDrvImp_Init(void);


/* **************************************************************************//**
 * @implementation
 * generic Task routine.
 *
 * @param       none
 * @return      void
 *
 * This function is intended to be called regularly in the main application loop.
 * *****************************************************************************/
void LinDrvImp_Task(void);


/**
*@details   LIN common communication initialization.
*
*@params[in] newNad new single address.
*
*@retval    None.
*/
void systemLinCommChangeNad(uint8_t newNad);   // LJM Added

Lin_Bool_t LINTest_LJM(void);   // LJM Added

#endif /* LINDRVIMP_CONFIG_H_ */

/*! @} */ /* SAMPLE_CONFIG */
/*! @} */ /* SAMPLE_PROJECT */

