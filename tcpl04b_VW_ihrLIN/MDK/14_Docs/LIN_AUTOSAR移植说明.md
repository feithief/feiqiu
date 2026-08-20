# LIN AUTOSAR 移植说明

LIN Slave 的最新移植边界、MCAL 接口、RTE 上层接口和配置文件位置统一见
[AUTOSAR分层目录与接口说明.md](./AUTOSAR分层目录与接口说明.md)。

当前 AUTOSAR 接口调用链为：

```text
ASW -> RTE -> Dcm/Com -> PduR -> LinTp/LinIf -> Lin MCAL -> Vendor SDK
```

旧的 `LinDiag.c`、`Lin_UpperInterface.c` 和 `PduR_Lin.c` 已删除。

注意：当前 `Lin.c/LinTp.c` 是新状态机，不是原 IHR 功能代码的薄包装。详细审计和
新 LDF/DID 最小配置流程见 `lin-autosar-minimal-config/SKILL.md`。
