/***************************************************************************//**
 * @file		rom_API.c
 *
 * @creator		RPY
 * @created		20.04.2015
 * @sdfv                TODO: Automotive Spice or Elmos Flow or Demo Flow
 *
 * @brief  		TODO: Short description of this module
 *
 * @purpose
 *
 * TODO: A detailed description of this module
 *
 * $Id: $
 *
 * $Revision: 1.1 $
 *
 ******************************************************************************/
 /* ******************************************************************************
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor AG provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used 
 * for other purposes or within non laboratory environments. Especially, the use 
 * or the integration in production systems, appliances or other installations is 
 * prohibited.
 * 
 * Disclaimer\:
 * Elmos Semiconductor AG shall not be liable for any damages arising out of 
 * defects resulting from 
 * (1) delivered hardware or software, 
 * (2) non observance of instructions contained in this document, or 
 * (3) misuse, abuse, use under abnormal conditions or alteration by anyone 
 * other than Elmos Semiconductor AG. To the extend permitted by law 
 * Elmos Semiconductor AG hereby expressively disclaims and user expressively 
 * waives any and all warranties of merchantability and of fitness for a 
 * particular purpose, statutory warranty of non-infringement and any other 
 * warranty or product liability that may arise by reason of usage of trade, 
 * custom or course of dealing.
 *
 *******************************************************************************/
 
#include "rom_API.h"

/***************************************************************************//**
 * @brief Pointer declaration
 ******************************************************************************/
/* PRQA S 3408,1514 3 #justification: This is an API entry point declaration for usage in other projects. */
/* PRQA S 0306      1 #justification: This is fixed address declaration and necessary for code placement. */
const romIf_cpMainInterfaceFunctions_t romIf_MainInterface = (romIf_cpMainInterfaceFunctions_t ) ROMIF_MAIN_INTERFACE_LOCATION;


/***************************************************************************//**
 * Get interface function wrapper.
 *
 * @param[in]  id interface id
 * @param[out] interfaceFunctions interface functions
 * @param[out] envSize            size of the enviormant for the iterface
 *
 * @return TRUE for success, otherwise FALSE
 ******************************************************************************/
/* PRQA S 1503,3408 1 #justification: This is an API function declaration for usage in other projects. */
bool_t romIf_Interface_Get(romIf_InterfaceId_t id, romIf_cpGenericInterface_t *interfaceFunctions, romIf_Length_t *envSize)

{
  bool_t rv = FALSE;
  
  if ( romIf_MainInterface->InterfaceVersion == ROMIF_MAIN_INTERFACE_VERSION )
  {
    rv = romIf_MainInterface->Interface_Get(id, interfaceFunctions, envSize);
  }
    
  return rv;
}
