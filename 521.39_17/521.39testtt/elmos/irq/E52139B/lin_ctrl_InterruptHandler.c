/***************************************************************************//**
 * @file			lin_ctrl_InterruptHandler.c
 *
 * @creator    poe
 * @created    23.01.2021
 * @sdfv       Elmos Flow - Grade 2
 * @project    521.39
 * @version    $Revision: 9392 $
 *
 * @brief      Interrupt handler for LIN PHY control module
 *
 * @purpose    Implements interrupt handler for LIN PHY control module
 *
 * $Id: lin_ctrl_InterruptHandler.c 9392 2021-11-18 09:45:59Z poe $
 *
 * $Revision: 9392 $
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
 * ****************************************************************************
 *
 * @section history_sec_lin_ctrl_InterruptHandler_c Revision History
 * $Id: lin_ctrl_InterruptHandler.c 9392 2021-11-18 09:45:59Z poe $
 * @verbatim
 * -------------------------------------------------------------------
 * Date       B/F Who Ver  Comment
 * -------------------------------------------------------------------
 * 2020/05/13 (F) POE 1.0 first creation
 * -------------------------------------------------------------------
 * @endverbatim
 *
 *****************************************************************************/
 
/* ****************************************************************************/
/* ******************** Helix QAC, MISRA Rule Suppression ***************************/
/* ****************************************************************************/
/* PRQA S 1505 EOF #justification(1505): This is an interface. Functions are being 
                   used from outside depending on need Clients also need to use the 
                   interface*/

/* PRQA S 0303, 0306 EOF #justification: This module addresses HW-register that are 
                         representet by numeric numbers. In this case we have to 
                         convert this numeric numbers to a pointer! */
/* Rule: 0303,0306 MISRA a Rule-11.4
 A conversion should not be performed between a pointer to object and an integer type
 Circumstances: memory mapped I/O ports must be addressed with this conversion
                    in embedded programming.
 Potential consequences: none
 Alternative safety assurance: none */

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include <el_helper.h>
#include "vic_InterruptHandler.h"
#include "lin_ctrl_InterruptHandler.h"

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
 * Handles the LIN_CTRL related interrupt requests.
 ******************************************************************************/
 __interrupt static void loc_lin_ctrl_InterruptHandler(void);

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
 * Enables an IRQ.
 *
 * @param[in] irqsrc            IRQ to be enabled
 *
 * @pre       A call back function to the related interrupt should have been
 *            registered with sci_RegisterInterruptCallback().
 *
 * @post      The related call back function will be called if the desired
 *            interrupt occurs.
 *
 * @implementation
 * Simple word write access to IRQ_VENABLE register, but with the difference
 * to lin_ctrl_SetIRQVEnable, that a special enum (lin_ctrl_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
// justification(1503): following function is being used externally
// PRQA S 1503 ++
void lin_ctrl_InterruptEnable(lin_ctrl_eInterruptVectorNum_t irqsrc)
// PRQA S 1503 --
{
  LIN_CTRL_IRQ_VENABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * Disables an IRQ.
 *
 * @param[in] irqsrc            IRQ to be disable
 *
 * @post      The interrupt will be disabled and the related callback function
 *            will no longer be called from the interrupt handler.
 *
 * @implementation
 * Simple word write access to IRQ_VDISABLE register, but with the difference
 * to lin_ctrl_SetIRQVDisable, that a special enum (lin_ctrl_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
void lin_ctrl_InterruptDisable(lin_ctrl_eInterruptVectorNum_t irqsrc)
{
  LIN_CTRL_IRQ_VDISABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * Registers/Adds callback function to the module interrupt vector table.
 *
 * @param irqvecnum         IRQ vector number
 * @param cbfnc             Pointer to desired callback function
 *
 * @pre     (optional) Which are the conditions to call this function? i.e. none
 *
 * @post    If the interrupt will be activated the registered callback function
 *          will be called if the IRQ occurs.
 *
 * @implementation
 * At first the module number is looked up in a table, because module number
 * and module instance base address have a logical connection. Then the
 * interrupt callback function pointer is saved in module interrupt vector
 * table.
 *
 ******************************************************************************/
// justification(1532): Function is being used to register interrupts. it is referenced in prject in different files. it is a normal operation
// PRQA S 1532 ++
void lin_ctrl_InterruptRegisterCallback( lin_ctrl_eInterruptVectorNum_t irqvecnum, lin_ctrl_InterruptCallback_t cbfnc)
// PRQA S 1532 --
{
  lin_ctrl_sInterruptEnvironmentData_t* environmentData = (lin_ctrl_sInterruptEnvironmentData_t*)  vic_GetPointerToEnvironmentData(vic_IRQ_LIN_CTRL);

  if ( ( irqvecnum < lin_ctrl_INTERRUPT_VECTOR_CNT ) && ( environmentData != NULL) ) 
  {
    environmentData->InterrupVectorTable[irqvecnum] = cbfnc;
  }
  else { /* empty. */ }
}

/***************************************************************************//**
 * Deregisters/deletes callback function from module interrupt vector table.
 *
 * @param irqvecnum         IRQ number
 *
 * @pre   The related IRQ should be disabled.
 *
 * @post  The entry in the module interrupt vector table will point to NULL and
 *        the related IRQ will be disabled.
 *
 * @implementation
 * Reverse function of "lin_ctrl_RegisterInterruptCallback". Sets the corresponding
 * entry in the module interrupt vector table back to NULL. Very imported,for
 * security reasons the corresponding interrupt is disabled. Otherwise the
 * program could possibly call NULL.
 *
 ******************************************************************************/
// justification(1532): Function is being used to deregister interrupts.it is not used now but will be used 
// PRQA S 1503 ++
void lin_ctrl_InterruptDeregisterCallback(lin_ctrl_eInterruptVectorNum_t irqvecnum)
// PRQA S 1503 --
{
  lin_ctrl_sInterruptEnvironmentData_t* environmentData = (lin_ctrl_sInterruptEnvironmentData_t*)  vic_GetPointerToEnvironmentData(vic_IRQ_LIN_CTRL);

  if ( ( irqvecnum < lin_ctrl_INTERRUPT_VECTOR_CNT) && ( environmentData != NULL ) ) 
  {
    lin_ctrl_InterruptDisable(irqvecnum);
    environmentData->InterrupVectorTable[irqvecnum] = NULL;
  }
  else { /* empty. */ }
}

/***************************************************************************//**
 * Handles the LIN_CTRL related interrupt requests.
 *
 * Every hardware LIN_CTRL module has it's own interrupt handler, also because each
 * module has it's own interrupt vector. So the module base address is fixed.
 * The interrupt vector table is provided by the user-application from RAM. The
 * module specific vector tables lie behind the VIC interrupt vector table, which
 * address is saved in the "TABLE_BASE". The module interrupt handler
 * gets the address from the function "vic_GetPointerToEviornmentData".
 * The interrupt vector number is saved in a local variable, because the value
 * of IRQ_VNO could possible change during processing of
 * InterruptHandler. Next the registered interrupt callback function
 * (lin_ctrl_RegisterInterruptCallback) is copied into a local function pointer,
 * checked if it's not NULL and at least called. At the end the interrupt
 * request flag is cleared, by writing back the interrupt vector number to
 * IRQ_VNO register.
 *
 ******************************************************************************/
 __interrupt static void loc_lin_ctrl_InterruptHandler(void)
{
  lin_ctrl_eInterruptVectorNum_t irqvecnum = (lin_ctrl_eInterruptVectorNum_t) LIN_CTRL_IRQ_VNO;
  
  if (irqvecnum < lin_ctrl_INTERRUPT_VECTOR_CNT) /* ensure the IRQ trigger is not already gone away until processing starts */
  {
    lin_ctrl_sInterruptEnvironmentData_t const* environmentData = (lin_ctrl_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_LIN_CTRL);

    lin_ctrl_InterruptCallback_t fptr;
  
    uint16_t vicVmaxBackup = 0u;
    uint16_t lin_ctrlVmaxBackup = 0u;
  
    /* IRQ nesting on VIC level: enabled by default */
    vicVmaxBackup = VIC_IRQ_VMAX;
    VIC_IRQ_VMAX = (uint16_t)vic_IRQ_LIN_CTRL;

    /* IRQ nesting on module level: by default disabled, handler may override this later on */
    lin_ctrlVmaxBackup = LIN_CTRL_IRQ_VMAX;
    LIN_CTRL_IRQ_VMAX = 0;
    
    /*-->2020-06-03, poe: ESWBL-52
     * Moved here to prevent event skipping, */
    /* Clear interrupt request flag */
    LIN_CTRL_IRQ_VNO = (uint16_t) irqvecnum;
     /*-->2020-06-03, poe: ESWBL-52 */
  
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
      LIN_CTRL_IRQ_VDISABLE = (uint16_t) irqvecnum;
    }
 
    /* IRQ nesting on module level */
    LIN_CTRL_IRQ_VMAX = lin_ctrlVmaxBackup;
  
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
  * Initializes LIN_CTRL module.
  *
  * @param vicIf            Constant pointer to VIC interface
  * @param environmentData  Pointer to Environment data for SCI module in
  *                         user RAM
  * @param contextdata      Lin Ctrl context data
  *
  * @pre        VIC (vic_VectorInterruptControl) and LIN_CTRL (linctrl_SystemStateModule)
  *             have to presented and initialized.
  *
  * @post       LIN_CTRL module is configured for use.
  *
  * Initializes the LIN_CTRL software and hardware module, including the module
  * interrupt vector table. Configures if IRQ nesting is active or not.
  *
  ******************************************************************************/
/* PRQA S 1532++ #justification: Function is being used to initialize interrupts and is referenced in different projects via API. */
void lin_ctrl_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, lin_ctrl_pInterruptEnvironmentData_t environmentData, lin_ctrl_pInterruptContextData_t contextdata)
/* PRQA S 1532-- */
{
  if (( vicIf != NULL ) && ( vicIf->InterfaceVersion == (uint16_t) VIC_INTERFACE_VERSION ) && ( environmentData != NULL ) )
  {
    environmentData->ContextData = contextdata;

    /* Register module at interrupt handler */
    vicIf->RegisterModule(vic_IRQ_LIN_CTRL, &loc_lin_ctrl_InterruptHandler, (void*) environmentData);
    vicIf->EnableModule(vic_IRQ_LIN_CTRL);
  }
  else { /* empty. */ }

}
