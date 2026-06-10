function coeff = calc_line_mix_coefficients(whiteXY, targetXY, pointXY)
%CALC_LINE_MIX_COEFFICIENTS 根据 W、T、P 三点距离计算共线参数。
%
% 三点顺序：
%   whiteXY ---- targetXY ---- pointXY
%
% 几何关系：
%   k1 = |pointXY - targetXY| = TP
%   k2 = |targetXY - whiteXY| = WT
%   k3 = |pointXY - whiteXY|  = WP
%
% 对应你推导里的参数：
%   1 - lambda = k1 / k3 = TP / WP
%   lambda     = k2 / k3 = WT / WP
%
% 因此目标点满足：
%   targetXY = (1-lambda) * whiteXY + lambda * pointXY

    coeff.k1 = norm(pointXY - targetXY);
    coeff.k2 = norm(targetXY - whiteXY);
    coeff.k3 = norm(pointXY - whiteXY);

    if coeff.k3 <= eps
        coeff.oneMinusLambda = 1;
        coeff.lambda = 0;
    else
        coeff.oneMinusLambda = coeff.k1 / coeff.k3;
        coeff.lambda = coeff.k2 / coeff.k3;
    end

    % 保留旧字段名，兼容之前的代码：
    %   whiteCoeff 对应 1-lambda
    %   pointCoeff 对应 lambda
    coeff.whiteCoeff = coeff.oneMinusLambda;
    coeff.pointCoeff = coeff.lambda;
end
