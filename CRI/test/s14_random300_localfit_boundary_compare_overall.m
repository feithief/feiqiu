% S14_RUN_LOCAL_WEIGHTED_WHITERATIO_FIT_RANDOM300
% 中文说明：
% 在 RGB 色域三角形内随机生成 300 个目标色点，然后逐点比较：
%   1. 精确法 Exact：射线扫描，作为离线基准
%   2. S14 曲面局部加权法：局部加权拟合 Best_SingleWhiteRatio，然后锁定目标色计算 Ra
%
% 输出：
%   triangle_grid_lut_output/s14_random300_localfit_boundary_compare_results.csv
%   triangle_grid_lut_output/s14_random300_localfit_boundary_compare_table.csv
%   triangle_grid_lut_output/s14_random300_localfit_boundary_compare_overall.csv

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S14 当前版本：随机300个色域内目标点 + 三方法Ra比较 + 总体统计\n');
fprintf('模型 = 精确法 Exact / S14 局部拟合 WhiteRatio / BoundaryMix 白点-边界混色\n');
fprintf('============================================================\n\n');

%% ================= 用户可配置参数 =================
nearestGridMetric = 'xy';        % neighbor search metric: 'xy' or 'uv1976'
localFitCoordinate = 'xy';       % local fit coordinate: 'xy' or 'uv1976'
localFitModel = 'plane';         % 'plane' or 'quadratic'
nearestGridK = 9;                % plane: >=3, recommended 8~12; quadratic: >=12
nearestGridWeightPower = 2;      % local fit weight = 1 / distance^power
fitRcondThreshold = 1e-10;       % ill-conditioned fallback threshold
rayScanCount = 1001;             % Exact ray scan count
printRealtimeRa = true;          % 是否实时打印结果

% 随机目标点配置
targetRandomCount = 300;
targetRandomSeed = 20260526;     % 固定随机种子，方便复现
targetRandomSpace = 'xy';        % 'xy' or 'uv1976'; 决定在哪个三角形空间内均匀随机
targetAutoY_lm = NaN;            % NaN -> use guangpu_case_config().target_xyY(3)

%% ================= 定位工程目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end
if ~exist(standardDir, 'dir')
    mkdir(standardDir);
end

lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
resultFile = fullfile(outputDir, 's14_random300_localfit_boundary_compare_results.csv');
compareTableFile = fullfile(outputDir, 's14_random300_localfit_boundary_compare_table.csv');
overallFile = fullfile(outputDir, 's14_random300_localfit_boundary_compare_overall.csv');
logFile = fullfile(outputDir, 's14_random300_localfit_boundary_compare_log.txt');

if ~exist(lutFile, 'file')
    error(['没有找到 LUT 文件：\n%s\n\n' ...
           '请先运行 s01_make_lut_adaptive.m 或当前工程对应的 LUT 生成脚本。'], lutFile);
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
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', ...
                  xyY_to_XYZ(actual_xyY.G).', ...
                  xyY_to_XYZ(actual_xyY.B).'];

if isnan(targetAutoY_lm)
    autoTargetY_lm = caseCfg.target_xyY(3);
else
    autoTargetY_lm = targetAutoY_lm;
end

%% ================= 随机生成目标颜色点 =================
rng(targetRandomSeed);
targetList = generate_random_targets_in_rgb_gamut(rgbTriangleXY, targetRandomCount, autoTargetY_lm, targetRandomSpace);

fprintf('目标点模式：色域内随机取点\n');
fprintf('随机空间：%s\n', targetRandomSpace);
fprintf('随机种子：%u\n', uint32(targetRandomSeed));
fprintf('目标点数量：%d\n', size(targetList, 1));
fprintf('目标点亮度Y：%.4f\n', autoTargetY_lm);

%% ================= 读取 LUT =================
lut = readtable(lutFile, 'VariableNamingRule', 'preserve');

requiredCols = {'Target_x','Target_y','Target_Y_lm','Target_u1976','Target_v1976', ...
    'Best_Ra','Best_SingleWhiteRatio','Best_RGBPoint_x','Best_RGBPoint_y', ...
    'Best_RGBPoint_Y_lm','Best_R_weight','Best_G_weight','Best_B_weight','BoundaryEdge'};
for iCol = 1:numel(requiredCols)
    if ~ismember(requiredCols{iCol}, lut.Properties.VariableNames)
        error('LUT 缺少列：%s', requiredCols{iCol});
    end
end

%% ================= 主循环 =================
nTarget = size(targetList, 1);
allResult = repmat(make_s14_random300_empty_result_struct(), nTarget, 1);
logLines = strings(0, 1);

logLines(end + 1, 1) = "S14 Random300 Local Weighted WhiteRatio Fit + Boundary Mix Compare";
logLines(end + 1, 1) = sprintf("Count=%u, K=%u, metric=%s, fitCoord=%s, model=%s", ...
    uint32(nTarget), uint32(nearestGridK), nearestGridMetric, localFitCoordinate, localFitModel);
logLines(end + 1, 1) = " ";

fprintf('开始批量计算，共 %d 个随机目标颜色点...\n', nTarget);
fprintf('%-4s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-5s %-12s\n', ...
    'No', 'x', 'y', 'ExactRa', 'S14Ra', 'BndRa', 'DiffS14', 'DiffBnd', 'S14-Bnd', 'ExactW', 'S14W', 'BndW', 'FB', 'Status');

for k = 1:nTarget
    target_xyY = targetList(k, :);
    targetUv = xy_to_uv1976(target_xyY(1:2));

    singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, dLambda);
    singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
    singleWhiteXY = singleMetrics.xyY(1:2);

    item = make_s14_random300_empty_result_struct();
    item.Index = k;
    item.Target_x = target_xyY(1);
    item.Target_y = target_xyY(2);
    item.Target_Y_lm = target_xyY(3);
    item.Target_u1976 = targetUv(1);
    item.Target_v1976 = targetUv(2);

    try
        exact = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, rayScanCount);
    catch ME
        warning('目标点 %d 精确法失败：%s', k, ME.message);
        exact = empty_best_result();
        item.ErrorMessage = "Exact: " + string(ME.message);
    end

    if exact.Valid
        targetBestRGBPointXY = exact.RGBPoint_xyY(1:2);
    else
        targetBestRGBPointXY = [NaN, NaN];
    end

    try
        s14 = find_stored_ratio_result(target_xyY, targetUv, targetBestRGBPointXY, singleWhiteXY, rgbTriangleXY, ...
            rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, lut, ...
            nearestGridMetric, localFitCoordinate, localFitModel, nearestGridK, nearestGridWeightPower, fitRcondThreshold);
    catch ME
        warning('目标点 %d S14失败：%s', k, ME.message);
        s14 = empty_stored_ratio_result();
        if strlength(item.ErrorMessage) > 0
            item.ErrorMessage = item.ErrorMessage + " | S14: " + string(ME.message);
        else
            item.ErrorMessage = "S14: " + string(ME.message);
        end
    end

    try
        boundaryMix = calculate_boundary_blend_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda);
    catch ME
        warning('目标点 %d 边界混色法失败：%s', k, ME.message);
        boundaryMix = empty_best_result();
        if strlength(item.ErrorMessage) > 0
            item.ErrorMessage = item.ErrorMessage + " | BoundaryMix: " + string(ME.message);
        else
            item.ErrorMessage = "BoundaryMix: " + string(ME.message);
        end
    end

    item.ExactValid = double(exact.Valid);
    item.S14Valid = double(s14.Valid);
    item.BoundaryMixValid = double(boundaryMix.Valid);

    if exact.Valid
        item.Exact_Ra = exact.Ra;
        item.Exact_Rmin8 = exact.Rmin8;
        item.Exact_SingleWhiteRatio = exact.SingleWhiteRatio;
        item.Exact_RGBWhiteRatio = exact.RGBWhiteRatio;
        item.Exact_RGBPoint_x = exact.RGBPoint_xyY(1);
        item.Exact_RGBPoint_y = exact.RGBPoint_xyY(2);
        item.Exact_RGBPoint_Y_lm = exact.RGBPoint_xyY(3);
        item.Exact_R_weight = exact.R_weight;
        item.Exact_G_weight = exact.G_weight;
        item.Exact_B_weight = exact.B_weight;
    end

    if s14.Valid
        item.S14_Ra = s14.Ra;
        item.S14_Rmin8 = s14.Rmin8;
        item.S14_SingleWhiteRatio = s14.SingleWhiteRatio;
        item.S14_RGBWhiteRatio = s14.RGBWhiteRatio;
        item.S14_DistanceWeightedAverageWhiteRatio = s14.RawWeightedWhiteRatio;
        item.S14_LocalFitPredictedWhiteRatio = s14.WeightedWhiteRatio;
        item.S14_LocalFitFallbackUsed = s14.LocalFitFallbackUsed;
        item.S14_LocalFitConditionValue = s14.LocalFitConditionValue;
        item.S14_OutputDeltaUv = s14.OutputDeltaUv;
        item.S14_RGBPoint_x = s14.RGBPoint_xyY(1);
        item.S14_RGBPoint_y = s14.RGBPoint_xyY(2);
        item.S14_RGBPoint_Y_lm = s14.RGBPoint_xyY(3);
        item.S14_R_weight = s14.R_weight;
        item.S14_G_weight = s14.G_weight;
        item.S14_B_weight = s14.B_weight;
        item.S14_KUsed = s14.CandidateCount;
    end

    if boundaryMix.Valid
        item.BoundaryMix_Ra = boundaryMix.Ra;
        item.BoundaryMix_Rmin8 = boundaryMix.Rmin8;
        item.BoundaryMix_SingleWhiteRatio = boundaryMix.SingleWhiteRatio;
        item.BoundaryMix_RGBWhiteRatio = boundaryMix.RGBWhiteRatio;
        item.BoundaryMix_RGBPoint_x = boundaryMix.RGBPoint_xyY(1);
        item.BoundaryMix_RGBPoint_y = boundaryMix.RGBPoint_xyY(2);
        item.BoundaryMix_RGBPoint_Y_lm = boundaryMix.RGBPoint_xyY(3);
        item.BoundaryMix_RayT = boundaryMix.RayT;
        item.BoundaryMix_R_weight = boundaryMix.R_weight;
        item.BoundaryMix_G_weight = boundaryMix.G_weight;
        item.BoundaryMix_B_weight = boundaryMix.B_weight;
    end

    if exact.Valid && s14.Valid
        item.RaDiff_ExactMinusS14 = exact.Ra - s14.Ra;
        item.WhiteRatioDiff_ExactMinusS14 = exact.SingleWhiteRatio - s14.SingleWhiteRatio;
    end

    if exact.Valid && boundaryMix.Valid
        item.RaDiff_ExactMinusBoundaryMix = exact.Ra - boundaryMix.Ra;
        item.WhiteRatioDiff_ExactMinusBoundaryMix = exact.SingleWhiteRatio - boundaryMix.SingleWhiteRatio;
    end

    if s14.Valid && boundaryMix.Valid
        item.RaDiff_S14MinusBoundaryMix = s14.Ra - boundaryMix.Ra;
        item.AbsRaDiff_S14MinusBoundaryMix = abs(item.RaDiff_S14MinusBoundaryMix);
        if item.RaDiff_S14MinusBoundaryMix > 1e-9
            item.BetterApproxMethod = "S14";
        elseif item.RaDiff_S14MinusBoundaryMix < -1e-9
            item.BetterApproxMethod = "BoundaryMix";
        else
            item.BetterApproxMethod = "Tie";
        end
    end

    if printRealtimeRa
        statusText = "OK";
        if ~(exact.Valid && s14.Valid && boundaryMix.Valid)
            statusText = "INVALID";
        end

        lineText = sprintf('%-4u %-10.5f %-10.5f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.5f %-10.5f %-10.5f %-5.0f %-12s', ...
            uint32(k), item.Target_x, item.Target_y, ...
            item.Exact_Ra, item.S14_Ra, item.BoundaryMix_Ra, ...
            item.RaDiff_ExactMinusS14, item.RaDiff_ExactMinusBoundaryMix, item.RaDiff_S14MinusBoundaryMix, ...
            item.Exact_SingleWhiteRatio, item.S14_SingleWhiteRatio, item.BoundaryMix_SingleWhiteRatio, ...
            item.S14_LocalFitFallbackUsed, char(statusText));
        fprintf('%s\n', lineText);
        logLines(end + 1, 1) = string(lineText);
        drawnow;
    end

    allResult(k) = item;
end

resultTable = struct2table(allResult);
writetable(resultTable, resultFile);

compareTable = resultTable(:, { ...
    'Index', 'Target_x', 'Target_y', 'Target_Y_lm', 'Target_u1976', 'Target_v1976', ...
    'Exact_Ra', 'S14_Ra', 'BoundaryMix_Ra', ...
    'RaDiff_ExactMinusS14', 'RaDiff_ExactMinusBoundaryMix', 'RaDiff_S14MinusBoundaryMix', ...
    'AbsRaDiff_S14MinusBoundaryMix', 'BetterApproxMethod', ...
    'Exact_SingleWhiteRatio', 'S14_LocalFitPredictedWhiteRatio', 'S14_SingleWhiteRatio', 'BoundaryMix_SingleWhiteRatio', ...
    'S14_LocalFitFallbackUsed', 'S14_LocalFitConditionValue'});
writetable(compareTable, compareTableFile);

overallSummary = build_overall_compare_summary(resultTable);
writetable(overallSummary, overallFile);

summaryLines = format_overall_compare_summary(overallSummary);
for si = 1:numel(summaryLines)
    fprintf('%s\n', summaryLines(si));
    logLines(end + 1, 1) = summaryLines(si);
end

fid = fopen(logFile, 'w');
if fid >= 0
    fprintf(fid, '%s\n', char(strjoin(logLines, newline)));
    fclose(fid);
end

fprintf('\n批量计算完成。\n结果文件：\n%s\n', resultFile);
fprintf('需求对比表：\n%s\n', compareTableFile);
fprintf('总体统计表：\n%s\n', overallFile);
fprintf('日志文件：\n%s\n', logFile);

%% ================= Batch helper functions =================
function s = make_s14_random300_empty_result_struct()
    s.Index = NaN;
    s.Target_x = NaN;
    s.Target_y = NaN;
    s.Target_Y_lm = NaN;
    s.Target_u1976 = NaN;
    s.Target_v1976 = NaN;

    s.ExactValid = NaN;
    s.S14Valid = NaN;
    s.BoundaryMixValid = NaN;

    s.Exact_Ra = NaN;
    s.Exact_Rmin8 = NaN;
    s.Exact_SingleWhiteRatio = NaN;
    s.Exact_RGBWhiteRatio = NaN;
    s.Exact_RGBPoint_x = NaN;
    s.Exact_RGBPoint_y = NaN;
    s.Exact_RGBPoint_Y_lm = NaN;
    s.Exact_R_weight = NaN;
    s.Exact_G_weight = NaN;
    s.Exact_B_weight = NaN;

    s.S14_Ra = NaN;
    s.S14_Rmin8 = NaN;
    s.S14_SingleWhiteRatio = NaN;
    s.S14_RGBWhiteRatio = NaN;
    s.S14_DistanceWeightedAverageWhiteRatio = NaN;
    s.S14_LocalFitPredictedWhiteRatio = NaN;
    s.S14_LocalFitFallbackUsed = NaN;
    s.S14_LocalFitConditionValue = NaN;
    s.S14_OutputDeltaUv = NaN;
    s.S14_RGBPoint_x = NaN;
    s.S14_RGBPoint_y = NaN;
    s.S14_RGBPoint_Y_lm = NaN;
    s.S14_R_weight = NaN;
    s.S14_G_weight = NaN;
    s.S14_B_weight = NaN;
    s.S14_KUsed = NaN;

    s.BoundaryMix_Ra = NaN;
    s.BoundaryMix_Rmin8 = NaN;
    s.BoundaryMix_SingleWhiteRatio = NaN;
    s.BoundaryMix_RGBWhiteRatio = NaN;
    s.BoundaryMix_RGBPoint_x = NaN;
    s.BoundaryMix_RGBPoint_y = NaN;
    s.BoundaryMix_RGBPoint_Y_lm = NaN;
    s.BoundaryMix_RayT = NaN;
    s.BoundaryMix_R_weight = NaN;
    s.BoundaryMix_G_weight = NaN;
    s.BoundaryMix_B_weight = NaN;

    s.RaDiff_ExactMinusS14 = NaN;
    s.RaDiff_ExactMinusBoundaryMix = NaN;
    s.RaDiff_S14MinusBoundaryMix = NaN;
    s.AbsRaDiff_S14MinusBoundaryMix = NaN;
    s.BetterApproxMethod = "";
    s.WhiteRatioDiff_ExactMinusS14 = NaN;
    s.WhiteRatioDiff_ExactMinusBoundaryMix = NaN;
    s.ErrorMessage = "";
end


function overallSummary = build_overall_compare_summary(resultTable)
    validS14BoundaryMask = isfinite(resultTable.S14_Ra) & isfinite(resultTable.BoundaryMix_Ra);
    validExactS14Mask = isfinite(resultTable.Exact_Ra) & isfinite(resultTable.S14_Ra);
    validExactBoundaryMask = isfinite(resultTable.Exact_Ra) & isfinite(resultTable.BoundaryMix_Ra);

    diffS14Boundary = resultTable.RaDiff_S14MinusBoundaryMix(validS14BoundaryMask);
    diffExactS14 = resultTable.RaDiff_ExactMinusS14(validExactS14Mask);
    diffExactBoundary = resultTable.RaDiff_ExactMinusBoundaryMix(validExactBoundaryMask);

    s14Ra = resultTable.S14_Ra(validS14BoundaryMask);
    boundaryRa = resultTable.BoundaryMix_Ra(validS14BoundaryMask);

    s14WinCount = sum(diffS14Boundary > 1e-9);
    boundaryWinCount = sum(diffS14Boundary < -1e-9);
    tieCount = sum(abs(diffS14Boundary) <= 1e-9);

    if isempty(diffS14Boundary)
        betterOverall = "Invalid";
    elseif mean(diffS14Boundary, 'omitnan') > 0
        betterOverall = "S14";
    elseif mean(diffS14Boundary, 'omitnan') < 0
        betterOverall = "BoundaryMix";
    else
        betterOverall = "Tie";
    end

    overallSummary = table( ...
        height(resultTable), ...
        sum(validS14BoundaryMask), ...
        mean(s14Ra, 'omitnan'), ...
        mean(boundaryRa, 'omitnan'), ...
        mean(diffS14Boundary, 'omitnan'), ...
        mean(abs(diffS14Boundary), 'omitnan'), ...
        max(abs(diffS14Boundary), [], 'omitnan'), ...
        median(diffS14Boundary, 'omitnan'), ...
        local_percentile(abs(diffS14Boundary), 95), ...
        s14WinCount, ...
        boundaryWinCount, ...
        tieCount, ...
        mean(diffExactS14, 'omitnan'), ...
        mean(abs(diffExactS14), 'omitnan'), ...
        mean(diffExactBoundary, 'omitnan'), ...
        mean(abs(diffExactBoundary), 'omitnan'), ...
        betterOverall, ...
        'VariableNames', { ...
            'TotalCount', ...
            'ValidS14BoundaryCount', ...
            'MeanS14Ra', ...
            'MeanBoundaryMixRa', ...
            'MeanRaDiff_S14MinusBoundaryMix', ...
            'MeanAbsRaDiff_S14Boundary', ...
            'MaxAbsRaDiff_S14Boundary', ...
            'MedianRaDiff_S14MinusBoundaryMix', ...
            'P95AbsRaDiff_S14Boundary', ...
            'S14WinCount', ...
            'BoundaryMixWinCount', ...
            'TieCount', ...
            'MeanRaDiff_ExactMinusS14', ...
            'MeanAbsRaDiff_ExactS14', ...
            'MeanRaDiff_ExactMinusBoundaryMix', ...
            'MeanAbsRaDiff_ExactBoundaryMix', ...
            'BetterOverall'});
end

function summaryLines = format_overall_compare_summary(overallSummary)
    summaryLines = strings(0, 1);
    summaryLines(end + 1, 1) = " ";
    summaryLines(end + 1, 1) = "================ Overall Comparison Summary ================";
    summaryLines(end + 1, 1) = sprintf("TotalCount = %u, ValidS14BoundaryCount = %u", ...
        uint32(overallSummary.TotalCount(1)), uint32(overallSummary.ValidS14BoundaryCount(1)));
    summaryLines(end + 1, 1) = sprintf("MeanS14Ra = %.6f, MeanBoundaryMixRa = %.6f", ...
        overallSummary.MeanS14Ra(1), overallSummary.MeanBoundaryMixRa(1));
    summaryLines(end + 1, 1) = sprintf("Mean(S14 - BoundaryMix) = %.6f", ...
        overallSummary.MeanRaDiff_S14MinusBoundaryMix(1));
    summaryLines(end + 1, 1) = sprintf("MeanAbs(S14 - BoundaryMix) = %.6f, MaxAbs = %.6f, P95Abs = %.6f", ...
        overallSummary.MeanAbsRaDiff_S14Boundary(1), ...
        overallSummary.MaxAbsRaDiff_S14Boundary(1), ...
        overallSummary.P95AbsRaDiff_S14Boundary(1));
    summaryLines(end + 1, 1) = sprintf("S14Win = %u, BoundaryMixWin = %u, Tie = %u, BetterOverall = %s", ...
        uint32(overallSummary.S14WinCount(1)), ...
        uint32(overallSummary.BoundaryMixWinCount(1)), ...
        uint32(overallSummary.TieCount(1)), ...
        char(overallSummary.BetterOverall(1)));
    summaryLines(end + 1, 1) = sprintf("MeanAbs(Exact - S14) = %.6f, MeanAbs(Exact - BoundaryMix) = %.6f", ...
        overallSummary.MeanAbsRaDiff_ExactS14(1), ...
        overallSummary.MeanAbsRaDiff_ExactBoundaryMix(1));
end

function value = local_percentile(data, percentileValue)
    data = data(isfinite(data));
    if isempty(data)
        value = NaN;
        return;
    end

    data = sort(data(:));
    if numel(data) == 1
        value = data(1);
        return;
    end

    pos = 1 + (numel(data) - 1) * percentileValue / 100;
    lo = floor(pos);
    hi = ceil(pos);
    if lo == hi
        value = data(lo);
    else
        value = data(lo) + (data(hi) - data(lo)) * (pos - lo);
    end
end

function targetList = generate_random_targets_in_rgb_gamut(rgbTriangleXY, targetCount, targetY_lm, randomSpace)
    targetList = nan(targetCount, 3);

    if strcmpi(randomSpace, 'uv1976')
        p = zeros(3, 2);
        for i = 1:3
            p(i, :) = xy_to_uv1976(rgbTriangleXY(i, :));
        end
        for i = 1:targetCount
            bary = random_uniform_barycentric();
            uv = bary(1) .* p(1, :) + bary(2) .* p(2, :) + bary(3) .* p(3, :);
            xy = uv1976_to_xy(uv);
            targetList(i, :) = [xy(1), xy(2), targetY_lm];
        end
    else
        p = rgbTriangleXY(:, 1:2);
        for i = 1:targetCount
            bary = random_uniform_barycentric();
            xy = bary(1) .* p(1, :) + bary(2) .* p(2, :) + bary(3) .* p(3, :);
            targetList(i, :) = [xy(1), xy(2), targetY_lm];
        end
    end
end

function bary = random_uniform_barycentric()
    a = rand();
    b = rand();

    if (a + b) > 1
        a = 1 - a;
        b = 1 - b;
    end

    c = 1 - a - b;
    bary = [a, b, c];
end


%% ================= Local functions =================

function result = find_stored_ratio_result(target_xyY, targetUv, targetBestRGBPointXY, singleWhiteXY, rgbTriangleXY, ...
    rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, lut, ...
    nearestGridMetric, localFitCoordinate, localFitModel, nearestGridK, nearestGridWeightPower, fitRcondThreshold)

    result = empty_stored_ratio_result();
    targetXY = target_xyY(1:2);
    targetFitCoord = get_fit_coordinate_from_xy(targetXY, localFitCoordinate);

    [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric);

    candidateStruct = repmat(empty_stored_ratio_candidate(), nearestGridK, 1);
    validCount = 0;

    for i = 1:numel(sortedIndex)
        lutIndex = sortedIndex(i);
        row = lut(lutIndex, :);
        storedWhiteRatio = row.Best_SingleWhiteRatio;
        storedBestRGBPointXY = [row.Best_RGBPoint_x, row.Best_RGBPoint_y];

        if ~isfinite(storedWhiteRatio) || any(~isfinite(storedBestRGBPointXY))
            continue;
        end

        validCount = validCount + 1;
        candidate = empty_stored_ratio_candidate();
        candidate.Valid = true;
        candidate.Rank = validCount;
        candidate.LutIndex0 = lutIndex - 1;
        candidate.CandidateDeltaUv = sortedDistanceList(i);
        candidate.StoredWhiteRatio = storedWhiteRatio;
        candidate.StoredBestRa = row.Best_Ra;
        candidate.StoredBestRGBPoint_xyY = [storedBestRGBPointXY, row.Best_RGBPoint_Y_lm];
        candidate.SelectedGridTarget_xyY = [row.Target_x, row.Target_y, row.Target_Y_lm];
        candidate.SelectedGridTarget_uv = [row.Target_u1976, row.Target_v1976];
        candidate.BoundaryEdge = string(row.BoundaryEdge);

        if all(isfinite(targetBestRGBPointXY)) && all(isfinite(storedBestRGBPointXY))
            storedBestRGBPointUv = xy_to_uv1976(storedBestRGBPointXY);
            targetBestRGBPointUv = xy_to_uv1976(targetBestRGBPointXY);
            candidate.BestRGBPointDeltaUvToTargetBest = norm(storedBestRGBPointUv - targetBestRGBPointUv);
        end

        singleRatioResult = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
            rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, storedWhiteRatio);

        if singleRatioResult.Valid
            candidate.Ra = singleRatioResult.Ra;
            candidate.Rmin8 = singleRatioResult.Rmin8;
            candidate.SingleWhiteRatio = singleRatioResult.SingleWhiteRatio;
            candidate.RGBWhiteRatio = singleRatioResult.RGBWhiteRatio;
            candidate.RGBPoint_xyY = singleRatioResult.RGBPoint_xyY;
            candidate.R_weight = singleRatioResult.R_weight;
            candidate.G_weight = singleRatioResult.G_weight;
            candidate.B_weight = singleRatioResult.B_weight;
            candidate.OutputDeltaUv = 0;
            candidate.Output_xyY = target_xyY;
            candidate.ResidualXYZ = 0;
        end

        candidateStruct(validCount) = candidate;

        if validCount >= nearestGridK
            break;
        end
    end

    if validCount <= 0
        return;
    end

    candidateStruct = candidateStruct(1:validCount);
    distanceList = [candidateStruct.CandidateDeltaUv].';
    whiteRatioList = [candidateStruct.StoredWhiteRatio].';

    zeroIdx = find(distanceList <= 1e-12, 1, 'first');
    if ~isempty(zeroIdx)
        fitWeights = zeros(validCount, 1);
        fitWeights(zeroIdx) = 1;
        predictedWhiteRatioRaw = whiteRatioList(zeroIdx);
        fitFallbackUsed = false;
        fitCondValue = Inf;
        fittedList = nan(validCount, 1);
        fitResidualList = nan(validCount, 1);
    else
        fitWeights = 1 ./ (distanceList .^ nearestGridWeightPower);
        fitWeights = fitWeights ./ sum(fitWeights);

        coordList = zeros(validCount, 2);
        for k = 1:validCount
            coordList(k, :) = get_fit_coordinate_from_xy(candidateStruct(k).SelectedGridTarget_xyY(1:2), localFitCoordinate);
        end

        [predictedWhiteRatioRaw, fitInfo] = local_weighted_whiteratio_fit(coordList, whiteRatioList, fitWeights, ...
            targetFitCoord, localFitModel, fitRcondThreshold);

        fitFallbackUsed = fitInfo.FallbackUsed;
        fitCondValue = fitInfo.ConditionValue;
        fittedList = fitInfo.FittedWhiteRatioList;
        fitResidualList = fitInfo.FitResidualList;
    end

    for i = 1:validCount
        candidateStruct(i).DistanceWeight = fitWeights(i);
        candidateStruct(i).WeightedContribution = fitWeights(i) * candidateStruct(i).StoredWhiteRatio;
        candidateStruct(i).FitModelWeight = fitWeights(i);
        if exist('fittedList', 'var') && numel(fittedList) >= i
            candidateStruct(i).FittedWhiteRatio = fittedList(i);
            candidateStruct(i).FitResidual = fitResidualList(i);
        end
    end

    finalRatioResult = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
        rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, predictedWhiteRatioRaw);

    result.CandidateCount = validCount;
    result.SearchDeltaUvLimit = NaN;
    result.UsedFallback = validCount < nearestGridK;
    result.CandidateTable = build_stored_ratio_candidate_table(candidateStruct);
    result.RawWeightedWhiteRatio = sum(fitWeights .* whiteRatioList);
    result.WeightedWhiteRatio = predictedWhiteRatioRaw;
    result.LocalFitFallbackUsed = double(fitFallbackUsed);
    result.LocalFitConditionValue = fitCondValue;

    if ~finalRatioResult.Valid
        return;
    end

    result.Valid = true;
    result.Ra = finalRatioResult.Ra;
    result.Rmin8 = finalRatioResult.Rmin8;
    result.SingleWhiteRatio = finalRatioResult.SingleWhiteRatio;
    result.RGBWhiteRatio = finalRatioResult.RGBWhiteRatio;
    result.RGBPoint_xyY = finalRatioResult.RGBPoint_xyY;
    result.R_weight = finalRatioResult.R_weight;
    result.G_weight = finalRatioResult.G_weight;
    result.B_weight = finalRatioResult.B_weight;
    result.ResidualXYZ = 0;
    result.OutputDeltaUv = 0;
    result.Output_xyY = target_xyY;
    result.StoredWhiteRatio = predictedWhiteRatioRaw;
    result.StoredBestRa = NaN;
    result.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    result.BestRGBPointDeltaUvToTargetBest = NaN;
    result.LutIndex0 = candidateStruct(1).LutIndex0;
    result.SelectedGridTarget_xyY = candidateStruct(1).SelectedGridTarget_xyY;
    result.SelectedGridTarget_uv = candidateStruct(1).SelectedGridTarget_uv;
    result.BoundaryEdge = "";
end

function coord = get_fit_coordinate_from_xy(xy, localFitCoordinate)
    if strcmpi(localFitCoordinate, 'uv1976')
        coord = xy_to_uv1976(xy);
    else
        coord = xy;
    end
end

function [predictedWhiteRatio, info] = local_weighted_whiteratio_fit(coordList, whiteRatioList, fitWeights, targetCoord, localFitModel, fitRcondThreshold)
    coordList = double(coordList);
    whiteRatioList = double(whiteRatioList(:));
    fitWeights = double(fitWeights(:));
    targetCoord = double(targetCoord(:).');

    if strcmpi(localFitModel, 'quadratic')
        A = [coordList(:,1).^2, coordList(:,2).^2, coordList(:,1).*coordList(:,2), coordList(:,1), coordList(:,2), ones(size(coordList,1),1)];
        aTarget = [targetCoord(1)^2, targetCoord(2)^2, targetCoord(1)*targetCoord(2), targetCoord(1), targetCoord(2), 1];
    else
        A = [coordList(:,1), coordList(:,2), ones(size(coordList,1),1)];
        aTarget = [targetCoord(1), targetCoord(2), 1];
    end

    sqrtW = sqrt(fitWeights);
    Aw = A .* sqrtW;
    bw = whiteRatioList .* sqrtW;
    normalMat = Aw.' * Aw;

    if size(A, 1) < size(A, 2) || rcond(normalMat) < fitRcondThreshold
        predictedWhiteRatio = sum(fitWeights .* whiteRatioList);
        fittedList = repmat(predictedWhiteRatio, size(whiteRatioList));
        info.FallbackUsed = true;
        info.ConditionValue = rcond(normalMat);
        info.FittedWhiteRatioList = fittedList;
        info.FitResidualList = fittedList - whiteRatioList;
        return;
    end

    coef = normalMat \ (Aw.' * bw);
    predictedWhiteRatio = aTarget * coef;
    fittedList = A * coef;

    info.FallbackUsed = false;
    info.ConditionValue = rcond(normalMat);
    info.FittedWhiteRatioList = fittedList;
    info.FitResidualList = fittedList - whiteRatioList;
end

function result = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
    rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, storedWhiteRatio)

    result = empty_best_result();

    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    maxWhiteRatio = white_ratio_from_rayt(target_xyY, singleWhiteXY, boundaryRayT);
    whiteRatio = min(max(storedWhiteRatio, 0), maxWhiteRatio);

    rayT = rayt_from_white_ratio(target_xyY, singleWhiteXY, whiteRatio);
    rayT = min(max(rayT, 1), boundaryRayT);

    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);
end

function T = build_stored_ratio_candidate_table(candidateStruct)
    validMask = [candidateStruct.Valid];
    if ~any(validMask)
        T = table();
        return;
    end

    c = candidateStruct(validMask);
    T = table( ...
        [c.Rank].', ...
        [c.LutIndex0].', ...
        [c.CandidateDeltaUv].', ...
        reshape([c.SelectedGridTarget_xyY], 3, []).', ...
        [c.StoredBestRa].', ...
        [c.StoredWhiteRatio].', ...
        [c.FitModelWeight].', ...
        [c.WeightedContribution].', ...
        [c.FittedWhiteRatio].', ...
        [c.FitResidual].', ...
        [c.Ra].', ...
        'VariableNames', { ...
            'Rank', ...
            'LutIndex0', ...
            'Distance', ...
            'GridTarget_xyY', ...
            'OwnBestRa', ...
            'StoredWhiteRatio', ...
            'FitWeight', ...
            'DistanceWeightedContribution', ...
            'FittedWhiteRatioAtGrid', ...
            'FitResidual', ...
            'SingleRatioAppliedToTargetRa'});

    T = sortrows(T, {'Rank'}, {'ascend'});
end

function print_result_summary(result, targetUv, label)
    if ~result.Valid
        fprintf('%s: invalid result.\n', label);
        return;
    end

    outputUv = xy_to_uv1976(result.RGBPoint_xyY(1:2));
    fprintf('%s Ra                = %.6f\n', label, result.Ra);
    fprintf('%s WhiteRatio        = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s RGBWhiteRatio     = %.6f\n', label, result.RGBWhiteRatio);
    fprintf('%s RGBPoint xyY      = (%.6f, %.6f, %.6f)\n', label, ...
        result.RGBPoint_xyY(1), result.RGBPoint_xyY(2), result.RGBPoint_xyY(3));
    fprintf('%s RGB weights       = (R=%.6f, G=%.6f, B=%.6f)\n', label, ...
        result.R_weight, result.G_weight, result.B_weight);
    fprintf('%s RGBPoint uv       = (%.6f, %.6f)\n', label, outputUv(1), outputUv(2));
    fprintf('%s OutputDeltaUv     = %.6f (constructed target ray)\n', label, 0.0);
end

function print_stored_ratio_summary(result, targetUv, label)
    if ~result.Valid
        fprintf('%s: invalid result.\n', label);
        return;
    end

    outputUv = xy_to_uv1976(result.Output_xyY(1:2));
    fprintf('%s Ra                  = %.6f\n', label, result.Ra);
    fprintf('%s WhiteRatio          = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s DistanceWeightedAverageWhiteRatio = %.6f\n', label, result.RawWeightedWhiteRatio);
    fprintf('%s LocalFitPredictedWhiteRatio       = %.6f\n', label, result.WeightedWhiteRatio);
    fprintf('%s UsedWhiteRatio                    = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s LocalFitFallbackUsed              = %.0f\n', label, result.LocalFitFallbackUsed);
    fprintf('%s LocalFitConditionValue            = %.6e\n', label, result.LocalFitConditionValue);
    fprintf('%s RGBWhiteRatio       = %.6f\n', label, result.RGBWhiteRatio);
    fprintf('%s ResidualXYZ         = %.9f\n', label, result.ResidualXYZ);
    fprintf('%s OutputDeltaUv       = %.9f\n', label, result.OutputDeltaUv);
    fprintf('%s LUT index           = %d\n', label, int32(result.LutIndex0));
    fprintf('%s Candidate count     = %u\n', label, uint32(result.CandidateCount));
    fprintf('%s Used fallback       = %u\n', label, uint32(result.UsedFallback));
    fprintf('%s Grid target uv      = (%.6f, %.6f)\n', label, ...
        result.SelectedGridTarget_uv(1), result.SelectedGridTarget_uv(2));
    fprintf('%s RatioK RGB xyY      = (%.6f, %.6f, %.6f)\n', label, ...
        result.RGBPoint_xyY(1), result.RGBPoint_xyY(2), result.RGBPoint_xyY(3));
    fprintf('%s Output xyY          = (%.6f, %.6f, %.6f)\n', label, ...
        result.Output_xyY(1), result.Output_xyY(2), result.Output_xyY(3));
    fprintf('%s Output uv           = (%.6f, %.6f)\n', label, outputUv(1), outputUv(2));
    fprintf('%s RGB weights         = (R=%.6f, G=%.6f, B=%.6f)\n', label, ...
        result.R_weight, result.G_weight, result.B_weight);
    fprintf('%s BoundaryEdge        = %s\n', label, char(result.BoundaryEdge));
    fprintf('%s Delta uv to target  = %.9f\n', label, norm(outputUv - targetUv));
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
end

function s = empty_stored_ratio_candidate()
    s.Valid = false;
    s.Rank = NaN;
    s.LutIndex0 = NaN;
    s.CandidateDeltaUv = NaN;
    s.DistanceWeight = NaN;
    s.WeightedContribution = NaN;
    s.DenoiseUsed = NaN;
    s.DenoiseDistanceWeight = NaN;
    s.DenoiseContribution = NaN;
    s.FitModelWeight = NaN;
    s.FittedWhiteRatio = NaN;
    s.FitResidual = NaN;
    s.StoredWhiteRatio = NaN;
    s.StoredBestRa = NaN;
    s.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    s.BestRGBPointDeltaUvToTargetBest = NaN;
    s.SelectedGridTarget_xyY = [NaN, NaN, NaN];
    s.SelectedGridTarget_uv = [NaN, NaN];
    s.BoundaryEdge = "";
    s.ResidualXYZ = NaN;
    s.OutputDeltaUv = NaN;
    s.Output_xyY = [NaN, NaN, NaN];
    s.Ra = NaN;
    s.Rmin8 = NaN;
    s.SingleWhiteRatio = NaN;
    s.RGBWhiteRatio = NaN;
    s.RGBPoint_xyY = [NaN, NaN, NaN];
    s.R_weight = NaN;
    s.G_weight = NaN;
    s.B_weight = NaN;
end

function s = empty_stored_ratio_result()
    s = empty_best_result();
    s.ResidualXYZ = NaN;
    s.OutputDeltaUv = NaN;
    s.Output_xyY = [NaN, NaN, NaN];
    s.StoredWhiteRatio = NaN;
    s.StoredBestRa = NaN;
    s.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    s.BestRGBPointDeltaUvToTargetBest = NaN;
    s.LutIndex0 = NaN;
    s.SelectedGridTarget_xyY = [NaN, NaN, NaN];
    s.SelectedGridTarget_uv = [NaN, NaN];
    s.BoundaryEdge = "";
    s.UsedFallback = false;
    s.CandidateCount = 0;
    s.SearchDeltaUvLimit = NaN;
    s.RawWeightedWhiteRatio = NaN;
    s.WeightedWhiteRatio = NaN;
    s.DenoiseTriggered = NaN;
    s.DenoiseDroppedRank = NaN;
    s.DenoiseAdaptiveThreshold = NaN;
    s.DenoiseGapToOthers = NaN;
    s.DenoiseUsedCount = NaN;
    s.LocalFitFallbackUsed = NaN;
    s.LocalFitConditionValue = NaN;
    s.CandidateTable = table();
end

function best = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount)

    best = empty_best_result();
    targetXY = target_xyY(1:2);
    [pointXY, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);

    rayTList = linspace(1, boundaryRayT, rayScanCount);
    rayTList(end) = boundaryRayT;

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


function result = calculate_boundary_blend_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda)

    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);

    % BoundaryMix method:
    % RGB point is the intersection between the ray from white point through target
    % and the RGB gamut boundary. The target color is still locked by mixing this
    % boundary RGB point with the single white point.
    boundaryRgbPointXY = singleWhiteXY + boundaryRayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, boundaryRgbPointXY, boundaryRayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);
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

function [pointXY, rayT, edgeName, edgeIndex] = find_ray_gamut_intersection(whiteXY, targetXY, triangleXY)
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    triangleXY = double(triangleXY(:, 1:2));
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_ray_gamut_intersection:SamePoint', 'White and target have the same xy.');
    end

    tol = 1e-12;
    rayT = Inf;
    edgeIndex = NaN;
    pointXY = [NaN, NaN];

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

function [weights, rgbSpd] = solve_rgb_xyz_spectrum(rgbXYZ, rgbSolveMatrix, spd)
    b = rgbXYZ(:);
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;

    if any(weights < -1e-8)
        error('solve_rgb_xyz_spectrum:OutsideGamut', ...
            'The residual RGB XYZ is outside the measured RGB gamut.');
    end

    rgbSpd = weights(1) .* spd.R + weights(2) .* spd.G + weights(3) .* spd.B;
end

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xy = XYZ_to_xyY(XYZ);
    xyY = [xy(1), xy(2), 683 * XYZ(2)];
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
    x = xyY(1);
    y = xyY(2);
    Y = xyY(3);
    if Y == 0
        XYZ = [0, 0, 0];
        return;
    end
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
    x = xy(1);
    y = xy(2);
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
        dist2(i) = sum((uv - bbuv) .^ 2);
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
    d2 = sum((uv - bbuv) .^ 2);
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
