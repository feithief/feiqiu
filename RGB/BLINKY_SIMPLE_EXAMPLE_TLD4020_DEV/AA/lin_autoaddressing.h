#ifndef _LIN_AUTOADDRESSING_H_
#define _LIN_AUTOADDRESSING_H_

#include "SystemType.h"
#include "genLinConfig.h"

/** @file
*
* @defgroup LIN_AUTOADDRESSING LIN auto-addressing function.
* @{
*
* @ingroup RGB_SYSTEM
*
* @brief This module is the state machine of auto-addressing, and other functions needed by auto-addressing.
*
* @details This module is the state machine of auto-addressing, and other functions needed by auto-addressing.
*/

#ifdef LIN_ENABLE_AA_BSM_B5

/**@brief defines the initial function for autp-addressing, user may need to do something when the AA function starts.*/
typedef void (*AA_INITIAL_CALLBACK)(void);

#define D_AUTO_ADDR_STEP1_START_AA    (0x1)
#define D_AUTO_ADDR_STEP2_ASSIGN_NAD  (0x2)
#define D_AUTO_ADDR_STEP3_STORE_NAD   (0x3)
#define D_AUTO_ADDR_STEP4_FINISH_AA   (0x4)

typedef enum AddressStatus
{
    AddressStatusSuccess = 0x0,
    AddressStatusWatting = 0x1,
}E_AddrStatus;

typedef enum StateMachineStatus
{
    E_StateMachineStatusIdle        = 0x0,
    E_StateMachineStatusStart       = 0x1,
    E_StateMachineStatusWaitting    = 0x2,
    E_StateMachineStatusAddressed   = 0x4,/*already addressed or error occur*/
}E_StateMachineStatus;


typedef struct AAMeasureData
{
    int16_t ADCMeasDiffOff;
    int16_t ADCMeasDiffPre;
    int16_t ADCMeasDiffSel;
    uint8_t isPreNode, isLastNode;
    //uint8_t lastSucMeasResult;
}S_AAMeasureData;
/**
*@details   LIN Auto-adressing initial function.
*
*@param[in] cb  callback function when AA starts.
*
*@retval    none.
*/
void LIN_AutoAddr_Init(AA_INITIAL_CALLBACK cb_start, AA_INITIAL_CALLBACK cb_end);

/**
*@details   LIN Auto-adressing entry function, when receives the enter auto-addressing command, call this function.
*
*@retval    none.
*/
void LIN_AutoAddr_Start(void);

/**
*@details   LIN Auto-adressing measure interrupt service routine, should be called by ADC interrupt.
*
*@retval    none.
*/
void LIN_AutoAddr_Measure_ISR(void);

/**
*@details   LIN Auto-adressing error interrupt service routine, should be called by error interrupt.
*
*@retval    none.
*/
void LIN_AutoAddr_Error_ISR(void);

/**
*@details   LIN Auto-adressing falling/rising edge interrupt service routine, should be called by EXF2 interrupt.
*
*@retval    none.
*/
void LIN_AutoAddr_Edge_ISR(void);

/**
*@details   When recives the new nad call this function, the return status will tell you the addressing status.
*
*@retval    retuens the status, if @AddressStatusSuccess then success otherwise this module is still waitting.
*/
E_AddrStatus LIN_AutoAddr_Assign_NAD(void);

/**
*@details   LIN Auto-adressing break signal rising edge interrupt service routine.
*
*@retval    none.
*/
void LIN_AutoAddr_Meas_Exit(void);

/**
*@details   When recives the end auto-addressing command, call this function.
*
*@retval    none.
*/
void LIN_AutoAddr_End(void);

/**
*@details   Get the lin auto-addressing status.
*
*@retval    the auto-addressing statemachine status.
*/
E_StateMachineStatus LIN_AutoAddr_Get_Status(void);

//void LIN_AutoAddr_Reset_flag(void);

#endif/*endif AUTOADDRESSING_ENABLED*/

/** @} */

#endif
