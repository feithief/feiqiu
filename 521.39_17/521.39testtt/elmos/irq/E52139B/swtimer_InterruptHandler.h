/***************************************************************************//**
 * @file       swtimer_InterruptHandler.h
 *
 * @creator		poe
 * @created		10.02.2021
 * @sdfv        Elmos Flow - Grade 2
 *
 * @brief		Module interrupt handler for SWTIMER hardware module.
 *
 * @purpose
 *
 * This module provides functions for handling SWTIMER interrupts
 * - Interrupt initialization
 * - Interrupt enabling
 * - Interrupt disabling
 * - Registering user callback function for interrupt handling
 * - De-registering user callback function for interrupt handling
 * - The interrupt handler itself, which provides the interrupt handling
 *   in hardware and calls the user callback function for functional 
 *   interrupt handling
 *
 * $Id: swtimer_InterruptHandler.h 9392 2021-11-18 09:45:59Z poe $
 *
 * $Revision: 9392 $
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

#ifndef SWTIMER_INTERRUPTHANDLER_H_          
#define SWTIMER_INTERRUPTHANDLER_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_types.h"
#include "vic_Interface.h"

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * SWTIMER IRQ vector numbers
 ******************************************************************************/
typedef enum 
{
  swtimer_IRQ_EVT_CNT_ZERO_0               = 0u,
  swtimer_IRQ_EVT_CNT_ZERO_1               = 1u,
  swtimer_IRQ_EVT_DIV_ZERO_0               = 2u,
  swtimer_IRQ_EVT_DIV_ZERO_1               = 3u,
  swtimer_INTERRUPT_VECTOR_CNT             = 4u  /**< Number of available interrupt vectors */
}swtimer_eInterruptVectorNum_t;
/***************************************************************************//**
 * Pointer to SWTIMER context data
 ******************************************************************************/
typedef void * swtimer_pInterruptContextData_t;

/***************************************************************************//**
 * Callback function pointer type
 ******************************************************************************/
typedef void (*swtimer_InterruptCallback_t) (swtimer_eInterruptVectorNum_t irqvecnum, swtimer_pInterruptContextData_t contextdata);

/***************************************************************************//**
 * SWTIMER environment data
 ******************************************************************************/
/* PRQA S 3630 2 #justification: this struct is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */   
typedef struct swtimer_sInterruptEnvironmentData
{
    /** Interrupt vector table of this module */
    swtimer_InterruptCallback_t InterrupVectorTable[swtimer_INTERRUPT_VECTOR_CNT];
    
    /** SWTIMER module context data */
    swtimer_pInterruptContextData_t ContextData;
    
} swtimer_sInterruptEnvironmentData_t;

/***************************************************************************//**
 * Pointer to SWTIMER environment data
 ******************************************************************************/
typedef swtimer_sInterruptEnvironmentData_t * swtimer_pInterruptEnvironmentData_t;

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ******************************* INTERRUPTS *********************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief     Enables an IRQ.
 *
 * @param[in] modulBaseAddress  Pointer to SWTIMER module base address
 * @param[in] irqsrc            IRQ to be enabled
 *
 * @pre       A call back function to the related interrupt should have been
 *            registered with swtimer_RegisterInterruptCallback().
 *
 * @post      The related call back function will be called if the desired
 *            interrupt occurs.
 *
 * @detaildesc
 * The SWTIMER IRQ_VENABLE register will be set the related IRQ number and therefore
 * the interrupt will be activated.
 *
 ******************************************************************************/
void swtimer_InterruptEnable(swtimer_eInterruptVectorNum_t irqsrc);

/***************************************************************************//**
 * @brief Disables an IRQ.
 *
 * @param[in] modulBaseAddress  Pointer to SWTIMER module base address
 * @param[in] irqsrc            IRQ to be disable
 *
 * @post      The interrupt will be disabled and the related callback function
 *            will no longer be called from the interrupt handler.
 *
 * @detaildesc
 * The SWTIMER IRQ_VDISABLE register will be set the related IRQ number and therefore
 * the interrupt will be deactivated.
 *
 ******************************************************************************/
void swtimer_InterruptDisable(swtimer_eInterruptVectorNum_t irqsrc);

/***************************************************************************//**
 * @brief Registers/Adds callback function to the module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to SWTIMER module base address
 * @param irqsrc            IRQ number
 * @param cbfnc             Pointer to desired callback function
 *
 * @pre     (optional) Which are the conditions to call this function? i.e. none
 *
 * @post    If the interrupt will be activated the registered callback function
 *          will be called if the IRQ occurs.
 *
 * @detaildesc
 * Registers the callback function at interrupt vector handling. It sets the
 * entry in the interrupt vector table to passed function pointer.
 *
 ******************************************************************************/
void swtimer_InterruptRegisterCallback(swtimer_eInterruptVectorNum_t irqvecnum, swtimer_InterruptCallback_t cbfnc);

/***************************************************************************//**
 * @brief Deregisters/deletes callback function from module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to SWTIMER module base address
 * @param irqvecnum         IRQ number
 * @param cbfnc             Pointer to desired callback function
 *
 * @pre   The related IRQ should be disabled.
 *
 * @post  The entry in the module interrupt vector table will point to NULL and
 *        the related IRQ will be disabled.
 *
 * @detaildesc
 * Deregisters the callback function from interrupt vector handling. It sets the
 * entry in the interrupt vector table to NULL and disables the related interrupt.
 *
 ******************************************************************************/
void swtimer_InterruptDeregisterCallback(swtimer_eInterruptVectorNum_t irqvecnum);

/***************************************************************************//**
 * @brief Initialize SWTIMER module
 *
 * @param environmentdata  Pointer to Environment data for SWTIMER module in
 *                         user RAM
 *
 * @pre        VIC (vic_VectorInterruptControl) and SWTIMER (swtimer_SystemStateModule)
 *             have to presented and initialized.
 *
 * @post       SWTIMER module is configured for use.
 *
 * @detaildesc
 * Initializes the SWTIMER software and hardware module, including the module
 * interrupt vector table. Configures if IRQ nesting is active and if IO2 and
 * IO3 are used as SWTIMERs or not.
 *
 ******************************************************************************/
void swtimer_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, swtimer_pInterruptEnvironmentData_t environmentdata, swtimer_pInterruptContextData_t contextdata);


#endif /* SWTIMER_INTERRUPTHANDLER_H_ */

