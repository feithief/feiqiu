/***************************************************************************//**
 * @file			vic_Implementation.h
 *
 * @creator		poe
 * @created		13.06.2020
 * @sdfv        Elmos Flow - Grade 2 *
 *
 * @brief       Interrupt handler for the VIC
 *
 * @purpose     Implements interrupt handler for the VIC
 *
 *
 * $Id: vic_InterruptHandler.h 9258 2021-09-28 17:30:48Z poe $
 *
 * $Revision: 9258 $
 *
 * ******************************************************************************
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

#ifndef VIC_INTERRUPTHANDLER_H_
#define VIC_INTERRUPTHANDLER_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "vic_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
extern const vic_sInterfaceFunctions_t vic_InterfaceFunctions;

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * Returns current module specific environment data
 *
 * @param module     Module number
 * 
 * @return           Pointer to module data (may be NULL)
 *
 ******************************************************************************/
vic_pInterruptModuleEnvironmentData_t vic_GetPointerToEnvironmentData(vic_eInterruptVectorNum_t module);

/***************************************************************************//**
 * Introduces a module handler for a particular module.
 *
 * @param module                  Module number
 * @param interrupthandler        Pointer to module specific interrupt handler
 * @param moduleenvironmentdata   Pointer to module specific interrupt handler runtime data
 *
 * A default IRQ handler is installed for the selected module.
 *
 ******************************************************************************/
void vic_RegisterModule(vic_eInterruptVectorNum_t    module, vic_InterruptCallback_t interrupthandler,
                                                     vic_cpInterruptModuleEnvironmentData_t moduleenvironmentdata);

/***************************************************************************//**
 * Removes module handler for a particular module.
 *
 * @param module                  Module number
 *
 * A default IRQ handler is installed for the selected module.
 *
 ******************************************************************************/
void vic_DeregisterModule(vic_eInterruptVectorNum_t    module);

/***************************************************************************//**
 * Enables interrupt processing for specified module.
 *
 * @param moduleIRQNum                  Module number
 *
 ******************************************************************************/
void vic_EnableModule(vic_eInterruptVectorNum_t moduleIRQNum);

/***************************************************************************//**
 * Disables interrupt processing for specified module.
 *
 * @param moduleIRQNum                  Module number
 *
 ******************************************************************************/
void vic_DisableModule(vic_eInterruptVectorNum_t moduleIRQNum);

/***************************************************************************//**
 * Enables general interrupt processing.
 *
 * @param none
 *
 ******************************************************************************/
void vic_EnableMain(void);

/***************************************************************************//**
 * Disables general interrupt processing.
 *
 * @param none
 *
 ******************************************************************************/
void vic_DisableMain(void);

/***************************************************************************//**
 * @brief Initialize VIC module
 *
 * @param environmentdata  Pointer to Environment data for VIC module in
 *                         user RAM
 *
 * @pre        VIC (vic_VectorInterruptControl) and SYSS (syss_SystemStateModule)
 *             have to present and initialized.
 *
 * @post       VIC module is configured for use.
 * 
 *
 ******************************************************************************/
void vic_IRQInitialisation(vic_pInterruptEnvironmentData_t penvironmentdata);


#endif /* VIC_INTERRUPTHANDLER_H_ */
