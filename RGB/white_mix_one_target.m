function step = white_mix_one_target(targetData, whiteXY, config)
%WHITE_MIX_ONE_TARGET Run the full white-point mixing flow for one target.

    if nargin < 2 || isempty(whiteXY)
        whiteXY = [0.3090 0.3176];
    end
    if nargin < 3 || isempty(config)
        config = default_white_mix_config();
    end

    targetData = double(targetData(:).');
    whiteXY = double(whiteXY(:).');

    if numel(targetData) == 2
        step.TargetXY = targetData;
        step.TargetY = targetData(2);
    elseif numel(targetData) == 3
        step.TargetXY = targetData(1:2);
        step.TargetY = targetData(3);
    else
        error('white_mix_one_target:InvalidTarget', ...
            'targetData must be [x y] or [x y Y].');
    end
    step.Target_xyY = [step.TargetXY, step.TargetY];
    step.IsValid = true;
    step.Status = 'ok';

    step.Config = config;
    step.IntersectionPrimaries = config.intersectionPrimaries;
    step.VerifyPrimaries = config.verifyPrimaries;

    if norm(step.TargetXY - whiteXY) <= eps
        step.IntersectXY = [NaN NaN];
        step.Edge = 'white-only';
        step.Ray_t = NaN;
        step.Coeff.k1 = 0;
        step.Coeff.k2 = 0;
        step.Coeff.k3 = 0;
        step.Coeff.oneMinusLambda = 1;
        step.Coeff.lambda = 0;
        step.Coeff.whiteCoeff = 1;
        step.Coeff.pointCoeff = 0;
    else
        try
            [pointXY, rayT, edgeName] = find_gamut_intersection( ...
                whiteXY, step.TargetXY, config.intersectionPrimaries);
            step.IntersectXY = pointXY;
            step.Edge = edgeName;
            step.Ray_t = rayT;
            step.Coeff = calc_line_mix_coefficients(whiteXY, step.TargetXY, pointXY);
        catch ME
            if strcmp(ME.identifier, 'find_gamut_intersection:NoIntersection')
                step.IsValid = false;
                step.Status = 'no-intersection';
                step.IntersectXY = [NaN NaN];
                step.Edge = 'no-intersection';
                step.Ray_t = NaN;
                step.Coeff.k1 = NaN;
                step.Coeff.k2 = NaN;
                step.Coeff.k3 = NaN;
                step.Coeff.oneMinusLambda = NaN;
                step.Coeff.lambda = NaN;
                step.Coeff.whiteCoeff = NaN;
                step.Coeff.pointCoeff = NaN;

                step.Luminance = struct( ...
                    'TargetY', step.TargetY, ...
                    'lambda', NaN, ...
                    'oneMinusLambda', NaN, ...
                    'WhiteY', NaN, ...
                    'PointY', NaN, ...
                    'White_xyY', [NaN NaN NaN], ...
                    'Point_xyY', [NaN NaN NaN], ...
                    'SumY', NaN, ...
                    'SumError', NaN, ...
                    'WhiteYRatio', NaN, ...
                    'PointYRatio', NaN);

                step.WhiteComponent_xyY = [NaN NaN NaN];
                step.PointComponent_xyY = [NaN NaN NaN];
                step.WhitePWM = [NaN NaN NaN];
                step.PointPWM = [NaN NaN NaN];
                step.MixPWM = [NaN NaN NaN];
                step.WhitePWMInfo = make_invalid_pwm_info(config.verifyPrimaries);
                step.PointPWMInfo = make_invalid_pwm_info(config.verifyPrimaries);
                step.Verify_xyY = [NaN NaN NaN];
                step.VerifyXY = [NaN NaN];
                step.VerifyY = NaN;
                step.ErrorXY = [NaN NaN];
                step.ErrorY = NaN;
                step.Delta_uv = NaN;
                step.DeltaY = NaN;
                step.RelativeDeltaY = NaN;
                step.SplitSumY = NaN;
                step.SplitErrorY = NaN;
                step.PointBack_xyY = [NaN NaN NaN];
                step.PointBackXY = [NaN NaN];
                step.PointBackY = NaN;
                step.PointBack_delta_uv = NaN;
                step.PointBack_deltaY = NaN;
                return;
            else
                rethrow(ME);
            end
        end
    end

    step.Luminance = split_target_Y_by_lambda( ...
        step.Target_xyY, whiteXY, step.IntersectXY, step.Coeff);

    step.WhiteComponent_xyY = step.Luminance.White_xyY;
    step.PointComponent_xyY = step.Luminance.Point_xyY;

    [step.WhitePWM, step.WhitePWMInfo] = xyY_to_pwm_fixed( ...
        step.WhiteComponent_xyY, config.verifyPrimaries);
    if any(isnan(step.PointComponent_xyY(1:2))) || step.Luminance.PointY == 0
        step.PointPWM = [0 0 0];
        step.PointPWMInfo = struct( ...
            'z_target', NaN, ...
            'D', [NaN; NaN; NaN], ...
            'luminanceDenominator', NaN, ...
            'scaleFactor', 0, ...
            'PWM', [0; 0; 0], ...
            'predictedY', 0, ...
            'usedPrimaries', config.verifyPrimaries);
    else
        [step.PointPWM, step.PointPWMInfo] = xyY_to_pwm_fixed( ...
            step.PointComponent_xyY, config.verifyPrimaries);
    end

    step.MixPWM = mix_pwm_channels(step.WhitePWM, step.PointPWM);

    step.Verify_xyY = pwm_to_xyY_fixed(step.MixPWM, config.verifyPrimaries);
    step.VerifyXY = step.Verify_xyY(1, 1:2);
    step.VerifyY = step.Verify_xyY(1, 3);
    step.ErrorXY = step.VerifyXY - step.TargetXY;
    step.ErrorY = step.VerifyY - step.TargetY;
    step.Delta_uv = delta_uv_from_xy(step.VerifyXY, step.TargetXY);
    step.DeltaY = abs(step.ErrorY);
    step.RelativeDeltaY = step.DeltaY / max(abs(step.TargetY), eps);

    step.SplitSumY = step.Luminance.SumY;
    step.SplitErrorY = step.Luminance.SumError;

    if any(~isnan(step.IntersectXY)) && step.Luminance.PointY ~= 0
        step.PointBack_xyY = pwm_to_xyY_fixed(step.PointPWM, config.verifyPrimaries);
        step.PointBackXY = step.PointBack_xyY(1, 1:2);
        step.PointBackY = step.PointBack_xyY(1, 3);
        step.PointBack_delta_uv = delta_uv_from_xy(step.PointBackXY, step.IntersectXY);
        step.PointBack_deltaY = abs(step.PointBackY - step.Luminance.PointY);
    else
        step.PointBack_xyY = [NaN NaN NaN];
        step.PointBackXY = [NaN NaN];
        step.PointBackY = NaN;
        step.PointBack_delta_uv = NaN;
        step.PointBack_deltaY = NaN;
    end
end

function info = make_invalid_pwm_info(primaries)
    info = struct( ...
        'z_target', NaN, ...
        'D', [NaN; NaN; NaN], ...
        'luminanceDenominator', NaN, ...
        'scaleFactor', NaN, ...
        'PWM', [NaN; NaN; NaN], ...
        'predictedY', NaN, ...
        'usedPrimaries', primaries);
end
