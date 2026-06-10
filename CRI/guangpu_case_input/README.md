# 可修改输入

以后如果要测试另一组 RGBW LED，只需要改这里：

1. `R.CSV`
2. `G.CSV`
3. `B.CSV`
4. `W.CSV`
5. `guangpu_case_config.m`

`guangpu_case_config.m` 里主要改这些：

- `cfg.target_xyY`
- `cfg.channel_xyY.R`
- `cfg.channel_xyY.G`
- `cfg.channel_xyY.B`
- `cfg.channel_xyY.W`
- `cfg.single_white_xyY`

说明：

- 如果换了整套 LED，通常 **RGBW 四个通道的光谱和 xyY 都要一起更新**。
- 不只是改白色坐标。因为脚本会用 `R/G/B/W` 的 `xyY` 去校准每个通道的亮度，再反解目标白光的混光比例。
- 只有在 `R/G/B` 三个通道完全没变、只是白光通道换了时，才可以只换 `W.CSV` 和白色相关的坐标。
