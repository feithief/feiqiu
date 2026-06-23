#include "lin_autoaddressing.h"
#include "lin_hal.h"
#include "adc11.h"
#include "ModuleClock.h"
#include "lin_autoaddressing_cfg.h"
#ifdef LIN_ENABLE_AA_BSM_B5

#define D_PRE_DIFF_THRESHOLD_POS    (g_linAutoAddrConfig.preDiffThresholdPos)
#define D_SEL_DIFF_THRESHOLD_POS    (g_linAutoAddrConfig.selDiffThresholdPos)

//#define D_FALLINGEDGE_DETECT_GAP    6

/**@brief Variant controls state Machine.*/
 volatile E_StateMachineStatus currentMachineStatus = E_StateMachineStatusIdle;
volatile uint32_t a = 0;
typedef enum AAMeasureStatus
{
    E_AAMeasureStatusIdle           = 0x0,
    //E_AAMeasureStatusCloseAll       = 0x0,
    E_AAMeasureStatusMeasureOffset  = 0x1,
    //E_AAMeasureStatusOpenC1         = 0x2,
    E_AAMeasureStatusMeasureC1      = 0x3,
    //E_AAMeasureStatusOpenC2         = 0x4,
    E_AAMeasureStatusMeasureC2_S1   = 0x5,
	  E_AAMeasureStatusMeasureC2_S2   = 0x6,
    //E_AAMeasureStatusSkip           = 0x6,
    E_AAMeasureStatusEnd            = 0x7,
}E_AAMeasureStatus;

 volatile E_AAMeasureStatus currentMeaStatus = E_AAMeasureStatusIdle;



 volatile S_AAMeasureData measureData = 
{
    .ADCMeasDiffOff = 0,
    .ADCMeasDiffPre = 0,
    .ADCMeasDiffSel = 0,
    .isPreNode = 0,
    .isLastNode = 0,
    //.lastSucMeasResult = 0,
};

 volatile S_AAMeasureData measureSuccessData = 
{
    .ADCMeasDiffOff = 0,
    .ADCMeasDiffPre = 0,
    .ADCMeasDiffSel = 0,
    .isPreNode = 0,
    .isLastNode = 0,
    //.lastSucMeasResult = 0,
};
volatile int16_t measureDataLinOutDiffOff = 0;
volatile int16_t measureDataLinOutDiffPre = 0;
volatile int16_t measureDataLinOutDiffSel = 0;
static volatile int16_t measureDataLinOutDiffOffPending = 0;
static volatile int16_t measureDataLinOutDiffPrePending = 0;
static volatile int16_t measureDataLinOutDiffSelPending = 0;

/**@brief Instance of the start of LIN AA callback function.*/
 AA_INITIAL_CALLBACK internal_callback_start = 0, internal_callback_end = 0;
 volatile uint8_t waittingForFallingEdge = 0;
 static volatile uint32_t linAaWaitIen = LIN_LIN_IEN_EXF2EN_Msk;
 static volatile uint32_t linAaMeasIen = (LIN_LIN_IEN_EXF2EN_Msk | LIN_LIN_IEN_AA_MEAS_RDY_IEN_Msk | LIN_LIN_IEN_AA_ERR_IEN_Msk);
 //tick_t lastRisingEdgeTick = 0;

volatile uint32_t count_succ = 0, count_fail = 0;
uint32_t debug_count_enterloop = 0, debug_count_startaa = 0;
uint32_t debug_count_aa_err = 0, debug_count_brkerr = 0, debug_count_adcerr = 0, debug_err_unknow = 0, debug_statemachine_err = 0;
tick_t debugTickBegin = 0, debugTickEnd = 0;
uint16_t debugLINM_PRE = 0, debugLINM_SEL = 0, debugLINS_PRE = 0, debugLINS_SEL = 0;
int16_t maxOffset = 0, minOffset = 0;
int16_t maxPre = 0, minPre = 0;
int16_t maxSel = 0, minSel = 0;
#define LIN_AA_T2_CON_EDGE_CFG         (g_linAutoAddrConfig.t2ConEdge)
#define LIN_AA_T2_MOD_FALLING_EDGE_CFG (g_linAutoAddrConfig.t2ModFallingEdge)
#define LIN_AA_T2_MOD_RISING_EDGE_CFG  (g_linAutoAddrConfig.t2ModRisingEdge)
#define LIN_AA_SET_FALLING_EDGE_FAST() do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_FALLING_EDGE_CFG; waittingForFallingEdge = 1; } while (0)
#define LIN_AA_SET_RISING_EDGE_FAST()  do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_RISING_EDGE_CFG; waittingForFallingEdge = 0; } while (0)
#define LIN_AA_SET_RISING_EDGE_ARMED_FAST() do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_RISING_EDGE_CFG; waittingForFallingEdge = 0; } while (0)
#define LIN_AA_ADC_READY_FAST()        ((ADC11->RESULT.reg & ADC11_RESULT_ADC_READY_Msk) != 0u)
#define LIN_AA_ADC_REINIT_FAST()       do { ADC11->CFG_0.reg = g_linAutoAddrConfig.adcCfg0Reinit; ADC11->IEN.reg = g_linAutoAddrConfig.adcIenReinit; ADC11->CFG_1.reg = g_linAutoAddrConfig.adcCfg1Reinit; } while (0)
#define LIN_AA_PREPARE_ADC_FAST()      do { while (!LIN_AA_ADC_READY_FAST()); LIN_AA_ADC_REINIT_FAST(); } while (0)
#define LIN_AA_PREPARE_ENTRY_FAST()    do { LIN_AA_PREPARE_ADC_FAST(); LIN->AA_I_DIFF_TH.reg = LIN_AutoAddr_BuildThresholdReg(); } while (0)
#define LIN_AA_MEASURE_RES_LIN_S_FAST() ((int16_t)(LIN->AA_ACC_RES_1.reg & LIN_AA_ACC_RES_1_ACC_RES_1_Msk))
#define LIN_AA_MEASURE_RES_LIN_M_FAST() ((int16_t)(LIN->AA_ACC_RES_2.reg & LIN_AA_ACC_RES_2_ACC_RES_2_Msk))
#define LIN_AA_START_PHASE_FAST(cfg, start) do { LIN->AA_CTRL.reg = (cfg); LIN->AA_CTRL.reg = (start); } while (0)
#define LIN_AA_MEAS_IEN_MASK          (LIN_LIN_IEN_AA_MEAS_RDY_IEN_Msk | LIN_LIN_IEN_AA_ERR_IEN_Msk)
#define LIN_AA_CAPTURE_WAIT_IEN_FAST() do { linAaWaitIen = (LIN->LIN_IEN.reg & ~LIN_AA_MEAS_IEN_MASK); linAaMeasIen = (linAaWaitIen | LIN_AA_MEAS_IEN_MASK); } while (0)
#define LIN_AA_ENABLE_MEAS_IEN_FAST() do { LIN->LIN_IEN.reg = linAaMeasIen; } while (0)
#define LIN_AA_DISABLE_MEAS_IEN_FAST() do { LIN->LIN_IEN.reg = linAaWaitIen; } while (0)
#define LIN_AA_PRELOAD_FSM_RESET_FAST() do { LIN->AA_CTRL.reg = AA_CTRL_FSM_RESET_ONLY; } while (0)
/* AA_CTRL register values; *_START includes PH_START and is written as the final trigger. */
#define AA_CTRL_FSM_RESET_ONLY         (g_linAutoAddrConfig.ctrlFsmResetOnly)
#define AA_CTRL_OFFSET_PHASE_ACTIVE    (g_linAutoAddrConfig.ctrlOffsetActive)
#define AA_CTRL_OFFSET_PHASE_INACTIVE  (g_linAutoAddrConfig.ctrlOffsetInactive)
#define AA_CTRL_PRE_PHASE_ACTIVE       (g_linAutoAddrConfig.ctrlPreActive)
#define AA_CTRL_SEL_PHASE_ACTIVE       (g_linAutoAddrConfig.ctrlSelActive)
#define AA_CTRL_SEL_PHASE_INACTIVE     (g_linAutoAddrConfig.ctrlSelInactive)
#define AA_CTRL_END_PHASE              (g_linAutoAddrConfig.ctrlEndPhase)
#define AA_CTRL_OFFSET_PHASE_ACTIVE_START    (g_linAutoAddrConfig.ctrlOffsetActiveStart)
#define AA_CTRL_OFFSET_PHASE_INACTIVE_START  (g_linAutoAddrConfig.ctrlOffsetInactiveStart)
#define AA_CTRL_PRE_PHASE_ACTIVE_START       (g_linAutoAddrConfig.ctrlPreActiveStart)
#define AA_CTRL_SEL_PHASE_ACTIVE_START       (g_linAutoAddrConfig.ctrlSelActiveStart)
#define AA_CTRL_SEL_PHASE_INACTIVE_START     (g_linAutoAddrConfig.ctrlSelInactiveStart)
static inline uint32_t LIN_AutoAddr_BuildThresholdReg(void)
{
    uint32_t thresholdReg = 0u;
    thresholdReg |= (((uint32_t)D_PRE_DIFF_THRESHOLD_POS << LIN_AA_I_DIFF_TH_I_DIFF_TH_1_VAL_Pos) &
                     LIN_AA_I_DIFF_TH_I_DIFF_TH_1_VAL_Msk);
    thresholdReg |= (((uint32_t)D_SEL_DIFF_THRESHOLD_POS << LIN_AA_I_DIFF_TH_I_DIFF_TH_2_VAL_Pos) &
                     LIN_AA_I_DIFF_TH_I_DIFF_TH_2_VAL_Msk);
    return thresholdReg;
}
static void enable_EXF2_Detect(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    LIN->LIN_ISC.reg = LIN_LIN_ISC_EXF2CLR_Msk;
    LIN->LIN_IEN.reg |= LIN_LIN_IEN_EXF2EN_Msk;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}
static void disable_EXF2_Detect(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    LIN->LIN_ISC.reg = LIN_LIN_ISC_EXF2CLR_Msk;
    LIN->LIN_IEN.reg &= ~LIN_LIN_IEN_EXF2EN_Msk;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}
/**
*@details   LIN Auto-adressing initial function.
*
*@param[in] cb  callback function when AA starts.
*
*@retval    none.
*/
void LIN_AutoAddr_Init(AA_INITIAL_CALLBACK cb_start, AA_INITIAL_CALLBACK cb_end)
{
    internal_callback_start = cb_start;
    internal_callback_end = cb_end;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    disable_EXF2_Detect();
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}
#if 0
uint8_t msg_received_flag = 0;
void LIN_AutoAddr_Reset_flag(void)
{
    msg_received_flag =1;
}
#endif
static void LIN_AutoAddr_SetFallingEdge(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    disable_EXF2_Detect();
    LIN->T2_CON.reg = LIN_AA_T2_CON_EDGE_CFG;
    LIN->T2_MOD.reg = LIN_AA_T2_MOD_FALLING_EDGE_CFG;
    waittingForFallingEdge = 1;
    enable_EXF2_Detect();
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}
static void LIN_AutoAddr_SetRisingEdge(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    disable_EXF2_Detect();
    LIN->T2_CON.reg = LIN_AA_T2_CON_EDGE_CFG;
    LIN->T2_MOD.reg = LIN_AA_T2_MOD_RISING_EDGE_CFG;
    waittingForFallingEdge = 0;
    enable_EXF2_Detect();
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}
//uint8_t AutoAdressFlg = 0;
/**
*@details   LIN Auto-adressing break falling edge interrupt service routine.
*
*@retval    none.
*/
static void LIN_AutoAddr_Meas_Entry(void)
{
    E_StateMachineStatus machineStatus = currentMachineStatus;
    E_AAMeasureStatus meaStatus = currentMeaStatus;

    GPIO->OUT.bit.P0 = 0;
    //debug_count_enterloop++;

    if (meaStatus == E_AAMeasureStatusIdle)
    {
        if (machineStatus == E_StateMachineStatusWaitting)
        {
            //debug_count_startaa++;
            /*measureData.ADCMeasDiffOff = 0;
            measureData.ADCMeasDiffPre = 0;
            measureData.ADCMeasDiffSel = 0;
            measureData.isPreNode = 0;*/

            LIN_AA_SET_RISING_EDGE_ARMED_FAST();
            LDRV->MOD_CFG.reg = 0u;
            LIN_AA_ENABLE_MEAS_IEN_FAST();
            currentMeaStatus = E_AAMeasureStatusMeasureOffset;
            LIN_AA_START_PHASE_FAST(AA_CTRL_OFFSET_PHASE_ACTIVE, AA_CTRL_OFFSET_PHASE_ACTIVE_START);
        }
        else if (machineStatus == E_StateMachineStatusAddressed)
        {
            //debug_count_startaa++;

            LIN_AA_SET_RISING_EDGE_ARMED_FAST();
            LDRV->MOD_CFG.reg = 0u;
            LIN_AA_ENABLE_MEAS_IEN_FAST();
            LIN->AA_FW_CTRL.reg = 1u;
            currentMeaStatus = E_AAMeasureStatusEnd;
            LIN_AA_START_PHASE_FAST(AA_CTRL_OFFSET_PHASE_INACTIVE, AA_CTRL_OFFSET_PHASE_INACTIVE_START);
        }
    }

    GPIO->OUT.bit.P0 = 1;
}
/**
*@details   LIN Auto-adressing break signal rising edge interrupt service routine.
*
*@retval    none.
*/
void LIN_AutoAddr_Meas_Exit(void)
{
    //lastRisingEdgeTick = moduleClockGet();
    if (currentMachineStatus == E_StateMachineStatusIdle)
    {
        debug_statemachine_err++;
        return;
    }
    currentMeaStatus = E_AAMeasureStatusIdle;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    LDRV->MOD_CFG.reg = LDRV_MOD_CFG_LD_ENABLE_Msk;
    LIN_AA_DISABLE_MEAS_IEN_FAST();
    LIN->LIN_ISC.reg = (LIN_LIN_ISC_AA_ERR_CLR_Msk | LIN_LIN_ISC_AA_MEAS_RDY_CLR_Msk);
    LIN->AA_CTRL.reg = AA_CTRL_END_PHASE;
    LIN_AA_PRELOAD_FSM_RESET_FAST();
    LIN_AutoAddr_SetFallingEdge();
    LIN_AA_CAPTURE_WAIT_IEN_FAST();
    LIN_AA_PREPARE_ENTRY_FAST();
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}

/**
*@details   LIN Auto-adressing error interrupt service routine, should be called by error interrupt.
*
*@retval    none.
*/
uint8_t debug_error = 0;
void LIN_AutoAddr_Error_ISR(void)
{
	
    // if (currentMachineStatus == E_StateMachineStatusIdle)
    // {
    //     LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/
    //     return;
    // }

    // /*reset flags*/
    // if (currentMachineStatus != E_StateMachineStatusAddressed)
    // {
    //     currentMachineStatus = E_StateMachineStatusWaitting;
    // }
    //LIN_AA_CTRL_ERR_CODE_CLR = 1;
    //currentMeaStatus = E_AAMeasureStatusIdle;

    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    debug_error = LIN_AA_STATUS_ERRORCODE;

    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
		if (debug_error & 0x1)
    {
        debug_count_adcerr++;
    }
		else if (debug_error & 0x2)
    {
        debug_count_brkerr++;
    }
		else
		{
				debug_err_unknow++;
		}

    debug_count_aa_err++;

    LIN_AutoAddr_Meas_Exit();
    /*restart mesurement*/
    //LIN_AutoAddr_Entry();
}

/**
*@details   LIN Auto-adressing falling/rising edge interrupt service routine, should be called by EXF2 interrupt.
*
*@retval    none.
*/

 volatile uint32_t fallingEdgeCounter = 0, risingEdgeCounter = 0; 
 volatile uint32_t breakCounter = 0, measureCounter = 0, dataCounter = 0;
 volatile tick_t beforeTick = 0, afterTick = 0, tickDiff = 0;
volatile tick_t debugMeasTickBefore = 0, debugMeasTickAfter = 0, debugMeasTickDiff = 0;
__attribute__((noinline)) static void LIN_AutoAddr_RisingEdge_ISR(void)
{
    if (LIN_AA_STATUS_TBIT >= 12)
    {
        breakCounter++;
        afterTick = moduleClockGet();
        tickDiff = afterTick - beforeTick;
        beforeTick = afterTick;
    }
    else
    {
        dataCounter++;
        fallingEdgeCounter--;
    }
    LIN_AutoAddr_Meas_Exit();
    risingEdgeCounter++;
}

void LIN_AutoAddr_Edge_ISR(void)
{
    if (waittingForFallingEdge)//falling edge
    {
        // GPIO->OUT.bit.P0 = 0;
    //    LIN_AutoAddr_SetRisingEdge();

            // LIN->AA_CTRL.reg = 0x03730085u;
        LIN_AutoAddr_Meas_Entry();
        fallingEdgeCounter++;
                // GPIO->OUT.bit.P0 = 1;
    }
    else//rising edge
    {
        LIN_AutoAddr_RisingEdge_ISR();
    }
}



/**
*@details   LIN Auto-adressing measure interrupt service routine, should be called by ADC interrupt.
*
*@retval    none.
*/

void LIN_AutoAddr_Measure_ISR(void)
{
    int16_t calTempLinM = 0, calTempLinS = 0;

    if (currentMachineStatus != E_StateMachineStatusWaitting)
    {
				debug_statemachine_err++;
        return;
    }

    switch (currentMeaStatus)
    {
                        case E_AAMeasureStatusMeasureOffset:
        {
            GPIO->OUT.bit.P0 = 0;
            currentMeaStatus = E_AAMeasureStatusMeasureC1;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            //measureData.ADCMeasDiffOff = (calTempLinS>>3) - (calTempLinM>>3);
            measureData.ADCMeasDiffOff = calTempLinS - calTempLinM;
            measureData.ADCMeasDiffOff = measureData.ADCMeasDiffOff>>3;
            measureData.ADCMeasDiffOff = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffOff);
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            LIN->LIN_ISC.reg = LIN_LIN_ISC_AA_ERR_CLR_Msk;
            LIN_AA_START_PHASE_FAST(AA_CTRL_PRE_PHASE_ACTIVE, AA_CTRL_PRE_PHASE_ACTIVE_START);
            GPIO->OUT.bit.P0 = 1;
            a = 1;
        }
        break;
                        case E_AAMeasureStatusMeasureC1:
        {
            GPIO->OUT.bit.P0 = 0;
            uint32_t aaCtrlReg = AA_CTRL_SEL_PHASE_INACTIVE;
            uint32_t aaCtrlStartReg = AA_CTRL_SEL_PHASE_INACTIVE_START;
            
//            currentMeaStatus = E_AAMeasureStatusMeasureC2_S1;
            currentMeaStatus = E_AAMeasureStatusMeasureC2_S2;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            debugLINM_PRE = calTempLinM;
            debugLINS_PRE = calTempLinS;
            //measureData.ADCMeasDiffPre = (calTempLinS>>3) - (calTempLinM>>3);
            measureData.ADCMeasDiffPre = calTempLinS - calTempLinM;
            measureData.ADCMeasDiffPre = measureData.ADCMeasDiffPre >> 3;
            measureData.ADCMeasDiffPre = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffPre);
            measureDataLinOutDiffOffPending = measureData.ADCMeasDiffOff;
            measureDataLinOutDiffPrePending = measureData.ADCMeasDiffPre;
            measureData.ADCMeasDiffPre = measureData.ADCMeasDiffPre - measureData.ADCMeasDiffOff; 
#if 0
            if (measureData.ADCMeasDiffPre <= 0)
            {
                measureData.ADCMeasDiffPre = 0;
            }
            else
            {
                measureData.ADCMeasDiffPre = measureData.ADCMeasDiffPre >> 3;
                measureData.ADCMeasDiffPre = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffPre);
                measureData.ADCMeasDiffPre = measureData.ADCMeasDiffPre - measureData.ADCMeasDiffOff;
            }
#endif
            if (measureData.ADCMeasDiffPre <= D_PRE_DIFF_THRESHOLD_POS)
            {
                aaCtrlReg = AA_CTRL_SEL_PHASE_ACTIVE;
                aaCtrlStartReg = AA_CTRL_SEL_PHASE_ACTIVE_START;
                measureData.isPreNode = 1;
            }
            else
            {
                measureData.isPreNode = 0;
            }
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            LIN->LIN_ISC.reg = LIN_LIN_ISC_AA_ERR_CLR_Msk;
            LIN_AA_START_PHASE_FAST(aaCtrlReg, aaCtrlStartReg);
        }
        GPIO->OUT.bit.P0 = 1;
        a = 2;
        break;
#if 0
				case E_AAMeasureStatusMeasureC2_S1:
				{
					currentMeaStatus = E_AAMeasureStatusMeasureC2_S2;
					/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
					calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
					calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
					/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/

					debugLINM_SEL = calTempLinM;
					debugLINS_SEL = calTempLinS;
					//measureData.ADCMeasDiffSel = (calTempLinS>>3) - (calTempLinM>>3);
					measureData.ADCMeasDiffSel = calTempLinS - calTempLinM;

					measureData.ADCMeasDiffSel = measureData.ADCMeasDiffSel >> 3;
					measureData.ADCMeasDiffSel = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffSel);
					measureData.ADCMeasDiffSel = measureData.ADCMeasDiffSel - measureData.ADCMeasDiffOff;
					
					if ((measureData.ADCMeasDiffSel <= D_SEL_DIFF_THRESHOLD_POS) 
							&& (measureData.isPreNode == 1))
					{
							measureData.isLastNode = 1;
//							measureSuccessData.ADCMeasDiffOff = measureData.ADCMeasDiffOff;
//							measureSuccessData.ADCMeasDiffPre = measureData.ADCMeasDiffPre;
//							measureSuccessData.ADCMeasDiffSel = measureData.ADCMeasDiffSel;
//							measureSuccessData.isLastNode = 1;
					}
					else
					{
							measureData.isLastNode = 0;
					}
					
					/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
					//LIN_AA_CTRL_FSM_RST = 1; /*force to reset firmware*/
					LIN_AA_CTRL_TBIT_START_MEAS = (D_STEP6_MEAS_SEL_S1_BIT<<1) | D_ADC_MEASURE_POSITION_SEL; /*pre measure step*/
					LIN_AA_CTRL_TBIT_START_ISRC = (D_STEP5_OPEN_C2_BIT<<1) | D_PULLUP_CONFIG_POSITION; /*open C1 source step*/

					LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/
					LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/

					//LIN_AA_CTRL_ISRC_EN = 1; /*enable current source*/
					//LIN_AA_CTRL_PU_DISC = 1; /*disable pull-up*/
					LIN_AA_CTRL_ISENSE_MEAS_NUMBER = 3; /*measure 8 samples*/
					LIN_AA_CTRL_ISENSE_EN = 1; /*measure enable*/
					LIN_AA_CTRL_LIN_AA_PH = 3; /*set firmware to SEL phase*/
					LIN_AA_CTRL_PH_START = 1; /*start the phase*/
					/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
				}
				break;
#endif
        case E_AAMeasureStatusMeasureC2_S2:
        {
            GPIO->OUT.bit.P0 = 0;
            currentMeaStatus = E_AAMeasureStatusEnd;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/

            debugLINM_SEL = calTempLinM;
            debugLINS_SEL = calTempLinS;
            //measureData.ADCMeasDiffSel = (calTempLinS>>3) - (calTempLinM>>3);
            measureData.ADCMeasDiffSel = calTempLinS - calTempLinM;

            measureData.ADCMeasDiffSel = measureData.ADCMeasDiffSel >> 3;
            measureData.ADCMeasDiffSel = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffSel);
            measureDataLinOutDiffSelPending = measureData.ADCMeasDiffSel;


            measureData.ADCMeasDiffSel = measureData.ADCMeasDiffSel - measureData.ADCMeasDiffOff;

						if (measureCounter == breakCounter)
						{
							if (measureData.ADCMeasDiffOff >= maxOffset)
							{
									maxOffset = measureData.ADCMeasDiffOff;
							}
							
							if (measureData.ADCMeasDiffOff <= minOffset)
							{
									minOffset = measureData.ADCMeasDiffOff;
							}

							if (measureData.ADCMeasDiffPre >= maxPre)
							{
									maxPre = measureData.ADCMeasDiffPre;
							}
							
							if (measureData.ADCMeasDiffPre <= minPre)
							{
									minPre = measureData.ADCMeasDiffPre;
							}

							if (measureData.ADCMeasDiffSel >= maxSel)
							{
									maxSel = measureData.ADCMeasDiffSel;
							}
							
							if (measureData.ADCMeasDiffSel <= minSel)
							{
									minSel = measureData.ADCMeasDiffSel;
							}
							
							debugMeasTickAfter = moduleClockGet();
							debugMeasTickDiff = debugMeasTickAfter - debugMeasTickBefore;
							debugMeasTickBefore = debugMeasTickAfter;

							measureCounter++;
							//if ((measureData.ADCMeasDiffSel < D_SEL_DIFF_THRESHOLD) && (measureData.isPreNode == 1) && (measureData.ADCMeasDiffSel > 0))
							if ((measureData.ADCMeasDiffSel <= D_SEL_DIFF_THRESHOLD_POS) 
									&& (measureData.isPreNode == 1) /*&& (measureData.isLastNode == 1)*/)
							{
									measureData.isLastNode = 1;
									measureSuccessData.ADCMeasDiffOff = measureData.ADCMeasDiffOff;
									measureSuccessData.ADCMeasDiffPre = measureData.ADCMeasDiffPre;
									measureSuccessData.ADCMeasDiffSel = measureData.ADCMeasDiffSel;
									measureSuccessData.isLastNode = 1;
									measureDataLinOutDiffOff = measureDataLinOutDiffOffPending;
									measureDataLinOutDiffPre = measureDataLinOutDiffPrePending;
									measureDataLinOutDiffSel = measureDataLinOutDiffSelPending;
									count_succ++;
							}
							else
							{
									count_fail++;
									measureData.isLastNode = 0;
//									measureSuccessData.ADCMeasDiffOff = measureData.ADCMeasDiffOff;
//									measureSuccessData.ADCMeasDiffPre = measureData.ADCMeasDiffPre;
//									measureSuccessData.ADCMeasDiffSel = measureData.ADCMeasDiffSel;
//									measureSuccessData.isLastNode = 0;
							}
						}
						else
						{
//							measureCounter = 0;
//							breakCounter = 0;
							while(1);
						}
            GPIO->OUT.bit.P0 = 1;            
        }
        
				a = 3;
        case E_AAMeasureStatusEnd:
        case E_AAMeasureStatusIdle:
        default:
            break;
    };
    // }
}

/**
*@details   LIN Auto-adressing entry function, when receives the enter auto-addressing command, call this function.
*
*@retval    none.
*/
// volatile uint32_t lin_aa_start_flag = 0;
void LIN_AutoAddr_Start(void)
{
	  count_succ = 0;
    count_fail = 0;
    debug_count_enterloop = 0;
    debug_count_startaa = 0;
    debug_count_aa_err = 0;
    debug_count_brkerr = 0;
    debug_count_adcerr = 0;
		debug_statemachine_err = 0;
    debugTickBegin = moduleClockGet();
		measureCounter = 0;
		breakCounter = 0;
		extern uint8_t B0FLAG;
		B0FLAG = 0;
//	lin_aa_start_flag = 0;
	
		//if (currentMachineStatus != E_StateMachineStatusIdle)
		{
			LIN_AutoAddr_End();
		}
	
//    if (currentMachineStatus == E_StateMachineStatusIdle)
//    {
        currentMachineStatus = E_StateMachineStatusStart;
        currentMeaStatus = E_AAMeasureStatusIdle;

        measureData.ADCMeasDiffOff = 0;
        measureData.ADCMeasDiffPre = 0;
        measureData.ADCMeasDiffSel = 0;
        measureData.isPreNode = 0;
        measureData.isLastNode = 0;
//    }
//		else
//		{
//			debug_statemachine_err++;
//		}

    //lastRisingEdgeTick = moduleClockGet() - D_FALLINGEDGE_DETECT_GAP;

    if (currentMachineStatus == E_StateMachineStatusStart)
    {
        if (internal_callback_start != 0)
        {
            internal_callback_start();
        }

        currentMachineStatus = E_StateMachineStatusWaitting;
        //currentMeaStatus = E_AAMeasureStatusIdle;
    }
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    LIN->AA_FW_CTRL.reg = 0u;
    LIN_AA_PRELOAD_FSM_RESET_FAST();
    LIN_AutoAddr_SetFallingEdge();
    LIN_AA_CAPTURE_WAIT_IEN_FAST();
    LIN_AA_PREPARE_ENTRY_FAST();
	/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}

/**
*@details   When recives the new nad call this function, the return status will tell you the addressing status.
*
*@retval    retuens the status, if @AddressStatusSuccess then success otherwise this module is still waitting.
*/
E_AddrStatus LIN_AutoAddr_Assign_NAD(void)
{
    if (currentMachineStatus == E_StateMachineStatusWaitting)
    {
					if (count_succ >= count_fail)
					{
						currentMachineStatus = E_StateMachineStatusAddressed;
						count_succ = 0;
						count_fail = 0;
//						lin_aa_start_flag++;
						return AddressStatusSuccess;
					}
					else
					{
//						if (lin_aa_start_flag == 0)
//						{
//							while(1);
//						}
						count_succ = 0;
						count_fail = 0;
						return AddressStatusWatting;
					}
    }
    else
    {
//			if (lin_aa_start_flag == 0)
//			{
//				while(1);
//			}
			count_succ = 0;
			count_fail = 0;
			return AddressStatusWatting;
    }
}

/**
*@details   When recives the end auto-addressing command, call this function.
*
*@retval    none.
*/
void LIN_AutoAddr_End(void)
{
//    if (currentMachineStatus == E_StateMachineStatusIdle)
//    {
//        return;
//    }
    currentMachineStatus = E_StateMachineStatusIdle;
    currentMeaStatus = E_AAMeasureStatusIdle;
    //close all AA register and interrupt
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    LIN_AA_DISABLE_MEAS_IEN_FAST();
    LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/
    LIN_AA_MEASURE_INT_CLR = 1;/*clear the measure interrupt*/
    LIN->AA_CTRL.reg = AA_CTRL_END_PHASE;
    disable_EXF2_Detect();
    LDRV->MOD_CFG.reg = LDRV_MOD_CFG_LD_ENABLE_Msk;
    //LIN->T2_CON.bit.EXEN2   = 0u;
    //LIN->LIN_IEN.bit.EXF2EN = 0u;
    //lin_hal_init_T2();
    //lin_hal_init();
    //LDRV->MOD_CFG.bit.LD_ENABLE = 1u;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    if (internal_callback_end != 0)
    {
        internal_callback_end();
    }
}

/**
*@details   Get the lin auto-addressing status.
*
*@retval    the auto-addressing statemachine status.
*/
E_StateMachineStatus LIN_AutoAddr_Get_Status(void)
{
    return currentMachineStatus;
}

#endif/*endif AUTOADDRESSING_ENABLED*/
