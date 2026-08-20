# RGB 应用层统一接口说明

## 1. 目标与边界

应用层不再使用 LDF/OEM 命名，例如 `BCM_RGB_Rot`、`Intensitaet`、`Dimmrampe`。这些名字只允许出现在 `04_BSW_COM/Com_Cfg.*` 生成配置以及 `02_RTE/Rte_Com.c` 的语义映射中。

稳定的数据流为：

`LDF信号 -> COM解包 -> Rte_Com.c语义映射 -> Rte_LightCommandType -> RGB应用 -> 光学算法 -> PWM`

本轮只统一 RGB 核心命令，不处理项目特有的特殊信号，也不修改 IHR、LinIf、PduR、COM 打包算法或 MCAL。

## 2. 应用层固定命名

统一类型定义在 `02_RTE/Rte_Types.h`：`Rte_LightCommandType`。

| 固定字段 | 含义 | 当前 LDF 映射 |
|---|---|---|
| `red` | 红色目标值 | `COM_SIG_RX_RED` |
| `green` | 绿色目标值 | `COM_SIG_RX_GREEN` |
| `blue` | 蓝色目标值 | `COM_SIG_RX_BLUE` |
| `brightness` | 亮度，当前有效范围 0～100 | `COM_SIG_RX_INTENSITY` |
| `brightness_normalized` | 亮度归一化属性 | `COM_SIG_RX_NORMALIZED_INTENSITY` |
| `dimming_time` | LDF 调光时间/斜坡编码值 | `COM_SIG_RX_DIM_RAMP` |
| `dimming_time_relative` | 相对/绝对调光时间属性 | `COM_SIG_RX_RELATIVE_DIM_TIME` |
| `dimming_time_base_250ms` | 50 ms/250 ms 时间基准 | `COM_SIG_RX_TIME_BASE_250MS` |

`sysLin` 直接使用同一个 `Rte_LightCommandType`，因此 RTE 命令与应用运行数据不再维护两套字段名称。

地址、寻址方式、有效位和工作模式暂时保留在该类型中；它们属于本项目现有行为，本轮不继续抽象。

## 3. 新 RGB LDF 的最小改动

1. 用 `16_Tools/LdfConfigGenerator` 生成并应用新的 `Com_Cfg.c/.h`。
2. 如果新的 LDF 只是改了信号名称，在 `02_RTE/Rte_Com.c` 把新的 `COM_SIG_RX_*` 分别映射到上述固定字段。
3. 不修改 `Rte_Types.h`、`Swc_Communication.c`、`Swc_LightControl.c` 和 RGB 光学算法。
4. 只有信号物理语义、范围或缩放真正变化时，才评估应用行为变更。

也就是说，新 RGB 项目允许修改的是“LDF 名称到固定字段”的一处映射，而不是整套应用。

## 4. 保留的功能行为

- RGB 全零时保留上一次颜色，允许仅把亮度降为零后恢复原色。
- `dimming_time × 时间基准` 的时长算法保持不变。
- 相对/绝对调光对亮度时长的处理保持不变。
- 地址过滤、有效位、工作模式、温度降额、故障关灯及状态反馈保持不变。

## 5. 最小验证

1. 编译 `swsc_normal` 与 `swsc_match_boot`，要求 0 Error、0 Warning。
2. 单独发送纯红、纯绿、纯蓝，核对三路 PWM 顺序。
3. 测试亮度 0、1、50、100。
4. 测试两种时间基准以及相对/绝对调光。
5. 在 CANoe 中核对 PID、DLC、方向、checksum 与原始八字节数据。
