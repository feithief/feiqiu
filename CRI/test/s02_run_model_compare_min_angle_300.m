% S02_RUN_MODEL_COMPARE_RATIOK_300
% 中文说明：
% 基于同一套 LUT，同时比较 6 种方法：
%   1. 精确法（射线扫描，离线基准）
%   2. 投影1点法（最近1个网格点的 Best_RGBPoint 投影）
%   3. 投影K点法（最近K个网格点的 Best_RGBPoint 加权投影）
%   4. 同比例法（最近1个网格点的白光比例直接套用到当前目标色）
%   5. 纯RGB法
%
% 输出：
%   triangle_grid_lut_output/model_compare_results.csv

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S02 当前版本：6模型统一比较 + 300个均匀色域目标点\n');
fprintf('模型 = 精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB\n');
fprintf('============================================================\n\n');

%% ================= 用户可配置参数 =================
nearestGridMetric = 'uv1976';   % 'uv1976' or 'xy'
nearestGridK = 6;               % 投影K点法使用的最近点数量
nearestGridWeightPower = 2;     % 距离权重幂次
rayScanCount = 1001;            % 精确法射线扫描点数
printRealtimeRa = true;         % 是否实时打印结果

% 目标颜色点来源：
%   'auto_uniform_gamut'：自动在RGB色域三角形内生成均匀分布目标点
%   'manual_list'        ：使用下面 manualTargetList 手动目标点
targetPointMode = 'auto_uniform_gamut';

% 自动目标点数量：
%   targetAutoCount = 300 时，脚本会自动选择23阶三角网格：
%   (23+1)*(23+2)/2 = 300 点
targetAutoCount = 300;

% 自动目标点亮度：
%   NaN：使用 guangpu_case_config().target_xyY(3)
%   指定数值：所有自动目标点使用该Y值
targetAutoY_lm = NaN;

% 手动目标点列表，仅当 targetPointMode = 'manual_list' 时使用。
manualTargetList = [ ...
0.3169  0.3310  3.2310;
0.3197  0.3497  3.1980;
0.4016  0.5034  2.9180;
0.4165  0.4966  2.8880;
0.4310  0.4900  2.8510;
0.4576  0.4619  2.7920;
0.4959  0.4449  2.6830;
0.5289  0.4162  2.5930;
0.5532  0.3927  2.5130;
0.5897  0.3655  2.3750;
0.6846  0.3072  1.9420;
0.6540  0.2906  1.9070;
0.6282  0.2821  1.9150;
0.5211  0.2316  1.8350;
0.2082  0.0680  1.2650;
0.1486  0.0288  0.5840;
0.1579  0.1053  1.7290;
0.1586  0.1409  1.9570;
0.1580  0.1664  2.0940;
0.1609  0.2060  2.2930;
0.1667  0.2407  2.4490;
0.1667  0.2828  2.5860;
0.1766  0.3402  2.7720;
0.1800  0.3800  2.8720;
0.1808  0.4144  2.9310;
0.1861  0.5109  3.1060;
0.1934  0.6418  3.2690;
0.2368  0.6205  3.2180;
0.3092  0.5649  3.1110;
0.3696  0.5362  3.0010];

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
resultFile = fullfile(outputDir, 'model_compare_results.csv');

if ~exist(lutFile, 'file')
    error(['没有找到 LUT 文件：\n%s\n\n' ...
           '请先运行 s01_make_lut_adaptive.m。'], lutFile);
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

%% ================= 生成目标颜色点 =================
if strcmpi(targetPointMode, 'auto_uniform_gamut')
    if isnan(targetAutoY_lm)
        autoTargetY_lm = caseCfg.target_xyY(3);
    else
        autoTargetY_lm = targetAutoY_lm;
    end

    targetList = generate_uniform_uv_targets_in_rgb_gamut(rgbTriangleXY, targetAutoCount, autoTargetY_lm);

    fprintf('目标点模式：自动均匀色域取点\n');
    fprintf('目标点数量：%d\n', size(targetList, 1));
    fprintf('目标点亮度Y：%.4f\n', autoTargetY_lm);
elseif strcmpi(targetPointMode, 'manual_list')
    targetList = manualTargetList;
    fprintf('目标点模式：手动目标点列表\n');
    fprintf('目标点数量：%d\n', size(targetList, 1));
else
    error('未知 targetPointMode：%s', targetPointMode);
end


%% ================= 读取 LUT =================
lut = readtable(lutFile, 'VariableNamingRule', 'preserve');

requiredCols = {'Target_x','Target_y','Target_u1976','Target_v1976', ...
    'Best_SingleWhiteRatio','Best_RGBPoint_x','Best_RGBPoint_y'};
for iCol = 1:numel(requiredCols)
    if ~ismember(requiredCols{iCol}, lut.Properties.VariableNames)
        error('LUT 缺少列：%s', requiredCols{iCol});
    end
end

%% ================= 主循环 =================
nTarget = size(targetList, 1);
allResult = repmat(make_empty_result_struct(), nTarget, 1);

fprintf('开始批量计算，共 %d 个目标颜色点...\n', nTarget);

for k = 1:nTarget
    target_xyY = targetList(k, :);
    singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, dLambda);
    singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
    singleWhiteXY = singleMetrics.xyY(1:2);

    item = make_empty_result_struct();
    item.Index = k;
    item.Target_x = target_xyY(1);
    item.Target_y = target_xyY(2);
    item.Target_Y_lm = target_xyY(3);
    item.Target_u1976 = xy_to_uv1976(target_xyY(1:2));
    item.Target_v1976 = item.Target_u1976(2);
    item.Target_u1976 = item.Target_u1976(1);

    fprintf('处理中 %02d/%02d: target=(%.4f, %.4f, %.4f)\n', ...
        k, nTarget, target_xyY(1), target_xyY(2), target_xyY(3));

    % 精确法
    try
        exact = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, rayScanCount);
    catch ME
        warning('目标点 %d 精确法失败：%s', k, ME.message);
        exact = empty_best_result();
    end
    item = fill_method_fields(item, 'Exact', exact);

    % 投影1点法
    try
        proj1 = find_grid_rgb_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, 1, nearestGridWeightPower);
    catch ME
        warning('目标点 %d 投影1点法失败：%s', k, ME.message);
        proj1 = empty_best_result();
    end
    item = fill_method_fields(item, 'Proj1', proj1);

    % 投影K点法
    try
        projK = find_grid_rgb_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower);
    catch ME
        warning('目标点 %d 投影K点法失败：%s', k, ME.message);
        projK = empty_best_result();
    end
    item = fill_method_fields(item, 'ProjK', projK);

    % 同比例法
    try
        ratio1 = find_grid_same_ratio_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric);
    catch ME
        warning('目标点 %d 同比例法失败：%s', k, ME.message);
        ratio1 = empty_best_result();
    end
    item = fill_method_fields(item, 'Ratio1', ratio1);

    % 同比例K点法：最近K个网格点的白光比例做加权平均
    try
        ratioK = find_grid_same_ratio_k_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, lut, nearestGridMetric, nearestGridK, nearestGridWeightPower);
    catch ME
        warning('目标点 %d 同比例K点法失败：%s', k, ME.message);
        ratioK = empty_best_result();
    end
    item = fill_method_fields(item, 'RatioK', ratioK);

    % 纯RGB法
    try
        pure = calculate_pure_rgb_result(target_xyY, rgbSolveMatrix, spd, std, wl, dLambda);
    catch ME
        warning('目标点 %d 纯RGB法失败：%s', k, ME.message);
        pure = empty_best_result();
    end
    item = fill_method_fields(item, 'PureRGB', pure);

    [item.Approx_Max_Ra, item.Approx_Max_Source] = select_max_ra( ...
        item.Proj1_Ra, item.ProjK_Ra, item.Ratio1_Ra, item.RatioK_Ra, item.PureRGB_Ra, ...
        ["Proj1","ProjK","Ratio1","RatioK","PureRGB"]);
    [item.Overall_Max_Ra, item.Overall_Max_Source] = select_max_ra( ...
        item.Exact_Ra, item.Proj1_Ra, item.ProjK_Ra, item.Ratio1_Ra, item.RatioK_Ra, item.PureRGB_Ra, ...
        ["Exact","Proj1","ProjK","Ratio1","RatioK","PureRGB"]);

    if printRealtimeRa
        fprintf(['Ra结果 %02d/%02d: 精确=%.4f | 投影1点=%.4f | 投影K点=%.4f | ' ...
                 '同比例1点=%.4f | 同比例K点=%.4f | 纯RGB=%.4f | 近似最大=%.4f | 来源=%s\n'], ...
            k, nTarget, item.Exact_Ra, item.Proj1_Ra, item.ProjK_Ra, ...
            item.Ratio1_Ra, item.RatioK_Ra, item.PureRGB_Ra, item.Approx_Max_Ra, char(item.Approx_Max_Source));

        fprintf(['              目标xy=(%.4f, %.4f) | K参考网格xy=(%.5f, %.5f) | ' ...
                 'K法RayT=%.4f | K法W比例=%.4f | K法RGB比例=%.4f\n'], ...
            item.Target_x, item.Target_y, ...
            item.ProjK_SelectedGridTarget_x, item.ProjK_SelectedGridTarget_y, ...
            item.ProjK_RayT, item.ProjK_SingleWhiteRatio, item.ProjK_RGBWhiteRatio);
        drawnow;
    end

    allResult(k) = item;
end

resultTable = struct2table(allResult);
writetable(resultTable, resultFile);

fprintf('\n批量计算完成。\n结果文件：\n%s\n', resultFile);

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


function targetList = generate_uniform_uv_targets_in_rgb_gamut(rgbTriangleXY, targetCount, targetY_lm)
    % 在RGB色域三角形内部生成均匀分布的目标点。
    % 均匀空间：CIE 1976 u'v'
    % 当 targetCount=300 时，自动选择 divCount=23，正好得到300点。

    divCount = find_triangle_div_count_for_point_count(targetCount);
    pointCount = (divCount + 1) * (divCount + 2) / 2;

    rgbUv = zeros(3, 2);
    for i = 1:3
        rgbUv(i, :) = xy_to_uv1976(rgbTriangleXY(i, :));
    end

    targetList = nan(pointCount, 3);
    idx = 0;

    for i = 0:divCount
        for j = 0:(divCount - i)
            k = divCount - i - j;

            uv = (i .* rgbUv(1, :) + j .* rgbUv(2, :) + k .* rgbUv(3, :)) ./ divCount;
            xy = uv1976_to_xy(uv);

            idx = idx + 1;
            targetList(idx, :) = [xy(1), xy(2), targetY_lm];
        end
    end
end

function divCount = find_triangle_div_count_for_point_count(targetCount)
    % 选择三角网格阶数，使点数尽量等于目标点数。
    % 若无法精确匹配，则选择第一个点数 >= targetCount 的阶数。
    divCount = 0;

    while true
        pointCount = (divCount + 1) * (divCount + 2) / 2;
        if pointCount >= targetCount
            return;
        end
        divCount = divCount + 1;
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
