% PLOT_TARGETS_GRID_RESULT_CN
% 中文说明：
% 将批量对比结果用图片输出。
% 图1：全部网格点、全部网格点对应的最佳 RGB 混色点、全部输入目标色、指定目标色的射线/垂线投影关系。
% 图2：每个目标色的精确法 Ra、网格法 Ra、Ra 差值。
%
% 使用前：
%   1. 先运行 calculate_guangpu_triangle_grid_lut_optimized_exact.m 生成 LUT。
%   2. 再运行 batch_compare_targets_grid_cn.m 生成批量对比结果。
%   3. 修改 selectedTargetIndex，然后运行本脚本。

clear;
clc;

%% ================= 用户可配置参数 =================
selectedTargetIndex = 18;        % 指定要重点标注的目标颜色序号，范围：1~目标颜色数量
savePng = true;                 % 是否保存图片到 triangle_grid_lut_output

%% ================= 定位工程目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
addpath(inputDir);

caseCfg = guangpu_case_config();
lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
resultFile = fullfile(outputDir, 'batch_compare_targets_grid_results.csv');

if ~exist(lutFile, 'file')
    error('没有找到LUT文件：%s\n请先运行 calculate_guangpu_triangle_grid_lut_optimized_exact.m。', lutFile);
end
if ~exist(resultFile, 'file')
    error('没有找到批量结果文件：%s\n请先运行 batch_compare_targets_grid_cn.m。', resultFile);
end

lut = readtable(lutFile);
lut = lut(lut.Valid == true, :);
res = readtable(resultFile);

if selectedTargetIndex < 1 || selectedTargetIndex > height(res)
    error('selectedTargetIndex 超出范围，应为 1~%d。', height(res));
end

sel = res(selectedTargetIndex, :);

Rxy = caseCfg.channel_xyY.R(1:2);
Gxy = caseCfg.channel_xyY.G(1:2);
Bxy = caseCfg.channel_xyY.B(1:2);
Wxy = caseCfg.single_white_xyY(1:2);
triangleX = [Rxy(1), Gxy(1), Bxy(1), Rxy(1)];
triangleY = [Rxy(2), Gxy(2), Bxy(2), Rxy(2)];

%% ================= 图1：色度图与投影关系 =================
figure('Name', '网格最佳RGB点与指定目标色投影关系', 'Color', 'w');
hold on;
grid on;
box on;

plot(triangleX, triangleY, 'k-', 'LineWidth', 1.5);
scatter(lut.Target_x, lut.Target_y, 18, [0.75 0.75 0.75], 'filled', 'DisplayName', '所有网格目标点');
scatter(lut.Best_RGBPoint_x, lut.Best_RGBPoint_y, 12, [0.20 0.55 0.95], 'filled', 'DisplayName', '所有网格点的最佳RGB混色点');
scatter(res.Target_x, res.Target_y, 24, [0.10 0.65 0.20], 'filled', 'DisplayName', '输入目标色点');

% 指定目标色相关点
T = [sel.Target_x, sel.Target_y];
selectedGridTarget = [sel.SelectedGridTarget_x, sel.SelectedGridTarget_y];
storedRgb = [sel.Grid_StoredBestRgb_x, sel.Grid_StoredBestRgb_y];
projectedRgb = [sel.Grid_ProjectedRgb_x, sel.Grid_ProjectedRgb_y];
exactRgb = [sel.Exact_RGBPoint_x, sel.Exact_RGBPoint_y];

scatter(Wxy(1), Wxy(2), 80, 'm', 'filled', 'DisplayName', '单白光点');
scatter(T(1), T(2), 90, 'r', 'filled', 'DisplayName', '指定目标色');
scatter(selectedGridTarget(1), selectedGridTarget(2), 90, 'y', 'filled', 'MarkerEdgeColor', 'k', 'DisplayName', '采用的网格点');
scatter(storedRgb(1), storedRgb(2), 90, 'c', 'filled', 'MarkerEdgeColor', 'k', 'DisplayName', '网格点存储的最佳RGB点');
scatter(projectedRgb(1), projectedRgb(2), 100, 's', 'filled', 'MarkerFaceColor', [1.0 0.45 0.0], 'MarkerEdgeColor', 'k', 'DisplayName', '投影后的RGB点');
scatter(exactRgb(1), exactRgb(2), 100, 'p', 'filled', 'MarkerFaceColor', [0.55 0.0 0.75], 'MarkerEdgeColor', 'k', 'DisplayName', '精确法最佳RGB点');

% 射线与垂线
plot([Wxy(1), projectedRgb(1)], [Wxy(2), projectedRgb(2)], 'r--', 'LineWidth', 1.3, 'DisplayName', '当前目标色射线');
plot([storedRgb(1), projectedRgb(1)], [storedRgb(2), projectedRgb(2)], 'b-', 'LineWidth', 1.2, 'DisplayName', '最佳RGB点到射线的垂线');
plot([selectedGridTarget(1), storedRgb(1)], [selectedGridTarget(2), storedRgb(2)], 'c:', 'LineWidth', 1.0, 'DisplayName', '网格点到其最佳RGB点');

text(T(1), T(2), sprintf('  T%d', selectedTargetIndex), 'Color', 'r', 'FontWeight', 'bold');
text(projectedRgb(1), projectedRgb(2), '  投影RGB', 'Color', [0.75 0.25 0.0], 'FontWeight', 'bold');
text(exactRgb(1), exactRgb(2), '  精确RGB', 'Color', [0.45 0.0 0.65], 'FontWeight', 'bold');

xlabel('x in CIE1931');
ylabel('y in CIE1931');
title(sprintf('目标%d：网格最佳RGB点投影到当前目标色射线', selectedTargetIndex));
legend('Location', 'bestoutside');
axis equal;

infoText = sprintf(['精确Ra = %.4f\n网格Ra = %.4f\nRa差值 = %.4f\nRGB点DeltaUV = %.5f\n采用第%d个候选网格点'], ...
    sel.Exact_Ra, sel.Grid_Ra, sel.Ra_Diff, sel.ExactVsProjectedRgb_DeltaUV, sel.SelectedGridRank);
annotation('textbox', [0.14 0.68 0.24 0.18], 'String', infoText, 'FitBoxToText', 'on', ...
    'BackgroundColor', 'w', 'EdgeColor', [0.2 0.2 0.2]);

if savePng
    saveas(gcf, fullfile(outputDir, sprintf('plot_grid_projection_target_%02d.png', selectedTargetIndex)));
end

%% ================= 图2：Ra原值和差值 =================
figure('Name', 'Ra原值与差值对比', 'Color', 'w');
tiledlayout(2,1);

nexttile;
hold on;
grid on;
plot(res.Index, res.Exact_Ra, 'o-', 'LineWidth', 1.2, 'DisplayName', '精确法Ra');
plot(res.Index, res.Grid_Ra, 's-', 'LineWidth', 1.2, 'DisplayName', '网格法Ra');
xline(selectedTargetIndex, 'r--', 'LineWidth', 1.2, 'DisplayName', '指定目标色');
xlabel('目标颜色序号');
ylabel('Ra');
title('精确法 Ra 与网格法 Ra');
legend('Location', 'best');

nexttile;
hold on;
grid on;
bar(res.Index, res.Ra_Diff);
xline(selectedTargetIndex, 'r--', 'LineWidth', 1.2);
xlabel('目标颜色序号');
ylabel('Ra差值');
title('Ra差值 = 精确法Ra - 网格法Ra');

if savePng
    saveas(gcf, fullfile(outputDir, 'plot_ra_exact_vs_grid_diff.png'));
end

fprintf('\n绘图完成。\n');
fprintf('指定目标序号 = %d\n', selectedTargetIndex);
fprintf('精确法Ra = %.6f，网格法Ra = %.6f，Ra差值 = %.6f\n', sel.Exact_Ra, sel.Grid_Ra, sel.Ra_Diff);
fprintf('图片输出目录：%s\n', outputDir);

%% ================= 局部函数 =================
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
