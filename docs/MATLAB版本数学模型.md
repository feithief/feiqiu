# MATLAB 版本数学模型

## 整体思路

MATLAB 版本承担“光谱级优化与参数生成”的数学任务，其思路也分为三层。

第一，单色最大 Ra。对一个给定目标颜色 T，扫描或搜索可行白光比例 ρ，计算每个 ρ 对应的 RGBW 混合光谱与 R<sub>a</sub>(ρ)，选取最大显色比例 ρ<sup>*</sup>(T)。

第二，全色域最大 Ra。在 RGB 色域内构造三角网格，对每个网格点重复单色最大 Ra 求解，得到基础最优比例场 𝓦<sub>0</sub>。这一建模步骤依赖的前提是：最大显色白光比例 ρ<sup>*</sup>(T) 是目标颜色 T 的局部连续函数，因此可由离散网格点推广为连续比例场。

<p align="center"><font face="Times New Roman">ρ<sup>*</sup>: 𝒢 → [0,1], &nbsp; ρ<sup>*</sup> ∈ C<sup>0</sup>(𝒢)</font></p>

第三，考虑温度的最大 Ra。对温度样本重新获得高温条件下的光谱级最优比例 W<sub>best</sub>，并拟合 W<sub>best</sub> 相对于 W<sub>25</sub> 的偏移 ΔW，得到温度曲面系数。这一建模步骤依赖的前提是：温度导致的比例变化量 Δρ，即 ΔW，是关于目标色坐标、基础比例和温度的连续函数。

<p align="center"><font face="Times New Roman">Δρ: 𝒢 × [0,1] × Θ → ℝ, &nbsp; Δρ ∈ C<sup>0</sup>(𝒢 × [0,1] × Θ)</font></p>

<p align="center"><font face="Times New Roman">S<sub>c</sub>(λ) → P<sub>ijk</sub> → Ω<sub>ijk</sub> → W<sub>0,ijk</sub> → α<sub>s,i</sub>,β<sub>s,i</sub></font></p>

## 符号、函数与算子

| 符号 | 含义 | 符号 | 含义 | 符号 | 含义 |
|---|---|---|---|---|---|
| S<sub>c,raw</sub>(λ) | 原始通道光谱 | S<sub>c</sub>(λ) | 归一化通道光谱 | κ<sub>c</sub> | 光谱尺度因子 |
| P<sub>c</sub> | 通道色度点 | V<sub>c</sub> | 通道 XYZ 向量 | 𝒢 | RGB 色域凸包 |
| P<sub>ijk</sub> | 网格目标点 | Ω<sub>ijk</sub> | 网格点可行比例集合 | ρ | 白光比例 |
| W<sub>0,ijk</sub> | 网格基础最优比例 | W<sub>best</sub> | 温度样本最优比例 | W<sub>25</sub> | 基础比例估计 |
| Δρ | 温度引起的最优比例变化量 | ΔW<sub>target</sub> | Δρ 的训练目标 | η<sub>n</sub> | 拟合样本权重 |
| α<sub>s,i</sub>,β<sub>s,i</sub> | 曲面系数 | θ | 温度变量 | t | 归一化温度变量 |
| θ | 温度变量 | t | 归一化温度 | φ<sub>i</sub> | 第 i 个基函数 |

| 函数/算子 | 含义 | 函数/算子 | 含义 | 函数/算子 | 含义 |
|---|---|---|---|---|---|
| Conv(·) | 凸包算子 | CRI[·] | 显色指数算子 | arg max | 极大值点算子 |
| Σ | 求和算子 | clamp(x,a,b) | 区间限幅算子 | min | 最小化优化算子 |

## 1. 光谱归一化与通道色度

每个通道 c 的原始光谱 S<sub>c,raw</sub>(λ) 先根据目标亮度进行尺度归一化：

<p align="center"><font face="Times New Roman">S<sub>c</sub>(λ) = κ<sub>c</sub>S<sub>c,raw</sub>(λ)</font></p>

其中 κ<sub>c</sub> 由目标 Y<sub>c</sub> 与原始光谱积分得到的 Y<sub>c,raw</sub> 决定。归一化后的 S<sub>c</sub>(λ) 再通过标准观察者积分得到 P<sub>c</sub> 与 V<sub>c</sub>。

## 2. RGB 色域离散化

基础色域定义为：

<p align="center"><font face="Times New Roman">𝒢 = Conv(P<sub>R</sub>,P<sub>G</sub>,P<sub>B</sub>)</font></p>

对三角网格阶数 N：

<p align="center"><font face="Times New Roman">P<sub>ijk</sub> = [iP<sub>R</sub> + jP<sub>G</sub> + kP<sub>B</sub>] / N,  i+j+k=N</font></p>

该网格给出基础比例场的采样点。

## 3. 单点最大显色搜索

对每个目标 P<sub>ijk</sub>，建立可行比例集合：

<p align="center"><font face="Times New Roman">Ω<sub>ijk</sub> = {ρ | 0 ≤ ρ ≤ 1, P<sub>RGB</sub>(ρ) ∈ 𝒢, w<sub>RGB</sub>(ρ) ≥ 0}</font></p>

对每个候选 ρ，计算 RGB 子目标、RGB 权重和混合光谱：

<p align="center"><font face="Times New Roman">S<sub>mix</sub>(λ;ρ) = ρS<sub>W</sub>(λ) + w<sub>R</sub>(ρ)S<sub>R</sub>(λ) + w<sub>G</sub>(ρ)S<sub>G</sub>(λ) + w<sub>B</sub>(ρ)S<sub>B</sub>(λ)</font></p>

显色目标：

<p align="center"><font face="Times New Roman">R<sub>a</sub>(ρ) = CRI[S<sub>mix</sub>(λ;ρ)]</font></p>

网格点最优比例：

<p align="center"><font face="Times New Roman">W<sub>0,ijk</sub> = arg max<sub>ρ∈Ω<sub>ijk</sub></sub> R<sub>a</sub>(ρ)</font></p>

这里 W<sub>0,ijk</sub> 是使 R<sub>a</sub> 最大的白光比例，而不是 R<sub>a</sub> 数值。

## 4. 基础比例场

全部网格点形成基础比例场：

<p align="center"><font face="Times New Roman">𝓦<sub>0</sub> = {(P<sub>ijk</sub>, W<sub>0,ijk</sub>)}</font></p>

该比例场是基础温度条件下光谱级最大显色问题的离散解。

## 5. 温度样本与比例偏移

在温度 θ 下，光谱级重新搜索可得到高温最优比例 W<sub>best</sub>。基础比例估计为 W<sub>25</sub>。温度扰动建模目标为二者差值：

<p align="center"><font face="Times New Roman">ΔW<sub>target</sub> = W<sub>best</sub> − W<sub>25</sub></font></p>

该差值表示温度变化相对于基础比例场造成的最优比例偏移。

## 6. 十项基函数

令：

<p align="center"><font face="Times New Roman">W = clamp(W<sub>25</sub>,0,1)</font></p>

十项基函数为：

| 序号 | 基函数 | 意义 |
|---:|---|---|
| 0 | 1 | 常数偏置 |
| 1 | x | 目标色度 x 的一阶影响 |
| 2 | y | 目标色度 y 的一阶影响 |
| 3 | W | 基础比例的一阶影响 |
| 4 | xy | x 与 y 的耦合影响 |
| 5 | xW | x 与基础比例的耦合影响 |
| 6 | yW | y 与基础比例的耦合影响 |
| 7 | W<sup>2</sup> | 基础比例二阶项 |
| 8 | x<sup>2</sup> | x 方向二阶曲率 |
| 9 | y<sup>2</sup> | y 方向二阶曲率 |

## 7. 温度曲面拟合

温度归一化：

<p align="center"><font face="Times New Roman">t = [clamp(θ,θ<sub>0</sub>,θ<sub>max</sub>) − θ<sub>0</sub>] / [θ<sub>max</sub> − θ<sub>0</sub>]</font></p>

每个分段 s 的曲面为：

<p align="center"><font face="Times New Roman">F<sub>1,s</sub> = Σ<sub>i=0</sub><sup>9</sup>α<sub>s,i</sub>φ<sub>i</sub></font></p>

<p align="center"><font face="Times New Roman">F<sub>2,s</sub> = Σ<sub>i=0</sub><sup>9</sup>β<sub>s,i</sub>φ<sub>i</sub></font></p>

模型偏移：

<p align="center"><font face="Times New Roman">ΔW<sub>model</sub> = tF<sub>1,s</sub> + t<sup>2</sup>F<sub>2,s</sub></font></p>

系数通过加权最小二乘确定：

<p align="center"><font face="Times New Roman">min Σ<sub>n</sub>η<sub>n</sub>[ΔW<sub>model,n</sub> − ΔW<sub>target,n</sub>]<sup>2</sup></font></p>

其中 η<sub>n</sub> 为温度曲面拟合样本权重。

## 8. MATLAB 版本输出的数学对象

MATLAB 版本最终生成两类数学对象：

| 对象 | 含义 |
|---|---|
| 𝓦<sub>0</sub> | 基础温度条件下最大显色比例场 |
| α<sub>s,i</sub>, β<sub>s,i</sub> | 温度扰动曲面系数 |

## 闭环表达

<p align="center"><font face="Times New Roman">完整光谱 → 最大 R<sub>a</sub> 搜索 → 基础比例场 → 温度扰动样本 → 分段曲面系数</font></p>
