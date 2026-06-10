function xyY = pwm_to_xyY_fixed(pwm, primaries)
%PWM_TO_XYY_FIXED Convert PWM back to xyY using a configurable verification triangle.

    if nargin < 2
        primaries = [];
    end

    xyY = RGB_to_xyY_32000(double(pwm), primaries);
end
