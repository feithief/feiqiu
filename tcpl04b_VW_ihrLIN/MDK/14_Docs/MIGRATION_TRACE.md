# AUTOSAR 重构迁移追踪

本文件只记录现工程有效的模块归属。完整接口、调用链和换芯片说明见
`AUTOSAR分层目录与接口说明.md`。

## 模块迁移关系

| 原实现职责 | 当前 AUTOSAR 模块 | 当前目录 | 说明 |
|---|---|---|---|
| 产品主循环与启动 | EcuM、BswM、SWC | `07_BSW_SYSTEM`、`01_ASW` | EcuM 初始化，BswM 调度 BSW，SWC 只负责业务 |
| LIN 信号读写 | COM、RTE | `04_BSW_COM`、`02_RTE` | ASW 通过 RTE 面向信号编程 |
| LIN 帧收发 | LinIf、Lin | `04_BSW_COM`、`09_MCAL` | LinIf 不访问寄存器，芯片差异收口到 Lin MCAL |
| LIN 诊断传输 | LinTp、PduR | `04_BSW_COM` | PduR 固定直连；当前 LinTp 是新实现，尚未证明与 IHR 等价 |
| B0/B2/B3/B6/B7 与 UDS | DCM DSL/DSD/DSP | `03_BSW_UDS` | 不再存在独立 LinDiag 模块 |
| DID 数据提供 | SWC Diagnostic、RTE | `01_ASW/03_Swc_Diagnostic`、`02_RTE` | DCM 不直接访问业务变量或 Flash |
| 配置持久化 | NvM、MemIf、Fee、Fls | `05_BSW_NVM`、`09_MCAL` | Flash 地址及芯片操作仅在 Fls/供应商适配层出现 |
| 故障事件 | Dem | `06_BSW_DEM` | ASW 经 Rte_Dem 报告事件，不直接依赖 Dem |
| ADC/PWM/温度/看门狗 | IoHwAb、MCAL | `08_BSW_ECUAL`、`09_MCAL` | 硬件抽象与寄存器访问分离 |

## 已删除的旧模块边界

- 独立 LIN 诊断处理文件；节点配置服务已经并入 DCM DSP。
- 上层直接读写 LIN 帧或 MCAL 的适配文件；上层统一经过 RTE/COM。
- 业务层直接操作 Flash 的模块；存储统一走 NvM/MemIf/Fee/Fls。
- 旧的 `System`、`LinService` 和 `Module*` 工程分组。

## 当前功能代码审计

当前 `Lin.c` 与 `LinTp.c` 不是原 IHR 实现的薄包装，而是新的状态机实现。目录归属和接口
隔离已经成立，但“未修改 IHR 功能代码”尚未成立。后续必须恢复相邻 TCPL04B 基线中的
IHR `linlib.lib` 及其生成配置/低层链路，再由 AUTOSAR 外壳调用；未完成台架回归前不得把
下面的编译结果当成功能等价证明。

## 验证结果

- `swsc_normal`：0 Error / 0 Warning。
- `swsc_match_boot`：0 Error / 0 Warning。
- LIN 主机测试覆盖 B0 攜带 A0 新 NAD、0x22/0x2E、长 DID 分段和响应生命周期。
- `16_Tools/verify.ps1` 校验目录、模块归属、配置隔离和两个 Keil Target。

编译通过不能替代台架验证。板级仍需确认 LIN 调度、NAD 切换、DID 数据、掉电保存、
PWM/ADC 和 Bootloader 跳转。
