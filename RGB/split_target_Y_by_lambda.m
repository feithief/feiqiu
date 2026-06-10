function split = split_target_Y_by_lambda(targetxyY, whiteXY, pointXY, coeff)
%SPLIT_TARGET_Y_BY_LAMBDA 用 lambda 把目标亮度 Y 分解为白点分量和交点分量。
%
% 这里直接对应你给出的推导：
%
%   x = (1-lambda)*x1 + lambda*x2
%   y = (1-lambda)*y1 + lambda*y2
%
%   Y1 = Y * ((1-lambda) * y1 / y)
%   Y2 = Y * (lambda * y2 / y)
%
% 其中：
%   1-lambda = k1 / k3
%   lambda   = k2 / k3
%
% 输入：
%   targetxyY : [x_t y_t Y_t]，如果只给 [x_t y_t]，则兼容旧逻辑，默认 Y_t = y_t
%   whiteXY   : 白点 [x_w y_w]
%   pointXY   : 交点 [x_p y_p]
%   coeff     : calc_line_mix_coefficients.m 输出的结构体
%
% 输出：
%   split.WhiteY / split.PointY   : 白点分量亮度、交点分量亮度
%   split.SumY / split.SumError   : Y1+Y2 及其与目标 Y 的误差
%   split.White_xyY / Point_xyY   : 两个分量各自的 xyY

    targetxyY = double(targetxyY(:).');
    whiteXY = double(whiteXY(:).');
    pointXY = double(pointXY(:).');

    if numel(targetxyY) == 2
        targetxyY = [targetxyY, targetxyY(2)];
    end

    if numel(targetxyY) ~= 3
        error('split_target_Y_by_lambda:InvalidTarget', ...
            'targetxyY must be [x y Y] or [x y].');
    end

    targetY = targetxyY(3);
    y = targetxyY(2);
    if abs(y) <= eps
        error('split_target_Y_by_lambda:InvalidTargetY', ...
            'Target y is too small, cannot split luminance.');
    end

    split.TargetY = targetY;
    split.lambda = coeff.lambda;
    split.oneMinusLambda = coeff.oneMinusLambda;

    if any(isnan(pointXY))
        split.WhiteY = targetY;
        split.PointY = 0;
        split.White_xyY = compose_xyY_from_xy_and_Y(whiteXY, split.WhiteY);
        split.Point_xyY = [NaN NaN 0];
    else
        y1 = whiteXY(2);
        y2 = pointXY(2);

        split.WhiteY = targetY * (split.oneMinusLambda * y1 / y);
        split.PointY = targetY * (split.lambda * y2 / y);

        split.White_xyY = compose_xyY_from_xy_and_Y(whiteXY, split.WhiteY);
        split.Point_xyY = compose_xyY_from_xy_and_Y(pointXY, split.PointY);
    end

    split.SumY = split.WhiteY + split.PointY;
    split.SumError = split.SumY - targetY;
    split.WhiteYRatio = split.WhiteY / max(abs(targetY), eps);
    split.PointYRatio = split.PointY / max(abs(targetY), eps);
end
