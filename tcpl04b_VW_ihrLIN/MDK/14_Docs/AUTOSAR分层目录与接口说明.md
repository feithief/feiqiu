# TCPL04B LIN Slave AUTOSAR 分层目录与接口说明

## 1. 目标

本工程的目标是保持原 LIN Slave 的帧、信号、诊断和灯光业务行为不变，只重新划分软件所有权。
“目录和接口符合 AUTOSAR 风格”不等于“已经证明功能代码未变”；当前实现状态见第 11 节。

- 芯片相关代码只存在于 MCAL、Device 和 Vendor SDK；
- ASW 不直接调用 DCM、COM、NvM、DEM 等 BSW 模块；
- BSW 不反向包含 ASW 头文件；
- 配置表从运行代码中独立出来；
- 更换 MCU 时，主要替换 `09_MCAL`、`10_Device` 和 `11_Vendor_SDK`。

## 2. 编号目录

编号表示自上而下的调用层级。诊断、通信、存储和故障管理是并列的 BSW 服务，因此各自独立成目录。

| 编号 | 目录 | 所有权 |
|---|---|---|
| 01 | `01_ASW` | 灯光 SWC、通信 SWC、诊断数据 SWC、算法 |
| 02 | `02_RTE` | ASW 与 BSW 之间的唯一业务接口层 |
| 03 | `03_BSW_UDS` | DCM：DSL、DSD、DSP 和诊断配置 |
| 04 | `04_BSW_COM` | COM、PduR、LinTp、LinIf |
| 05 | `05_BSW_NVM` | NvM、MemIf、Fee |
| 06 | `06_BSW_DEM` | Dem 事件状态和事件配置 |
| 07 | `07_BSW_SYSTEM` | EcuM 初始化、BswM 周期调度、SchM 临界区 |
| 08 | `08_BSW_ECUAL` | IoHwAb 设备功能抽象 |
| 09 | `09_MCAL` | Lin、Adc、Fls、Mcu 及 LIN 硬件适配 |
| 10 | `10_Device` | TCPL04B CMSIS、寄存器和启动文件 |
| 11 | `11_Vendor_SDK` | 泰矽微原始 LL/PAL，仅供 MCAL/IoHwAb 使用 |
| 12 | `12_Config` | 板级静态配置头文件 |
| 13 | `13_Common` | `ComStack_Types` 和项目基础类型 |
| 14~16 | Docs/Tests/Tools | 说明、测试和验证工具 |

`11_Vendor_SDK` 中保留厂商原始文件名，是为了保留芯片 SDK 来源；这些文件不属于项目自研 AUTOSAR BSW。项目自研 C 文件全部放在 01~09 的模块目录中。

## 3. 通信调用链

普通信号帧：

```text
LIN 总线
  -> Lin_Hw.c / Lin.c
  -> LinIf.c
  -> PduR.c
  -> Com.c
  -> Rte_Com.c
  -> Swc_Communication.c
```

响应方向完全反向：SWC 写 RTE，RTE 调用 COM，LinIf 在 Header 到达时通过 PduR 触发发送。

面向信号的位位置、位长度、方向和 I-PDU 初始值全部在 `04_BSW_COM/Com_Cfg.c`；`Com.c` 不再使用信号 switch 硬编码。

## 4. 诊断调用链

```text
LIN MRF 0x3C
  -> Lin / LinIf
  -> PduR
  -> LinTp
  -> PduR
  -> Dcm DSL
  -> Dcm DSD
  -> Dcm DSP
  -> Rte_Dcm
  -> Swc_Diagnostic
  -> Rte_NvM / IoHwAb
```

响应方向：

```text
Swc_Diagnostic
  -> Rte_Dcm
  -> Dcm DSP/DSL
  -> PduR
  -> LinTp
  -> PduR
  -> LinIf
  -> Lin SRF 0x3D
```

### DCM 文件职责

| 文件 | 职责 |
|---|---|
| `Dcm.c` | AUTOSAR 对外入口，只转交内部子层 |
| `Dcm_Dsl.c` | 请求/响应缓冲、NAD 元数据、TP Copy 接口、并发状态 |
| `Dcm_Dsd.c` | SID 分发和统一否定响应 |
| `Dcm_Dsp.c` | 0x22、0x2E、B0、B2、B3、B6、B7 服务处理 |
| `Dcm_Cfg.c` | DID 表、NAD 范围、校准 NAD 和节点标识配置 |

工程中已经不存在 `LinDiag.c`。LIN 2.x 节点配置服务 B0/B2/B3/B6/B7 由 DCM DSP 统一处理。

### B0 改 NAD 行为

收到 `01 06 B0 00 00 00 00 A0` 后：

1. DCM DSP 先以初始 NAD 1 生成 `01 01 F0 FF FF FF FF FF`；
2. DCM 当前 NAD 改成 A0；
3. RTE/ASW 取得 NAD 变化并请求 NvM 持久化；
4. 如果主机没有调度旧 SRF 而直接发送新请求，LinTp 取消未调度旧响应，避免后续所有响应错位；
5. 非本节点 NAD 的请求不会取消已经准备好的本节点响应。

## 5. DID 配置

DID 的访问权限、NAD 权限和长度只在 `03_BSW_UDS/Dcm_Cfg.c` 配置；DID 对应的业务数据由
`01_ASW/03_Swc_Diagnostic/Swc_Diagnostic.c` 提供。DCM 不保存 ASW 结构偏移或 NvM 布局。

| DID | 长度 | 访问 | NAD | 数据含义 |
|---|---:|---|---|---|
| 0002 | 2 | 读写 | A0 | LED supplier |
| 0003 | 2 | 读写 | A0 | application address；写入后同步 LIN NAD |
| 0004 | 2 | 读写 | A0 | group address mask |
| 0005 | 2 | 读写 | A0 | platform |
| 0006 | 2 | 读写 | A0 | intensity factor |
| 0007 | 8 | 读写 | A0 | Red CIE 校准 |
| 0008 | 8 | 读写 | A0 | Green CIE 校准 |
| 0009 | 8 | 读写 | A0 | Blue CIE 校准 |
| 000A | 1 | 写 | A0 | 校准灯控制/采样保存 |
| 0100 | 16 | 读 | 普通/A0 | 调试采样快照 |
| 6269 | 11 | 读写 | A0 | 零件号 |
| 6469 | 5 | 读 | A0 | 软件版本 |
| 6869 | 3 | 读写 | A0 | 硬件版本 |
| 6A69 | 20 | 读写 | A0 | 序列号；LinTp 多帧响应 |

DCM 配置不再保存 NvM 结构偏移。`Dcm_Cfg.c` 只提供逻辑数据 ID；`Swc_Diagnostic.c` 在应用层解析这些 ID，因此以后替换 NvM 块布局时无需修改 DCM。

## 6. 存储调用链

```text
ASW
  -> Rte_NvM
  -> NvM
  -> MemIf
  -> Fee
  -> Fls
  -> pal_store / 芯片 Flash
```

- `NvM_Cfg.c`：唯一的块描述、ROM 默认值、版本校验和 V2 到 V3 迁移；
- `NvM.c`：块状态、RAM block、异步写请求；
- `MemIf.c`：存储抽象路由；
- `Fee.c`：逻辑块到 Flash 地址的映射；
- `Fls.c`：MCAL Flash 接口。

原 `ModuleFlash.c` 已删除。

## 7. 故障管理

ASW 通过 `Rte_Dem` 报告和读取事件，不直接调用 Dem。事件 ID、失败阈值和恢复阈值在 `06_BSW_DEM/Dem_Cfg.c` 配置。目前配置了电压、LED 输出和 NvM 存储三个事件扩展点。

## 8. 配置文件规则

本单通道 LIN Slave 不把每个 AUTOSAR 模块都做成一份可编辑配置。实际入口为：

- 新 LDF：`04_BSW_COM/Com_Cfg.c`，集中放信号位、I-PDU 初始值、帧表和波特率；
- DID/NAD/节点：`03_BSW_UDS/Dcm_Cfg.c`；
- DID 业务数据：`01_ASW/03_Swc_Diagnostic/Swc_Diagnostic.c`；
- DID 改变持久化结构时才修改 `05_BSW_NVM/NvM_Cfg.c`；
- Dem 事件：`06_BSW_DEM/Dem_Cfg.c`。

固定拓扑的 PduR 直接路由，不存在 `PduR_Cfg.*`；LinTp 不存在项目级 `LinTp_Cfg.*`；
LinIf 帧表和 LIN 参数已经合并到 `Com_Cfg.c`，不再存在 `LinIf_Cfg.*` 或 `Lin_Cfg.*`。
详细操作见 `14_Docs/lin-autosar-minimal-config/SKILL.md`。

## 9. 换芯片时修改范围

正常情况下只替换：

1. `09_MCAL/Lin_Hw.c`：实现 `Lin_Driver_*`；
2. `09_MCAL/Adc.c`、`Fls.c`、`Mcu.c`；
3. `10_Device` 和 `11_Vendor_SDK`；
4. `12_Config` 中的板级地址、引脚和时钟配置。

不得修改 `Dcm`、`LinTp`、`PduR`、`Com`、RTE 或 ASW 来适配新寄存器。

## 10. 验证

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\16_Tools\verify.ps1 -Build
```

脚本检查目录、禁止模块、B0~B7 归属、14 个 DID、ASW/BSW 反向依赖、Keil 文件清单，并编译 `swsc_normal` 和 `swsc_match_boot`。

## 11. IHR 功能代码审计结论

当前 `09_MCAL/Lin.c` 和 `04_BSW_COM/LinTp.c` 是重新实现的 LIN/LIN TP 状态机，并非原
IHR 协议实现的逐字保留或薄包装。因此现在只能确认 AUTOSAR 分层与编译通过，不能宣称
“原 IHR 功能代码未修改”。相邻能工作的 TCPL04B 基线仍保留 `linlib.lib`、`lin_cfg.c`、
`LinLowLevel.c`、`LinStackService.c` 和 `LinDiagnosticService.c`，这才是后续恢复功能基线时
应对照的实现。

正确的收口方式是：保留 IHR 协议状态机/库作为 CDD 或 Vendor Stack；只在其外部提供
COM、LinTp、PduR、DCM 和 RTE 接口；芯片操作只进入 `Lin_Hw.c`。在完成该恢复及台架回归前，
不得用当前新状态机替代 IHR 后声称功能等价。
