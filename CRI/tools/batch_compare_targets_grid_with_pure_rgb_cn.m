% BATCH_COMPARE_TARGETS_GRID_CN
% 中文说明：
% 1. 将下方 target_xyY_list 中的目标颜色逐个计算。
% 2. 精确法：对每个目标色，按 backup 脚本的射线扫描逻辑单独求最大 Ra 与最佳 RGB 点。
% 3. 网格法：读取 triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv，
%    取最近有效网格点的最佳 RGB 点，并投影到当前目标色射线上，再计算 Ra。
% 4. 输出所有目标色的精确法/网格法结果，包括 Ra、差值、RGB 点、RayT、RGB 权重等。
%
% 使用前：
%   先运行 calculate_guangpu_triangle_grid_lut_optimized_exact.m 生成三角网格 LUT。

clear;
clc;

%% ================= 用户可配置参数 =================
% 每行格式：[x, y, Y]
target_xyY_list = [
    0.3169, 0.3310, 3.2310;
    0.3197, 0.3497, 3.1980;
    0.4016, 0.5034, 2.9180;
    0.4165, 0.4966, 2.8880;
    0.4310, 0.4900, 2.8510;
    0.4576, 0.4619, 2.7920;
    0.4959, 0.4449, 2.6830;
    0.5289, 0.4162, 2.5930;
    0.5532, 0.3927, 2.5130;
    0.5897, 0.3655, 2.3750;
    0.6846, 0.3072, 1.9420;
    0.6540, 0.2906, 1.9070;
    0.6282, 0.2821, 1.9150;
    0.5211, 0.2316, 1.8350;
    0.2082, 0.0680, 1.2650;
    0.1486, 0.0288, 0.5840;
    0.1579, 0.1053, 1.7290;
    0.1586, 0.1409, 1.9570;
    0.1580, 0.1664, 2.0940;
    0.1609, 0.2060, 2.2930;
    0.1667, 0.2407, 2.4490;
    0.1667, 0.2828, 2.5860;
    0.1766, 0.3402, 2.7720;
    0.1800, 0.3800, 2.8720;
    0.1808, 0.4144, 2.9310;
    0.1861, 0.5109, 3.1060;
    0.1934, 0.6418, 3.2690;
    0.2368, 0.6205, 3.2180;
    0.3092, 0.5649, 3.1110;
    0.3696, 0.5362, 3.0010];

rayScanCountExact = 1001;         % 精确法射线扫描点数，保持和 backup 脚本一致
maxCandidateCheck = 120;          % 最近候选无效时，最多检查多少个网格点
lutFileName = 'guangpu_triangle_grid_lut_results.csv';
outputResultCsv = 'batch_compare_targets_grid_results.csv';
outputResultMat = 'batch_compare_targets_grid_results.mat';

%% ================= 定位工程目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');
addpath(inputDir);

caseCfg = guangpu_case_config();
if isfield(caseCfg, 'inputDir') && ~isempty(caseCfg.inputDir)
    spectraDir = caseCfg.inputDir;
else
    spectraDir = inputDir;
end

singleWhiteFile = fullfile(spectraDir, 'W.CSV');  % 保持和 backup 文件一致
lutFile = fullfile(outputDir, lutFileName);
if ~exist(lutFile, 'file')
    error('没有找到LUT文件：%s\n请先运行 calculate_guangpu_triangle_grid_lut_optimized_exact.m。', lutFile);
end

%% ================= 一次性初始化：不要在每个目标点重复读取 =================
wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;
std = load_standard_data(standardDir, wl);

raw.R = read_spd(fullfile(spectraDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(spectraDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(spectraDir, 'B.CSV'), '', wl);
raw.W = read_spd(fullfile(spectraDir, 'W.CSV'), '', wl);
raw.SingleWhite = read_spd(singleWhiteFile, '', wl);

spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);
actual_xyY.W = spectrum_xyY(spd.W, std, dLambda);
actual_xyY.SingleWhite = spectrum_xyY(spd.SingleWhite, std, dLambda);

singleWhiteXY = actual_xyY.SingleWhite(1:2);
rgbTriangleXY = [actual_xyY.R(1:2); actual_xyY.G(1:2); actual_xyY.B(1:2)];
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', xyY_to_XYZ(actual_xyY.G).', xyY_to_XYZ(actual_xyY.B).'];

lut = readtable(lutFile);
lut = lut(lut.Valid == true, :);
if height(lut) == 0
    error('LUT文件中没有有效网格点。');
end

%% ================= 批量计算 =================
nTarget = size(target_xyY_list, 1);
allResult = repmat(make_empty_result_struct(), nTarget, 1);

fprintf('\n开始批量计算，共 %d 个目标颜色点...\n', nTarget);
for k = 1:nTarget
    target_xyY = target_xyY_list(k, :);
    fprintf('处理中 %02d/%02d: target=(%.4f, %.4f, %.4f)\n', k, nTarget, target_xyY(1), target_xyY(2), target_xyY(3));

    singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda);

    item = make_empty_result_struct();
    item.Index = k;
    item.Target_x = target_xyY(1);
    item.Target_y = target_xyY(2);
    item.Target_Y = target_xyY(3);

    try
        exact = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, rayScanCountExact);
    catch ME
        warning('目标点 %d 精确法失败：%s', k, ME.message);
        allResult(k) = item;
        continue;
    end

    if ~exact.Valid
        warning('目标点 %d 精确法无有效解。', k);
        allResult(k) = item;
        continue;
    end

    try
        grid = find_grid_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, lut, maxCandidateCheck);
    catch ME
        warning('目标点 %d 网格法失败：%s', k, ME.message);
        grid = empty_best_result();
    end

    exactRgbUv = xy_to_uv1976(exact.RGBPoint_xyY(1:2));

    item.Exact_Ra = exact.Ra;
    item.Exact_Rmin8 = exact.Rmin8;
    item.Exact_RGBPoint_x = exact.RGBPoint_xyY(1);
    item.Exact_RGBPoint_y = exact.RGBPoint_xyY(2);
    item.Exact_RGBPoint_u = exactRgbUv(1);
    item.Exact_RGBPoint_v = exactRgbUv(2);
    item.Exact_RayT = exact.RayT;
    item.Exact_R_weight = exact.R_weight;
    item.Exact_G_weight = exact.G_weight;
    item.Exact_B_weight = exact.B_weight;
    item.Exact_SingleWhiteRatio = exact.SingleWhiteRatio;
    item.Exact_RGBWhiteRatio = exact.RGBWhiteRatio;

    if grid.Valid
        gridStoredRgbUv = xy_to_uv1976(grid.StoredBestRgbXY);
        gridProjectedRgbUv = xy_to_uv1976(grid.RGBPoint_xyY(1:2));

        item.Grid_Ra = grid.Ra;
        item.Grid_Rmin8 = grid.Rmin8;
        item.Ra_Diff = exact.Ra - grid.Ra;
        item.Abs_Ra_Diff = abs(item.Ra_Diff);
        item.TargetToSelectedGrid_DeltaUV = grid.SelectedGridDeltaUv;
        item.SelectedGridRank = grid.SelectedCandidateRank;
        item.SkippedCandidateCount = grid.SkippedBeforeValid;
        item.SelectedGridTarget_x = grid.SelectedGridTargetXY(1);
        item.SelectedGridTarget_y = grid.SelectedGridTargetXY(2);

        item.Grid_StoredBestRgb_x = grid.StoredBestRgbXY(1);
        item.Grid_StoredBestRgb_y = grid.StoredBestRgbXY(2);
        item.Grid_StoredBestRgb_u = gridStoredRgbUv(1);
        item.Grid_StoredBestRgb_v = gridStoredRgbUv(2);
        item.Grid_ProjectedRgb_x = grid.RGBPoint_xyY(1);
        item.Grid_ProjectedRgb_y = grid.RGBPoint_xyY(2);
        item.Grid_ProjectedRgb_u = gridProjectedRgbUv(1);
        item.Grid_ProjectedRgb_v = gridProjectedRgbUv(2);
        item.Grid_RawRayT = grid.RawRayT;
        item.Grid_RayT = grid.RayT;
        item.Grid_R_weight = grid.R_weight;
        item.Grid_G_weight = grid.G_weight;
        item.Grid_B_weight = grid.B_weight;
        item.Grid_SingleWhiteRatio = grid.SingleWhiteRatio;
        item.Grid_RGBWhiteRatio = grid.RGBWhiteRatio;
        item.ExactVsProjectedRgb_DeltaUV = norm(exactRgbUv - gridProjectedRgbUv);
        item.StoredVsProjectedRgb_Distance_xy = norm(grid.StoredBestRgbXY - grid.RGBPoint_xyY(1:2));
    end

    allResult(k) = item;
end

%% ================= 输出结果 =================
resultTable = struct2table(allResult);
if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end
writetable(resultTable, fullfile(outputDir, outputResultCsv));
save(fullfile(outputDir, outputResultMat), 'resultTable', 'allResult', 'target_xyY_list', ...
    'singleWhiteXY', 'rgbTriangleXY', 'lut');

fprintf('\n批量计算完成。\n');
fprintf('结果CSV：%s\n', fullfile(outputDir, outputResultCsv));
fprintf('结果MAT：%s\n', fullfile(outputDir, outputResultMat));

%% ================= 局部函数 =================
function s = make_empty_result_struct()
    names = { ...
        'Index','Target_x','Target_y','Target_Y','Exact_Ra','Grid_Ra','Ra_Diff','Abs_Ra_Diff', ...
        'Exact_Rmin8','Grid_Rmin8','TargetToSelectedGrid_DeltaUV','SelectedGridRank','SkippedCandidateCount', ...
        'SelectedGridTarget_x','SelectedGridTarget_y', ...
        'Exact_RGBPoint_x','Exact_RGBPoint_y','Exact_RGBPoint_u','Exact_RGBPoint_v','Exact_RayT', ...
        'Exact_R_weight','Exact_G_weight','Exact_B_weight','Exact_SingleWhiteRatio','Exact_RGBWhiteRatio', ...
        'Grid_StoredBestRgb_x','Grid_StoredBestRgb_y','Grid_StoredBestRgb_u','Grid_StoredBestRgb_v', ...
        'Grid_ProjectedRgb_x','Grid_ProjectedRgb_y','Grid_ProjectedRgb_u','Grid_ProjectedRgb_v','Grid_RawRayT','Grid_RayT', ...
        'Grid_R_weight','Grid_G_weight','Grid_B_weight','Grid_SingleWhiteRatio','Grid_RGBWhiteRatio', ...
        'PureRGB_Valid','PureRGB_Ra','PureRGB_Rmin8','PureRGB_R_weight','PureRGB_G_weight','PureRGB_B_weight', ...
        'PureRGB_Is_RayMax','Final_Max_Ra','Final_Max_Source', ...
        'ExactVsProjectedRgb_DeltaUV','StoredVsProjectedRgb_Distance_xy'};
    for i = 1:numel(names)
        s.(names{i}) = NaN;
    end
end

function result = calculate_pure_rgb_result(target_xyY, rgbSolveMatrix, spd, std, wl, dLambda)
    result = empty_best_result();
    try
        [weights, rgbSpd] = solve_rgb_point_spectrum(target_xyY, rgbSolveMatrix, spd);
    catch
        return;
    end
    if any(weights < -1e-8)
        return;
    end

    m = spectrum_metrics(rgbSpd, std, wl, dLambda);
    result.Valid = true;
    result.Ra = m.Ra;
    result.Rmin8 = m.Rmin8;
    result.WorstIndex = m.WorstIndex;
    result.RayT = 1;
    result.SingleWhiteRatio = 0;
    result.RGBWhiteRatio = 1;
    result.RGBPoint_xyY = target_xyY;
    result.R_weight = weights(1);
    result.G_weight = weights(2);
    result.B_weight = weights(3);
end

function gridResult = find_grid_projection_result(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, lut, maxCandidateCheck)

    targetUv = xy_to_uv1976(target_xyY(1:2));
    lutTargetUv = [lut.Target_u1976, lut.Target_v1976];
    lutTargetDeltaUvList = vecnorm(lutTargetUv - targetUv, 2, 2);
    [sortedTargetDeltaUv, sortedIndex] = sort(lutTargetDeltaUvList, 'ascend');

    targetXY = target_xyY(1:2);
    rayDir = targetXY - singleWhiteXY;
    if norm(rayDir) <= eps
        gridResult = empty_best_result();
        return;
    end

    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    candidateLimit = min(maxCandidateCheck, numel(sortedIndex));
    gridResult = empty_best_result();
    skippedBeforeValid = 0;

    for candidateRank = 1:candidateLimit
        rowIndex = sortedIndex(candidateRank);
        candidateRow = lut(rowIndex, :);
        candidateBestRgbXY = [candidateRow.Best_RGBPoint_x, candidateRow.Best_RGBPoint_y];

        rawRayT = dot(candidateBestRgbXY - singleWhiteXY, rayDir) / dot(rayDir, rayDir);
        if rawRayT < 1 - 1e-10 || rawRayT > boundaryRayT + 1e-10
            skippedBeforeValid = skippedBeforeValid + 1;
            continue;
        end

        projectedRgbXY = singleWhiteXY + rawRayT .* rayDir;
        candidateResult = calculate_result_from_rgb_point(target_xyY, projectedRgbXY, rawRayT, ...
            singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

        if candidateResult.Valid
            gridResult = candidateResult;
            gridResult.StoredBestRgbXY = candidateBestRgbXY;
            gridResult.SelectedGridDeltaUv = sortedTargetDeltaUv(candidateRank);
            gridResult.SelectedGridTargetXY = [candidateRow.Target_x, candidateRow.Target_y];
            gridResult.SelectedCandidateRank = candidateRank;
            gridResult.SkippedBeforeValid = skippedBeforeValid;
            gridResult.RawRayT = rawRayT;
            return;
        end

        skippedBeforeValid = skippedBeforeValid + 1;
    end
end

function best = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount)
    best = empty_best_result();
    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    rayTList = linspace(1, boundaryRayT, rayScanCount).';

    for i = 1:numel(rayTList)
        rayT = rayTList(i);
        rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);
        candidate = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
            singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);
        if candidate.Valid && candidate.Ra > best.Ra
            best = candidate;
        end
    end
end

function result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
    singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda)
    result = empty_best_result();
    split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
    rgbPoint_xyY = [rgbPointXY, split.RGBY];
    try
        [pointWeights, rgbPointSpd] = solve_rgb_point_spectrum(rgbPoint_xyY, rgbSolveMatrix, spd);
    catch
        return;
    end
    if any(pointWeights < -1e-8)
        return;
    end

    singlePartSpd = split.WhiteYRatio .* singleNorm;
    mixSpd = singlePartSpd + rgbPointSpd;
    m = spectrum_metrics(mixSpd, std, wl, dLambda);

    result.Valid = true;
    result.Ra = m.Ra;
    result.Rmin8 = m.Rmin8;
    result.WorstIndex = m.WorstIndex;
    result.RayT = rayT;
    result.SingleWhiteRatio = split.WhiteYRatio;
    result.RGBWhiteRatio = split.RGBYRatio;
    result.RGBPoint_xyY = rgbPoint_xyY;
    result.R_weight = pointWeights(1);
    result.G_weight = pointWeights(2);
    result.B_weight = pointWeights(3);
end

function s = empty_best_result()
    s.Valid = false;
    s.Ra = -Inf;
    s.Rmin8 = -Inf;
    s.WorstIndex = NaN;
    s.RayT = NaN;
    s.SingleWhiteRatio = NaN;
    s.RGBWhiteRatio = NaN;
    s.RGBPoint_xyY = [NaN NaN NaN];
    s.R_weight = NaN;
    s.G_weight = NaN;
    s.B_weight = NaN;
    s.StoredBestRgbXY = [NaN NaN];
    s.SelectedGridDeltaUv = NaN;
    s.SelectedGridTargetXY = [NaN NaN];
    s.SelectedCandidateRank = NaN;
    s.SkippedBeforeValid = NaN;
    s.RawRayT = NaN;
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
    if target_y <= eps
        error('split_target_on_ray:InvalidTarget', 'Target y is too small for luminance splitting.');
    end

    lambda = 1 ./ rayT;
    oneMinusLambda = 1 - lambda;

    split.WhiteXYCoeff = oneMinusLambda;
    split.RGBXYCoeff = lambda;
    split.WhiteY = targetY * oneMinusLambda * whiteXY(2) / target_y;
    split.RGBY = targetY * lambda * pointXY(2) / target_y;
    split.WhiteYRatio = split.WhiteY / targetY;
    split.RGBYRatio = split.RGBY / targetY;
end

function [weights, rgbSpd] = solve_rgb_point_spectrum(point_xyY, rgbSolveMatrix, spd)
    b = xyY_to_XYZ(point_xyY).';
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;
    if any(weights < -1e-8)
        error('solve_rgb_point_spectrum:OutsideGamut', ...
            'RGB candidate xy=(%.6f, %.6f) is outside the measured RGB gamut.', point_xyY(1), point_xyY(2));
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
    if ~exist(folder, 'dir')
        mkdir(folder);
    end
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

function spdScaled = scale_to_photopic_Y(spd, targetYlm, std, ~, dLambda)
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
    [CCT, Duv] = estimate_cct_uv(uv, std.cmf, wl, dLambda);
    refSpd = reference_spd(CCT, std, wl);
    Ri = cri_R1_R8(spd, refSpd, std.cmf, std.tcs, dLambda);
    [Rmin8, worstIndex] = min(Ri);
    m.XYZ = XYZ;
    m.xyY = [xyY(1), xyY(2), photopicY];
    m.photopicY = photopicY;
    m.uv1960 = uv;
    m.CCT = CCT;
    m.Duv = Duv;
    m.Ri = Ri;
    m.Ra = mean(Ri);
    m.Rmin8 = Rmin8;
    m.WorstIndex = worstIndex;
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
        u = sampleUV(1); v = sampleUV(2);
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
            error('find_project_root:NotFound', 'Cannot find %s from script folder or its parent folders.', inputFolderName);
        end
        projectRoot = parentDir;
    end
end
