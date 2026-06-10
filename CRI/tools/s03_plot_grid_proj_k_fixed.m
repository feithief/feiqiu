% S03_PLOT_GRID_PROJ_K
% 中文说明：
% 图示“u'v'最近K点RGB投影”的几何关系。
%
% 重要说明：
%   1. 这版不再画“垂线投影”，因为新 batch 算法已经删除垂线。
%   2. 优先读取 batch_compare_targets_grid_results.mat 里面保存的 lut，
%      保证图里的网格点和 batch 实际使用的网格点完全一致。
%   3. G 点会用 batch 输出的 SelectedGridTarget_x/y，并且检查它是否存在于同一份 LUT 中。
%
% 使用顺序：
%   calculate_guangpu_triangle_grid_lut_uniform_xy
%   batch_compare_targets_grid_ratio_pure_rgb_xy_nearest_cn
%   plot_grid_ratio_geometry_zh_fixed

clear;
clc;

%% ================= 用户可配置参数 =================
selectedTargetIndex = 2;

%% ================= 定位文件 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

batchCsvFile = fullfile(outputDir, 'batch_compare_targets_grid_results.csv');
batchMatFile = fullfile(outputDir, 'batch_compare_targets_grid_results.mat');
lutCsvFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');

if ~exist(batchCsvFile, 'file')
    error('没有找到batch结果CSV：%s\n请先运行 batch_compare_targets_grid_ratio_pure_rgb_xy_nearest_cn。', batchCsvFile);
end

batchData = readtable(batchCsvFile);

if selectedTargetIndex < 1 || selectedTargetIndex > height(batchData)
    error('selectedTargetIndex=%d 超出范围，当前batch结果只有 %d 行。', selectedTargetIndex, height(batchData));
end

%% ================= 读取与batch一致的LUT =================
if exist(batchMatFile, 'file')
    matData = load(batchMatFile);
    if isfield(matData, 'lut')
        lut = matData.lut;
    else
        lut = readtable(lutCsvFile);
    end
else
    lut = readtable(lutCsvFile);
end

if ismember('Valid', lut.Properties.VariableNames)
    lut = lut(logical(lut.Valid), :);
end

%% ================= 读取色域 =================
addpath(inputDir);
caseCfg = guangpu_case_config();

try
    [rgbTriangleXY, singleWhiteXY] = load_actual_triangle_and_white(inputDir, standardDir, caseCfg);
catch
    rgbTriangleXY = [caseCfg.channel_xyY.R(1:2); ...
                     caseCfg.channel_xyY.G(1:2); ...
                     caseCfg.channel_xyY.B(1:2)];
    singleWhiteXY = caseCfg.single_white_xyY(1:2);
end

Rxy = rgbTriangleXY(1, :);
Gxy = rgbTriangleXY(2, :);
Bxy = rgbTriangleXY(3, :);
Wxy = singleWhiteXY;

%% ================= 当前目标点数据 =================
row = batchData(selectedTargetIndex, :);

Txy = [row.Target_x, row.Target_y];
selectedGridXY = [row.SelectedGridTarget_x, row.SelectedGridTarget_y];
storedBestRgbXY = [row.Grid_StoredBestRgb_x, row.Grid_StoredBestRgb_y];
ratioRgbXY = [row.Grid_ProjectedRgb_x, row.Grid_ProjectedRgb_y];
exactRgbXY = [row.Exact_RGBPoint_x, row.Exact_RGBPoint_y];

targetGridXY = [lut.Target_x, lut.Target_y];
allBestRgbXY = [lut.Best_RGBPoint_x, lut.Best_RGBPoint_y];

% 检查 G 是否真的来自当前LUT。
gridDistanceToG = vecnorm(targetGridXY - selectedGridXY, 2, 2);
[minGridDistanceToG, nearestGridRow] = min(gridDistanceToG);
isGInCurrentLut = minGridDistanceToG < 1e-10;

% 为了调试，也计算当前xy平面下真正最近的LUT点。
xyDistanceToTarget = vecnorm(targetGridXY - Txy, 2, 2);
[minXYDistance, nearestXYRow] = min(xyDistanceToTarget);
nearestXYGrid = targetGridXY(nearestXYRow, :);

% 当前方法的点是否在RGB色域内。
storedInside = point_in_triangle(storedBestRgbXY, rgbTriangleXY);
ratioInside = point_in_triangle(ratioRgbXY, rgbTriangleXY);
exactInside = point_in_triangle(exactRgbXY, rgbTriangleXY);

%% ================= 绘图 =================
fig = figure('Name', sprintf('u''v''最近K点RGB投影 - 目标点 %d', selectedTargetIndex), ...
             'NumberTitle', 'off', ...
             'Color', 'w', ...
             'Position', [80, 60, 1450, 780]);

ax = axes('Parent', fig, 'Position', [0.055, 0.105, 0.68, 0.82]);
hold(ax, 'on');
grid(ax, 'on');
box(ax, 'on');
axis(ax, 'equal');

% RGB三角色域
patch(ax, ...
      [Rxy(1), Gxy(1), Bxy(1)], ...
      [Rxy(2), Gxy(2), Bxy(2)], ...
      [0.94, 0.94, 0.94], ...
      'EdgeColor', [0.15, 0.15, 0.15], ...
      'LineWidth', 1.8, ...
      'FaceAlpha', 0.55, ...
      'DisplayName', 'RGB三角色域');

% 所有网格点和最佳RGB点
scatter(ax, targetGridXY(:,1), targetGridXY(:,2), 12, ...
    'MarkerEdgeColor', [0.55, 0.55, 0.55], ...
    'MarkerFaceColor', [0.72, 0.72, 0.72], ...
    'DisplayName', 'LUT目标色网格点');

scatter(ax, allBestRgbXY(:,1), allBestRgbXY(:,2), 15, ...
    'MarkerEdgeColor', [0.10, 0.30, 0.85], ...
    'MarkerFaceColor', [0.25, 0.50, 1.00], ...
    'DisplayName', '所有网格点最佳RGB点');

% 当前目标射线 W->T->P
if all(isfinite(ratioRgbXY))
    plot(ax, [Wxy(1), Txy(1), ratioRgbXY(1)], ...
             [Wxy(2), Txy(2), ratioRgbXY(2)], ...
             '-', 'Color', [0.85, 0.05, 0.05], 'LineWidth', 2.2, ...
             'DisplayName', '当前目标射线 W→T→P');
else
    plot(ax, [Wxy(1), Txy(1)], [Wxy(2), Txy(2)], ...
             '-', 'Color', [0.85, 0.05, 0.05], 'LineWidth', 2.2, ...
             'DisplayName', '当前目标射线 W→T');
end

% 新算法不画垂线，只画“P0提供RayT比例”关系
if all(isfinite(storedBestRgbXY)) && all(isfinite(ratioRgbXY))
    plot(ax, [storedBestRgbXY(1), ratioRgbXY(1)], ...
             [storedBestRgbXY(2), ratioRgbXY(2)], ...
             ':', 'Color', [0.0, 0.45, 0.85], 'LineWidth', 1.6, ...
             'DisplayName', 'K点加权RGB参考');
end

% 关键点
plot(ax, Wxy(1), Wxy(2), 'ko', 'MarkerSize', 10, 'LineWidth', 1.8, ...
    'MarkerFaceColor', 'w', 'DisplayName', '单白光点W');

plot(ax, Txy(1), Txy(2), 'o', 'MarkerSize', 10, 'LineWidth', 1.8, ...
    'MarkerEdgeColor', [0.85,0,0], 'MarkerFaceColor', [1.0,0.25,0.25], ...
    'DisplayName', '当前目标点T');

plot(ax, selectedGridXY(1), selectedGridXY(2), 'd', 'MarkerSize', 10, 'LineWidth', 1.8, ...
    'MarkerEdgeColor', [0.55,0,0.75], 'MarkerFaceColor', [0.85,0.25,1.0], ...
    'DisplayName', 'batch采用网格点G');

% 如果当前xy最近点和batch采用G不一致，额外标出来
if norm(nearestXYGrid - selectedGridXY) > 1e-10
    plot(ax, nearestXYGrid(1), nearestXYGrid(2), 'h', 'MarkerSize', 11, 'LineWidth', 1.8, ...
        'MarkerEdgeColor', [1.0,0.45,0.0], 'MarkerFaceColor', [1.0,0.85,0.25], ...
        'DisplayName', '当前xy真正最近网格点');
    text(ax, nearestXYGrid(1), nearestXYGrid(2), '  xy最近', ...
        'FontSize', 10, 'FontWeight', 'bold', 'Color', [0.75,0.30,0]);
end

plot(ax, storedBestRgbXY(1), storedBestRgbXY(2), 's', 'MarkerSize', 10, 'LineWidth', 1.8, ...
    'MarkerEdgeColor', [0.0,0.15,0.85], 'MarkerFaceColor', [0.25,0.45,1.0], ...
    'DisplayName', 'K点加权参考RGB点Pw');

plot(ax, ratioRgbXY(1), ratioRgbXY(2), 'p', 'MarkerSize', 13, 'LineWidth', 1.8, ...
    'MarkerEdgeColor', [0.0,0.45,0.1], 'MarkerFaceColor', [0.3,0.85,0.35], ...
    'DisplayName', '按RayT比例得到的RGB点P');

plot(ax, exactRgbXY(1), exactRgbXY(2), '^', 'MarkerSize', 10, 'LineWidth', 1.8, ...
    'MarkerEdgeColor', [0,0.55,0.65], 'MarkerFaceColor', [0.35,0.95,1.0], ...
    'DisplayName', '精确法最佳RGB点');

text(ax, Wxy(1), Wxy(2), '  W', 'FontSize', 10, 'FontWeight', 'bold');
text(ax, Txy(1), Txy(2), sprintf('  T%d', selectedTargetIndex), 'FontSize', 10, 'FontWeight', 'bold', 'Color', [0.85,0,0]);
text(ax, selectedGridXY(1), selectedGridXY(2), '  G', 'FontSize', 10, 'FontWeight', 'bold', 'Color', [0.55,0,0.75]);
text(ax, storedBestRgbXY(1), storedBestRgbXY(2), '  Pw', 'FontSize', 10, 'FontWeight', 'bold', 'Color', [0.0,0.15,0.85]);
text(ax, ratioRgbXY(1), ratioRgbXY(2), '  P', 'FontSize', 10, 'FontWeight', 'bold', 'Color', [0.0,0.45,0.1]);
text(ax, exactRgbXY(1), exactRgbXY(2), '  Exact', 'FontSize', 10, 'FontWeight', 'bold', 'Color', [0,0.55,0.65]);

xlabel(ax, 'CIE 1931 色坐标 x');
ylabel(ax, 'CIE 1931 色坐标 y');
title(ax, sprintf('目标点 %d：u''v''最近K点RGB投影', selectedTargetIndex), ...
    'FontSize', 13, 'FontWeight', 'bold');

allXY = [rgbTriangleXY; Wxy; Txy; selectedGridXY; storedBestRgbXY; ratioRgbXY; exactRgbXY; targetGridXY; allBestRgbXY];
allXY = allXY(all(isfinite(allXY), 2), :);
xPad = 0.06;
yPad = 0.06;
xlim(ax, [min(allXY(:,1))-xPad, max(allXY(:,1))+xPad]);
ylim(ax, [min(allXY(:,2))-yPad, max(allXY(:,2))+yPad]);

legend(ax, 'Location', 'southoutside', 'NumColumns', 3, 'Box', 'off');

% 右侧信息面板
panel = uipanel('Parent', fig, ...
    'Title', '当前目标点计算结果', ...
    'FontSize', 11, ...
    'FontWeight', 'bold', ...
    'BackgroundColor', [0.98, 0.98, 0.98], ...
    'Position', [0.76, 0.12, 0.22, 0.78]);

infoText = sprintf([ ...
    '目标点序号：%d\n\n' ...
    '目标点 T:\n  (%.6f, %.6f)\n\n' ...
    'batch采用网格点 G:\n  (%.6f, %.6f)\n  是否存在于当前LUT：%d\n  到当前LUT最近距离：%.3g\n\n' ...
    '当前xy真正最近网格点:\n  (%.6f, %.6f)\n  xy距离：%.6g\n\n' ...
    'K点加权RGB参考点 Pw:\n  (%.6f, %.6f)\n  色域内：%d\n\n' ...
    '投影后RGB点 P:\n  (%.6f, %.6f)\n  色域内：%d\n\n' ...
    '精确法最佳RGB点:\n  (%.6f, %.6f)\n  色域内：%d\n\n' ...
    '精确法 Ra：%.6f\n' ...
    '网格法 Ra：%.6f\n' ...
    '投影法 RayT：%.6f\n' ...
    'K点数量：%g\n' ...
    '权重幂次：%g\n\n' ...
    '网格法白色比例：%.6f\n' ...
    '网格法RGB比例：%.6f'], ...
    selectedTargetIndex, ...
    Txy(1), Txy(2), ...
    selectedGridXY(1), selectedGridXY(2), isGInCurrentLut, minGridDistanceToG, ...
    nearestXYGrid(1), nearestXYGrid(2), minXYDistance, ...
    storedBestRgbXY(1), storedBestRgbXY(2), storedInside, ...
    ratioRgbXY(1), ratioRgbXY(2), ratioInside, ...
    exactRgbXY(1), exactRgbXY(2), exactInside, ...
    row.Exact_Ra, row.Grid_Ra, row.Grid_RayT, ...
    get_table_value(row, 'Grid_KUsed'), get_table_value(row, 'Grid_WeightPower'), ...
    row.Grid_SingleWhiteRatio, row.Grid_RGBWhiteRatio);

uicontrol('Parent', panel, ...
    'Style', 'edit', ...
    'String', infoText, ...
    'Units', 'normalized', ...
    'Position', [0.05, 0.04, 0.90, 0.92], ...
    'Max', 2, ...
    'Min', 0, ...
    'HorizontalAlignment', 'left', ...
    'FontName', 'Consolas', ...
    'FontSize', 10, ...
    'BackgroundColor', [1, 1, 1]);

hold(ax, 'off');

try
    ax.Toolbar.Visible = 'off';
catch
end

outputPng = fullfile(outputDir, sprintf('grid_proj_k_geometry_target_%02d.png', selectedTargetIndex));
try
    exportapp(fig, outputPng);
catch
    try
        exportgraphics(ax, outputPng, 'Resolution', 200);
    catch
        warning('图片导出失败，但界面图已正常显示。');
    end
end

fprintf('\n图示完成：%s\n', outputPng);

%% ================= Local functions =================
function inside = point_in_triangle(p, tri)
    if any(~isfinite(p))
        inside = false;
        return;
    end

    a = tri(1, :);
    b = tri(2, :);
    c = tri(3, :);

    v0 = c - a;
    v1 = b - a;
    v2 = p - a;

    dot00 = dot(v0, v0);
    dot01 = dot(v0, v1);
    dot02 = dot(v0, v2);
    dot11 = dot(v1, v1);
    dot12 = dot(v1, v2);

    denom = dot00 * dot11 - dot01 * dot01;
    if abs(denom) < 1e-12
        inside = false;
        return;
    end

    invDenom = 1 / denom;
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    tol = 1e-7;
    inside = (u >= -tol) && (v >= -tol) && ((u + v) <= 1 + tol);
end

function [rgbTriangleXY, singleWhiteXY] = load_actual_triangle_and_white(inputDir, standardDir, caseCfg)
    if isfield(caseCfg, 'inputDir') && ~isempty(caseCfg.inputDir)
        spectraDir = caseCfg.inputDir;
    else
        spectraDir = inputDir;
    end

    wl = caseCfg.wavelength_nm;
    dLambda = caseCfg.dLambda;
    std = load_standard_data(standardDir, wl);

    raw.R = read_spd(fullfile(spectraDir, 'R.CSV'), '', wl);
    raw.G = read_spd(fullfile(spectraDir, 'G.CSV'), '', wl);
    raw.B = read_spd(fullfile(spectraDir, 'B.CSV'), '', wl);
    raw.W = read_spd(fullfile(spectraDir, 'W.CSV'), '', wl);

    spd.R = scale_to_photopic_Y(raw.R, caseCfg.channel_xyY.R(3), std, dLambda);
    spd.G = scale_to_photopic_Y(raw.G, caseCfg.channel_xyY.G(3), std, dLambda);
    spd.B = scale_to_photopic_Y(raw.B, caseCfg.channel_xyY.B(3), std, dLambda);
    spd.W = scale_to_photopic_Y(raw.W, caseCfg.single_white_xyY(3), std, dLambda);

    rxyY = spectrum_xyY(spd.R, std, dLambda);
    gxyY = spectrum_xyY(spd.G, std, dLambda);
    bxyY = spectrum_xyY(spd.B, std, dLambda);
    wxyY = spectrum_xyY(spd.W, std, dLambda);

    rgbTriangleXY = [rxyY(1:2); gxyY(1:2); bxyY(1:2)];
    singleWhiteXY = wxyY(1:2);
end

function std = load_standard_data(standardDir, wl)
    xyzPath = fullfile(standardDir, 'CIE_xyz_1931_2deg.csv');
    if ~exist(xyzPath, 'file')
        error('缺少标准数据文件：%s', xyzPath);
    end
    xyz = readmatrix(xyzPath);
    std.cmf = interp1(xyz(:,1), xyz(:,2:4), wl, 'linear', 'extrap');
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
    spd = spd(:);
    spd(spd < 0) = 0;
end

function spdScaled = scale_to_photopic_Y(spd, targetYlm, std, dLambda)
    Ylm = 683 * sum(spd(:) .* std.cmf(:,2)) * dLambda;
    if Ylm <= 0
        error('光谱Y为0，无法归一化。');
    end
    spdScaled = spd(:) .* (targetYlm / Ylm);
end

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = sum(spd(:) .* std.cmf, 1) .* dLambda;
    s = sum(XYZ);
    if s <= 0
        xyY = [NaN, NaN, NaN];
    else
        xyY = [XYZ(1) / s, XYZ(2) / s, 683 * XYZ(2)];
    end
end


function value = get_table_value(row, name)
    if ismember(name, row.Properties.VariableNames)
        value = row.(name);
    else
        value = NaN;
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
            error('Cannot find %s from script folder or parent folders.', inputFolderName);
        end

        projectRoot = parentDir;
    end
end
