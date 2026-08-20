# LIN AUTOSAR 诊断与 DID

最新的 DCM DSL/DSD/DSP 分层、B0~B7 归属、14 个 DID 表和 NvM 持久化链见
[AUTOSAR分层目录与接口说明.md](./AUTOSAR分层目录与接口说明.md)。

DID 权限、NAD 和长度的唯一配置位置是 `03_BSW_UDS/Dcm_Cfg.c`；应用数据处理位置是
`01_ASW/03_Swc_Diagnostic/Swc_Diagnostic.c`。

PduR 和 LinTp 对增删 DID 均不需要配置。具体步骤见
`lin-autosar-minimal-config/SKILL.md`。
