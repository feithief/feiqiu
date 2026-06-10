% S03_PLOT_MODEL_GEOMETRY_RATIOK
% 中文说明：
% 图示某一个目标色的几何关系：
%   精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB
%
% 使用前：
%   先运行 s02_run_model_compare_ratioK.m

clear;
clc;

%% ================= 用户可配置参数 =================
selectedTargetIndex = 100;
viewMode = 'zoom';   % 'zoom'：围绕关键点缩放；'full'：显示完整RGB三角色域

%% ================= 定位目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');

resultFile = fullfile(outputDir, 'model_compare_results.csv');
outputPng = fullfile(outputDir, sprintf('model_geometry_ratioK_target_%02d.png', selectedTargetIndex));

if ~exist(resultFile, 'file')
    error('没有找到结果文件：%s\n请先运行 s02_run_model_compare_ratioK.m。', resultFile);
end

T = readtable(resultFile, 'VariableNamingRule', 'preserve');

if selectedTargetIndex < 1 || selectedTargetIndex > height(T)
    error('selectedTargetIndex=%d 超出范围。当前结果共有 %d 个目标点。', selectedTargetIndex, height(T));
end

row = T(selectedTargetIndex, :);

addpath(inputDir);
caseCfg = guangpu_case_config();

singleWhiteXY = caseCfg.single_white_xyY(1:2);
rgbTriangleXY = [caseCfg.channel_xyY.R(1:2); ...
                 caseCfg.channel_xyY.G(1:2); ...
                 caseCfg.channel_xyY.B(1:2)];
targetXY = [row.Target_x, row.Target_y];

%% ================= 绘图 =================
fig = figure('Name', sprintf('模型几何图示 - 目标点 %d', selectedTargetIndex), ...
    'NumberTitle', 'off', ...
    'Color', 'w', ...
    'Position', [60, 60, 1500, 820]);

ax = axes('Parent', fig, 'Position', [0.055, 0.11, 0.66, 0.80]);
hold(ax, 'on');
grid(ax, 'on');
box(ax, 'on');
axis(ax, 'equal');

% RGB色域
patch(ax, rgbTriangleXY(:,1), rgbTriangleXY(:,2), [0.92, 0.96, 1.00], ...
    'EdgeColor', [0.25, 0.35, 0.85], ...
    'LineWidth', 1.8, ...
    'FaceAlpha', 0.45, ...
    'DisplayName', 'RGB色域');

% W->T射线
plot(ax, [singleWhiteXY(1), targetXY(1)], [singleWhiteXY(2), targetXY(2)], '--', ...
    'Color', [0.25, 0.25, 0.25], 'LineWidth', 1.4, 'DisplayName', 'W→T射线');

% 基础点
plot_point(ax, singleWhiteXY, 'o', [0,0,0], [1,0.9,0.1], '单白光W');
plot_point(ax, targetXY, 'p', [0,0,0], [1,0.65,0.1], '目标点T');

% 各模型点
plot_point(ax, [row.Exact_RGBPoint_x,row.Exact_RGBPoint_y], 'o', [0,0.35,0.75], [0,0.45,0.85], '精确法');
plot_point(ax, [row.Proj1_RGBPoint_x,row.Proj1_RGBPoint_y], 'd', [0.85,0.33,0.10], [0.85,0.33,0.10], '投影1点');
plot_point(ax, [row.ProjK_RGBPoint_x,row.ProjK_RGBPoint_y], 'v', [0.20,0.55,0.10], [0.20,0.65,0.20], '投影K点');
plot_point(ax, [row.Ratio1_RGBPoint_x,row.Ratio1_RGBPoint_y], 'h', [0.50,0.20,0.60], [0.65,0.35,0.75], '同比例1点');
plot_point(ax, [row.RatioK_RGBPoint_x,row.RatioK_RGBPoint_y], '>', [0.10,0.55,0.65], [0.30,0.85,0.95], '同比例K点');
plot_point(ax, [row.PureRGB_RGBPoint_x,row.PureRGB_RGBPoint_y], 'x', [0.65,0.08,0.18], [0.65,0.08,0.18], '纯RGB');

% 参考网格点
plot_point(ax, [row.Proj1_SelectedGridTarget_x,row.Proj1_SelectedGridTarget_y], 's', [0.85,0.33,0.10], 'none', '投影1点参考网格');
plot_point(ax, [row.ProjK_SelectedGridTarget_x,row.ProjK_SelectedGridTarget_y], '^', [0.20,0.55,0.10], 'none', '投影K点参考网格');
plot_point(ax, [row.Ratio1_SelectedGridTarget_x,row.Ratio1_SelectedGridTarget_y], 'h', [0.50,0.20,0.60], 'none', '同比例1点参考网格');
plot_point(ax, [row.RatioK_SelectedGridTarget_x,row.RatioK_SelectedGridTarget_y], '>', [0.10,0.55,0.65], 'none', '同比例K点参考网格');

% 标注
label_point(ax, singleWhiteXY, 'W');
label_point(ax, targetXY, 'T');
label_point(ax, [row.Exact_RGBPoint_x,row.Exact_RGBPoint_y], 'Exact');
label_point(ax, [row.Proj1_RGBPoint_x,row.Proj1_RGBPoint_y], 'P1');
label_point(ax, [row.ProjK_RGBPoint_x,row.ProjK_RGBPoint_y], 'PK');
label_point(ax, [row.Ratio1_RGBPoint_x,row.Ratio1_RGBPoint_y], 'R1');
label_point(ax, [row.RatioK_RGBPoint_x,row.RatioK_RGBPoint_y], 'RK');
label_point(ax, [row.PureRGB_RGBPoint_x,row.PureRGB_RGBPoint_y], 'Pure');

xlabel(ax, 'CIE 1931 x');
ylabel(ax, 'CIE 1931 y');
title(ax, sprintf('目标点 %d：精确 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB', selectedTargetIndex), ...
    'FontSize', 13, 'FontWeight', 'bold');

importantPoints = [singleWhiteXY; targetXY; ...
    row.Exact_RGBPoint_x,row.Exact_RGBPoint_y; ...
    row.Proj1_RGBPoint_x,row.Proj1_RGBPoint_y; ...
    row.ProjK_RGBPoint_x,row.ProjK_RGBPoint_y; ...
    row.Ratio1_RGBPoint_x,row.Ratio1_RGBPoint_y; ...
    row.RatioK_RGBPoint_x,row.RatioK_RGBPoint_y; ...
    row.PureRGB_RGBPoint_x,row.PureRGB_RGBPoint_y];

importantPoints = importantPoints(all(isfinite(importantPoints),2),:);

if strcmpi(viewMode,'full')
    axisPoints = [rgbTriangleXY; importantPoints];
else
    axisPoints = importantPoints;
end

pad = 0.035;
xlim(ax, [min(axisPoints(:,1))-pad, max(axisPoints(:,1))+pad]);
ylim(ax, [min(axisPoints(:,2))-pad, max(axisPoints(:,2))+pad]);

legend(ax, 'Location', 'southoutside', 'NumColumns', 4, 'Box', 'off');

% 右侧信息面板
panel = uipanel('Parent', fig, ...
    'Title', '当前目标点计算结果', ...
    'FontSize', 11, ...
    'FontWeight', 'bold', ...
    'BackgroundColor', [0.98,0.98,0.98], ...
    'Position', [0.75, 0.13, 0.23, 0.75]);

infoText = sprintf([ ...
    '目标点序号：%d\nT=(%.4f, %.4f)\n\n' ...
    '精确法 Ra      = %.4f\n' ...
    '投影1点 Ra     = %.4f\n' ...
    '投影K点 Ra     = %.4f\n' ...
    '同比例1点 Ra   = %.4f\n' ...
    '同比例K点 Ra   = %.4f\n' ...
    '纯RGB Ra       = %.4f\n\n' ...
    '投影K点 K值    = %.0f\n' ...
    '同比例K点 K值  = %.0f\n' ...
    '近似最优来源   = %s\n' ...
    '近似最大Ra     = %.4f'], ...
    selectedTargetIndex, targetXY(1), targetXY(2), ...
    row.Exact_Ra, row.Proj1_Ra, row.ProjK_Ra, row.Ratio1_Ra, row.RatioK_Ra, row.PureRGB_Ra, ...
    row.ProjK_KUsed, row.RatioK_KUsed, char(string(row.Approx_Max_Source)), row.Approx_Max_Ra);

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
    'BackgroundColor', [1,1,1]);

try
    ax.Toolbar.Visible = 'off';
catch
end

try
    exportapp(fig, outputPng);
catch
    saveas(fig, outputPng);
end

fprintf('\n几何图已保存：\n%s\n', outputPng);

%% ================= Local functions =================
function plot_point(ax, xy, marker, edgeColor, faceColor, displayName)
    if any(~isfinite(xy))
        return;
    end

    if ischar(faceColor) && strcmp(faceColor, 'none')
        plot(ax, xy(1), xy(2), marker, ...
            'Color', edgeColor, 'MarkerSize', 9, 'LineWidth', 1.7, 'DisplayName', displayName);
    else
        plot(ax, xy(1), xy(2), marker, ...
            'Color', edgeColor, 'MarkerFaceColor', faceColor, ...
            'MarkerSize', 9, 'LineWidth', 1.7, 'DisplayName', displayName);
    end
end

function label_point(ax, xy, labelText)
    if any(~isfinite(xy))
        return;
    end
    text(ax, xy(1), xy(2), ['  ' labelText], 'FontSize', 10, 'FontWeight', 'bold');
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
