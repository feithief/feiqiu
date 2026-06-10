# BLINKY_SIMPLE_EXAMPLE_TLD4020 验证方案

## 验证的是哪一步

这里验证的是 `Algorithm.c` 里“白点分解”这一步。

输入：

- 目标色 `targetColor(x, y, Y)`

输出：

- 白点分量 `splitWhiteCIE`
- 交点分量 `splitPointCIE`
- 比例变量 `splitLambda`
- 比例变量 `splitOneMinusLambda`
- 射线参数 `splitRayT`

## 现在统一使用的工程参数

全部按 `BLINKY_SIMPLE_EXAMPLE_TLD4020` 工程里的固定值来：

```text
savedConfig.redx   = 7038
savedConfig.redy   = 2953
savedConfig.redY   = 7767

savedConfig.greenx = 1792
savedConfig.greeny = 7356
savedConfig.greenY = 19320

savedConfig.bluex  = 1491
savedConfig.bluey  = 333
savedConfig.blueY  = 2253

savedConfig.eolAddr = 22170
savedConfig.whiteY  = 22170
```

除以 `10000` 后就是：

```text
R = (0.7038, 0.2953, 0.7767)
G = (0.1792, 0.7356, 1.9320)
B = (0.1491, 0.0333, 0.2253)
W_Y = 2.2170
```

白点坐标按 `Algorithm.c` 当前固定值：

```text
W = (0.3152, 0.3147)
```

## MATLAB 参考逻辑

现在不再单独使用“额外三角形”。

MATLAB 里直接用这组固定色域三角形做交点求解：

```text
R = (0.7038, 0.2953)
G = (0.1792, 0.7356)
B = (0.1491, 0.0333)
```

也就是说：

- 色域三角形
- 交点求解三角形

现在是同一个三角形。

## 唯一验证流程

只保留这一条流程：

1. 在 VSCode 里编译并运行 `host_gcc_validation/flat_source` 里的 C 文件
2. 运行后生成：

```text
E:\AUDI_COLOR\RGB\host_gcc_validation\flat_source\algorithm_validation_output.csv
```

3. 在 MATLAB 里直接运行：

```matlab
cd('E:\AUDI_COLOR\RGB\c_validation')
verify_blinky_algorithm_split
```

这个 `.m` 文件会自动读取上面那份 CSV，然后和 MATLAB 参考结果对比。

## CSV 需要包含的列

CSV 至少要有这些列：

```text
index
target_x10000, target_y10000, target_Y10000
valid
white_x10000, white_y10000, white_Y10000
point_x10000, point_y10000, point_Y10000
edge_index
lambda_fix16
one_minus_lambda_fix16
ray_t_fix16
```

## MATLAB 输出

`verify_blinky_algorithm_split.m` 会输出 3 张表：

1. `referenceTable`
   - MATLAB 参考结果

2. `compareTable`
   - C 和 MATLAB 的逐项误差

3. `summaryTable`
   - 最大误差和有效点统计

## 算法里要看的比例变量

在 `BLINKY_SIMPLE_EXAMPLE_TLD4020/Algorithm/Algorithm.c` 里，主要看这几个：

- `splitLambda`
  - 交点侧比例 `λ`

- `splitOneMinusLambda`
  - 白点侧比例 `1-λ`

- `splitRayT`
  - 白点射线到交点的位置参数 `t`

另外还有一组“特殊固定比例”，只在这条分支里使用：

```c
if ((inputColor->colorU == 200u) && (inputColor->colorV == 460u))
```

对应变量是：

- `fix16WhiteRgbWhiteRatio`
- `fix16WhiteRgbPointRatio`

这两个不是通用几何求交出来的比例，而是特殊点的固定比例。
