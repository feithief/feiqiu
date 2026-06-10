# 单白灯与 RGB 白光同色异谱混合提升 CRI 的计算方法

本文基于光谱线性叠加原理，先由 R、G、B 三通道求解目标白点对应的 RGB 合成白光，再将 RGB 白光与单白灯白光统一到相同亮度尺度，最后扫描混合比例并计算 R1 至 R8、Ra 和 Rmin8，从而确定最佳同色异谱混合方案。

---

## 1. 光谱统一与基础色度计算

所有光谱统一到 380 nm 至 780 nm，间隔 5 nm：

$$
\lambda=380,385,\cdots,780,\qquad \Delta\lambda=5\text{ nm}
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

---

## 2. RGB 目标白光权重求解

m 文件中的输入为：R(0.7013, 0.2979, 1.02)、G(0.1432, 0.7249, 1.8628)、B(0.1510, 0.0303, 0.35)、目标白点 T(0.3090, 0.3176, 2.39)。先将 xyY 转为 XYZ：

$$
X_c=\frac{x_c}{y_c}Y_c,\qquad
Y_c=Y_c,\qquad
Z_c=\frac{1-x_c-y_c}{y_c}Y_c,\qquad
c\in\{R,G,B,T\}
$$

由 R、G、B 的 XYZ 构造矩阵并求解 RGB 权重：

$$
A=
\begin{bmatrix}
X_R&X_G&X_B\\
Y_R&Y_G&Y_B\\
Z_R&Z_G&Z_B
\end{bmatrix},
\qquad
\begin{bmatrix}
r\\g\\b
\end{bmatrix}
=
A^{-1}
\begin{bmatrix}
X_T\\Y_T\\Z_T
\end{bmatrix}
$$

本次结果为：

$$
r=0.640411080154,\qquad
g=0.882500416468,\qquad
b=0.265311206990
$$

---

## 3. 通道亮度校准与 RGB 白光合成

R、G、B 不校准到同一个亮度，而是分别校准到各自给定的光通量：R = 1.02 lm，G = 1.8628 lm，B = 0.35 lm。通用校准公式为：

$$
\alpha_c=\frac{Y_c}{683\sum S_c^{raw}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_c(\lambda)=\alpha_cS_c^{raw}(\lambda),\qquad
c\in\{R,G,B\}
$$

随后合成 RGB 初始白光，并整体归一到目标亮度 2.39 lm：

$$
S_{RGB0}(\lambda)=rS_R(\lambda)+gS_G(\lambda)+bS_B(\lambda)
$$

$$
\alpha_T=\frac{2.39}{683\sum S_{RGB0}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_{RGB}(\lambda)=\alpha_TS_{RGB0}(\lambda)
$$

代入本次权重：

$$
S_{RGB0}(\lambda)=0.640411080154S_R(\lambda)+0.882500416468S_G(\lambda)+0.265311206990S_B(\lambda)
$$

---

## 4. 单白灯与 RGB 白光同亮度混合

混合前，单白灯白光和 RGB 合成白光都归一到 2.39 lm，使 $p$ 表示同亮度白光之间的混合比例。

$$
S_{mix}(\lambda,p)=pS_{SingleNorm}(\lambda)+(1-p)S_{RGBNorm}(\lambda),\qquad
p=0,0.001,\cdots,1
$$

其中，$p=1$ 为纯单白灯，$p=0$ 为纯 RGB 同色异谱白光。

---

## 5. CRI 计算与最优比例筛选

对每一个 $p$，将 $S_{mix}(\lambda,p)$ 作为测试光源，并根据 CCT 生成参考光源：

$$
CCT<5000K:\ S_{ref}=S_{BB}(\lambda,T),\qquad
CCT\geq5000K:\ S_{ref}=S_D(\lambda,T)
$$

标准色样反射光谱和色样 XYZ 为：

$$
S_{i,test}=S_{mix}(\lambda,p)\rho_i(\lambda),\qquad
S_{i,ref}=S_{ref}(\lambda)\rho_i(\lambda)
$$

$$
k_j=\frac{100}{\sum S_j(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
[X_{i,j},Y_{i,j},Z_{i,j}]=k_j\sum S_j(\lambda)\rho_i(\lambda)[\overline{x}(\lambda),\overline{y}(\lambda),\overline{z}(\lambda)]\Delta\lambda
$$

将色样 XYZ 转为 CIE 1960 uv，并进行测试光源到参考光源的色适应：

$$
u=\frac{4X}{X+15Y+3Z},\qquad
v=\frac{6Y}{X+15Y+3Z}
$$

$$
c=\frac{4-u-10v}{v},\quad
d=\frac{1.708v+0.404-1.481u}{v},\quad
D_i=16.518+1.481\frac{c_r}{c_t}c_i-\frac{d_r}{d_t}d_i
$$

$$
u_i^a=\frac{10.872+0.404\frac{c_r}{c_t}c_i-4\frac{d_r}{d_t}d_i}{D_i},\qquad
v_i^a=\frac{5.520}{D_i}
$$

最后计算 UVW 色差、单项显色指数和筛选指标：

$$
W_i^*=25Y_i^{\frac{1}{3}}-17,\qquad
U_i^*=13W_i^*(u_i^a-u_r),\qquad
V_i^*=13W_i^*(v_i^a-v_r)
$$

$$
\Delta E_i=\sqrt{(U_i^*-U_{i,ref}^*)^2+(V_i^*-V_{i,ref}^*)^2+(W_i^*-W_{i,ref}^*)^2},\qquad
R_i=100-4.6\Delta E_i
$$

$$
R_a(p)=\frac{1}{8}\sum_{i=1}^{8}R_i(p),\qquad
R_{min,8}(p)=\min_{1\leq i\leq8}R_i(p)
$$

$$
p_{Ra}=\arg\max_{0\leq p\leq1}R_a(p),\qquad
p_{min}=\arg\max_{0\leq p\leq1}R_{min,8}(p)
$$

---

## 计算结果

| 优化目标 | 单白灯比例 | RGB 白光比例 | Ra | Rmin8 | xy |
|---|---:|---:|---:|---:|---|
| Best Ra | 0.869 | 0.131 | 94.5729 | 88.5835 | (0.30835, 0.31639) |
| Best Rmin8 | 0.893 | 0.107 | 93.7224 | 88.9348 | (0.30833, 0.31637) |

---

## 结论

本文方法先将 R、G、B 原始光谱分别校准到各自给定的光通量尺度，再求解目标白点对应的 RGB 合成权重，得到 RGB 目标白光。随后，RGB 目标白光与单白灯白光被统一到 2.39 lm，并按比例扫描叠加。结果表明，当单白灯比例为 0.869、RGB 白光比例为 0.131 时，Ra 达到 94.5729；当单白灯比例为 0.893、RGB 白光比例为 0.107 时，Rmin8 达到 88.9348。因此，RGB 白光在该方法中不是单独作为高显色白光使用，而是作为小比例光谱修正量，提高单白灯在 R1 至 R8 色样上的显色性能。
