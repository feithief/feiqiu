# ADC 驱动分层说明

ADC 在当前工程中的有效调用链为：

`SWC -> IoHwAb_Adc -> Adc(MCAL) -> TCPL04B Vendor SDK -> ADC 寄存器`

## 文件职责

- `08_BSW_ECUAL/IoHwAb_Adc.c`：周期采样协调、工程量缓存和有效状态；不访问寄存器。
- `09_MCAL/Adc.c`：TCPL04B ADC 初始化、同步采样、Trim 及整数工程量换算。
- `11_Vendor_SDK/tcpl04x_ll_adc.c`：供应商寄存器级驱动，保留原厂命名以便追溯。

## 分层约束

- ASW 不包含 `Adc.h`，只通过 IoHwAb 获取电压和温度。
- IoHwAb 不包含供应商 LL 头文件。
- 寄存器、时钟、通道和 Trim 差异只允许出现在 MCAL/供应商层。
- 更换 MCU 时，保持 IoHwAb 接口不变，只重做 `Adc.c` 及其下层适配。

当前实现采用同步轮询，不使用 ADC 中断或 DMA。初始化顺序仍遵守：打开外设时钟、
复位/停止、配置模拟输入与通道、清空结果 FIFO、允许软件触发，运行时再触发并读取结果。
