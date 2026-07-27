/***************************************************************************//**
 * @file      linsci_InterruptHandler.c
 *
 * @creator    poe
 * @created    13.05.2020
 * @sdfv       Elmos Flow - Grade 2
 * @project    521.39
 * @version    $Revision: 9263 $
 *
 * @brief      Interrupt handler for LIN PHY control module
 *
 * @purpose    Implements interrupt handler for LIN PHY control module
 *
 * $Id: linsci_InterruptHandler.c 9263 2021-09-29 13:42:10Z poe $
 *
 * $Revision: 9263 $
 *

 ******************************************************************************
 *
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
 * $Id: linsci_InterruptHandler.c 9263 2021-09-29 13:42:10Z poe $
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
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"
#include "vic_InterruptHandler.h"
#include "linsci_InterruptHandler.h"

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
__interrupt void loc_linsci_InterruptHandler(void);
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
 * to sci_SetIRQVEnable, that a special enum (sci_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void linsci_InterruptEnable(linsci_eInterruptVectorNum_t irqvecnum)
{
  LINSCI_IRQ_VENABLE = (uint16_t) irqvecnum;
}

/***************************************************************************//**
 * @implementation
 * Simple word write access to IRQ_VDISABLE register, but with the difference
 * to sci_SetIRQVDisable, that a special enum (sci_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void linsci_InterruptDisable(linsci_eInterruptVectorNum_t irqvecnum)
{
  LINSCI_IRQ_VDISABLE = (uint16_t) irqvecnum;
}

/***************************************************************************//**
 * @implementation
 * At first the module number is looked up in a table, because module number
 * and module instance base address have a logical connection. Then the
 * interrupt callback function pointer is saved in module interrupt vector
 * table.
 *
 ******************************************************************************/
/* PRQA S 1503 3 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
/* PRQA S 1532 1 #justification: Function is being used to register interrupts and is referenced in project in different files.*/
void linsci_InterruptRegisterCallback(linsci_eInterruptVectorNum_t irqvecnum, linsci_InterruptCallback_t cbfnc)
{
  linsci_sInterruptEnvironmentData_t* environmentData = (linsci_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_SCI);
  
  if ((irqvecnum < linsci_INTERRUPT_VECTOR_CNT) && (environmentData != NULL)) 
  {
    environmentData->InterrupVectorTable[irqvecnum] = cbfnc;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Reverse function of "sci_RegisterInterruptCallback". Sets the corresponding
 * entry in the module interrupt vector table back to NULL. Very imported,for
 * security reasons the corresponding interrupt is disabled. Otherwise the
 * program could possibly call NULL.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void linsci_InterruptDeregisterCallback(linsci_eInterruptVectorNum_t irqvecnum)
{
  linsci_sInterruptEnvironmentData_t* environmentData = (linsci_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_SCI);

  if ((irqvecnum < linsci_INTERRUPT_VECTOR_CNT) && (environmentData != NULL)) 
  {
    linsci_InterruptDisable(irqvecnum);
    environmentData->InterrupVectorTable[irqvecnum] = NULL;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Every hardware SCI module has it's own interrupt handler, also because each
 * module has it's own interrupt vector. So the module base address is fixed.
 * The interrupt vector table is provided by the user-application from RAM. The
 * module specific vector tables lie behind the VIC interrupt vector table, which
 * address is saved in the "TABLE_BASE". The module interrupt handler
 * gets the address from the function "vic_GetPointerToEviornmentData".
 * The interrupt vector number is saved in a local variable, because the value
 * of IRQ_VECTOR could possible change during processing of
 * sci_SCI0_InterruptHandler. Next the registered interrupt callback function
 * (sci_RegisterInterruptCallback) is copied into a local function pointer,
 * checked if it's not NULL and at least called. At the end the interrupt
 * request flag is cleared, by writing back the interrupt vector number to
 * IRQ_VNO register.
 *
 ******************************************************************************/
__interrupt void loc_linsci_InterruptHandler(void)
{
  linsci_eInterruptVectorNum_t irqvecnum = (linsci_eInterruptVectorNum_t) LINSCI_IRQ_VNO;

  if (irqvecnum < linsci_INTERRUPT_VECTOR_CNT) /* ensure the IRQ trigger is not already gone away until processing starts */
  {  
    linsci_cpInterruptEnvironmentData_t environmentData = (linsci_cpInterruptEnvironmentData_t) vic_GetPointerToEnvironmentData(vic_IRQ_SCI);

    linsci_InterruptCallback_t fptr;
  
    uint16_t vicVmaxBackup  = 0u;
    uint16_t sciVmaxBackup  = 0u;
  
    /* IRQ nesting on VIC level: enabled by default */
    vicVmaxBackup = VIC_IRQ_VMAX;
    VIC_IRQ_VMAX = (uint16_t)vic_IRQ_SCI;

    /* IRQ nesting on module level: by default disabled, handler may override this later on */
    sciVmaxBackup = LINSCI_IRQ_VMAX;
    LINSCI_IRQ_VMAX = 0u;
    
    /*-->2020-06-03, poe: ESWBL-52
     * Moved before calling the handler to prevent event skipping, */
    /* Clear interrupt request flag */
    LINSCI_IRQ_VNO = (uint16_t) irqvecnum;
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
      LINSCI_IRQ_VDISABLE = (uint16_t) irqvecnum;
    }
  
    /* IRQ nesting on mocule level */
    LINSCI_IRQ_VMAX = sciVmaxBackup;
  
    /* IRQ nesting on VIC level */
    VIC_IRQ_VMAX = vicVmaxBackup;
  }
  else {}
      
  /* RETI will reenable IRQs here */ 
}

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
/* PRQA S 1532++ #justification: Function is being used to initialize interrupts and is referenced in different projects via API. */
void linsci_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, linsci_pInterruptEnvironmentData_t environmentdata, linsci_pInterruptContextData_t contextdata)
/* PRQA S 1532-- */
{
  if ((vicIf != NULL) && (vicIf->InterfaceVersion == (uint16_t) VIC_INTERFACE_VERSION) && (environmentdata != NULL))
  {
    environmentdata->ContextData = contextdata;

    /* Register module at interrupt handler */
    vicIf->RegisterModule(vic_IRQ_SCI, &loc_linsci_InterruptHandler, (void*) environmentdata);
    vicIf->EnableModule(vic_IRQ_SCI);
  }
  else {}
  
}
