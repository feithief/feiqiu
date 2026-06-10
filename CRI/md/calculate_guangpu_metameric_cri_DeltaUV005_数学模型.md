# 目标 DeltaUV 容差内单白光与 RGB 射线扫描混合提升 CRI 的数学模型

本文对应当前 `calculate_guangpu_metameric_cri.m`。它是在《单白灯_RGB同色异谱混合提升CRI_正文.md》和固定目标射线模型的基础上进一步扩展：目标白点不再只取一个固定 $T_0$，而是在标称目标点周围满足 $\Delta uv\leq0.005$ 的候选目标集合中，寻找 CRI 最大值。

光谱积分、XYZ、CCT、参考光源、R1 至 R8、Ra 和 Rmin8 的计算链路保持原正文不变。

---

## 1. 标称目标点与 DeltaUV 约束

标称目标点为：

$$
T_0=(x_0,y_0,Y_T)=cfg.target\_xyY
$$

当前模型允许目标色坐标在 $T_0$ 附近移动，但亮度仍保持 $Y_T$：

$$
T_q=(x_q,y_q,Y_T)
$$

DeltaUV 使用 CIE 1976 $u'v'$ 色度坐标：

$$
u'=\frac{4x}{-2x+12y+3},\qquad
v'=\frac{9y}{-2x+12y+3}
$$

候选目标点集合定义为：

$$
\mathcal{T}_{0.005}=
\left\{
T_q\mid
\sqrt{(u'_q-u'_0)^2+(v'_q-v'_0)^2}\leq0.005,\quad Y_q=Y_T
\right\}
$$

其中：

$$
\Delta uv(T_q,T_0)=\sqrt{(u'_q-u'_0)^2+(v'_q-v'_0)^2}
$$

---

## 2. 光谱统一与实测基色建模

每个通道仍按原正文的方式先做光谱积分和光通量标定：

$$
\alpha_c=\frac{Y_c}{683\sum S_c^{raw}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_c(\lambda)=\alpha_cS_c^{raw}(\lambda)
$$

其中：

$$
c\in\{R,G,B,W,SingleWhite\}
$$

再由标定后光谱重新积分得到实际色坐标：

$$
C_R=(x_R,y_R,Y_R),\qquad
C_G=(x_G,y_G,Y_G),\qquad
C_B=(x_B,y_B,Y_B)
$$

单白光实际几何白点为：

$$
W=(x_W,y_W)
$$

单白光谱归一到目标亮度：

$$
S_{SingleNorm}(\lambda)\rightarrow Y_T
$$

---

## 3. 对每一个候选目标点建立射线

对候选目标 $T_q$，RGB 候选点位于从单白点 $W$ 指向 $T_q$ 并延伸到 RGB 色域边界的射线上：

$$
P_q(t)=W+t(T_q-W),\qquad t\geq1
$$

当 $t=1$ 时：

$$
P_q(1)=T_q
$$

当 $t=t_{edge,q}$ 时，射线到达 RGB 色域边界：

$$
W+t_{edge,q}(T_q-W)=E_i+s(E_{i+1}-E_i),\qquad 0\leq s\leq1
$$

脚本选择三条边中满足 $t\geq1$ 的最近交点：

$$
t_{edge,q}=\min(t),\qquad t\geq1
$$

因此每个候选目标点对应的射线扫描范围为：

$$
1\leq t\leq t_{edge,q}
$$

---

## 4. 候选目标点下的唯一混光比例

对给定 $T_q$ 和 $P_q(t)$：

$$
T_q=(1-\mu)W+\mu P_q(t)
$$

由射线定义可得：

$$
\mu=\frac{1}{t},\qquad 1-\mu=\frac{t-1}{t}
$$

所以当前模型的自由变量是二元变量：

$$
(T_q,t),\qquad T_q\in\mathcal{T}_{0.005},\quad 1\leq t\leq t_{edge,q}
$$

单白分量和 RGB 分量的光通量拆分为：

$$
Y_W(T_q,t)=Y_T(1-\mu)\frac{y_W}{y_q}
$$

$$
Y_P(T_q,t)=Y_T\mu\frac{y_{P_q(t)}}{y_q}
$$

并满足：

$$
Y_W(T_q,t)+Y_P(T_q,t)=Y_T
$$

实际单白分量光谱为：

$$
S_W(\lambda,T_q,t)=\frac{Y_W(T_q,t)}{Y_T}S_{SingleNorm}(\lambda)
$$

---

## 5. RGB 权重反解与混合光谱

RGB 基色矩阵为：

$$
A=
\begin{bmatrix}
X_R&X_G&X_B\\
Y_R&Y_G&Y_B\\
Z_R&Z_G&Z_B
\end{bmatrix}
$$

对每一个 $P_q(t)$，先由 xyY 转换为 XYZ：

$$
X_{P_q(t)}=\frac{x_{P_q(t)}}{y_{P_q(t)}}Y_P(T_q,t)
$$

$$
Y_{P_q(t)}=Y_P(T_q,t)
$$

$$
Z_{P_q(t)}=\frac{1-x_{P_q(t)}-y_{P_q(t)}}{y_{P_q(t)}}Y_P(T_q,t)
$$

再反解 RGB 权重：

$$
\begin{bmatrix}
r(T_q,t)\\g(T_q,t)\\b(T_q,t)
\end{bmatrix}
=
A^{-1}
\begin{bmatrix}
X_{P_q(t)}\\Y_{P_q(t)}\\Z_{P_q(t)}
\end{bmatrix}
$$

RGB 分量光谱为：

$$
S_{RGB}(\lambda,T_q,t)=
r(T_q,t)S_R(\lambda)+g(T_q,t)S_G(\lambda)+b(T_q,t)S_B(\lambda)
$$

最终混合光谱为：

$$
S_{mix}(\lambda,T_q,t)=S_W(\lambda,T_q,t)+S_{RGB}(\lambda,T_q,t)
$$

其约束为：

$$
xyY(S_{mix})=T_q,\qquad \Delta uv(T_q,T_0)\leq0.005
$$

---

## 6. CRI 最大化目标

对每一个候选组合 $(T_q,t)$，仍按原正文计算 $R_1$ 至 $R_8$：

$$
R_a(T_q,t)=\frac{1}{8}\sum_{i=1}^{8}R_i(T_q,t)
$$

$$
R_{min,8}(T_q,t)=\min_{1\leq i\leq8}R_i(T_q,t)
$$

当前模型的最优筛选为：

$$
(T_{Ra},t_{Ra})=
\arg\max_{\substack{T_q\in\mathcal{T}_{0.005}\\1\leq t\leq t_{edge,q}}}
R_a(T_q,t)
$$

以及：

$$
(T_{min},t_{min})=
\arg\max_{\substack{T_q\in\mathcal{T}_{0.005}\\1\leq t\leq t_{edge,q}}}
R_{min,8}(T_q,t)
$$

---

## 7. 当前脚本中的离散搜索实现

为缩短运行时间，当前脚本使用两级离散搜索。

粗搜目标点：

$$
\Delta u',\Delta v'=0,\pm0.0015,\pm0.0030,\cdots
$$

并保留满足：

$$
\sqrt{(\Delta u')^2+(\Delta v')^2}\leq0.005
$$

的候选点。粗搜每条射线采样：

$$
N_{coarse}=61
$$

细搜以粗搜最佳点为中心，半径为：

$$
r_{fine}=0.0010
$$

细搜步长为：

$$
\Delta u'_{fine}=\Delta v'_{fine}=0.0005
$$

细搜每条射线采样：

$$
N_{fine}=151
$$

这些默认值可由 `guangpu_case_config.m` 中的字段覆盖：

| 字段 | 默认值 |
|---|---:|
| `targetDeltaUvLimit` | 0.005 |
| `targetCoarseUvStep` | 0.0015 |
| `targetFineUvStep` | 0.0005 |
| `targetFineUvRadius` | 0.0010 |
| `coarseRaySamples` | 61 |
| `fineRaySamples` | 151 |

---

## 8. 输出字段对应关系

| 字段 | 含义 |
|---|---|
| `SearchStage` | `coarse` 或 `fine` |
| `Target_x`, `Target_y` | 当前候选目标点 $T_q$ 的 xy |
| `TargetDelta_uv` | $\Delta uv(T_q,T_0)$ |
| `RayT` | 当前候选目标点下的射线参数 $t$ |
| `WhiteXYCoeff` | 几何系数 $1-\mu$ |
| `RGBXYCoeff` | 几何系数 $\mu=1/t$ |
| `SingleWhiteRatio` | 单白光通量占目标亮度比例 $Y_W/Y_T$ |
| `RGBWhiteRatio` | RGB 分量光通量占目标亮度比例 $Y_P/Y_T$ |
| `RGBPoint_x`, `RGBPoint_y` | 当前 RGB 候选点 $P_q(t)$ 的 xy |
| `RGBPoint_Y_lm` | 当前 RGB 候选点亮度 $Y_P(T_q,t)$ |
| `R_weight`, `G_weight`, `B_weight` | 当前 $P_q(t)$ 反解得到的 RGB 权重 |
| `Ra`, `Rmin_8` | 当前混合光谱的显色指标 |
| `Edge` | 射线最终碰到的 RGB 色域边 |

---

## 结论

当前模型比固定目标点版本多了一层目标色容差搜索。它不是只在标称目标 $T_0$ 上找最佳射线点，而是在 $\Delta uv\leq0.005$ 的目标集合内，同时优化目标点 $T_q$ 和射线参数 $t$。每个候选组合都会重新拆分单白亮度、反解 RGB 权重、合成光谱并计算 CRI；最终输出的是容差范围内的全局 Best Ra 和 Best Rmin8。
