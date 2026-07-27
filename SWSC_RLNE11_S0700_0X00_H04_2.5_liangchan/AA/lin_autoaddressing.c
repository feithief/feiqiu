#include "lin_autoaddressing.h"
#include "lin_hal.h"
#include "adc11.h"
#include "ModuleClock.h"

#ifdef LIN_ENABLE_AA_BSM_B5

#define D_STEP1_CLOSE_ALL_BIT    0
#define D_STEP2_MEAS_OFFSET_BIT  1
#define D_STEP3_OPEN_C1_BIT      4
#define D_STEP4_MEAS_PRE_BIT     5
#define D_STEP5_OPEN_C2_BIT      8
#define D_STEP6_MEAS_SEL_S1_BIT  9
#define D_STEP6_MEAS_SEL_S2_BIT  10
#define D_STEP7_RESTORE_BIT      12

//#define D_STEP1_CLOSE_ALL_BIT    1
//#define D_STEP2_MEAS_OFFSET_BIT  2
//#define D_STEP3_OPEN_C1_BIT      5
//#define D_STEP4_MEAS_PRE_BIT     6
//#define D_STEP5_OPEN_C2_BIT      9
//#define D_STEP6_MEAS_SEL_BIT     10
//#define D_STEP7_RESTORE_BIT      13

#define D_ADC_MESURE_AT_MIDDLE          1
#define D_ADC_MEASURE_AT_BEGINING       0

#define D_ADC_MEASURE_POSITION_OFFSET   D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_PRE      D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_SEL      D_ADC_MESURE_AT_MIDDLE

#define D_PULLUP_CONFIG_AT_MIDDLE       1
#define D_PULLUP_CONFIG_AT_BEGINNING    0

#define D_PULLUP_CONFIG_POSITION  D_PULLUP_CONFIG_AT_BEGINNING

#define D_PRE_CURRENT_SETTING   LIN_AA_CURRENT_SETTING_0_45MA
#define D_SEL_CURRENT_SETTING   LIN_AA_CURRENT_SETTING_3_05MA

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
 //tick_t lastRisingEdgeTick = 0;

volatile uint32_t count_succ = 0, count_fail = 0;
uint32_t debug_count_enterloop = 0, debug_count_startaa = 0;
uint32_t debug_count_aa_err = 0, debug_count_brkerr = 0, debug_count_adcerr = 0, debug_err_unknow = 0, debug_statemachine_err = 0;
tick_t debugTickBegin = 0, debugTickEnd = 0;
uint16_t debugLINM_PRE = 0, debugLINM_SEL = 0, debugLINS_PRE = 0, debugLINS_SEL = 0;
int16_t maxOffset = 0, minOffset = 0;
int16_t maxPre = 0, minPre = 0;
int16_t maxSel = 0, minSel = 0;

static void enable_EXF2_Detect(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
		LIN->LIN_ISC.bit.EXF2CLR = 1u;
    LIN_TIMER_EXT_IR_EN = 1;
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}

static void disable_EXF2_Detect(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
		LIN->LIN_ISC.bit.EXF2CLR = 1u;
    LIN_TIMER_EXT_IR_EN = 0;
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
    LIN_TIMER_CAP_REL = 1;
    LIN_TIMER_EX_IN_SEL = 0;
    LIN_TIMER_EXT_EN = 1;

    LIN_TIMER_CAP_EDGE_SELECT = 0;
    LIN_TIMER_EXT_START = 0;
    waittingForFallingEdge = 1;
    enable_EXF2_Detect();
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
}

static void LIN_AutoAddr_SetRisingEdge(void)
{
    /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
    disable_EXF2_Detect();
    LIN_TIMER_CAP_REL = 1;
    LIN_TIMER_EX_IN_SEL = 0;
    LIN_TIMER_EXT_EN = 1;

    LIN_TIMER_CAP_EDGE_SELECT = 1;
    LIN_TIMER_EXT_START = 0;
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
//	AutoAdressFlg = 1;
	debug_count_enterloop++;
    //tick_t currentTick = moduleClockGet();

    //if (!(DTickAfter(currentTick, lastRisingEdgeTick + D_FALLINGEDGE_DETECT_GAP)))
    //{
    //    return;
    //}
    
    /*need aa operation while in these three status*/
    if ((currentMachineStatus & (E_StateMachineStatusWaitting|
                                E_StateMachineStatusAddressed)) != 0)
    {
        if (currentMeaStatus == E_AAMeasureStatusIdle)
        {
			debug_count_startaa++;

            /*close all pullup or current source*/
            if (currentMachineStatus != E_StateMachineStatusAddressed)
            {
                measureData.ADCMeasDiffOff = 0;
                measureData.ADCMeasDiffPre = 0;
                measureData.ADCMeasDiffSel = 0;
                measureData.isPreNode = 0;
                //measureData.isLastNode = 0;
            }
						
						LIN->LIN_ISC.reg = (LIN_LIN_ISC_AA_ERR_CLR_Msk | LIN_LIN_ISC_AA_MEAS_RDY_CLR_Msk);
						while (!ADC11->RESULT.bit.ADC_READY);
						moduleAdcInit();

            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            LIN_AutoAddr_SetRisingEdge();

						LDRV->MOD_CFG.bit.LD_ENABLE = 0u;
            LIN_AA_CTRL_LIN_AA_EN = 0;  /*disable LIN AA first*/

            LIN_AA_CTRL_FSM_RST = 1; /*force to reset firmware*/

            LIN_AA_MEAS_INT_EN = 1; /*enable measure adc interrupt*/
            LIN_AA_ERR_EN = 1; /*enable the error interrupt*/

            if (currentMachineStatus == E_StateMachineStatusAddressed)
            {
                LIN_AA_FW_CONTROL_ALRDY_ADDR = 1; /*inform firmware this node has already addressed*/
                currentMeaStatus = E_AAMeasureStatusEnd; /*jump to end*/
            }
            else
            {
                LIN_AA_FW_CONTROL_ALRDY_ADDR = 0; /*inform firmware this node need to perform aa*/
                LIN_AA_CTRL_TBIT_START_MEAS = (D_STEP2_MEAS_OFFSET_BIT<<1) | D_ADC_MEASURE_POSITION_OFFSET; /*offset step*/
                currentMeaStatus = E_AAMeasureStatusMeasureOffset;
            }
            
            LIN_AA_CTRL_TBIT_START_ISRC = (D_STEP1_CLOSE_ALL_BIT<<1) | D_PULLUP_CONFIG_POSITION; /*close all step*/

            LIN_AA_THRESHOLD_PRE_IDIFF = D_PRE_DIFF_THRESHOLD_POS; /*set pre threshold*/
            LIN_AA_THRESHOLD_SELECT_IDIFF = D_SEL_DIFF_THRESHOLD_POS; /*set selection threshold*/

            LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/
            LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/
						
            LIN_AA_CTRL_ISRC_CONF_CTRL = LIN_AA_CURRENT_SETTING_0_00MA; /*set to 0mA*/
            LIN_AA_CTRL_ISRC_EN = 1; /*disable current source*/
            LIN_AA_CTRL_PU_DISC = 1; /*disable pull-up*/
            LIN_AA_CTRL_ISENSE_MEAS_NUMBER = 3; /*measure 8 samples*/
            if (currentMachineStatus == E_StateMachineStatusAddressed)
            {
                LIN_AA_CTRL_ISENSE_EN = 0; /*measure enable*/
            }
            else
            {
                LIN_AA_CTRL_ISENSE_EN = 1; /*measure enable*/
            }
            LIN_AA_CTRL_LIN_AA_PH = 1; /*set firmware to offset phase*/
            LIN_AA_CTRL_LIN_AA_EN = 1; /*enable LIN aa*/
            LIN_AA_CTRL_PH_START = 1; /*start the offset phase*/
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
        }
        else
        {
            //do nothing...
        }
    }
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
    LDRV->MOD_CFG.bit.LD_ENABLE = 1u;
    LIN_AA_MEAS_INT_EN = 0; /*disable measure adc interrupt*/
    LIN_AA_ERR_EN = 0; /*disable the error interrupt*/

    LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/

    LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/
    LIN_AA_MEASURE_INT_CLR = 1;/*clear the measure interrupt*/

    LIN_AA_CTRL_ISRC_CONF_CTRL = LIN_AA_CURRENT_SETTING_0_00MA; /*set to 0mA*/
    LIN_AA_CTRL_ISRC_EN = 0; /*disable current source*/
    LIN_AA_CTRL_PU_DISC = 0; /*enable pull-up*/

    LIN_AA_CTRL_ISENSE_EN = 0; /*measure disable*/

    LIN_AA_CTRL_LIN_AA_EN = 0; /*disable LIN aa*/
    LIN_AA_CTRL_LIN_AA_PH = 0; /*set firmware to END phase*/
    LIN_AA_CTRL_PH_START = 0; /*start the phase*/

    LIN_AutoAddr_SetFallingEdge();
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
void LIN_AutoAddr_Edge_ISR(void)
{
    if (waittingForFallingEdge)//falling edge
    {
        LIN_AutoAddr_Meas_Entry();
				fallingEdgeCounter++;
    }
    else//rising edge
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
            currentMeaStatus = E_AAMeasureStatusMeasureC1;
					
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = (int16_t)LIN_AA_MEASURE_RES_LIN_M;
            calTempLinS = (int16_t)LIN_AA_MEASURE_RES_LIN_S;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
			
            //measureData.ADCMeasDiffOff = (calTempLinS>>3) - (calTempLinM>>3);
            measureData.ADCMeasDiffOff = calTempLinS - calTempLinM;

            measureData.ADCMeasDiffOff = measureData.ADCMeasDiffOff>>3;
            measureData.ADCMeasDiffOff = ADC11_compensateLinAutoAddrDiff(measureData.ADCMeasDiffOff);

            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/
            LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/

            LIN_AA_THRESHOLD_PRE_IDIFF = D_PRE_DIFF_THRESHOLD_POS; /*set pre threshold*/
            LIN_AA_THRESHOLD_SELECT_IDIFF = D_SEL_DIFF_THRESHOLD_POS; /*set selection threshold*/

            LIN_AA_CTRL_ISRC_CONF_CTRL = D_PRE_CURRENT_SETTING; /*set to 0.45mA*/
            LIN_AA_CTRL_ISRC_EN = 1; /*enable current source*/
            LIN_AA_CTRL_TBIT_START_MEAS = (D_STEP4_MEAS_PRE_BIT<<1) | D_ADC_MEASURE_POSITION_PRE; /*pre measure step*/
            LIN_AA_CTRL_TBIT_START_ISRC = (D_STEP3_OPEN_C1_BIT<<1) | D_PULLUP_CONFIG_POSITION; /*open C1 source step*/
            //LIN_AA_CTRL_ISRC_EN = 1; /*enable current source*/
            //LIN_AA_CTRL_PU_DISC = 1; /*disable pull-up*/
            LIN_AA_CTRL_ISENSE_MEAS_NUMBER = 3; /*measure 8 samples*/
            LIN_AA_CTRL_ISENSE_EN = 1; /*measure enable*/
            LIN_AA_CTRL_LIN_AA_PH = 2; /*set firmware to PRE phase*/
            LIN_AA_CTRL_PH_START = 1; /*start the phase*/
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
        }
        break;
        case E_AAMeasureStatusMeasureC1:
        {
//            currentMeaStatus = E_AAMeasureStatusMeasureC2_S1;
            currentMeaStatus = E_AAMeasureStatusMeasureC2_S2;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = (int16_t)LIN_AA_MEASURE_RES_LIN_M;
            calTempLinS = (int16_t)LIN_AA_MEASURE_RES_LIN_S;
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
                /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
                LIN_AA_CTRL_ISRC_CONF_CTRL = D_SEL_CURRENT_SETTING; /*set to 2.5mA*/
                LIN_AA_CTRL_ISRC_EN = 1; /*enable current source*/
                /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
                measureData.isPreNode = 1;
            }
            else
            {
                /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
                LIN_AA_CTRL_ISRC_CONF_CTRL = LIN_AA_CURRENT_SETTING_0_00MA; /*set to 0mA*/
                LIN_AA_CTRL_ISRC_EN = 1; /*disable current source*/
                /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
                measureData.isPreNode = 0;
            }
            
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
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
#if 0
				case E_AAMeasureStatusMeasureC2_S1:
				{
					currentMeaStatus = E_AAMeasureStatusMeasureC2_S2;
					/*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
					calTempLinM = (int16_t)LIN_AA_MEASURE_RES_LIN_M;
					calTempLinS = (int16_t)LIN_AA_MEASURE_RES_LIN_S;
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
            currentMeaStatus = E_AAMeasureStatusEnd;
            /*LIN REGISTER OPERATION, SHOULD RE-ORGNAIZE IN HAL LAYER*/
            calTempLinM = (int16_t)LIN_AA_MEASURE_RES_LIN_M;
            calTempLinS = (int16_t)LIN_AA_MEASURE_RES_LIN_S;
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
        }
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
		//LDRV->MOD_CFG.bit.LD_ENABLE = 0u;
    LIN_AutoAddr_SetFallingEdge();
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
    LIN_AA_CTRL_LIN_AA_EN = 0;  /*disable LIN AA first*/

    LIN_AA_MEAS_INT_EN = 0; /*enable measure adc interrupt*/
		
    LIN_AA_CTRL_ERR_CODE_CLR = 1; /*clear error code*/
    LIN_AA_ERR_INT_CLR = 1; /*clear error interrupt*/
    LIN_AA_MEASURE_INT_CLR = 1;/*clear the measure interrupt*/


    LIN_AA_CTRL_ISRC_CONF_CTRL = LIN_AA_CURRENT_SETTING_0_00MA; /*set to 0mA*/
    LIN_AA_CTRL_ISRC_EN = 0; /*disable current source*/
    LIN_AA_CTRL_PU_DISC = 0; /*enable pull-up*/
    LIN_AA_CTRL_ISENSE_EN = 0; /*measure disable*/
    LIN_AA_CTRL_LIN_AA_EN = 0; /*disable LIN aa*/
    disable_EXF2_Detect();
		
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
