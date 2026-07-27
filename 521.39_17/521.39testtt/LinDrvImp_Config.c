/***************************************************************************//**
 * @file     LinDrvImp_Config.c
 *
 * @creator  poe
 * @created  2021/02/23 
 *
 * @brief    LIN Driver Configuration and Intitialization
 *
 * @purpose  This module contains the LIN driver configuration data
 *           and handles the initialization process.
 *
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
 * $Id: $
 *
 * $Revision: 1929 $
 *
 ******************************************************************************/

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include <el_helper.h>
#include "Lin_Basictypes.h"
#include "LinDrvImp_Config.h"

#include "LinDrvImp_FrameConfig.h"
#include "LinDrvImp_Callbacks.h"

#include "ModuleFlash.h"   // LJM Added
#if (LINDRVIMP_USE_SNPD_PROXY == 1u)  
#include "LinSNPDProxyLED_Implementation.h"
#endif
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FORWARD DECLARATIONS ****************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @implementation
 * Loads app cfg data, if available and assigns ROMIF pointer, 
 * if selected as such.
 *
 * @param       none
 * @return      LIN_TRUE , if cfg runtime and ROM target selection was successful
 *              LIN_FALSE, else
 * *****************************************************************************/
Lin_Bool_t loc_CheckForROMIfCfg(void);

/* **************************************************************************//**
 * @implementation
 * Initialization of Bus, Lookup and/or Protocol layer.
 *
 * @param       none
 * @return      LIN_TRUE , if initalization was successful
 *              LIN_FALSE, else
 * *****************************************************************************/
Lin_Bool_t loc_Init_Bus_ProtoLayer(void);


/***************************************************************************//**
 * @implementation
 * Initialization of Diagnostic layer.
 *
 * @param       none
 * @return      LIN_TRUE , if initalization was successful
 *              LIN_FALSE, else
 ******************************************************************************/
Lin_Bool_t loc_Init_DiagLayer(void);


/***************************************************************************//**
 * @implementation
 * Initialization of SNPD LED proxy.
 *
 * @param       linDiagImpCfgData   Configuration data for the LIN diagnostic layer
 * @return      LIN_TRUE            if initalization was successful
 *              LIN_FALSE           else
 ******************************************************************************/
Lin_Bool_t loc_Init_DiagLayer_SNPDProxySetup(LinDiagImp_pCfgData_t linDiagImpCfgData);

/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/

LinBusIf_cpInterfaceFunctions_t               LinDrvImp_BusIfFuns;                                        /**< LIN bus layer interface pointer */
LinLookupIf_cpInterfaceFunctions_t            LinDrvImp_LookupIfFuns;                                     /**< LIN lookup layer interface pointer */
LinProtoIf_cpInterfaceFunctions_t             LinDrvImp_ProtoIfFuns;                                      /**< LIN protocol layer interface pointer */
LinTransIf_cpInterfaceFunctions_t             LinDrvImp_TransIfFuns;                                      /**< LIN transport layer interface pointer */
LinDiagIf_cpInterfaceFunctions_t              LinDrvImp_DiagIfFuns = &LinDiagImp_InterfaceFunctions;      /**< LIN diagnostic layer interface pointer  */
eepromIf_cpInterfaceFunctions_t               EEImp_IfFuns;                                               /**> EE NVM access interface pointer */
flashIf_cpInterfaceFunctions_t                FlashImp_IfFuncs;


#if (LINDRVIMP_USE_SNPD_PROXY == 1u)  
LinSNPDIf_cpInterfaceFunctions_t              LinDrvImp_SNPDProxyLEDIfFuns = &LinSNPDProxyLEDImp_InterfaceFunctions;  /*<< Set SNPD proxy layer IF pointer */ 
#endif

#pragma data_alignment=2
Lin_uint8_t LinDrvImp_BusEnvData[LINBUSIMP_ENVIRONMENT_DATA_SZE];                                             /**< Allocate memory for LIN bus layer enviroment data */
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_LookupEnvData[LINDRV_LOOKUP_SEQ_ENVIRONMENT_DATA_SZE];                                  /**< Allocate memory for LIN lookup layer enviroment data */
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_ProtoEnvData[LINPROTOIMP_ENVIRONMENT_DATA_SZE(LINPROTOIMP_MSG_BUF_LEN)];                /**< Allocate memory for LIN protocol layer enviroment data */
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_FrmDescLstEnvData[LINPROTOIMP_PER_FRM_DESC_LST_ENV_DATA_SZE(LINPROTOIMP_FRAMEID_CNT)];  /**< Allocate memory for LIN frame description list */
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_TransEnvData[LINTRANSIMP_ENVIRONMENT_DATA_SZE(LINTRANSIMP_MSG_BUF_LEN,0,0)];            /**< Allocate memory for LIN transport layer enviroment data */
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_DiagEnvData[LINDIAGIMP_ENVIRONMENT_DATA_SZE];                                           /**< Allocate memory for LIN diagnostic layer enviroment data */
#define TRANS_FRM_DESC_CNT 3
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_DiagFrmDescLstEnvData[LINPROTOIMP_PER_FRM_DESC_LST_ENV_DATA_SZE(TRANS_FRM_DESC_CNT)];   /**< Allocate memory for LIN service description list */

#if (LINDRVIMP_USE_SNPD_PROXY == 1u)  
#pragma data_alignment=2
Lin_uint8_t LinDrvImp_SNPDEnvData[LINSNPDPROXYLED_IMP_ENVIRONMENT_DATA_SZE];                                  /*<< Allocate memory for SNPD proxy layer enviroment data */
#endif
/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/
  
/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/

static const LinProtoIf_sCallbackFunctions_t LinDrvImp_ProtoCallbacks =         /**< Set LIN protocol layer callbacks */
{
  .CallbackVersion = LINDRVIMP_PROTO_USED_IF_VERSION,

  .Error    = &LinDrvImp_ProtoErrorCbFun,
  .Restart  = LIN_NULL,
  .MeasDone = LIN_NULL,
  .Idle     = &LinDrvImp_ProtoIdleCbFun,
  .WakeUp   = &LinDrvImp_ProtoWakupCbFun
};


 /* LinDrv diagnostic layer callbacks */
static const LinDiagIf_sCallbackFunctions_t LinDrvImp_DiagCallbacks =           /*<< Set LIN diagnostic layer callbacks */
{
  .CallbackVersion = LINDRVIMP_DIAG_USED_IF_VERSION,

  .GoToSleep       = LinDrvImp_DiagGoToSleepCbFun,
  .Error           = LinDrvImp_DiagErrorCbFun,
#if ( LINDIAG_SUP_SNPD == 1 )
  .CheckSNPDMethod = LinDrvImp_DiagCheckSNPDMethodIdCbFun,
  .StoreNAD        = LinDrvImp_DiagStoreNadCbFun
#endif  
};


Lin_Bool_t LinDrvImp_Diag_triggerNADStorage = LIN_FALSE;
/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/

/* **************************************************************************//**
 * @implementation
 * Initializes the LIN driver.
 *
 * @param   none
 * @return  TRUE        LIN driver initialization was successfull,
 *          FALSE       else
 * *****************************************************************************/
Lin_Bool_t LinDrvImp_Init(void)
{
  Lin_Bool_t retVal = LIN_FALSE;
  
  /* Check for ROM APIs to load. */
  retVal = loc_CheckForROMIfCfg();
  
  /* ROM IF is valid? */
  if ( retVal == LIN_TRUE )
  {
    /* Initialize LIN bUS layer */
    retVal = loc_Init_Bus_ProtoLayer();
  }
  else { /* ERROR: No valid ROM if found! */ }

  if (retVal == LIN_TRUE)
  {
    /* Initialize DIAG layer */
    retVal = loc_Init_DiagLayer();
  }
  else{ /* ERROR: Bus Init failed or no valid ROM IF found! */ }
  
  return(retVal);
}

/***************************************************************************//**
 * @implementation
 * Loads app cfg data, if available and assigns ROMIF pointer, 
 * if selected as such.
 *
 * @param       none
 * @return      LIN_TRUE , if cfg runtime and ROM target selection was successful
 *              LIN_FALSE, else
 * *****************************************************************************/
Lin_Bool_t loc_CheckForROMIfCfg(void)
{
  /* Initialize default return value. */
  Lin_Bool_t retVal = LIN_TRUE;

  /* ROM API INIT */
  /* Retrieve bus layer ROM API */
  if(romIf_MainInterface->Interface_Get(ROMIF_LIN_BUS, (romIf_cpGenericInterface_t *) &LinDrvImp_BusIfFuns, LIN_NULL) != TRUE)
  {
    /* ERROR: Could not retrieve LIN bus layer interface! */ 
    retVal = LIN_FALSE;
  }
  
  /* Retrieve lookup layer ROM API */
  if(retVal == LIN_TRUE && romIf_MainInterface->Interface_Get(ROMIF_LIN_LOOKUP_SEQUENTIAL, (romIf_cpGenericInterface_t *) &LinDrvImp_LookupIfFuns, LIN_NULL) != TRUE)
  {
  /* ERROR: Could not retrieve LIN lookup layer interface! */ 
    retVal = LIN_FALSE;
  }
  
  /* Retrieve protocol layer ROM API */
  if(retVal == LIN_TRUE && romIf_MainInterface->Interface_Get(ROMIF_LIN_PROTO, (romIf_cpGenericInterface_t *) &LinDrvImp_ProtoIfFuns, LIN_NULL) != TRUE)
  {
  /* ERROR: Could not retrieve LIN protocol layer interface! */ 
    retVal = LIN_FALSE;
  }
  
  /* Retrieve transport layer ROM API */
  if( retVal == LIN_TRUE && romIf_MainInterface->Interface_Get(ROMIF_LIN_TRANS, (romIf_cpGenericInterface_t *) &LinDrvImp_TransIfFuns, LIN_NULL)  != TRUE)
  {
    retVal = LIN_FALSE;
  }
  
  /* Retrieve ee_access ROM API */
  if(retVal == LIN_TRUE && romIf_MainInterface->Interface_Get(ROMIF_MTP_ACCESS, (romIf_cpGenericInterface_t *) &EEImp_IfFuns, LIN_NULL)  != TRUE)
  {
    retVal = LIN_FALSE;
  }
    
  /* Retrieve flash_access ROM API */
  if(retVal == LIN_TRUE && romIf_MainInterface->Interface_Get(ROMIF_FLASH_ACCESS, (romIf_cpGenericInterface_t *) &FlashImp_IfFuncs, LIN_NULL)  != TRUE)
  {
    retVal = LIN_FALSE;
  }
    
  return retVal;
}


#if (LINDRVIMP_USE_SNPD_PROXY == 1u)  
/***************************************************************************//**
 * @implementation
 * Initialization of SNPD LED proxy.
 *
 * @param       linDiagImpCfgData   Configuration data for the LIN diagnostic layer
 * @return      LIN_TRUE            if initalization was successful
 *              LIN_FALSE           else
 ******************************************************************************/
Lin_Bool_t loc_Init_DiagLayer_SNPDProxySetup(LinDiagImp_pCfgData_t linDiagImpCfgData)
{
  Lin_Bool_t retVal = LIN_FALSE;
  
   LinSNPDProxyLEDImp_sCfgData_t linSNPDProxyLEDImpCfgData =
  {
   .LinSNPDIfThisPointer = 
    {
      .IfFunsTbl              = LIN_NULL,
      .envData                = LIN_NULL
    }
  };
  

  if(LinDrvImp_BusIfFuns->GetSubInterface(LinDrvImp_BusEnvData, Lin_IfId_SNPD, (Lin_pThis_t) &(linSNPDProxyLEDImpCfgData.LinSNPDIfThisPointer)) == LIN_FALSE)//retrieve snpd if from bus layer
  {          
    linSNPDProxyLEDImpCfgData.LinSNPDIfThisPointer.IfFunsTbl = LIN_NULL;
    linSNPDProxyLEDImpCfgData.LinSNPDIfThisPointer.envData   = LIN_NULL;
  }
  
  /* Initialize SNPD proxy layer */
  retVal = LinDrvImp_SNPDProxyLEDIfFuns->Initialization((LinSNPDIf_pGenericEnvData_t) LinDrvImp_SNPDEnvData, sizeof(LinDrvImp_SNPDEnvData), &linSNPDProxyLEDImpCfgData);
  
  if (retVal == LIN_TRUE)
  {
    linDiagImpCfgData->LinSNPDIfThisPointer.IfFunsTbl = LinDrvImp_SNPDProxyLEDIfFuns; //set snpd if from snpd proxy
    linDiagImpCfgData->LinSNPDIfThisPointer.envData   = LinDrvImp_SNPDEnvData; //set snpd env from snpd proxy
  }
  return retVal;
}
#endif

  
/* **************************************************************************//**
 * @implementation
 * Initialization of Bus, Lookup and/or Protocol layer.
 *
 * @param       none
 * @return      LIN_TRUE , if initalization was successful
 *              LIN_FALSE, else
 * *****************************************************************************/
//uint8_t frameIDFromEeprom = 0;   // LJM added

void systemLinCommChangeNad(uint8_t newNad)
{
    if(newNad >= 1 && newNad < LIN_MAX_FRAMEID_CNT) // 大小来源 loc_DIAG_PIDS[LIN_MAX_FRAMEID_CNT]
    {   
        lastSavedSingalAdress = savedConfig.singleAddr;
    }
    else
    {
        newNad = 8;
    }
    savedConfig.singleAddr = newNad;
    /* 修改状态帧的ID */
    LinDrvImp_FrameDescriptionList[1].DefaultFrameID = newNad; 
    LinDrvImp_DiagIfFuns->SetNAD((LinDiagIf_pGenericEnvData_t) LinDrvImp_DiagEnvData, (uint8_t)savedConfig.singleAddr);
}


Lin_Bool_t LINTest_LJM(void)
{
  Lin_Bool_t ret = LIN_FALSE;
  ret = LinDrvImp_ProtoIfFuns->AddFrameDescLst((LinProtoIf_pGenericEnvData_t) LinDrvImp_ProtoEnvData, (LinProtoIf_pGenericFrameDescriptionList_t) LinDrvImp_FrameDescriptionList,
                                                    LIN_TRUE, (LinProtoIf_pGenericFrmDescLstEnvData_t) LinDrvImp_FrmDescLstEnvData, sizeof(LinDrvImp_FrmDescLstEnvData), LINDRVIMP_PROTO_FRM_DESC_LST_CB_CTX_DATA);
  return ret;
}


Lin_Bool_t loc_Init_Bus_ProtoLayer(void)
{
  /* Initialize default return value. */
  Lin_Bool_t retVal = LIN_FALSE;

   /* LIN Bus Layer Configuration */
  LinBusImp_CfgData_t linBusImpCfgData =
  {
    .Version                              = LINBUSIMP_CONFIG_DATA_VERSION,
    .ClockFrequency                       = LINBUSIMP_CLK_FREQUENCY,
    
    .ConfigFlags                          = 
    {
      .CallMeasDone                       = 0u,
      .CheckBaudrate                      = 1u,
      .EnableHighSpeed                    = 0u,
      .DetectPostPIDCollisions            = 1u,
      .EnableShortDetection               = 1u,
      .EnableSynchByteExtension           = 0u,
    },
    
    .DebouncerValue                       = LINBUSIMP_DEFAULT_DEBOUNCER_VALUE,
#if !defined(EL_DEVICE_E52139B)    
    .ShortDetectDebouncerValue            = (LinBusImp_eShortDetectDebounceSelect_t)LINBUSIMP_DEFAULT_SHORT_DEBOUNCER_VALUE,
#endif    
    .SendHeaderBreakLen                   = LINBUSIMP_SEND_HEADER_BREAK_LEN,
    .IgnoreMsgTimeouts                    = LINBUSIMP_DEFAULT_IGNORE_MSGTOUT,
    
    .TxDTimeoutSelect                     = LinBusImp_TXDTO_DEFAULT,
    
#if LINDIAG_SUP_SNPD == 1
    .SNPDModeSelect                       = LinSNPDIf_eSNPDMode_TYPE1,
    .ReverseCurrentDetectionThreshold     = 0u,
    .SNPDConfigFlags                      = 
    {
#if !defined(EL_DEVICE_E52139B)    
      .EnableType3ReverseCurrentDetection = LinBusImp_SNPD_T3_REVERSECURRDETECT_OFF
#else
      .EnableReverseCurrentDetection      = LinBusImp_SNPD_REVERSECURRDETECT_ON,
#endif
    },
    .SNPDFSMConfigSelect                  =
    {  
      .EndOfBreakMode                     = LinBusImp_SNPD_EOB_DEFAULT,
      .Step4WaitMode                      = LinBusImp_SNPD_STEP4_SW_WAIT_IMMEDIATE, 
      .Step5WaitMode                      = LinBusImp_SNPD_STEP5_WAIT_IMMEDIATE, 
      .DisableADCTrigger                  = LinBusImp_eADCTRIGGER_ENABLED,
      .FSMDiffSelect                      = LinBusImp_eFSMDiffSelect_OFF  
    },
    .SNPDPhyConfigSelect                  = 
    {
      .MeasInMuxSelect                    = LinBusImp_AAMEASMUX_HIGH,     
      .GainSelect                         = LinBusImp_AAGAIN_3,
#if !defined(EL_DEVICE_E52139B)         
      .EnableFilter                       = LinBusImp_AAFILTER_ENABLED,         
      .OffsetSelect                       = LinBusImp_AAOFFSET_LOW,
#endif      
      .AmpSelect                          = LinBusImp_AAAMPSEL_LOW,
      .EnableCommonModeMeas               = LinBusImp_AACMMEAS_DISABLED,
      .OutputCurrentDestination           = LinBusImp_SNPD_OUTPUT_CURRENT_LIN_M
   },
   .ConfigMode_Default                    =
   {
      .EnablePullUp                       = LIN_TRUE,
      .EnableCurrentSources               = LIN_FALSE,
      .EnableCurrentRamp                  = LIN_FALSE,
      .SNPDPullUpCurrent                  = 0u,
      .Oversampling                       = LinBusImp_SNPD_OVERSAMPLE_4X,
#if defined(EL_DEVICE_E52139B)           
      .EnableFilter                       = LIN_FALSE  
#endif        
   },
    .ConfigMode_Offset                    =
    {
      .EnablePullUp                       = LIN_FALSE,
      .EnableCurrentSources               = LIN_FALSE,
      .EnableCurrentRamp                  = LIN_FALSE,
      .SNPDPullUpCurrent                  = 0u,
      .Oversampling                       = LinBusImp_SNPD_OVERSAMPLE_16X,
#if defined(EL_DEVICE_E52139B)           
      .EnableFilter                       = TRUE 
#endif          
    },
    .ConfigMode_Preselect                 =
    {
      .EnablePullUp                       = LIN_FALSE,
      .EnableCurrentSources               = LIN_TRUE,
      .EnableCurrentRamp                  = LIN_FALSE,
      .SNPDPullUpCurrent                  = SNPD_PRESEL_PULLUPCURR,
      .Oversampling                       = LinBusImp_SNPD_OVERSAMPLE_16X,
#if defined(EL_DEVICE_E52139B)           
      .EnableFilter                       = TRUE 
#endif       
    },
    .ConfigMode_Select                    =
    {
      .EnablePullUp                       = LIN_FALSE,
      .EnableCurrentSources               = LIN_TRUE,
      .EnableCurrentRamp                  = LIN_FALSE,
      .SNPDPullUpCurrent                  = SNPD_SEL_PULLUPCURR,  
      .Oversampling                       = LinBusImp_SNPD_OVERSAMPLE_16X,
#if defined(EL_DEVICE_E52139B)           
      .EnableFilter                       = LIN_TRUE 
#endif       
    },
#if !defined(EL_DEVICE_E52139B)      
    .Type3MeasConfig	                  =
    {
      .EnableMeasOnlyUsage                = LinBusImp_Type3MeasOnly_OFF,
      .MaxShuntMeasConfig                 = 0u
    },	
#else
    .EnableType3MeasOnlyUsage             = LIN_FALSE,
#endif
  #if defined(EL_DEVICE_E52139A)
    .SNPDLINSwapCheck_Enable              = LIN_TRUE,       /* 1 - Enable LIN Swap-Detection; 0 - do not. */
    .SNPDLINSwapCancelOnError             = LIN_FALSE,      /* 1 - Do not assign NAD upon detected LIN swap; 0 - assign NAD anyway. */ 
    .SNPDLINSwapCheck_ThdPresel           = 75u,            /* LIN Swap detection threshold for preselect phase. */
    .SNPDLINSwapCheck_ThdSel              = 75u,            /* LIN Swap detecion threshold for selection phase. */
  #endif
  #if defined(EL_DEVICE_E52139B)   
    .ReverseCurrentDetectionThreshold           = 75u,
    .ReverseCurrentPreselectDetectionThreshold  = 75u,  
    .SNPDLINSwapCancelOnError                   = LIN_TRUE,
  #endif
		
    .SNPDDebugDataFun                     = NULL,
    .SNPDPlusParametersFun                = LinDrvImp_GetSNPDPlusParametersCbFun, /* Set callback function ptr for SNPD tresholds callback. */
#endif  
  };  

    /* LIN Protocol Layer Configuration */
    LinProtoImp_sCfgData_t linProtoImpCfgData =
    {
      .Version                = LINPROTOIMP_CONFIG_DATA_VERSION,
      .LinBusIfInitParam      =
      {
        .IfFunsTbl            = LinDrvImp_BusIfFuns,
        .envData              = &LinDrvImp_BusEnvData,
        .EnvDataLen           = sizeof(LinDrvImp_BusEnvData),
        .baudrate             = LINBUSIMP_BAUDRATE,      
        .ImpCfgData           = &linBusImpCfgData
      },
      .LinLookupIfInitParam   =
      {
        .IfFunsTbl            = LinDrvImp_LookupIfFuns,
        .envData              = &LinDrvImp_LookupEnvData,
        .EnvDataLen           = sizeof(LinDrvImp_LookupEnvData),
        .ImpCfgData           = LIN_NULL
      },
    };  


  /* Initialize LIN protocol layer (which initializes LIN bus layer). */
  retVal = LinDrvImp_ProtoIfFuns->Initialization((LinProtoIf_pGenericEnvData_t) LinDrvImp_ProtoEnvData, sizeof(LinDrvImp_ProtoEnvData),
                                                 &LinDrvImp_ProtoCallbacks, LINDRVIMP_PROTO_CALLBACK_CTX_DATA, &linProtoImpCfgData);
  /* Was proto init successful? */
  if( retVal == LIN_TRUE )
  {   
    systemLinCommChangeNad((uint8_t)savedConfig.singleAddr) ; //LJM added 
    /* Add frame description list to proto layer. */
   // retVal = LinDrvImp_ProtoIfFuns->AddFrameDescLst((LinProtoIf_pGenericEnvData_t) LinDrvImp_ProtoEnvData, (LinProtoIf_pGenericFrameDescriptionList_t) LinDrvImp_FrameDescriptionList,
                                                    //LIN_TRUE, (LinProtoIf_pGenericFrmDescLstEnvData_t) LinDrvImp_FrmDescLstEnvData, sizeof(LinDrvImp_FrmDescLstEnvData), LINDRVIMP_PROTO_FRM_DESC_LST_CB_CTX_DATA);
    retVal = LINTest_LJM();
  } 
  
  LinDrvImp_BusIfFuns->ChangeIdleTimeout((LinBusIf_pGenericEnvData_t)LinDrvImp_BusEnvData, 4000);
  return retVal;
}

/***************************************************************************//**
 * @implementation
 * Initialization of Diagnostic layer.
 *
 * @param       none
 * @return      LIN_TRUE , if initalization was successful
 *              LIN_FALSE, else
 ******************************************************************************/
//uint8_t nadFromEeprom = 0;
Lin_Bool_t loc_Init_DiagLayer(void)
{
  Lin_Bool_t retVal = LIN_FALSE;
 
  /* LIN Transport Layer Configuration */
  LinTransImp_sCfgData_t linTransImpCfgData =
  {
    .Version = LINTRANSIMP_CONFIG_DATA_VERSION,
    .LinProtoInsThisPointer =
    {
        .IfFunsTbl  = LinDrvImp_ProtoIfFuns,
        .envData    = &LinDrvImp_ProtoEnvData,
    },
  };
  
  const LinTransIf_sProtoFrameDescriptionInfo_t protoFrmDescInfo =
  {
    .ReqFrameInfo =
    {
      .MsgID   = LINDIAGIMP_REQST_CH_FRM_ID,
      .Length  = LINDIAGIMP_REQST_CH_MSG_SZE,
      .CrcType = LINDIAGIMP_REQST_CH_CRC_TYPE
    },
    .RespFrameInfo =
    {
      .MsgID   = LINDIAGIMP_RESP_CH_FRM_ID,
      .Length  = LINDIAGIMP_RESP_CH_MSG_SZE,
      .CrcType = LINDIAGIMP_RESP_CH_CRC_TYPE
    },
    
    .FrameDescListEnvData    = LinDrvImp_DiagFrmDescLstEnvData,
    .FrameDescListEnvDataSze = sizeof(LinDrvImp_DiagFrmDescLstEnvData),

    .LdfRelevance = LIN_FALSE
  };
  
  LinTransIf_sCfgDataFlags_t linTransInitCfgFlags =
  {
    .AbortProc              = 1u
  };
  
  LinDiagImp_sCfgData_t linDiagImpCfgData =
  {
    .Version = LINDIAGIMP_CONFIG_DATA_VERSION,

    .LinTransIfInitParam =
    {
      .IfFunsTbl              = LinDrvImp_TransIfFuns,
      .envData                = &LinDrvImp_TransEnvData,
      .EnvDataLen             = sizeof(LinDrvImp_TransEnvData),
      .protoFrmDescInfo       = &protoFrmDescInfo,
      .nasTimeout             = LINDIAGIMP_NAS_TIMEOUT,
      .NcrTimeout             = LINDIAGIMP_NCR_TIMEOUT,
      .ImpCfgData             = &linTransImpCfgData,
      .InitCfgFlags           = linTransInitCfgFlags
    },

  #if LINDIAG_SUP_SNPD == 1
    .LinSNPDIfThisPointer = 
    {
      .IfFunsTbl              = LIN_NULL,
      .envData                = LIN_NULL
    },
    
    .ImmediateNADUpdate  = LIN_TRUE,
    .UseOldSNPDServiceID = LIN_FALSE,
  #endif

    .LinBusIfThisPointer =
    {
      .IfFunsTbl = LinDrvImp_BusIfFuns,
      .envData =  &LinDrvImp_BusEnvData
    },

    .LinLookupIfThisPointer =
    {
      .IfFunsTbl = LinDrvImp_LookupIfFuns,
      .envData =   LinDrvImp_LookupEnvData
    },

      .InitialProdIdent.SupplierID = LINDIAGIMP_SUPPLIERID,
      .InitialProdIdent.FunctionID = LINDIAGIMP_FUNCTIONID,
      .InitialProdIdent.VariantID  = savedConfig.ProductID[4],
      .InitialNad = LINDIAGIMP_INITIAL_NAD,
      .InitialSerialNumber = LINDIAGIMP_SERIALNUMBER
  };
 
  if(LinDrvImp_ProtoIfFuns->GetSubInterface(LinDrvImp_ProtoEnvData, Lin_IfId_LOOKUP, (Lin_pThis_t) &(linDiagImpCfgData.LinLookupIfThisPointer)) == LIN_FALSE)
  {
    linDiagImpCfgData.LinLookupIfThisPointer.IfFunsTbl = LIN_NULL;
    linDiagImpCfgData.LinLookupIfThisPointer.envData   = LIN_NULL;
  }
    
#if (LINDRVIMP_USE_SNPD_PROXY == 1u)    
  loc_Init_DiagLayer_SNPDProxySetup(&linDiagImpCfgData);
#endif /* LINDRVIMP_USE_SNPD_PROXY == 1 */    
 
#if (LINDIAG_SUP_SNPD == 1u) && (LINDRVIMP_USE_SNPD_PROXY != 1u)
  if(LinDrvImp_BusIfFuns->GetSubInterface(LinDrvImp_BusEnvData, Lin_IfId_SNPD, (Lin_pThis_t) &(linDiagImpCfgData.LinSNPDIfThisPointer)) == LIN_FALSE)//retrieve snpd if from bus layer
  {          
    linDiagImpCfgData.LinSNPDIfThisPointer.IfFunsTbl = LIN_NULL;
    linDiagImpCfgData.LinSNPDIfThisPointer.envData   = LIN_NULL;
  }
#endif
         
  retVal = LinDrvImp_DiagIfFuns->Initialization((LinDiagIf_pGenericEnvData_t) LinDrvImp_DiagEnvData, sizeof(LinDrvImp_DiagEnvData),
                                                &LinDrvImp_DiagCallbacks, LINDRVIMP_DIAG_CALLBACK_CTX_DATA,
                                                LIN_FALSE, &linDiagImpCfgData);
  /*　设置新的NAD */
  systemLinCommChangeNad((uint8_t)savedConfig.singleAddr);
        
  if(retVal == LIN_TRUE)
  {
    retVal = LinDrvImp_TransIfFuns->AddSIDDescriptionList((LinTransIf_pGenericEnvData_t) LinDrvImp_TransEnvData,
                                                          (LinTransIf_pSIDDescription_t) LinDrvImp_SIDDescrLst,
                                                          LIN_NULL, LIN_NULL);
  }
    
  if(retVal == LIN_TRUE)
  {
    retVal = LinDrvImp_DiagIfFuns->AddRbiTable((LinDiagIf_pGenericEnvData_t) LinDrvImp_DiagEnvData, (LinDiagIf_pRbiLookupEntry_t) LinDrvImp_RbiTbl);
  }
  return retVal;
}




/* **************************************************************************//**
 * @implementation
 * generic Task routine
 *
 * @param       none
 * @return      none
 * *****************************************************************************/
void LinDrvImp_Task(void)
{  
  if (LinDrvImp_Diag_triggerNADStorage == LIN_TRUE)
  {
    /* Clear storage flag. */
    LinDrvImp_Diag_triggerNADStorage = LIN_FALSE;
    
    /* Is there currently no ongoing LIN communication? */
    if (LinDrvImp_TransIfFuns->GetStatus((LinTransIf_pGenericEnvData_t) LinDrvImp_TransEnvData) == LinTransIf_ComStatus_IDLE)
    {
      /* Get current nad. */
      uint32_t loc_SimpleNadStorage = LinDrvImp_DiagIfFuns->GetNAD((LinDiagIf_pGenericEnvData_t) LinDrvImp_DiagEnvData, LinDiagIf_NADType_NORMAL);
      
      /* Store NAD to EEPROM. */
      if (EEImp_IfFuns->MemoryWrite(EE_PASSWORD, (eeprom_cpData_t)&loc_SimpleNadStorage, (uint16_t*)EXAMPLE_EE_NAD_LOC, 4, 0, 0) != TRUE)
      {
        /* Switch off blinking LED color for error signaling. */
        PWM_STOP2 = 0u;
      }
      else { /* Continue. */ }
    }
    else { /* Currently busy - try later. */ }
  }
  else { /* Continue. */ }
}


/*! @} */ /* SAMPLE_CONFIG */
/*! @} */ /* SAMPLE_PROJECT */

