#include "lin_autoaddressing_cfg.h"

/*
 * External auto-addressing configuration instance.
 *
 * Usage flow:
 * 1. Macros in lin_autoaddressing_cfg.h are expanded into g_linAutoAddrConfig.
 * 2. AA/lin_autoaddressing.c reads the values through g_linAutoAddrConfig.
 * 3. The values are written to hardware registers by whole-register writes such as xxx->REG.reg = value.
 *
 * Comment format:
 * - Used in: function and switch case, if applicable.
 * - Assignment: whether the value is finally used as a whole-register write or only stored for traceability.
 * - Register fields: affected register fields and bit positions.
 * - AA stage: split into numbered steps.
 * - Meaning: why the setting exists in the AA flow.
 */
const LinAutoAddr_Config_t g_linAutoAddrConfig =
{
    /*
     * Macro: LIN_AA_CFG_T2_CON_EDGE
     * Used in:
     * - LIN_AutoAddr_SetFallingEdge(): During AA start or re-arm after one window exits, configures T2 to wait for the break falling edge.
     * - LIN_AutoAddr_SetRisingEdge(): During the AA measurement window, configures T2 to wait for the break rising edge.
     * Assignment: whole-register write, LIN->T2_CON.reg = t2ConEdge.
     * Register fields:
     * - CP_RL2[0]=1: Capture-mode select; lets Timer2 latch the selected LIN edge time.
     * - EXEN2[3]=1: External event capture enable for the selected T2EX LIN edge.
     * - C_T2[1]=0: Timer mode select; Timer2 uses the internal timer clock instead of external counter input.
     * - TR2[2]=0: Timer2 run-control bit; this write does not start Timer2.
     * - T2INSEL[9:8]=0: Timer2 input-source select; kept at the default source by this full-register write.
     * - T2EXINSEL[11:10]=0: Timer2 external capture-source select; kept at the default source by this full-register write.
     * AA stage 1: LIN_AutoAddr_Start() arms T2 before waiting for the first break falling edge.
     * AA stage 2: LIN_AutoAddr_Meas_Entry() keeps T2 configured when the break edge enters the AA measurement window.
     * AA stage 3: LIN_AutoAddr_Meas_Exit() re-arms T2 for the next AA measurement window.
     * Meaning: enables Timer2 capture on LIN edges during auto-addressing.
     */
    .t2ConEdge = LIN_AA_CFG_T2_CON_EDGE,

    /*
     * Macro: LIN_AA_CFG_T2_MOD_FALLING_EDGE
     * Used in:
     * - LIN_AutoAddr_SetFallingEdge(): During AA start or re-arm after one window exits, configures T2 to wait for the break falling edge.
     * - LIN_AutoAddr_Start(): AA start phase; prepares T2, ADC, thresholds, and the AA FSM.
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * Assignment: whole-register write, LIN->T2_MOD.reg = t2ModFallingEdge.
     * Register fields:
     * - PREN[4]=1 prescaler enabled: Timer2 prescaler enable for AA edge timing.
     * - EDGESEL[5]=0 falling edge selected: T2EX edge select; chooses falling-edge wait or rising-edge wait.
     * - DCEN[0]=0: Up/down counter mode control; disabled here.
     * - T2PRE[3:1]=0: Timer2 prescaler selection; written as 0 for the current default encoding.
     * - T2RHEN[6]=0: Timer2 reload-high extension control; disabled here.
     * - T2REGS[7]=0: Timer2 register-set select; default register set is used here.
     * - Full-register note: fields not listed above are written according to the constant value by the whole-register write.
     * AA stage 1: before AA starts, wait for break falling edge.
     * AA stage 2: after one measurement exits, wait for the next break falling edge.
     * AA stage 3: falling edge calls LIN_AutoAddr_Meas_Entry() and starts the AA measurement window.
     * Meaning: detects the beginning of one auto-addressing measurement window.
     */
    .t2ModFallingEdge = LIN_AA_CFG_T2_MOD_FALLING_EDGE,

    /*
     * Macro: LIN_AA_CFG_T2_MOD_RISING_EDGE
     * Used in:
     * - LIN_AutoAddr_Meas_Entry(): Break falling-edge entry phase; starts OFFSET active/inactive according to node state.
     * - LIN_AA_SET_RISING_EDGE_ARMED_FAST(): Fast entry path that switches T2 from falling-edge wait to rising-edge wait.
     * - LIN_AutoAddr_SetRisingEdge(): During the AA measurement window, configures T2 to wait for the break rising edge.
     * - State branch E_StateMachineStatusWaitting: this node state selects the setting.
     * Assignment: whole-register write, LIN->T2_MOD.reg = t2ModRisingEdge.
     * Register fields:
     * - PREN[4]=1 prescaler enabled: Timer2 prescaler enable for AA edge timing.
     * - EDGESEL[5]=1 rising edge selected: T2EX edge select; chooses falling-edge wait or rising-edge wait.
     * - DCEN[0]=0: Up/down counter mode control; disabled here.
     * - T2PRE[3:1]=0: Timer2 prescaler selection; written as 0 for the current default encoding.
     * - T2RHEN[6]=0: Timer2 reload-high extension control; disabled here.
     * - T2REGS[7]=0: Timer2 register-set select; default register set is used here.
     * - Full-register note: fields not listed above are written according to the constant value by the whole-register write.
     * AA stage 1: break falling edge has been detected.
     * AA stage 2: AA hardware phase is started and rising edge detection is armed.
     * AA stage 3: break rising edge exits the current AA measurement window through LIN_AutoAddr_RisingEdge_ISR().
     * Meaning: detects the end of one auto-addressing measurement window.
     */
    .t2ModRisingEdge = LIN_AA_CFG_T2_MOD_RISING_EDGE,

    /*
     * Macro: LIN_AA_CFG_ADC_CFG0_REINIT
     * Used in:
     * - LIN_AA_ADC_REINIT_FAST(): Fast path that restores ADC11 configuration registers before AA measurement.
     * - LIN_AA_PREPARE_ENTRY_FAST(): Before AA entry, waits for ADC ready, restores ADC, and loads thresholds.
     * - LIN_AutoAddr_Start(): AA start phase; prepares T2, ADC, thresholds, and the AA FSM.
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * Assignment: whole-register write, ADC11->CFG_0.reg = adcCfg0Reinit.
     * Register fields:
     * - ADC_COMP_VAL[12:0]=4: ADC comparator reference/baseline value.
     * - ADC_OVERS_CFG[21:20]=3 for 8 samples: ADC oversampling configuration, controls samples per measurement.
     * - ADC_SEL_4V[16]=0: ADC reference/input selection; default path is kept here.
     * - ADC_STC[23:22]=0: ADC sample-time control; restored to default here.
     * - DTS_OVT_EN[28]=0 and other bits are written as 0 by this full-register write: DTS over-temperature path enable; disabled here.
     * AA stage 1: before AA starts, restore ADC configuration.
     * AA stage 2: after one AA measurement exits, restore ADC again before the next measurement.
     * AA stage 3: AA current measurement uses the restored sampling configuration.
     * Meaning: keeps ADC oversampling and comparator baseline deterministic for AA current measurement.
     */
    .adcCfg0Reinit = LIN_AA_CFG_ADC_CFG0_REINIT,

    /*
     * Macro: LIN_AA_CFG_ADC_IEN_REINIT
     * Used in:
     * - LIN_AA_ADC_REINIT_FAST(): Fast path that restores ADC11 configuration registers before AA measurement.
     * - LIN_AA_PREPARE_ENTRY_FAST(): Before AA entry, waits for ADC ready, restores ADC, and loads thresholds.
     * - LIN_AutoAddr_Start(): AA start phase; prepares T2, ADC, thresholds, and the AA FSM.
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * Assignment: whole-register write, ADC11->IEN.reg = adcIenReinit.
     * Register fields:
     * - ADC_FW_EOC[0]=1 end-of-conversion interrupt enabled: ADC end-of-conversion interrupt enable for firmware result handling.
     * - DTS_OVT[8]=0 and other bits are written as 0 by this full-register write: DTS over-temperature interrupt enable; disabled here.
     * AA stage 1: before AA starts, enable ADC interrupt path.
     * AA stage 2: after one AA measurement exits, re-enable ADC interrupt path.
     * AA stage 3: ADC completion can trigger the AA measurement handling flow.
     * Meaning: makes the ADC measurement completion visible to firmware during AA.
     */
    .adcIenReinit = LIN_AA_CFG_ADC_IEN_REINIT,

    /*
     * Macro: LIN_AA_CFG_ADC_CFG1_REINIT
     * Used in:
     * - LIN_AA_ADC_REINIT_FAST(): Fast path that restores ADC11 configuration registers before AA measurement.
     * - LIN_AA_PREPARE_ENTRY_FAST(): Before AA entry, waits for ADC ready, restores ADC, and loads thresholds.
     * - LIN_AutoAddr_Start(): AA start phase; prepares T2, ADC, thresholds, and the AA FSM.
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * Assignment: whole-register write, ADC11->CFG_1.reg = adcCfg1Reinit.
     * Register fields:
     * - ADC_ENABLE[8]=1: ADC module enable bit.
     * - ADC_SOC[0]=0: ADC software conversion trigger; no new conversion is triggered here.
     * - ADC_CHNR[5:1]=0: ADC channel select; restored to default channel encoding here.
     * - ADC_COMP_EN[9]=0 and other bits are written as 0 by this full-register write: ADC comparator path enable; disabled here.
     * AA stage 1: before AA starts, enable ADC module.
     * AA stage 2: after one AA measurement exits, restore ADC module state.
     * AA stage 3: AA current sensing can use ADC hardware.
     * Meaning: makes sure ADC is enabled before AA current measurement.
     */
    .adcCfg1Reinit = LIN_AA_CFG_ADC_CFG1_REINIT,

    /*
     * Macro: LIN_AA_CFG_CTRL_FSM_RESET_ONLY
     * Used in:
     * - LIN_AA_PRELOAD_FSM_RESET_FAST(): Before a phase starts or after one exits, quickly resets the AA hardware FSM.
     * - LIN_AutoAddr_Start(): AA start phase; prepares T2, ADC, thresholds, and the AA FSM.
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlFsmResetOnly.
     * Register fields:
     * - FSM_RST[19]=1: AA hardware state-machine reset bit.
     * - all other AA_CTRL fields are written as 0 by this full-register write.
     * AA stage 1: reset AA FSM before entering auto-addressing.
     * AA stage 2: reset AA FSM after one measurement window exits.
     * AA stage 3: next AA phase starts from a known hardware state.
     * Meaning: clears the AA hardware state machine without starting a phase.
     */
    .ctrlFsmResetOnly = LIN_AA_CFG_CTRL_FSM_RESET_ONLY,

    /*
     * Macro: LIN_AA_CFG_CTRL_END_PHASE
     * Used in:
     * - LIN_AutoAddr_Meas_Exit(): Measurement-window exit phase; closes the current phase and prepares the next window.
     * - LIN_AutoAddr_End(): AA end phase; closes and cleans up the AA hardware phase.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlEndPhase.
     * Register fields:
     * - ERR_CODE_CLR[23]=1: AA error-code clear bit.
     * - all other AA_CTRL fields are written as 0 by this full-register write.
     * AA stage 1: rising edge or error exits the current AA measurement window.
     * AA stage 2: active AA phase is closed and AA error state is cleared.
     * AA stage 3: firmware either prepares the next window or fully ends AA.
     * Meaning: closes the current AA phase and clears AA error status.
     */
    .ctrlEndPhase = LIN_AA_CFG_CTRL_END_PHASE,

    /*
     * Macro: LIN_AA_CFG_CTRL_OFFSET_ACTIVE
     * Used in:
     * - LIN_AutoAddr_Meas_Entry(): Break falling-edge entry phase; starts OFFSET active/inactive according to node state.
     * - State branch E_StateMachineStatusWaitting: this node state selects the setting.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlOffsetActive, then ctrlOffsetActiveStart is written as the final trigger.
     * Register fields:
     * - TBIT_START_MEAS[5:0]: LIN bit-count position where AA current sampling starts.
     * - TBIT_START_ISRC[11:6]: LIN bit-count position where AA current-source/pull-up action starts.
     * - ISRC_CONF_CTRL[14:12]=0mA: AA current-source amplitude configuration.
     * - ISRC_EN[15]=1: AA current-source control enable.
     * - PU_DISC[16]=1: Pull-up disconnect control bit.
     * - PH_START[17]=0: AA phase start trigger bit; *_START macros write this last.
     * - ISENSE_MEAS_NUMBER[21:20]=3 for 8 measures: Current-sense sample-count configuration.
     * - ISENSE_EN[22]=1: AA current-sense enable.
     * - ERR_CODE_CLR[23]=1: AA error-code clear bit.
     * - LIN_AA_EN[24]=1: LIN auto-addressing hardware enable.
     * - LIN_AA_PH[26:25]=1 OFFSET: AA phase select for OFFSET/PRESEL/SEL.
     * AA stage 1: break falling edge enters LIN_AutoAddr_Meas_Entry().
     * AA stage 2: OFFSET active phase configuration is preloaded without PH_START.
     * AA stage 3: ctrlOffsetActiveStart starts the hardware phase.
     * Meaning: prepares offset current measurement for a node that is still waiting for address assignment.
     */
    .ctrlOffsetActive = LIN_AA_CFG_CTRL_OFFSET_ACTIVE,

    /*
     * Macro: LIN_AA_CFG_CTRL_OFFSET_INACTIVE
     * Used in:
     * - LIN_AutoAddr_Meas_Entry(): Break falling-edge entry phase; starts OFFSET active/inactive according to node state.
     * - State branch E_StateMachineStatusAddressed: this node state selects the setting.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlOffsetInactive, then ctrlOffsetInactiveStart is written as the final trigger.
     * Register fields:
     * - ISRC_CONF_CTRL[14:12]=0mA: AA current-source amplitude configuration.
     * - ISRC_EN[15]=1: AA current-source control enable.
     * - PU_DISC[16]=1: Pull-up disconnect control bit.
     * - PH_START[17]=0: AA phase start trigger bit; *_START macros write this last.
     * - ISENSE_MEAS_NUMBER[21:20]=3: Current-sense sample-count configuration.
     * - ISENSE_EN[22]=0: AA current-sense enable.
     * - ERR_CODE_CLR[23]=1: AA error-code clear bit.
     * - LIN_AA_EN[24]=1: LIN auto-addressing hardware enable.
     * - LIN_AA_PH[26:25]=1 OFFSET: AA phase select for OFFSET/PRESEL/SEL.
     * - timing fields are 0.
     * AA stage 1: break falling edge is seen on a node that is already addressed.
     * AA stage 2: OFFSET inactive configuration is preloaded.
     * AA stage 3: ctrlOffsetInactiveStart starts an inactive hardware phase.
     * Meaning: keeps already-addressed nodes synchronized with the AA flow without active offset measurement.
     */
    .ctrlOffsetInactive = LIN_AA_CFG_CTRL_OFFSET_INACTIVE,

    /*
     * Macro: LIN_AA_CFG_CTRL_PRE_ACTIVE
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureOffset: the setting is used in this measurement-state branch.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlPreActive, then ctrlPreActiveStart is written as the final trigger.
     * Register fields:
     * - TBIT_START_MEAS[5:0]: LIN bit-count position where AA current sampling starts.
     * - TBIT_START_ISRC[11:6]: LIN bit-count position where AA current-source/pull-up action starts.
     * - ISRC_CONF_CTRL[14:12]=0.45mA: AA current-source amplitude configuration.
     * - ISRC_EN[15]=1: AA current-source control enable.
     * - PU_DISC[16]=1: Pull-up disconnect control bit.
     * - PH_START[17]=0: AA phase start trigger bit; *_START macros write this last.
     * - ISENSE_MEAS_NUMBER[21:20]=3: Current-sense sample-count configuration.
     * - ISENSE_EN[22]=1: AA current-sense enable.
     * - ERR_CODE_CLR[23]=1: AA error-code clear bit.
     * - LIN_AA_EN[24]=1: LIN auto-addressing hardware enable.
     * - LIN_AA_PH[26:25]=2 PRESEL: AA phase select for OFFSET/PRESEL/SEL.
     * AA stage 1: OFFSET measurement result is read in case E_AAMeasureStatusMeasureOffset.
     * AA stage 2: PRESEL active phase configuration is preloaded.
     * AA stage 3: ctrlPreActiveStart starts PRESEL measurement.
     * Meaning: measures the pre-selection current difference after offset compensation.
     */
    .ctrlPreActive = LIN_AA_CFG_CTRL_PRE_ACTIVE,

    /*
     * Macro: LIN_AA_CFG_CTRL_SEL_ACTIVE
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureC1: the setting is used in this measurement-state branch.
     * - Condition: selected when measureData.
     * Assignment: whole-register write through local aaCtrlReg, LIN->AA_CTRL.reg = ctrlSelActive, then ctrlSelActiveStart is written as the final trigger.
     * Register fields:
     * - TBIT_START_MEAS[5:0]: LIN bit-count position where AA current sampling starts.
     * - TBIT_START_ISRC[11:6]: LIN bit-count position where AA current-source/pull-up action starts.
     * - ISRC_CONF_CTRL[14:12]=3.05mA: AA current-source amplitude configuration.
     * - ISRC_EN[15]=1: AA current-source control enable.
     * - PU_DISC[16]=1: Pull-up disconnect control bit.
     * - PH_START[17]=0: AA phase start trigger bit; *_START macros write this last.
     * - ISENSE_MEAS_NUMBER[21:20]=3: Current-sense sample-count configuration.
     * - ISENSE_EN[22]=1: AA current-sense enable.
     * - ERR_CODE_CLR[23]=1: AA error-code clear bit.
     * - LIN_AA_EN[24]=1: LIN auto-addressing hardware enable.
     * - LIN_AA_PH[26:25]=3 SEL: AA phase select for OFFSET/PRESEL/SEL.
     * AA stage 1: PRESEL result is evaluated in case E_AAMeasureStatusMeasureC1.
     * AA stage 2: if PRE threshold is met, SEL active configuration is selected.
     * AA stage 3: ctrlSelActiveStart starts SEL measurement for a candidate node.
     * Meaning: performs active SEL measurement to decide whether this is the last node.
     */
    .ctrlSelActive = LIN_AA_CFG_CTRL_SEL_ACTIVE,

    /*
     * Macro: LIN_AA_CFG_CTRL_SEL_INACTIVE
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureC1: the setting is used in this measurement-state branch.
     * Assignment: whole-register write through local aaCtrlReg, LIN->AA_CTRL.reg = ctrlSelInactive, then ctrlSelInactiveStart is written as the final trigger.
     * Register fields:
     * - TBIT_START_MEAS[5:0]: same as SEL active, keeps SEL sample timing aligned.
     * - TBIT_START_ISRC[11:6]: same as SEL active, keeps SEL current-source timing aligned.
     * - LIN_AA_PH[26:25]=3: selects SEL phase.
     * - ISRC_CONF_CTRL[14:12]=0mA: inactive node does not drive SEL current.
     * - PH_START[17]=0: preload value does not start the phase; the matching START value triggers it later.
     * AA stage 1: PRESEL result is evaluated in case E_AAMeasureStatusMeasureC1.
     * AA stage 2: if PRE threshold is not met, SEL inactive configuration is kept.
     * AA stage 3: ctrlSelInactiveStart starts inactive SEL flow.
     * Meaning: keeps SEL timing aligned while preventing non-candidate nodes from actively driving SEL current.
     */
    .ctrlSelInactive = LIN_AA_CFG_CTRL_SEL_INACTIVE,

    /*
     * Macro: LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START
     * Used in:
     * - LIN_AutoAddr_Meas_Entry(): Break falling-edge entry phase; starts OFFSET active/inactive according to node state.
     * - State branch E_StateMachineStatusWaitting: this node state selects the setting.
     * - Write order: written immediately after ctrlOffsetActive as the final trigger.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlOffsetActiveStart.
     * Register fields:
     * - PH_START[17]=1: AA phase start trigger bit; *_START macros write this last.
     * - ERR_CODE_CLR[23]=0: AA error-code clear bit.
     * AA stage 1: OFFSET active preload has already been written.
     * AA stage 2: this value is written last.
     * AA stage 3: AA hardware FSM starts OFFSET active phase.
     * Meaning: final trigger for OFFSET active phase.
     */
    .ctrlOffsetActiveStart = LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START,

    /*
     * Macro: LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START
     * Used in:
     * - LIN_AutoAddr_Meas_Entry(): Break falling-edge entry phase; starts OFFSET active/inactive according to node state.
     * - State branch E_StateMachineStatusAddressed: this node state selects the setting.
     * - Write order: written immediately after ctrlOffsetInactive as the final trigger.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlOffsetInactiveStart.
     * Register fields:
     * - PH_START[17]=1: AA phase start trigger bit; *_START macros write this last.
     * - ERR_CODE_CLR[23]=0: AA error-code clear bit.
     * AA stage 1: OFFSET inactive preload has already been written.
     * AA stage 2: this value is written last.
     * AA stage 3: AA hardware FSM starts OFFSET inactive phase.
     * Meaning: final trigger for already-addressed node inactive flow.
     */
    .ctrlOffsetInactiveStart = LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START,

    /*
     * Macro: LIN_AA_CFG_CTRL_PRE_ACTIVE_START
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureOffset: the setting is used in this measurement-state branch.
     * - Write order: written immediately after ctrlPreActive as the final trigger.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlPreActiveStart.
     * Register fields:
     * - PH_START[17]=1: AA phase start trigger bit; *_START macros write this last.
     * - ERR_CODE_CLR[23]=0: AA error-code clear bit.
     * AA stage 1: PRESEL active preload has already been written.
     * AA stage 2: this value is written last inside case E_AAMeasureStatusMeasureOffset.
     * AA stage 3: AA hardware FSM starts PRESEL active phase.
     * Meaning: final trigger for PRESEL phase.
     */
    .ctrlPreActiveStart = LIN_AA_CFG_CTRL_PRE_ACTIVE_START,

    /*
     * Macro: LIN_AA_CFG_CTRL_SEL_ACTIVE_START
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureC1: the setting is used in this measurement-state branch.
     * - Write order: written immediately after ctrlSelActive when PRE threshold is met as the final trigger.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlSelActiveStart.
     * Register fields:
     * - PH_START[17]=1: AA phase start trigger bit; *_START macros write this last.
     * - ERR_CODE_CLR[23]=0: AA error-code clear bit.
     * AA stage 1: SEL active preload has already been selected.
     * AA stage 2: this value is written last inside case E_AAMeasureStatusMeasureC1.
     * AA stage 3: AA hardware FSM starts SEL active phase.
     * Meaning: final trigger for candidate-node SEL measurement.
     */
    .ctrlSelActiveStart = LIN_AA_CFG_CTRL_SEL_ACTIVE_START,

    /*
     * Macro: LIN_AA_CFG_CTRL_SEL_INACTIVE_START
     * Used in:
     * - LIN_AutoAddr_Measure_ISR(): AA measurement-ready ISR; advances OFFSET, PRESEL, and SEL cases.
     * - switch case E_AAMeasureStatusMeasureC1: the setting is used in this measurement-state branch.
     * - Write order: written immediately after ctrlSelInactive when PRE threshold is not met as the final trigger.
     * Assignment: whole-register write, LIN->AA_CTRL.reg = ctrlSelInactiveStart.
     * Register fields:
     * - PH_START[17]=1: AA phase start trigger bit; *_START macros write this last.
     * - ERR_CODE_CLR[23]=0: AA error-code clear bit.
     * AA stage 1: SEL inactive preload has already been selected.
     * AA stage 2: this value is written last inside case E_AAMeasureStatusMeasureC1.
     * AA stage 3: AA hardware FSM starts SEL inactive phase.
     * Meaning: final trigger for non-candidate-node inactive SEL flow.
     */
    .ctrlSelInactiveStart = LIN_AA_CFG_CTRL_SEL_INACTIVE_START,

    /*
     * Macro: LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS
     * Used in:
     * - LIN_AutoAddr_BuildThresholdReg(): builds thresholdReg and places this value into I_DIFF_TH_1_VAL[11:0].
     * - LIN_AutoAddr_Start(): indirectly loads the PRE threshold before AA starts.
     * - LIN_AutoAddr_Meas_Exit(): indirectly reloads the PRE threshold after one window exits.
     * - LIN_AutoAddr_Measure_ISR(): uses this threshold to evaluate the PRESEL result.
     * - switch case E_AAMeasureStatusMeasureC1: PRESEL decision branch selects SEL active/inactive.
     * Assignment: threshold register is built and then written as a whole-register write, LIN->AA_I_DIFF_TH.reg = thresholdReg.
     * Register fields:
     * - I_DIFF_TH_1_VAL[11:0]=preDiffThresholdPos: PRE current-difference threshold field.
     * - other threshold fields are determined by the combined thresholdReg value.
     * AA stage 1: threshold is loaded before AA starts and after each measurement exit.
     * AA stage 2: PRESEL result is compared in case E_AAMeasureStatusMeasureC1.
     * AA stage 3: comparison decides whether SEL active or SEL inactive is used.
     * Meaning: PRE current-difference threshold for selecting candidate nodes.
     */
    .preDiffThresholdPos = LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS,

    /*
     * Macro: LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS
     * Used in:
     * - LIN_AutoAddr_BuildThresholdReg(): builds thresholdReg and places this value into I_DIFF_TH_2_VAL[27:16].
     * - LIN_AutoAddr_Start(): indirectly loads the SEL threshold before AA starts.
     * - LIN_AutoAddr_Meas_Exit(): indirectly reloads the SEL threshold after one window exits.
     * - LIN_AutoAddr_Measure_ISR(): uses this threshold to evaluate the SEL result.
     * - switch case E_AAMeasureStatusMeasureC2_S2: SEL decision branch updates isLastNode/count_succ/count_fail.
     * Assignment: threshold register is built and then written as a whole-register write, LIN->AA_I_DIFF_TH.reg = thresholdReg.
     * Register fields:
     * - I_DIFF_TH_2_VAL[27:16]=selDiffThresholdPos: SEL current-difference threshold field.
     * - other threshold fields are determined by the combined thresholdReg value.
     * AA stage 1: threshold is loaded before AA starts and after each measurement exit.
     * AA stage 2: SEL result is compared in case E_AAMeasureStatusMeasureC2_S2.
     * AA stage 3: comparison updates isLastNode and count_succ/count_fail.
     * Meaning: SEL current-difference threshold for last-node decision.
     */
    .selDiffThresholdPos = LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS,

    /*
     * Macro: D_STEP1_CLOSE_ALL_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_ISRC[11:6].
     * Register fields:
     * - affects OFFSET TBIT_START_ISRC[11:6] through LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION).
     * AA stage 1: OFFSET active configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: LIN_AutoAddr_Meas_Entry() writes the generated AA_CTRL value.
     * AA stage 3: hardware performs the close/disconnect pull-up timing in OFFSET phase.
     * Meaning: OFFSET current-source/pull-up action bit timing.
     */
    .step1CloseAllBit = D_STEP1_CLOSE_ALL_BIT,

    /*
     * Macro: D_STEP2_MEAS_OFFSET_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_MEAS[5:0].
     * Register fields:
     * - affects OFFSET TBIT_START_MEAS[5:0] through LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET).
     * AA stage 1: OFFSET active configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: LIN_AutoAddr_Meas_Entry() writes the generated AA_CTRL value.
     * AA stage 3: hardware samples offset current at this configured bit timing.
     * Meaning: OFFSET measurement bit timing.
     */
    .step2MeasOffsetBit = D_STEP2_MEAS_OFFSET_BIT,

    /*
     * Macro: D_STEP3_OPEN_C1_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_ISRC[11:6].
     * Register fields:
     * - affects PRESEL TBIT_START_ISRC[11:6] through LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION).
     * AA stage 1: PRESEL active configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: case E_AAMeasureStatusMeasureOffset writes the generated AA_CTRL value.
     * AA stage 3: hardware applies PRE current-source timing.
     * Meaning: PRESEL current-source/pull-up action bit timing.
     */
    .step3OpenC1Bit = D_STEP3_OPEN_C1_BIT,

    /*
     * Macro: D_STEP4_MEAS_PRE_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_MEAS[5:0].
     * Register fields:
     * - affects PRESEL TBIT_START_MEAS[5:0] through LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE).
     * AA stage 1: PRESEL active configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: case E_AAMeasureStatusMeasureOffset writes the generated AA_CTRL value.
     * AA stage 3: hardware samples PRE current at this configured bit timing.
     * Meaning: PRESEL measurement bit timing.
     */
    .step4MeasPreBit = D_STEP4_MEAS_PRE_BIT,

    /*
     * Macro: D_STEP5_OPEN_C2_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_ISRC[11:6].
     * Register fields:
     * - affects SEL TBIT_START_ISRC[11:6] through LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION).
     * AA stage 1: SEL active/inactive configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: case E_AAMeasureStatusMeasureC1 writes the selected generated AA_CTRL value.
     * AA stage 3: hardware applies SEL current-source timing.
     * Meaning: SEL current-source/pull-up action bit timing.
     */
    .step5OpenC2Bit = D_STEP5_OPEN_C2_BIT,

    /*
     * Macro: D_STEP6_MEAS_SEL_S1_BIT
     * Used in:
     * Assignment: no direct register write from this struct field; it contributes to whole-register AA_CTRL values through TBIT_START_MEAS[5:0].
     * Register fields:
     * - affects SEL TBIT_START_MEAS[5:0] through LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL).
     * AA stage 1: SEL active/inactive configuration is built in lin_autoaddressing_cfg.h.
     * AA stage 2: case E_AAMeasureStatusMeasureC1 writes the selected generated AA_CTRL value.
     * AA stage 3: hardware samples SEL current at this configured bit timing.
     * Meaning: SEL measurement bit timing.
     */
    .step6MeasSelS1Bit = D_STEP6_MEAS_SEL_S1_BIT,

    /*
     * Macro: D_STEP6_MEAS_SEL_S2_BIT
     * Used in:
     * Assignment: no direct register write and no current hardware effect.
     * Register fields:
     * - none in the current running code.
     * AA stage 1: reserved for a possible second SEL sampling point.
     * AA stage 2: not used by current switch cases.
     * AA stage 3: no current AA hardware action.
     * Meaning: reserved SEL second-sample timing.
     */
    .step6MeasSelS2Bit = D_STEP6_MEAS_SEL_S2_BIT,

    /*
     * Macro: D_STEP7_RESTORE_BIT
     * Used in:
     * Assignment: no direct register write and no current hardware effect.
     * Register fields:
     * - none in the current running code.
     * AA stage 1: reserved for possible post-AA restore timing.
     * AA stage 2: not used by current switch cases.
     * AA stage 3: no current AA hardware action.
     * Meaning: reserved restore bit timing.
     */
    .step7RestoreBit = D_STEP7_RESTORE_BIT
};