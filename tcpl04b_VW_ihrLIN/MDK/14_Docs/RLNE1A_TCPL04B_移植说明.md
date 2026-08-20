# RLNE1A 轻量 IHR 工程移植到 TCPL04B

## 移植范围

- `01_ASW`：采用 RLNE1A 的灯控、状态、降额、通信信号处理、算法和 DID 应用逻辑。
- `03_BSW_UDS`：采用 RLNE1A 的轻量 DCM、DID 配置和 IHR LinTp。
- `04_BSW_COM`：采用 RLNE1A 的 IHR Com、LinIf 和生成配置。
- `05_BSW_NVM`：保留 RLNE1A 的 `savedConfig` 数据模型和默认值，通过 TCPL04B Fls 写入保留扇区 `0x0000BC00`。
- `07_BSW_SYSTEM`：保留 RLNE1A 的逻辑休眠接口；物理低功耗进入仍由 TCPL04B MCAL 负责。
- `08_BSW_ECUAL`：向上保持原 `moduleLed/moduleTemp/moduleBat/moduleClock/moduleWatchdog` 接口，向下只调用 MCAL。
- `09_MCAL`、`10_Device`、`11_Vendor_SDK`：使用 TCPL04B 驱动、启动代码和 SDK。

旧 RTE、DEM、PduR、通用 AUTOSAR DCM/COM/NvM 实现以及第二套 Vendor IHR 头文件已从工程和目录中清除。

## 初始化和调用链

```text
TcMain
  -> systemInit                         (RLNE1A ASW)
     -> moduleFlash/Adc/Temp/Led/...    (ECUAL 兼容接口)
     -> l_sys_init                      (IHR LinIf)
        -> lin_hal_init                 (TCPL04B Lin_Hw)
  -> systemMainLoop                     (RLNE1A ASW)
     -> LIN_Task -> Dcm_MainFunction
     -> 信号读取/灯控算法/响应信号写入
```

## 换芯片边界

换芯片时只需要替换 `09_MCAL` 和芯片 SDK/启动文件：

- LIN：`lin_hal_*`、`Lin_Driver_EnterCritical/ExitCritical`
- ADC：`Adc_*`
- LED/PWM：`Led_*`
- Flash：`Fls_*`
- Watchdog：`Wdg_*`
- 时基/中断：`Mcu_*`

ASW、DCM/LinTp、Com/LinIf 和 `module*` 应用接口不接触寄存器或 Vendor PAL。

## 验证结果

- `swsc_normal`：`Code=26212, RO=1192, RW=76, ZI=1732`，0 Error，0 Warning。
- `swsc_match_boot`：`Code=23296, RO=1168, RW=76, ZI=1732`，0 Error，0 Warning。
- RLNE1A 的 `03_BSW_UDS` 与 `04_BSW_COM` 文件逐文件 SHA-256 一致。
- Slave 配置为 `LIN_SLAVE + LIN_PROTOCOL_VERSION_2_0 + LIN_COOKED_API`；链接 MAP 中没有 Master schedule/header 符号。

## 上板注意

当前完成的是源码迁移、边界收口和全量编译验证。首次上板建议依次确认：

1. `LinIf_Init.c` 调到 `lin_hal_init()`。
2. `Lin_Hw.c` 收到 PID 和 8 字节数据。
3. `systemLinCommSignalUpdate()` 返回更新。
4. `moduleLedUpdatePWM()` 到达 `Led_SetDuty()`。
5. 诊断请求依次进入 `LinTp.c -> Dcm.c -> Swc_Diagnostic.c`。

物理 MCU 深度休眠没有在本次迁移中启用；逻辑休眠、LED 关闭和 LIN 接收保持有效，待板级唤醒源确认后只在 MCAL 中补充。
