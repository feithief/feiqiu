function [summaryTable, errorTable, config, allResult] = run_test_white_mix_config(targetIndex)
%RUN_TEST_WHITE_MIX_CONFIG Standalone configuration entry for test_white_mix.
% You only need to edit this file.
%
% Usage:
%   run_test_white_mix_config
%   run_test_white_mix_config(2)
%   [summaryTable, errorTable, config] = run_test_white_mix_config(2)
%
% Configuration split:
%   1) intersectionTriangleXY : only used to compute the geometric intersection point P
%      Input format: 3x2, each row is [x y]
%   2) verifyPrimaries : used for xyY -> PWM and PWM -> xyY validation
%      Input format: 3x3, each row is [x y Y]

    if nargin < 1 || isempty(targetIndex)
        targetIndex = 4;
    end

    %% 1. Verification gamut triangle: full xyY input
    verifyPrimaries = [ ...
        0.7040 0.2970 1.5474; ...
        0.1568 0.7411 3.5194; ...
        0.1532 0.0310 0.5766];

    %% 2. Intersection triangle: direct xy input
    % This triangle is computed from the common intersection region of all
    % TtoCIE temperature-shifted gamut triangles. It is guaranteed to stay
    % inside every temperature-shifted triangle and is much less conservative
    % than the earlier similar-shrink example.
    intersectionTriangleXY = [ ...
        0.696009803665 0.302285065989; ...
        0.187053259340 0.712603373017; ...
        0.155869020184 0.039199210384];

    %% 3. Build config and call the main function
    config = default_white_mix_config(intersectionTriangleXY, verifyPrimaries);
    [summaryTable, errorTable, allResult] = test_white_mix(targetIndex, config);
end
