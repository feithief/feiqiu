#ifndef LIN_AUTOADDRESSING_CFG_CN_COMMENT_COPY_H
#define LIN_AUTOADDRESSING_CFG_CN_COMMENT_COPY_H

/*
 * 中文注释副本。
 * 本文件只用于阅读，不 include，不加入 Keil 工程。
 * 实际参与编译的是 lin_autoaddressing_cfg.h。
 */

#include <stdint.h>

/*
 * 自动寻址外部配置 ABI。
 * 修改 lin_autoaddressing_cfg.h 中的值后，Rebuild SWSC_Application 即可生效。
 *
 * 寄存器对应关系和赋值方式：
 * - LIN->T2_CON：Timer2 捕获/外部事件控制。
 * - LIN->T2_MOD：Timer2 预分频和边沿选择控制。
 * - ADC11->CFG_0 / ADC11->IEN / ADC11->CFG_1：自动寻址测量前恢复 ADC11 配置。
 * - LIN->AA_CTRL：LIN 自动寻址 phase、时序、电流源、电流检测控制。
 * - LIN->AA_I_DIFF_TH：PRE/SEL 电流差阈值。
 * - 运行代码使用 xxx->REG.reg = value 这种整体寄存器赋值，不是逐 bit 位域赋值。
 */

/* 选择默认 step 表。
 * 定义 LIN_AA_USE_DELAYED_STEP_TIMING：使用 delayed timing，step 整体后移。
 * 注释 LIN_AA_USE_DELAYED_STEP_TIMING：使用 original timing，step 更靠前。
 * 注意：选定分支后，仍然可以直接修改该分支内每个 D_STEPx 的值，Rebuild 后会生效。
 */
#define LIN_AA_USE_DELAYED_STEP_TIMING

/* LIN_AA_CFG_T2_CON_EDGE。
 * 对应寄存器：LIN->T2_CON.reg。
 * 赋值方式：在 LIN_AutoAddr_SetFallingEdge() 和 LIN_AutoAddr_SetRisingEdge() 中整体寄存器赋值。
 * 影响位：
 * - CP_RL2[0]=1：捕获模式。
 * - C_T2[1]=0：timer 模式。
 * - TR2[2]=0：本次写入不启动 T2。
 * - EXEN2[3]=1：外部事件/捕获使能。
 * - T2INSEL[9:8]=0：保持默认 Timer2 输入源。
 * - T2EXINSEL[11:10]=0：保持默认 Timer2 外部捕获输入源。
 * - 其他位=0：因为整体写 0x09，被该常量写为 0。
 */
#define LIN_AA_CFG_T2_CON_EDGE                 (0x00000009u)

/* LIN_AA_CFG_T2_MOD_FALLING_EDGE / LIN_AA_CFG_T2_MOD_RISING_EDGE。
 * 对应寄存器：LIN->T2_MOD.reg。
 * 赋值方式：整体寄存器赋值。
 * 影响位：
 * - 0x10 时 PREN[4]=1：使能 Timer2 预分频。
 * - 0x10 时 EDGESEL[5]=0：选择 T2EX 下降沿。
 * - 0x30 时 PREN[4]=1：使能 Timer2 预分频。
 * - 0x30 时 EDGESEL[5]=1：选择 T2EX 上升沿。
 * - DCEN[0]=0：关闭向上/向下计数模式。
 * - T2PRE[3:1]=0：使用默认预分频编码。
 * - T2RHEN[6]=0：关闭 reload-high 扩展。
 * - T2REGS[7]=0：选择默认 Timer2 寄存器组。
 * - 其他位=0：因为整体写入，被该常量写为 0。
 */
#define LIN_AA_CFG_T2_MOD_FALLING_EDGE         (0x00000010u)
#define LIN_AA_CFG_T2_MOD_RISING_EDGE          (0x00000030u)

/* 自动寻址测量前恢复 ADC11 配置。
 * 对应寄存器：
 * - LIN_AA_CFG_ADC_CFG0_REINIT -> ADC11->CFG_0.reg。
 * - LIN_AA_CFG_ADC_IEN_REINIT  -> ADC11->IEN.reg。
 * - LIN_AA_CFG_ADC_CFG1_REINIT -> ADC11->CFG_1.reg。
 * 赋值方式：在 LIN_AA_ADC_REINIT_FAST() 中整体寄存器赋值，不是单 bit 位域赋值。
 * 影响位：
 * - ADC11->CFG_0.ADC_COMP_VAL[12:0]=4：ADC 比较参考值。
 * - ADC11->CFG_0.ADC_SEL_4V[16]=0：保持默认 ADC 参考/输入选择。
 * - ADC11->CFG_0.ADC_OVERS_CFG[21:20]=3：选择 8 次采样。
 * - ADC11->CFG_0.ADC_STC[23:22]=0：恢复默认 ADC 采样时间控制。
 * - ADC11->CFG_0.DTS_OVT_EN[28]=0：关闭 DTS 过温路径。
 * - ADC11->CFG_0 其他位=0：整体写 0x00300004 时清 0。
 * - ADC11->IEN.ADC_FW_EOC[0]=1：使能 ADC 转换完成中断。
 * - ADC11->IEN.DTS_OVT[8]=0：关闭 DTS 过温中断。
 * - ADC11->IEN 其他位=0：整体写 0x00000001 时清 0。
 * - ADC11->CFG_1.ADC_SOC[0]=0：本次写入不触发 ADC 软件转换。
 * - ADC11->CFG_1.ADC_CHNR[5:1]=0：恢复默认 ADC 通道编码。
 * - ADC11->CFG_1.ADC_ENABLE[8]=1：使能 ADC 模块。
 * - ADC11->CFG_1.ADC_COMP_EN[9]=0：关闭 ADC 比较器路径。
 * - ADC11->CFG_1 其他位=0：整体写 0x00000100 时清 0。
 */
#define LIN_AA_CFG_ADC_CFG0_REINIT             (0x00300004u)
#define LIN_AA_CFG_ADC_IEN_REINIT              (0x00000001u)
#define LIN_AA_CFG_ADC_CFG1_REINIT             (0x00000100u)

/* LIN->AA_CTRL 直接辅助值。
 * 对应寄存器：LIN->AA_CTRL.reg。
 * 赋值方式：整体寄存器赋值。
 * 影响位：
 * - FSM_RESET_ONLY 0x00080000：FSM_RST[19]=1，其他 AA_CTRL 位清 0，用于复位 AA 硬件 FSM。
 * - END_PHASE 0x00800000：ERR_CODE_CLR[23]=1，其他 AA_CTRL 位清 0，用于结束/清理 AA phase。
 */
#define LIN_AA_CFG_CTRL_FSM_RESET_ONLY         (0x00080000u)
#define LIN_AA_CFG_CTRL_END_PHASE              (0x00800000u)

/* 一个 LIN bit 内的测量位置。
 * 宏本身不直接写寄存器，会通过 LIN_AA_CFG_TBIT() 合成到 AA_CTRL 整体寄存器值中。
 * 影响位：
 * - LIN->AA_CTRL.TBIT_START_MEAS[5:0]：控制 AA 电流采样开始的 LIN bit 位置。
 * - LIN->AA_CTRL.TBIT_START_ISRC[11:6]：控制 AA 电流源/上拉动作开始的 LIN bit 位置。
 * BEGINING=0：配置在 bit 开始位置。
 * MIDDLE=1：配置在 bit 中间位置。
 */
#define D_ADC_MESURE_AT_MIDDLE                 (1u)
#define D_ADC_MEASURE_AT_BEGINING              (0u)

#ifdef LIN_AA_USE_DELAYED_STEP_TIMING
/* delayed 自动寻址时序表。
 * step 值是 AA_CTRL.TBIT_START_MEAS / TBIT_START_ISRC 使用的 bit 计数索引。
 * 赋值方式：这里不直接写硬件；这些值会在下面的 AA_CTRL phase 宏展开时进入整体寄存器值。
 */
#define D_STEP1_CLOSE_ALL_BIT                  (1u)
#define D_STEP2_MEAS_OFFSET_BIT                (2u)
#define D_STEP3_OPEN_C1_BIT                    (5u)
#define D_STEP4_MEAS_PRE_BIT                   (6u)
#define D_STEP5_OPEN_C2_BIT                    (9u)
#define D_STEP6_MEAS_SEL_S1_BIT                (10u)
#define D_STEP6_MEAS_SEL_S2_BIT                (11u)
#define D_STEP7_RESTORE_BIT                    (13u)

/* delayed 时序下每个 phase 的测量位置。 */
#define D_ADC_MEASURE_POSITION_OFFSET          D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_PRE             D_ADC_MEASURE_AT_BEGINING
#define D_ADC_MEASURE_POSITION_SEL             D_ADC_MESURE_AT_MIDDLE
#else
/* original 自动寻址时序表。
 * 赋值方式同 delayed 表：不直接写硬件，只影响生成的 AA_CTRL 整体寄存器值。
 */
#define D_STEP1_CLOSE_ALL_BIT                  (0u)
#define D_STEP2_MEAS_OFFSET_BIT                (1u)
#define D_STEP3_OPEN_C1_BIT                    (4u)
#define D_STEP4_MEAS_PRE_BIT                   (5u)
#define D_STEP5_OPEN_C2_BIT                    (8u)
#define D_STEP6_MEAS_SEL_S1_BIT                (9u)
#define D_STEP6_MEAS_SEL_S2_BIT                (10u)
#define D_STEP7_RESTORE_BIT                    (12u)

/* original 时序下每个 phase 的测量位置。 */
#define D_ADC_MEASURE_POSITION_OFFSET          D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_PRE             D_ADC_MESURE_AT_MIDDLE
#define D_ADC_MEASURE_POSITION_SEL             D_ADC_MESURE_AT_MIDDLE
#endif

/* 上拉/电流源配置位置。
 * 影响位：
 * - LIN->AA_CTRL.TBIT_START_ISRC[11:6]：控制 AA 电流源/上拉动作开始的 LIN bit 位置。
 * 赋值方式：通过 AA_CTRL 整体寄存器值生效，宏本身不直接写硬件。
 */
#define D_PULLUP_CONFIG_AT_BEGINNING           (0u)
#define D_PULLUP_CONFIG_POSITION               D_PULLUP_CONFIG_AT_BEGINNING

/* LIN->AA_CTRL.ISRC_CONF_CTRL 电流配置。
 * 影响位：
 * - LIN->AA_CTRL.ISRC_CONF_CTRL[14:12]：配置 AA 电流源幅值。
 * 赋值方式：通过 AA_CTRL 整体寄存器值生效，宏本身不直接写硬件。
 * 0=0mA，1=0.45mA，2=1.00mA，3=1.45mA，4=2.05mA，5=2.50mA，6=3.05mA，7=3.50mA。
 */
#define D_PRE_CURRENT_SETTING                  (1u)
#define D_SEL_CURRENT_SETTING                  (6u)
#define D_INACTIVE_CURRENT_SETTING             (0u)

/* 构造 AA_CTRL.TBIT_START_* 字段。
 * 影响位：
 * - TBIT_START_MEAS[5:0]：由该辅助宏生成测量开始位置字段值。
 * - TBIT_START_ISRC[11:6]：由该辅助宏生成电流源/上拉动作开始位置字段值。
 * 赋值方式：辅助宏，不直接写硬件；最终由 LIN->AA_CTRL.reg 整体写入。
 * 字段是 6 bit：(step_bit << 1) | position。
 */
#define LIN_AA_CFG_TBIT(step_bit, position) \
    ((((uint32_t)(step_bit) << 1) | (uint32_t)(position)) & 0x3Fu)

/* 根据手册 bitfield 构造 LIN->AA_CTRL 整体寄存器值。
 * 对应寄存器：LIN->AA_CTRL.reg。
 * 赋值方式：该宏只构造 32 bit 值；运行代码最终用 LIN->AA_CTRL.reg = value 整体写入。
 * [ 5: 0] TBIT_START_MEAS
 * [11: 6] TBIT_START_ISRC
 * [14:12] ISRC_CONF_CTRL
 * [15]    ISRC_EN
 * [16]    PU_DISC
 * [17]    PH_START
 * [18]    PH_STOP
 * [19]    FSM_RST
 * [21:20] ISENSE_MEAS_NUMBER，3 表示 8 次电流测量
 * [22]    ISENSE_EN
 * [23]    ERR_CODE_CLR
 * [24]    LIN_AA_EN
 * [26:25] LIN_AA_PH，0=END，1=OFFSET，2=PRESEL，3=SEL
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

/* LIN->AA_CTRL phase 值，不带 PH_START。
 * 对应寄存器：LIN->AA_CTRL.reg。
 * 赋值方式：整体寄存器赋值。库先写 preload 值，再写对应 *_START 值作为最后触发。
 */

/* OFFSET active phase。
 * TBIT_START_MEAS = D_STEP2_MEAS_OFFSET_BIT + OFFSET 测量位置。
 * TBIT_START_ISRC = D_STEP1_CLOSE_ALL_BIT + 上拉/电流源位置。
 * 影响位：
 * - ISRC_CONF_CTRL[14:12]=0mA：OFFSET 阶段使用 inactive 电流配置。
 * - ISRC_EN[15]=1：使能电流源控制。
 * - PU_DISC[16]=1：自动寻址时序中断开 pull-up。
 * - PH_START[17]=0：preload 值不启动 phase。
 * - ISENSE_MEAS_NUMBER[21:20]=3：选择 8 次电流测量。
 * - ISENSE_EN[22]=1：使能电流检测。
 * - ERR_CODE_CLR[23]=1：phase 启动前清除旧 AA 错误码。
 * - LIN_AA_EN[24]=1：使能 AA 硬件。
 * - LIN_AA_PH[26:25]=1：选择 OFFSET phase。
 */
#define LIN_AA_CFG_CTRL_OFFSET_ACTIVE          LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 1u)

/* OFFSET inactive phase。
 * 已寻址节点使用：保持 AA enabled 和 OFFSET phase，但 ISENSE_EN=0，不做有效 offset 电流测量。
 */
#define LIN_AA_CFG_CTRL_OFFSET_INACTIVE        LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 0u, 1u, 1u, 1u)

/* PRESEL active phase。
 * TBIT_START_MEAS = D_STEP4_MEAS_PRE_BIT + PRE 测量位置。
 * TBIT_START_ISRC = D_STEP3_OPEN_C1_BIT + 上拉/电流源位置。
 * 影响位：
 * - ISRC_CONF_CTRL[14:12]=0.45mA：PRE 电流源配置。
 * - ISRC_EN[15]=1：使能电流源控制。
 * - PU_DISC[16]=1：自动寻址时序中断开 pull-up。
 * - PH_START[17]=0：preload 值不启动 phase。
 * - ISENSE_MEAS_NUMBER[21:20]=3：选择 8 次电流测量。
 * - ISENSE_EN[22]=1：使能电流检测。
 * - LIN_AA_PH[26:25]=2：选择 PRESEL phase。
 */
#define LIN_AA_CFG_CTRL_PRE_ACTIVE             LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 2u)

/* SEL active phase。
 * TBIT_START_MEAS = D_STEP6_MEAS_SEL_S1_BIT + SEL 测量位置。
 * TBIT_START_ISRC = D_STEP5_OPEN_C2_BIT + 上拉/电流源位置。
 * 影响位：
 * - ISRC_CONF_CTRL[14:12]=3.05mA：SEL 电流源配置。
 * - ISRC_EN[15]=1：使能电流源控制。
 * - PU_DISC[16]=1：自动寻址时序中断开 pull-up。
 * - PH_START[17]=0：preload 值不启动 phase。
 * - ISENSE_MEAS_NUMBER[21:20]=3：选择 8 次电流测量。
 * - ISENSE_EN[22]=1：使能电流检测。
 * - LIN_AA_PH[26:25]=3：选择 SEL phase。
 */
#define LIN_AA_CFG_CTRL_SEL_ACTIVE             LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)

/* SEL inactive phase。
 * TBIT_START_MEAS[5:0]：与 SEL active 相同，保持 SEL 采样时刻一致。
 * TBIT_START_ISRC[11:6]：与 SEL active 相同，保持 SEL 电流源时序一致。
 * ISRC_CONF_CTRL[14:12]=0mA：inactive 节点不输出 SEL 电流。
 * LIN_AA_PH[26:25]=3：选择 SEL phase。
 */
#define LIN_AA_CFG_CTRL_SEL_INACTIVE           LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 0u, 0u, 0u, 3u, 1u, 1u, 1u, 3u)

/* *_START 值。
 * 赋值方式：整体写 LIN->AA_CTRL.reg。
 * 配置内容：与对应 preload phase 基本一致。
 * PH_START[17]=1：最后写入该位，用于触发 AA 硬件 FSM phase。
 * ERR_CODE_CLR[23]=0：START 触发写入时不再清错。
 */
#define LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START    LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP2_MEAS_OFFSET_BIT, D_ADC_MEASURE_POSITION_OFFSET), LIN_AA_CFG_TBIT(D_STEP1_CLOSE_ALL_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 1u)
#define LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START  LIN_AA_CFG_CTRL_BUILD(0u, 0u, D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 0u, 0u, 1u, 1u)
#define LIN_AA_CFG_CTRL_PRE_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP4_MEAS_PRE_BIT, D_ADC_MEASURE_POSITION_PRE), LIN_AA_CFG_TBIT(D_STEP3_OPEN_C1_BIT, D_PULLUP_CONFIG_POSITION), D_PRE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 2u)
#define LIN_AA_CFG_CTRL_SEL_ACTIVE_START       LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_SEL_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)
#define LIN_AA_CFG_CTRL_SEL_INACTIVE_START     LIN_AA_CFG_CTRL_BUILD(LIN_AA_CFG_TBIT(D_STEP6_MEAS_SEL_S1_BIT, D_ADC_MEASURE_POSITION_SEL), LIN_AA_CFG_TBIT(D_STEP5_OPEN_C2_BIT, D_PULLUP_CONFIG_POSITION), D_INACTIVE_CURRENT_SETTING, 1u, 1u, 1u, 0u, 0u, 3u, 1u, 0u, 1u, 3u)

/* LIN->AA_I_DIFF_TH 阈值。
 * 对应寄存器：LIN->AA_I_DIFF_TH.reg。
 * 赋值方式：LIN_AutoAddr_BuildThresholdReg() 先组合成一个 32 bit 阈值寄存器值，再整体写入。
 * 影响位：
 * - PRE threshold 写入 I_DIFF_TH_1_VAL[11:0]。
 * - SEL threshold 写入 I_DIFF_TH_2_VAL[27:16]。
 * - 其他位由组合后的 thresholdReg 决定。
 */
#define LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS      (20)
#define LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS      (11)

/* 与封库 SWSC_Core.lib 共享的 ABI。
 * 不要调整已有字段顺序；如果以后增加字段，只能追加在结构体末尾。
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