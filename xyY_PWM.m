function [Dred,Dgreen,Dblue,info] = xyY_PWM(xyY_red,xyY_green,xyY_blue,xyY_target)
% Double-precision but numerically more stable:
%   1) Form XYZ via Y/y * [x;y;z]
%   2) Column scaling to improve conditioning
%   3) Use backslash (QR) rather than inv
%   4) Report condition number & residual
%
% 中文说明：
% 这个函数的作用是：给定 RGB 三个基色的 xyY，以及一个目标 xyY，
% 求出目标色需要多少比例的 Red、Green、Blue。
% 输出 Dred、Dgreen、Dblue 是三基色的线性混合比例。

    % 取出三个基色和目标色的 x、y、Y。
    xr=xyY_red(1);   yr=xyY_red(2);   Yr=xyY_red(3);
    xg=xyY_green(1); yg=xyY_green(2); Yg=xyY_green(3);
    xb=xyY_blue(1);  yb=xyY_blue(2);  Yb=xyY_blue(3);
    xt=xyY_target(1);yt=xyY_target(2);Yt=xyY_target(3);

    % CIE XYZ 中，z = 1 - x - y。
    zr = 1 - xr - yr;  zg = 1 - xg - yg;  zb = 1 - xb - yb;  zt = 1 - xt - yt;

    % 防止 y 太小导致除 0。
    epsy = 1e-18;
    Yr_over_yr = Yr / max(yr,epsy);
    Yg_over_yg = Yg / max(yg,epsy);
    Yb_over_yb = Yb / max(yb,epsy);

    % 把三个基色从 xyY 转成 XYZ。
    % 公式：
    %   X = Y / y * x
    %   Z = Y / y * z
    Xr = Yr_over_yr * xr;   YrXYZ = Yr;         Zr = Yr_over_yr * zr;
    Xg = Yg_over_yg * xg;   YgXYZ = Yg;         Zg = Yg_over_yg * zg;
    Xb = Yb_over_yb * xb;   YbXYZ = Yb;         Zb = Yb_over_yb * zb;

    % 构造 3x3 基色矩阵。
    % 每一列是一个基色的 XYZ：
    %   M * [Dred; Dgreen; Dblue] = TargetXYZ
    M = [Xr, Xg, Xb;
         YrXYZ, YgXYZ, YbXYZ;
         Zr, Zg, Zb];

    % 把目标色 xyY 也转成目标 XYZ。
    Yt_over_yt = Yt / max(yt,epsy);
    Xt = Yt_over_yt * xt;  Zt = Yt_over_yt * zt;
    T = [Xt; Yt; Zt];

    % ---- 列缩放：改善矩阵条件数，让求解更稳定 ----
    % 直接解 M \ T 也可以，但当矩阵数值尺度差异大时误差会变大。
    % 这里先把每一列缩放到相近尺度，求解后再缩放回来。
    cn = max(abs(M),[],1);             % column norms (max-abs)
    S  = diag(1./max(cn,eps));         % scale columns ~1
    Ms = M * S;                        % scaled system
    % 先解 Ms * u = T，再用 D = S * u 还原真正的三基色比例。
    u  = Ms \ T;
    D  = S * u;

    % 输出三个基色的比例。
    Dred = D(1); Dgreen = D(2); Dblue = D(3);

    % 诊断信息：条件数和残差，用来判断求解是否稳定。
    info.cond_M   = cond(M);
    info.cond_Ms  = cond(Ms);
    info.residual = norm(M*D - T, 2);
end
