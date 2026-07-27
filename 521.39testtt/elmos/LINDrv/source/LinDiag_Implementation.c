/***************************************************************************//**
 * @file        LinDiag_Implementation.c
 *
 * @creator     sbai
 * @created     12.02.2015
 *
       Implements the LIN Diagnostic Layer
 *
 * @purpose     -Handles Diagnostic services
 *              
 *
 * @addtogroup ELMOS_LIN_DRIVER Elmos LIN Driver
 * @{
 * @addtogroup DIAG DIAG Layer
 * @{
 *
 * $Id$
 *
 * $Revision$
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

/* ****************************************************************************/
/* ******************************** INCLUDES **********************************/
/* ****************************************************************************/
#include "LinDiag_Implementation.h"
#include "LinDrvImp_CompilationConfig.h"
#include "Lin_Basictypes.h"
#include "el_helper.h"

#include "moduleFlash.h"
#if LINDIAG_SUP_DATASTG == 1
  #include "LinDataStg_Implementation.h"
  #include "LinDataStg_DataValueIDs.h"
#endif

#if LINDIAG_SUP_SNPD == 1
  #include "LinDiag_Types.h"
#endif

#if LINDRVIMP_CHECK_DEFS_ENVDATA_SIZES == 1
#include "Lin_EnvDataSizes.h"
#include "assert.h"
#endif

/* ****************************************************************************/
/* *************************** DEFINES AND MACROS *****************************/
/* ****************************************************************************/
#if LINDRVIMP_AQR_MOD_SZE == 1
#define DIAG_LAYER_CODE _Pragma("location=\"DIAG_LAYER_CODE\"")
#define DIAG_LAYER_DATA _Pragma("location=\"DIAG_LAYER_DATA\"")
#else
#define DIAG_LAYER_CODE
#define DIAG_LAYER_DATA
#endif

#define LOC_MAX_RBI_DATA_LEN           (5u)

#define LOC_SUB_FUNCTION_MSK           (0x7Fu)
#define LOC_SUPPRESS_POS_RESP_BIT_MSK  (0x80u)

#define LOC_TIMER_OVERFLOW_BIT         (0x80000000)
#define LOC_TIMER_MAX_VAL              (0x7FFFFFFF)

// LIN 2.0 chap. 2.5 (page 121)
// The unassign frame id: PID value if service B1 is sent (assign_frame_id)
#define LOC_UNASSIGN_FRAME_ID_B1   (0x40)

// LIN 2.2 chap. 4.2.5.5 (page 77)
// The unassign frame id: PID value if service B7 is sent (assign_frame_id_range)
#define LOC_UNASSIGN_FRAME_ID_B7   (0x00)


#if (LINDIAG_SUP_ASSIGN_NAD == 0) && (LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER == 0) && (LINDIAG_SUP_READ_BY_ID == 0) && \
    (LINDIAG_SUP_CONDITIONAL_CHANGE_NAD == 0) && (LINDIAG_SUP_SNPD == 0) && (LINDIAG_SUP_SAVE_CONFIGURATION == 0) && \
    (LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE == 0)
   #error "No diagnostic service activated! No need for the LIN DIAG LAYER!"
#endif

#if (LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER == 1) || (LINDIAG_SUP_READ_BY_ID == 1)
    #define LOC_USE_APPENDDATATOMSGBUFFFER_FUN
#endif

/* ****************************************************************************/
/* *********************** STRUCTS, ENUMS AND TYPEDEFS ************************/
/* ****************************************************************************/

/***************************************************************************//**
 * @brief Timer struct for diagnostic layer multi timeout measurement.
 ******************************************************************************/
struct loc_sDiagTimer
{
  Lin_uint32_t                  Start;                  /* @brief timer start offset. */
  LinDiagIf_Timeout_t           Timeout;                /* @brief timeout timer threshold. */
  LinDiagIf_pGenericCbCtxData_t CbCtxData;              /* @brief callback context data in case of timeout occurrence. */
};

typedef struct loc_sDiagTimer loc_sDiagTimer_t;         /* @brief typedef for diagnostic layer timer control unit. */

/***************************************************************************//**
 * @brief Local environment data of diagnostic layer
 ******************************************************************************/
struct loc_sDiagEnvironmentData
{
  LinTransIf_NAD_t                   InitialNAD;                        /* @brief Initial NAD. */
  LinTransIf_NAD_t                   NAD;                               /* @brief Assigned NAD. */
  LinDiagIf_sProductIdentification_t ProductIdentification;             /* @brief LIN product information. */
  LinDiagIf_SerialNumber_t           SerialNumber;                      /* @brief LIN serial number. */
  Lin_Bool_t                         InvalidReadByIDAnswered;   
  LinTransIf_cpInterfaceFunctions_t  TransIfFuns;                       /* @brief LIN transport layer interface ptr. */
  LinTransIf_pGenericEnvData_t       TransEnvData;                      /* @brief LIN transport layer environment data ptr. */
  LinLookupIf_sThis_t                LookupThisPtr;                     /* @brief LIN lookup layer interface ptr. */
  LinBusIf_sThis_t                   BusThisPtr;                        /* @brief LIN bus layer interface ptr. */
#if LINDIAG_SUP_DATASTG == 1    
  LinDataStgIf_sThis_t               DataStgThisPtr;                    /* @brief LIN datastorage layer interface ptr. */
#endif  
  LinDiagIf_cpCallbackFunctions_t    Callbacks;                         /* @brief LIN diagnostic layer callback table. */
  LinDiagIf_pGenericCbCtxData_t      CallbackCtxData;                   /* @brief LIN diagnostic layer callback context data ptr. */
  LinDiagIf_pRbiLookupEntry_t        RbiTbl[LINDIAG_MAX_RBI_TBL_CNT];   /* @brief LIN rbi lookup table. */
  LinDiagIf_Length_t                 UserAppendedDataLen;
#if LINDIAG_MAX_SUPPORTED_TIMER > 0
  loc_sDiagTimer_t                   Timer[LINDIAG_MAX_SUPPORTED_TIMER];
#endif
#if LINDIAG_SUP_SNPD == 1
  LinSNPDIf_sThis_t                  LinSNPDIfThisPointer;
  Lin_Bool_t                         ImmediateNADUpdate;                /* @brief TRUE - NAD is assigned during KEEP NAD frame; FALSE - NAD is assigned during STORE NAD frame. */
  Lin_Bool_t                         UseOldSNPDServiceID;               /* @brief Assign the SNPD autoaddressing handler to the old SNPD service id 0xB5. */
  LinTransIf_NAD_t                   SNPDNewNAD;                        /* @brief Assigned new nad from LIN autoaddressing cycle. */
  Lin_Bool_t                         SNPDNewNADValid;                   /* @brief TRUE - assigned new nad is valid; FALSE - else. */
  Lin_Bool_t                         SNPDStarted;                       /* @brief TRUE - LIN autoadressing sequence is actively running; FALSE - no autoaddressing active. */
  Lin_Bool_t                         SNPDAddressed;                     /* @brief TRUE - this device is already addressed; FALSE - this device has not been addressed yet. */
#endif
};

typedef struct loc_sDiagEnvironmentData    loc_sDiagEnvironmentData_t;
typedef        loc_sDiagEnvironmentData_t* loc_pDiagEnvironmentData_t;

enum loc_eReqLenCompType
{
  loc_RLCT_GreaterThenOrEqual,
  loc_RLCT_Equal,
  loc_RLCT_LessThenOrEqual
};

typedef enum loc_eReqLenCompType loc_eReqLenCompType_t;

struct loc_sDiagServiceControlData
{
    loc_pDiagEnvironmentData_t DiagEnvData;
    LinTransIf_eSIDReaction_t  SIDReaction;
    LinTransIf_SID_t           RequestedSID;
    LinDiagIf_Length_t         RequestLength;
    loc_eReqLenCompType_t      ReqLenCompType;
    Lin_Bool_t                 SupportSubFunction;
    Lin_uint8_t                SubFunction;
    Lin_Bool_t                 SuppressPosRspMsg;
    Lin_Bool_t                 SendNegativeResponse;
    LinDiagIf_NRC_t            NegativeResponseCode;
};

typedef struct loc_sDiagServiceControlData    loc_sDiagServiceControlData_t;
typedef        loc_sDiagServiceControlData_t* loc_pDiagServiceControlData_t;


/* ****************************************************************************/
/* ******************************** ASSERTS ***********************************/
/* ****************************************************************************/

#if LINDRVIMP_CHECK_DEFS_ENVDATA_SIZES == 1
/* Check reserved dataspace for layer environment data during compile-time. */
static_assert(sizeof(loc_sDiagEnvironmentData_t) == LINDIAGIMP_ENVIRONMENT_DATA_SZE, "size does not match");
#endif

/* ****************************************************************************/
/* ******************** FORWARD DECLARATIONS / PROTOTYPES *********************/
/* ****************************************************************************/

/***************************************************************************//**
 * Error callback from LIN transport layer.
 *
 * @param thisTransIf           - Transport layer interface ptr struct,
 * @param error                 - error code that occurred,       
 * @param nad                   - error connected NAD,
 * @param sidtype               - error connected sid type, 
 * @param sid                   - error connected sid,
 * @param genericTransCbCtxData - callback context data
 *
 * @return none
 *
 ******************************************************************************/
DIAG_LAYER_CODE static void loc_TransErrorCallback( LinTransIf_cpThis_t thisTransIf,
                                                    LinTransIf_Error_t error,          
                                                    LinTransIf_NAD_t nad,
                                                    LinTransIf_eSIDType_t sidtype, 
                                                    LinTransIf_SID_t sid,
                                                    LinTransIf_pGenericCbCtxData_t genericTransCbCtxData );
  

/***************************************************************************//**
 * Check target NAD callback from LIN transport layer.
 *
 * @param thisTransIf           - Transport layer interface ptr struct,
 * @param nad                   - addressed NAD,               
 * @param sidtype               - sid type,            
 * @param sid                   - service identifier
 * @param sidDesc               - sid description table entry,
 * @param genericTransCbCtxData - callback context data
 * 
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static LinTransIf_eCheckNADResult_t loc_TransCheckNADCallback( LinTransIf_cpThis_t thisTransIf,
                                                                               LinTransIf_NAD_t nad,                 
                                                                               LinTransIf_eSIDType_t sidtype,             
                                                                               LinTransIf_SID_t  sid,
                                                                               LinTransIf_cpSIDDescription_t sidDesc,
                                                                               LinTransIf_pGenericCbCtxData_t genericTransCbCtxData );

/***************************************************************************//**
 * Goto Sleep callback from LIN transport layer.
 *
 * @param thisTransIf           - Transport layer interface ptr struct,
 * @param genericTransCbCtxData - callback context data
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static void loc_TransGoToSleepCallback( LinTransIf_cpThis_t thisTransIf,
                                                        LinTransIf_pGenericCbCtxData_t genericTransCbCtxData );

/***************************************************************************//**
 * Local goto sleep callback.
 *
 * @param diagEnvData - Diagnostic layer environment data ptr.
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallGoToSleepCallback( loc_pDiagEnvironmentData_t diagEnvData );

/***************************************************************************//**
 * Local call error callback.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallErrorCallback( loc_pDiagEnvironmentData_t diagEnvData, 
                                                          LinTransIf_SID_t SID, 
                                                          LinDiagIf_Error_t error );

/***************************************************************************//**
 * Local call timeout callback.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
#if LINDIAG_MAX_SUPPORTED_TIMER > 0
DIAG_LAYER_CODE static inline void loc_CallTimeoutCallback( loc_pDiagEnvironmentData_t diagEnvData, 
                                                            Lin_uint8_t timerNum, 
                                                            LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );
#endif

#if (LINDIAG_SUP_SNPD == 1) && (LINDIAG_SUP_DATASTG == 0)
/***************************************************************************//**
 * Local call store nad callback.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallStoreNADCallback( loc_pDiagEnvironmentData_t diagEnvData, 
                                                             LinDiagIf_NAD_t nad, 
                                                             LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData );
#endif

#if LINDIAG_SUP_READ_BY_ID == 1
/***************************************************************************//**
 * Local handler for RBI lookup.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static LinDiagIf_pRbiLookupEntry_t loc_LookupRbi( loc_pDiagEnvironmentData_t diagEnvData, 
                                                                  LinDiagIf_RbIdentifier_t rbIdentifier );

/***************************************************************************//**
 * Local handler to check given RBI table.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CheckRbiTbl(LinDiagIf_pRbiLookupEntry_t rbiTbl);
#endif

#if LINDIAG_SUP_DATASTG == 1
/***************************************************************************//**
 * Retrieves data from data storage.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDataStgIf_eErrorCodes_t loc_DataStgGetValue(loc_pDiagEnvironmentData_t diagEnvData, LinDataStgIf_DataValueID_t dataValueId,
                                                                             LinDataStgIf_pData_t       buffer,      LinDataStgIf_Length_t      bufferLen);

/***************************************************************************//**
 * Stores data to data storage.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDataStgIf_eErrorCodes_t loc_DataStgSetValue(loc_pDiagEnvironmentData_t diagEnvData, LinDataStgIf_DataValueID_t dataValueId,
                                                                             LinDataStgIf_pData_t       buffer,      LinDataStgIf_Length_t      bufferLen);
#endif

/***************************************************************************//**
 * Local append data to msg buffer.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_AppendDataToMsgBuffer(LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                                   LinTransIf_pData_t data, LinTransIf_BufLength_t dataLen);
/***************************************************************************//**
 * Local prepare a negative response handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_PrepareNegativeResponse(loc_pDiagServiceControlData_t            diagSerCtrlData,
                                                                     LinTransIf_cpResponseSubInterfaceThis_t  thisRespIf);

/***************************************************************************//**
 * Local prepare a positive response handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_PreparePositiveResponse(loc_pDiagServiceControlData_t           diagSerCtrlData, LinDiagIf_pData_t  pData,
                                                                     LinDiagIf_Length_t                      complDataLen,    LinDiagIf_Length_t appendDataLen,
                                                                     LinTransIf_cpResponseSubInterfaceThis_t thisRespIf);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_InitSerCtrlData(loc_pDiagServiceControlData_t diagSerCtrlData, LinTransIf_pGenericCbCtxData_t genericTransCbCtxData,
                                                             LinTransIf_SID_t              requestedSID,    LinDiagIf_Length_t             requestedLen,
                                                             loc_eReqLenCompType_t         reqLenCompType,  Lin_Bool_t                     supportSubFunction);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_ValidateAndExtractServiceData(loc_pDiagServiceControlData_t diagSerCtrlData, LinDiagIf_pData_t pData,
                                                                           LinDiagIf_Length_t            dataLen);

#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 * Local handler to check given snpd method id for validity.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDiagIf_MethodIdCheckResult_t loc_CallCheckSNPDMethodId(loc_pDiagEnvironmentData_t diagEnvData, LinDiagIf_MethodId_t methodId);
#endif

#if LINDIAG_SUP_ASSIGN_NAD == 1
/***************************************************************************//**
 * Local assign NAD service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AssignNAD(LinTransIf_cpThis_t                     thisTransIf,
                                                                      LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                      LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                      LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);

#endif

#if LINDIAG_SUP_TARGETED_RESET == 1
/***************************************************************************//**
 * Local targeted reset service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_TargetedReset(LinTransIf_cpThis_t                     thisTransIf,
                                                                          LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                          LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                          LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif


#if LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER == 1
/***************************************************************************//**
 * Local assign frame id service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AssignFrameIdentifier(LinTransIf_cpThis_t                     thisTransIf,
                                                                                  LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                                  LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                                  LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif

#if LINDIAG_SUP_READ_BY_ID == 1
/***************************************************************************//**
 * local read by identifier handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_ReadByIdentifier(LinTransIf_cpThis_t                     thisTransIf,
                                                                             LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                             LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                             LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);

/***************************************************************************//**
 * Local product indentification rbi callback handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static Lin_Bool_t loc_RbiCbProdIdent(LinDiagIf_pGenericEnvData_t   genericDiagEnvData,   LinDiagIf_cpInterfaceFunctions_t        ifFunctions,
                                                     LinDiagIf_RbIdentifier_t      rbIdentifier,         LinDiagIf_AppendDataToMsgBuffer_t       appendDataToMsgBufFun,
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);

/***************************************************************************//**
 * Local product serial number rbi callback handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static Lin_Bool_t loc_RbiCbSerialNum(LinDiagIf_pGenericEnvData_t   genericDiagEnvData,   LinDiagIf_cpInterfaceFunctions_t        ifFunctions,
                                                     LinDiagIf_RbIdentifier_t      rbIdentifier,         LinDiagIf_AppendDataToMsgBuffer_t       appendDataToMsgBufFun,
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData);
#endif

#if LINDIAG_SUP_CONDITIONAL_CHANGE_NAD == 1
/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_ConditionalChangeNAD(LinTransIf_cpThis_t                     thisTransIf,
                                                                                 LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                                 LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                                 LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif

#if LINDIAG_SUP_DATA_DUMP == 1
/***************************************************************************//**
 * Local data dump service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_DataDump(LinTransIf_cpThis_t                     thisTransIf,
                                                                     LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                     LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                     LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif

#if LINDIAG_SUP_SAVE_CONFIGURATION == 1
/***************************************************************************//**
 * Local save configuration service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_SaveConfiguration(LinTransIf_cpThis_t                     thisTransIf,
                                                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif

#if LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE == 1
/***************************************************************************//**
 * Local assign frame id range service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AssignFrameIdentifierRange(LinTransIf_cpThis_t                     thisTransIf,
                                                                                       LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                                       LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                                       LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif

#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CallStartMeasurement( LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallEndMeasurement ( LinSNPDIf_pGenericEnvData_t genericSNPDEnvData);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
//DIAG_LAYER_CODE static inline void loc_CallNextMeasurement (LinSNPDIf_pGenericEnvData_t genericSNPDEnvData, Lin_Bool_t addressed);

/***************************************************************************//**
 * TODO: A short description.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinSNPDIf_eState_t loc_CallSNPDGetState (LinSNPDIf_pGenericEnvData_t   genericLinSNPDIfEnvData);


/***************************************************************************//**
 * Local LIN autoaddressing service handler.
 *
 * @param param TODO: Parameter description
 *
 * @return      TODO: return description
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AutoAddrRequest(LinTransIf_cpThis_t                     thisTransIf,
                                                                            LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                            LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                            LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData);
#endif


/* ****************************************************************************/
/* **************************** GLOBALE VARIABLES *****************************/
/* ****************************************************************************/
#if LINDIAGIMP_EXT_IFFUN_STRCT_ACCESS == 1
DIAG_LAYER_DATA const LinDiagIf_sInterfaceFunctions_t LinDiagImp_InterfaceFunctions =
#else
DIAG_LAYER_DATA static const LinDiagIf_sInterfaceFunctions_t LinDiagImp_InterfaceFunctions =
#endif
{
  .InterfaceVersion  = LINDIAG_INTERFACE_MODULE_API_VERSION,
  .Initialization    = &LinDiagImp_Initialization,
  .Task              = &LinDiagImp_Task,
  .GetSubInterface   = &LinDiagImp_GetSubInterface,
  .GetNAD            = &LinDiagImp_GetNad,
  .SetNAD            = &LinDiagImp_SetNad,
  .AddRbiTable       = &LinDiagImp_AddRbiTable,
  .RmvRbiTable       = &LinDiagImp_RmvRbiTable,
};

/* ****************************************************************************/
/* ************************ MODULE GLOBALE VARIABLES **************************/
/* ****************************************************************************/


/***************************************************************************//**
 * Array with all PIDs (i.e. frame ID's with CRC-Bits)
 * The index of the array is the corresponding PID of the frame ID
 ******************************************************************************/
static const Lin_uint8_t loc_DIAG_PIDS[LIN_MAX_FRAMEID_CNT] =
{
    0x80u, 0xC1u, 0x42u, 0x03u, 0xC4u, 0x85u, 0x06u, 0x47u,
    0x08u, 0x49u, 0xCAu, 0x8Bu, 0x4Cu, 0x0Du, 0x8Eu, 0xCFu,
    0x50u, 0x11u, 0x92u, 0xD3u, 0x14u, 0x55u, 0xD6u, 0x97u,
    0xD8u, 0x99u, 0x1Au, 0x5Bu, 0x9Cu, 0xDDu, 0x5Eu, 0x1Fu,
    0x20u, 0x61u, 0xE2u, 0xA3u, 0x64u, 0x25u, 0xA6u, 0xE7u,
    0xA8u, 0xE9u, 0x6Au, 0x2Bu, 0xECu, 0xADu, 0x2Eu, 0x6Fu,
    0xF0u, 0xB1u, 0x32u, 0x73u, 0xB4u, 0xF5u, 0x76u, 0x37u,
    0x78u, 0x39u, 0xBAu, 0xFBu, 0x3Cu, 0x7Du, 0xFEu, 0xBFu
};



DIAG_LAYER_DATA static const LinTransIf_sSIDDescription_t loc_DiagSidLst[] =
{
   //LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) KBDLinDiagIf_SID_0x2E,             &loc_KBDRequest,                  LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),  //由于没有回复的例子，放弃这里
   //LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) KBDLinDiagIf_SID_0x22,             &loc_KBDRequest,                  LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#if LINDIAG_SUP_ASSIGN_NAD == 1
   LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_AssignNAD,             &loc_AssignNAD,                  LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_AssignFrameID,        &loc_AssignFrameIdentifier,      LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_READ_BY_ID == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_ReadByIdentifier,     &loc_ReadByIdentifier,           LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_CONDITIONAL_CHANGE_NAD == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_ConditionalChangeNAD, &loc_ConditionalChangeNAD,       LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_SNPD == 1 /* Attn: for the 0xB5 SID the SNPD entry has to be placed in front of the TargetedReset entry to achieve the desired priorization and runtime configurability */ 
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_Old_SNPD,             &loc_AutoAddrRequest,            LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_TARGETED_RESET == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_TargetedReset,        &loc_TargetedReset,              LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_DATA_DUMP == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_DataDump,    &loc_DataDump,          LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif

#if LINDIAG_SUP_SAVE_CONFIGURATION == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_SaveConfiguration,    &loc_SaveConfiguration,          LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_AssignFrameIDRange,   &loc_AssignFrameIdentifierRange, LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
#if LINDIAG_SUP_SNPD == 1
    LINTRANS_INTERFACE_ADD_SERVICE( (LinDiagIf_SID_t) LinDiagIf_SID_SNPD,                 &loc_AutoAddrRequest,            LinTransIf_SIDDescLstCbCtxType_PER_LST,   LIN_NULL),
#endif
    {.CallbackFnc =  LIN_NULL}
};

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_DATA static const LinTransIf_sCallbackFunctions_t loc_LinTransCallbacks =
{
  .CallbackVersion      = LINTRANS_INTERFACE_MODULE_API_VERSION,

  .Error                = &loc_TransErrorCallback,
  .SIDProcessed         = LIN_NULL,
  .CheckNAD             = &loc_TransCheckNADCallback,
  .GoToSleep            = &loc_TransGoToSleepCallback
};

#if LINDIAG_SUP_READ_BY_ID == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_DATA static const LinDiagIf_sRbiLookupEntry_t loc_RbiTbl[3] =
{
  {.FirstRbIdentifier = LINDIAGIF_RB_IDENTIFIER_PROD_IDENT, .LastRbIdentifier = LINDIAGIF_RB_IDENTIFIER_PROD_IDENT, .Callback = &loc_RbiCbProdIdent, .LengthType = LinDiagIf_RbiLenType_Value, .CbCtxData = LIN_NULL, .Length = (LinDiagIf_RbiLenght_t) LINDIAGIF_RB_IDENTIFIER_PROD_IDENT_LEN},
  {.FirstRbIdentifier = LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM, .LastRbIdentifier = LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM, .Callback = &loc_RbiCbSerialNum, .LengthType = LinDiagIf_RbiLenType_Value, .CbCtxData = LIN_NULL, .Length = (LinDiagIf_RbiLenght_t) LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM_LEN},
  {.FirstRbIdentifier = 0u, .LastRbIdentifier = 0u, .Callback = LIN_NULL, .CbCtxData = LIN_NULL, .Length = 0u},
};
#endif

/* ****************************************************************************/
/* ************************** FUNCTION DEFINITIONS ****************************/
/* ****************************************************************************/


#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDiagIf_MethodIdCheckResult_t loc_CallCheckSNPDMethodId(loc_pDiagEnvironmentData_t diagEnvData, LinDiagIf_MethodId_t methodId)
{
  LinDiagIf_MethodIdCheckResult_t result = LinDiagIf_MethodId_Accept;

  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  
  if( callbacks != LIN_NULL )
  {
    const LinDiagIf_CheckSNPDMethodIdCbFun_t fun = callbacks->CheckSNPDMethod;
    if ( fun != LIN_NULL )
    {
      result = fun( diagEnvData, &LinDiagImp_InterfaceFunctions, methodId, diagEnvData->CallbackCtxData );
    }
    else { /* Error: No valid callback found! */ }
  }
  else { /* Error: No valid callback table found! */ }
  
  return result;
}
#endif


/***************************************************************************//**
 * LIN diagnostic layer initialization function.
 *
 * @param genericDiagEnvData      - 
 * @param diagEnvDataSze          -
 * @param diagCbFuns              -
 * @param genericDiagCbCtxData    -
 * @param invalidReadByIDAnswered -
 * @param genericDiagImpCfgData   -
 *
 * @return TRUE  - Diagnostic layer has been initalized successfully,
 *         FALSE - else.
 ******************************************************************************/
DIAG_LAYER_CODE Lin_Bool_t LinDiagImp_Initialization( LinDiagIf_pGenericEnvData_t genericDiagEnvData,      
                                                      LinDiagIf_EnvDataSze_t diagEnvDataSze,
                                                      LinDiagIf_cpCallbackFunctions_t diagCbFuns,              
                                                      LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData,
                                                      Lin_Bool_t invalidReadByIDAnswered, 
                                                      LinDiagIf_pGenericImpCfgData_t genericDiagImpCfgData )
{
  Lin_Bool_t returnValue = LIN_FALSE;
  
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;
  LinDiagImp_pCfgData_t diagImpCfgData = (LinDiagImp_pCfgData_t) genericDiagImpCfgData;

  if( ( genericDiagEnvData != LIN_NULL ) &&
      ( diagEnvDataSze >= sizeof(loc_sDiagEnvironmentData_t) ) &&
      ( diagCbFuns != LIN_NULL ) &&
      ( genericDiagImpCfgData != LIN_NULL ) &&
      ( diagCbFuns->CallbackVersion == LINDIAG_INTERFACE_MODULE_API_VERSION ) &&
#if LINDIAG_SUP_SNPD == 1
      ( diagImpCfgData->LinSNPDIfThisPointer.IfFunsTbl != LIN_NULL ) &&
      ( diagImpCfgData->LinSNPDIfThisPointer.IfFunsTbl->InterfaceVersion == LINSNPD_INTERFACE_MODULE_API_VERSION ) &&
#endif
#if LINDIAG_SUP_DATASTG == 1
      ( diagImpCfgData->LinDataStgIfThisPointer.IfFunsTbl != LIN_NULL ) &&
      ( diagImpCfgData->LinDataStgIfThisPointer.IfFunsTbl->InterfaceVersion == LINDATASTG_INTERFACE_MODULE_API_VERSION ) &&
#endif
      ( diagImpCfgData->LinBusIfThisPointer.IfFunsTbl != LIN_NULL ) &&
      ( diagImpCfgData->LinLookupIfThisPointer.IfFunsTbl != LIN_NULL ) &&
      ( diagImpCfgData->LinTransIfInitParam.IfFunsTbl != LIN_NULL ) &&
      ( diagImpCfgData->LinBusIfThisPointer.IfFunsTbl->InterfaceVersion == LINBUS_INTERFACE_MODULE_API_VERSION ) &&
      ( diagImpCfgData->LinLookupIfThisPointer.IfFunsTbl->InterfaceVersion == LINLOOKUP_INTERFACE_MODULE_API_VERSION ) &&
      ( diagImpCfgData->LinTransIfInitParam.IfFunsTbl->InterfaceVersion == LINTRANS_INTERFACE_MODULE_API_VERSION) )
  {
#if LINDIAG_SUP_DATASTG == 1
    LinTransIf_NAD_t initialNad;
#endif
    Lin_uint8_t i;

    /* Initialize lookup environment data */
    for( i = 0; i < sizeof(loc_sDiagEnvironmentData_t); i++)
    {
      ( (Lin_uint8_t*)genericDiagEnvData )[i] = 0;
    }

    diagEnvData->TransIfFuns             = diagImpCfgData->LinTransIfInitParam.IfFunsTbl;
    diagEnvData->TransEnvData            = diagImpCfgData->LinTransIfInitParam.envData;

    diagEnvData->BusThisPtr              = diagImpCfgData->LinBusIfThisPointer;
    diagEnvData->LookupThisPtr           = diagImpCfgData->LinLookupIfThisPointer;
#if LINDIAG_SUP_DATASTG == 1
    diagEnvData->DataStgThisPtr          = diagImpCfgData->LinDataStgIfThisPointer;
#endif

    diagEnvData->Callbacks               = diagCbFuns;

    diagEnvData->InvalidReadByIDAnswered = invalidReadByIDAnswered;

#if LINDIAG_SUP_SNPD == 1

    diagEnvData->LinSNPDIfThisPointer    = diagImpCfgData->LinSNPDIfThisPointer;
    diagEnvData->ImmediateNADUpdate      =  diagImpCfgData->ImmediateNADUpdate;
    diagEnvData->UseOldSNPDServiceID     = diagImpCfgData->UseOldSNPDServiceID;

    diagEnvData->SNPDNewNAD              = 0u;
    diagEnvData->SNPDNewNADValid         = LIN_FALSE;
    diagEnvData->SNPDStarted             = LIN_FALSE;
    diagEnvData->SNPDAddressed           = LIN_FALSE;

#endif      

#if LINDIAG_SUP_DATASTG == 1
    /* Get Product Identification */
    if( loc_DataStgGetValue( diagEnvData, 
                             LinDataStgIf_DVID_ProductIdentification,
                             (LinDataStgIf_pData_t)&(diagEnvData->ProductIdentification),
                             sizeof(diagEnvData->ProductIdentification) ) 
                             == LinDataStgIf_ERR_NO_ERROR )
    {
      returnValue = LIN_TRUE;
    }
    else
    {
      #if LINDIAG_INIT_DEFAULT_VALUES == 1
      LinDiagIf_sProductIdentification_t defaultProductIdentification;

      defaultProductIdentification.SupplierID = LINDIAG_DEFAULT_SUPPLIERID;
      defaultProductIdentification.FunctionID = LINDIAG_DEFAULT_FUNCTIONID;
      defaultProductIdentification.VariantID  = LINDIAG_DEFAULT_VARIANTID;

      if( loc_DataStgSetValue( diagEnvData, 
                               LinDataStgIf_DVID_ProductIdentification, 
                               (LinDataStgIf_pData_t)&defaultProductIdentification, 
                               sizeof(defaultProductIdentification) ) 
                               == LinDataStgIf_ERR_NO_ERROR)
      {
        diagEnvData->ProductIdentification.SupplierID = LINDIAG_DEFAULT_SUPPLIERID;
        diagEnvData->ProductIdentification.FunctionID = LINDIAG_DEFAULT_FUNCTIONID;
        diagEnvData->ProductIdentification.VariantID  = LINDIAG_DEFAULT_VARIANTID;
        
        returnValue = LIN_TRUE;
      }
      else
      {
        returnValue = LIN_FALSE;
      }
      #endif // LINDIAG_INIT_DEFAULT_VALUES == 1
    }

    /* Get Serial Number */
    if( (loc_DataStgGetValue( diagEnvData, LinDataStgIf_DVID_SerialNumber,
                             (LinDataStgIf_pData_t) &(diagEnvData->SerialNumber),
                             sizeof(diagEnvData->SerialNumber) )
                             == LinDataStgIf_ERR_NO_ERROR) 
                             && (returnValue == LIN_TRUE) )
    {
      returnValue = LIN_TRUE;
    }
    else
    {
      #if LINDIAG_INIT_DEFAULT_VALUES == 1
      LinDiagIf_SerialNumber_t defaultSerialNumber = LINDIAG_DEFAULT_SERIAL_NUMBER;

      if(loc_DataStgSetValue(diagEnvData, LinDataStgIf_DVID_SerialNumber, (LinDataStgIf_pData_t) &defaultSerialNumber, sizeof(defaultSerialNumber)) == LinDataStgIf_ERR_NO_ERROR)
      {
        diagEnvData->SerialNumber = LINDIAG_DEFAULT_SERIAL_NUMBER;
        
        returnValue = LIN_TRUE;
      }
      else
      {
        returnValue = LIN_FALSE;
      }
      #endif // LINDIAG_INIT_DEFAULT_VALUES == 1
    }

    /* Get initial NAD. */
    if((loc_DataStgGetValue(diagEnvData, LinDataStgIf_DVID_NAD,
                            (LinDataStgIf_pData_t) &(initialNad),
                            sizeof(diagEnvData->NAD)) == LinDataStgIf_ERR_NO_ERROR) &&
                            (returnValue == LIN_TRUE))
    {
      /* Set NADs */
      if(   ( initialNad != LINTRANSIF_FUNCTIONAL_NAD ) 
         && ( initialNad != LINTRANSIF_GOTOSLEEP_NAD  )
         && ( initialNad != LINTRANSIF_BROADCAST_NAD  ) )
      {
        //diagEnvData->InitialNAD = initialNad;
        //diagEnvData->NAD        = initialNad;
        diagEnvData->InitialNAD = savedConfig.singleAddr;    // LJM added on 20230426
        diagEnvData->NAD        = savedConfig.singleAddr;  
        
        returnValue = LIN_TRUE;
      }
      else
      {
        /* ERROR: Invalid NAD */
        loc_CallErrorCallback( diagEnvData, 0xFF, LinDiagIf_ERR_INVALID_NAD );
      }
    }
    else
    {
      #if LINDIAG_INIT_DEFAULT_VALUES == 1
      LinTransIf_NAD_t defaultNad = LINDIAG_DEFAULT_NAD;

      if(    ( defaultNad != LINTRANSIF_FUNCTIONAL_NAD ) 
          && ( defaultNad != LINTRANSIF_GOTOSLEEP_NAD  )
          && ( defaultNad != LINTRANSIF_BROADCAST_NAD  ) ) 
      {
        if( oc_DataStgSetValue( diagEnvData, LinDataStgIf_DVID_NAD, (LinDataStgIf_pData_t) &defaultNad, sizeof(defaultNad) ) == LinDataStgIf_ERR_NO_ERROR)
        {
//          diagEnvData->InitialNAD = defaultNad;
//          diagEnvData->NAD        = defaultNad;
            diagEnvData->InitialNAD = savedConfig.singleAddr;    // LJM added on 20230426
            diagEnvData->NAD        = savedConfig.singleAddr;  
            returnValue = LIN_TRUE;
        }
        else
        {
          returnValue = LIN_FALSE;
        }
      }
      else
      {
        /* ERROR: Invalid NAD */
        loc_CallErrorCallback(diagEnvData, 0xFF, LinDiagIf_ERR_INVALID_NAD);
      }
      #endif // LINDIAG_INIT_DEFAULT_VALUES == 1
    }
#else
    /* Set Serial Number */
    diagEnvData->SerialNumber          = diagImpCfgData->InitialSerialNumber;

    /* Set Product Identification */
    diagEnvData->ProductIdentification = diagImpCfgData->InitialProdIdent;

    /* Set NADs */
    if(    ( diagImpCfgData->InitialNad != LINTRANSIF_FUNCTIONAL_NAD )
        && ( diagImpCfgData->InitialNad != LINTRANSIF_GOTOSLEEP_NAD  )
        && ( diagImpCfgData->InitialNad != LINTRANSIF_BROADCAST_NAD  ) )
    {
//      diagEnvData->InitialNAD = diagImpCfgData->InitialNad;
//      diagEnvData->NAD        = diagImpCfgData->InitialNad;
        diagEnvData->InitialNAD = savedConfig.singleAddr;    // LJM added on 20230426
        diagEnvData->NAD        = savedConfig.singleAddr;      
      returnValue = LIN_TRUE;
    }
    else
    {
      /* ERROR: Invalid NAD */
      loc_CallErrorCallback( diagEnvData, 0xFFu, LinDiagIf_ERR_INVALID_NAD );
    }
#endif

    if( returnValue == LIN_TRUE )
    {
      returnValue = diagEnvData->TransIfFuns->Initialization( diagImpCfgData->LinTransIfInitParam.envData,          
															  diagImpCfgData->LinTransIfInitParam.EnvDataLen,
                                                              diagImpCfgData->LinTransIfInitParam.protoFrmDescInfo, 
                                                              &loc_LinTransCallbacks, diagEnvData,
                                                              diagImpCfgData->LinTransIfInitParam.nasTimeout,
                                                              diagImpCfgData->LinTransIfInitParam.NcrTimeout,       
															  diagImpCfgData->LinTransIfInitParam.InitCfgFlags,
                                                              diagImpCfgData->LinTransIfInitParam.ImpCfgData );
    }
    else{}
    
    if( returnValue == LIN_TRUE )
    {
      returnValue = diagEnvData->TransIfFuns->AddSIDDescriptionList( diagEnvData->TransEnvData,  
                                                                     (LinTransIf_pSIDDescription_t) loc_DiagSidLst,
                                                                     (LinTransIf_CheckNADCbFun_t) loc_TransCheckNADCallback, 
                                                                     (LinTransIf_pGenericCbCtxData_t) genericDiagEnvData );

      #if LINDIAG_SUP_READ_BY_ID == 1
      if( returnValue == LIN_TRUE )
      {
        diagEnvData->UserAppendedDataLen = 0u;
        
        returnValue = LinDiagImp_AddRbiTable( genericDiagEnvData, (LinDiagIf_pRbiLookupEntry_t) loc_RbiTbl);
      }
      #endif
    }
    else{}
  }
  else{}

  return(returnValue);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE void LinDiagImp_Task(LinDiagIf_pGenericEnvData_t genericDiagEnvData)
{
  if(genericDiagEnvData != LIN_NULL)
  {
    #if LINDIAG_MAX_SUPPORTED_TIMER > 0
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;
    Lin_uint8_t i;
	
    for(i = 0; i < LINDIAG_MAX_SUPPORTED_TIMER; i++)
    {
      if(diagEnvData->Timer[i].Start != 0 && diagEnvData->Timer[i].Timeout != 0)
      {
        istate_t istate = __get_interrupt_state();
        __disable_interrupt();
        Lin_Bool_t callCb = LIN_FALSE;
        LinBusIf_Tick_t tempTimerVal  = diagEnvData->BusThisPtr.IfFunsTbl->GetMilliseconds(diagEnvData->BusThisPtr.envData);
        Lin_uint32_t    savedTimerVal = diagEnvData->Timer[i].Start;
        Lin_uint32_t    tempCompVal   = tempTimerVal - savedTimerVal;
  
        if((savedTimerVal & LOC_TIMER_OVERFLOW_BIT) != (tempTimerVal & LOC_TIMER_OVERFLOW_BIT))
        {
          tempCompVal = tempTimerVal + (LOC_TIMER_MAX_VAL - savedTimerVal);
        }
        else{}
  
        if(tempCompVal >= diagEnvData->Timer[i].Timeout)
        {
          diagEnvData->Timer[i].Start = 0;
          diagEnvData->Timer[i].Timeout = 0;
          callCb = LIN_TRUE;
        }
        else{}
        
        __set_interrupt_state(istate);
        
        if(callCb == LIN_TRUE)
        {
          loc_CallTimeoutCallback(diagEnvData, i, diagEnvData->Timer[i].CbCtxData);
        }
        else{}
      }
      else{}
    }
    #endif
  }
  else{}
}

/***************************************************************************//**
 * TODO: Document exactly the meaning of return value LIN_TRUE and LIN_FALSE.
 ******************************************************************************/
DIAG_LAYER_CODE Lin_Bool_t LinDiagImp_GetSubInterface(LinDiagIf_pGenericEnvData_t genericDiagEnvData, Lin_eInterfaceIds_t interfaceId, Lin_pThis_t ifThisPtr)
{
  Lin_Bool_t retVal = LIN_FALSE;

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE LinTransIf_NAD_t LinDiagImp_GetNad(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_eNADType_t nadtype)
{
  LinTransIf_NAD_t nad;

  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

    if(nadtype == LinDiagIf_NADType_INITIAL)
    {
      nad = diagEnvData->InitialNAD;
    }
    else
    {
      nad = diagEnvData->NAD;
    }
  }

  return(nad);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static void LinDiagImp_SetNad(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinTransIf_NAD_t nad)
{
  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

    /* Set NADs */
    if(nad != LINTRANSIF_FUNCTIONAL_NAD &&
       nad != LINTRANSIF_GOTOSLEEP_NAD &&
       nad != LINTRANSIF_BROADCAST_NAD)
    {
      diagEnvData->NAD = nad;
    }
    else
    {
      /* ERROR: Invalid NAD */
      loc_CallErrorCallback(diagEnvData, 0xFF, LinDiagIf_ERR_INVALID_NAD);
    }
  }
  else{}
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE Lin_Bool_t LinDiagImp_AddRbiTable(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl)
{
  Lin_Bool_t retVal = LIN_FALSE;

  #if LINDIAG_SUP_READ_BY_ID == 1
  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;
    Lin_uint8_t i;

    if(loc_CheckRbiTbl(rbiTbl) == LIN_TRUE)
    {
      for(i = 0; i < LINDIAG_MAX_RBI_TBL_CNT; i++)
      {
        if(diagEnvData->RbiTbl[i] == LIN_NULL)
        {
          diagEnvData->RbiTbl[i] = rbiTbl;
          retVal = LIN_TRUE;
          break;
        }
        else{}
      }
    }
  }
  else{}
  #endif

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE Lin_Bool_t LinDiagImp_RmvRbiTable(LinDiagIf_pGenericEnvData_t genericDiagEnvData, LinDiagIf_pRbiLookupEntry_t rbiTbl)
{
  Lin_Bool_t retVal = LIN_FALSE;

  #if LINDIAG_SUP_READ_BY_ID == 1
  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;
    Lin_uint8_t i;

    if(loc_CheckRbiTbl(rbiTbl) == LIN_TRUE)
    {
      for(i = 0; i < LINDIAG_MAX_RBI_TBL_CNT; i++)
      {
        if(diagEnvData->RbiTbl[i] == LIN_NULL)
        {
          diagEnvData->RbiTbl[i] = rbiTbl;
          retVal = LIN_TRUE;
          break;
        }
        else{}
      }
    }
  }
  else{}
  #endif

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE Lin_Bool_t LinDiagImp_SetUpTimer(LinDiagIf_pGenericEnvData_t genericDiagEnvData, Lin_uint8_t                   timerNum,
                                                 LinDiagIf_Timeout_t         timeout,            LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  Lin_Bool_t retVal = LIN_FALSE;

  #if LINDIAG_MAX_SUPPORTED_TIMER > 0
  if((genericDiagEnvData != LIN_NULL) && (timeout <= LOC_TIMER_MAX_VAL))
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

    diagEnvData->Timer[timerNum].Start = diagEnvData->BusThisPtr.IfFunsTbl->GetMilliseconds(diagEnvData->BusThisPtr.envData);
    diagEnvData->Timer[timerNum].Timeout = timeout;
    diagEnvData->Timer[timerNum].CbCtxData = genericDiagCbCtxData;

    retVal = LIN_TRUE;
  }
  #endif

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static void loc_TransErrorCallback(LinTransIf_cpThis_t            thisTransIf,
                                                   LinTransIf_Error_t             error,          
                                                   LinTransIf_NAD_t               nad,
                                                   LinTransIf_eSIDType_t          sidtype, LinTransIf_SID_t      sid,
                                                   LinTransIf_pGenericCbCtxData_t genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;

  loc_CallErrorCallback(diagEnvData, sid, error);
}




/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static LinTransIf_eCheckNADResult_t loc_TransCheckNADCallback (LinTransIf_cpThis_t           thisTransIf,
                                                                               LinTransIf_NAD_t              nad,                 
                                                                               LinTransIf_eSIDType_t         sidtype,             LinTransIf_SID_t                  sid,
                                                                               LinTransIf_cpSIDDescription_t sidDesc,             LinTransIf_pGenericCbCtxData_t    genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t   diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eCheckNADResult_t returnValue = LinTransIf_ChkNADRes_Unknown;

  if((sid == 0xB0 && diagEnvData->InitialNAD == nad) || (sid != 0xB0 && diagEnvData->NAD == nad) || (nad == 0x7F))
  {
    returnValue = LinTransIf_ChkNADRes_Accept;
  }
  else{}

  return(returnValue);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static void loc_TransGoToSleepCallback(LinTransIf_cpThis_t                     thisTransIf,
                                                       LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t   diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;

  loc_CallGoToSleepCallback(diagEnvData);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallGoToSleepCallback(loc_pDiagEnvironmentData_t diagEnvData)
{
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_GoToSleepCbFun_t fun = callbacks->GoToSleep;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData, &LinDiagImp_InterfaceFunctions, diagEnvData->CallbackCtxData);
    }
    else{}
  }
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallErrorCallback(loc_pDiagEnvironmentData_t diagEnvData, LinTransIf_SID_t SID, LinDiagIf_Error_t error)
{
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_ErrorCbFun_t fun = callbacks->Error;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData, &LinDiagImp_InterfaceFunctions, error, SID, diagEnvData->CallbackCtxData);
    }
    else{}
  }
}

#if LINDIAG_MAX_SUPPORTED_TIMER > 0
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallTimeoutCallback(loc_pDiagEnvironmentData_t diagEnvData, Lin_uint8_t timerNum, LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;

  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_TimeoutCbFun_t fun = callbacks->Timeout;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData, &LinDiagImp_InterfaceFunctions, timerNum, genericDiagCbCtxData);
    }
    else{}
  }
}
#endif

#if (LINDIAG_SUP_SNPD == 1) && (LINDIAG_SUP_DATASTG == 0)
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallStoreNADCallback(loc_pDiagEnvironmentData_t diagEnvData, LinDiagIf_NAD_t nad, LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;

  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_StoreNADCbFun_t fun = callbacks->StoreNAD;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData, &LinDiagImp_InterfaceFunctions, nad, genericDiagCbCtxData);
    }
    else{}
  }
}
#endif

#if LINDIAG_SUP_READ_BY_ID == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static LinDiagIf_pRbiLookupEntry_t loc_LookupRbi(loc_pDiagEnvironmentData_t diagEnvData, LinDiagIf_RbIdentifier_t rbIdentifier)
{
  LinDiagIf_pRbiLookupEntry_t retVal = LIN_NULL;
  Lin_uint8_t i = 0;

  /* Iterate through all tables */
  for(i = 0; i < LINDIAG_MAX_RBI_TBL_CNT; i++)
  {
    Lin_uint8_t j = 0;
  
    if(diagEnvData->RbiTbl[i] == LIN_NULL)
    {
      break;
    }

    while(diagEnvData->RbiTbl[i][j].Callback != LIN_NULL)
    {
      /* Check if there is a callback function for this RBIdentifier. */
      if(rbIdentifier >=  diagEnvData->RbiTbl[i][j].FirstRbIdentifier && rbIdentifier <=  diagEnvData->RbiTbl[i][j].LastRbIdentifier)
      {
        /* Return pointer to RBIdentifier entry. */
        retVal = &(diagEnvData->RbiTbl[i][j]);
        break;
      }
      
      j++;
    }
  }

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CheckRbiTbl(LinDiagIf_pRbiLookupEntry_t rbiTbl)
{
  Lin_Bool_t retVal = LIN_FALSE;
  Lin_uint8_t i = 0;

  while(rbiTbl[i].Callback != LIN_NULL)
  {
    //Check range
    if(rbiTbl[i].FirstRbIdentifier <= rbiTbl[i].LastRbIdentifier)
    {
      // Check length type parameter
      if(rbiTbl[i].LengthType == LinDiagIf_RbiLenType_Value)
      {
        retVal = LIN_TRUE;
      }
      else if(rbiTbl[i].LengthType == LinDiagIf_RbiLenType_Callback)
      {
        if(rbiTbl[i].Length != LIN_NULL)
        {
          retVal = LIN_TRUE;
        }
      }
      else{}
    }
    else{}
    
    i++;
  }

  return(retVal);
}
#endif

#if LINDIAG_SUP_DATASTG == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDataStgIf_eErrorCodes_t loc_DataStgGetValue(loc_pDiagEnvironmentData_t diagEnvData, LinDataStgIf_DataValueID_t dataValueId,
                                                                             LinDataStgIf_pData_t       buffer,      LinDataStgIf_Length_t      bufferLen)
{
  LinDataStgIf_eErrorCodes_t retVal = LinDataStgIf_ERR_DATA_VALUE_ACCESS_FAILED;

  retVal = diagEnvData->DataStgThisPtr.IfFunsTbl->GetDataValue(diagEnvData->DataStgThisPtr.envData, dataValueId,
                                                               buffer, bufferLen, LIN_NULL);

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinDataStgIf_eErrorCodes_t loc_DataStgSetValue(loc_pDiagEnvironmentData_t diagEnvData, LinDataStgIf_DataValueID_t dataValueId,
                                                                             LinDataStgIf_pData_t       buffer,      LinDataStgIf_Length_t      bufferLen)
{
  LinDataStgIf_eErrorCodes_t retVal = LinDataStgIf_ERR_DATA_VALUE_ACCESS_FAILED;

  retVal = diagEnvData->DataStgThisPtr.IfFunsTbl->SetDataValue(diagEnvData->DataStgThisPtr.envData, dataValueId,
                                                               buffer, bufferLen);

  return(retVal);
}
#endif

#ifdef LOC_USE_APPENDDATATOMSGBUFFFER_FUN
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_AppendDataToMsgBuffer(LinDiagIf_pGenericEnvData_t genericDiagEnvData, 
                                                                   LinTransIf_pData_t data, LinTransIf_BufLength_t dataLen)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;
  Lin_Bool_t returnValue = LIN_FALSE;
  LinTransIf_sResponseSubInterfaceThis_t TransRespThisPtr;

  diagEnvData->UserAppendedDataLen += dataLen;

  returnValue = diagEnvData->TransIfFuns->GetSubInterface(diagEnvData->TransEnvData, Lin_IfId_TRANS_RESP_BUILD, (Lin_pThis_t)&TransRespThisPtr);
  
  if (returnValue != LIN_TRUE)
  {
    return LIN_FALSE;
  }
  else
  {
    return(TransRespThisPtr.IfFunsTbl->AppendDataToMsgBuffer((LinTransIf_pGenericEnvData_t) diagEnvData->TransEnvData, data, dataLen));
  }
}
#endif

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_PrepareNegativeResponse(loc_pDiagServiceControlData_t            diagSerCtrlData,
                                                                     LinTransIf_cpResponseSubInterfaceThis_t  thisRespIf)
{
  Lin_Bool_t returnValue = LIN_FALSE;
  loc_pDiagEnvironmentData_t diagEnvData = diagSerCtrlData->DiagEnvData;

  returnValue = thisRespIf->IfFunsTbl->InitResponse(diagEnvData->TransEnvData, diagEnvData->NAD, LINDIAGIF_NEG_RESP_RSID, LINDIAGIF_NEG_RESP_LEN);

  if(returnValue == LIN_TRUE)
  {
    LinDiagIf_Data_t buffer[2];

    buffer[0] = diagSerCtrlData->RequestedSID;
    buffer[1] = diagSerCtrlData->NegativeResponseCode;

    returnValue = thisRespIf->IfFunsTbl->AppendDataToMsgBuffer((LinTransIf_pGenericEnvData_t) diagEnvData->TransEnvData, buffer, sizeof(buffer));

    if(returnValue != LIN_TRUE)
    {
      /* Appending data to buffer in Transport Layer failed.
       * Report error. */
      loc_CallErrorCallback(diagSerCtrlData->DiagEnvData, diagSerCtrlData->RequestedSID, LinDiagIf_ERR_TRANS_LAYER);
    }
    else{}
  }
  else
  {
    /* Initialization of response failed in Transport Layer. This function
     * only returns LIN_FALSE if the Transport Layer Environment Data pointer
     * is NULL, so report an error, but don't send a negative response because
     * communication via LIN Transport Layer is most likely impossible. */
    diagSerCtrlData->SIDReaction = LinTransIf_SIDRA_IGNORED;

    loc_CallErrorCallback(diagEnvData, diagSerCtrlData->RequestedSID, LinDiagIf_ERR_TRANS_LAYER);
  }

  return(returnValue);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_PreparePositiveResponse(loc_pDiagServiceControlData_t           diagSerCtrlData, LinDiagIf_pData_t  pData,
                                                                     LinDiagIf_Length_t                      complDataLen,    LinDiagIf_Length_t appendDataLen,
                                                                     LinTransIf_cpResponseSubInterfaceThis_t thisRespIf)
{
  Lin_Bool_t returnValue = LIN_FALSE;
  loc_pDiagEnvironmentData_t diagEnvData = diagSerCtrlData->DiagEnvData;

  /* Init response */
  returnValue = thisRespIf->IfFunsTbl->InitResponse(diagEnvData->TransEnvData, diagEnvData->NAD,
                                                       diagSerCtrlData->RequestedSID + LINDIAGIF_RSID_OFFSET,
                                                       complDataLen);

  if(returnValue == LIN_TRUE)
  {
    if(appendDataLen > 0)
    {
      returnValue = thisRespIf->IfFunsTbl->AppendDataToMsgBuffer((LinTransIf_pGenericEnvData_t) diagEnvData->TransEnvData,
                                                                    pData, appendDataLen);

      if(returnValue != LIN_TRUE)
      {
        /* Appending data to buffer in Transport Layer failed.
         * Report error and try to send a negative response. */
        diagSerCtrlData->SendNegativeResponse = LIN_TRUE;
        diagSerCtrlData->NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;

        loc_CallErrorCallback(diagSerCtrlData->DiagEnvData, diagSerCtrlData->RequestedSID, LinDiagIf_ERR_TRANS_LAYER);
      }
      else{}
    }
    else{}
  }
  else
  {
    /* Initialization of response failed in Transport Layer. This function
     * only returns LIN_FALSE if the Transport Layer Environment Data pointer
     * is NULL, so report an error, but don't send a negative response because
     * communication via LIN Transport Layer is most likely impossible. */
    diagSerCtrlData->SIDReaction = LinTransIf_SIDRA_IGNORED;

    loc_CallErrorCallback(diagEnvData, diagSerCtrlData->RequestedSID, LinDiagIf_ERR_TRANS_LAYER);
  }

  return(returnValue);
}


/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_InitSerCtrlData(loc_pDiagServiceControlData_t diagSerCtrlData, LinTransIf_pGenericCbCtxData_t genericTransCbCtxData,
                                                             LinTransIf_SID_t              requestedSID,    LinDiagIf_Length_t             requestedLen,
                                                             loc_eReqLenCompType_t         reqLenCompType,  Lin_Bool_t                     supportSubFunction)
{
  Lin_Bool_t returnValue = LIN_FALSE;

  if(genericTransCbCtxData != LIN_NULL)
  {
    diagSerCtrlData->DiagEnvData          = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
    diagSerCtrlData->SIDReaction          = LinTransIf_SIDRA_SEND_RESPONSE;
    diagSerCtrlData->RequestedSID         = requestedSID;
    diagSerCtrlData->RequestLength        = requestedLen;
    diagSerCtrlData->ReqLenCompType       = reqLenCompType;
    diagSerCtrlData->SupportSubFunction   = supportSubFunction;
    diagSerCtrlData->SubFunction          = 0;
    diagSerCtrlData->SuppressPosRspMsg    = LIN_FALSE;
    diagSerCtrlData->SendNegativeResponse = LIN_FALSE;
    diagSerCtrlData->NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;

    /* Reset counter for data appended to buffer by user. */
    diagSerCtrlData->DiagEnvData->UserAppendedDataLen = 0;

    returnValue = LIN_TRUE;
  }
  else
  {
    diagSerCtrlData->SIDReaction = LinTransIf_SIDRA_IGNORED;
  }

  return(returnValue);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_ValidateAndExtractServiceData(loc_pDiagServiceControlData_t diagSerCtrlData, LinDiagIf_pData_t pData,
                                                                           LinDiagIf_Length_t            dataLen)
{
  Lin_Bool_t returnValue = LIN_FALSE;

  if(diagSerCtrlData->ReqLenCompType == loc_RLCT_GreaterThenOrEqual)
  {
    if(dataLen >= diagSerCtrlData->RequestLength)
    {
      returnValue = LIN_TRUE;
    }
    else{}
  }
  else if(diagSerCtrlData->ReqLenCompType == loc_RLCT_Equal)
  {
    if(dataLen == diagSerCtrlData->RequestLength)
    {
      returnValue = LIN_TRUE;
    }
    else{}
  }
  else if(diagSerCtrlData->ReqLenCompType == loc_RLCT_LessThenOrEqual)
  {
    if(dataLen <= diagSerCtrlData->RequestLength)
    {
      returnValue = LIN_TRUE;
    }
    else{}
  }
  else{}

  if(returnValue == LIN_TRUE)
  {
    if(diagSerCtrlData->SupportSubFunction == LIN_TRUE)
    {
      /* Suppress positive response message? */
      if(pData[0] & LOC_SUPPRESS_POS_RESP_BIT_MSK)
      {
        diagSerCtrlData->SuppressPosRspMsg = LIN_TRUE;
      }
      else{}

      diagSerCtrlData->SubFunction = pData[0] & LOC_SUB_FUNCTION_MSK;
    }
    else{}
  }
  else
  {
    /* Data length mismatch */
    diagSerCtrlData->SendNegativeResponse = LIN_TRUE;
    diagSerCtrlData->NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;

    loc_CallErrorCallback(diagSerCtrlData->DiagEnvData, diagSerCtrlData->RequestedSID, LinDiagIf_ERR_IN_CORRECT_MESSAGE_LENGHT_INVALID_FORMAT);
  }

  return(returnValue);
}


#if LINDIAG_SUP_TARGETED_RESET == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CallTargetedResetCallback(loc_pDiagEnvironmentData_t diagEnvData, LinTransIf_Error_t errorCode)
{
  Lin_Bool_t result = LIN_FALSE;
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_TargetedResetFun_t fun = callbacks->TargetedReset;
    if (fun != LIN_NULL)
    {
      result = fun(diagEnvData, &LinDiagImp_InterfaceFunctions, errorCode, diagEnvData->CallbackCtxData);
    }
    else{}
  }
  return result;
}


/***************************************************************************//**
 * SAE-J2602 5.7.3: Targeted Reset
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_TargetedReset(LinTransIf_cpThis_t                     thisTransIf,
                                                                          LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                          LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                          LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  // call application callback for reset
  Lin_Bool_t resetSuccess = loc_CallTargetedResetCallback(diagEnvData, serviceData->ErrorCode);
  Lin_uint8_t responseCode = LINDIAGIF_NEG_RESP_RSID;

  // if broadcast: send no response (neither positive nor negative)
  if (serviceData->NAD == LINTRANSIF_BROADCAST_NAD)
  {
    returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
  }
  else
  {
    // if reset was successfull, set positive response code
    if (resetSuccess)
    {
      responseCode = LinDiagIf_SID_TargetedReset + LINDIAGIF_RSID_OFFSET;
    }

    // fill response buffer with supplier-Id, function-Id, variant-Id
    LinDiagIf_Data_t buffer[5];
    buffer[0] = ((uint8_t*)(&diagEnvData->ProductIdentification.SupplierID))[0];
    buffer[1] = ((uint8_t*)(&diagEnvData->ProductIdentification.SupplierID))[1];
    buffer[2] = ((uint8_t*)(&diagEnvData->ProductIdentification.FunctionID))[0];
    buffer[3] = ((uint8_t*)(&diagEnvData->ProductIdentification.FunctionID))[1];
    buffer[4] =  diagEnvData->ProductIdentification.VariantID;

    // send response
    thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->NAD, responseCode, sizeof(buffer));
    thisRespIf->IfFunsTbl->AppendDataToMsgBuffer(thisRespIf->envData, buffer, sizeof(buffer));
    returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
  }

  return(returnValue);
}
#endif

#if LINDIAG_SUP_DATA_DUMP == 1
/***************************************************************************//**
 * Callback for DataDump
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CallDataDumpCallback(loc_pDiagEnvironmentData_t diagEnvData, LinTransIf_Error_t errorCode,  LinDiagIf_pDataDumpData_t dataDumpReqBuffer, LinDiagIf_pDataDumpData_t dataDumpRespBuffer)
{
  Lin_Bool_t result = LIN_FALSE;
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_DataDumpCbFun_t fun = callbacks->DataDump;
    if (fun != LIN_NULL)
    {
      result = fun(diagEnvData, &LinDiagImp_InterfaceFunctions, errorCode, dataDumpReqBuffer, dataDumpRespBuffer, diagEnvData->CallbackCtxData);
    }
    else{}
  }
  return result;
}

/***************************************************************************//**
 * DataDump service handler
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_DataDump( LinTransIf_cpThis_t thisTransIf,
                                                                      LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                      LinTransIf_cpSIDCbServiceData_t serviceData,
                                                                      LinTransIf_pGenericCbCtxData_t genericTransCbCtxData)
{
  LinDiagIf_sDataDumpData_t loc_DataDumpBuffer;
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  if ( serviceData->DataLen == LINDIAGIF_DATA_DUMP_REQLEN )
  {
    // call application callback for handling
    Lin_Bool_t dataDumpOK = loc_CallDataDumpCallback(diagEnvData, serviceData->ErrorCode, (LinDiagIf_pDataDumpData_t)serviceData->DataBuffer, &loc_DataDumpBuffer  );
  
    // if broadcast: send no response (neither positive nor negative)
    if (serviceData->NAD == LINTRANSIF_BROADCAST_NAD || dataDumpOK == LIN_FALSE)
    {
      returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
    }
    else
    {
      // send positive response
      if ( thisRespIf->IfFunsTbl->InitResponse( thisRespIf->envData, diagEnvData->NAD,  LinDiagIf_SID_DataDump + LINDIAGIF_RSID_OFFSET, LINDIAGIF_DATA_DUMP_REQLEN ) == LIN_TRUE)
      {
        if ( thisRespIf->IfFunsTbl->AppendDataToMsgBuffer(thisRespIf->envData, (LinTransIf_cpData_t)&loc_DataDumpBuffer, LINDIAGIF_DATA_DUMP_REQLEN ) == LIN_TRUE )
        {
          returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
        }
      }
    }
  }
  else{ /* Error: wrong request length. */ }

  return(returnValue);
}

#endif


#if LINDIAG_SUP_SAVE_CONFIGURATION == 1
/***************************************************************************//**
 * Callback for SaveConfiguration
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CallSaveConfigurationCallback(loc_pDiagEnvironmentData_t diagEnvData, LinTransIf_Error_t errorCode)
{
  Lin_Bool_t result = LIN_FALSE;
  const LinDiagIf_cpCallbackFunctions_t callbacks = diagEnvData->Callbacks;
  if(callbacks != LIN_NULL)
  {
    const LinDiagIf_SaveConfigurationCbFun_t fun = callbacks->SaveConfiguration;
    if (fun != LIN_NULL)
    {
      result = fun(diagEnvData, &LinDiagImp_InterfaceFunctions, errorCode, diagEnvData->CallbackCtxData);
    }
    else{}
  }
  return result;
}


/***************************************************************************//**
 * SAE-J2602 5.7.3: Targeted Reset
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_SaveConfiguration(LinTransIf_cpThis_t                     thisTransIf,
                                                                              LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                              LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                              LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  // call application callback for reset
  Lin_Bool_t saveOK = loc_CallSaveConfigurationCallback(diagEnvData, serviceData->ErrorCode);

  // if broadcast: send no response (neither positive nor negative)
  if (serviceData->NAD == LINTRANSIF_BROADCAST_NAD || saveOK == LIN_FALSE)
  {
    returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
  }
  else
  {
    // send positive response (with no data) 
    thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->NAD,  LinDiagIf_SID_SaveConfiguration + LINDIAGIF_RSID_OFFSET, 0);
    returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
  }

  return(returnValue);
}

#endif /* LINDIAG_SUP_SAVE_CONFIGURATION */

#if LINDIAG_SUP_ASSIGN_NAD == 1
/***************************************************************************//**
 *
 ******************************************************************************/

// LJM added on 20230425
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AssignNAD(LinTransIf_cpThis_t                     thisTransIf,
                                                                      LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                      LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                      LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  if(serviceData->DataLen == LINDIAGIF_ASSIGNNAD_REQLEN)
  {
    LinDiagIf_SupplierID_t supplierID = 0;
    LinDiagIf_FunctionID_t functionID = 0;

    supplierID = serviceData->DataBuffer[0];
    supplierID |= serviceData->DataBuffer[1] << 8;
    functionID = serviceData->DataBuffer[2];
    functionID |= serviceData->DataBuffer[3] << 8;

    if ( ((supplierID == 0x7fff) && (functionID == 0xffff)) ||
        ((supplierID == diagEnvData->ProductIdentification.SupplierID) && (functionID == diagEnvData->ProductIdentification.FunctionID)) )
    {     
      thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->InitialNAD, LinDiagIf_SID_AssignNAD + LINDIAGIF_RSID_OFFSET,  0);
      diagEnvData->NAD = serviceData->DataBuffer[4];
      returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
    }
    else{}
  }

  return(returnValue);
}
#endif

#if LINDIAG_SUP_ASSIGN_FRAME_IDENTIFIER == 1
/***************************************************************************//**
 *
 ******************************************************************************/
#include "LinDrvImp_FrameConfig.h"
uint8_t loc_GetFrameIDFromPID(uint8_t aPID)
{
  uint8_t cnt;
  for (cnt = 0; cnt < LIN_MAX_FRAMEID_CNT; cnt++)
  {
    if (loc_DIAG_PIDS[cnt] == aPID)
    {
      break;
    }
  }
  return cnt;
}

uint16_t SupplierIDLJM;
static inline LinTransIf_eSIDReaction_t loc_AssignFrameIdentifier(LinTransIf_cpThis_t                     thisTransIf,
                                                                  LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                  LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                  LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  Lin_Bool_t assign = LIN_FALSE;
  Lin_uint8_t frameId;
  SupplierIDLJM = (uint16_t)(((uint16_t)(serviceData->DataBuffer[1])<<(uint16_t)8) | (uint16_t)serviceData->DataBuffer[0]);
  if (genericTransCbCtxData != LIN_NULL)
  {
    if(serviceData->DataLen == 5)  // LJM added
    {
      // copy supplier id from received data (i.e.: (data[1] << 8) | data[0])
      SupplierIDLJM = (uint16_t)(((uint16_t)(serviceData->DataBuffer[1])<<(uint16_t)8) | (uint16_t)serviceData->DataBuffer[0]);
      // check if our supplier id
      if ((SupplierIDLJM == diagEnvData->ProductIdentification.SupplierID) || ( SupplierIDLJM == 0x7fff))  // LJM added
      {
        uint8_t pid =  serviceData->DataBuffer[4];

        if (pid == LOC_UNASSIGN_FRAME_ID_B1)
        {
          frameId = LinLookupIf_AssignFrameIDRangeCmd_UNASSIGN;
          assign = LIN_TRUE;
        }
        else
        {
          frameId = pid & LIN_PID_MASK;
          if(loc_DIAG_PIDS[frameId] == pid)
          {
            assign = LIN_TRUE;
          }
          else 
          {
          }
        }
      }
      else 
      {
        /* not our supplier-ID */ 
      }

      if(assign == LIN_TRUE)
      {
        thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->NAD, LinDiagIf_SID_AssignFrameID + LINDIAGIF_RSID_OFFSET, 0);
        returnValue = LinTransIf_SIDRA_SEND_RESPONSE;  
      }
      else
      {
      }
    }
    else
    {
    }
  }
  else
  {
    // genericTransCbCtxData == LIN_NULL
    /* TODO: ERROR */
  }

  return(returnValue);
}
#endif




#if LINDIAG_SUP_READ_BY_ID == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_ReadByIdentifier(LinTransIf_cpThis_t                     thisTransIf,
                                                                             LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                             LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                             LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  loc_sDiagServiceControlData_t diagSerCtrlData;

  /* Init Diagnostic Service Control Data. */
  if(loc_InitSerCtrlData(&diagSerCtrlData, genericTransCbCtxData, LinDiagIf_SID_ReadByIdentifier, LINDIAGIF_READBYIDENTIFIER_REQLEN,
                         loc_RLCT_Equal, LIN_FALSE) == LIN_TRUE)
  {
    if(loc_ValidateAndExtractServiceData(&diagSerCtrlData, serviceData->DataBuffer, serviceData->DataLen) == LIN_TRUE)
    {
      LinDiagIf_SupplierID_t supplierID = 0;
      LinDiagIf_FunctionID_t functionID = 0;
      LinDiagIf_RbIdentifier_t rbiIdentifier = (LinDiagIf_RbIdentifier_t) serviceData->DataBuffer[0];
      LinDiagIf_Length_t sendDataLen = 0;
      LinDiagIf_pRbiLookupEntry_t pRbiLookupEntry = LIN_NULL;

      /* Read out Supplier and Function ID */
      supplierID = serviceData->DataBuffer[1];
      supplierID |= serviceData->DataBuffer[2] << 8;
      functionID = serviceData->DataBuffer[3];
      functionID |= serviceData->DataBuffer[4] << 8;

      /* Are Supplier and Function ID correct or are the wildcards used? */
      if((supplierID == diagEnvData->ProductIdentification.SupplierID ||
          supplierID == LINDIAGIF_SUPPLIER_ID_WILDCARD) &&
         (functionID == diagEnvData->ProductIdentification.FunctionID ||
          functionID == LINDIAGIF_FUNCTION_ID_WILDCARD))
      {
        /* Lookup RBI identifier. */
        pRbiLookupEntry = loc_LookupRbi(diagEnvData, rbiIdentifier);

        if(pRbiLookupEntry != LIN_NULL)
        {
          /* Acquire 'Read by identifier' data length. */
          if(pRbiLookupEntry->LengthType == LinDiagIf_RbiLenType_Value)
          {
            sendDataLen = (LinDiagIf_Length_t) pRbiLookupEntry->Length;
          }
          else if(pRbiLookupEntry->LengthType == LinDiagIf_RbiLenType_Callback)
          {
            /* Callback null-pointer check. */
            if(pRbiLookupEntry->Length != LIN_NULL)
            {
              sendDataLen = ((LinDiagIf_RbILenCbFun_t) pRbiLookupEntry->Length)((LinDiagIf_pGenericEnvData_t) diagEnvData,
                                                                                &LinDiagImp_InterfaceFunctions, rbiIdentifier);
            }
            else
            {
              /* Length-Callback function is a null-pointer,
               * report error and send negative response. */
              diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
              diagSerCtrlData.NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;
              loc_CallErrorCallback(diagEnvData, diagSerCtrlData.RequestedSID, LinDiagIf_ERR_RBI_DEF);
            }
          }
          else
          {
            /* Invalid length-type in RBI definition. */
            diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
            diagSerCtrlData.NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;
            loc_CallErrorCallback(diagEnvData, diagSerCtrlData.RequestedSID, LinDiagIf_ERR_RBI_DEF);
          }
        }
        else
        {
          /* No RBI identifier found in lookup tables, send
           * negative response and report an error. */
          diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
          diagSerCtrlData.NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;
          loc_CallErrorCallback(diagEnvData, diagSerCtrlData.RequestedSID, LinDiagIf_ERR_RBI_NOT_FOUND);
        }

        if(diagSerCtrlData.SendNegativeResponse == LIN_FALSE)
        {
          if(loc_PreparePositiveResponse(&diagSerCtrlData, LIN_NULL, sendDataLen, 0, thisRespIf) == LIN_TRUE)
          {
            if(pRbiLookupEntry->Callback((LinDiagIf_pGenericEnvData_t) diagEnvData, &LinDiagImp_InterfaceFunctions,
                                         rbiIdentifier, &loc_AppendDataToMsgBuffer, pRbiLookupEntry->CbCtxData) != LIN_TRUE)
            {
              /* User returned LIN_FALSE, send negative
               * response but don't report and error. */
              diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
            }
            else
            {
              if(diagEnvData->UserAppendedDataLen != sendDataLen)
              {
                /* RBI data length mismatch, report error
                 * and send negative response. */
                diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
                diagSerCtrlData.NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;
                loc_CallErrorCallback(diagEnvData, diagSerCtrlData.RequestedSID, LinDiagIf_ERR_RBI_DATA_LEN);
              }
              else{}
            }
          }
          else{}
        }
        else{}
      }
      else
      {
        /* Answer with a negative response according to LIN specification
         * or not according to the LIN test specification. */
        if(diagEnvData->InvalidReadByIDAnswered == LIN_TRUE)
        {
          /* Invalid Supplier or Function ID, don't
           * report error, but send negative response. */
          diagSerCtrlData.SendNegativeResponse = LIN_TRUE;
          diagSerCtrlData.NegativeResponseCode = LinDiagIf_NRC_Sub_Function_Not_Supported;
        }
        else
        {
          diagSerCtrlData.SIDReaction = LinTransIf_SIDRA_IGNORED;
        }
      }
    }
    else
    {
       diagSerCtrlData.SendNegativeResponse = LIN_FALSE;
       diagSerCtrlData.SIDReaction = LinTransIf_SIDRA_IGNORED;
    }

    /* If an error occurred and LIN communication is
     * still possible, send a negative response. */
    if(diagSerCtrlData.SendNegativeResponse == LIN_TRUE)
    {
      loc_PrepareNegativeResponse(&diagSerCtrlData, thisRespIf);
    }
    else{}
  }
  else{}

  return(diagSerCtrlData.SIDReaction);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static Lin_Bool_t loc_RbiCbProdIdent(LinDiagIf_pGenericEnvData_t   genericDiagEnvData,   LinDiagIf_cpInterfaceFunctions_t        ifFunctions,
                                                     LinDiagIf_RbIdentifier_t      rbIdentifier,         LinDiagIf_AppendDataToMsgBuffer_t       appendDataToMsgBufFun,
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  Lin_Bool_t retVal = LIN_FALSE;
  LinDiagIf_Data_t buffer[5];

  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

    buffer[0] = (diagEnvData->ProductIdentification.SupplierID & 0x00FF)     ;
    buffer[1] = (diagEnvData->ProductIdentification.SupplierID & 0xFF00) >> 8;
    buffer[2] = (diagEnvData->ProductIdentification.FunctionID & 0x00FF)     ;
    buffer[3] = (diagEnvData->ProductIdentification.FunctionID & 0xFF00) >> 8;
    buffer[4] =  diagEnvData->ProductIdentification.VariantID;

    retVal = appendDataToMsgBufFun(genericDiagEnvData, (LinDiagIf_pData_t) buffer, sizeof(buffer));
  }

  return(retVal);
}

/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static Lin_Bool_t loc_RbiCbSerialNum(LinDiagIf_pGenericEnvData_t   genericDiagEnvData,   LinDiagIf_cpInterfaceFunctions_t        ifFunctions,
                                                     LinDiagIf_RbIdentifier_t      rbIdentifier,         LinDiagIf_AppendDataToMsgBuffer_t       appendDataToMsgBufFun,
                                                     LinDiagIf_pGenericCbCtxData_t genericDiagCbCtxData)
{
  Lin_Bool_t retVal = LIN_FALSE;

  if(genericDiagEnvData != LIN_NULL)
  {
    loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

    /*
    buffer[0] = (diagEnvData->SerialNumber & 0x000000FF)      ;
    buffer[1] = (diagEnvData->SerialNumber & 0x0000FF00) >> 8 ;
    buffer[2] = (diagEnvData->SerialNumber & 0x00FF0000) >> 16;
    buffer[3] = (diagEnvData->SerialNumber & 0xFF000000) >> 24;*/

    retVal = appendDataToMsgBufFun(genericDiagEnvData, (LinDiagIf_pData_t) &(diagEnvData->SerialNumber), sizeof(diagEnvData->SerialNumber));
  }

  return(retVal);
}
#endif

#if LINDIAG_SUP_CONDITIONAL_CHANGE_NAD == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_ConditionalChangeNAD(LinTransIf_cpThis_t                     thisTransIf,
                                                                                 LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                                 LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                                 LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;

  if(serviceData->DataLen == LINDIAGIF_CONDITIONALCHANGENAD_REQLEN)
  {
    LinDiagIf_pConditionalChangeNADData_t pCondChgNADData = (LinDiagIf_pConditionalChangeNADData_t) serviceData->DataBuffer;
    LinDiagIf_Data_t result = 0xFF;
    LinDiagIf_pData_t pByte;
    Lin_uint8_t buffer[5];

    switch(pCondChgNADData->Id)
    {
      case LINDIAGIF_RB_IDENTIFIER_PROD_IDENT:
        if(pCondChgNADData->Byte < (sizeof(diagEnvData->ProductIdentification.SupplierID) +
                                    sizeof(diagEnvData->ProductIdentification.FunctionID) +
                                    sizeof(diagEnvData->ProductIdentification.VariantID)))
        {
          buffer[0] = diagEnvData->ProductIdentification.SupplierID & 0x00FF;
          buffer[1] = (diagEnvData->ProductIdentification.SupplierID & 0xFF00) >> 8;
          buffer[2] = diagEnvData->ProductIdentification.FunctionID & 0x00FF;
          buffer[3] = (diagEnvData->ProductIdentification.FunctionID & 0xFF00) >> 8;
          buffer[4] = diagEnvData->ProductIdentification.VariantID;

          result = buffer[(pCondChgNADData->Byte) - 1];
          result ^= pCondChgNADData->Invert;
          result &= pCondChgNADData->Mask;
        }
        else{}
        break;

      case LINDIAGIF_RB_IDENTIFIER_SERIAL_NUM:
        if(pCondChgNADData->Byte < sizeof(diagEnvData->SerialNumber))
        {
          pByte = (LinDiagIf_pData_t) &(diagEnvData->SerialNumber);
          result = pByte[(pCondChgNADData->Byte) - 1];
          result ^= pCondChgNADData->Invert;
          result &= pCondChgNADData->Mask;
        }
        else{}
        break;
      default:
        break;
    }

    if(result == 0)
    {
      thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->NAD, LinDiagIf_SID_ConditionalChangeNAD + LINDIAGIF_RSID_OFFSET, 0);
      diagEnvData->NAD = pCondChgNADData->NewNAD;
      returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
    }
  }

  return(returnValue);
}
#endif




#if LINDIAG_SUP_ASSIGN_FRAME_ID_RANGE == 1
/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AssignFrameIdentifierRange(LinTransIf_cpThis_t                     thisTransIf,
                                                                                       LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                                       LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                                       LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;
  LinTransIf_eSIDReaction_t  returnValue = LinTransIf_SIDRA_IGNORED;
  Lin_Bool_t assign = LIN_TRUE;

  if(genericTransCbCtxData != LIN_NULL)
  {
    if(serviceData->DataLen == LINDIAGIF_ASSIGNFRAMEIDRANGE_REQLEN)
    {
      /* Convert the "Assign frame ID Range" values to internal values */
      Lin_uint8_t i;
      for(i = 1; i < serviceData->DataLen; i++)
      {
        if(serviceData->DataBuffer[i] == LOC_UNASSIGN_FRAME_ID_B7)
        {
          serviceData->DataBuffer[i] = LinLookupIf_AssignFrameIDRangeCmd_UNASSIGN;
        }
        else if(serviceData->DataBuffer[i] != 0xFF)
        {
          /* Convert PIDs to Frame IDs */
          if(loc_DIAG_PIDS[serviceData->DataBuffer[i] & LIN_PID_MASK] == serviceData->DataBuffer[i])
          {
            Lin_uint8_t tempFrameID;
            tempFrameID = serviceData->DataBuffer[i] & LIN_PID_MASK;
            if(tempFrameID < 0x3C)
            {
              serviceData->DataBuffer[i] = tempFrameID;
            }
            else
            {
              assign = LIN_FALSE;
            }
          }
          else{}
        }
        else{}
      }

      if(assign == LIN_TRUE)
      {
        if(diagEnvData->LookupThisPtr.IfFunsTbl->AssignFrameIDRange(diagEnvData->LookupThisPtr.envData, serviceData->DataBuffer[0], &serviceData->DataBuffer[1], 4) == LIN_TRUE)
        {
          thisRespIf->IfFunsTbl->InitResponse(thisRespIf->envData, diagEnvData->NAD, LinDiagIf_SID_AssignFrameIDRange + LINDIAGIF_RSID_OFFSET, 0);
          returnValue = LinTransIf_SIDRA_SEND_RESPONSE;
        }
      }
      else{}
    }
  }
  else
  {
    /* TODO: ERROR */
  }

  //LinTransImp_InterfaceFunctions.AppendDataToMsgBuffer(&LinTransEnvData, data, dataLen);

  return(returnValue);
}
#endif

#if LINDIAG_SUP_SNPD == 1
/***************************************************************************//**
 * Callback for LIN SNPD Start Measurement API call
 ******************************************************************************/
DIAG_LAYER_CODE static inline Lin_Bool_t loc_CallStartMeasurement(LinDiagIf_pGenericEnvData_t genericDiagEnvData)
{
  Lin_Bool_t rv = LIN_FALSE;
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

  LinSNPDIf_cpInterfaceFunctions_t ifuns = diagEnvData->LinSNPDIfThisPointer.IfFunsTbl;
  if(ifuns != LIN_NULL)
  {
    const LinSNPDIf_StartMeasurementIfFun_t fun = ifuns->StartMeasurement;
    if (fun != LIN_NULL)
    {
      rv = fun(diagEnvData->LinSNPDIfThisPointer.envData);
    }
    else{}
  }

  return rv;
}

/***************************************************************************//**
 * Callback for LIN SNPD End Measurement API call
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallEndMeasurement(LinDiagIf_pGenericEnvData_t genericDiagEnvData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

  LinSNPDIf_cpInterfaceFunctions_t ifuns = diagEnvData->LinSNPDIfThisPointer.IfFunsTbl;
  if(ifuns != LIN_NULL)
  {
    const LinSNPDIf_EndMeasurementIfFun_t fun = ifuns->EndMeasurement;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData->LinSNPDIfThisPointer.envData);
    }
    else{}
  }
}

/***************************************************************************//**
 * Callback for LIN SNPD Prepare Next Measurement API call
 ******************************************************************************/
DIAG_LAYER_CODE static inline void loc_CallNextMeasuremen(LinDiagIf_pGenericEnvData_t genericDiagEnvData, Lin_Bool_t addressed)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

  LinSNPDIf_cpInterfaceFunctions_t ifuns = diagEnvData->LinSNPDIfThisPointer.IfFunsTbl;
  if(ifuns != LIN_NULL)
  {
    const LinSNPDIf_NextMeasurementIfFun_t fun = ifuns->NextMeasurement;
    if (fun != LIN_NULL)
    {
      fun(diagEnvData->LinSNPDIfThisPointer.envData,addressed);
    }
    else{}
  }
}

/***************************************************************************//**
 * Callback for LIN SNPD Get Current SNPD state API call
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinSNPDIf_eState_t loc_CallSNPDGetState(LinDiagIf_pGenericEnvData_t genericDiagEnvData)
{
  LinSNPDIf_eState_t rv = LinSNPDIf_DISABLED;
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericDiagEnvData;

  LinSNPDIf_cpInterfaceFunctions_t ifuns = diagEnvData->LinSNPDIfThisPointer.IfFunsTbl;
  if(ifuns != LIN_NULL)
  {
    const LinSNPDIf_GetStateIfFun_t fun = ifuns->GetState;
    if (fun != LIN_NULL)
    {
      rv = fun(diagEnvData->LinSNPDIfThisPointer.envData);
    }
    else{}
  }

  return rv;
}


/***************************************************************************//**
 *
 ******************************************************************************/
DIAG_LAYER_CODE static inline LinTransIf_eSIDReaction_t loc_AutoAddrRequest(LinTransIf_cpThis_t                     thisTransIf,
                                                                            LinTransIf_cpResponseSubInterfaceThis_t thisRespIf,
                                                                            LinTransIf_cpSIDCbServiceData_t         serviceData,
                                                                            LinTransIf_pGenericCbCtxData_t          genericTransCbCtxData)
{
  loc_pDiagEnvironmentData_t diagEnvData = (loc_pDiagEnvironmentData_t) genericTransCbCtxData;

  LinTransIf_eSIDReaction_t returnValue = LinTransIf_SIDRA_IGNORED;

  if(genericTransCbCtxData != LIN_NULL)
  {
    if ( ( diagEnvData->UseOldSNPDServiceID == LIN_TRUE && serviceData->SID == LinDiagIf_SID_Old_SNPD ) || 
         ( diagEnvData->UseOldSNPDServiceID == LIN_FALSE && serviceData->SID == LinDiagIf_SID_SNPD ) )
    {
    
      if(serviceData->DataLen == LINDIAG_SNPD_REQ_MSG_LEN)
      {
        if ( loc_CallCheckSNPDMethodId(diagEnvData, serviceData->DataBuffer[LINDIAG_SNPD_METHOD_ID_POS]) == LinDiagIf_MethodId_Accept )
        {
          
          switch(serviceData->DataBuffer[LINDIAG_SNPD_CMD_POS])
          {
          /* Start SNPD Measurement. */
          case LINDIAG_SNPD_CMD_MEASURE:
            {
              diagEnvData->SNPDNewNAD = 0u;
              diagEnvData->SNPDNewNADValid = LIN_FALSE;
              
              diagEnvData->SNPDStarted = loc_CallStartMeasurement(diagEnvData);
              diagEnvData->SNPDAddressed = LIN_FALSE;
              
              returnValue=LinTransIf_SIDRA_SUCCESSFULLY_PROC;
              break;
            }
          /* Keep NAD frame. */  
          case LINDIAG_SNPD_CMD_KEEP_NAD:
            {
              if (diagEnvData->SNPDStarted != LIN_FALSE )
              {
                LinSNPDIf_eState_t st=loc_CallSNPDGetState(diagEnvData);
                
              /* Check status of SNPD round. */
                switch(st)
                {
                case LinSNPDIf_DONE_LAST: // keep nad ...
                  {
                    LinTransIf_NAD_t tmpnad=serviceData->DataBuffer[LINDIAG_SNPD_NAD_POS];
                    
                    if ( tmpnad != LINTRANSIF_FUNCTIONAL_NAD &&
                        tmpnad != LINTRANSIF_GOTOSLEEP_NAD     &&
                          tmpnad != LINTRANSIF_BROADCAST_NAD  )
                    {
                      diagEnvData->SNPDNewNAD = tmpnad;
                      diagEnvData->SNPDNewNADValid = LIN_TRUE;
                      
                      if ( diagEnvData->ImmediateNADUpdate != LIN_FALSE )
                      {
                        diagEnvData->NAD = diagEnvData->SNPDNewNAD;
                      }
                      else {}
                      
                    }
                    
                    diagEnvData->SNPDAddressed = LIN_TRUE;
                    loc_CallNextMeasuremen(diagEnvData,LIN_TRUE);  // TODO: eventually check return value for LIN_TRUE
                    break;
                  }
                  
                case LinSNPDIf_DONE_NOT_LAST: // nothing to do...
                case LinSNPDIf_SKIPED:
                  {
                    loc_CallNextMeasuremen(diagEnvData,diagEnvData->SNPDAddressed); // TODO: eventually check return value for LIN_TRUE
                    break;
                  }
                  
                  
                case LinSNPDIf_DISABLED: // these are error cases ....
                case LinSNPDIf_WAITING:
                case LinSNPDIf_FAILED:
                default:
                  {
                    diagEnvData->SNPDNewNAD      = 0u;   
                    diagEnvData->SNPDNewNADValid = LIN_FALSE;
                    diagEnvData->SNPDStarted     = LIN_FALSE;
                    diagEnvData->SNPDAddressed   = LIN_FALSE;
                    
                    loc_CallEndMeasurement(diagEnvData);
                    
                    break;
                  }
                }
              }
              
              returnValue=LinTransIf_SIDRA_SUCCESSFULLY_PROC;
              break;
            }
            
            
          case LINDIAG_SNPD_CMD_STORE_NAD:
            {
              if ( (diagEnvData->SNPDStarted != LIN_FALSE ) && ( diagEnvData->SNPDNewNADValid != LIN_FALSE ) )
              {
#if LINDIAG_SUP_DATASTG == 1
                if (diagEnvData->DataStgThisPtr.IfFunsTbl->SetDataValue( diagEnvData->DataStgThisPtr.envData, 
				                                                         LinDataStgIf_DVID_NAD,
                                                                         &diagEnvData->SNPDNewNAD, 
																		 sizeof(diagEnvData->SNPDNewNAD ) ) 
																		 == LinDataStgIf_ERR_NO_ERROR ) )
                {
                  diagEnvData->InitialNAD = diagEnvData->SNPDNewNAD;
                  diagEnvData->NAD = diagEnvData->SNPDNewNAD;
                }
                else {}
#else
                loc_CallStoreNADCallback(diagEnvData, diagEnvData->SNPDNewNAD, diagEnvData->CallbackCtxData);
                diagEnvData->InitialNAD = diagEnvData->SNPDNewNAD;    //Assign new NAD to initial NAD.
                diagEnvData->NAD        = diagEnvData->SNPDNewNAD;    //Assign new NAD to current NAD.
#endif
              }
              else {}
              
              returnValue=LinTransIf_SIDRA_SUCCESSFULLY_PROC;
              break;
            }
            
            /* Stop autoaddr frame. */  
          case LINDIAG_SNPD_CMD_DISABLE:
            {
              diagEnvData->SNPDNewNAD       = 0u;
              diagEnvData->SNPDNewNADValid  = LIN_FALSE;   //Invalidate new NAD valid flag.
              diagEnvData->SNPDStarted      = LIN_FALSE;   //Clear SNPD active flag.
              diagEnvData->SNPDAddressed    = LIN_FALSE;   //Clear SNPD addressed status.
              
              loc_CallEndMeasurement(diagEnvData);         //Stop actual SNPD.
              
              returnValue = LinTransIf_SIDRA_SUCCESSFULLY_PROC;
              break;
            }
            
          default:
            {
              break;
            }
          }
        } 
        else {} /* loc_CallCheckSNPDMethodId */
      }
      else {}   /* dataLen == LINDIAG_SNPD_REQ_MSG_LEN */
    }
    else {}   /* service ID is not matching new/old SID config */      
  }
  else {}     /* genericTransCbCtxData != LIN_NULL */

  return(returnValue);
}
#endif



/*! @} */ /* DIAG */
/*! @} */ /* ELMOS_LIN_DRIVER */

