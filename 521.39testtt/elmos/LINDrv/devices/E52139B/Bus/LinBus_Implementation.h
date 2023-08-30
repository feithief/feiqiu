/***************************************************************************//**
 * @file   LinBus_Implementation.h
 *
 * @creator      POE
 * @created      2021/11/16
 * @sdfv         Elmos Flow Grade 2
 *
 * @brief  LIN bus ("PHY") implementation (Code).
 *
 * @purpose
 *
 * Implements an abstraction layer which encapsulates all the low-level
 * transport effort. Basically an event based API is exposed, which
 * can be used by some higher-level protocol layer to implement a LIN stack.
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup LIN_E52139B E52139B
 * @{
 *
 * $Id: LinBus_Implementation.h 9426 2021-11-22 09:06:07Z poe $
 *
 * $Revision: 9426 $
 *
 ******************************************************************************
 *
 * Demo Code Usage Restrictions\:
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
 * @section history_sec_linbus_implementation_h Revision History
 * $Id: LinBus_Implementation.h 9426 2021-11-22 09:06:07Z poe $
 * @verbatim
 * -------------------------------------------------------------------
 * Date       B/F Who Ver  Comment
 * -------------------------------------------------------------------
 * 2021/11/17 (F) POE 1.3 updated config & config version 104 for EB.
 * 2021/09/10 (F) POE 1.2 updated config for EA1.
 * 2020/09/14 (F) POE 1.1 updated config & config version 102 for MC.
 * 2020/07/01 (F) POE 1.0 first creation for MA.
 * -------------------------------------------------------------------
 ******************************************************************************/

#ifndef LINBUS_IMPLEMENTATION_H_
#define LINBUS_IMPLEMENTATION_H_

/* ****************************************************************************/
/* ***************************** INCLUDES *************************************/
/* ****************************************************************************/

#include "LinBus_Interface.h"
#include "LinSNPD_interface.h"

/* ****************************************************************************/
/* ************************ DEFINES AND MACROS ********************************/
/* ****************************************************************************/
#define LINBUSIMP_CONFIG_DATA_VERSION      (0x0104u)  /**< Expected config data version */
#define LINBUSIMP_IGNORE_MSGTOUT_NEVER     0x00u    /**< Do not ignore message timeouts  */
#define LINBUSIMP_IGNORE_MSGTOUT_FOR_EVER  0xffu    /**< Ignore message timeouts for ever */

/* ****************************************************************************/
/* *************************** GLOBALE VARIABLES ******************************/
/* ****************************************************************************/
#ifdef LINBUSIMP_EXT_IFFUN_STRCT_ACCESS
extern const LinBusIf_sInterfaceFunctions_t  LinBusImp_InterfaceFunctions;
extern const LinSNPDIf_sInterfaceFunctions_t LinBusImp_SNPDInterfaceFunctions;
#endif /* LINBUSIMP_EXT_IFFUN_STRCT_ACCESS  */

/* ****************************************************************************/
/* ******************** STRUCTS, ENUMS AND TYPEDEFS ***************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Struct for BUS layer implementation specific configuration flags used in configuration data.
 ******************************************************************************/
typedef struct 
{
  Lin_uint8_t                         CallMeasDone              ;  /**< 1 = Allow application to validate baudrate measurement */
  Lin_uint8_t                         EnableHighSpeed           ;  /**< 1 = Switch LIN-Phy into high speed (aka "Flash" ) mode for baudrates >20k */
  Lin_uint8_t                         CheckBaudrate             ;  /**< 1 = Verify if selected baudrate can be exactly represented as a SCI clock divider value */ 
  Lin_uint8_t                         DetectPostPIDCollisions   ;  /**< 1 = Monitor the phase between PID reception and beginn of slave transmision for any disturbance or master activity. Report them as bus collisions */ 
  Lin_uint8_t                         EnableShortDetection      ;  /**< 1 = SCI will stop transmition immediately if LIN-Phy reports a short (to VBAT) condition */ 
  Lin_uint8_t                         EnableSynchByteExtension  ;  /**< 1 = Allow relaxed synch byte measurement.  */
} LinBusImp_sCfgDataFlags_t;
/***************************************************************************//**
 * @brief TxD Timeout monitoring 
 ******************************************************************************/
typedef enum 
{  
  LinBusImp_TXDTO_4MS           = 0u, /**< 4ms TxD Timeout  */ 
  LinBusImp_TXDTO_12MS          = 1u, /**< 12ms TxD Timeout */ 
  LinBusImp_TXDTO_20MS          = 2u, /**< 20ms TxD Timeout */ 
  LinBusImp_TXDTO_28MS          = 3u, /**< 28ms TxD Timeout */ 
  LinBusImp_TXDTO_36MS          = 4u, /**< 36ms TxD Timeout */ 
  LinBusImp_TXDTO_44MS          = 5u, /**< 44ms TxD Timeout */ 
  LinBusImp_TXDTO_52MS          = 6u, /**< 52ms TxD Timeout */ 
  LinBusImp_TXDTO_60MS          = 7u, /**< 60ms TxD Timeout */ 
  
  LinBusImp_TXDTO_DISABLED      = 255u, /**< TxD Timeout monitoring disabled */ 
    
} LinBusImp_eTxDTimeoutSelect_t; /* Take care: Enum value encoding related to HW register content, do not change! */ 

#define LinBusImp_TXDTO_DEFAULT LinBusImp_TXDTO_12MS

typedef Lin_uint32_t  LinBusImp_ClockFreqValue_t;  /**< typedef for clock frequency value. */

#define LINBUSIMP_DEBUG_DATA_INVALID_ADC 0xFFFFu

typedef enum
{
  LinBusImp_AAD_SAMPLE_DATA = 0,   /**< corresponding entry reports the next sampled value */
  LinBusImp_AAD_FINISCH     = 1,   /**< this entry has been submitted due to a AA finisch IRQ */
  LinBusImp_AAD_TIME_OUT    = 2,   /**< this entry has been submitted due to a AA timeout IRQ */
    
} LinBusImp_SNPDDebugDataEntryType_t;

/***************************************************************************//**
 * Data type definition for a SNPD debug data entry
 ******************************************************************************/
typedef struct
{
  LinBusImp_SNPDDebugDataEntryType_t EntryType;     /**< type of data reported */
  Lin_uint8_t                        SeqNumber;     /**< a sequence number, to be able to find data overruns. (Reset on a LinSNPDIf_StartMeasurementIfFun_t call) */ 
  
  Lin_uint16_t                       ADCValue;      /**< ampled ADC value in case of LinBusImp_AAD_SAMPLE_DATA, 0xffff otherwise) */
  Lin_uint16_t                       AAStatus;      /**< content of the LIN_CTRL status register */ 
  
} LinBusImp_SNPDDebugDataEntry_t;

/***************************************************************************//**
 * Data type definition for a pointer to SNPD debug data entries
 ******************************************************************************/
typedef const LinBusImp_SNPDDebugDataEntry_t * LinBusImp_cpSNPDDebugDataEntry_t;

    
/***************************************************************************//**
 * @brief Auto-Addressing configuration wait mode types enumerator
 ******************************************************************************/
typedef enum 
{
  LinBusImp_SNPD_STEP5_WAIT_IMMEDIATE    = 0u, /**< switch to pull-up conf. #2 (selection phase) at the beginning of step 5 */ 
  LinBusImp_SNPD_STEP5_WAIT_MID          = 1u  /**< switch to pull-up conf. #2 (selection phase) in the middle of step 5 */
  
} LinBusImp_eSNPDStep5WaitMode_t;

#define LinBusImp_SNPD_STEP5_WAIT_DEFAULT LinBusImp_SNPD_STEP5_WAIT_IMMEDIATE

/***************************************************************************//**
 * @brief Auto-Addressing configuration wait mode types enumerator
 ******************************************************************************/
typedef enum 
{
  LinBusImp_SNPD_STEP4_SW_WAIT_IMMEDIATE    = 0u, /**< do not wait for sw decision in step 4. */ 
  LinBusImp_SNPD_STEP4_SW_WAIT              = 1u  /**< wait for sw decision in step 4. */
  
} LinBusImp_eSNPDStep4WaitMode_t;

#define LinBusImp_SNPD_STEP4_SW_WAIT_DEFAULT LinBusImp_SNPD_STEP4_SW_WAIT_IMMEDIATE

/***************************************************************************//**
 * @brief Auto-Addressing configuration break mode types enumarator
 ******************************************************************************/
typedef enum 
{
  LinBusImp_SNPD_EOB_12TBIT             = 0u, /**< restore default pull-up config after 12 Tbit */
  LinBusImp_SNPD_EOB_13TBIT             = 1u  /**< restore default pull-up config after 13 Tbit (LIN BSM spec. conform) */
    
} LinBusImp_eSNPDEndOfBreakMode_t;

#define LinBusImp_SNPD_EOB_DEFAULT LinBusImp_SNPD_EOB_12TBIT

/***************************************************************************//**
 * @brief Auto-Addressing configuration reverse current detection enumerator
 ******************************************************************************/
/*-->2021-11-17, poe: ESWBL-97
 * Updated due to rc detect merge in hardware.*/
typedef enum 
{
  LinBusImp_SNPD_REVERSECURRDETECT_OFF             = 0u, /**< Disable reverse current detection for auto addressing. */
  LinBusImp_SNPD_REVERSECURRDETECT_ON              = 1u  /**< Enable reverse current detection for auto addressing. */
    
} LinBusImp_eSNPDReverseCurrentDetect_t;

#define LinBusImp_SNPD_REVCURRDETECT_DEFAULT LinBusImp_SNPD_REVERSECURRDETECT_ON

/*-->2021-11-17, poe: ESWBL-97 */
/***************************************************************************//**
 * @brief Auto-Addressing output current destination enumerator
 ******************************************************************************/
/*-->2021-11-17, poe: Datasheet synch
 * Updated typedef according to datasheet spec -> LIN_M output = 1. */
typedef enum 
{
  LinBusImp_SNPD_OUTPUT_CURRENT_LIN_S             = 0u, /**< Output current source to LIN_S. */
  LinBusImp_SNPD_OUTPUT_CURRENT_LIN_M             = 1u  /**< Output current source to LIN_M. */
    
} LinBusImp_eSNPDOutputCurrentDestination_t;

#define LinBusImp_SNPD_OUTPUT_CURRENT_DEFAULT LinBusImp_SNPD_OUTPUT_CURRENT_LIN_M
/*-->2021-11-17, poe: Datasheet synch */


/***************************************************************************//**
 * @brief Auto-Addressing configuration oversample mode types enumarator
 ******************************************************************************/
typedef enum 
{  
  LinBusImp_SNPD_OVERSAMPLE_2X          = 0u, /**< calculate average adc value over 2 samples */
  LinBusImp_SNPD_OVERSAMPLE_4X          = 1u, /**< calculate average adc value over 4 samples */
  LinBusImp_SNPD_OVERSAMPLE_8X          = 2u, /**< calculate average adc value over 4 samples */
  LinBusImp_SNPD_OVERSAMPLE_16X         = 3u, /**< calculate average adc value over 4 samples */
    
} LinBusImp_eSNPDOversamplingMode_t;

#define LinBusImp_SNPD_OVERSAMPLE_DEFAULT LinBusImp_SNPD_OVERSAMPLE_4X

/*-->2021-11-17, poe: spec synch
 * For the 521.39 gain default value for type 1 shouldalso  be set to 3. */
#define LinBusImp_SNPD_LIN_AA_GAIN_TYPE1_DEFAULT  3u
/*-->2021-11-17, poe: spec synch */
// justification: To have symmetry in the names of same interface macros. We can ignore it 
// PRQA S 0791 ++
#define LinBusImp_SNPD_LIN_AA_GAIN_TYPE23_DEFAULT 3u
// PRQA S 0791 --


/***************************************************************************//**
 * Data type definition for AA adc trigger select
 ******************************************************************************/
typedef enum
{
  LinBusImp_eFSMDiffSelect_OFF     = 0u,
  LinBusImp_eFSMDiffSelect_ON      = 1u

} LinBusImp_eFSDMDiffSelect_t;

/***************************************************************************//**
 * Data type definition for AA adc trigger select
 ******************************************************************************/
typedef enum
{
  LinBusImp_eADCTRIGGER_ENABLED       = 0u,
  LinBusImp_eADCTRIGGER_DISABLED      = 1u

} LinBusImp_eADCTriggerSelect_t;

/*-->2021-11-17, poe: ESWBL-97
 * Added typedef for new rc stop flag. */
/***************************************************************************//**
 * Data type definition for AA rc stop select
 ******************************************************************************/
typedef enum
{
  LinBusImp_eRC_STOP_DISABLED       = 0u,
  LinBusImp_eRC_STOP_ENABLED        = 1u

} LinBusImp_eRCStop_Select_t;
/*-->2021-11-17, poe: ESWBL-97 */



/***************************************************************************//**
 * Data type definition for SNPD Pullup current
 ******************************************************************************/
typedef Lin_uint8_t LinBusImp_SNPDPullUpCurrent_t;
#define LinBusImp_SNPD_PU_CURRENT_DEFAULT 16 /* 2mA = 16 * 0.1 mA + 0.4 mA */

/***************************************************************************//**
 * Data type definition for SNPD Shunt value
 ******************************************************************************/
typedef Lin_uint8_t   LinBusImp_SNPDShuntValue_t;
/***************************************************************************//**
 * Data type definition for SNPD Plus step max
 ******************************************************************************/
typedef Lin_uint8_t   LinBusImp_SNPDPlusStepMax_t;
/***************************************************************************//**
 * Data type definition for SNPD plus step
 ******************************************************************************/
typedef Lin_uint16_t  LinBusImp_SNPDPlusStep_t;
/***************************************************************************//**
 * Data type definition for SNPD threshold
 ******************************************************************************/
typedef Lin_uint16_t  LinBusImp_SNPDThres_t;

/***************************************************************************//**
 * Data type definition for SNPD max shunt threshold
 ******************************************************************************/
typedef Lin_uint8_t LinBusImp_MaxShuntConfig_t;

/***************************************************************************//**
 * Data type definition for SNPD reverse current detection threshold
 ******************************************************************************/
typedef Lin_uint16_t LinBusImp_ReverseCurrentDetectionThreshold_t;

/***************************************************************************//**
 * Structure definition for SNPD callback config
 ******************************************************************************/
struct LinBusImp_sSNPDCallbackConfig
{
  LinBusImp_SNPDThres_t         pre_thres;
  LinBusImp_SNPDThres_t         sel_thres;
  LinBusImp_SNPDThres_t         type3_thres;
  LinBusImp_SNPDShuntValue_t    max_iShunt;
  LinBusImp_SNPDPlusStepMax_t   max_stepSize;
  LinBusImp_SNPDPlusStep_t      stepSize;
};

/***************************************************************************//**
 * Data type definition for SNPD config data flags
 ******************************************************************************/
/* -->2021-11-17, poe: ESWBL-97
 * Updated due to reverse current detection merge in hardware. */
typedef struct
{
  LinBusImp_eSNPDReverseCurrentDetect_t   EnableReverseCurrentDetection;
 }LinBusImp_sSNPDCfgDataFlags_t;
/* -->2021-11-17, poe: ESWBL-97 */
/***************************************************************************//**
 * Data type definition for AA measurement mux select
 ******************************************************************************/
typedef enum
{
  LinBusImp_AAMEASMUX_LOW    = 0u,
  LinBusImp_AAMEASMUX_HIGH   = 1u
} LinBusImp_eAAMeasurementMuxSelect_t;

/*-->2021-11-17, poe: ESWBL-101
 * removed obsolete configuration typedefs
 * for filterSelect and offsetSelect. */
 /*-->2021-11-17, poe: ESWBL-101 */

/***************************************************************************//**
 * Data type definition for AA Amp select
 ******************************************************************************/
typedef enum
{
  LinBusImp_AAAMPSEL_LOW    = 0u,
  LinBusImp_AAAMPSEL_HIGH   = 1u
} LinBusImp_eAAAmpSelect_t;

/***************************************************************************//**
 * Data type definition for AA CM measurement select
 ******************************************************************************/
typedef enum
{
  LinBusImp_AACMMEAS_DISABLED  = 0u,
  LinBusImp_AACMMEAS_ENABLED   = 1u
} LinBusImp_eAACMMeasSelect_t;

/***************************************************************************//**
 * Data type definition for Amp Gain select
 ******************************************************************************/
typedef enum
{
  LinBusImp_AAGAIN_0  = 0u,
  LinBusImp_AAGAIN_1  = 1u,
  LinBusImp_AAGAIN_2  = 2u,
  LinBusImp_AAGAIN_3  = 3u
} LinBusImp_eAmpGainSelect_t;

/***************************************************************************//**
 * Data type definition for SNPD physical config
 ******************************************************************************/
typedef struct
{
  LinBusImp_eAAMeasurementMuxSelect_t       MeasInMuxSelect         ;
  LinBusImp_eAmpGainSelect_t                GainSelect              ;
/*-->2021-11-17, poe: ESWBL-101
 * removed obsolete configuration typedefs
 * for filterSelect and offsetSelect. */
 /*-->2021-11-17, poe: ESWBL-101 */
  LinBusImp_eAAAmpSelect_t                  AmpSelect               ;
  LinBusImp_eAACMMeasSelect_t               EnableCommonModeMeas    ;
  LinBusImp_eSNPDOutputCurrentDestination_t OutputCurrentDestination;
} LinBusImp_sCfgSNPDPhyConfig_t;

/***************************************************************************//**
 * Data type definition for SNPD state machine config
 ******************************************************************************/
typedef struct 
{
  LinBusImp_eSNPDEndOfBreakMode_t EndOfBreakMode            ; 
  LinBusImp_eSNPDStep4WaitMode_t  Step4WaitMode             ;
  LinBusImp_eSNPDStep5WaitMode_t  Step5WaitMode             ;
  LinBusImp_eADCTriggerSelect_t   DisableADCTrigger         ;
  LinBusImp_eFSDMDiffSelect_t     FSMDiffSelect             ; 
/*-->2021-11-17, poe: ESWBL-97
 * Added rc stop flag. */  
  LinBusImp_eRCStop_Select_t      RevCurrentStop            ;
/*-->2021-11-17, poe: ESWBL-97 */
} LinBusImp_sSNPDFSMCfg_t;

/***************************************************************************//**
 * Data type definition for mode configuration
 ******************************************************************************/
typedef struct
{
  Lin_Bool_t                          EnablePullUp;
  Lin_Bool_t                          EnableCurrentSources;
  Lin_Bool_t                          EnableCurrentRamp;
  LinBusImp_SNPDPullUpCurrent_t       SNPDPullUpCurrent;
  LinBusImp_eSNPDOversamplingMode_t   Oversampling;
/*-->2021-11-17, poe: ESWBL-97
 * added filter bit to mode config. */  
  Lin_Bool_t                          EnableFilter;
/*-->2021-11-17, poe: ESWBL-97 */  
}LinBusImp_ModeConfig_t;

/***************************************************************************//**
 * Data type definition for SNPD callback config
 ******************************************************************************/
typedef struct  LinBusImp_sSNPDCallbackConfig   LinBusImp_SNPDCallbackConfig_t;
/***************************************************************************//**
 * Data type definition for pointer to SNPD callback config
 ******************************************************************************/
typedef         LinBusImp_SNPDCallbackConfig_t* LinBusImp_pSNPDCallbackConfig_t;
/***************************************************************************//**
 * Data type definition for constant pointer to SNPD callback config
 ******************************************************************************/
typedef const   LinBusImp_SNPDCallbackConfig_t* LinBusImp_cpSNPDCallbackConfig_t;


#define LinBusImp_SNPD_THRES_PRE_DEFAULT 100  /* TODO: t.b.d. */
#define LinBusImp_SNPD_THRES_SEL_DEFAULT 75  /* TODO: t.b.d. */

#define  LinBusImp_SNPDPLUS_THRES_SEL_DEFAULT     0x0Bu /* TODO: t.b.d. */
#define  LinBusImp_SNPDPLUS_MAX_ISHUNT_DEFAULT    0x06u /* TODO: t.b.d. */
#define  LinBusImp_SNPDPLUS_MAX_STEPSIZE_DEFAULT  0x0Au /* TODO: t.b.d. */
#define  LinBusImp_SNPDPLUS_STEPSIZE_DEFAULT      0xF0u /* TODO: t.b.d. */
/*-->2021-11-17, poe: ESWBL-101
 * Removed remove short debounce typedef. */
/*-->2021-11-17, poe: ESWBL-101 */

/***************************************************************************//**
 * Data type definition for Type3 measurement only select
 ******************************************************************************/
typedef enum
{
  LinBusImp_Type3MeasOnly_OFF  = 0u,
  LinBusImp_Type3MeasOnly_ON   = 1u
} LinBusImp_eType3MeasOnly_t;

/***************************************************************************//**
 * Data type definition for meas only selection
 ******************************************************************************/
/*-->2021-11-19, poe: ESWBL-97,
 * Without update of max i shunt, the LinBusImp_Type3MeasOnlyConfig struct
 * has been replaced with a simple enable MeasOnly flag. */
/*-->2021-11-19, poe: ESWBL-97 */

typedef Lin_Bool_t LinBusImp_Type12LINSwapDetectConfig_t;
typedef Lin_Bool_t LinBusImp_Type12LINSwapCancelOnError_t;
/* ****************************************************************************/
/* ******************** EXTERNAL FUNCTIONS / INTERFACE ************************/
/* ****************************************************************************/

/*! @addtogroup LinBusImpInterfaceFunctions */
/*! @{ */

/***************************************************************************//**
 * LIN BUS layer 'Initialization' function.
 *
 * @param[in] genericBusEnvData   Pointer to reserved BUS Layer environment data.
 * @param[in] busEnvDataSze       Size of the reserved RAM for BUS environment
 *                                data.
 * @param[in] busCbFuns           Pointer to BUS Layer callback function.
 *                                struct. Implemented in the higher layer or
 *                                user application.
 *                                (LinBusTrIf_sCallbackFunctions).
 * @param[in] genericBusCbCtxData Pointer to BUS Layer callback context data.
 * @param[in] init_baudrate       Initial baudrate.
 * @param[in] genericImpCfgData   Pointer to implementation dependent
 *                                configuration data for the BUS Layer
 *                                (LinBusImp_sCfgData).
 *
 * @return  LIN_TRUE if the initialization was successfully LIN_FALSE if not.
 *
 * Initializes the BUS Layer.
 *
 ******************************************************************************/
Lin_Bool_t LinBusImp_Initialization(LinBusIf_pGenericEnvData_t     genericBusEnvData, LinBusIf_EnvDataSze_t         busEnvDataSze,
                                    LinBusIf_cpCallbackFunctions_t busCbFuns,         LinBusIf_pGenericCbCtxData_t  genericBusCbCtxData,
                                    LinBusIf_Baudrate_t            init_baudrate,     LinBusIf_pGenericImpCfgData_t genericImpCfgData);

/***************************************************************************//**
 * Get address pointer to the implementation of the Bus- Layer subinterface with the given interface id.
 * In this case only pointer to SNPD interface functions is available.
 *
 * @param genericBusEnvData Struct pointer to LIN Bus environment data.
 * @param interfaceId Id of the implemented subinterface.
 * @param ifThisPtr Pointer to itself
 *
 * @return Returns "FALSE" if Interface Id is not known resp. not implemented or if pointer to environment data is  a null pointer.
 *
 ******************************************************************************/
Lin_Bool_t LinBusImp_GetSubInterface(LinBusIf_pGenericEnvData_t genericBusEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * Implementation of LIN BUS layer 'Task' function.
 *
 * The Task function has to be called periodically to process scheduled task of
 * the BUS Layer.
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
 *
 ******************************************************************************/
 void LinBusImp_Task(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Get the  milliseconds counter value.
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  *
  * @return The milliseconds counter value.
  *
  ******************************************************************************/
 LinBusIf_Tick_t LinBusImp_GetMilliseconds(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Shutdown the LIN communication. It disconnects the LIN driver from the
  * physical bus.
  *
  * Terminates genericBusEnvData processing, setup hardware to a safe mode and go to state
  * LBS_OFFLINE.
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  *
  ******************************************************************************/
 void LinBusImp_ShutdownBus(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
 * Set's the LIN driver to the Sleep-Mode. It doesn't respond to LIN frames
 * anymore. The same mode will be set if the idle timeout has been reached.
  *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 *
  ******************************************************************************/
 void LinBusImp_GoToSleep(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Get the state of the BUS Layer.
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  *
  * @return  The state of the BUS Layer.
  *
  ******************************************************************************/
 LinBusIf_eState_t LinBusImp_GetBusState(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Set the LIN bus baud rate.
  *
  * The 'new_baudrate' value will be used as current expected baud rate,
  * sync measurement baud rate and timeouts are updated accordingly.
  *
  * New baud rate will become effective the next time genericBusEnvData starts
  * break detection and sync measurement.
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  * @param[in] new_baudrate      New baudrate to set
  * @param[in] try_only          Set to LIN_TRUE to try to set the new baudrate
  *                              first.
  *
  * @return Returns LIN_TRUE if the baud rate has been set successfully.
  *
  ******************************************************************************/
Lin_Bool_t LinBusImp_BusChangeBaudrate(LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_Baudrate_t new_baudrate,Lin_Bool_t try_only);

/***************************************************************************//**
 * Change genericBusEnvData idle timeout.
 *
 * Set the LIN idle timeout.
 * New idle time will become effective after next message has been processed (or
 * after LinBusImp_RestartBus() )
 *
 *
 * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
 * @param[in] timeout           Desired idle timeout.
 *
 *
 * @see LIN 2.2 Specification - 2.6.1 SLAVE COMMUNICATION STATE DIAGRAM
 *
 ******************************************************************************/
 void LinBusImp_BusChangeIdleTimeout(LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_TimeOut_t timeout);

 /***************************************************************************//**
  *  Changes genericBusEnvData idle timeout.
  *
  * Sets the LIN idle timeout.
  * New idle time will become effective after next message has been processed (or
  * after LinBusImp_RestartBus() )
  *
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  * @param[in] timeout           Desired idle timeout.
  *
  *
  * @see LIN 2.2 Specification - 2.6.1 SLAVE COMMUNICATION STATE DIAGRAM
  *
  ******************************************************************************/
 void LinBusImp_RestartBus(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Sends a wakeup signal for the LIN cluster.
  *
  * Afterwards the genericBusEnvData will be in LBS_SYNCING state.
  *
  * Currently this method is blocking. It will return after the wakeup has been
  * completed (see LINBUSIMP_DEFAULT_WAKEUP_DURATION (~2,5 ms)).
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  *
  ******************************************************************************/
 void LinBusImp_WakeupCluster(LinBusIf_pGenericEnvData_t genericBusEnvData);

 /***************************************************************************//**
  * Sends a LIN frame header to the bus. (Normally only for Master-functionality)
  *
  * Once the PID has been send out a LINBUSIMP_EventPIDReceivedCallback_t is
  * reported to continue the message body.
  *
  * There is no queuing, once header sending is initiated, other requests are
  * ignored up to PID reporting.
  *
  * @param[in] genericBusEnvData Pointer to reserved BUS Layer environment data.
  * @param[in] frameID           Frame ID for frame header.
  *
  * @return LIN_TRUE, if the header has been send to the LIN bus.
  *
  ******************************************************************************/
Lin_Bool_t LinBusImp_SendHeader(LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_FrameID_t frameID);

/*! @} */ /* LinBusImpInterfaceFunctions */

#if (LINBUSIMP_ENABLE_AUTO_ADDR == 1u)

/*! @addtogroup LinSNPDImpInterfaceFunctions */
/*! @{ */

/***************************************************************************//**
 * Initializes SNPD functionality.
 *
 * @param genericSNPDEnvData    Pointer to reserved SNPD environment data
 * @param SNPDEnvDataSze        size of SNPD environment data
 * @param genericSNPDImpCfgData SNPD configuration data
 *
 * @ return LIN_TRUE for success, otherwise LIN_FALSE.
 ******************************************************************************/
Lin_Bool_t LinBusImp_SNPDInitialization(LinSNPDIf_pGenericEnvData_t    genericSNPDEnvData, LinSNPDIf_EnvDataSze_t SNPDEnvDataSze,
                                                  LinSNPDIf_pGenericImpCfgData_t genericSNPDImpCfgData);

/***************************************************************************//**
 * Gets SNPD subinterface.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 * @param interfaceId        id of subinterface
 * @param ifThisPtr          pointer to subinterface if found
 *
 * @ return LIN_TRUE if subinterfac was found, otherwise LIN_FALSE.
 ******************************************************************************/
Lin_Bool_t LinBusImp_SNPDGetSubInterface(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr);

/***************************************************************************//**
 * Starts SNPD measurement.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 *
 * @ return LIN_TRUE for success, otherwise LIN_FALSE.
 ******************************************************************************/
Lin_Bool_t LinBusImp_SNPDStartMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * Ends SNPD measurement.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 *
 ******************************************************************************/
void LinBusImp_SNPDEndMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * Starts next SNPD measurement.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 *
 * @ return LIN_TRUE for success, otherwise LIN_FALSE.
 ******************************************************************************/
Lin_Bool_t LinBusImp_SNPDNextMeasurement(LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_Bool_t addressed);

/***************************************************************************//**
 * Gets SNPD state.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 *
 * @ return actual SNPD state.
 ******************************************************************************/
LinSNPDIf_eState_t LinBusImp_SNPDGetState(LinSNPDIf_pGenericEnvData_t   genericSNPDEnvData);

/***************************************************************************//**
 * Gets SNPD measurement count.
 *
 * @param genericSNPDEnvData Pointer to reserved SNPD environment data
 *
 * @ return actual measurement count.
 ******************************************************************************/
LinSNPDIf_MeasCount_t LinBusImp_SNPDGetMeasurementCount(LinSNPDIf_pGenericEnvData_t   genericSNPDEnvData);

/*! @} */ /* LinSNPDImpInterfaceFunctions */
#endif


/*! @addtogroup LinSNPDImpInterfaceCbFunctions */
/*! @{ */

/***************************************************************************//**
 * Type of callback to get SNPD threshold values.
 *
 * @return         values filled into corresponding return parameter.
 *
 ******************************************************************************/
typedef void (* LinBusImp_GetSNPDThresholdsCbFun_t) (LinBusIf_pGenericEnvData_t busEnvData, LinBusIf_cpInterfaceFunctions_t busIfFuns, LinBusImp_SNPDThres_t *pre_thres, LinBusImp_SNPDThres_t *sel_thres, LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * Type of callback to get SNPD Type3 parameter values.
 *
 * @return         values filled into corresponding return parameter.
 *
 ******************************************************************************/
typedef void (* LinBusImp_GetSNPDPlusParametersCbFun_t) (LinBusIf_pGenericEnvData_t busEnvData, LinBusIf_cpInterfaceFunctions_t busIfFuns, LinBusImp_pSNPDCallbackConfig_t snpdConfigData, LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/***************************************************************************//**
 * Type of callback which will be inwoked for SNPD sample data reports.
 *
 * @param genericBusEnvData     Pointer to reserved RAM space for BUS environment
 *                              data.
 *
 ******************************************************************************/
typedef void (* LinBusImp_PushSNPDDebugDataCbFun_t) (LinBusIf_pGenericEnvData_t genericBusEnvData, LinBusIf_cpInterfaceFunctions_t busIfFuns,
                                                    LinBusImp_cpSNPDDebugDataEntry_t debug_data, LinBusIf_pGenericCbCtxData_t genericBusCbCtxData);

/*! @} */ /* LinSNPDImpInterfaceCbFunctions */

/***************************************************************************//**
 * Struct for BUS layer implementation specific configuration data.
 ******************************************************************************/
struct LinBusImp_sCfgData
{
  Lin_Version_t                                 Version;             /* Config data struct version */
  
  LinBusImp_ClockFreqValue_t                    ClockFrequency;
  
  LinBusImp_sCfgDataFlags_t                     ConfigFlags;
  Lin_uint8_t                                   DebouncerValue;
/*-->2021/11/17, poe: ESWBL-101
 * Removed short debounce parameter. */
/*-->2021/11/17, poe: ESWBL-101 */  
  Lin_uint8_t                                   SendHeaderBreakLen;
  Lin_uint8_t                                   IgnoreMsgTimeouts;   /* Ignore message timeouts for n initial messages (successfull PID reception is the counting point) (0x00 = never, 0xff = for ever)  */
  LinBusImp_eTxDTimeoutSelect_t                 TxDTimeoutSelect;    /* Configure TxD Timeout  */
  
#if (LINBUSIMP_ENABLE_AUTO_ADDR  == 1u)
  LinSNPDIf_SNPDMode_t                          SNPDModeSelect;
  LinBusImp_sSNPDCfgDataFlags_t                 SNPDConfigFlags;

  LinBusImp_sCfgSNPDPhyConfig_t                 SNPDPhyConfigSelect;
  LinBusImp_sSNPDFSMCfg_t                       SNPDFSMConfigSelect;
  LinBusImp_ModeConfig_t                        ConfigMode_Default;
  LinBusImp_ModeConfig_t                        ConfigMode_Offset;
  LinBusImp_ModeConfig_t                        ConfigMode_Preselect;
  LinBusImp_ModeConfig_t                        ConfigMode_Select;
  LinBusImp_ReverseCurrentDetectionThreshold_t  ReverseCurrentDetectionThreshold;  /**< Threshold for reverse current detection of type3 FSM. */
  Lin_Bool_t                                    EnableType3MeasOnlyUsage;
  LinBusImp_ReverseCurrentDetectionThreshold_t  ReverseCurrentPreselectDetectionThreshold;
/*-->2021/11/19, poe: ESWBL-97,
 * Removed obsolete SW lin swap relevant parameters.*/
  Lin_Bool_t                                    SNPDLINSwapCancelOnError;
 /*-->2021/11/19, poe: ESWBL-97 */
  LinBusImp_PushSNPDDebugDataCbFun_t            SNPDDebugDataFun;           /* Will be called for each AA sample and final/failure status (only, if compiled with AA debug enabled) */
  LinBusImp_GetSNPDPlusParametersCbFun_t        SNPDPlusParametersFun;     /* Will be called in the start measurement function to update current threshold values (Type 3). */

#endif
};

/***************************************************************************//**
 * Data type definition for BUS layer implementation specific configuration data.
 ******************************************************************************/
typedef struct LinBusImp_sCfgData    LinBusImp_CfgData_t;  /* Typedef of LinBusImp_sCfgData. */
/***************************************************************************//**
 * Data type definition for pointer to BUS layer implementation specific configuration data.
 ******************************************************************************/
typedef        LinBusImp_CfgData_t* LinBusImp_pCfgData_t;  /* Typedef of pointer to LinBusImp_sCfgData. */
typedef const  LinBusImp_CfgData_t* LinBusImp_cpCfgData_t; /* Typedef of pointer to LinBusImp_sCfgData const*. */



#endif /* LINBUS_IMPLEMENTATION_H_ */

/*! @} */ /* E52139B */
/*! @} */ /* ELMOS_LIN_DRIVER */
