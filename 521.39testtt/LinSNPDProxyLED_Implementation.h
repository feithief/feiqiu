/***************************************************************************//**
 * @file		LinSNPDProxyLED_Implementation.h
 *
 * @creator		poe
 * @created		05.02.2016
 *
 * @brief  		Proxy layer implementation functioning as visualization aid for autoaddressing process.
 *
 * @purpose             This module is a proxy between diagnostic layer and auto addressing interface
 *                      of the Bus layer. It is thought to aid the user informing him of the current
 *                      status of auto addressing by setting a specific color on the LED output.
 *
 *                      - Yellow:  Node is neither addressed, nor preselected.
 *                      - Purple:  Node is not yet addressed, but preselected.                    
 *                      - Green :  Node is the last one addressed.
 *                      - Blue  :  Node has already been adressed, skipped processing.
 *                      - Red   :  An error occured during auto addressing.
 *
 *
 * Demo Code Usage Restrictions:
 * Elmos Semiconductor SE provides this source code file simply and solely for 
 * IC evaluation purposes in laboratory and this file must not be used for other 
 * purposes or within non laboratory environments. Especially, the use or the 
 * integration in production systems, appliances or other installations is 
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
 * $Id: LinSNPDProxyLED_Implementation.h 8918 2021-05-20 15:08:32Z poe $
 *
 * $Revision: 8918 $
 *
 ******************************************************************************/

#ifndef LINSNPDPROXYLED_IMPLEMENTATION_H_
#define LINSNPDPROXYLED_IMPLEMENTATION_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include "LinSNPD_Interface.h"
/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#define LINSNPDPROXYLED_IMP_ENVIRONMENT_DATA_SZE 8                              /*<< Number of bytes required for environment data */
/* ****************************************************************************/
/* ******************** STRUCTS, ENUMS AND TYPEDEFS ***************************/
/* ****************************************************************************/

struct LinSNPDProxyLEDImp_sCfgData                                              /*<< SNPD proxy layer configuration data */
{
  LinSNPDIf_sThis_t LinSNPDIfThisPointer;    
};

typedef struct LinSNPDProxyLEDImp_sCfgData      LinSNPDProxyLEDImp_sCfgData_t;
typedef        LinSNPDProxyLEDImp_sCfgData_t*   LinSNPDProxyLEDImp_pCfgData_t;


/* *****************************************************************************
 * @implementation
 * Initializes the SNPD Proxy layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @param   SNPDEnvDataSze        allocated environment data buffer size
 * @param   genericSNPDImpCfgData configuration data for the proxy layer
 * @return  Lin_Bool_t            TRUE,  if genericSNPDEnvData and genericSNPDImpCfgData are not NULL and SNPDEnvDataSze is big enough
 *                                FALSE, else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_Initialization(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, LinSNPDIf_EnvDataSze_t SNPDEnvDataSze, LinSNPDIf_pGenericImpCfgData_t genericSNPDImpCfgData);

/* *****************************************************************************
 * @implementation
 * Updates the given pointer to point to SNPProxy layer's subinterface, if available.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @param   interfaceId           sub interface identifier
 * @param   ifThisPtr             this pointer of SNPD proxy
 * @return  Lin_Bool_t            TRUE , valid sub interface found and pointer updated successfully
 *                                FALSE, else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_GetSubInterface(LinSNPDIf_pGenericEnvData_t genericBusEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/* *****************************************************************************
 * @implementation
 * Updates the LED status at the start of an auto addressing measurement.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_StartMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/* *****************************************************************************
 * @implementation
 * Updates the LED status at the start of another auto addressing measurement round.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_NextMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_Bool_t addressed);

/* *****************************************************************************
 * @implementation
 * Propagates the end measurement command to the LIN BUS layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
void LinSNPDProxyLEDImp_EndMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/* *****************************************************************************
 * @implementation
 * Returns the current auto addressing state from the LIN BUS layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  LinSNPDIf_eState_t    current auto addressing state
 * ************************************************************************** */
LinSNPDIf_eState_t  LinSNPDProxyLEDImp_GetState(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/* *****************************************************************************
 * @implementation
 * Returns the current count of done auto addressing measurements.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  LinSNPDIf_MeasCount_t current number of done auto addressing measurements.
 * ************************************************************************** */
LinSNPDIf_MeasCount_t  LinSNPDProxyLEDImp_GetMeasurementCount(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);
/***************************************************************************//**
 *
 ******************************************************************************/

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#ifdef  LINSNPDPROXYLED_EXT_IFFUN_STRCT_ACCESS
extern const LinSNPDIf_sInterfaceFunctions_t  LinSNPDProxyLEDImp_InterfaceFunctions;
#endif /*  LINSNPDPROXYLED_EXT_IFFUN_STRCT_ACCESS */
#endif
