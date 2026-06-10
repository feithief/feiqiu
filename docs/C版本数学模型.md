# C 版本数学模型

## 整体思路

C 版本承担“低成本在线近似求解”的数学任务，对应整体模型的后两层。

第一，在线获得全色域连续比例。C 版本不重新计算光谱显色指数，而是使用基础最优比例场的压缩表示，通过邻域搜索与欧式范数加权局部回归得到目标点的 W<sub>25</sub>。该近似成立的前提是：基础白光比例函数在局部色域内连续，因而邻域样本能够稳定预测目标点比例。

<p align="center"><font face="Times New Roman">ρ<sup>*</sup>: 𝒢 → [0,1], &nbsp; ρ<sup>*</sup> ∈ C<sup>0</sup>(𝒢)</font></p>

第二，在线考虑温度后的最大 Ra 近似。C 版本使用温度曲面系数计算 ΔW，得到 W<sub>formula</sub>；再通过可行域边界 W<sub>max</sub> 限制，得到 W<sub>final</sub>；最后完成 RGBW 分解。该近似成立的前提是：温度引起的白光比例变化量 Δρ 连续，且可由目标色坐标、基础比例和温度的低阶曲面表达。

<p align="center"><font face="Times New Roman">Δρ: 𝒢 × [0,1] × Θ → ℝ, &nbsp; Δρ ∈ C<sup>0</sup>(𝒢 × [0,1] × Θ)</font></p>

<p align="center"><font face="Times New Roman">Z<sub>0</sub> → 𝒩 → W<sub>25</sub> → W<sub>formula</sub> → W<sub>final</sub> → d<sub>RGBW</sub></font></p>

## 符号、函数与算子

| 符号 | 含义 | 符号 | 含义 | 符号 | 含义 |
|---|---|---|---|---|---|
| Z<sub>0</sub> | 在线目标点 | Z<sub>n</sub> | 邻域样本点 | 𝒩 | 邻域样本集合 |
| u′,v′ | CIE 1976 坐标 | x,y | CIE 1931 坐标 | P<sub>ijk</sub> | 重构网格点 |
| W<sub>n</sub> | 邻域样本比例 | W<sub>25</sub> | 基础比例估计 | W | 限幅后的基础比例 |
| d<sub>n</sub> | 欧式范数距离 | ω<sub>n</sub> | 局部权重 | a,b,c | 局部仿射模型参数 |
| θ | 温度变量 | t | 归一化温度 | s | 分段索引 |
| φ<sub>i</sub> | 第 i 个基函数 | α<sub>s,i</sub>,β<sub>s,i</sub> | 曲面系数 | Δρ | 温度引起的最优比例变化量 |
| ΔW | Δρ 在模型中的记号 | θ | 温度变量 | t | 归一化温度变量 |
| W<sub>formula</sub> | 温度修正比例 | W<sub>max</sub> | 最大可行白光比例 | W<sub>final</sub> | 最终比例 |
| P<sub>RGB</sub> | RGB 子目标点 | A<sub>RGB</sub> | RGB 三刺激矩阵 | d<sub>RGBW</sub> | 四通道驱动向量 |

| 函数/算子 | 含义 | 函数/算子 | 含义 | 函数/算子 | 含义 |
|---|---|---|---|---|---|
| ‖·‖<sub>2</sub> | 欧式范数算子 | clamp(x,a,b) | 区间限幅算子 | min(·,·) | 最小值算子 |
| XYZ(P,Y) | xyY 到 XYZ 映射 | Σ | 求和算子 | f<sub>mix</sub> | RGBW 分解映射 |

## 1. 输入色度与目标点

在线目标点记为：

<p align="center"><font face="Times New Roman">Z<sub>0</sub> = (u′<sub>0</sub>,v′<sub>0</sub>)</font></p>

温度曲面需要 xy 坐标，u′v′ 到 xy 的映射为：

<p align="center"><font face="Times New Roman">d = 6u′ − 16v′ + 12</font></p>

<p align="center"><font face="Times New Roman">x = 9u′ / d,  y = 4v′ / d</font></p>

## 2. 网格点重构

C 版本保存 RGB 三角形顶点与网格比例值。对网格索引 i、j、k：

<p align="center"><font face="Times New Roman">P<sub>ijk</sub> = [iP<sub>R</sub> + jP<sub>G</sub> + kP<sub>B</sub>] / N,  i+j+k=N</font></p>

该点可转换到 u′v′ 空间，形成邻域候选点 Z<sub>n</sub>。因此，C 版本的基础比例预测问题是从离散样本：

<p align="center"><font face="Times New Roman">𝒩 = {(Z<sub>n</sub>, W<sub>n</sub>)}</font></p>

估计目标点 Z<sub>0</sub> 处的 W<sub>25</sub>。

## 3. 欧式范数加权局部回归

局部仿射模型：

<p align="center"><font face="Times New Roman">W(u′,v′) = a(u′ − u′<sub>0</sub>) + b(v′ − v′<sub>0</sub>) + c</font></p>

欧式范数距离：

<p align="center"><font face="Times New Roman">d<sub>n</sub> = ‖Z<sub>n</sub> − Z<sub>0</sub>‖<sub>2</sub></font></p>

距离权重：

<p align="center"><font face="Times New Roman">ω<sub>n</sub> ∝ 1 / (d<sub>n</sub><sup>2</sup> + ε)</font></p>

加权最小二乘：

<p align="center"><font face="Times New Roman">min<sub>a,b,c</sub> Σ<sub>n</sub>ω<sub>n</sub>[aΔu′<sub>n</sub> + bΔv′<sub>n</sub> + c − W<sub>n</sub>]<sup>2</sup></font></p>

由于目标点处 Δu′ = 0 且 Δv′ = 0，基础比例估计为：

<p align="center"><font face="Times New Roman">W<sub>25</sub> = W(u′<sub>0</sub>,v′<sub>0</sub>) = c</font></p>

## 4. 十项基函数

令：

<p align="center"><font face="Times New Roman">W = clamp(W<sub>25</sub>,0,1)</font></p>

十项基函数为：

| 序号 | 基函数 | 意义 |
|---:|---|---|
| 0 | 1 | 常数偏置 |
| 1 | x | 色度 x 一阶项 |
| 2 | y | 色度 y 一阶项 |
| 3 | W | 基础比例一阶项 |
| 4 | xy | 色度耦合项 |
| 5 | xW | x 与比例耦合项 |
| 6 | yW | y 与比例耦合项 |
| 7 | W<sup>2</sup> | 比例二阶项 |
| 8 | x<sup>2</sup> | x 方向二阶项 |
| 9 | y<sup>2</sup> | y 方向二阶项 |

## 5. 温度曲面求值

温度归一化：

<p align="center"><font face="Times New Roman">t = [clamp(θ,θ<sub>0</sub>,θ<sub>max</sub>) − θ<sub>0</sub>] / [θ<sub>max</sub> − θ<sub>0</sub>]</font></p>

对分段 s：

<p align="center"><font face="Times New Roman">F<sub>1,s</sub> = Σ<sub>i=0</sub><sup>9</sup>α<sub>s,i</sub>φ<sub>i</sub></font></p>

<p align="center"><font face="Times New Roman">F<sub>2,s</sub> = Σ<sub>i=0</sub><sup>9</sup>β<sub>s,i</sub>φ<sub>i</sub></font></p>

<p align="center"><font face="Times New Roman">ΔW = tF<sub>1,s</sub> + t<sup>2</sup>F<sub>2,s</sub></font></p>

<p align="center"><font face="Times New Roman">W<sub>formula</sub> = clamp(W + ΔW,0,1)</font></p>

## 6. 色域边界与最终比例

设 W<sub>max</sub> 为目标颜色在 RGB 凸包中允许的最大白光比例，则：

<p align="center"><font face="Times New Roman">W<sub>final</sub> = min(W<sub>formula</sub>,W<sub>max</sub>)</font></p>

该步骤保证温度曲面输出不会破坏 RGB 子目标的可实现性。

## 7. RGBW 分解

令 ρ = W<sub>final</sub>，则：

<p align="center"><font face="Times New Roman">Y<sub>W,out</sub> = ρY<sub>T</sub>,  Y<sub>RGB,out</sub> = (1−ρ)Y<sub>T</sub></font></p>

RGB 子目标：

<p align="center"><font face="Times New Roman">P<sub>RGB</sub> = P<sub>W</sub> + [P<sub>T</sub> − P<sub>W</sub>] / [1 − ρy<sub>T</sub>/y<sub>W</sub>]</font></p>

RGB 权重：

<p align="center"><font face="Times New Roman">w<sub>RGB</sub> = A<sub>RGB</sub><sup>−1</sup>XYZ(P<sub>RGB</sub>,Y<sub>RGB,out</sub>)</font></p>

归一化驱动向量：

<p align="center"><font face="Times New Roman">d<sub>RGBW</sub> = [w<sub>R</sub>,w<sub>G</sub>,w<sub>B</sub>,ρ]<sup>T</sup></font></p>

## 闭环表达

<p align="center"><font face="Times New Roman">Z<sub>0</sub> → f<sub>local</sub> → W<sub>25</sub> → f<sub>thermal</sub> → W<sub>formula</sub> → min(·,W<sub>max</sub>) → W<sub>final</sub> → d<sub>RGBW</sub></font></p>
