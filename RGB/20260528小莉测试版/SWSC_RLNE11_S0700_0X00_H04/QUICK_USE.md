# 极简使用说明

## 1. 编译

- 打开 `SWSC_Application.uvprojx`，直接 `Rebuild`。
- 或运行 `build_all.cmd`。
- 输出 HEX：`Objects/SWSC_Application.hex`。

## 2. 自动寻址配置

只改这里：

`Config/lin_autoaddressing_cfg.h`

可配置内容：

- `LIN_AA_USE_DELAYED_STEP_TIMING`
- `D_STEP1_CLOSE_ALL_BIT` 到 `D_STEP7_RESTORE_BIT`
- T2 配置
- ADC restore 配置
- AA phase register 配置
- threshold 配置

改完直接 `Rebuild`，会生效到 HEX。

## 3. 底层 Driver

`RTE/Device/TLD4020-3ET/*.c` 没有封库。

修改这些底层 driver 后，`Rebuild` 会生效到 HEX。

## 4. 封库部分

`SWSC_Core.lib` 已封装：

`System / Protocol / modules / Hal / Generated / dimming / App / Algorithm / AA`

这些源码不参与应用 rebuild。

所以不要指望修改封库内部实现宏后自动生效。

## 5. 判断是否生效

- 改 `Config/lin_autoaddressing_cfg.h`：应生效。
- 改 `RTE/Device` 的 `.c/.h`：通常生效。
- 只改注释：HEX 不变是正常的。
- 改没有被使用的宏：HEX 不变是正常的。
- 改封库内部实现代码：不会生效。

## 6. 交付规则

交付工程只需要：

- `SWSC_Application.uvprojx`
- `Config`
- `RTE`
- `PublicInclude`
- `Library/Objects/SWSC_Core.lib`
- `build_all.cmd`

不要交付封库源码。
