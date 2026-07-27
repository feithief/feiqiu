/***************************************************************************//**
 * @file       gpio_InterruptHandler.c
 *
 * @creator    poe
 * @created    23.01.2021
 * @sdfv       Elmos Flow - Grade 2
 * @project    521.39 
 * @version    $Revision: 8389 $
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
 * $Id: gpio_InterruptHandler.c 8389 2021-02-23 09:41:13Z poe $
 *
 * $Revision: 8389 $
 *
 * *****************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the
 * integration in production systems, appliances or other installations is
 * prohibited.
 *
 * Disclaimer\:
 * Elmos Semiconductor SE shall not be liable for any damages arising out of
 * defects resulting from
 * (1) delivered hardware or software,
 * (2) non observance of instructions contained in this document, or
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone
 * other than Elmos Semiconductor SE. To the extend permitted by law
 * Elmos Semiconductor AG hereby expressively disclaims and user expressively
 * waives any and all warranties of merchantability and of fitness for a
 * particular purpose, statutory warranty of non-infringement and any other
 * warranty or product liability that may arise by reason of usage of trade,
 * custom or course of dealing.
 *
 * ****************************************************************************
 *
 * @section history_sec Revision History
 *
 * @verbatim
 * -------------------------------------------------------------------
 * Date     B/F Who Ver  Comment
 * -------------------------------------------------------------------
 * 21/01/23 (F) poe 1001 first creation
 * -------------------------------------------------------------------
 * @endverbatim
 *
 *****************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"

#include "vic_InterruptHandler.h"
#include "gpio_InterruptHandler.h"


/* ****************************************************************************/
/* ***************************** QA-C EXCLUDES ********************************/
/* ****************************************************************************/

/*  
* Exception\:
* Msg(2:1503) The function '...' is defined but is not used within this project.
*
* Circumstances and justification:
* The interrupt handling is provided as a hardware support API for all modules
* but may not be used for the actual project for a specific module. 
* 
* Potential consequences:
* None, just don't care about those functions
*
* Alternative safety assurance:
* None
*/
// PRQA S 1503 EOF


/*  
* Exception\:
* Msg(2:1505) The function '...' is only referenced in the translation unit
              where it is defined.
*
* Circumstances and justification:
* The interrupt handling is provided as a hardware support API for all modules
* but may not be used for the actual project for a specific module. The actual
* function is used within this module, but may or may not be used outside
* this module depending on the current application.
* 
* Potential consequences:
* None
*
* Alternative safety assurance:
* None
*/
// PRQA S 1505 EOF

/*  
* Exception\:
* Msg(2:0316) [I] Cast from a pointer to void to a pointer to object type.
*
* Circumstances and justification:
* nvic_GetPointerToEnvironmentData() returns a generic pointer to void for
* every module, which is locally casted to the actual module implementation.
* 
* Potential consequences:
* Data structure may not be as expected
*
* Alternative safety assurance:
* Message is only deactivated when accessing IRQ environment data. Correct
* usage has been checked when developing this module.
*/

/* PRQA S 0306 EOF #justification: This module addresses HW-register that are 
                   representet by numeric numbers. In this case we have to 
                   convert this numeric numbers to a pointer! */

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
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/***************************************************************************//**
 * Handles the GPIO related interrupt requests.
 *
 * @param       void
 *
 * @return      void
 *
 * @pre         
 *
 * Interrupt handler for interrupts of GPIO module. It gets the modul's
 * interrupt number and calls the corresponding application callback function, 
 * if there is one registered. If there is not application callback function
 * registered, the interrupt is disabled for the corresponding IRQ number.
 * Finally, the interrupt is cleared.
 *
 ******************************************************************************/
 __interrupt static void loc_gpio_InterruptHandler(void);

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ******************************* INTERRUPTS *********************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @implementation
 * Simple word write access to IRQ_VENABLE register, but with the difference
 * to gpio_SetIRQVEnable, that a special enum (gpio_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
void gpio_InterruptEnable(gpio_eInterruptVectorNum_t irqsrc)
{
  GPIO_IRQ_VENABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * @implementation
 * Simple word write access to IRQ_VDISABLE register, but with the difference
 * to gpio_SetIRQVDisable, that a special enum (gpio_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
void gpio_InterruptDisable(gpio_eInterruptVectorNum_t irqsrc)
{
  GPIO_IRQ_VDISABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * @implementation
 * At first the module number is looked up in a table, because module number
 * and module instance base address have a logical connection. Then the
 * interrupt callback function pointer is saved in module interrupt vector
 * table.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: This is an interface function and used outside of this file/project's scope. */ 
void gpio_InterruptRegisterCallback(gpio_eInterruptVectorNum_t irqvecnum, gpio_InterruptCallback_t cbfnc)
{
  gpio_sInterruptEnvironmentData_t* evironmentData = (gpio_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_GPIO);

  if ((irqvecnum < gpio_INTERRUPT_VECTOR_CNT) && (evironmentData != NULL)) 
  {
    evironmentData->InterrupVectorTable[irqvecnum] = cbfnc;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Reverse function of "gpio_RegisterInterruptCallback". Sets the corresponding
 * entry in the module interrupt vector table back to NULL. Very imported,for
 * security reasons the corresponding interrupt is disabled. Otherwise the
 * program could possibly call NULL.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: This is an interface function and just outside of this file/project's scope. */ 
void gpio_InterruptDeregisterCallback(gpio_eInterruptVectorNum_t irqvecnum)
{
  gpio_sInterruptEnvironmentData_t* evironmentData =  (gpio_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_GPIO);

  if ((irqvecnum < gpio_INTERRUPT_VECTOR_CNT) && (evironmentData != NULL)) 
  {
    gpio_InterruptDisable(irqvecnum);
    evironmentData->InterrupVectorTable[irqvecnum] = NULL;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Every hardware GPIO module has it's own interrupt handler, also because each
 * module has it's own interrupt vector. So the module base address is fixed.
 * The interrupt vector table is provided by the user-application from RAM. The
 * module specific vector tables lie behind the VIC interrupt vector table, which
 * address is saved in the "TABLE_BASE". The module interrupt handler
 * gets the address from the function "vic_GetPointerToEviornmentData".
 * The interrupt vector number is saved in a local variable, because the value
 * of IRQ_VECTOR could possible change during processing of
 * gpio_GPIO0_InterruptHandler. Next the registered interrupt callback function
 * (gpio_RegisterInterruptCallback) is copied into a local function pointer,
 * checked if it's not NULL and at least called. At the end the interrupt
 * request flag is cleared, by writing back the interrupt vector number to
 * IRQ_VNO register.
 *
 ******************************************************************************/
 __interrupt static void loc_gpio_InterruptHandler(void)
{
  gpio_eInterruptVectorNum_t irqvecnum = (gpio_eInterruptVectorNum_t) GPIO_IRQ_VNO;
  
  if (irqvecnum < gpio_INTERRUPT_VECTOR_CNT) /* ensure the IRQ trigger is not already gone away until processing starts */
  {
    gpio_sInterruptEnvironmentData_t const* environmentData = (gpio_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_GPIO);

    gpio_InterruptCallback_t fptr;
  
    uint16_t vicVmaxBackup  = 0u;
    uint16_t gpioVmaxBackup = 0u;
  
    /* IRQ nesting on VIC level: enabled by default */
    vicVmaxBackup = VIC_IRQ_VMAX;
    VIC_IRQ_VMAX = (uint16_t)vic_IRQ_GPIO;

    /* IRQ nesting on module level: by default disabled, handler may override this later on */
    gpioVmaxBackup = GPIO_IRQ_VMAX;
    GPIO_IRQ_VMAX = 0u;
    
    /*-->2020-09-14, poe: ESWBL-52
     * Moved here to prevent event skipping, */ 
    /* Clear interrupt request flag */
    GPIO_IRQ_VNO = (uint16_t) irqvecnum;
    
    /*-->2020-09-14, poe: ESWBL-52 */
  
    fptr = environmentData->InterrupVectorTable[irqvecnum];
          
    if(fptr != NULL)
    {          
      __enable_interrupt();
        
      /* Call interrupt callback function */
      fptr(irqvecnum,  (void*) environmentData->ContextData);      
      
      __disable_interrupt();
    }
    else 
    {
      /* if there is no handler function for a particular IRQ, disable this IRQ  */ 
      GPIO_IRQ_VDISABLE = (uint16_t) irqvecnum;
    }
  
    /* IRQ nesting on module level */
    GPIO_IRQ_VMAX = gpioVmaxBackup;
  
    /* IRQ nesting on VIC level */
    VIC_IRQ_VMAX = vicVmaxBackup;
  }
  else
  {
    /* No valid irq vector no. */
  }
  
  /* RETI will reenable IRQs here */ 
} 

/***************************************************************************//**
 * @implementation
 * First check, whether the interface to NVIC is set and the NVIC interface
 * version fits. Additionally, the module's interrupt environment data has to 
 * be set.
 * If the checks are passed, the module's interrupt handler and its environment
 * is registered in the NVIC.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: This is an interface function and just outside of this file/project's scope. */ 
void gpio_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, gpio_pInterruptEnvironmentData_t environmentdata, gpio_pInterruptContextData_t contextdata)
{
  if ((vicIf != (const void *)NULL) && (vicIf->InterfaceVersion == (uint16_t) VIC_INTERFACE_VERSION) && (environmentdata != NULL))
  {
    environmentdata->ContextData = contextdata;

    /* Register module at interrupt handler */
    vicIf->RegisterModule(vic_IRQ_GPIO, &loc_gpio_InterruptHandler, (void *) environmentdata);
    vicIf->EnableModule(vic_IRQ_GPIO);
  }
  else {}
}

