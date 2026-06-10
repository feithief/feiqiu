# 附录 A 原理性公式汇总

## A.1 CIE 标准观察者函数与波长轴

$$
\lambda=380,385,\cdots,780,\qquad \Delta\lambda=5\text{ nm}
$$

$$
\overline{x}(\lambda),\qquad \overline{y}(\lambda),\qquad \overline{z}(\lambda)
$$

其中，$\overline{x}(\lambda)$、$\overline{y}(\lambda)$、$\overline{z}(\lambda)$ 为 CIE 1931 2° 标准观察者函数，是光谱转换为 XYZ 的人眼响应权重。

---

## A.2 光谱到 XYZ、xy 与光通量

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

## A.3 xyY 到 XYZ

$$
X_c=\frac{x_c}{y_c}Y_c,\qquad
Y_c=Y_c,\qquad
Z_c=\frac{1-x_c-y_c}{y_c}Y_c,\qquad
c\in\{R,G,B,T\}
$$

---

## A.4 RGB 权重求解

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

$$
r=0.640411080154,\qquad
g=0.882500416468,\qquad
b=0.265311206990
$$

---

## A.5 通道光谱亮度校准

$$
\alpha_c=\frac{Y_c}{683\sum S_c^{raw}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_c(\lambda)=\alpha_cS_c^{raw}(\lambda),\qquad
c\in\{R,G,B\}
$$

$$
Y_R=1.02\text{ lm},\qquad
Y_G=1.8628\text{ lm},\qquad
Y_B=0.35\text{ lm}
$$

---

## A.6 RGB 白光合成与目标亮度归一化

$$
S_{RGB0}(\lambda)=rS_R(\lambda)+gS_G(\lambda)+bS_B(\lambda)
$$

$$
S_{RGB0}(\lambda)=0.640411080154S_R(\lambda)+0.882500416468S_G(\lambda)+0.265311206990S_B(\lambda)
$$

$$
\alpha_T=\frac{2.39}{683\sum S_{RGB0}(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
S_{RGB}(\lambda)=\alpha_TS_{RGB0}(\lambda)
$$

---

## A.7 单白灯与 RGB 白光混合

$$
S_{SingleNorm}(\lambda)\rightarrow 2.39\text{ lm},\qquad
S_{RGBNorm}(\lambda)\rightarrow 2.39\text{ lm}
$$

$$
S_{mix}(\lambda,p)=pS_{SingleNorm}(\lambda)+(1-p)S_{RGBNorm}(\lambda),\qquad
p=0,0.001,\cdots,1
$$

---

## A.8 CCT、Duv 与参考光源

$$
u=\frac{4X}{X+15Y+3Z},\qquad
v=\frac{6Y}{X+15Y+3Z}
$$

$$
CCT=\arg\min_T\left[(u_{test}-u_{BB}(T))^2+(v_{test}-v_{BB}(T))^2\right]
$$

$$
Duv=\sqrt{(u_{test}-u_{BB}(CCT))^2+(v_{test}-v_{BB}(CCT))^2}
$$

$$
CCT<5000K:\ S_{ref}(\lambda)=S_{BB}(\lambda,T),\qquad
CCT\geq5000K:\ S_{ref}(\lambda)=S_D(\lambda,T)
$$

---

## A.9 黑体参考光谱与日光参考光谱

$$
S_{BB}(\lambda,T)=\frac{1}{\lambda^5\left(e^{\frac{c_2}{\lambda T}}-1\right)}
$$

$$
S_D(\lambda,T)=S_0(\lambda)+M_1S_1(\lambda)+M_2S_2(\lambda)
$$

$$
y_D=-3x_D^2+2.870x_D-0.275
$$

$$
T\leq7000K:\ x_D=0.244063+\frac{0.09911\times10^3}{T}+\frac{2.9678\times10^6}{T^2}-\frac{4.6070\times10^9}{T^3}
$$

$$
T>7000K:\ x_D=0.237040+\frac{0.24748\times10^3}{T}+\frac{1.9018\times10^6}{T^2}-\frac{2.0064\times10^9}{T^3}
$$

$$
M_1=\frac{-1.3515-1.7703x_D+5.9114y_D}{0.0241+0.2562x_D-0.7341y_D},\qquad
M_2=\frac{0.0300-31.4424x_D+30.0717y_D}{0.0241+0.2562x_D-0.7341y_D}
$$

---

## A.10 标准色样反射光谱与色样 XYZ

$$
S_{i,test}(\lambda,p)=S_{mix}(\lambda,p)\rho_i(\lambda),\qquad
S_{i,ref}(\lambda)=S_{ref}(\lambda)\rho_i(\lambda),\qquad
i=1,2,\cdots,8
$$

$$
k_j=\frac{100}{\sum S_j(\lambda)\overline{y}(\lambda)\Delta\lambda},\qquad
j\in\{test,ref\}
$$

$$
[X_{i,j},Y_{i,j},Z_{i,j}]
=
k_j\sum S_j(\lambda)\rho_i(\lambda)
[\overline{x}(\lambda),\overline{y}(\lambda),\overline{z}(\lambda)]
\Delta\lambda
$$

---

## A.11 CIE 1960 uv 与色适应

$$
u=\frac{4X}{X+15Y+3Z},\qquad
v=\frac{6Y}{X+15Y+3Z}
$$

$$
c=\frac{4-u-10v}{v},\qquad
d=\frac{1.708v+0.404-1.481u}{v}
$$

$$
D_i=16.518+1.481\frac{c_r}{c_t}c_i-\frac{d_r}{d_t}d_i
$$

$$
u_i^a=\frac{10.872+0.404\frac{c_r}{c_t}c_i-4\frac{d_r}{d_t}d_i}{D_i},\qquad
v_i^a=\frac{5.520}{D_i}
$$

---

## A.12 UVW 色差与显色指数

$$
W_i^*=25Y_i^{\frac{1}{3}}-17,\qquad
U_i^*=13W_i^*(u_i^a-u_r),\qquad
V_i^*=13W_i^*(v_i^a-v_r)
$$

$$
\Delta E_i=
\sqrt{(U_i^*-U_{i,ref}^*)^2+(V_i^*-V_{i,ref}^*)^2+(W_i^*-W_{i,ref}^*)^2}
$$

$$
R_i=100-4.6\Delta E_i
$$

---

## A.13 Ra、Rmin8 与最优比例筛选

$$
R_a(p)=\frac{1}{8}\sum_{i=1}^{8}R_i(p),\qquad
R_{min,8}(p)=\min_{1\leq i\leq8}R_i(p)
$$

$$
p_{Ra}=\arg\max_{0\leq p\leq1}R_a(p),\qquad
p_{min}=\arg\max_{0\leq p\leq1}R_{min,8}(p)
$$
