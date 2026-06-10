function weightedPWM = scale_pwm_by_coeff(basePWM, coeff)
%SCALE_PWM_BY_COEFF 按线段权重缩放某个色点的 R/G/B 比例。
%
% 例如：
%   whitePWM = scale_pwm_by_coeff(whiteBasePWM, whiteCoeff)

    weightedPWM = coeff * basePWM;
end
