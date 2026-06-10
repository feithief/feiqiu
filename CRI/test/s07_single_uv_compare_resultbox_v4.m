% S07_SINGLE_UV_COMPARE_RESULTBOX
% 中文说明：
% 只计算一个 CIE 1976 u'v' 输入点。
%
% 输出内容：
%   1. 先单独输出 K点比例法结果
%   2. 再输出全部方法对比结果：
%      精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB
%   3. 每个方法输出：
%      Ra、RGB点、白色比例、RGB比例、PWM估算
%   4. 输出最优方法；如果多个方法 Ra 相同，全部列出。
%
% 注意：
%   PWM 估算规则：
%      RGB_PWM = clamp(round(R/G/B_weight * 255), 0, 255)
%      W_PWM   = clamp(round(SingleWhiteRatio * 255), 0, 255)
%   这里用于方法对比，不等同于量产代码中所有温度/电流/限幅后的最终PWM。

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S07 当前版本：单个 u''v'' 输入点，全部方法结果框输出\n');
fprintf('============================================================\n\n');

%% ================= 用户可配置参数 =================
% 输入 CIE 1976 u'v'
% 支持两种写法：
%   0.200 / 0.470  ：直接输入真实 u'v'
%   40    / 94     ：LIN/工程输入值，脚本会先 ×5，再 ÷1000，即 40→0.200，94→0.470
target_u1976_input = 103;
target_v1976_input = 104;

% 目标亮度 Y，单位沿用工程中的 lm / photopic Y 标定。
target_Y_lm = 3.0000;

nearestGridMetric = 'uv1976';   % 'uv1976' or 'xy'
nearestGridK = 6;               % K点比例法 / 投影K点法使用的最近点数量
nearestGridWeightPower = 2;     % 距离权重幂次
rayScanCount = 1001;            % 精确法射线扫描点数
pwmMax = 255;                   % 结果显示用 PWM 最大值
bestTol = 1e-6;                 % Ra 相同判定容差

%% ================= 输入归一化 =================
% 输入大于1时，按工程/LIN输入处理：
%   输入值 40 / 94
%   先乘以5得到 200 / 470
%   再除以1000得到真实 u'v' = 0.200 / 0.470
if target_u1976_input > 1
    target_u1976 = (target_u1976_input * 5) / 1000;
else
    target_u1976 = target_u1976_input;
end

if target_v1976_input > 1
    target_v1976 = (target_v1976_input * 5) / 1000;
else
    target_v1976 = target_v1976_input;
end

target_xy = uv1976_to_xy([target_u1976, target_v1976]);

if any(~isfinite(target_xy))
    error('输入 u''v'' 无法转换成有效 xy。u=%.6f, v=%.6f', target_u1976, target_v1976);
end

target_xyY = [target_xy(1), target_xy(2), target_Y_lm];

%% ================= 定位工程目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
outputTxtFile = fullfile(outputDir, 'single_uv_compare_result.txt');

if ~exist(lutFile, 'file')
    error(['没有找到 LUT 文件：\n%s\n\n' ...
           '请先运行 s01_make_lut_24_final.m。'], lutFile);
end

%% ================= 读取配置和光谱 =================
addpath(inputDir);
caseCfg = guangpu_case_config();

wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;

std = load_standard_data(standardDir, wl);

raw.R = read_spd(fullfile(inputDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(inputDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(inputDir, 'B.CSV'), '', wl);
raw.W = read_spd(fullfile(inputDir, 'W.CSV'), '', wl);
raw.SingleWhite = read_spd(fullfile(inputDir, 'W.CSV'), '', wl);

spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, dLambda);

actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);
actual_xyY.W = spectrum_xyY(spd.W, std, dLambda);

rgbTriangleXY = [actual_xyY.R(1:2); actual_xyY.G(1:2); actual_xyY.B(1:2)];
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', xyY_to_XYZ(actual_xyY.G).', xyY_to_XYZ(actual_xyY.B).'];

lut = readtable(lutFile, 'VariableNamingRule', 'preserve');

requiredCols = {'Target_x','Target_y','Target_u1976','Target_v1976', ...
    'Best_SingleWhiteRatio','Best_RGBPoint_x','Best_RGBPoint_y'};
for iCol = 1:numel(requiredCols)
    if ~ismember(requiredCols{iCol}, lut.Properties.VariableNames)
        error('LUT 缺少列：%s', requiredCols{iCol});
    end
end

singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, dLambda);
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
singleWhiteXY = singleMetrics.xyY(1:2);

fprintf('原始输入：Uin=%.6f, Vin=%.6f\n', target_u1976_input, target_v1976_input);
fprintf('归一化后：u''=%.6f, v''=%.6f, Y=%.6f\n', target_u1976, target_v1976, target_Y_lm);
fprintf('转换后：x=%.6f, y=%.6f\n', target_xyY(1), target_xyY(2));

%% ================= 先计算 K点比例法 =================
ratioK = run_safe_method(@() find_grid_same_ratio_k_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower));

%% ================= 再计算所有方法 =================
exact = run_safe_method(@() find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount));

proj1 = run_safe_method(@() find_grid_rgb_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, 1, nearestGridWeightPower));

projK = run_safe_method(@() find_grid_rgb_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower));

ratio1 = run_safe_method(@() find_grid_same_ratio_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric));

pureRgb = run_safe_method(@() calculate_pure_rgb_result(target_xyY, rgbSolveMatrix, spd, std, wl, dLambda));

methodNames = {'精确法', '投影1点', '投影K点', '同比例1点', '同比例K点', '纯RGB'};
methodKeys = {'Exact', 'Proj1', 'ProjK', 'Ratio1', 'RatioK', 'PureRGB'};
methodResults = {exact, proj1, projK, ratio1, ratioK, pureRgb};

%% ================= 生成结果文本 =================
resultText = make_single_uv_result_text( ...
    target_u1976, target_v1976, target_xyY, nearestGridK, pwmMax, bestTol, ...
    ratioK, methodNames, methodKeys, methodResults);

fprintf('\n%s\n', resultText);

fid = fopen(outputTxtFile, 'w');
if fid >= 0
    fprintf(fid, '%s\n', resultText);
    fclose(fid);
end

show_result_text_window(resultText, outputTxtFile);

%% ================= S07 Local helper functions =================
function result = run_safe_method(methodFunc)
    try
        result = methodFunc();
        result.Reason = "";
    catch ME
        result = empty_best_result();
        result.Reason = string(ME.message);
    end
end

function resultText = make_single_uv_result_text(targetU, targetV, target_xyY, nearestGridK, pwmMax, bestTol, ...
    ratioK, methodNames, methodKeys, methodResults)

    lines = strings(0, 1);

    lines(end+1) = "================ 单个 u'v' 输入计算结果 ================";
    lines(end+1) = sprintf("归一化 u'v' = (%.6f, %.6f)", targetU, targetV);
    lines(end+1) = sprintf("转换 xyY = (%.6f, %.6f, %.6f)", target_xyY(1), target_xyY(2), target_xyY(3));
    lines(end+1) = sprintf("K点数量 = %d", nearestGridK);
    lines(end+1) = sprintf("PWM显示规则 = round(weight_or_ratio * %d) 后限幅到 0~%d", pwmMax, pwmMax);
    lines(end+1) = " ";

    lines(end+1) = "---------------- K点比例法单独结果 ----------------";
    lines = append_method_detail(lines, "同比例K点", "RatioK", ratioK, pwmMax);
    lines(end+1) = " ";

    lines(end+1) = "---------------- 全部方法对比 ----------------";
    lines(end+1) = sprintf("%-12s %-8s %10s %23s %10s %10s %22s %24s", ...
        "方法", "有效", "Ra", "RGB点(x,y)", "W比例", "RGB比例", "RGB权重(R,G,B)", "PWM(R,G,B,W)");
    lines(end+1) = string(repmat('-', 1, 132));

    for i = 1:numel(methodNames)
        lines(end+1) = make_method_row(methodNames{i}, methodResults{i}, pwmMax);
    end

    lines(end+1) = " ";
    lines(end+1) = "---------------- 最优方法 ----------------";

    [overallBestNames, overallBestRa] = find_best_method_names(methodNames, methodResults, bestTol);
    [approxBestNames, approxBestRa] = find_best_method_names(methodNames(2:end), methodResults(2:end), bestTol);

    lines(end+1) = sprintf("全部方法最优：%s，Ra = %.6f", strjoin(overallBestNames, " / "), overallBestRa);
    lines(end+1) = sprintf("近似方法最优：%s，Ra = %.6f", strjoin(approxBestNames, " / "), approxBestRa);

    lines(end+1) = " ";
    lines(end+1) = "---------------- 无效方法说明 ----------------";
    invalidCount = 0;
    for i = 1:numel(methodNames)
        r = methodResults{i};
        if ~r.Valid
            invalidCount = invalidCount + 1;
            if isfield(r, 'Reason') && strlength(string(r.Reason)) > 0
                reason = string(r.Reason);
            else
                reason = "未返回有效结果";
            end
            lines(end+1) = sprintf("%s：%s", methodNames{i}, reason);
        end
    end

    if invalidCount == 0
        lines(end+1) = "无";
    end

    resultText = char(strjoin(lines, newline));
end

function lines = append_method_detail(lines, methodName, methodKey, result, pwmMax)
    lines(end+1) = sprintf("方法：%s (%s)", methodName, methodKey);
    lines(end+1) = sprintf("有效：%s", valid_text(result.Valid));

    if result.Valid
        pwm = calc_display_pwm(result, pwmMax);
        lines(end+1) = sprintf("Ra：%.6f", result.Ra);
        lines(end+1) = sprintf("Rmin8：%.6f", result.Rmin8);
        lines(end+1) = sprintf("RGB点 xyY：(%.6f, %.6f, %.6f)", result.RGBPoint_xyY(1), result.RGBPoint_xyY(2), result.RGBPoint_xyY(3));
        lines(end+1) = sprintf("比例：WhiteRatio=%.6f, RGBRatio=%.6f", result.SingleWhiteRatio, result.RGBWhiteRatio);
        lines(end+1) = sprintf("RGB权重：R=%.6f, G=%.6f, B=%.6f", result.R_weight, result.G_weight, result.B_weight);
        lines(end+1) = sprintf("PWM估算：R=%3d, G=%3d, B=%3d, W=%3d", pwm(1), pwm(2), pwm(3), pwm(4));
        lines(end+1) = sprintf("参考网格点：G=(%.6f, %.6f), KUsed=%.0f", ...
            result.SelectedGridTargetXY(1), result.SelectedGridTargetXY(2), result.KUsed);
    else
        if isfield(result, 'Reason')
            lines(end+1) = sprintf("失败原因：%s", string(result.Reason));
        else
            lines(end+1) = "失败原因：未返回有效结果";
        end
    end
end

function rowText = make_method_row(methodName, result, pwmMax)
    if result.Valid
        pwm = calc_display_pwm(result, pwmMax);
        rgbPointText = sprintf("(%.5f,%.5f)", result.RGBPoint_xyY(1), result.RGBPoint_xyY(2));
        weightText = sprintf("(%.4f,%.4f,%.4f)", result.R_weight, result.G_weight, result.B_weight);
        pwmText = sprintf("(%3d,%3d,%3d,%3d)", pwm(1), pwm(2), pwm(3), pwm(4));

        rowText = sprintf("%-12s %-8s %10.4f %23s %10.4f %10.4f %22s %24s", ...
            methodName, "YES", result.Ra, rgbPointText, ...
            result.SingleWhiteRatio, result.RGBWhiteRatio, weightText, pwmText);
    else
        rowText = sprintf("%-12s %-8s %10s %23s %10s %10s %22s %24s", ...
            methodName, "NO", "Invalid", "-", "-", "-", "-", "-");
    end
end

function txt = valid_text(validFlag)
    if validFlag
        txt = "YES";
    else
        txt = "NO";
    end
end

function pwm = calc_display_pwm(result, pwmMax)
    values = [result.R_weight, result.G_weight, result.B_weight, result.SingleWhiteRatio];
    values(~isfinite(values)) = 0;
    values = min(max(values, 0), 1);
    pwm = round(values .* pwmMax);
end

function [bestNames, bestRa] = find_best_method_names(methodNames, methodResults, tol)
    raList = nan(numel(methodResults), 1);

    for i = 1:numel(methodResults)
        if methodResults{i}.Valid && isfinite(methodResults{i}.Ra)
            raList(i) = methodResults{i}.Ra;
        end
    end

    if all(~isfinite(raList))
        bestNames = "无有效方法";
        bestRa = NaN;
        return;
    end

    bestRa = max(raList, [], 'omitnan');
    bestIndex = find(abs(raList - bestRa) <= tol);
    bestNames = string(methodNames(bestIndex));
end

function show_result_text_window(resultText, outputTxtFile)
    try
        d = dialog('Name', '单个UV输入：全部方法结果', ...
                   'Position', [120, 80, 1100, 760], ...
                   'WindowStyle', 'normal');

        uicontrol('Parent', d, ...
                  'Style', 'text', ...
                  'String', '单个UV输入计算结果（可复制）', ...
                  'Units', 'normalized', ...
                  'Position', [0.03, 0.94, 0.94, 0.04], ...
                  'FontSize', 11, ...
                  'FontWeight', 'bold', ...
                  'HorizontalAlignment', 'left');

        uicontrol('Parent', d, ...
                  'Style', 'edit', ...
                  'String', resultText, ...
                  'Units', 'normalized', ...
                  'Position', [0.03, 0.10, 0.94, 0.83], ...
                  'Max', 2, ...
                  'Min', 0, ...
                  'HorizontalAlignment', 'left', ...
                  'FontName', 'Consolas', ...
                  'FontSize', 10);

        uicontrol('Parent', d, ...
                  'Style', 'text', ...
                  'String', ['结果文件：' outputTxtFile], ...
                  'Units', 'normalized', ...
                  'Position', [0.03, 0.04, 0.74, 0.035], ...
                  'FontSize', 9, ...
                  'HorizontalAlignment', 'left');

        uicontrol('Parent', d, ...
                  'Style', 'pushbutton', ...
                  'String', '关闭', ...
                  'Units', 'normalized', ...
                  'Position', [0.82, 0.035, 0.12, 0.045], ...
                  'Callback', @(~,~) close(d));
    catch ME
        fprintf('\n结果框打开失败：%s\n', ME.message);
    end
end

function xy = uv1976_to_xy(uv)
    u = uv(1);
    v = uv(2);
    den = 6 * u - 16 * v + 12;

    if abs(den) < eps
        xy = [NaN, NaN];
    else
        x = 9 * u / den;
        y = 4 * v / den;
        xy = [x, y];
    end
end


%% ================= Local functions =================
function s = make_empty_result_struct()
    % 预先定义所有模型可能写入的字段，保证 allResult(k)=item 时结构体完全一致。
    s.Index = NaN;
    s.Target_x = NaN;
    s.Target_y = NaN;
    s.Target_Y_lm = NaN;
    s.Target_u1976 = NaN;
    s.Target_v1976 = NaN;

    methodPrefixList = {'Exact','Proj1','ProjK','Ratio1','RatioK','PureRGB'};

    for methodIdx = 1:numel(methodPrefixList)
        prefix = methodPrefixList{methodIdx};

        s.([prefix '_Valid']) = NaN;
        s.([prefix '_Ra']) = NaN;
        s.([prefix '_Rmin8']) = NaN;

        s.([prefix '_SelectedGridTarget_x']) = NaN;
        s.([prefix '_SelectedGridTarget_y']) = NaN;

        s.([prefix '_StoredBestRgb_x']) = NaN;
        s.([prefix '_StoredBestRgb_y']) = NaN;

        s.([prefix '_RGBPoint_x']) = NaN;
        s.([prefix '_RGBPoint_y']) = NaN;

        s.([prefix '_RayT']) = NaN;
        s.([prefix '_SingleWhiteRatio']) = NaN;
        s.([prefix '_RGBWhiteRatio']) = NaN;

        s.([prefix '_R_weight']) = NaN;
        s.([prefix '_G_weight']) = NaN;
        s.([prefix '_B_weight']) = NaN;

        s.([prefix '_KUsed']) = NaN;
    end

    s.Approx_Max_Ra = NaN;
    s.Overall_Max_Ra = NaN;
    s.Approx_Max_Source = "";
    s.Overall_Max_Source = "";
end

function item = fill_method_fields(item, prefix, result)
    item = set_numeric_field(item, [prefix '_Valid'], double(result.Valid));
    item = set_numeric_field(item, [prefix '_Ra'], result.Ra);
    item = set_numeric_field(item, [prefix '_Rmin8'], result.Rmin8);

    if isfield(result, 'SelectedGridTargetXY') && numel(result.SelectedGridTargetXY) >= 2
        item = set_numeric_field(item, [prefix '_SelectedGridTarget_x'], result.SelectedGridTargetXY(1));
        item = set_numeric_field(item, [prefix '_SelectedGridTarget_y'], result.SelectedGridTargetXY(2));
    end

    if isfield(result, 'StoredBestRgbXY') && numel(result.StoredBestRgbXY) >= 2
        item = set_numeric_field(item, [prefix '_StoredBestRgb_x'], result.StoredBestRgbXY(1));
        item = set_numeric_field(item, [prefix '_StoredBestRgb_y'], result.StoredBestRgbXY(2));
    end

    if isfield(result, 'RGBPoint_xyY') && numel(result.RGBPoint_xyY) >= 2
        item = set_numeric_field(item, [prefix '_RGBPoint_x'], result.RGBPoint_xyY(1));
        item = set_numeric_field(item, [prefix '_RGBPoint_y'], result.RGBPoint_xyY(2));
    end

    if isfield(result, 'RayT')
        item = set_numeric_field(item, [prefix '_RayT'], result.RayT);
    end

    if isfield(result, 'SingleWhiteRatio')
        item = set_numeric_field(item, [prefix '_SingleWhiteRatio'], result.SingleWhiteRatio);
    end

    if isfield(result, 'RGBWhiteRatio')
        item = set_numeric_field(item, [prefix '_RGBWhiteRatio'], result.RGBWhiteRatio);
    end

    if isfield(result, 'R_weight')
        item = set_numeric_field(item, [prefix '_R_weight'], result.R_weight);
        item = set_numeric_field(item, [prefix '_G_weight'], result.G_weight);
        item = set_numeric_field(item, [prefix '_B_weight'], result.B_weight);
    end

    if isfield(result, 'KUsed')
        item = set_numeric_field(item, [prefix '_KUsed'], result.KUsed);
    end
end

function item = set_numeric_field(item, fieldName, value)
    if isfield(item, fieldName)
        item.(fieldName) = value;
    end
end

function [maxRa, sourceName] = select_max_ra(varargin)
    names = varargin{end};
    values = [varargin{1:end-1}];
    validMask = isfinite(values);
    if ~any(validMask)
        maxRa = NaN;
        sourceName = "Invalid";
        return;
    end
    validValues = values(validMask);
    validNames = names(validMask);
    [maxRa, idx] = max(validValues);
    sourceName = validNames(idx);
end

function result = find_grid_rgb_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower)

    targetXY = target_xyY(1:2);
    [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric);

    rayDir = targetXY - singleWhiteXY;
    rayNorm2 = dot(rayDir, rayDir);
    if rayNorm2 <= eps
        result = empty_best_result();
        return;
    end

    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);

    validBestRgbXY = nan(nearestGridK, 2);
    validTargetXY = nan(nearestGridK, 2);
    validDistance = nan(nearestGridK, 1);
    validCount = 0;

    for i = 1:numel(sortedIndex)
        row = lut(sortedIndex(i), :);
        bestRgbXY = [row.Best_RGBPoint_x, row.Best_RGBPoint_y];
        if any(~isfinite(bestRgbXY))
            continue;
        end

        rawRayT = dot(bestRgbXY - singleWhiteXY, rayDir) / rayNorm2;
        if rawRayT < 1 - 1e-10 || rawRayT > boundaryRayT + 1e-10
            continue;
        end

        projectedRgbXY = singleWhiteXY + rawRayT .* rayDir;
        candidate = calculate_result_from_rgb_point(target_xyY, projectedRgbXY, rawRayT, ...
            singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

        if ~candidate.Valid
            continue;
        end

        validCount = validCount + 1;
        validBestRgbXY(validCount, :) = bestRgbXY;
        validTargetXY(validCount, :) = [row.Target_x, row.Target_y];
        validDistance(validCount) = sortedDistanceList(i);

        if validCount >= nearestGridK
            break;
        end
    end

    if validCount <= 0
        result = empty_best_result();
        return;
    end

    validBestRgbXY = validBestRgbXY(1:validCount, :);
    validTargetXY = validTargetXY(1:validCount, :);
    validDistance = validDistance(1:validCount);

    zeroIdx = find(validDistance <= 1e-12, 1, 'first');
    if ~isempty(zeroIdx)
        weights = zeros(validCount, 1);
        weights(zeroIdx) = 1;
    else
        weights = 1 ./ (validDistance .^ nearestGridWeightPower);
        weights = weights ./ sum(weights);
    end

    weightedBestRgbXY = sum(validBestRgbXY .* weights, 1);
    weightedRayT = dot(weightedBestRgbXY - singleWhiteXY, rayDir) / rayNorm2;
    weightedRayT = min(max(weightedRayT, 1), boundaryRayT);
    projectedRgbXY = singleWhiteXY + weightedRayT .* rayDir;

    result = calculate_result_from_rgb_point(target_xyY, projectedRgbXY, weightedRayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

    if result.Valid
        result.StoredBestRgbXY = weightedBestRgbXY;
        result.SelectedGridTargetXY = validTargetXY(1, :);
        result.KUsed = validCount;
    end
end

function result = find_grid_same_ratio_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric)

    targetXY = target_xyY(1:2);
    [~, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric);
    row = lut(sortedIndex(1), :);

    storedWhiteRatio = row.Best_SingleWhiteRatio;
    if ~isfinite(storedWhiteRatio)
        result = empty_best_result();
        return;
    end

    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    maxWhiteRatio = white_ratio_from_rayt(target_xyY, singleWhiteXY, boundaryRayT);
    whiteRatio = min(max(storedWhiteRatio, 0), maxWhiteRatio);

    rayT = rayt_from_white_ratio(target_xyY, singleWhiteXY, whiteRatio);
    rayT = min(max(rayT, 1), boundaryRayT);

    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

    if result.Valid
        result.SelectedGridTargetXY = [row.Target_x, row.Target_y];
    end
end


function result = find_grid_same_ratio_k_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower)

    targetXY = target_xyY(1:2);
    [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric);

    whiteRatioList = nan(nearestGridK, 1);
    distanceList = nan(nearestGridK, 1);
    targetXYList = nan(nearestGridK, 2);
    validCount = 0;

    for i = 1:numel(sortedIndex)
        row = lut(sortedIndex(i), :);
        storedWhiteRatio = row.Best_SingleWhiteRatio;

        if ~isfinite(storedWhiteRatio)
            continue;
        end

        validCount = validCount + 1;
        whiteRatioList(validCount) = storedWhiteRatio;
        distanceList(validCount) = sortedDistanceList(i);
        targetXYList(validCount, :) = [row.Target_x, row.Target_y];

        if validCount >= nearestGridK
            break;
        end
    end

    if validCount <= 0
        result = empty_best_result();
        return;
    end

    whiteRatioList = whiteRatioList(1:validCount);
    distanceList = distanceList(1:validCount);
    targetXYList = targetXYList(1:validCount, :);

    zeroIdx = find(distanceList <= 1e-12, 1, 'first');
    if ~isempty(zeroIdx)
        weights = zeros(validCount, 1);
        weights(zeroIdx) = 1;
    else
        weights = 1 ./ (distanceList .^ nearestGridWeightPower);
        weights = weights ./ sum(weights);
    end

    weightedWhiteRatio = sum(weights .* whiteRatioList);

    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    maxWhiteRatio = white_ratio_from_rayt(target_xyY, singleWhiteXY, boundaryRayT);
    whiteRatio = min(max(weightedWhiteRatio, 0), maxWhiteRatio);

    rayT = rayt_from_white_ratio(target_xyY, singleWhiteXY, whiteRatio);
    rayT = min(max(rayT, 1), boundaryRayT);

    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

    if result.Valid
        result.SelectedGridTargetXY = targetXYList(1, :);
        result.KUsed = validCount;
    end
end


function [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric)
    if strcmpi(nearestGridMetric, 'xy')
        lutTarget = [lut.Target_x, lut.Target_y];
        distanceList = vecnorm(lutTarget - targetXY, 2, 2);
    else
        targetUv = xy_to_uv1976(targetXY);
        lutTarget = [lut.Target_u1976, lut.Target_v1976];
        distanceList = vecnorm(lutTarget - targetUv, 2, 2);
    end
    [sortedDistanceList, sortedIndex] = sort(distanceList, 'ascend');
end

function result = calculate_pure_rgb_result(target_xyY, rgbSolveMatrix, spd, std, wl, dLambda)
    [weights, rgbSpd] = solve_rgb_point_spectrum(target_xyY, rgbSolveMatrix, spd);
    metrics = spectrum_metrics(rgbSpd, std, wl, dLambda);

    result = empty_best_result();
    result.Valid = true;
    result.Ra = metrics.Ra;
    result.Rmin8 = metrics.Rmin8;
    result.RGBPoint_xyY = [target_xyY(1), target_xyY(2), target_xyY(3)];
    result.RayT = 1;
    result.SingleWhiteRatio = 0;
    result.RGBWhiteRatio = 1;
    result.R_weight = weights(1);
    result.G_weight = weights(2);
    result.B_weight = weights(3);
end

function best = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount)

    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    rayTList = linspace(1, boundaryRayT, rayScanCount).';

    best = empty_best_result();

    for i = 1:numel(rayTList)
        rayT = rayTList(i);
        rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);
        result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
            singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

        if ~result.Valid
            continue;
        end

        if ~best.Valid || result.Ra > best.Ra
            best = result;
        end
    end
end

function result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
    singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda)

    result = empty_best_result();

    split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
    point_xyY = [rgbPointXY, split.RGBY];

    try
        [weights, rgbSpd] = solve_rgb_point_spectrum(point_xyY, rgbSolveMatrix, spd);
    catch
        return;
    end

    mixSpd = split.WhiteYRatio .* singleNorm + rgbSpd;
    metrics = spectrum_metrics(mixSpd, std, wl, dLambda);

    result.Valid = true;
    result.Ra = metrics.Ra;
    result.Rmin8 = metrics.Rmin8;
    result.RGBPoint_xyY = point_xyY;
    result.RayT = rayT;
    result.SingleWhiteRatio = split.WhiteYRatio;
    result.RGBWhiteRatio = split.RGBYRatio;
    result.R_weight = weights(1);
    result.G_weight = weights(2);
    result.B_weight = weights(3);
end

function s = empty_best_result()
    s.Valid = false;
    s.Ra = NaN;
    s.Rmin8 = NaN;
    s.RGBPoint_xyY = [NaN, NaN, NaN];
    s.RayT = NaN;
    s.SingleWhiteRatio = NaN;
    s.RGBWhiteRatio = NaN;
    s.R_weight = NaN;
    s.G_weight = NaN;
    s.B_weight = NaN;
    s.StoredBestRgbXY = [NaN, NaN];
    s.SelectedGridTargetXY = [NaN, NaN];
    s.KUsed = NaN;
end

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xy = XYZ_to_xyY(XYZ);
    xyY = [xy(1), xy(2), 683 * XYZ(2)];
end

function [pointXY, rayT, edgeName, edgeIndex] = find_ray_gamut_intersection(whiteXY, targetXY, triangleXY)
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    triangleXY = double(triangleXY(:, 1:2));
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_ray_gamut_intersection:SamePoint', 'The fixed single white and target have the same xy.');
    end

    tol = 1e-12;
    rayT = Inf;
    edgeIndex = NaN;
    pointXY = [NaN NaN];

    for i = 1:3
        edgeStart = triangleXY(i, :);
        edgeEnd = triangleXY(mod(i, 3) + 1, :);
        edgeVector = edgeEnd - edgeStart;
        systemMatrix = [direction(:), -edgeVector(:)];
        if abs(det(systemMatrix)) < tol
            continue;
        end

        params = systemMatrix \ (edgeStart - whiteXY).';
        t = params(1);
        s = params(2);

        if t >= 1 - tol && s >= -tol && s <= 1 + tol && t < rayT
            rayT = t;
            edgeIndex = i;
            pointXY = whiteXY + t .* direction;
        end
    end

    if ~isfinite(rayT)
        error('find_ray_gamut_intersection:NoIntersection', ...
            'The fixed-white to target ray does not hit the RGB gamut boundary after the target.');
    end

    edgeName = edgeNames{edgeIndex};
end

function split = split_target_on_ray(target_xyY, whiteXY, pointXY, rayT)
    targetY = target_xyY(3);
    target_y = target_xyY(2);

    lambda = 1 ./ rayT;
    oneMinusLambda = 1 - lambda;

    split.WhiteXYCoeff = oneMinusLambda;
    split.RGBXYCoeff = lambda;
    split.WhiteY = targetY * oneMinusLambda * whiteXY(2) / target_y;
    split.RGBY = targetY * lambda * pointXY(2) / target_y;
    split.WhiteYRatio = split.WhiteY / targetY;
    split.RGBYRatio = split.RGBY / targetY;
end

function rayT = rayt_from_white_ratio(target_xyY, whiteXY, whiteRatio)
    targetYCoord = target_xyY(2);
    whiteYCoord = whiteXY(2);

    if whiteRatio <= 0
        rayT = 1;
        return;
    end

    a = whiteRatio * targetYCoord / whiteYCoord;
    if a >= 1
        rayT = Inf;
    else
        rayT = 1 / (1 - a);
    end
end

function whiteRatio = white_ratio_from_rayt(target_xyY, whiteXY, rayT)
    targetYCoord = target_xyY(2);
    whiteYCoord = whiteXY(2);

    if rayT <= 1
        whiteRatio = 0;
        return;
    end

    whiteRatio = (1 - 1 / rayT) * whiteYCoord / targetYCoord;
end

function [weights, rgbSpd] = solve_rgb_point_spectrum(point_xyY, rgbSolveMatrix, spd)
    b = xyY_to_XYZ(point_xyY).';
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;

    if any(weights < -1e-8)
        error('solve_rgb_point_spectrum:OutsideGamut', ...
            'RGB candidate xy=(%.6f, %.6f) is outside the measured RGB gamut.', ...
            point_xyY(1), point_xyY(2));
    end

    rgbSpd = weights(1) .* spd.R + weights(2) .* spd.G + weights(3) .* spd.B;
end

function std = load_standard_data(standardDir, wl)
    xyzPath = ensure_file(standardDir, 'CIE_xyz_1931_2deg.csv', 'https://files.cie.co.at/CIE_xyz_1931_2deg.csv');
    tcsPath = ensure_file(standardDir, 'CIE_srf_cri.csv', 'https://files.cie.co.at/CIE_srf_cri.csv');
    dPath = ensure_file(standardDir, 'CIE_illum_Dxx_comp.csv', 'https://files.cie.co.at/CIE_illum_Dxx_comp.csv');

    xyz = readmatrix(xyzPath);
    tcs = readmatrix(tcsPath);
    d = readmatrix(dPath);

    std.cmf = interp1(xyz(:,1), xyz(:,2:4), wl, 'linear', 'extrap');
    std.tcs = interp1(tcs(:,1), tcs(:,2:9), wl, 'linear', 'extrap');
    std.daylightComponents = interp1(d(:,1), d(:,2:4), wl, 'linear', 'extrap');
end

function path = ensure_file(folder, fileName, url)
    path = fullfile(folder, fileName);
    if ~exist(path, 'file')
        websave(path, url);
    end
end

function spd = read_spd(fileName, sheetName, wlOut)
    if isempty(sheetName)
        M = readmatrix(fileName);
    else
        M = readmatrix(fileName, 'Sheet', sheetName);
    end
    wl = M(:,1);
    if size(M, 2) >= 3
        s = M(:,3);
    else
        s = M(:,2);
    end
    ok = isfinite(wl) & isfinite(s);
    wl = wl(ok);
    s = s(ok);
    [wl, order] = sort(wl);
    s = s(order);
    spd = interp1(wl, s, wlOut, 'linear', 0);
    spd(spd < 0) = 0;
end

function XYZ = xyY_to_XYZ(xyY)
    x = xyY(1); y = xyY(2); Y = xyY(3);
    X = x / y * Y;
    Z = (1 - x - y) / y * Y;
    XYZ = [X, Y, Z];
end

function spdScaled = scale_to_photopic_Y(spd, targetYlm, std, dLambda)
    Ylm = 683 * sum(spd(:) .* std.cmf(:,2)) * dLambda;
    if Ylm <= 0
        error('The spectrum photopic Y is 0 and cannot be normalized.');
    end
    spdScaled = spd(:) .* (targetYlm / Ylm);
end

function m = spectrum_metrics(spd, std, wl, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xyY = XYZ_to_xyY(XYZ);
    photopicY = 683 * XYZ(2);
    uv = XYZ_to_uv1960(XYZ);
    [CCT, ~] = estimate_cct_uv(uv, std.cmf, wl, dLambda);
    refSpd = reference_spd(CCT, std, wl);
    Ri = cri_R1_R8(spd, refSpd, std.cmf, std.tcs, dLambda);
    [Rmin8, ~] = min(Ri);
    m.xyY = [xyY(1), xyY(2), photopicY];
    m.photopicY = photopicY;
    m.CCT = CCT;
    m.Ri = Ri;
    m.Ra = mean(Ri);
    m.Rmin8 = Rmin8;
end

function XYZ = source_XYZ(spd, cmf, dLambda)
    XYZ = sum(spd(:) .* cmf, 1) .* dLambda;
end

function xyY = XYZ_to_xyY(XYZ)
    s = sum(XYZ);
    if s <= 0
        xyY = [NaN, NaN, NaN];
    else
        xyY = [XYZ(1) / s, XYZ(2) / s, XYZ(2)];
    end
end

function uv = XYZ_to_uv1960(XYZ)
    den = XYZ(1) + 15 * XYZ(2) + 3 * XYZ(3);
    uv = [4 * XYZ(1) / den, 6 * XYZ(2) / den];
end

function uv = xy_to_uv1976(xy)
    x = xy(1); y = xy(2);
    den = -2 * x + 12 * y + 3;
    uv = [4 * x / den, 9 * y / den];
end

function [CCT, Duv] = estimate_cct_uv(uv, cmf, wl, dLambda)
    gridT = linspace(1000, 25000, 481);
    dist2 = zeros(size(gridT));
    for i = 1:numel(gridT)
        bb = blackbody_spd(gridT(i), wl);
        bbXYZ = source_XYZ(bb, cmf, dLambda);
        bbuv = XYZ_to_uv1960(bbXYZ);
        dist2(i) = sum((uv - bbuv).^2);
    end
    [~, idx] = min(dist2);
    lo = max(1000, gridT(max(1, idx - 2)));
    hi = min(25000, gridT(min(numel(gridT), idx + 2)));
    CCT = fminbnd(@(T) blackbody_uv_distance2(T, uv, cmf, wl, dLambda), lo, hi);
    Duv = sqrt(blackbody_uv_distance2(CCT, uv, cmf, wl, dLambda));
end

function d2 = blackbody_uv_distance2(T, uv, cmf, wl, dLambda)
    bb = blackbody_spd(T, wl);
    bbXYZ = source_XYZ(bb, cmf, dLambda);
    bbuv = XYZ_to_uv1960(bbXYZ);
    d2 = sum((uv - bbuv).^2);
end

function spd = blackbody_spd(T, wl)
    c2 = 1.438776877e-2;
    lambda = wl(:) * 1e-9;
    spd = 1 ./ (lambda .^ 5 .* (exp(c2 ./ (lambda .* T)) - 1));
    spd = spd ./ max(spd);
end

function spd = reference_spd(CCT, std, wl)
    if CCT < 5000
        spd = blackbody_spd(CCT, wl);
        return;
    end
    T = min(max(CCT, 4000), 25000);
    if T <= 7000
        xD = 0.244063 + 0.09911e3 / T + 2.9678e6 / T^2 - 4.6070e9 / T^3;
    else
        xD = 0.237040 + 0.24748e3 / T + 1.9018e6 / T^2 - 2.0064e9 / T^3;
    end
    yD = -3.000 * xD^2 + 2.870 * xD - 0.275;
    den = 0.0241 + 0.2562 * xD - 0.7341 * yD;
    M1 = (-1.3515 - 1.7703 * xD + 5.9114 * yD) / den;
    M2 = (0.0300 - 31.4424 * xD + 30.0717 * yD) / den;
    spd = std.daylightComponents(:,1) + M1 .* std.daylightComponents(:,2) + M2 .* std.daylightComponents(:,3);
    spd = max(spd, 0);
end

function Ri = cri_R1_R8(testSpd, refSpd, cmf, tcs, dLambda)
    testXYZ = source_XYZ(testSpd, cmf, dLambda);
    refXYZ = source_XYZ(refSpd, cmf, dLambda);
    testUV = XYZ_to_uv1960(testXYZ);
    refUV = XYZ_to_uv1960(refXYZ);

    testUVW = tcs_uvw(testSpd, cmf, tcs, dLambda, testUV, refUV, true);
    refUVW = tcs_uvw(refSpd, cmf, tcs, dLambda, refUV, refUV, false);

    Ri = zeros(8, 1);
    for i = 1:8
        deltaE = norm(refUVW(i,:) - testUVW(i,:));
        Ri(i) = 100 - 4.6 * deltaE;
    end
end

function UVW = tcs_uvw(sourceSpd, cmf, tcs, dLambda, sourceUV, refUV, doAdapt)
    sourceY = sum(sourceSpd(:) .* cmf(:,2)) * dLambda;
    k = 100 / sourceY;
    UVW = zeros(size(tcs, 2), 3);

    cfun = @(u, v) (4 - u - 10 * v) / v;
    dfun = @(u, v) (1.708 * v + 0.404 - 1.481 * u) / v;
    sourceC = cfun(sourceUV(1), sourceUV(2));
    sourceD = dfun(sourceUV(1), sourceUV(2));
    refC = cfun(refUV(1), refUV(2));
    refD = dfun(refUV(1), refUV(2));

    for i = 1:size(tcs, 2)
        sampleXYZ = k .* sum(sourceSpd(:) .* tcs(:,i) .* cmf, 1) .* dLambda;
        sampleXY = XYZ_to_xyY(sampleXYZ);
        sampleUV = XYZ_to_uv1960(sampleXYZ);
        u = sampleUV(1);
        v = sampleUV(2);
        if doAdapt
            sampleC = cfun(u, v);
            sampleD = dfun(u, v);
            denominator = 16.518 + 1.481 * refC / sourceC * sampleC - refD / sourceD * sampleD;
            u = (10.872 + 0.404 * refC / sourceC * sampleC - 4 * refD / sourceD * sampleD) / denominator;
            v = 5.520 / denominator;
        end
        W = 25 * sampleXY(3)^(1/3) - 17;
        U = 13 * W * (u - refUV(1));
        V = 13 * W * (v - refUV(2));
        UVW(i,:) = [U, V, W];
    end
end

function projectRoot = find_project_root(startDir, inputFolderName)
    projectRoot = startDir;
    while true
        if exist(fullfile(projectRoot, inputFolderName), 'dir')
            return;
        end
        parentDir = fileparts(projectRoot);
        if strcmp(parentDir, projectRoot)
            error('find_project_root:NotFound', ...
                'Cannot find %s from script folder or its parent folders.', inputFolderName);
        end
        projectRoot = parentDir;
    end
end
