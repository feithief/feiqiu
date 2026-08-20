# TCPL04B LIN Slave AUTOSAR 工程

当前有效的软件架构说明只有：

- `AUTOSAR分层目录与接口说明.md`：目录、调用链、接口、DID、配置与移植范围；
- `ADC驱动框架说明.md`：ADC MCAL/IoHwAb 专项说明；
- `RGB应用层移植说明.md`：灯光算法和应用行为说明。

工程入口：`MDK/tcpl04b_swsc_lean.uvprojx`。

验证命令：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\16_Tools\verify.ps1 -Build
```

当前已验证：

- `swsc_normal`：0 Error，0 Warning；
- `swsc_match_boot`：0 Error，0 Warning；
- LIN Slave 的普通信号、B0→A0、0x22、0x2E、20 字节多帧 DID、未调度旧响应取消和外部 NAD 隔离主机测试通过。
