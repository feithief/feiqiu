/*
 * 中文注释副本。
 * 本文件只用于阅读，不加入 Keil 工程编译。
 * 如果加入工程，会和 lin_autoaddressing_cfg.c 重复定义 g_linAutoAddrConfig。
 */
#include "lin_autoaddressing_cfg.h"

/*
 * 自动寻址外部配置实例。
 *
 * 使用链路：
 * 1. lin_autoaddressing_cfg.h 中的宏展开成 g_linAutoAddrConfig。
 * 2. AA/lin_autoaddressing.c 通过 g_linAutoAddrConfig 读取配置。
 * 3. 最终写入 LIN->T2_CON、LIN->T2_MOD、ADC11、LIN->AA_CTRL、LIN->AA_I_DIFF_TH。
 *
 * 注释规则：
 * - 使用位置：写明函数；如果在 switch 中使用，写到具体 case。
 * - 赋值方式：写明是整体寄存器赋值，还是仅保存记录。
 * - 影响位：写明具体寄存器字段和 bit 位置。
 * - 阶段：拆成 1/2/3 点描述自动寻址流程阶段。
 * - 意义：说明这个配置在自动寻址中控制什么。
 */
const LinAutoAddr_Config_t g_linAutoAddrConfig =
{
    /*
     * 宏：LIN_AA_CFG_T2_CON_EDGE
     * 使用位置：
     * - LIN_AutoAddr_SetFallingEdge()：自动寻址开始前或一次测量退出后，配置 T2 等待 break 下降沿。
     * - LIN_AutoAddr_SetRisingEdge()：自动寻址测量窗口内，配置 T2 等待 break 上升沿。
     * 赋值方式：整体寄存器赋值，LIN->T2_CON.reg = t2ConEdge。
     * 影响位：
     * - CP_RL2[0]=1 捕获模式：捕获模式选择，用于让 Timer2 锁存选定 LIN 边沿时间。
     * - EXEN2[3]=1 外部事件使能：外部事件捕获使能，用于响应 T2EX 上的 LIN 边沿。
     * - C_T2[1]=0：选择 timer 模式，Timer2 使用内部计时而不是外部计数。
     * - TR2[2]=0：Timer2 运行控制位；这里不通过该写入启动 Timer2。
     * - T2INSEL[9:8]=0：Timer2 输入源选择；整体写入时保持默认输入源。
     * - T2EXINSEL[11:10]=0：Timer2 外部捕获输入源选择；整体写入时保持默认输入源。
     * - 整体写入说明：整体写 0x09，未单独置位的字段会按该常量写入。
     * 阶段 1：LIN_AutoAddr_Start() 中准备 T2，等待第一次 break 下降沿。
     * 阶段 2：LIN_AutoAddr_Meas_Entry() 进入测量窗口时保持 T2 捕获配置。
     * 阶段 3：LIN_AutoAddr_Meas_Exit() 退出一次测量后重新准备下一次边沿捕获。
     * 意义：自动寻址期间使能 Timer2 对 LIN 边沿的捕获。
     */
    .t2ConEdge = LIN_AA_CFG_T2_CON_EDGE,

    /*
     * 宏：LIN_AA_CFG_T2_MOD_FALLING_EDGE
     * 使用位置：
     * - LIN_AutoAddr_SetFallingEdge()：自动寻址开始前或一次测量退出后，配置 T2 等待 break 下降沿。
     * - LIN_AutoAddr_Start()：自动寻址启动阶段，准备 T2、ADC、阈值和 AA FSM。
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * 赋值方式：整体寄存器赋值，LIN->T2_MOD.reg = t2ModFallingEdge。
     * 影响位：
     * - PREN[4]=1 预分频使能：Timer2 预分频使能，用于自动寻址边沿计时。
     * - EDGESEL[5]=0 选择下降沿：T2EX 边沿选择，决定等待下降沿还是上升沿。
     * - DCEN[0]=0：向上/向下计数模式控制；这里关闭该模式。
     * - T2PRE[3:1]=0：Timer2 预分频档位；这里写 0 使用当前默认编码。
     * - T2RHEN[6]=0：Timer2 reload-high 扩展控制；这里关闭。
     * - T2REGS[7]=0：Timer2 寄存器组选择；这里使用默认寄存器组。
     * - 整体写入说明：整体写入被清 0，未单独置位的字段会按该常量写入。
     * 阶段 1：自动寻址开始前，等待 break 下降沿。
     * 阶段 2：一次测量退出后，重新等待下一次 break 下降沿。
     * 阶段 3：下降沿触发 LIN_AutoAddr_Meas_Entry()，进入自动寻址测量窗口。
     * 意义：检测一次自动寻址测量窗口的开始。
     */
    .t2ModFallingEdge = LIN_AA_CFG_T2_MOD_FALLING_EDGE,

    /*
     * 宏：LIN_AA_CFG_T2_MOD_RISING_EDGE
     * 使用位置：
     * - LIN_AutoAddr_Meas_Entry()：break 下降沿进入测量窗口阶段，根据节点状态启动 OFFSET active/inactive。
     * - LIN_AA_SET_RISING_EDGE_ARMED_FAST()：测量窗口入口的快速路径，把 T2 从下降沿等待切到上升沿等待。
     * - LIN_AutoAddr_SetRisingEdge()：自动寻址测量窗口内，配置 T2 等待 break 上升沿。
     * 赋值方式：整体寄存器赋值，LIN->T2_MOD.reg = t2ModRisingEdge。
     * 影响位：
     * - PREN[4]=1 预分频使能：Timer2 预分频使能，用于自动寻址边沿计时。
     * - EDGESEL[5]=1 选择上升沿：T2EX 边沿选择，决定等待下降沿还是上升沿。
     * - DCEN[0]=0：向上/向下计数模式控制；这里关闭该模式。
     * - T2PRE[3:1]=0：Timer2 预分频档位；这里写 0 使用当前默认编码。
     * - T2RHEN[6]=0：Timer2 reload-high 扩展控制；这里关闭。
     * - T2REGS[7]=0：Timer2 寄存器组选择；这里使用默认寄存器组。
     * - 整体写入说明：整体写入被清 0，未单独置位的字段会按该常量写入。
     * 阶段 1：break 下降沿已捕获。
     * 阶段 2：AA 硬件 phase 已启动，同时切到等待 break 上升沿。
     * 阶段 3：break 上升沿通过 LIN_AutoAddr_RisingEdge_ISR() 退出当前测量窗口。
     * 意义：检测一次自动寻址测量窗口的结束。
     */
    .t2ModRisingEdge = LIN_AA_CFG_T2_MOD_RISING_EDGE,

    /*
     * 宏：LIN_AA_CFG_ADC_CFG0_REINIT
     * 使用位置：
     * - LIN_AA_ADC_REINIT_FAST()：自动寻址测量前快速恢复 ADC11 配置寄存器。
     * - LIN_AA_PREPARE_ENTRY_FAST()：自动寻址进入前等待 ADC ready、恢复 ADC，并加载阈值寄存器。
     * - LIN_AutoAddr_Start()：自动寻址启动阶段，准备 T2、ADC、阈值和 AA FSM。
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * 赋值方式：整体寄存器赋值，ADC11->CFG_0.reg = adcCfg0Reinit。
     * 影响位：
     * - ADC_COMP_VAL[12:0]=4：ADC 比较参考/基准值。
     * - ADC_OVERS_CFG[21:20]=3 表示 8 次采样：ADC 过采样配置，决定每次测量采样次数。
     * - ADC_SEL_4V[16]=0：ADC 参考/输入选择位；这里保持默认路径。
     * - ADC_STC[23:22]=0：ADC 采样时间控制位；这里恢复默认值。
     * - DTS_OVT_EN[28]=0 和其他位因整体写入被清 0：DTS 过温路径使能；这里关闭。
     * 阶段 1：自动寻址开始前恢复 ADC11->CFG_0。
     * 阶段 2：一次自动寻址测量退出后，再次恢复 ADC11->CFG_0。
     * 阶段 3：AA 电流测量使用该采样配置。
     * 意义：保证 AA 电流测量的 ADC 过采样和比较基准配置确定。
     */
    .adcCfg0Reinit = LIN_AA_CFG_ADC_CFG0_REINIT,

    /*
     * 宏：LIN_AA_CFG_ADC_IEN_REINIT
     * 使用位置：
     * - LIN_AA_ADC_REINIT_FAST()：自动寻址测量前快速恢复 ADC11 配置寄存器。
     * - LIN_AA_PREPARE_ENTRY_FAST()：自动寻址进入前等待 ADC ready、恢复 ADC，并加载阈值寄存器。
     * - LIN_AutoAddr_Start()：自动寻址启动阶段，准备 T2、ADC、阈值和 AA FSM。
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * 赋值方式：整体寄存器赋值，ADC11->IEN.reg = adcIenReinit。
     * 影响位：
     * - ADC_FW_EOC[0]=1 转换完成中断使能：ADC 转换完成中断使能，用于通知固件处理测量结果。
     * - DTS_OVT[8]=0 和其他中断使能位因整体写入被清 0：DTS 过温中断使能；这里关闭。
     * 阶段 1：自动寻址开始前使能 ADC 完成中断路径。
     * 阶段 2：一次自动寻址测量退出后重新使能 ADC 完成中断路径。
     * 阶段 3：ADC 完成后可以触发 AA 测量处理流程。
     * 意义：保证 AA 期间 ADC 测量完成事件能被固件处理。
     */
    .adcIenReinit = LIN_AA_CFG_ADC_IEN_REINIT,

    /*
     * 宏：LIN_AA_CFG_ADC_CFG1_REINIT
     * 使用位置：
     * - LIN_AA_ADC_REINIT_FAST()：自动寻址测量前快速恢复 ADC11 配置寄存器。
     * - LIN_AA_PREPARE_ENTRY_FAST()：自动寻址进入前等待 ADC ready、恢复 ADC，并加载阈值寄存器。
     * - LIN_AutoAddr_Start()：自动寻址启动阶段，准备 T2、ADC、阈值和 AA FSM。
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * 赋值方式：整体寄存器赋值，ADC11->CFG_1.reg = adcCfg1Reinit。
     * 影响位：
     * - ADC_ENABLE[8]=1：ADC 模块使能位。
     * - ADC_SOC[0]=0：ADC 软件触发转换位；这里不触发新转换。
     * - ADC_CHNR[5:1]=0：ADC 通道选择；这里恢复默认通道编码。
     * - ADC_COMP_EN[9]=0 和其他位因整体写入被清 0：ADC 比较器路径使能；这里关闭。
     * 阶段 1：自动寻址开始前使能 ADC 模块。
     * 阶段 2：一次自动寻址测量退出后恢复 ADC 模块状态。
     * 阶段 3：AA 电流检测可以使用 ADC 硬件。
     * 意义：确保 AA 电流测量前 ADC 已处于可用状态。
     */
    .adcCfg1Reinit = LIN_AA_CFG_ADC_CFG1_REINIT,

    /*
     * 宏：LIN_AA_CFG_CTRL_FSM_RESET_ONLY
     * 使用位置：
     * - LIN_AA_PRELOAD_FSM_RESET_FAST()：自动寻址 phase 开始前或退出后，快速复位 AA 硬件 FSM。
     * - LIN_AutoAddr_Start()：自动寻址启动阶段，准备 T2、ADC、阈值和 AA FSM。
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlFsmResetOnly。
     * 影响位：
     * - FSM_RST[19]=1：AA 硬件状态机复位位。
     * - AA_CTRL 其他字段因整体写入被清 0。
     * 阶段 1：进入自动寻址前复位 AA FSM。
     * 阶段 2：一次测量窗口退出后复位 AA FSM。
     * 阶段 3：下一次 AA phase 从确定硬件状态开始。
     * 意义：复位 AA 硬件状态机，但不启动 phase。
     */
    .ctrlFsmResetOnly = LIN_AA_CFG_CTRL_FSM_RESET_ONLY,

    /*
     * 宏：LIN_AA_CFG_CTRL_END_PHASE
     * 使用位置：
     * - LIN_AutoAddr_Meas_Exit()：一次测量窗口退出阶段，结束当前 phase 并准备下一次窗口。
     * - LIN_AutoAddr_End()：自动寻址结束阶段，关闭/清理 AA phase。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlEndPhase。
     * 影响位：
     * - ERR_CODE_CLR[23]=1：AA 错误码清除位。
     * - AA_CTRL 其他字段因整体写入被清 0。
     * 阶段 1：上升沿或错误触发当前 AA 测量窗口退出。
     * 阶段 2：关闭当前 AA phase，并清理 AA 错误状态。
     * 阶段 3：固件准备下一次窗口，或完全结束自动寻址。
     * 意义：结束当前 AA phase 并清除 AA 错误状态。
     */
    .ctrlEndPhase = LIN_AA_CFG_CTRL_END_PHASE,

    /*
     * 宏：LIN_AA_CFG_CTRL_OFFSET_ACTIVE
     * 使用位置：
     * - LIN_AutoAddr_Meas_Entry()：break 下降沿进入测量窗口阶段，根据节点状态启动 OFFSET active/inactive。
     * - 状态分支 E_StateMachineStatusWaitting：该节点状态下选择此配置。
     * 赋值方式：整体寄存器赋值，先写 LIN->AA_CTRL.reg = ctrlOffsetActive，再写 ctrlOffsetActiveStart 作为最后触发。
     * 影响位：
     * - TBIT_START_MEAS[5:0]：AA 电流采样开始的 LIN bit 计数位置。
     * - TBIT_START_ISRC[11:6]：AA 电流源/上拉动作开始的 LIN bit 计数位置。
     * - ISRC_CONF_CTRL[14:12]=0mA：AA 电流源幅值配置。
     * - ISRC_EN[15]=1：AA 电流源控制使能。
     * - PU_DISC[16]=1：断开 pull-up 控制位。
     * - PH_START[17]=0：AA phase 启动触发位；START 宏最后写入该位。
     * - ISENSE_MEAS_NUMBER[21:20]=3：电流检测采样次数配置。
     * - ISENSE_EN[22]=1：AA 电流检测使能。
     * - ERR_CODE_CLR[23]=1：AA 错误码清除位。
     * - LIN_AA_EN[24]=1：LIN 自动寻址硬件使能。
     * - LIN_AA_PH[26:25]=1 OFFSET：AA phase 选择，决定 OFFSET/PRESEL/SEL。
     * 阶段 1：break 下降沿进入 LIN_AutoAddr_Meas_Entry()。
     * 阶段 2：预装 OFFSET active phase 配置，但不置 PH_START。
     * 阶段 3：ctrlOffsetActiveStart 置 PH_START，启动硬件 phase。
     * 意义：为未寻址节点准备 offset 电流测量。
     */
    .ctrlOffsetActive = LIN_AA_CFG_CTRL_OFFSET_ACTIVE,

    /*
     * 宏：LIN_AA_CFG_CTRL_OFFSET_INACTIVE
     * 使用位置：
     * - LIN_AutoAddr_Meas_Entry()：break 下降沿进入测量窗口阶段，根据节点状态启动 OFFSET active/inactive。
     * - 状态分支 E_StateMachineStatusAddressed：该节点状态下选择此配置。
     * 赋值方式：整体寄存器赋值，先写 LIN->AA_CTRL.reg = ctrlOffsetInactive，再写 ctrlOffsetInactiveStart 作为最后触发。
     * 影响位：
     * - ISRC_CONF_CTRL[14:12]=0mA：AA 电流源幅值配置。
     * - ISRC_EN[15]=1：AA 电流源控制使能。
     * - PU_DISC[16]=1：断开 pull-up 控制位。
     * - PH_START[17]=0：AA phase 启动触发位；START 宏最后写入该位。
     * - ISENSE_MEAS_NUMBER[21:20]=3：电流检测采样次数配置。
     * - ISENSE_EN[22]=0：AA 电流检测使能。
     * - ERR_CODE_CLR[23]=1：AA 错误码清除位。
     * - LIN_AA_EN[24]=1：LIN 自动寻址硬件使能。
     * - LIN_AA_PH[26:25]=1 OFFSET：AA phase 选择，决定 OFFSET/PRESEL/SEL。
     * - 时序字段为 0。
     * 阶段 1：已寻址节点检测到 break 下降沿。
     * 阶段 2：预装 OFFSET inactive 配置。
     * 阶段 3：ctrlOffsetInactiveStart 启动 inactive 硬件 phase。
     * 意义：让已寻址节点跟随 AA 流程，但不再进行有效 offset 测量。
     */
    .ctrlOffsetInactive = LIN_AA_CFG_CTRL_OFFSET_INACTIVE,

    /*
     * 宏：LIN_AA_CFG_CTRL_PRE_ACTIVE
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureOffset：该配置在这个测量状态分支内生效。
     * 赋值方式：整体寄存器赋值，先写 LIN->AA_CTRL.reg = ctrlPreActive，再写 ctrlPreActiveStart 作为最后触发。
     * 影响位：
     * - TBIT_START_MEAS[5:0]：AA 电流采样开始的 LIN bit 计数位置。
     * - TBIT_START_ISRC[11:6]：AA 电流源/上拉动作开始的 LIN bit 计数位置。
     * - ISRC_CONF_CTRL[14:12]=0.45mA：AA 电流源幅值配置。
     * - ISRC_EN[15]=1：AA 电流源控制使能。
     * - PU_DISC[16]=1：断开 pull-up 控制位。
     * - PH_START[17]=0：AA phase 启动触发位；START 宏最后写入该位。
     * - ISENSE_MEAS_NUMBER[21:20]=3：电流检测采样次数配置。
     * - ISENSE_EN[22]=1：AA 电流检测使能。
     * - ERR_CODE_CLR[23]=1：AA 错误码清除位。
     * - LIN_AA_EN[24]=1：LIN 自动寻址硬件使能。
     * - LIN_AA_PH[26:25]=2 PRESEL：AA phase 选择，决定 OFFSET/PRESEL/SEL。
     * 阶段 1：case E_AAMeasureStatusMeasureOffset 中读取 OFFSET 测量结果。
     * 阶段 2：预装 PRESEL active phase 配置。
     * 阶段 3：ctrlPreActiveStart 启动 PRESEL 测量。
     * 意义：offset 补偿后测量 PRE 电流差。
     */
    .ctrlPreActive = LIN_AA_CFG_CTRL_PRE_ACTIVE,

    /*
     * 宏：LIN_AA_CFG_CTRL_SEL_ACTIVE
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureC1：该配置在这个测量状态分支内生效。
     * - 条件：当 measureData.ADCMeasDiffPre <= D_PRE_DIFF_THRESHOLD_POS 时选择。
     * 赋值方式：通过局部变量 aaCtrlReg 整体写入，LIN->AA_CTRL.reg = ctrlSelActive，再写 ctrlSelActiveStart 作为最后触发。
     * 影响位：
     * - TBIT_START_MEAS[5:0]：AA 电流采样开始的 LIN bit 计数位置。
     * - TBIT_START_ISRC[11:6]：AA 电流源/上拉动作开始的 LIN bit 计数位置。
     * - ISRC_CONF_CTRL[14:12]=3.05mA：AA 电流源幅值配置。
     * - ISRC_EN[15]=1：AA 电流源控制使能。
     * - PU_DISC[16]=1：断开 pull-up 控制位。
     * - PH_START[17]=0：AA phase 启动触发位；START 宏最后写入该位。
     * - ISENSE_MEAS_NUMBER[21:20]=3：电流检测采样次数配置。
     * - ISENSE_EN[22]=1：AA 电流检测使能。
     * - ERR_CODE_CLR[23]=1：AA 错误码清除位。
     * - LIN_AA_EN[24]=1：LIN 自动寻址硬件使能。
     * - LIN_AA_PH[26:25]=3 SEL：AA phase 选择，决定 OFFSET/PRESEL/SEL。
     * 阶段 1：case E_AAMeasureStatusMeasureC1 中判断 PRESEL 结果。
     * 阶段 2：PRE 阈值满足时选择 SEL active 配置。
     * 阶段 3：ctrlSelActiveStart 启动候选节点的 SEL 测量。
     * 意义：执行 active SEL 测量，用于判断本节点是否为最后节点。
     */
    .ctrlSelActive = LIN_AA_CFG_CTRL_SEL_ACTIVE,

    /*
     * 宏：LIN_AA_CFG_CTRL_SEL_INACTIVE
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureC1：该配置在这个测量状态分支内生效。
     * 赋值方式：通过局部变量 aaCtrlReg 整体写入，LIN->AA_CTRL.reg = ctrlSelInactive，再写 ctrlSelInactiveStart 作为最后触发。
     * 影响位：
     * - TBIT_START_MEAS[5:0]：与 SEL active 相同，保持 SEL 采样时刻一致。
     * - TBIT_START_ISRC[11:6]：与 SEL active 相同，保持 SEL 电流源时序一致。
     * - LIN_AA_PH[26:25]=3：选择 SEL phase。
     * - ISRC_CONF_CTRL[14:12]=0mA：inactive 节点不输出 SEL 电流。
     * - PH_START[17]=0：preload 值不启动 phase，等待对应 START 值最后触发。
     * 阶段 1：case E_AAMeasureStatusMeasureC1 中判断 PRESEL 结果。
     * 阶段 2：PRE 阈值不满足时保持 SEL inactive 配置。
     * 阶段 3：ctrlSelInactiveStart 启动 inactive SEL 流程。
     * 意义：保持 SEL 时序一致，但避免非候选节点主动输出 SEL 电流。
     */
    .ctrlSelInactive = LIN_AA_CFG_CTRL_SEL_INACTIVE,

    /*
     * 宏：LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START
     * 使用位置：
     * - LIN_AutoAddr_Meas_Entry()：break 下降沿进入测量窗口阶段，根据节点状态启动 OFFSET active/inactive。
     * - 状态分支 E_StateMachineStatusWaitting：该节点状态下选择此配置。
     * - 写入顺序：紧跟 ctrlOffsetActive 后写入，用作最后触发。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlOffsetActiveStart。
     * 影响位：
     * - 继承配置：TBIT_START_MEAS、TBIT_START_ISRC、ISRC、ISENSE、LIN_AA_PH 等字段与 ctrlOffsetActive 相同。
     * - PH_START[17]=1：最后写入该位，触发 AA 硬件 FSM 启动 OFFSET active phase。
     * - ERR_CODE_CLR[23]=0：START 触发写入时不再执行清错，避免覆盖启动动作。
     * 阶段 1：OFFSET active preload 已写入。
     * 阶段 2：该值作为最后一次写寄存器。
     * 阶段 3：AA 硬件 FSM 启动 OFFSET active phase。
     * 意义：OFFSET active phase 的最终触发值。
     */
    .ctrlOffsetActiveStart = LIN_AA_CFG_CTRL_OFFSET_ACTIVE_START,

    /*
     * 宏：LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START
     * 使用位置：
     * - LIN_AutoAddr_Meas_Entry()：break 下降沿进入测量窗口阶段，根据节点状态启动 OFFSET active/inactive。
     * - 状态分支 E_StateMachineStatusAddressed：该节点状态下选择此配置。
     * - 写入顺序：紧跟 ctrlOffsetInactive 后写入，用作最后触发。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlOffsetInactiveStart。
     * 影响位：
     * - 继承配置：TBIT、ISRC、ISENSE、LIN_AA_PH 等字段与 ctrlOffsetInactive 相同。
     * - PH_START[17]=1：最后写入该位，触发 AA 硬件 FSM 启动 OFFSET inactive phase。
     * - ERR_CODE_CLR[23]=0：START 触发写入时不再执行清错，避免覆盖启动动作。
     * 阶段 1：OFFSET inactive preload 已写入。
     * 阶段 2：该值作为最后一次写寄存器。
     * 阶段 3：AA 硬件 FSM 启动 OFFSET inactive phase。
     * 意义：已寻址节点 inactive 流程的最终触发值。
     */
    .ctrlOffsetInactiveStart = LIN_AA_CFG_CTRL_OFFSET_INACTIVE_START,

    /*
     * 宏：LIN_AA_CFG_CTRL_PRE_ACTIVE_START
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureOffset：该配置在这个测量状态分支内生效。
     * - 写入顺序：紧跟 ctrlPreActive 后写入，用作最后触发。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlPreActiveStart。
     * 影响位：
     * - 继承配置：TBIT_START_MEAS、TBIT_START_ISRC、PRE 电流、ISENSE、LIN_AA_PH 等字段与 ctrlPreActive 相同。
     * - PH_START[17]=1：最后写入该位，触发 AA 硬件 FSM 启动 PRESEL active phase。
     * - ERR_CODE_CLR[23]=0：START 触发写入时不再执行清错，避免覆盖启动动作。
     * 阶段 1：PRESEL active preload 已写入。
     * 阶段 2：该值在 case E_AAMeasureStatusMeasureOffset 内最后写入。
     * 阶段 3：AA 硬件 FSM 启动 PRESEL active phase。
     * 意义：PRESEL phase 的最终触发值。
     */
    .ctrlPreActiveStart = LIN_AA_CFG_CTRL_PRE_ACTIVE_START,

    /*
     * 宏：LIN_AA_CFG_CTRL_SEL_ACTIVE_START
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureC1：该配置在这个测量状态分支内生效。
     * - 写入顺序：紧跟 ctrlSelActive 后写入，用作最后触发。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlSelActiveStart。
     * 影响位：
     * - 继承配置：TBIT_START_MEAS、TBIT_START_ISRC、SEL 电流、ISENSE、LIN_AA_PH 等字段与 ctrlSelActive 相同。
     * - PH_START[17]=1：最后写入该位，触发 AA 硬件 FSM 启动 SEL active phase。
     * - ERR_CODE_CLR[23]=0：START 触发写入时不再执行清错，避免覆盖启动动作。
     * 阶段 1：SEL active preload 已被选择。
     * 阶段 2：该值在 case E_AAMeasureStatusMeasureC1 内最后写入。
     * 阶段 3：AA 硬件 FSM 启动 SEL active phase。
     * 意义：候选节点 SEL 测量的最终触发值。
     */
    .ctrlSelActiveStart = LIN_AA_CFG_CTRL_SEL_ACTIVE_START,

    /*
     * 宏：LIN_AA_CFG_CTRL_SEL_INACTIVE_START
     * 使用位置：
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断，根据当前 case 推进 OFFSET、PRESEL、SEL 阶段。
     * - switch case E_AAMeasureStatusMeasureC1：该配置在这个测量状态分支内生效。
     * - 写入顺序：紧跟 ctrlSelInactive 后写入，用作最后触发。
     * 赋值方式：整体寄存器赋值，LIN->AA_CTRL.reg = ctrlSelInactiveStart。
     * 影响位：
     * - 继承配置：TBIT_START_MEAS、TBIT_START_ISRC、0mA 电流、ISENSE、LIN_AA_PH 等字段与 ctrlSelInactive 相同。
     * - PH_START[17]=1：最后写入该位，触发 AA 硬件 FSM 启动 SEL inactive phase。
     * - ERR_CODE_CLR[23]=0：START 触发写入时不再执行清错，避免覆盖启动动作。
     * 阶段 1：SEL inactive preload 已被选择。
     * 阶段 2：该值在 case E_AAMeasureStatusMeasureC1 内最后写入。
     * 阶段 3：AA 硬件 FSM 启动 SEL inactive phase。
     * 意义：非候选节点 inactive SEL 流程的最终触发值。
     */
    .ctrlSelInactiveStart = LIN_AA_CFG_CTRL_SEL_INACTIVE_START,

    /*
     * 宏：LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS
     * 使用位置：
     * - LIN_AutoAddr_BuildThresholdReg()：构造 thresholdReg，并把该值放入 I_DIFF_TH_1_VAL[11:0]。
     * - LIN_AutoAddr_Start()：自动寻址启动前间接加载 PRE 阈值。
     * - LIN_AutoAddr_Meas_Exit()：一次测量退出后间接重新加载 PRE 阈值。
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断中使用该阈值判断 PRESEL 结果。
     * - switch case E_AAMeasureStatusMeasureC1：PRESEL 判断分支内决定后续 SEL active/inactive。
     * 赋值方式：先组合 thresholdReg，再整体写 LIN->AA_I_DIFF_TH.reg = thresholdReg。
     * 影响位：
     * - I_DIFF_TH_1_VAL[11:0]=preDiffThresholdPos：PRE 电流差阈值字段。
     * - 其他阈值位由组合后的 thresholdReg 决定。
     * 阶段 1：自动寻址开始前和每次测量退出后加载阈值。
     * 阶段 2：case E_AAMeasureStatusMeasureC1 中比较 PRESEL 结果。
     * 阶段 3：比较结果决定进入 SEL active 还是 SEL inactive。
     * 意义：PRE 电流差阈值，用于筛选候选节点。
     */
    .preDiffThresholdPos = LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS,

    /*
     * 宏：LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS
     * 使用位置：
     * - LIN_AutoAddr_BuildThresholdReg()：构造 thresholdReg，并把该值放入 I_DIFF_TH_2_VAL[27:16]。
     * - LIN_AutoAddr_Start()：自动寻址启动前间接加载 SEL 阈值。
     * - LIN_AutoAddr_Meas_Exit()：一次测量退出后间接重新加载 SEL 阈值。
     * - LIN_AutoAddr_Measure_ISR()：AA 测量完成中断中使用该阈值判断 SEL 结果。
     * - switch case E_AAMeasureStatusMeasureC2_S2：SEL 判断分支内决定 isLastNode/count_succ/count_fail。
     * 赋值方式：先组合 thresholdReg，再整体写 LIN->AA_I_DIFF_TH.reg = thresholdReg。
     * 影响位：
     * - I_DIFF_TH_2_VAL[27:16]=selDiffThresholdPos：SEL 电流差阈值字段。
     * - 其他阈值位由组合后的 thresholdReg 决定。
     * 阶段 1：自动寻址开始前和每次测量退出后加载阈值。
     * 阶段 2：case E_AAMeasureStatusMeasureC2_S2 中比较 SEL 结果。
     * 阶段 3：比较结果更新 isLastNode、count_succ、count_fail。
     * 意义：SEL 电流差阈值，用于最后节点判断。
     */
    .selDiffThresholdPos = LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS,

    /*
     * 宏：D_STEP1_CLOSE_ALL_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_ISRC[11:6] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - OFFSET phase 的 LIN->AA_CTRL.TBIT_START_ISRC[11:6]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 OFFSET active 配置。
     * 阶段 2：LIN_AutoAddr_Meas_Entry() 写入生成后的 AA_CTRL 值。
     * 阶段 3：硬件在 OFFSET phase 按该 bit 时刻执行关闭/断开上拉相关动作。
     * 意义：OFFSET 电流源/上拉动作 bit 时刻。
     */
    .step1CloseAllBit = D_STEP1_CLOSE_ALL_BIT,

    /*
     * 宏：D_STEP2_MEAS_OFFSET_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_MEAS[5:0] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - OFFSET phase 的 LIN->AA_CTRL.TBIT_START_MEAS[5:0]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 OFFSET active 配置。
     * 阶段 2：LIN_AutoAddr_Meas_Entry() 写入生成后的 AA_CTRL 值。
     * 阶段 3：硬件在该 bit 时刻采样 offset 电流。
     * 意义：OFFSET 测量 bit 时刻。
     */
    .step2MeasOffsetBit = D_STEP2_MEAS_OFFSET_BIT,

    /*
     * 宏：D_STEP3_OPEN_C1_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_ISRC[11:6] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - PRESEL phase 的 LIN->AA_CTRL.TBIT_START_ISRC[11:6]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 PRESEL active 配置。
     * 阶段 2：case E_AAMeasureStatusMeasureOffset 写入生成后的 AA_CTRL 值。
     * 阶段 3：硬件按该 bit 时刻施加 PRE 电流源动作。
     * 意义：PRESEL 电流源/上拉动作 bit 时刻。
     */
    .step3OpenC1Bit = D_STEP3_OPEN_C1_BIT,

    /*
     * 宏：D_STEP4_MEAS_PRE_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_MEAS[5:0] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - PRESEL phase 的 LIN->AA_CTRL.TBIT_START_MEAS[5:0]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 PRESEL active 配置。
     * 阶段 2：case E_AAMeasureStatusMeasureOffset 写入生成后的 AA_CTRL 值。
     * 阶段 3：硬件在该 bit 时刻采样 PRE 电流。
     * 意义：PRESEL 测量 bit 时刻。
     */
    .step4MeasPreBit = D_STEP4_MEAS_PRE_BIT,

    /*
     * 宏：D_STEP5_OPEN_C2_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_ISRC[11:6] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - SEL phase 的 LIN->AA_CTRL.TBIT_START_ISRC[11:6]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 SEL active/inactive 配置。
     * 阶段 2：case E_AAMeasureStatusMeasureC1 写入选中的 AA_CTRL 值。
     * 阶段 3：硬件按该 bit 时刻施加 SEL 电流源动作。
     * 意义：SEL 电流源/上拉动作 bit 时刻。
     */
    .step5OpenC2Bit = D_STEP5_OPEN_C2_BIT,

    /*
     * 宏：D_STEP6_MEAS_SEL_S1_BIT
     * 使用位置：
     * 赋值方式：本字段不直接写寄存器；通过 TBIT_START_MEAS[5:0] 进入 AA_CTRL 整体寄存器值。
     * 影响位：
     * - SEL phase 的 LIN->AA_CTRL.TBIT_START_MEAS[5:0]。
     * 阶段 1：lin_autoaddressing_cfg.h 中生成 SEL active/inactive 配置。
     * 阶段 2：case E_AAMeasureStatusMeasureC1 写入选中的 AA_CTRL 值。
     * 阶段 3：硬件在该 bit 时刻采样 SEL 电流。
     * 意义：SEL 测量 bit 时刻。
     */
    .step6MeasSelS1Bit = D_STEP6_MEAS_SEL_S1_BIT,

    /*
     * 宏：D_STEP6_MEAS_SEL_S2_BIT
     * 使用位置：
     * 赋值方式：不直接写寄存器，当前没有硬件效果。
     * 影响位：
     * - 当前运行代码无影响位。
     * 阶段 1：预留给可能的 SEL 第二采样点。
     * 阶段 2：当前 switch case 未使用。
     * 阶段 3：当前无 AA 硬件动作。
     * 意义：预留 SEL 第二采样 bit 时刻。
     */
    .step6MeasSelS2Bit = D_STEP6_MEAS_SEL_S2_BIT,

    /*
     * 宏：D_STEP7_RESTORE_BIT
     * 使用位置：
     * 赋值方式：不直接写寄存器，当前没有硬件效果。
     * 影响位：
     * - 当前运行代码无影响位。
     * 阶段 1：预留给可能的自动寻址后 restore 时刻。
     * 阶段 2：当前 switch case 未使用。
     * 阶段 3：当前无 AA 硬件动作。
     * 意义：预留 restore bit 时刻。
     */
    .step7RestoreBit = D_STEP7_RESTORE_BIT
};