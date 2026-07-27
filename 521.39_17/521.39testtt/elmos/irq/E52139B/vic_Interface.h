/***************************************************************************//**
 * @file      vic_Interface.h
 *
 * @creator   poe
 * @created   13.05.2020
 * @sdfv      Elmos Flow - Grade 2
 * @project   521.39 
 * @version   $Revision: 9392 $
 *
 * @brief     Interfaces for the VIC
 *
 * @purpose   Provides functions for the VIC handling and control
 *
 * $Id: vic_Interface.h 9392 2021-11-18 09:45:59Z poe $
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
 * Disclaimer:
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

#ifndef VIC_INTERFACE_H_
#define VIC_INTERFACE_H_

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#define VIC_INTERFACE_VERSION           0x0101u

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

typedef unsigned short vic_InterfaceVersion_t;         /**< Data type for interface version representation **/

typedef void * vic_pInterruptModuleEnvironmentData_t;  /**< Generic pointer type to module specific environment data **/
typedef const  vic_pInterruptModuleEnvironmentData_t vic_cpInterruptModuleEnvironmentData_t;

/***************************************************************************//**
 * @brief Interrupt vector table type
 *
 * Auto combine vector number and table base to create vector number
 * related CPU interrupt pointer.
 ******************************************************************************/
typedef enum vic_eInterruptTableType
{
  /** base value is combined with vector number to be used as CPU
   * interrupt pointer (an interrupt service routine per module)*/
  vic_TT_COMBINE_WITH_VECTOR_NUM = 0,
  
  /** base value is directly used as CPU interrupt pointer
   * (one common interrupt service routine) */
  vic_TT_DIRECTLY_USED           = 1
    
}vic_eInterruptTableType_t;

/***************************************************************************//**
 * @brief VIC IRQ vector numbers
 ******************************************************************************/
typedef enum vic_eInterruptVectorNum
{
  vic_IRQ_SYS_STATE                        = 0u,
  vic_IRQ_DIVIDER                          = 1u,
  vic_IRQ_ADC_CTRL                         = 2u,
  vic_IRQ_PWM                              = 3u,
  vic_IRQ_SCI                              = 4u,
  vic_IRQ_SWTIMER                          = 5u,
  vic_IRQ_GPIO                             = 6u,
  vic_IRQ_LIN_CTRL                         = 7u,
  vic_IRQ_EEPROM_CTRL                      = 8u,

  vic_INTERRUPT_VECTOR_CNT                 = 9u  /**< Number of available interrupt vectors */
    
}vic_eInterruptVectorNum_t;

/***************************************************************************//**
 * @brief Interrupt handler function typedef
 *
 * This function type represents the actual interrupt handler function as 
 * implemented in each module. 
 *
 ******************************************************************************/
typedef void (__interrupt *vic_InterruptCallback_t) (void);

/***************************************************************************//**
 * @brief VIC environment data type
 ******************************************************************************/
/* PRQA S 3630 2 #justification: this struct is not referenced by in this project, but can be accessed
 *                               externally over an API by other projects. */   
typedef struct vic_sInterruptEnvironmentData
{
    /** Interrupt vector table to module interrupt handler */
    vic_InterruptCallback_t InterrupVectorTable[vic_INTERRUPT_VECTOR_CNT];
    
    /** Pointer to module vector table */
    vic_pInterruptModuleEnvironmentData_t ModuleEnvironmentData[vic_INTERRUPT_VECTOR_CNT];
    
} vic_sInterruptEnvironmentData_t;

/***************************************************************************//**
 * @brief Pointer to VIC environment data.
 ******************************************************************************/
typedef vic_sInterruptEnvironmentData_t * vic_pInterruptEnvironmentData_t;

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ********************* EXTERNAL FUNCTIONS / INTERFACE ***********************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Returns current module specific environment data
 *
 * @param module  Module number 
 *
 * @return        Pointer to module data (may be NULL)
 *
 ******************************************************************************/
typedef vic_pInterruptModuleEnvironmentData_t (*vic_GetPointerToEnvironmentData_t)(vic_eInterruptVectorNum_t module);

/***************************************************************************//**
 * @brief Introduces a module into the global VIC based IRQ handling
 *
 * @param module                  Module number
 *
 * @param interrupthandler        Pointer to module specific interrupt handler
 *
 * @param moduleenvironmentdata   Pointer to module specific interrupt handler runtime data
 *
 *
 ******************************************************************************/
typedef void (*vic_RegisterModule_t)(vic_eInterruptVectorNum_t    module, vic_InterruptCallback_t interrupthandler,
                                     vic_cpInterruptModuleEnvironmentData_t moduleenvironmentdata);

/***************************************************************************//**
 * @brief Removes module handler for a particular module. 
 *
 * @param module                  Module number
 *
 * A default IRQ handler is installed for the selected module.
 *
 ******************************************************************************/
typedef void (*vic_DeregisterModule_t)(vic_eInterruptVectorNum_t module);

/***************************************************************************//**
 * @brief Enables interrupt processing for specified module
 *
 * @param module                  Module number
 *
 ******************************************************************************/
typedef void (*vic_EnableModule_t)(vic_eInterruptVectorNum_t moduleIRQNum);

/***************************************************************************//**
 * @brief Disables interrupt processing for specified module
 *
 * @param module                  Module number
 *
 ******************************************************************************/
typedef void (*vic_DisableModule_t)(vic_eInterruptVectorNum_t moduleIRQNum);

/***************************************************************************//**
 * @brief Enables general interrupt processing
 *
 ******************************************************************************/
typedef void (*vic_EnableMain_t)(void);

/***************************************************************************//**
 * @brief Disables general interrupt processing
 *
 ******************************************************************************/
typedef void (*vic_DisableMain_t)(void);

/***************************************************************************//**
 * @brief Initializes global interrupt handling
 *
 * @param environmentdata  Pointer to Environment data for VIC module in
 *                         user RAM
 *
 ******************************************************************************/
typedef void (*vic_IRQInitialisation_t)(vic_pInterruptEnvironmentData_t penvironmentdata);

/***************************************************************************//**
 * @brief LIN driver interface function pointer
 ******************************************************************************/
/* PRQA S 3630 1 #justification: This is an interface definition for external usage and must not be hidden. */
struct vic_sInterfaceFunctions
{
  vic_InterfaceVersion_t            InterfaceVersion;

  vic_IRQInitialisation_t           IRQInitialisation;

  vic_GetPointerToEnvironmentData_t GetPointerToEnvironmentData;

  vic_RegisterModule_t              RegisterModule;
  vic_DeregisterModule_t            DeregisterModule;
  vic_EnableModule_t                EnableModule;
  vic_DisableModule_t               DisableModule;

  vic_EnableMain_t                  EnableMain;
  vic_DisableMain_t                 DisableMain;
};

typedef struct vic_sInterfaceFunctions    vic_sInterfaceFunctions_t;
typedef        vic_sInterfaceFunctions_t* vic_pInterfaceFunctions_t;
typedef const  vic_sInterfaceFunctions_t* vic_cpInterfaceFunctions_t;

#endif /* VIC_INTERFACE_H_ */




