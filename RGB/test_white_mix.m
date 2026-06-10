function [summaryTable, errorTable, allResult] = test_white_mix(targetIndex, config)
%TEST_WHITE_MIX Show one target color with configurable intersection/verification triangles.
%
% Usage:
%   [summaryTable, errorTable] = test_white_mix
%   [summaryTable, errorTable] = test_white_mix(2)
%
% The two triangles can be edited directly in this file:
%   intersectionTriangleXY : used only to find the ray/triangle intersection point P
%   verifyPrimaries        : used for xyY -> PWM and PWM -> xyY validation
%
% intersectionTriangleXY is a 3x2 matrix:
%   [x_r y_r;
%    x_g y_g;
%    x_b y_b]
%
% verifyPrimaries is a 3x3 matrix:
%   [x_r y_r Y_r;
%    x_g y_g Y_g;
%    x_b y_b Y_b]

    if nargin < 1 || isempty(targetIndex)
        targetIndex = 1;
    end

    if nargin < 2 || isempty(config)
        verifyPrimaries = [ ...
            0.7035 0.2952 0.7915; ...
            0.1788 0.7356 1.991; ...
            0.1490 0.0332 0.2312];


        intersectionTriangleXY = [ ...
            0.7035 0.2952; ...
            0.1788 0.7356; ...
            0.1490 0.0332];
        config = default_white_mix_config(intersectionTriangleXY, verifyPrimaries);
    end

    here = fileparts(mfilename('fullpath'));
    oldDir = pwd;
    cleanup = onCleanup(@() cd(oldDir)); %#ok<NASGU>
    cd(here);

    addpath(fileparts(here));

    whiteXY = [0.3152 0.3154];
    targetxyY = get_target_xyY_list();

    if targetIndex < 1 || targetIndex > size(targetxyY, 1)
        error('test_white_mix:InvalidTargetIndex', ...
            'targetIndex must be between 1 and %d.', size(targetxyY, 1));
    end

    step = white_mix_one_target(targetxyY(targetIndex, :), whiteXY, config);

    summaryTable = table( ...
        {'目标颜色'; '白点分量'; '交点分量'}, ...
        [step.Target_xyY(1); step.WhiteComponent_xyY(1); step.PointComponent_xyY(1)], ...
        [step.Target_xyY(2); step.WhiteComponent_xyY(2); step.PointComponent_xyY(2)], ...
        [step.Target_xyY(3); step.WhiteComponent_xyY(3); step.PointComponent_xyY(3)], ...
        'VariableNames', {'项目', 'x', 'y', 'Y'});

    errorTable = table( ...
        step.VerifyXY(1), step.VerifyXY(2), step.VerifyY, ...
        string(format_scientific_value(step.ErrorXY(1))), ...
        string(format_scientific_value(step.ErrorXY(2))), ...
        string(format_scientific_value(step.ErrorY)), ...
        string(format_scientific_value(step.Delta_uv)), ...
        string(format_scientific_value(step.DeltaY)), ...
        string(format_scientific_value(step.RelativeDeltaY)), ...
        string(format_scientific_value(step.SplitErrorY)), ...
        'VariableNames', { ...
            '验证x', '验证y', '验证Y', ...
            '误差x', '误差y', '误差Y', ...
            'DeltaUV', 'DeltaY', '相对DeltaY', '亮度分解误差'});

    fprintf('\n第 %d 个目标颜色的 xyY 及其分解结果:\n', targetIndex);
    disp(summaryTable);

    fprintf('\n第 %d 个目标颜色的误差:\n', targetIndex);
    disp(errorTable);

    fprintf('\n算交点色域三角形 (R/G/B 的 xy):\n');
    disp(config.intersectionPrimaries);

    fprintf('验算色域三角形 (R/G/B 的 xyY):\n');
    disp(config.verifyPrimaries);

    allResult = white_mix_intersection(targetxyY, whiteXY, config);

    plot_target_result(step, whiteXY, targetIndex);
    plot_all_color_results(allResult, whiteXY, config, targetIndex);
end
