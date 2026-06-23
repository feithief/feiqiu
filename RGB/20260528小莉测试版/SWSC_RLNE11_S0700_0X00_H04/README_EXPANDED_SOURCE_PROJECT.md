# Expanded Source Project

这个工程是 `21us_library_project` 的库展开版。

## 关键点

- `SWSC_Core.lib` 不参与链接。
- `System / Protocol / modules / Hal / Generated / dimming / App / Algorithm / AA` 源码已放回工程并参与编译。
- `Config/lin_autoaddressing_cfg.c` 仍然保留并参与编译。
- `AA/lin_autoaddressing.c` 通过 `g_linAutoAddrConfig` 使用外部配置，和封库工程内部逻辑一致。

## 编译

打开：

`SWSC_Application.uvprojx`

直接 `Rebuild`。

或者运行：

`build_all.cmd`

输出：

`Objects/SWSC_Application.hex`

## 已验证

当前 HEX 与封库工程完全一致：

`21us_library_project/SWSC_RLNE11_S0700_0X00_H04/Objects/SWSC_Application.hex`

SHA256：

`4C090EA85129DDAC10A2C05765E7FCE9C39874142DD38D4846E82A297BC10672`

Build result：

`0 Error(s), 0 Warning(s)`
