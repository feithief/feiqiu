function [Rpwm, Gpwm, Bpwm, info] = xyY_PWM(xyY_red, xyY_green, xyY_blue, xyY_target)
%XYY_PWM 用固定三基色和目标 xyY 计算最终 PWM。
%
% 这个版本对应你给出的两张公式图，分成两步：
%
% 第 1 步：只根据色度坐标求三基色系数 D
%
%   z_target = 1 - x_target - y_target
%   z_red    = 1 - x_red    - y_red
%   z_green  = 1 - x_green  - y_green
%   z_blue   = 1 - x_blue   - y_blue
%
%   [D_red; D_green; D_blue] = M^(-1) * [x_target; y_target; z_target]
%
% 其中
%   M = [ (Y_red/y_red)*x_red    (Y_green/y_green)*x_green    (Y_blue/y_blue)*x_blue
%         (Y_red/y_red)*y_red    (Y_green/y_green)*y_green    (Y_blue/y_blue)*y_blue
%         (Y_red/y_red)*z_red    (Y_green/y_green)*z_green    (Y_blue/y_blue)*z_blue ]
%
% 第 2 步：再把目标亮度 Y_target 加进去，得到最终 PWM
%
%   R_PWM = Y_target / (Y_red*D_red + Y_green*D_green + Y_blue*D_blue) * MaxPWM * D_red
%   G_PWM = Y_target / (Y_red*D_red + Y_green*D_green + Y_blue*D_blue) * MaxPWM * D_green
%   B_PWM = Y_target / (Y_red*D_red + Y_green*D_green + Y_blue*D_blue) * MaxPWM * D_blue
%
% 这里 MaxPWM 固定为 32000。
%
% 输出：
%   Rpwm, Gpwm, Bpwm : 最终 PWM 计数
%   info             : 中间量，包含 D 系数、矩阵、缩放因子等

    xr = xyY_red(1);    yr = xyY_red(2);    Yr = xyY_red(3);
    xg = xyY_green(1);  yg = xyY_green(2);  Yg = xyY_green(3);
    xb = xyY_blue(1);   yb = xyY_blue(2);   Yb = xyY_blue(3);

    xt = xyY_target(1); yt = xyY_target(2); Yt = xyY_target(3);

    zr = 1 - xr - yr;
    zg = 1 - xg - yg;
    zb = 1 - xb - yb;
    zt = 1 - xt - yt;

    epsy = 1e-18;
    Yr_over_yr = Yr / max(yr, epsy);
    Yg_over_yg = Yg / max(yg, epsy);
    Yb_over_yb = Yb / max(yb, epsy);

    M = [ ...
        Yr_over_yr * xr,  Yg_over_yg * xg,  Yb_over_yb * xb; ...
        Yr_over_yr * yr,  Yg_over_yg * yg,  Yb_over_yb * yb; ...
        Yr_over_yr * zr,  Yg_over_yg * zg,  Yb_over_yb * zb];

    targetVector = [xt; yt; zt];

    if rcond(M) <= eps
        error('xyY_PWM:SingularMatrix', ...
            'The primary matrix M is singular or nearly singular.');
    end

    M_inverse = inv(M);
    D = M_inverse * targetVector;
    D(abs(D) < 1e-12) = 0;

    luminanceDenominator = Yr * D(1) + Yg * D(2) + Yb * D(3);
    if abs(luminanceDenominator) <= eps
        error('xyY_PWM:ZeroLuminanceDenominator', ...
            'The luminance denominator is too small.');
    end

    maxPWM = fixed_max_pwm();
    scaleFactor = Yt / luminanceDenominator * maxPWM;
    PWM = scaleFactor * D;
    PWM(abs(PWM) < 1e-10) = 0;

    Rpwm = PWM(1);
    Gpwm = PWM(2);
    Bpwm = PWM(3);

    info.maxPWM = maxPWM;
    info.z_target = zt;
    info.z_red = zr;
    info.z_green = zg;
    info.z_blue = zb;
    info.matrix = M;
    info.matrix_inverse = M_inverse;
    info.matrix_scaled = M;
    info.targetVector = targetVector;
    info.D = D;
    info.Dred = D(1);
    info.Dgreen = D(2);
    info.Dblue = D(3);
    info.luminanceDenominator = luminanceDenominator;
    info.scaleFactor = scaleFactor;
    info.PWM = PWM;
    info.RGB32000Input = PWM;
    info.cond_M = cond(M);
    info.cond_Ms = cond(M);
    info.residualD = norm(M * D - targetVector, 2);
    info.predictedY = (Yr * PWM(1) + Yg * PWM(2) + Yb * PWM(3)) / maxPWM;
end
