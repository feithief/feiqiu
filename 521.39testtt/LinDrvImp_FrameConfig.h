/***************************************************************************//**
 * @file      LinDrvImp_FrameConfig.h
 *
 * @creator   poe
 * @created   2021.02.23 
 *
 * @brief     LIN driver custom frame id and handler definition. 
 *
 * @purpose   Custom frame id description and handling is done here.
 *            
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
 * $Id: LinDrvImp_FrameConfig.h 9596 2022-01-19 14:17:54Z poe $
 *
 * $Revision: 9596 $
 *
 ******************************************************************************/

#ifndef LINDRVIMP_FRAMECONFIG_H_
#define LINDRVIMP_FRAMECONFIG_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDrvImp_Config.h"
/* ****************************************************************************/
/* ***************************** USER INCLUDES ********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

  #define LINPROTOIMP_FRAMEID_CNT           2                                   /*<< number of custom defined frame ids */
  #define LINPROTOIMP_FRAMEDESCLST_SZE      LINPROTOIMP_FRAMEID_CNT + 1         /*<< actual frame description list size */
extern LinProtoIf_sFrameDescription_t LinDrvImp_FrameDescriptionList[LINPROTOIMP_FRAMEDESCLST_SZE];
  #define LINDRVIMP_PROTO_FRM_DESC_LST_CB_CTX_DATA  LIN_NULL

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** SIGNAL DEFINITIONS ************************** */
/* ****************************************************************************/



/* ****************************************************************************/
/* ************************* SIGNAL CONFIGRUATIONS ************************** */
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ FRAME DESCRIPTION LIST ************************** */
/* ****************************************************************************/
extern LinProtoIf_sFrameDescription_t LinDrvImp_FrameDescriptionList[LINPROTOIMP_FRAMEDESCLST_SZE];   /**< frame description list containing customized frame ids */

#define KBDSIDNum  (6)
//extern const LinTransIf_sSIDDescription_t LinDrvImp_SIDDescrLst[3];
extern const LinTransIf_sSIDDescription_t LinDrvImp_SIDDescrLst[KBDSIDNum];


extern const LinDiagIf_sRbiLookupEntry_t LinDrvImp_RbiTbl[2];
extern Lin_Bool_t LinDrvImp_ResponseErrorSignal;

extern uint16_t eepromStoreNADValue;
extern uint8_t eepromStoreNADFlag;
extern uint16_t eepromReadTest;
extern uint8_t masterSendBuffer[8];
extern uint8_t masterReadBuffer[8];




#endif /* LINDRVIMP_FRAMECONFIG_H_ */
