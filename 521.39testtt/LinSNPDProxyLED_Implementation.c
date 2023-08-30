/***************************************************************************//**
 * @file		LinSNPDProxyLED_Implementation.c
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
 * $Id: LinSNPDProxyLED_Implementation.c 8918 2021-05-20 15:08:32Z poe $
 *
 * $Revision: 8918 $
 *
 ******************************************************************************/




/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/
#include <el_helper.h>
#include "LinSNPDProxyLED_Implementation.h"

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#define SHOW_LED_SNPD_STATUS (1u)                                                  /**< 0 do not use LED status indication | 1 show status */
/* ****************************************************************************/
/* ******************** STRUCTS, ENUMS AND TYPEDEFS ***************************/
/* ****************************************************************************/

struct loc_sSNPDProxyLEDEnvironmentData                                         /**< Environment runtime data */
{
  LinSNPDIf_sThis_t LinSNPDIfThisPointer;    
};

typedef struct loc_sSNPDProxyLEDEnvironmentData      loc_SNPDProxyLEDEnvironmentData_t;
typedef        loc_SNPDProxyLEDEnvironmentData_t*    loc_pSNPDProxyLEDEnvironmentData_t;


/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
const LinSNPDIf_sInterfaceFunctions_t LinSNPDProxyLEDImp_InterfaceFunctions =   /**< Interface function definitions */
{
  .InterfaceVersion         = LINSNPD_INTERFACE_MODULE_API_VERSION,
  
  .Initialization           = &LinSNPDProxyLEDImp_Initialization,
  .GetSubInterface          = &LinSNPDProxyLEDImp_GetSubInterface,
  .StartMeasurement         = &LinSNPDProxyLEDImp_StartMeasurement,
  .EndMeasurement           = &LinSNPDProxyLEDImp_EndMeasurement,
  .NextMeasurement          = &LinSNPDProxyLEDImp_NextMeasurement,
  .GetState                 = &LinSNPDProxyLEDImp_GetState,
  .GetMeasurementCount      = &LinSNPDProxyLEDImp_GetMeasurementCount,
};

/* *************************************************************************** */
/* ************************** FORWARD DECLARATIONS *************************** */
/* *************************************************************************** */

/* *****************************************************************************
 * @implementation
 * Sets the LED color according to given auto addressing status.
 *
 * @param   snpdStatus                     current auto addressing status
 * @return  none
 * ************************************************************************** */
static void loc_LinSNPDProxyLEDImp_SetSNPDStatusLED(LinSNPDIf_eState_t snpdStatus);


/* *************************************************************************** */
/* ************************* FUNCTION DECLARATIONS *************************** */
/* *************************************************************************** */
#define SET_STATUS_DONE_WAITING()   
#define SET_STATUS_DONE_LAST()     {PWM_STOP0 = PWM_PERIOD0;    PWM_STOP1 = PWM_PERIOD1;    PWM_STOP3 = PWM_PERIOD3;}
#define SET_STATUS_DONE_NOT_LAST() {PWM_STOP0 = 0;              PWM_STOP1 = 0u;             PWM_STOP3 = PWM_PERIOD3;} 
#define SET_STATUS_DONE_SKIPED()   {PWM_STOP0 = PWM_PERIOD0>>2; PWM_STOP1 = PWM_PERIOD1>>2; PWM_STOP3 = PWM_PERIOD3>>2;} 
#define SET_STATUS_ERROR()

/* *****************************************************************************
 * @implementation
 * Sets the LED color according to given auto addressing status.
 *
 * @param   snpdStatus                     current auto addressing status
 * @return  none
 * ************************************************************************** */
static void loc_LinSNPDProxyLEDImp_SetSNPDStatusLED(LinSNPDIf_eState_t snpdStatus)
{
  switch(snpdStatus)
  {
    case LinSNPDIf_DONE_NOT_LAST        :  {SET_STATUS_DONE_NOT_LAST();  break; }// Yellow  (Not yet addressed, not preselected)
    
    case LinSNPDIf_WAITING              : { SET_STATUS_DONE_WAITING();   break; }// Purple  (Not yet addressed, but preselected)
                       
    case LinSNPDIf_DONE_LAST            : { SET_STATUS_DONE_LAST();      break; } // Green (The last one)
                      
    case LinSNPDIf_SKIPED               : { SET_STATUS_DONE_SKIPED();    break; } // Blue (Already adressed, skipped processing)

    default                             : { SET_STATUS_ERROR();           break;} // Red (Errors)                      
  }
}


/* *****************************************************************************
 * @implementation
 * Initializes the SNPD Proxy layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @param   SNPDEnvDataSze        allocated environment data buffer size
 * @param   genericSNPDImpCfgData configuration data for the proxy layer
 * @return  Lin_Bool_t            TRUE , if genericSNPDEnvData and genericSNPDImpCfgData
 *                                       are not NULL and SNPDEnvDataSze is big enough
 *                                FALSE, else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_Initialization(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, LinSNPDIf_EnvDataSze_t SNPDEnvDataSze, LinSNPDIf_pGenericImpCfgData_t genericSNPDImpCfgData)
{
  Lin_Bool_t rv = LIN_FALSE;
    
  if (genericSNPDEnvData !=  NULL && 
      SNPDEnvDataSze >= sizeof(loc_SNPDProxyLEDEnvironmentData_t) && 
      genericSNPDImpCfgData != NULL )
  {  
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDProxyLEDImp_pCfgData_t snpdImpCfgData  = (LinSNPDProxyLEDImp_pCfgData_t) (genericSNPDImpCfgData);

    /* Ensure valid interface function */
    if (snpdImpCfgData->LinSNPDIfThisPointer.IfFunsTbl != NULL) 
    {    
      snpdImpEnv->LinSNPDIfThisPointer = snpdImpCfgData->LinSNPDIfThisPointer;    
      rv=LIN_TRUE;
    }
    else {}
  }
  else {}
  
  return rv;    
}


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
Lin_Bool_t LinSNPDProxyLEDImp_GetSubInterface(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr)
{
  Lin_Bool_t rv = LIN_FALSE;
  
  if (genericSNPDEnvData !=  NULL)
  {
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDIf_cpInterfaceFunctions_t iFuns        = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
    if(iFuns != LIN_NULL)
    {
      const LinSNPDIf_GetSubInterfaceIfFun_t fun  = iFuns->GetSubInterface;
      if (fun != LIN_NULL)
      {
        rv = fun(snpdImpEnv->LinSNPDIfThisPointer.envData, interfaceId, ifThisPtr);
      }
      else{}
    }
     else {}
  }
  else {}
  return(rv);
}


/* *****************************************************************************
 * @implementation
 * Updates the LED status at the start of an auto addressing measurement.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_StartMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData)
{ 
  Lin_Bool_t rv = LIN_FALSE;
  
   if (genericSNPDEnvData !=  NULL)
    {
      loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
      LinSNPDIf_cpInterfaceFunctions_t iFuns        = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
      if(iFuns != LIN_NULL)
      {
        const LinSNPDIf_StartMeasurementIfFun_t fun = iFuns->StartMeasurement;
        if (fun != LIN_NULL)
        {             
          rv = fun(snpdImpEnv->LinSNPDIfThisPointer.envData);
#if SHOW_LED_SNPD_STATUS == 1
          LinSNPDIf_eState_t state = LinSNPDProxyLEDImp_GetState(genericSNPDEnvData);
          loc_LinSNPDProxyLEDImp_SetSNPDStatusLED(state);//PWM Color Coding
#endif  
        }
        else{}
      }
      else {}
    }
   else {}
  return(rv);
}

/* *****************************************************************************
 * @implementation
 * Updates the LED status at the start of another auto addressing measurement round.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
Lin_Bool_t LinSNPDProxyLEDImp_NextMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_Bool_t addressed)
{
  Lin_Bool_t rv = LIN_FALSE;
  
  if (genericSNPDEnvData !=  NULL)
  {
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDIf_cpInterfaceFunctions_t iFuns        = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
    if (iFuns != NULL)
    {
      const LinSNPDIf_NextMeasurementIfFun_t fun  = iFuns->NextMeasurement;
      if (fun != LIN_NULL)
      {
#if SHOW_LED_SNPD_STATUS == 1
          LinSNPDIf_eState_t state = LinSNPDProxyLEDImp_GetState(genericSNPDEnvData);
          loc_LinSNPDProxyLEDImp_SetSNPDStatusLED(state);//PWM Color Coding
#endif  
        rv = fun(snpdImpEnv->LinSNPDIfThisPointer.envData, addressed);        
      }
      else{}
    }
    else {}
  }
  else{}
 
  return(rv);
}

/* *****************************************************************************
 * @implementation
 * Propagates the end measurement command to the LIN BUS layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  Lin_Bool_t            FALSE , given environment data pointer was NULL
 *                                TRUE  , else
 * ************************************************************************** */
void LinSNPDProxyLEDImp_EndMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData)
{
  if (genericSNPDEnvData !=  NULL)
  {
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDIf_cpInterfaceFunctions_t iFuns        = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
    if (iFuns != NULL)
    {
      const LinSNPDIf_EndMeasurementIfFun_t fun   = iFuns->EndMeasurement;
      if (fun != LIN_NULL)
      {
        fun(snpdImpEnv->LinSNPDIfThisPointer.envData);         
      }
      else{}
    }
    else {}
  }
  else{}
}


/* *****************************************************************************
 * @implementation
 * Returns the current auto addressing state from the LIN BUS layer.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  LinSNPDIf_eState_t    current auto addressing state
 * ************************************************************************** */
LinSNPDIf_eState_t LinSNPDProxyLEDImp_GetState(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData)
{
  LinSNPDIf_eState_t rv = LinSNPDIf_DISABLED;
  
  if (genericSNPDEnvData !=  NULL)
  {
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDIf_cpInterfaceFunctions_t iFuns        = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
    if (iFuns != NULL)
    {  
      const LinSNPDIf_GetStateIfFun_t fun         = iFuns->GetState;
      if (fun != LIN_NULL)
      {
        rv = fun(snpdImpEnv->LinSNPDIfThisPointer.envData);
      }
      else{}
    }
    else{}
  }
  else {}  
  return(rv);
}


/* *****************************************************************************
 * @implementation
 * Returns the current count of done auto addressing measurements.
 *
 * @param   genericSNPDEnvData    pointer to environment data buffer
 * @return  LinSNPDIf_MeasCount_t current number of done auto addressing measurements.
 * ************************************************************************** */
LinSNPDIf_MeasCount_t LinSNPDProxyLEDImp_GetMeasurementCount(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData)
{  
  LinSNPDIf_MeasCount_t rv = 0;
  
  if (genericSNPDEnvData !=  NULL)
  {
    loc_pSNPDProxyLEDEnvironmentData_t snpdImpEnv     = (loc_pSNPDProxyLEDEnvironmentData_t)(genericSNPDEnvData);
    LinSNPDIf_cpInterfaceFunctions_t iFuns            = snpdImpEnv->LinSNPDIfThisPointer.IfFunsTbl;
    if (iFuns != NULL)
    {
      const LinSNPDIf_GetMeasurementCountIfFun_t fun  = iFuns->GetMeasurementCount;
      if (fun != LIN_NULL)
      {
        rv = fun(snpdImpEnv->LinSNPDIfThisPointer.envData);
      }
      else{}
    }
    else{}
  }
  else {}  
  return(rv);
}


