function result = white_mix_intersection(targetData, whiteXY, config)
%WHITE_MIX_INTERSECTION 批量执行“白点 + 交点”混光验证。
% 输入：
%   targetData : Nx3 的 [x y Y]，也兼容 Nx2 的 [x y]
%   whiteXY    : 白点 [x y]
%   config     : 配置结构体
%
% 输出：
%   result     : 便于查看和导出的中文表格

    if nargin < 2
        whiteXY = [0.3090 0.3176];
    end
    if nargin < 3 || isempty(config)
        config = default_white_mix_config();
    end

    targetData = double(targetData);
    whiteXY = double(whiteXY(:).');

    if size(targetData, 2) ~= 2 && size(targetData, 2) ~= 3
        error('white_mix_intersection:InvalidTarget', ...
            'targetData must be an Nx2 or Nx3 matrix.');
    end

    if size(targetData, 2) == 2
        targetXY = targetData;
        targetY = targetData(:, 2);
    else
        targetXY = targetData(:, 1:2);
        targetY = targetData(:, 3);
    end

    n = size(targetData, 1);

    index = (1:n).';
    isValid = false(n, 1);
    statusText = cell(n, 1);
    edgeName = cell(n, 1);

    intersectXY = NaN(n, 2);
    rayT = NaN(n, 1);

    k1 = NaN(n, 1);
    k2 = NaN(n, 1);
    k3 = NaN(n, 1);
    oneMinusLambda = NaN(n, 1);
    lambda = NaN(n, 1);

    whitePartY = NaN(n, 1);
    pointPartY = NaN(n, 1);
    splitSumY = NaN(n, 1);
    splitErrorY = NaN(n, 1);
    whiteYRatio = NaN(n, 1);
    pointYRatio = NaN(n, 1);

    whitePWM = NaN(n, 3);
    pointPWM = NaN(n, 3);
    mixPWM = NaN(n, 3);
    backInputPWM = NaN(n, 3);

    verifyXY = NaN(n, 2);
    verifyY = NaN(n, 1);
    errorXY = NaN(n, 2);
    errorY = NaN(n, 1);
    deltaUV = NaN(n, 1);
    deltaY = NaN(n, 1);
    relativeDeltaY = NaN(n, 1);

    pointBackXY = NaN(n, 2);
    pointBackDeltaUV = NaN(n, 1);

    for i = 1:n
        step = white_mix_one_target([targetXY(i, :), targetY(i)], whiteXY, config);

        isValid(i) = step.IsValid;
        statusText{i} = step.Status;
        edgeName{i} = step.Edge;

        intersectXY(i, :) = step.IntersectXY;
        rayT(i) = step.Ray_t;

        k1(i) = step.Coeff.k1;
        k2(i) = step.Coeff.k2;
        k3(i) = step.Coeff.k3;
        oneMinusLambda(i) = step.Coeff.oneMinusLambda;
        lambda(i) = step.Coeff.lambda;

        whitePartY(i) = step.Luminance.WhiteY;
        pointPartY(i) = step.Luminance.PointY;
        splitSumY(i) = step.SplitSumY;
        splitErrorY(i) = step.SplitErrorY;
        whiteYRatio(i) = step.Luminance.WhiteYRatio;
        pointYRatio(i) = step.Luminance.PointYRatio;

        whitePWM(i, :) = step.WhitePWM;
        pointPWM(i, :) = step.PointPWM;
        mixPWM(i, :) = step.MixPWM;
        backInputPWM(i, :) = step.MixPWM;

        verifyXY(i, :) = step.VerifyXY;
        verifyY(i) = step.VerifyY;
        errorXY(i, :) = step.ErrorXY;
        errorY(i) = step.ErrorY;
        deltaUV(i) = step.Delta_uv;
        deltaY(i) = step.DeltaY;
        relativeDeltaY(i) = step.RelativeDeltaY;

        pointBackXY(i, :) = step.PointBackXY;
        pointBackDeltaUV(i) = step.PointBack_delta_uv;
    end

    result = table( ...
        index, ...
        isValid, statusText, ...
        targetXY(:, 1), targetXY(:, 2), ...
        intersectXY(:, 1), intersectXY(:, 2), ...
        edgeName, rayT, ...
        k1, k2, k3, oneMinusLambda, lambda, ...
        targetY, whitePartY, pointPartY, splitSumY, splitErrorY, whiteYRatio, pointYRatio, ...
        whitePWM(:, 1), whitePWM(:, 2), whitePWM(:, 3), ...
        pointPWM(:, 1), pointPWM(:, 2), pointPWM(:, 3), ...
        mixPWM(:, 1), mixPWM(:, 2), mixPWM(:, 3), ...
        backInputPWM(:, 1), backInputPWM(:, 2), backInputPWM(:, 3), ...
        verifyXY(:, 1), verifyXY(:, 2), verifyY, ...
        errorXY(:, 1), errorXY(:, 2), errorY, deltaUV, deltaY, relativeDeltaY, ...
        pointBackXY(:, 1), pointBackXY(:, 2), pointBackDeltaUV, ...
        'VariableNames', { ...
            '序号', ...
            '是否有效', '状态', ...
            '目标x', '目标y', ...
            '交点x', '交点y', ...
            '相交边', '射线t', ...
            'k1交点到目标', 'k2目标到白点', 'k3交点到白点', ...
            '一减lambda', 'lambda', ...
            '目标Y', '白点分量Y', '交点分量Y', '亮度分解和', '亮度分解误差', ...
            '白点亮度占比', '交点亮度占比', ...
            '白点分解R', '白点分解G', '白点分解B', ...
            '交点分解R', '交点分解G', '交点分解B', ...
            '混合PWM_R', '混合PWM_G', '混合PWM_B', ...
            '回算输入R', '回算输入G', '回算输入B', ...
            '验证x', '验证y', '验证Y', ...
            '误差x', '误差y', '误差Y', '验证DeltaUV', '验证DeltaY', '验证相对DeltaY', ...
            '交点回算x', '交点回算y', '交点回算DeltaUV'});
end
