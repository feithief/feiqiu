/***************************************************************************//**
 * @file      linsci_InterruptHandler.h
 *
 * @creator    poe
 * @created    13.05.2020
 * @sdfv       Elmos Flow - Grade 2
 * @project    521.39
 * @version    $Revision: 9258 $
 *
 * @brief      Interrupt handler for LIN PHY control module
 *
 * @purpose    Implements interrupt handler for LIN PHY control module
 *
 * $Id: linsci_InterruptHandler.h 9258 2021-09-28 17:30:48Z poe $
 *
 * $Revision: 9258 $
 *
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

#ifndef LINSCI_INTERRUPTHANDLER_H_          
#define LINSCI_INTERRUPTHANDLER_H_

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
 * SCI IRQ vector numbers
 ******************************************************************************/
typedef enum {
  linsci_IRQ_RXD_FALLING                   = 0u,
  linsci_IRQ_RXD_RISING                    = 1u,
  linsci_IRQ_SCI_TIMER_CMP                 = 2u,
  linsci_IRQ_SCI_TIMER_OV                  = 3u,
  linsci_IRQ_BUS_ERR                       = 4u,
  linsci_IRQ_RECEIVER_ERR                  = 5u,
  linsci_IRQ_HEADER_ERR                    = 6u,
  linsci_IRQ_BREAK_EVT                     = 7u,
  linsci_IRQ_SYNC_EVT                      = 8u,
  linsci_IRQ_PID_EVT                       = 9u,
  linsci_IRQ_RX_FIFO_FULL                  = 10u,
  linsci_IRQ_RX_DMA_FINISHED               = 11u,
  linsci_IRQ_TX_FIFO_EMPTY                 = 12u,
  linsci_IRQ_TX_DMA_FINISHED               = 13u,
  linsci_IRQ_TX_FINISH_EVT                 = 14u,
  linsci_IRQ_TICK_1MS                      = 15u,
  
  linsci_INTERRUPT_VECTOR_CNT              = 16u  /**< Number of available interrupt vectors */
         
} linsci_eInterruptVectorNum_t;

/***************************************************************************//**
 * Pointer to SCI context data
 ******************************************************************************/
typedef void * linsci_pInterruptContextData_t;

/***************************************************************************//**
 * Callback function pointer type
 ******************************************************************************/
typedef void (*linsci_InterruptCallback_t) (linsci_eInterruptVectorNum_t irqvecnum, linsci_pInterruptContextData_t contextdata);

/***************************************************************************//**
 * SCI environment data
 ******************************************************************************/
typedef struct linsci_sInterruptEnvironmentData
{
    /** Interrupt vector table of this module             */
    linsci_InterruptCallback_t InterrupVectorTable[linsci_INTERRUPT_VECTOR_CNT];
    
    /** SCI module context data */
    linsci_pInterruptContextData_t ContextData;
    
} linsci_sInterruptEnvironmentData_t;

/***************************************************************************//**
 * Pointer to SCI environment data
 ******************************************************************************/
typedef       linsci_sInterruptEnvironmentData_t * linsci_pInterruptEnvironmentData_t;
typedef const linsci_sInterruptEnvironmentData_t * linsci_cpInterruptEnvironmentData_t;

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
 * @param[in] modulBaseAddress  Pointer to SCI module base address
 * @param[in] irqvecnum         IRQ to be enabled
 *
 * @pre       A call back function to the related interrupt should have been
 *            registered with sci_RegisterInterruptCallback().
 *
 * @post      The related call back function will be called if the desired
 *            interrupt occurs.
 * 
 * The SCI IRQ_VENABLE register will be set the related IRQ number and therefore
 * the interrupt will be activated.
 *
 ******************************************************************************/
void linsci_InterruptEnable(linsci_eInterruptVectorNum_t irqvecnum);

/***************************************************************************//**
 * @brief Disables an IRQ.
 *
 * @param[in] modulBaseAddress  Pointer to SCI module base address
 * @param[in] irqvecnum         IRQ to be disable
 *
 * @post      The interrupt will be disabled and the related callback function
 *            will no longer be called from the interrupt handler.
 * 
 * The SCI IRQ_VDISABLE register will be set the related IRQ number and therefore
 * the interrupt will be deactivated.
 *
 ******************************************************************************/
void linsci_InterruptDisable(linsci_eInterruptVectorNum_t irqvecnum);

/***************************************************************************//**
 * @brief Registers/Adds callback function to the module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to SCI module base address
 * @param irqvecnum         IRQ number
 * @param cbfnc             Pointer to desired callback function
 *
 * @pre     (optional) Which are the conditions to call this function? i.e. none
 *
 * @post    If the interrupt will be activated the registered callback function
 *          will be called if the IRQ occurs.
 * 
 * Registers the callback function at interrupt vector handling. It sets the
 * entry in the interrupt vector table to passed function pointer.
 *
 ******************************************************************************/
void linsci_InterruptRegisterCallback(linsci_eInterruptVectorNum_t irqvecnum, linsci_InterruptCallback_t cbfnc);

/***************************************************************************//**
 * @brief Deregisters/deletes callback function from module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to SCI module base address
 * @param irqvecnum         IRQ number
 * @param cbfnc             Pointer to desired callback function
 *
 * @pre   The related IRQ should be disabled.
 *
 * @post  The entry in the module interrupt vector table will point to NULL and
 *        the related IRQ will be disabled.
 * 
 * Deregisters the callback function from interrupt vector handling. It sets the
 * entry in the interrupt vector table to NULL and disables the related interrupt.
 *
 ******************************************************************************/
void linsci_InterruptDeregisterCallback(linsci_eInterruptVectorNum_t irqvecnum);

/***************************************************************************//**
 * @brief Initialize SCI module
 *
 * @param environmentdata  Pointer to Environment data for SCI module in
 *                         user RAM
 *
 * @pre        VIC (vic_VectorInterruptControl) and SCI (sci_SystemStateModule)
 *             have to presented and initialized.
 *
 * @post       SCI module is configured for use.
 * 
 * Initializes the SCI software and hardware module, including the module
 * interrupt vector table. Configures if IRQ nesting is active and if IO2 and
 * IO3 are used as SCIs or not.
 *
 ******************************************************************************/
void linsci_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, linsci_pInterruptEnvironmentData_t environmentdata, linsci_pInterruptContextData_t contextdata);


#endif /* LINSCI_INTERRUPTHANDLER_H_ */

