function mixPWM = mix_pwm_channels(whitePWM, pointPWM)
%MIX_PWM_CHANNELS 将白点和交点的 R/G/B 比例逐通道相加。
%
% 这是实际混光的通道相加步骤：
%   mixPWM = whitePWM + pointPWM

    mixPWM = whitePWM + pointPWM;
end
