/***************************************************************************//**
 * @file       gpio_InterruptHandler.h
 *
 * @creator    poe
 * @created    23.01.2021
 * @sdfv       Elmos Flow - Grade 2
 * @project    521.39 
 * @version    $Revision: 9392 $
 *
 * @brief Module interrupt handler for GPIO hardware module.
 *
 * @purpose
 * This module provides functions for handling GPIO interrupts
 * - Interrupt initialization
 * - Interrupt enabling
 * - Interrupt disabling
 * - Registering user callback function for interrupt handling
 * - De-registering user callback function for interrupt handling
 * - The interrupt handler itself, which provides the interrupt handling
 *   in hardware and calls the user callback function for functional 
 *   interrupt handling

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

#ifndef GPIO_INTERRUPTHANDLER_H_          
#define GPIO_INTERRUPTHANDLER_H_

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
 * GPIO IRQ vector numbers
 ******************************************************************************/
typedef enum {
  gpio_IRQ_EVT_POS_0               = 0u,
  gpio_IRQ_EVT_NEG_0               = 1u,
  gpio_IRQ_EVT_POS_1               = 2u,
  gpio_IRQ_EVT_NEG_1               = 3u,
  gpio_IRQ_EVT_POS_2               = 4u,
  gpio_IRQ_EVT_NEG_2               = 5u,
  gpio_IRQ_EVT_POS_3               = 6u,
  gpio_IRQ_EVT_NEG_3               = 7u,
  gpio_IRQ_EVT_POS_4               = 8u,
  gpio_IRQ_EVT_NEG_4               = 9u,
  
  gpio_INTERRUPT_VECTOR_CNT        = 10u  /**< Number of available interrupt vectors */
} gpio_eInterruptVectorNum_t;

/***************************************************************************//**
 * Pointer to GPIO context data
 ******************************************************************************/
typedef void * gpio_pInterruptContextData_t;

/***************************************************************************//**
 * Callback function pointer type
 ******************************************************************************/
typedef void (*gpio_InterruptCallback_t) (gpio_eInterruptVectorNum_t irqvecnum, gpio_pInterruptContextData_t contextdata);

/***************************************************************************//**
 * GPIO environment data
 ******************************************************************************/
typedef struct gpio_sInterruptEnvironmentData
{
    /** Interrupt vector table of this module */
    gpio_InterruptCallback_t InterrupVectorTable[gpio_INTERRUPT_VECTOR_CNT];
    
    /** GPIO module context data */
    gpio_pInterruptContextData_t ContextData;
    
} gpio_sInterruptEnvironmentData_t;

/***************************************************************************//**
 * Pointer to GPIO environment data
 ******************************************************************************/
typedef gpio_sInterruptEnvironmentData_t * gpio_pInterruptEnvironmentData_t;

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
 * @param[in] modulBaseAddress  Pointer to GPIO module base address
 * @param[in] irqsrc            IRQ to be enabled
 *
 * @pre       A call back function to the related interrupt should have been
 *            registered with gpio_RegisterInterruptCallback().
 *
 * @post      The related call back function will be called if the desired
 *            interrupt occurs.
 *
 * @detaildesc
 * The GPIO IRQ_VENABLE register will be set the related IRQ number and therefore
 * the interrupt will be activated.
 *
 ******************************************************************************/
void gpio_InterruptEnable(gpio_eInterruptVectorNum_t irqsrc);

/***************************************************************************//**
 * @brief Disables an IRQ.
 *
 * @param[in] modulBaseAddress  Pointer to GPIO module base address
 * @param[in] irqsrc            IRQ to be disable
 *
 * @post      The interrupt will be disabled and the related callback function
 *            will no longer be called from the interrupt handler.
 *
 * @detaildesc
 * The GPIO IRQ_VDISABLE register will be set the related IRQ number and therefore
 * the interrupt will be deactivated.
 *
 ******************************************************************************/
void gpio_InterruptDisable(gpio_eInterruptVectorNum_t irqsrc);

/***************************************************************************//**
 * @brief Registers/Adds callback function to the module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to GPIO module base address
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
void gpio_InterruptRegisterCallback(gpio_eInterruptVectorNum_t irqvecnum, gpio_InterruptCallback_t cbfnc);

/***************************************************************************//**
 * @brief Deregisters/deletes callback function from module interrupt vector table.
 *
 * @param modulBaseAddress  Pointer to GPIO module base address
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
void gpio_InterruptDeregisterCallback(gpio_eInterruptVectorNum_t irqvecnum);

/***************************************************************************//**
 * @brief Initialize GPIO module
 *
 * @param environmentdata  Pointer to Environment data for GPIO module in
 *                         user RAM
 *
 * @pre        VIC (vic_VectorInterruptControl) and GPIO (gpio_SystemStateModule)
 *             have to presented and initialized.
 *
 * @post       GPIO module is configured for use.
 *
 * @detaildesc
 * Initializes the GPIO software and hardware module, including the module
 * interrupt vector table. Configures if IRQ nesting is active and if IO2 and
 * IO3 are used as GPIOs or not.
 *
 ******************************************************************************/
void gpio_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, gpio_pInterruptEnvironmentData_t environmentdata, gpio_pInterruptContextData_t contextdata);


#endif /* GPIO_INTERRUPTHANDLER_H_ */

