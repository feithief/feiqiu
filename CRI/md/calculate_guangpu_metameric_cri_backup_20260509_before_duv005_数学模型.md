# 固定目标白点下单白光与 RGB 射线扫描混合提升 CRI 的数学模型

本文对应 `calculate_guangpu_metameric_cri_backup_20260509_before_duv005.m`。它是在《单白灯_RGB同色异谱混合提升CRI_正文.md》的基础上，把“单白光与一个固定 RGB 白光按比例扫描”改为“目标白点固定，沿单白光到目标点的射线寻找 RGB 色域边界内 CRI 最大点”。

光谱到 XYZ、CCT、参考光源、R1 至 R8、Ra 和 Rmin8 的计算链路保持原正文不变。

---

## 1. 光谱统一与基础色度计算

所有通道光谱统一到脚本配置的波长轴：

$$
\lambda=\lambda_1,\lambda_2,\cdots,\lambda_n,\qquad \Delta\lambda=cfg.dLambda
$$

任意光谱 $S(\lambda)$ 的三刺激值、色坐标和光通量为：

$$
X=\sum S(\lambda)\overline{x}(\lambda)\Delta\lambda,\qquad
Y=\sum S(\lambda)\overline{y}(\lambda)\Delta\lambda,\qquad
Z=\sum S(\lambda)\overline{z}(\lambda)\Delta\lambda
$$

$$
x=\frac{X}{X+Y+Z},\qquad
y=\frac{Y}{X+Y+Z},\qquad
\Phi_v=683Y
$$

每个实测通道先按给定光通量标定：

$$
\alpha_c=\frac{Y_c}{683\sum S_c^{raw}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_c(\lambda)=\alpha_cS_c^{raw}(\lambda)
$$

其中：

$$
c\in\{R,G,B,W,SingleWhite\}
$$

---

## 2. 固定目标点与实测单白点

目标白点固定为：

$$
T=(x_T,y_T,Y_T)=cfg.target\_xyY
$$

单白光不再作为可自由扫描的 RGB 白光端点，而是由白光光谱积分得到其实际色坐标：

$$
W=(x_W,y_W)
$$

R、G、B 三个实测基色形成 RGB 色域三角形：

$$
C_R=(x_R,y_R,Y_R),\qquad
C_G=(x_G,y_G,Y_G),\qquad
C_B=(x_B,y_B,Y_B)
$$

脚本中 `cfg.single_white_xyY` 用于把单白光谱标定到指定亮度；几何白点 $W$ 使用标定后单白光谱重新积分得到的实际 xy。

---

## 3. 从单白点到 RGB 色域边界的射线

RGB 候选点必须落在从单白点 $W$ 指向目标点 $T$ 并继续延伸的射线上：

$$
P(t)=W+t(T-W),\qquad t\geq1
$$

当 $t=1$ 时：

$$
P(1)=T
$$

当 $t=t_{edge}$ 时，$P(t)$ 到达 RGB 色域边界。若色域边为 $E_iE_{i+1}$，则交点满足：

$$
W+t_{edge}(T-W)=E_i+s(E_{i+1}-E_i),\qquad 0\leq s\leq1
$$

脚本在三条边中选择满足 $t\geq1$ 的最近交点：

$$
t_{edge}=\min(t),\qquad t\geq1
$$

最终扫描区间为：

$$
1\leq t\leq t_{edge}
$$

---

## 4. 固定目标点下的混光比例

对任意候选 RGB 点 $P(t)$，目标点 $T$ 在 $W$ 与 $P(t)$ 之间的位置唯一确定：

$$
T=(1-\mu)W+\mu P(t)
$$

由 $P(t)=W+t(T-W)$ 可得：

$$
\mu=\frac{1}{t},\qquad 1-\mu=\frac{t-1}{t}
$$

因此，当 RGB 点 $P(t)$ 固定时，混光比例也固定。模型扫描的是射线参数 $t$，不是自由比例 $p$。

考虑 xy 混光与亮度 $Y_T$，单白分量和 RGB 分量的光通量拆分为：

$$
Y_W(t)=Y_T(1-\mu)\frac{y_W}{y_T}
$$

$$
Y_P(t)=Y_T\mu\frac{y_{P(t)}}{y_T}
$$

并且：

$$
Y_W(t)+Y_P(t)=Y_T
$$

单白光谱先归一到目标亮度：

$$
S_{SingleNorm}(\lambda)\rightarrow Y_T
$$

实际参与混光的单白分量为：

$$
S_W(\lambda,t)=\frac{Y_W(t)}{Y_T}S_{SingleNorm}(\lambda)
$$

---

## 5. RGB 候选点权重反解

由 R、G、B 的 XYZ 构造矩阵：

$$
A=
\begin{bmatrix}
X_R&X_G&X_B\\
Y_R&Y_G&Y_B\\
Z_R&Z_G&Z_B
\end{bmatrix}
$$

对每一个射线点 $P(t)$，先把其 xyY 转成 XYZ：

$$
X_{P(t)}=\frac{x_{P(t)}}{y_{P(t)}}Y_P(t),\qquad
Y_{P(t)}=Y_P(t),\qquad
Z_{P(t)}=\frac{1-x_{P(t)}-y_{P(t)}}{y_{P(t)}}Y_P(t)
$$

再反解 RGB 权重：

$$
\begin{bmatrix}
r(t)\\g(t)\\b(t)
\end{bmatrix}
=
A^{-1}
\begin{bmatrix}
X_{P(t)}\\Y_{P(t)}\\Z_{P(t)}
\end{bmatrix}
$$

候选 RGB 分量光谱为：

$$
S_{RGB}(\lambda,t)=r(t)S_R(\lambda)+g(t)S_G(\lambda)+b(t)S_B(\lambda)
$$

最终混合光谱为：

$$
S_{mix}(\lambda,t)=S_W(\lambda,t)+S_{RGB}(\lambda,t)
$$

其约束为：

$$
xyY(S_{mix})=T
$$

---

## 6. CRI 评价与最优点筛选

对每一个 $t_i$：

$$
t_i=1+i\frac{t_{edge}-1}{N-1},\qquad i=0,1,\cdots,N-1
$$

计算：

$$
P(t_i),\qquad Y_W(t_i),\qquad Y_P(t_i),\qquad r(t_i),g(t_i),b(t_i)
$$

并得到测试光源：

$$
S_{mix}(\lambda,t_i)
$$

CRI 指标保持原正文写法：

$$
R_a(t)=\frac{1}{8}\sum_{i=1}^{8}R_i(t),\qquad
R_{min,8}(t)=\min_{1\leq i\leq8}R_i(t)
$$

最优点筛选为：

$$
t_{Ra}=\arg\max_{1\leq t\leq t_{edge}}R_a(t),\qquad
t_{min}=\arg\max_{1\leq t\leq t_{edge}}R_{min,8}(t)
$$

---

## 7. 输出字段对应关系

| 字段 | 含义 |
|---|---|
| `RayT` | 射线参数 $t$ |
| `WhiteXYCoeff` | 几何系数 $1-\mu$ |
| `RGBXYCoeff` | 几何系数 $\mu=1/t$ |
| `SingleWhiteRatio` | 单白光通量占目标亮度比例 $Y_W/Y_T$ |
| `RGBWhiteRatio` | RGB 分量光通量占目标亮度比例 $Y_P/Y_T$ |
| `RGBPoint_x`, `RGBPoint_y` | 候选 RGB 点 $P(t)$ 的 xy |
| `RGBPoint_Y_lm` | 候选 RGB 点亮度 $Y_P(t)$ |
| `R_weight`, `G_weight`, `B_weight` | 当前 $P(t)$ 反解得到的 RGB 权重 |
| `Ra`, `Rmin_8` | 当前混合光谱的显色指标 |
| `Edge` | 射线最终碰到的 RGB 色域边 |

---

## 结论

该模型不再寻找“单白光与固定 RGB 白光之间的最佳比例”。目标点 $T$ 固定时，候选 RGB 点 $P(t)$ 一旦确定，单白分量和 RGB 分量的亮度比例也随之唯一确定。真正被优化的是 $P(t)$ 沿着 $W\rightarrow T$ 射线在 RGB 色域内的位置；每个位置都重新反解 RGB 权重，并保证最终混合光谱仍落在同一个目标 xyY 上。
