# 固定单白光与 RGB 射线扫描混合提升 CRI 的模型变更说明

本文只说明相对《单白灯_RGB同色异谱混合提升CRI_正文.md》的改动部分。光谱到 XYZ、CCT、参考光源、R1 至 R8、Ra 和 Rmin8 的计算链路保持不变。

---

## 1. 输入含义的变化

原模型中，先由 R、G、B 直接合成一个目标白点 RGB 白光，再让该 RGB 白光与单白光按比例扫描。

新模型中，目标颜色点固定为：

$$
T=(x_T,y_T,Y_T)=cfg.target\_xyY
$$

单白光不再作为可移动白点，而是由白光光谱实测结果确定：

$$
W=(x_W,y_W)
$$

R、G、B 三基色同样由各自光谱实测结果确定，并形成 RGB 色域三角形：

$$
C_R=(x_R,y_R,Y_R),\qquad
C_G=(x_G,y_G,Y_G),\qquad
C_B=(x_B,y_B,Y_B)
$$

其中，$cfg.channel\_xyY.W$ 是 W 通道的实测或标定 xyY，$cfg.single\_white\_xyY$ 表示参与混光的单白光谱亮度标定值。当前计算中，单白光最终使用光谱重新积分得到的 $W=(x_W,y_W)$ 作为几何白点。

---

## 2. RGB 权重求解对象的变化

原模型只求一次目标白点对应的 RGB 权重：

$$
\begin{bmatrix}
r\\g\\b
\end{bmatrix}
=
A^{-1}
\begin{bmatrix}
X_T\\Y_T\\Z_T
\end{bmatrix}
$$

新模型仍然使用同一个 RGB 基色矩阵：

$$
A=
\begin{bmatrix}
X_R&X_G&X_B\\
Y_R&Y_G&Y_B\\
Z_R&Z_G&Z_B
\end{bmatrix}
$$

但 RGB 权重不再只对应目标点 $T$，而是对应射线上的每一个候选 RGB 点 $P(t)$：

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

候选 RGB 光谱为：

$$
S_{RGB}(\lambda,t)=r(t)S_R(\lambda)+g(t)S_G(\lambda)+b(t)S_B(\lambda)
$$

因此，CRI 最大化时变化的不是一个固定 RGB 白光的混合比例，而是 RGB 修正点 $P(t)$ 在射线上的位置。

---

## 3. 从单白到目标再到 RGB 色域边界的射线

新模型规定 RGB 候选点必须落在从单白光 $W$ 指向目标点 $T$ 的射线上：

$$
P(t)=W+t(T-W),\qquad t\geq1
$$

当 $t=1$ 时：

$$
P(1)=T
$$

当 $t=t_{edge}$ 时，$P(t)$ 到达 RGB 色域三角形边界。若某条边为 $E_iE_{i+1}$，则边界交点满足：

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

对任意一个候选点 $P(t)$，目标点 $T$ 在 $W$ 和 $P(t)$ 之间的位置已经唯一确定：

$$
T=(1-\mu)W+\mu P(t)
$$

由 $P(t)=W+t(T-W)$ 可得：

$$
\mu=\frac{1}{t},\qquad 1-\mu=\frac{t-1}{t}
$$

所以，如果 RGB 点 $P(t)$ 已经固定，几何混光比例就是固定的；新模型扫描的是 $t$，不是自由扫描 $p$。

由于 xy 混光还要满足亮度 $Y_T$，单白分量和 RGB 分量的光通量拆分为：

$$
Y_W(t)=Y_T(1-\mu)\frac{y_W}{y_T}
$$

$$
Y_{P}(t)=Y_T\mu\frac{y_{P(t)}}{y_T}
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

因此最终混合光谱为：

$$
S_{mix}(\lambda,t)=S_W(\lambda,t)+S_{RGB}(\lambda,t)
$$

该混合光谱在色坐标和亮度上被约束为：

$$
xyY(S_{mix})=T
$$

---

## 5. 最优 CRI 筛选变量的变化

原模型的扫描变量为同亮度混光比例：

$$
S_{mix}(\lambda,p)=pS_{SingleNorm}(\lambda)+(1-p)S_{RGBNorm}(\lambda),\qquad
p=0,0.001,\cdots,1
$$

新模型的扫描变量为射线参数 $t$：

$$
t_i=1+i\frac{t_{edge}-1}{N-1},\qquad i=0,1,\cdots,N-1
$$

其中，$N$ 为射线采样点数。对每一个 $t_i$，重新计算：

$$
P(t_i),\qquad Y_W(t_i),\qquad Y_P(t_i),\qquad r(t_i),g(t_i),b(t_i)
$$

再得到：

$$
S_{mix}(\lambda,t_i)
$$

CRI 指标写法保持原有形式，只是自变量从 $p$ 改为 $t$：

$$
R_a(t)=\frac{1}{8}\sum_{i=1}^{8}R_i(t),\qquad
R_{min,8}(t)=\min_{1\leq i\leq8}R_i(t)
$$

最优点筛选改为：

$$
t_{Ra}=\arg\max_{1\leq t\leq t_{edge}}R_a(t),\qquad
t_{min}=\arg\max_{1\leq t\leq t_{edge}}R_{min,8}(t)
$$

---

## 6. 输出字段对应关系

新模型输出表中，主要字段含义如下：

| 字段 | 含义 |
|---|---|
| `RayT` | 射线参数 $t$ |
| `WhiteXYCoeff` | 几何系数 $1-\mu$ |
| `RGBXYCoeff` | 几何系数 $\mu=1/t$ |
| `SingleWhiteRatio` | 单白光通量占目标亮度的比例 $Y_W/Y_T$ |
| `RGBWhiteRatio` | RGB 分量光通量占目标亮度的比例 $Y_P/Y_T$ |
| `RGBPoint_x`, `RGBPoint_y` | 当前候选 RGB 点 $P(t)$ 的 xy |
| `RGBPoint_Y_lm` | 当前候选 RGB 点的亮度 $Y_P(t)$ |
| `R_weight`, `G_weight`, `B_weight` | 当前 $P(t)$ 反解得到的 RGB 权重 |
| `Ra`, `Rmin_8` | 当前混合光谱的显色指标 |

---

## 结论

新模型不再寻找“单白光与某一个固定 RGB 白光之间的最佳比例”。在目标颜色点 $T$ 固定的前提下，只要候选 RGB 点 $P(t)$ 固定，混光比例也随之固定。真正被优化的是 $P(t)$ 沿着单白光到目标点再延伸到 RGB 色域边界这条射线的位置；每一个位置都重新反解 RGB 权重，并保证最终混合光谱仍然落在同一个目标 xyY 上。
