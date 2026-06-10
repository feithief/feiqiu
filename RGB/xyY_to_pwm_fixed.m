function [pwm, info] = xyY_to_pwm_fixed(xyY, primaries)
%XYY_TO_PWM_FIXED Convert xyY to PWM using a configurable verification triangle.

    if nargin < 2
        primaries = [];
    end

    [xyY_red, xyY_green, xyY_blue] = fixed_rgb_primaries(primaries);
    [r, g, b, info] = xyY_PWM(xyY_red, xyY_green, xyY_blue, xyY);

    pwm = [r, g, b];
    pwm(abs(pwm) < 1e-10) = 0;
    info.usedPrimaries = [xyY_red; xyY_green; xyY_blue];
end
