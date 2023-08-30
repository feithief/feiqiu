/***************************************************************************//**
 * @file      CommonInterruptHandler.c
 *
 * @creator     sbai
 * @created     08.05.2015
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief       Common Interrupt handler for interrupt controller implementation.
 *
 * @purpose     Common Interrupt handler for interrupt controller implementation.
 *
 * $Id: CommonInterruptHandler.c 8389 2021-02-23 09:41:13Z poe $
 *
 * $Revision: 8389 $
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

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"
#include "CommonInterruptHandler.h"

#include "vic_InterruptHandler.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/
/* See justification at the file comment header */
// PRQA S 3218 ++
static vic_sInterruptEnvironmentData_t loc_VicEnvironmentData;
// PRQA S 3218 --

/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Initialize interrupt handler
 *
 * @param nvicIf LIN driver interface function pointer
 ******************************************************************************/
// PRQA S 1532 1 #justification(1532): following function is used to initialize interrupt handler, is used in the project externally.
void InterruptHandlerInitialize(vic_cpInterfaceFunctions_t vicIf)
{
  if ((vicIf != NULL) && (vicIf->InterfaceVersion == VIC_INTERFACE_VERSION))
  {    
    /* Initialize environment data with 0 */
    el_FillBytes(0u, (uint8_t*) &loc_VicEnvironmentData, sizeof(vic_sInterruptEnvironmentData_t));

    /* Init global IRQ handling */ 
    vicIf->IRQInitialisation(&loc_VicEnvironmentData);
  
    /* Enable all IRQs at VIC level */
    vicIf->EnableMain();
  }
  else { }
}

