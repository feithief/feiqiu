/***************************************************************************//**
 * @file		swtimer_InterruptHandler.c
 *
 * @creator		poe
 * @created		26.01.2021
 * @sdfv       Elmos Flow - Grade 2
 * @brief      Interrupt handler for the SWTIMER HW module
 *
 * @purpose     Implements interrupt handler for the SWTIMER module.
 *
 * $Id: swtimer_InterruptHandler.c 9392 2021-11-18 09:45:59Z poe $
 *
 * $Revision: 9392 $
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
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "el_helper.h"
#include "vic_InterruptHandler.h"
#include "swtimer_InterruptHandler.h"

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
 * Handles the SWTIMER related interrupt requests.
 *
 * @param none
 * @return none
 *
 ******************************************************************************/
__interrupt void loc_swtimer_InterruptHandler(void);

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
 * to swtimer_SetIRQVEnable, that a special enum (swtimer_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void swtimer_InterruptEnable(swtimer_eInterruptVectorNum_t irqsrc)
{
  SWTIMER_IRQ_VENABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * @implementation
 * Simple word write access to IRQ_VDISABLE register, but with the difference
 * to swtimer_SetIRQVDisable, that a special enum (swtimer_eInterruptVectorNum_t)
 * is passed as parameter, for security reasons and a more concrete interface
 * definition.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void swtimer_InterruptDisable(swtimer_eInterruptVectorNum_t irqsrc)
{
  SWTIMER_IRQ_VDISABLE = (uint16_t) irqsrc;
}

/***************************************************************************//**
 * @implementation
 * At first the module number is looked up in a table, because module number
 * and module instance base address have a logical connection. Then the
 * interrupt callback function pointer is saved in module interrupt vector
 * table.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void swtimer_InterruptRegisterCallback(swtimer_eInterruptVectorNum_t irqvecnum, swtimer_InterruptCallback_t cbfnc)
{
  swtimer_sInterruptEnvironmentData_t* evironmentData = (swtimer_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_SWTIMER);

  if ((irqvecnum < swtimer_INTERRUPT_VECTOR_CNT) && (evironmentData != NULL)) 
  {
    evironmentData->InterrupVectorTable[irqvecnum] = cbfnc;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Reverse function of "swtimer_RegisterInterruptCallback". Sets the corresponding
 * entry in the module interrupt vector table back to NULL. Very imported,for
 * security reasons the corresponding interrupt is disabled. Otherwise the
 * program could possibly call NULL.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: because this is a generic linsci interrupt handler, this function is 
  part of this module although it is not used in this project */
void swtimer_InterruptDeregisterCallback(swtimer_eInterruptVectorNum_t irqvecnum)
{
  swtimer_sInterruptEnvironmentData_t* evironmentData =  (swtimer_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_SWTIMER);

  if ((irqvecnum < swtimer_INTERRUPT_VECTOR_CNT) && (evironmentData != NULL)) 
  {
    swtimer_InterruptDisable(irqvecnum);
    evironmentData->InterrupVectorTable[irqvecnum] = NULL;
  }
  else {}
}

/***************************************************************************//**
 * @implementation
 * Every hardware SWTIMER module has it's own interrupt handler, also because each
 * module has it's own interrupt vector. So the module base address is fixed.
 * The interrupt vector table is provided by the user-application from RAM. The
 * module specific vector tables lie behind the VIC interrupt vector table, which
 * address is saved in the "TABLE_BASE". The module interrupt handler
 * gets the address from the function "vic_GetPointerToEviornmentData".
 * The interrupt vector number is saved in a local variable, because the value
 * of IRQ_VECTOR could possible change during processing of
 * swtimer_SWTIMER0_InterruptHandler. Next the registered interrupt callback function
 * (swtimer_RegisterInterruptCallback) is copied into a local function pointer,
 * checked if it's not NULL and at least called. At the end the interrupt
 * request flag is cleared, by writing back the interrupt vector number to
 * IRQ_VNO register.
 *
 ******************************************************************************/
__interrupt void loc_swtimer_InterruptHandler(void)
{
  swtimer_eInterruptVectorNum_t irqvecnum = (swtimer_eInterruptVectorNum_t) SWTIMER_IRQ_VNO;
  
  if (irqvecnum < swtimer_INTERRUPT_VECTOR_CNT) /* ensure the IRQ trigger is not already gone away until processing starts */
  {
    swtimer_sInterruptEnvironmentData_t const* environmentData = (swtimer_sInterruptEnvironmentData_t*) vic_GetPointerToEnvironmentData(vic_IRQ_SWTIMER);

    swtimer_InterruptCallback_t fptr;
  
    uint16_t vicVmaxBackup  = 0u;
    uint16_t gpioVmaxBackup = 0u;
  
    /* IRQ nesting on VIC level: enabled by default */
    vicVmaxBackup = VIC_IRQ_VMAX;
    VIC_IRQ_VMAX  = (uint16_t)vic_IRQ_SWTIMER;

    /* IRQ nesting on module level: by default disabled, handler may override this later on */
    gpioVmaxBackup   = SWTIMER_IRQ_VMAX;
    SWTIMER_IRQ_VMAX = 0u;
    
    /*-->2020-09-14, poe: ESWBL-52
     * Moved here to prevent event skipping, */ 
    /* Clear interrupt request flag */
    SWTIMER_IRQ_VNO = (uint16_t) irqvecnum;
    
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
      SWTIMER_IRQ_VDISABLE = (uint16_t) irqvecnum;
    }
  
    /* IRQ nesting on mocule level */
    SWTIMER_IRQ_VMAX = gpioVmaxBackup;
  
    /* IRQ nesting on VIC level */
    VIC_IRQ_VMAX = vicVmaxBackup;
  }
  else {}
      
  /* RETI will reenable IRQs here */ 
}

/***************************************************************************//**
 * @implementation
 * TODO: Concrete implementation description.
 *
 ******************************************************************************/
/* PRQA S 1503 2 #justification: This is an interface function and just outside of this file/project's scope. */ 
void swtimer_InterruptInitialisation(vic_cpInterfaceFunctions_t vicIf, swtimer_pInterruptEnvironmentData_t environmentdata, swtimer_pInterruptContextData_t contextdata)
{
  if ((vicIf != NULL) && (vicIf->InterfaceVersion == (uint16_t) VIC_INTERFACE_VERSION) && (environmentdata != NULL))
  {
    environmentdata->ContextData = contextdata;

    /* Register module at interrupt handler */
    vicIf->RegisterModule(vic_IRQ_SWTIMER, &loc_swtimer_InterruptHandler, (void *) environmentdata);
    vicIf->EnableModule(vic_IRQ_SWTIMER);
  }
  else {}
}
