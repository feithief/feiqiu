% S01_PLOT_BEST_GRID_3D_VECTOR
% Visualize S01 triangular-grid best points as an interactive 3D vector field.
%
% Input:
%   Same output used by s01_make_lut_xy_grid_final.m:
%   projectRoot/triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv
%
% Meaning:
%   Each grid target point is shown at (target x, target y, best white ratio).
%   Each vector starts from the target point and points to the best RGB point
%   found by S01. Color shows either best white ratio or best Ra.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S01 3D vector visualization: target grid -> best RGB point\n');
fprintf('============================================================\n\n');

%% User options
colorMode = 'ratio';        % 'ratio' or 'ra'
vectorScale = 0.85;         % 0: hide arrow length, 1: true xy length, >1: magnify
surfaceGridN = 90;          % interpolation resolution for trend surface
showPointLabels = false;    % true: draw sparse text labels, slower and visually denser
labelStep = 70;             % label every Nth point if showPointLabels=true
enableDataCursorAtStart = false; % false: mouse drag rotates the 3D view by default

%% Locate S01 result
% Keep the same project-root rule as s01_make_lut_xy_grid_final.m:
% search upward for guangpu_case_input, then use projectRoot/triangle_grid_lut_output.
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
generatorFile = fullfile(projectRoot, 'guangpu_case_input', 's01_make_lut_xy_grid_final.m');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
csvFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');

if ~exist(generatorFile, 'file')
    error('Cannot find matching generator script: %s', generatorFile);
end

if ~exist(csvFile, 'file')
    error('S01 CSV not found: %s\nPlease run s01_make_lut_xy_grid_final.m first.', csvFile);
end

T = readtable(csvFile, 'TextType', 'string');
requiredVars = {'GridI','GridJ','GridK', ...
    'Target_u1976','Target_v1976','Target_x','Target_y', ...
    'Best_RGBPoint_u1976','Best_RGBPoint_v1976','Best_RGBPoint_x','Best_RGBPoint_y', ...
    'Best_SingleWhiteRatio','Best_RGBWhiteRatio','Best_Ra','Best_Rmin_8','Best_RayT','Valid'};
assert_required_variables(T, requiredVars);

if islogical(T.Valid)
    validMask = T.Valid;
else
    validMask = T.Valid ~= 0;
end

T = T(validMask, :);
if isempty(T)
    error('S01 CSV contains no valid rows: %s', csvFile);
end

x0 = T.Target_x;
y0 = T.Target_y;
w0 = T.Best_SingleWhiteRatio;

bestX = T.Best_RGBPoint_x;
bestY = T.Best_RGBPoint_y;
dx = (bestX - x0) * vectorScale;
dy = (bestY - y0) * vectorScale;
dw = zeros(size(dx));

switch lower(colorMode)
    case 'ratio'
        colorValue = w0;
        colorName = 'Best Single White Ratio';
    case 'ra'
        colorValue = T.Best_Ra;
        colorName = 'Best Ra';
    otherwise
        error('Unsupported colorMode: %s. Use ratio or ra.', colorMode);
end

%% Build trend surface on xy plane
xGrid = linspace(min(x0), max(x0), surfaceGridN);
yGrid = linspace(min(y0), max(y0), surfaceGridN);
[Xq, Yq] = meshgrid(xGrid, yGrid);
Wq = griddata(x0, y0, w0, Xq, Yq, 'natural');

%% Figure 1: ratio trend + vector field
fig1 = figure('Name', 'S01 Best White Ratio 3D Vector Field', ...
    'Color', 'w', 'NumberTitle', 'off');
ax1 = axes(fig1);
hold(ax1, 'on');
grid(ax1, 'on');
box(ax1, 'on');

surf(ax1, Xq, Yq, Wq, ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.48, ...
    'FaceColor', 'interp');

scatterObj = scatter3(ax1, x0, y0, w0, 24, colorValue, 'filled', ...
    'MarkerEdgeColor', [0.15 0.15 0.15], ...
    'MarkerEdgeAlpha', 0.25);

quiverObj = quiver3(ax1, x0, y0, w0, dx, dy, dw, 0, ...
    'Color', [0.06 0.18 0.45], ...
    'LineWidth', 0.75, ...
    'MaxHeadSize', 0.65);

if showPointLabels
    labelIdx = 1:labelStep:height(T);
    for k = labelIdx
        text(ax1, x0(k), y0(k), w0(k), ...
            sprintf('(%g,%g,%g)', T.GridI(k), T.GridJ(k), T.GridK(k)), ...
            'FontSize', 7, 'Color', [0.1 0.1 0.1]);
    end
end

xlabel(ax1, 'Target x');
ylabel(ax1, 'Target y');
zlabel(ax1, 'Best Single White Ratio');
title(ax1, 'S01 Grid: Best White Ratio Surface and Target-to-Best-RGB Vectors');
cb1 = colorbar(ax1);
cb1.Label.String = colorName;
colormap(ax1, turbo_or_parula());
view(ax1, 42, 30);

setappdata(scatterObj, 'S01GridTable', T);
setappdata(scatterObj, 'S01ZMode', 'ratio');
dcm1 = datacursormode(fig1);
set(dcm1, 'Enable', ternary_on_off(enableDataCursorAtStart), 'UpdateFcn', @s01_data_tip);
rotate3d(fig1, ternary_on_off(~enableDataCursorAtStart));

%% Figure 2: Ra trend, useful for seeing quality distribution
fig2 = figure('Name', 'S01 Best Ra Trend Surface', ...
    'Color', 'w', 'NumberTitle', 'off');
ax2 = axes(fig2);
hold(ax2, 'on');
grid(ax2, 'on');
box(ax2, 'on');

Raq = griddata(x0, y0, T.Best_Ra, Xq, Yq, 'natural');
surf(ax2, Xq, Yq, Raq, ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.55, ...
    'FaceColor', 'interp');
scatterRaObj = scatter3(ax2, x0, y0, T.Best_Ra, 22, T.Best_Ra, 'filled', ...
    'MarkerEdgeColor', [0.15 0.15 0.15], ...
    'MarkerEdgeAlpha', 0.25);

xlabel(ax2, 'Target x');
ylabel(ax2, 'Target y');
zlabel(ax2, 'Best Ra');
title(ax2, 'S01 Grid: Best Ra Trend Surface');
cb2 = colorbar(ax2);
cb2.Label.String = 'Best Ra';
colormap(ax2, turbo_or_parula());
view(ax2, 42, 30);

setappdata(scatterRaObj, 'S01GridTable', T);
setappdata(scatterRaObj, 'S01ZMode', 'ra');
dcm2 = datacursormode(fig2);
set(dcm2, 'Enable', ternary_on_off(enableDataCursorAtStart), 'UpdateFcn', @s01_data_tip);
rotate3d(fig2, ternary_on_off(~enableDataCursorAtStart));

%% Print summary and export figures
[~, idxMaxRa] = max(T.Best_Ra);
[~, idxMaxRatio] = max(T.Best_SingleWhiteRatio);
[~, idxMinRatio] = min(T.Best_SingleWhiteRatio);

fprintf('Input CSV : %s\n', csvFile);
fprintf('Matched S01 generator: %s\n', generatorFile);
fprintf('Valid grid point count = %d\n', height(T));
fprintf('\nMax Ra point:\n');
print_row_summary(T(idxMaxRa, :));
fprintf('\nMax white-ratio point:\n');
print_row_summary(T(idxMaxRatio, :));
fprintf('\nMin white-ratio point:\n');
print_row_summary(T(idxMinRatio, :));

png1 = fullfile(outputDir, 's01_best_grid_xy_3d_vector_ratio.png');
png2 = fullfile(outputDir, 's01_best_grid_xy_3d_surface_ra.png');
try
    exportgraphics(fig1, png1, 'Resolution', 200);
    exportgraphics(fig2, png2, 'Resolution', 200);
    fprintf('\nExported figure:\n  %s\n  %s\n', png1, png2);
catch
    saveas(fig1, png1);
    saveas(fig2, png2);
    fprintf('\nExported figure by saveas:\n  %s\n  %s\n', png1, png2);
end

fprintf('\nTip: click any scatter point in the figure to inspect its exact grid and best-point values.\n');
fprintf('Tip: 3D rotation is enabled by default. Use the figure toolbar Data Cursor button when you want to click points.\n');

%% Local functions
function rootDir = find_project_root(startDir, markerName)
    rootDir = startDir;
    while true
        if exist(fullfile(rootDir, markerName), 'dir') || exist(fullfile(rootDir, markerName), 'file')
            return;
        end

        parentDir = fileparts(rootDir);
        if isempty(parentDir) || strcmp(parentDir, rootDir)
            error('Cannot find project root containing %s from %s', markerName, startDir);
        end
        rootDir = parentDir;
    end
end

function assert_required_variables(T, names)
    missing = setdiff(names, T.Properties.VariableNames);
    if ~isempty(missing)
        error('S01 CSV missing required columns: %s', strjoin(missing, ', '));
    end
end

function s = ternary_on_off(tf)
    if tf
        s = 'on';
    else
        s = 'off';
    end
end

function cmap = turbo_or_parula()
    try
        cmap = turbo(256);
    catch
        cmap = parula(256);
    end
end

function txt = s01_data_tip(~, eventObj)
    pos = get(eventObj, 'Position');
    target = get(eventObj, 'Target');
    T = getappdata(target, 'S01GridTable');
    zMode = getappdata(target, 'S01ZMode');

    if isempty(T)
        txt = {sprintf('x = %.6f', pos(1)), sprintf('y = %.6f', pos(2)), sprintf('z = %.6f', pos(3))};
        return;
    end

    if strcmp(zMode, 'ra')
        dist = (T.Target_x - pos(1)).^2 + (T.Target_y - pos(2)).^2 + ...
            ((T.Best_Ra - pos(3)) ./ max(1, range_or_one(T.Best_Ra))).^2;
    else
        dist = (T.Target_x - pos(1)).^2 + (T.Target_y - pos(2)).^2 + ...
            (T.Best_SingleWhiteRatio - pos(3)).^2;
    end

    [~, idx] = min(dist);
    r = T(idx, :);

    txt = { ...
        sprintf('Grid = (%g, %g, %g)', r.GridI, r.GridJ, r.GridK), ...
        sprintf('Target xy = (%.6f, %.6f)', r.Target_x, r.Target_y), ...
        sprintf('Target u''v'' = (%.6f, %.6f)', r.Target_u1976, r.Target_v1976), ...
        sprintf('Best RGB xy = (%.6f, %.6f)', r.Best_RGBPoint_x, r.Best_RGBPoint_y), ...
        sprintf('Best RGB u''v'' = (%.6f, %.6f)', r.Best_RGBPoint_u1976, r.Best_RGBPoint_v1976), ...
        sprintf('SingleWhiteRatio = %.8f', r.Best_SingleWhiteRatio), ...
        sprintf('RGBWhiteRatio = %.8f', r.Best_RGBWhiteRatio), ...
        sprintf('Best Ra = %.6f', r.Best_Ra), ...
        sprintf('Best Rmin8 = %.6f', r.Best_Rmin_8), ...
        sprintf('RayT = %.8f', r.Best_RayT) ...
        };
end

function y = range_or_one(x)
    y = max(x) - min(x);
    if y <= 0
        y = 1;
    end
end

function print_row_summary(r)
    fprintf('  Grid=(%g,%g,%g)\n', r.GridI, r.GridJ, r.GridK);
    fprintf('  Target xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        r.Target_x, r.Target_y, r.Target_u1976, r.Target_v1976);
    fprintf('  Best RGB xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        r.Best_RGBPoint_x, r.Best_RGBPoint_y, r.Best_RGBPoint_u1976, r.Best_RGBPoint_v1976);
    fprintf('  Ratio: Single=%.8f, RGB=%.8f\n', ...
        r.Best_SingleWhiteRatio, r.Best_RGBWhiteRatio);
    fprintf('  Ra=%.6f, Rmin8=%.6f, RayT=%.8f\n', ...
        r.Best_Ra, r.Best_Rmin_8, r.Best_RayT);
end
