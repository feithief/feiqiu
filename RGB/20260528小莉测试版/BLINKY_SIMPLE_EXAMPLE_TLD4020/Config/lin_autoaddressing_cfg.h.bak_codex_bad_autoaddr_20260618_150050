#ifndef LIN_AUTOADDRESSING_CFG_H
#define LIN_AUTOADDRESSING_CFG_H

#include <stdint.h>

/*
 * External configuration ABI used by the sealed auto-addressing library.
 * Modify the values below and Rebuild SWSC_Application.
 *
 * Register map reference and write style:
 * - LIN->T2_CON: Timer2 capture/external event control.
 * - LIN->T2_MOD: Timer2 prescaler and edge select control.
 * - ADC11->CFG_0/IEN/CFG_1: ADC restore configuration before AA measurement.
 * - LIN->AA_CTRL: LIN auto-addressing phase/timing/current-sense control.
 * - LIN->AA_I_DIFF_TH: PRE/SEL current-difference thresholds.
 * - Runtime code writes these configuration values as whole registers
 *   (xxx->REG.reg = value), not as individual bit-field assignments.
 */

/* Select default step table.
 * Defined   : delayed timing table, steps are shifted later.
 * Undefined : original timing table, steps are earlier.
 * The D_STEPx values below can still be edited directly after selecting a table.
 */
#define LIN_AA_USE_DELAYED_STEP_TIMING

/* LIN_AA_CFG_T2_CON_EDGE.
 * Register: LIN->T2_CON.reg.
 * Assignment: whole-register write in LIN_AutoAddr_SetFallingEdge()
 * and LIN_AutoAddr_SetRisingEdge().
 * Affected fields:
 * - CP_RL2[0]=1: capture mode.
 * - C_T2[1]=0: timer mode.
 * - TR2[2]=0: T2 is not started by this write.
 * - EXEN2[3]=1: external event/capture enabled.
 * - T2INSEL[9:8]=0: keeps the default Timer2 input source.
 * - T2EXINSEL[11:10]=0: keeps the default Timer2 external capture source.
 * - Other bits=0: cleared because the full register value 0x09 is written.
 */
#define LIN_AA_CFG_T2_CON_EDGE                 (0x00000009u)

/* LIN_AA_CFG_T2_MOD_FALLING_EDGE / LIN_AA_CFG_T2_MOD_RISING_EDGE.
 * Register: LIN->T2_MOD.reg.
 * Assignment: whole-register write.
 * Affected fields:
 * - For 0x10, PREN[4]=1: enables Timer2 prescaler.
 * - For 0x10, EDGESEL[5]=0: selects falling edge at T2EX.
 * - For 0x30, PREN[4]=1: enables Timer2 prescaler.
 * - For 0x30, EDGESEL[5]=1: selects rising edge at T2EX.
 * - DCEN[0]=0: disables up/down counter mode.
 * - T2PRE[3:1]=0: keeps the default prescaler encoding.
 * - T2RHEN[6]=0: disables reload-high extension.
 * - T2REGS[7]=0: selects the default Timer2 register set.
 * - Other bits=0: cleared because the full register value is written.
 */
#define LIN_AA_CFG_T2_MOD_FALLING_EDGE         (0x00000010u)
#define LIN_AA_CFG_T2_MOD_RISING_EDGE          (0x00000030u)

/* ADC11 restore values used before AA measurement.
 * Register mapping:
 * - LIN_AA_CFG_ADC_CFG0_REINIT -> ADC11->CFG_0.reg.
 * - LIN_AA_CFG_ADC_IEN_REINIT  -> ADC11->IEN.reg.
 * - LIN_AA_CFG_ADC_CFG1_REINIT -> ADC11->CFG_1.reg.
 * Assignment: whole-register writes in LIN_AA_ADC_REINIT_FAST(), not
 * individual bit-field writes.
 * Affected fields:
 * - ADC11->CFG_0.ADC_COMP_VAL[12:0]=4: ADC comparator reference value.
 * - ADC11->CFG_0.ADC_SEL_4V[16]=0: keeps the default ADC reference/input selection.
 * - ADC11->CFG_0.ADC_OVERS_CFG[21:20]=3: selects 8 samples per measurement.
 * - ADC11->CFG_0.ADC_STC[23:22]=0: restores default ADC sample-time control.
 * - ADC11->CFG_0.DTS_OVT_EN[28]=0: disables DTS over-temperature path.
 * - ADC11->CFG_0 other bits=0: cleared by whole-register write 0x00300004.
 * - ADC11->IEN.ADC_FW_EOC[0]=1: enables ADC end-of-conversion interrupt.
 * - ADC11->IEN.DTS_OVT[8]=0: disables DTS over-temperature interrupt.
 * - ADC11->IEN other bits=0: cleared by whole-register write 0x00000001.
 * - ADC11->CFG_1.ADC_SOC[0]=0: no software conversion trigger from this write.
 * - ADC11->CFG_1.ADC_CHNR[5:1]=0: restores default ADC channel encoding.
 * - ADC11->CFG_1.ADC_ENABLE[8]=1: enables ADC module.
 * - ADC11->CFG_1.ADC_COMP_EN[9]=0: disables ADC comparator path.
 * - ADC11->CFG_1 other bits=0: cleared by whole-register write 0x00000100.
 */
#define LIN_AA_CFG_ADC_CFG0_REINIT             (0x00300004u)
#define LIN_AA_CFG_ADC_IEN_REINIT              (0x00000001u)
#define LIN_AA_CFG_ADC_CFG1_REINIT             (0x00000100u)

/* LIN->AA_CTRL direct helper values.
 * Register: LIN->AA_CTRL.reg.
 * Assignment: whole-register write.
 * Affected fields:
 * - FSM_RESET_ONLY 0x00080000: FSM_RST[19]=1; other AA_CTRL fields
 *   are cleared. Used to reset the AA hardware FSM.
 * - END_PHASE 0x00800000: ERR_CODE_CLR[23]=1; other AA_CTRL fields
 *   are cleared. Used to close/clear an AA phase.
 */
#define LIN_AA_CFG_CTRL_FSM_RESET_ONLY         (0x00080000u)
#define LIN_AA_CFG_CTRL_END_PHASE              (0x00800000u)

/* TBIT position inside one LIN bit.
 * Register field affected after macro expansion:
 * - LIN->AA_CTRL.TBIT_START_MEAS[5:0].
 * - LIN->AA_CTRL.TBIT_START_ISRC[11:6].
 * Assignment: these position macros do not write hardware directly; they
 * are combined into whole AA_CTRL register values by LIN_AA_CFG_TBIT().
 * BEGINING=0 selects the beginning of the configured bit.
 * MIDDLE  =1 selects the middle of the configured bit.
 */
#define D_ADC_MESURE_AT_MIDDLE                 (1u)
#define D_ADC_MEASURE_AT_BEGINING              (0u)

#ifdef LIN_AA_USE_DELAYED_STEP_TIMING
/* Delayed AA timing table.
 * Step number is the LIN AA TBIT counter index used by AA_CTRL timing fields.
 * Assignment: no direct hardware write here. These values are folded into
 * LIN->AA_CTRL.TBIT_START_MEAS[5:0] or TBIT_START_ISRC[11:6] when the
 * AA_CTRL phase macros below are expanded.
 */
#define D_STEP1_CLOSE_ALL_BIT                  (1u)
#define D_STEP2_MEAS_OFFSET_BIT                (2u)
#define D_STEP3_OPEN_C1_BIT                    (5u)
#define D_STEP4_MEAS_PRE_BIT                   (6u)
#define D_STEP5_OPEN_C2_BIT                    (9u)
#define D_STEP6_MEAS_SEL_S1_BIT                (10u)
#define D_STEP6_MEAS_SEL_S2_BIT                (11u)
#define D_STEP7_RESTORE_BIT                    (13u)

/* Measurement position for each phase in delayed timing table. */
#define D_ADC_MEASURE_POSITION_OFFSET          D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_PRE             D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_SEL             D_ADC_MESURE_AT_MIDDLE
#else
/* Original AA timing table.
 * Assignment: same as delayed table; values only affect generated AA_CTRL
 * whole-register values.
 */
#define D_STEP1_CLOSE_ALL_BIT                  (0u)
#define D_STEP2_MEAS_OFFSET_BIT                (1u)
#define D_STEP3_OPEN_C1_BIT                    (4u)
#define D_STEP4_MEAS_PRE_BIT                   (5u)
#define D_STEP5_OPEN_C2_BIT                    (8u)
#define D_STEP6_MEAS_SEL_S1_BIT                (9u)
#define D_STEP6_MEAS_SEL_S2_BIT                (10u)
#define D_STEP7_RESTORE_BIT                    (12u)

/* Measurement position for each phase in original timing table. */
#define D_ADC_MEASURE_POSITION_OFFSET          D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_PRE             D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_SEL             D_ADC_MESURE_AT_MIDDLE
#endif

/* Pull-up/current-source timing position.
 * Register field affected after macro expansion:
 * - LIN->AA_CTRL.TBIT_START_ISRC[11:6].
 * Assignment: combined into AA_CTRL whole-register values; no direct
 * hardware write from this macro itself.
 * Current implementation configures ISRC timing at the beginning of the bit.
 */
#define D_PULLUP_CONFIG_AT_BEGINNING           (0u)
#define D_PULLUP_CONFIG_POSITION               D_PULLUP_CONFIG_AT_BEGINNING

/* LIN->AA_CTRL.ISRC_CONF_CTRL current setting.
 * Register field affected after macro expansion:
 * - LIN->AA_CTRL.ISRC_CONF_CTRL[14:12].
 * Assignment: combined into AA_CTRL whole-register values; no direct
 * hardware write from these macros themselves.
 * 0=0mA, 1=0.45mA, 2=1.00mA, 3=1.45mA,
 * 4=2.05mA, 5=2.50mA, 6=3.05mA, 7=3.50mA.
 */
#define D_PRE_CURRENT_SETTING                  (1u)
#define D_SEL_CURRENT_SETTING                  (6u)
#define D_INACTIVE_CURRENT_SETTING             (0u)

/* Build AA_CTRL.TBIT_START_* field value.
 * Register field affected after expansion:
 * - Used as the value for TBIT_START_MEAS[5:0] or TBIT_START_ISRC[11:6].
 * Assignment: helper macro only; hardware is written later as a whole
 * LIN->AA_CTRL.reg value.
 * AA_CTRL.TBIT_START_MEAS/ISRC is 6 bits: (step_bit << 1) | position.
 */
#define LIN_AA_CFG_TBIT(step_bit, position) \
    ((((uint32_t)(step_bit) << 1) | (uint32_t)(position)) & 0x3Fu)

/* Build LIN->AA_CTRL register value from manual bitfields.
 * Register: LIN->AA_CTRL.reg.
 * Assignment: this macro builds one whole-register value. The AA runtime
 * writes the resulting value with LIN->AA_CTRL.reg = value.
 * [ 5: 0] TBIT_START_MEAS
 * [11: 6] TBIT_START_ISRC
 * [14:12] ISRC_CONF_CTRL
 * [15]    ISRC_EN
 * [16]    PU_DISC
 * [17]    PH_START
 * [18]    PH_STOP
 * [19]    FSM_RST
 * [21:20] ISENSE_MEAS_NUMBER, 3 means 8 current measures
 * [22]    ISENSE_EN
 * [23]    ERR_CODE_CLR
 * [24]    LIN_AA_EN
 * [26:25] LIN_AA_PH, 0=END, 1=OFFSET, 2=PRESEL, 3=SEL
 */
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

/* LIN->AA_CTRL phase values without PH_START.
 * Register: LIN->AA_CTRL.reg.
 * Assignment: whole-register writes. The library writes the preload value
 * first, then writes the matching *_START value as the final trigger.
 */

/* OFFSET active phase:
 * TBIT_START_MEAS = D_STEP2_MEAS_OFFSET_BIT + OFFSET position.
 * TBIT_START_ISRC = D_STEP1_CLOSE_ALL_BIT + pull-up/current-source position.
 * ISRC_CONF_CTRL[14:12]=0mA: offset phase uses inactive current setting.
 * ISRC_EN[15]=1: enables current-source control.
 * PU_DISC[16]=1: disconnects pull-up during AA timing.
 * PH_START[17]=0: preload value does not start the phase.
 * ISENSE_MEAS_NUMBER[21:20]=3: selects 8 current measurements.
 * ISENSE_EN[22]=1: enables current-sense measurement.
 * ERR_CODE_CLR[23]=1: clears previous AA error code before phase start.
 * LIN_AA_EN[24]=1: enables AA hardware.
 * LIN_AA_PH[26:25]=1: selects OFFSET phase.
 */
#define LIN_AA_CFG_CTRL_OFFSET_ACTIVE          LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 1u)

/* OFFSET inactive phase:
 * Keeps AA enabled in OFFSET phase with pull-up disconnected and current source
 * enabled at 0mA; current-sense measurement is disabled.
 */
#define LIN_AA_CFG_CTRL_OFFSET_INACTIVE        LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 0u, 1u, 1u, 1u)

/* PRESEL active phase:
 * TBIT_START_MEAS = D_STEP4_MEAS_PRE_BIT + PRE position.
 * TBIT_START_ISRC = D_STEP3_OPEN_C1_BIT + pull-up/current-source position.
 * ISRC_CONF_CTRL[14:12]=0.45mA: PRE current source setting.
 * ISRC_EN[15]=1: enables current-source control.
 * PU_DISC[16]=1: disconnects pull-up during AA timing.
 * PH_START[17]=0: preload value does not start the phase.
 * ISENSE_MEAS_NUMBER[21:20]=3: selects 8 current measurements.
 * ISENSE_EN[22]=1: enables current-sense measurement.
 * LIN_AA_PH[26:25]=2: selects PRESEL phase.
 */
#define LIN_AA_CFG_CTRL_PRE_ACTIVE             LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 2u)

/* SEL active phase:
 * TBIT_START_MEAS = D_STEP6_MEAS_SEL_S1_BIT + SEL position.
 * TBIT_START_ISRC = D_STEP5_OPEN_C2_BIT + pull-up/current-source position.
 * ISRC_CONF_CTRL[14:12]=3.05mA: SEL current source setting.
 * ISRC_EN[15]=1: enables current-source control.
 * PU_DISC[16]=1: disconnects pull-up during AA timing.
 * PH_START[17]=0: preload value does not start the phase.
 * ISENSE_MEAS_NUMBER[21:20]=3: selects 8 current measurements.
 * ISENSE_EN[22]=1: enables current-sense measurement.
 * LIN_AA_PH[26:25]=3: selects SEL phase.
 */
#define LIN_AA_CFG_CTRL_SEL_ACTIVE             LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)

/* SEL inactive phase:
 * TBIT_START_MEAS[5:0]: same SEL sample timing as SEL active.
 * TBIT_START_ISRC[11:6]: same SEL current-source timing as SEL active.
 * ISRC_CONF_CTRL[14:12]=0mA: inactive node does not drive SEL current.
 * LIN_AA_PH[26:25]=3: selects SEL phase.
 */
#define LIN_AA_CFG_CTRL_SEL_INACTIVE           LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)

/* *_START values:
 * Same phase configuration as the matching preload value.
 * PH_START[17]=1: written last to trigger the AA hardware FSM phase.
 * ERR_CODE_CLR[23]=0: START write does not clear errors again.
 */
#define LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START    LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 1u)
#define LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START  LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 0u, 0u, 1u, 1u)
#define LIN_AA_CFG_CTRL_PRE_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 2u)
#define LIN_AA_CFG_CTRL_SEL_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)
#define LIN_AA_CFG_CTRL_SEL_INACTIVE_START     LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)

/* LIN->AA_I_DIFF_TH thresholds.
 * Register: LIN->AA_I_DIFF_TH.reg.
 * Assignment: LIN_AutoAddr_BuildThresholdReg() builds one combined
 * threshold register value, then runtime writes the whole register.
 * Affected fields:
 * - PRE threshold maps to I_DIFF_TH_1_VAL[11:0].
 * - SEL threshold maps to I_DIFF_TH_2_VAL[27:16].
 * - Other bits are determined by the combined threshold register value.
 */
#define LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS      (20)
#define LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS      (11)

/* ABI shared with sealed SWSC_Core.lib.
 * Do not reorder existing fields. New fields can only be appended.
 */
typedef struct
{
    uint32_t t2ConEdge;
    uint32_t t2ModFallingEdge;
    uint32_t t2ModRisingEdge;

    uint32_t adcCfg0Reinit;
    uint32_t adcIenReinit;
    uint32_t adcCfg1Reinit;

    uint32_t ctrlFsmResetOnly;
    uint32_t ctrlEndPhase;
    uint32_t ctrlOffsetActive;
    uint32_t ctrlOffsetInactive;
    uint32_t ctrlPreActive;
    uint32_t ctrlSelActive;
    uint32_t ctrlSelInactive;
    uint32_t ctrlOffsetActiveStart;
    uint32_t ctrlOffsetInactiveStart;
    uint32_t ctrlPreActiveStart;
    uint32_t ctrlSelActiveStart;
    uint32_t ctrlSelInactiveStart;

    int16_t preDiffThresholdPos;
    int16_t selDiffThresholdPos;

    uint8_t step1CloseAllBit;
    uint8_t step2MeasOffsetBit;
    uint8_t step3OpenC1Bit;
    uint8_t step4MeasPreBit;
    uint8_t step5OpenC2Bit;
    uint8_t step6MeasSelS1Bit;
    uint8_t step6MeasSelS2Bit;
    uint8_t step7RestoreBit;
} LinAutoAddr_Config_t;

extern const LinAutoAddr_Config_t g_linAutoAddrConfig;

#endif
