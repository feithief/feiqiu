#include "lin_autoaddressing.h"
#include "lin_hal.h"
#include "adc11.h"
#include "ModuleClock.h"

#ifdef LIN_ENABLE_AA_BSM_B5

#define LIN_AA_USE_DELAYED_STEP_TIMING
/* Select LIN AA phase timing by compile macro.
 * Default: 0/1/4/5/8/9/10/12 timing.
 * Define LIN_AA_USE_DELAYED_STEP_TIMING for 1/2/5/6/9/10 timing.
 */
#ifdef LIN_AA_USE_DELAYED_STEP_TIMING
#define D_STEP1_CLOSE_ALL_BIT    1
#define D_STEP2_MEAS_OFFSET_BIT  2
#define D_STEP3_OPEN_C1_BIT      5
#define D_STEP4_MEAS_PRE_BIT     6
#define D_STEP5_OPEN_C2_BIT      9
#define D_STEP6_MEAS_SEL_S1_BIT  10
#define D_STEP6_MEAS_SEL_S2_BIT  11
#define D_STEP7_RESTORE_BIT      13
#else
#define D_STEP1_CLOSE_ALL_BIT    0
#define D_STEP2_MEAS_OFFSET_BIT  1
#define D_STEP3_OPEN_C1_BIT      4
#define D_STEP4_MEAS_PRE_BIT     5
#define D_STEP5_OPEN_C2_BIT      8
#define D_STEP6_MEAS_SEL_S1_BIT  9
#define D_STEP6_MEAS_SEL_S2_BIT  10
#define D_STEP7_RESTORE_BIT      12
#endif

#define D_ADC_MESURE_AT_MIDDLE          1
#define D_ADC_MEASURE_AT_BEGINING       0

#ifdef LIN_AA_USE_DELAYED_STEP_TIMING
#define D_ADC_MEASURE_POSITION_OFFSET   D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_PRE      D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_SEL      D_ADC_MESURE_AT_MIDDLE
#else
#define D_ADC_MEASURE_POSITION_OFFSET   D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_PRE      D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_SEL      D_ADC_MESURE_AT_MIDDLE
#endif
#define D_PULLUP_CONFIG_AT_MIDDLE       1
#define D_PULLUP_CONFIG_AT_BEGINNING    0

#define D_PULLUP_CONFIG_POSITION  D_PULLUP_CONFIG_AT_BEGINNING

#define D_PRE_CURRENT_SETTING   LIN_AA_CURRENT_SETTING_0_45MA
#define D_SEL_CURRENT_SETTING   LIN_AA_CURRENT_SETTING_3_05MA
#define D_INACTIVE_CURRENT_SETTING LIN_AA_CURRENT_SETTING_0_00MA

#define LIN_AA_CFG_TBIT(step_bit, position) \
    ((((uint32_t)(step_bit) << 1) | (uint32_t)(position)) & 0x3Fu)

#define LIN_AA_CFG_CTRL_BUILD(tbit_meas, tbit_isrc, isrc_conf, isrc_en, pu_disc, ph_start, ph_stop, fsm_rst, meas_num, isense_en, err_clear, aa_en, aa_ph) \
    ((((uint32_t)(tbit_meas) & 0x3Fu) << 0) | \
     (((uint32_t)(tbit_isrc) & 0x3Fu) << 6) | \
     (((uint32_t)(isrc_conf) & 0x7u) << 12) | \
     (((uint32_t)(isrc_en) & 0x1u) << 15) | \
     (((uint32_t)(pu_disc) & 0x1u) << 16) | \
     (((uint32_t)(ph_start) & 0x1u) << 17) | \
     (((uint32_t)(ph_stop) & 0x1u) << 18) | \
     (((uint32_t)(fsm_rst) & 0x1u) << 19) | \
     (((uint32_t)(meas_num) & 0x3u) << 20) | \
     (((uint32_t)(isense_en) & 0x1u) << 22) | \
     (((uint32_t)(err_clear) & 0x1u) << 23) | \
     (((uint32_t)(aa_en) & 0x1u) << 24) | \
     (((uint32_t)(aa_ph) & 0x3u) << 25))

#if 0
#define D_PRE_DIFF_THRESHOLD_POS    (30)
#define D_SEL_DIFF_THRESHOLD_POS    (60)
//#define D_PRE_DIFF_THRESHOLD_NEG    (-30)
//#define D_SEL_DIFF_THRESHOLD_NEG    (-60)
#else
int16_t D_PRE_DIFF_THRESHOLD_POS = 20;
int16_t D_SEL_DIFF_THRESHOLD_POS = 11;
//int16_t D_PRE_DIFF_THRESHOLD_NEG = -100;
//int16_t D_SEL_DIFF_THRESHOLD_NEG = -100;
#endif

//#define D_FALLINGEDGE_DETECT_GAP    6

/**@brief Variant controls state Machine.*/
 volatile E_StateMachineStatus currentMachineStatus = E_StateMachineStatusIdle;
volatile uint32_t a = 0;
typedef enum AAMeasureStatus
{
    E_AAMeasureStatusIdle           = 0x0,
    //E_AAMeasureStatusCloseAll       = 0x0,
    E_AAMeasureStatusMeasureOffset  = 0x1,
    E_AAMeasureStatusMeasureOffset2 = 0x2,
    E_AAMeasureStatusMeasureC1      = 0x3,
    E_AAMeasureStatusMeasureC1_2    = 0x4,
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
static volatile int16_t linAaFirstSampleDiff = 0;

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
#ifndef LIN_AA_DEBUG_IO_ENABLE
#define LIN_AA_DEBUG_IO_ENABLE         (1u)
#endif

#if (LIN_AA_DEBUG_IO_ENABLE != 0u)
#define LIN_AA_DEBUG_IO_LOW()          do { GPIO->OUT.bit.P0 = 0; } while (0)
#define LIN_AA_DEBUG_IO_HIGH()         do { GPIO->OUT.bit.P0 = 1; } while (0)
#else
#define LIN_AA_DEBUG_IO_LOW()          do { } while (0)
#define LIN_AA_DEBUG_IO_HIGH()         do { } while (0)
#endif
#define LIN_AA_T2_CON_EDGE_CFG         (LIN_T2_CON_CP_RL2_Msk | LIN_T2_CON_EXEN2_Msk)
#define LIN_AA_T2_MOD_FALLING_EDGE_CFG (LIN_T2_MOD_PREN_Msk)
#define LIN_AA_T2_MOD_RISING_EDGE_CFG  (LIN_T2_MOD_PREN_Msk | LIN_T2_MOD_EDGESEL_Msk)
#define LIN_AA_SET_FALLING_EDGE_FAST() do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_FALLING_EDGE_CFG; waittingForFallingEdge = 1; } while (0)
#define LIN_AA_SET_RISING_EDGE_FAST()  do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_RISING_EDGE_CFG; waittingForFallingEdge = 0; } while (0)
#define LIN_AA_SET_RISING_EDGE_ARMED_FAST() do { LIN->T2_MOD.reg = LIN_AA_T2_MOD_RISING_EDGE_CFG; waittingForFallingEdge = 0; } while (0)
#define LIN_AA_ADC_READY_FAST()        ((ADC11->RESULT.reg & ADC11_RESULT_ADC_READY_Msk) != 0u)
#define LIN_AA_ADC_OVERS_SHIFT         (20u)
#define LIN_AA_ADC_OVERS_MASK          (0x3u << LIN_AA_ADC_OVERS_SHIFT)
#define LIN_AA_ADC_OVERS_FIRST         (1u)
#define LIN_AA_ADC_OVERS_SECOND        (2u)
#define LIN_AA_ADC_CFG0_WITH_OVERS(overs) (((uint32_t)ADC11_CFG_0 & ~LIN_AA_ADC_OVERS_MASK) | ((((uint32_t)(overs)) & 0x3u) << LIN_AA_ADC_OVERS_SHIFT))
#define LIN_AA_ADC_REINIT_FAST(overs)  do { ADC11->CFG_0.reg = LIN_AA_ADC_CFG0_WITH_OVERS(overs); ADC11->IEN.reg = (uint32_t)ADC11_IEN; ADC11->CFG_1.reg = (uint32_t)ADC11_CFG_1; } while (0)
#define LIN_AA_PREPARE_ADC_FAST(overs) do { while (!LIN_AA_ADC_READY_FAST()); LIN_AA_ADC_REINIT_FAST(overs); } while (0)
#define LIN_AA_PREPARE_ENTRY_FAST()    do { LIN_AA_PREPARE_ADC_FAST(LIN_AA_ADC_OVERS_FIRST); LIN->AA_I_DIFF_TH.reg = LIN_AutoAddr_BuildThresholdReg(); } while (0)
#define LIN_AA_SET_ADC_OVERS_FAST(overs) do { ADC11->CFG_0.bit.ADC_OVERS_CFG = (overs); } while (0)
#define LIN_AA_MEASURE_RES_LIN_S_FAST() ((int16_t)(LIN->AA_ACC_RES_1.reg & LIN_AA_ACC_RES_1_ACC_RES_1_Msk))
#define LIN_AA_MEASURE_RES_LIN_M_FAST() ((int16_t)(LIN->AA_ACC_RES_2.reg & LIN_AA_ACC_RES_2_ACC_RES_2_Msk))
#define LIN_AA_START_PHASE_FAST(cfg, start) do { LIN->AA_CTRL.reg = (cfg); LIN->AA_CTRL.reg = (start); } while (0)
#define LIN_AA_MEAS_IEN_MASK          (LIN_LIN_IEN_AA_MEAS_RDY_IEN_Msk | LIN_LIN_IEN_AA_ERR_IEN_Msk)
#define LIN_AA_CAPTURE_WAIT_IEN_FAST() do { linAaWaitIen = (LIN->LIN_IEN.reg & ~LIN_AA_MEAS_IEN_MASK); linAaMeasIen = (linAaWaitIen | LIN_AA_MEAS_IEN_MASK); } while (0)
#define LIN_AA_ENABLE_MEAS_IEN_FAST() do { LIN->LIN_IEN.reg = linAaMeasIen; } while (0)
#define LIN_AA_DISABLE_MEAS_IEN_FAST() do { LIN->LIN_IEN.reg = linAaWaitIen; } while (0)
#define LIN_AA_PRELOAD_FSM_RESET_FAST() do { LIN->AA_CTRL.reg = AA_CTRL_FSM_RESET_ONLY; } while (0)
/* AA_CTRL register values; *_START includes PH_START and is written as the final trigger. */
#define AA_CTRL_FSM_RESET_ONLY         (0x00080000u)
#define AA_CTRL_OFFSET_PHASE_ACTIVE    LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 1u)
#define AA_CTRL_OFFSET_PHASE_INACTIVE  LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 0u, 1u, 1u, 1u)
#define AA_CTRL_PRE_PHASE_ACTIVE       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 2u)
#define AA_CTRL_SEL_PHASE_ACTIVE       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)
#define AA_CTRL_SEL_PHASE_INACTIVE     LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)
#define AA_CTRL_END_PHASE              (0x00800000u)
#define AA_CTRL_OFFSET_PHASE_ACTIVE_START    LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 1u)
#define AA_CTRL_OFFSET_PHASE_INACTIVE_START  LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 0u, 0u, 1u, 1u)
#define AA_CTRL_PRE_PHASE_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 2u)
#define AA_CTRL_SEL_PHASE_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)
#define AA_CTRL_SEL_PHASE_INACTIVE_START     LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)
static inline uint32_t LIN_AutoAddr_BuildThresholdReg(void)
{
    uint32_t thresholdReg = 0u;
    thresholdReg |= (((uint32_t)D_PRE_DIFF_THRESHOLD_POS << LIN_AA_I_DIFF_TH_I_DIFF_TH_1_VAL_Pos) &
                     LIN_AA_I_DIFF_TH_I_DIFF_TH_1_VAL_Msk);
    thresholdReg |= (((uint32_t)D_SEL_DIFF_THRESHOLD_POS << LIN_AA_I_DIFF_TH_I_DIFF_TH_2_VAL_Pos) &
                     LIN_AA_I_DIFF_TH_I_DIFF_TH_2_VAL_Msk);
    return thresholdReg;
}
static inline int16_t LIN_AutoAddr_CalcCompensatedDiff(int16_t calTempLinM, int16_t calTempLinS)
{
    int16_t compensatedDiff = calTempLinS - calTempLinM;
    compensatedDiff = compensatedDiff >> 3;
    compensatedDiff = ADC11_compensateLinAutoAddrDiff(compensatedDiff);
    return compensatedDiff;
}

static inline int16_t LIN_AutoAddr_AverageTwoSamples(int16_t firstSample, int16_t secondSample)
{
    return (int16_t)(((int32_t)firstSample + (int32_t)secondSample) / 2);
}

static inline void LIN_AutoAddr_RestartPhaseSecondSample(uint32_t aaCtrlReg, uint32_t aaCtrlStartReg)
{
    LIN->LIN_ISC.reg = LIN_LIN_ISC_AA_ERR_CLR_Msk;
    LIN_AA_SET_ADC_OVERS_FAST(LIN_AA_ADC_OVERS_SECOND);
    LIN_AA_START_PHASE_FAST(aaCtrlReg, aaCtrlStartReg);
}

static inline void LIN_AutoAddr_StartPhaseFirstSample(uint32_t aaCtrlReg, uint32_t aaCtrlStartReg)
{
    LIN->LIN_ISC.reg = LIN_LIN_ISC_AA_ERR_CLR_Msk;
    LIN_AA_SET_ADC_OVERS_FAST(LIN_AA_ADC_OVERS_FIRST);
    LIN_AA_START_PHASE_FAST(aaCtrlReg, aaCtrlStartReg);
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

    LIN_AA_DEBUG_IO_LOW();
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
            LIN_AA_SET_ADC_OVERS_FAST(LIN_AA_ADC_OVERS_FIRST);
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
            LIN_AA_SET_ADC_OVERS_FAST(LIN_AA_ADC_OVERS_FIRST);
            LIN_AA_START_PHASE_FAST(AA_CTRL_OFFSET_PHASE_INACTIVE, AA_CTRL_OFFSET_PHASE_INACTIVE_START);
        }
    }

    LIN_AA_DEBUG_IO_HIGH();
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
            LIN_AA_DEBUG_IO_LOW();
            currentMeaStatus = E_AAMeasureStatusMeasureOffset2;
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            linAaFirstSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            LIN_AutoAddr_RestartPhaseSecondSample(AA_CTRL_OFFSET_PHASE_ACTIVE, AA_CTRL_OFFSET_PHASE_ACTIVE_START);
            LIN_AA_DEBUG_IO_HIGH();
            a = 1;
        }
        break;

        case E_AAMeasureStatusMeasureOffset2:
        {
            int16_t secondSampleDiff = 0;

            LIN_AA_DEBUG_IO_LOW();
            currentMeaStatus = E_AAMeasureStatusMeasureC1;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            secondSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            measureData.ADCMeasDiffOff = LIN_AutoAddr_AverageTwoSamples(linAaFirstSampleDiff, secondSampleDiff);
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            LIN_AutoAddr_StartPhaseFirstSample(AA_CTRL_PRE_PHASE_ACTIVE, AA_CTRL_PRE_PHASE_ACTIVE_START);
            LIN_AA_DEBUG_IO_HIGH();
            a = 1;
        }
        break;

        case E_AAMeasureStatusMeasureC1:
        {
            LIN_AA_DEBUG_IO_LOW();
            currentMeaStatus = E_AAMeasureStatusMeasureC1_2;
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            linAaFirstSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            LIN_AutoAddr_RestartPhaseSecondSample(AA_CTRL_PRE_PHASE_ACTIVE, AA_CTRL_PRE_PHASE_ACTIVE_START);
            LIN_AA_DEBUG_IO_HIGH();
            a = 2;
        }
        break;

        case E_AAMeasureStatusMeasureC1_2:
        {
            int16_t secondSampleDiff = 0;

            LIN_AA_DEBUG_IO_LOW();
            uint32_t aaCtrlReg = AA_CTRL_SEL_PHASE_INACTIVE;
            uint32_t aaCtrlStartReg = AA_CTRL_SEL_PHASE_INACTIVE_START;
            
            currentMeaStatus = E_AAMeasureStatusMeasureC2_S1;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            debugLINM_PRE = calTempLinM;
            debugLINS_PRE = calTempLinS;
            secondSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            measureData.ADCMeasDiffPre = LIN_AutoAddr_AverageTwoSamples(linAaFirstSampleDiff, secondSampleDiff);
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
            LIN_AutoAddr_StartPhaseFirstSample(aaCtrlReg, aaCtrlStartReg);
            LIN_AA_DEBUG_IO_HIGH();
            a = 2;
        }
        break;

        case E_AAMeasureStatusMeasureC2_S1:
        {
            LIN_AA_DEBUG_IO_LOW();
            uint32_t aaCtrlReg = AA_CTRL_SEL_PHASE_INACTIVE;
            uint32_t aaCtrlStartReg = AA_CTRL_SEL_PHASE_INACTIVE_START;

            if (measureData.isPreNode == 1)
            {
                aaCtrlReg = AA_CTRL_SEL_PHASE_ACTIVE;
                aaCtrlStartReg = AA_CTRL_SEL_PHASE_ACTIVE_START;
            }

            currentMeaStatus = E_AAMeasureStatusMeasureC2_S2;
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            linAaFirstSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            LIN_AutoAddr_RestartPhaseSecondSample(aaCtrlReg, aaCtrlStartReg);
            LIN_AA_DEBUG_IO_HIGH();
            a = 3;
        }
        break;

        case E_AAMeasureStatusMeasureC2_S2:
        {
            int16_t secondSampleDiff = 0;

            LIN_AA_DEBUG_IO_LOW();
            currentMeaStatus = E_AAMeasureStatusEnd;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = LIN_AA_MEASURE_RES_LIN_M_FAST();
            calTempLinS = LIN_AA_MEASURE_RES_LIN_S_FAST();
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/

            debugLINM_SEL = calTempLinM;
            debugLINS_SEL = calTempLinS;
            secondSampleDiff = LIN_AutoAddr_CalcCompensatedDiff(calTempLinM, calTempLinS);
            measureData.ADCMeasDiffSel = LIN_AutoAddr_AverageTwoSamples(linAaFirstSampleDiff, secondSampleDiff);
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
//                  measureSuccessData.ADCMeasDiffOff = measureData.ADCMeasDiffOff;
//                  measureSuccessData.ADCMeasDiffPre = measureData.ADCMeasDiffPre;
//                  measureSuccessData.ADCMeasDiffSel = measureData.ADCMeasDiffSel;
//                  measureSuccessData.isLastNode = 0;
                }
            }
            else
            {
//              measureCounter = 0;
//              breakCounter = 0;
                while(1);
            }
            LIN_AA_DEBUG_IO_HIGH();            
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
