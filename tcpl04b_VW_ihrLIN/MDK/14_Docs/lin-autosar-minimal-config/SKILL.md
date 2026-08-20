---
name: lin-autosar-minimal-config
description: 为本 TCPL04B LIN Slave AUTOSAR 风格工程导入新 LDF、修改帧/信号映射或增删诊断 DID，并把改动限制在最小配置入口。用于用户要求保留原 IHR LIN 协议功能代码、不增加 PduR/LinTp 配置、完成 Keil 编译和台架核对的场景。
---

# LIN AUTOSAR 最小配置

## 强制边界

- 只在 `tcpl04b_swsc_lean_ihrLIN - 副本` 内工作。
- 先保存旧工程、LDF 与构建产物的基线，不得把编译通过等同于功能等价。
- 不改 IHR 的 LIN 帧状态机、诊断传输状态机、超时顺序和中断处理逻辑。
- PduR 为单通道固定直连，不创建 `PduR_Cfg.*`。
- LinTp 不暴露工程配置，不创建 `LinTp_Cfg.*`。
- 新 MCU 只允许通过 `09_MCAL/Lin_Hw.c` 的 `Lin_Driver_*` 接口适配。

## 导入新 LDF

1. 解析 LDF 的波特率、从节点发布/订阅帧、Frame ID、长度、校验模型、信号位位置、位长、方向和初始值。
2. 如果信号语义和数量不变，只编辑 `04_BSW_COM/Com_Cfg.c`：
   - `Com_SignalConfig`
   - `Com_RxIpduInit` / `Com_TxIpduInit`
   - `LinIf_FrameConfig`
   - `Lin_Config.Baudrate`
3. 保持诊断帧 MRF/SRF 为 0x3C/0x3D、8 字节、经典校验；除非新 LDF 明确取消诊断。
4. RGB 核心语义固定使用 `Rte_LightCommandType` 的 `red/green/blue/brightness/dimming_time`。如果 LDF 只改变信号名，只修改 `02_RTE/Rte_Com.c` 的 Signal ID 映射，不修改 SWC。
5. 只有新增/删除业务语义时，才同步编辑 `02_RTE/Rte_Com.*` 端口和对应 SWC。不要让 ASW 直接操作帧缓冲区，也不要把 LDF/OEM 信号名带入 ASW。
6. 不因 LDF 变化修改 `PduR.c` 或 `LinTp.c`。只有新增第二条逻辑路由或第二通道时，才重新评估是否需要路由配置。

## 增删 DID

1. 在 `03_BSW_UDS/Dcm_Cfg.c` 的 `s_didTable` 增删一行：DID、读写权限、允许 NAD、准确长度。
2. 在 `01_ASW/03_Swc_Diagnostic/Swc_Diagnostic.c` 按 DID 增删数据处理：
   - 普通持久化字段放入 `Swc_Diagnostic_ResolveConfigData`；
   - 计算型或动态 DID 放入 `Swc_Diagnostic_ReadData`；
   - 有副作用的写 DID 放入 `Swc_Diagnostic_WriteData`。
3. 仅当新增 DID 改变持久化结构时，才修改 `NvM_Cfg.c` 的 ROM default、版本和迁移；只读动态 DID 不修改 NvM。
4. 删除 DID 时，先删 DCM 表行使其不可访问，再清理无用业务分支；不得修改 DCM DSL/DSD/DSP 或 LinTp。

## 配置范围判定

| 变化 | 必改 | 通常不改 |
|---|---|---|
| PID、信号位、波特率变化 | `Com_Cfg.c` | PduR、LinTp、DCM |
| LDF 信号仅改名、RGB 语义不变 | `Com_Cfg.c/.h`、`Rte_Com.c` 映射 | ASW、PduR、LinTp、DCM |
| 信号业务语义新增或删除 | `Com_Cfg.c/.h`、RTE/对应 SWC | LinTp、DCM |
| DID 新增或删除 | `Dcm_Cfg.c`、`Swc_Diagnostic.c` | PduR、LinTp、LinIf、Lin MCAL |
| DID 持久化结构变化 | 上述文件 + `NvM_Cfg.c` 迁移 | PduR、LinTp |
| 更换 MCU | `Lin_Hw.c` 和厂商 SDK | COM、PduR、LinTp、DCM、ASW |

## 验证

执行：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\16_Tools\verify.ps1 -Build
```

必须同时满足：两个 Keil Target 为 0 Error/0 Warning；LDF 帧的 PID/方向/校验/信号字节一致；0x22/0x2E 单帧及多帧 DID 正确；B0/B2/B3/B6/B7 与原 IHR 基线一致；换 NAD 后按真实主机调度验证响应地址。

如当前 `Lin.c` 或 `LinTp.c` 不是原 IHR 状态机的无行为变更包装，停止宣称“功能保持不变”，先恢复 IHR 基线或提供逐项回归证据。
