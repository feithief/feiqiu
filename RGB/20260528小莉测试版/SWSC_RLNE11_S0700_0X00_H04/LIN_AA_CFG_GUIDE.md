# LIN 自动寻址配置说明

## 1. 修改位置

只改：

`Config/lin_autoaddressing_cfg.h`

改完直接 `Rebuild`，输出：

`Objects/SWSC_Application.hex`

## 2. T2 计时器配置

| 宏 | 寄存器 | 当前值 | 含义 |
|---|---|---:|---|
| `LIN_AA_CFG_T2_CON_EDGE` | `LIN->T2_CON` | `0x09` | `CP_RL2=1` 捕获模式，`EXEN2=1` 外部事件使能 |
| `LIN_AA_CFG_T2_MOD_FALLING_EDGE` | `LIN->T2_MOD` | `0x10` | `PREN=1`，`EDGESEL=0`，检测下降沿 |
| `LIN_AA_CFG_T2_MOD_RISING_EDGE` | `LIN->T2_MOD` | `0x30` | `PREN=1`，`EDGESEL=1`，检测上升沿 |

## 3. ADC 恢复配置

| 宏 | 寄存器 | 当前值 | 含义 |
|---|---|---:|---|
| `LIN_AA_CFG_ADC_CFG0_REINIT` | `ADC11->CFG_0` | `0x00300004` | `ADC_OVERS_CFG=3`，8 次采样；`ADC_COMP_VAL=4` |
| `LIN_AA_CFG_ADC_IEN_REINIT` | `ADC11->IEN` | `0x00000001` | `ADC_FW_EOC=1`，ADC 完成中断使能 |
| `LIN_AA_CFG_ADC_CFG1_REINIT` | `ADC11->CFG_1` | `0x00000100` | `ADC_ENABLE=1`，ADC 使能 |

## 4. AA_CTRL 字段

`LIN_AA_CFG_CTRL_BUILD()` 最终生成 `LIN->AA_CTRL`：

| 参数 | AA_CTRL 字段 |
|---|---|
| `tbit_meas` | `TBIT_START_MEAS[5:0]` |
| `tbit_isrc` | `TBIT_START_ISRC[11:6]` |
| `isrc_conf` | `ISRC_CONF_CTRL[14:12]` |
| `isrc_en` | `ISRC_EN[15]` |
| `pu_disc` | `PU_DISC[16]` |
| `ph_start` | `PH_START[17]` |
| `ph_stop` | `PH_STOP[18]` |
| `fsm_rst` | `FSM_RST[19]` |
| `meas_num` | `ISENSE_MEAS_NUMBER[21:20]`，`3` 表示 8 次测量 |
| `isense_en` | `ISENSE_EN[22]` |
| `err_clear` | `ERR_CODE_CLR[23]` |
| `aa_en` | `LIN_AA_EN[24]` |
| `aa_ph` | `LIN_AA_PH[26:25]`，`1=OFFSET`，`2=PRESEL`，`3=SEL` |

## 5. Step 配置

`TBIT_START_MEAS/ISRC = (D_STEPx << 1) | position`

| 宏 | 用途 |
|---|---|
| `LIN_AA_USE_DELAYED_STEP_TIMING` | 选择 delayed 默认 step 表 |
| `D_STEP1_CLOSE_ALL_BIT` | OFFSET 阶段电流源/上拉动作时刻 |
| `D_STEP2_MEAS_OFFSET_BIT` | OFFSET 测量时刻 |
| `D_STEP3_OPEN_C1_BIT` | PRESEL 阶段电流源/上拉动作时刻 |
| `D_STEP4_MEAS_PRE_BIT` | PRESEL 测量时刻 |
| `D_STEP5_OPEN_C2_BIT` | SEL 阶段电流源/上拉动作时刻 |
| `D_STEP6_MEAS_SEL_S1_BIT` | SEL 测量时刻 |
| `D_STEP6_MEAS_SEL_S2_BIT` | 预留 step，当前库未写入 AA_CTRL |
| `D_STEP7_RESTORE_BIT` | 预留 restore step，当前库未写入 AA_CTRL |

## 6. AA phase 宏

| 宏 | 寄存器 | 作用 |
|---|---|---|
| `LIN_AA_CFG_CTRL_FSM_RESET_ONLY` | `LIN->AA_CTRL` | `FSM_RST=1`，复位 AA FSM |
| `LIN_AA_CFG_CTRL_END_PHASE` | `LIN->AA_CTRL` | `ERR_CODE_CLR=1`，结束/清错误 |
| `LIN_AA_CFG_CTRL_OFFSET_ACTIVE` | `LIN->AA_CTRL` | OFFSET 阶段，测 offset，0mA |
| `LIN_AA_CFG_CTRL_OFFSET_INACTIVE` | `LIN->AA_CTRL` | OFFSET inactive，关闭测量 |
| `LIN_AA_CFG_CTRL_PRE_ACTIVE` | `LIN->AA_CTRL` | PRESEL 阶段，0.45mA，8 次测量 |
| `LIN_AA_CFG_CTRL_SEL_ACTIVE` | `LIN->AA_CTRL` | SEL 阶段，3.05mA，8 次测量 |
| `LIN_AA_CFG_CTRL_SEL_INACTIVE` | `LIN->AA_CTRL` | SEL 阶段，0mA，8 次测量 |
| `*_START` | `LIN->AA_CTRL` | 与对应 phase 相同，但 `PH_START=1`，最后写入触发硬件 FSM |

库的写法是：

1. 先写普通 phase 宏，预装配置。
2. 再写对应 `*_START` 宏，只把 `PH_START` 作为最后触发。

## 7. 阈值

| 宏 | 寄存器 | 字段 |
|---|---|---|
| `LIN_AA_CFG_PRE_DIFF_THRESHOLD_POS` | `LIN->AA_I_DIFF_TH` | `I_DIFF_TH_1_VAL[11:0]` |
| `LIN_AA_CFG_SEL_DIFF_THRESHOLD_POS` | `LIN->AA_I_DIFF_TH` | `I_DIFF_TH_2_VAL[27:16]` |

## 8. 使用原则

- 想调自动寻址时序，优先改 `D_STEPx_*_BIT` 和 `D_ADC_MEASURE_POSITION_*`。
- 想调电流，改 `D_PRE_CURRENT_SETTING` 或 `D_SEL_CURRENT_SETTING`。
- 想调成功率判定，改两个 threshold。
- 不要重排 `LinAutoAddr_Config_t` 字段；这是和 `SWSC_Core.lib` 对接的 ABI。
- 修改注释不会改变 HEX；修改未被库使用的宏也不会改变硬件行为。
