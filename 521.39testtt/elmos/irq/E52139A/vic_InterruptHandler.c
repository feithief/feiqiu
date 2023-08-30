/***************************************************************************//**
 * @file			vic_Implementation.c
 *
 * @creator		poe
 * @created		13.06.2020
 * @sdfv        Elmos Flow - Grade 2 *
 *
 * @brief       Interrupt handler for the VIC
 *
 * @purpose     Implements interrupt handler for the VIC
 *
 * $Id: vic_InterruptHandler.c 9258 2021-09-28 17:30:48Z poe $
 *
 * $Revision: 9258 $
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

/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/***************************************************************************//**
 * Default VIC interrupt handler, which disables triggered interrupt on call.
 *
 * @param none.
 *
 ******************************************************************************/
__interrupt static void loc_DummyInterruptHandler(void);


const vic_sInterfaceFunctions_t vic_InterfaceFunctions =
{
  .InterfaceVersion            = VIC_INTERFACE_VERSION,

  .IRQInitialisation           = &vic_IRQInitialisation,

  .GetPointerToEnvironmentData = &vic_GetPointerToEnvironmentData,

  .RegisterModule              = &vic_RegisterModule,
  .DeregisterModule            = &vic_DeregisterModule,
  .EnableModule                = &vic_EnableModule,
  .DisableModule               = &vic_DisableModule,

  .EnableMain                  = &vic_EnableMain,
  .DisableMain                 = &vic_DisableMain
};

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/***************************************************************************//**
 * Returns current module specific environment data
 *
 * @param module     Module number
 * 
 * @return           Pointer to module data (may be NULL)
 *
 ******************************************************************************/
vic_pInterruptModuleEnvironmentData_t vic_GetPointerToEnvironmentData(vic_eInterruptVectorNum_t module)
{
  vic_sInterruptEnvironmentData_t const* penvironmentdata = (vic_pInterruptEnvironmentData_t) VIC_TABLE_BASE; // PRQA S 0306
  vic_pInterruptModuleEnvironmentData_t rv = NULL;

  if (module < vic_INTERRUPT_VECTOR_CNT)
  { 
    rv = penvironmentdata->ModuleEnvironmentData[module];
  }
  else {}
  
  return rv;
}

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
/* PRQA S 3673 4 #justification: ptr is already const. The suggested "* const" is not recognized by QAC
 *                               even if implemented. const * is sufficient enough for the moduleEnvData ptr. */
void vic_RegisterModule(vic_eInterruptVectorNum_t    module, vic_InterruptCallback_t interrupthandler,
                                                     vic_cpInterruptModuleEnvironmentData_t moduleenvironmentdata)
{
  vic_pInterruptEnvironmentData_t penvironmentdata = (vic_pInterruptEnvironmentData_t) VIC_TABLE_BASE; // PRQA S 0306

  if (module < vic_INTERRUPT_VECTOR_CNT)
  {
    penvironmentdata->InterrupVectorTable[module]   = interrupthandler;
    penvironmentdata->ModuleEnvironmentData[module] = moduleenvironmentdata;
  }
  else {}
}

/***************************************************************************//**
 * Removes module handler for a particular module.
 *
 * @param module                  Module number
 *
 * A default IRQ handler is installed for the selected module.
 *
 ******************************************************************************/
void vic_DeregisterModule(vic_eInterruptVectorNum_t module)
{
  vic_pInterruptEnvironmentData_t penvironmentdata = (vic_pInterruptEnvironmentData_t) VIC_TABLE_BASE; // PRQA S 0306

  if (module < vic_INTERRUPT_VECTOR_CNT)
  {
    vic_DisableModule(module);
    penvironmentdata->InterrupVectorTable[module] = &loc_DummyInterruptHandler;
    penvironmentdata->ModuleEnvironmentData[module] = NULL;
  }
  else {}
}

/***************************************************************************//**
 * Enables interrupt processing for specified module.
 *
 * @param moduleIRQNum                  Module number
 *
 ******************************************************************************/
void vic_EnableModule(vic_eInterruptVectorNum_t moduleIRQNum)
{
  VIC_IRQ_VENABLE = (uint16_t) moduleIRQNum;
}

/***************************************************************************//**
 * Disables interrupt processing for specified module.
 *
 * @param moduleIRQNum                  Module number
 *
 ******************************************************************************/
void vic_DisableModule(vic_eInterruptVectorNum_t moduleIRQNum)
{
  VIC_IRQ_VDISABLE = (uint16_t) moduleIRQNum;
}

/***************************************************************************//**
 * Enables general interrupt processing.
 *
 * @param none
 *
 ******************************************************************************/
void vic_EnableMain(void)
{
  VIC_MAIN_ENABLE = (uint16_t) 1u;
}

/***************************************************************************//**
 * Disables general interrupt processing.
 *
 * @param none
 *
 ******************************************************************************/
void vic_DisableMain(void)
{
  VIC_MAIN_ENABLE = (uint16_t) 0u;
}

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
 * @detaildesc
 *
 ******************************************************************************/
void vic_IRQInitialisation(vic_pInterruptEnvironmentData_t penvironmentdata)
{
  uint8_t i;

  VIC_TABLE_BASE = (uint16_t) penvironmentdata; // PRQA S 0306
  VIC_TABLE_TYPE = (uint16_t) vic_TT_COMBINE_WITH_VECTOR_NUM;

  /* Initialize module IRQ table with dummy functions */
  for(i = 0u; i < (uint8_t) vic_INTERRUPT_VECTOR_CNT; i++ )
  {
    penvironmentdata->InterrupVectorTable[i] = &loc_DummyInterruptHandler;
    penvironmentdata->ModuleEnvironmentData[i] = NULL;
  }
}

/***************************************************************************//**
 * @implementation
 * Generic default IRQ handler implementation for unused IRQs.
 *
 ******************************************************************************/
__interrupt static void loc_DummyInterruptHandler(void)
{
  vic_eInterruptVectorNum_t irqvecnum = (vic_eInterruptVectorNum_t) VIC_IRQ_VNO;

  /* Clear IRQ */
  VIC_IRQ_VNO = (uint16_t) irqvecnum;

  /* Disable interrupt */
  VIC_IRQ_VDISABLE = (uint16_t) irqvecnum;
}
