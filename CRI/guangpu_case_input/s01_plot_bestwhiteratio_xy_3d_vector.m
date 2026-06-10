% S01_PLOT_BESTWHITERATIO_XY_3D_VECTOR
% Visualize S01 triangular-grid BestWhiteRatio as an interactive xy 3D distribution.
%
% Input:
%   projectRoot/triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv
%
% Meaning:
%   Each valid grid point is shown at (target x, target y, BestWhiteRatio).
%   Color and vertical axis both show BestWhiteRatio.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S01 xy 3D distribution visualization: BestWhiteRatio\n');
fprintf('============================================================\n\n');

%% User options
surfaceGridN = 90;               % interpolation resolution for trend surface
showVectorField = false;         % true: also draw Target xy -> Best RGBPoint xy arrows
vectorScale = 0.85;              % only used when showVectorField=true
showPointLabels = false;         % true: draw sparse text labels
labelStep = 70;                  % label every Nth point if showPointLabels=true
enableDataCursorAtStart = false; % false: mouse drag rotates the 3D view by default

%% Locate S01 result
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
z0 = T.Best_SingleWhiteRatio;

bestX = T.Best_RGBPoint_x;
bestY = T.Best_RGBPoint_y;
dx = (bestX - x0) * vectorScale;
dy = (bestY - y0) * vectorScale;
dz = zeros(size(dx));

%% Build BestWhiteRatio trend surface on xy plane
xGrid = linspace(min(x0), max(x0), surfaceGridN);
yGrid = linspace(min(y0), max(y0), surfaceGridN);
[Xq, Yq] = meshgrid(xGrid, yGrid);
Zq = griddata(x0, y0, z0, Xq, Yq, 'natural');

%% Figure: BestWhiteRatio 3D distribution
fig = figure('Name', 'S01 BestWhiteRatio xy 3D Distribution', ...
    'Color', 'w', 'NumberTitle', 'off');
ax = axes(fig);
hold(ax, 'on');
grid(ax, 'on');
box(ax, 'on');

surfObj = surf(ax, Xq, Yq, Zq, ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.42, ...
    'FaceColor', 'interp');

scatterObj = scatter3(ax, x0, y0, z0, 32, z0, 'filled', ...
    'MarkerEdgeColor', [0.15 0.15 0.15], ...
    'MarkerEdgeAlpha', 0.25);

if showVectorField
    quiver3(ax, x0, y0, z0, dx, dy, dz, 0, ...
        'Color', [0.06 0.18 0.45], ...
        'LineWidth', 0.75, ...
        'MaxHeadSize', 0.65);
end

if showPointLabels
    labelIdx = 1:labelStep:height(T);
    for k = labelIdx
        text(ax, x0(k), y0(k), z0(k), ...
            sprintf('(%g,%g,%g)', T.GridI(k), T.GridJ(k), T.GridK(k)), ...
            'FontSize', 7, 'Color', [0.1 0.1 0.1]);
    end
end

xlabel(ax, 'Target x');
ylabel(ax, 'Target y');
zlabel(ax, 'BestWhiteRatio');
title(ax, 'S01 Grid: BestWhiteRatio 3D Distribution');
cb = colorbar(ax);
cb.Label.String = 'BestWhiteRatio';
colormap(ax, turbo_or_parula());
view(ax, 42, 30);

setappdata(surfObj, 'S01GridTable', T);
setappdata(scatterObj, 'S01GridTable', T);
dcm = datacursormode(fig);
set(dcm, 'Enable', ternary_on_off(enableDataCursorAtStart), 'UpdateFcn', @s01_data_tip);
rotate3d(fig, ternary_on_off(~enableDataCursorAtStart));

%% Print summary and export figure
[~, idxMaxRatio] = max(T.Best_SingleWhiteRatio);
[~, idxMinRatio] = min(T.Best_SingleWhiteRatio);

fprintf('Input CSV : %s\n', csvFile);
fprintf('Matched S01 generator: %s\n', generatorFile);
fprintf('Valid grid point count = %d\n', height(T));
fprintf('\nMax BestWhiteRatio point:\n');
print_row_summary(T(idxMaxRatio, :));
fprintf('\nMin BestWhiteRatio point:\n');
print_row_summary(T(idxMinRatio, :));

pngFile = fullfile(outputDir, 's01_bestwhiteratio_xy_3d_distribution.png');
try
    exportgraphics(fig, pngFile, 'Resolution', 200);
    fprintf('\nExported figure:\n  %s\n', pngFile);
catch
    saveas(fig, pngFile);
    fprintf('\nExported figure by saveas:\n  %s\n', pngFile);
end

fprintf('\nTip: click any scatter point in the figure to inspect exact values.\n');
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

    if isempty(T)
        txt = {sprintf('x = %.6f', pos(1)), sprintf('y = %.6f', pos(2)), sprintf('BestWhiteRatio = %.8f', pos(3))};
        return;
    end

    dist = (T.Target_x - pos(1)).^2 + (T.Target_y - pos(2)).^2 + ...
        (T.Best_SingleWhiteRatio - pos(3)).^2;
    [~, idx] = min(dist);
    r = T(idx, :);

    txt = { ...
        sprintf('Grid = (%g, %g, %g)', r.GridI, r.GridJ, r.GridK), ...
        sprintf('Target xy = (%.6f, %.6f)', r.Target_x, r.Target_y), ...
        sprintf('Target u''v'' = (%.6f, %.6f)', r.Target_u1976, r.Target_v1976), ...
        sprintf('Best RGB xy = (%.6f, %.6f)', r.Best_RGBPoint_x, r.Best_RGBPoint_y), ...
        sprintf('Best RGB u''v'' = (%.6f, %.6f)', r.Best_RGBPoint_u1976, r.Best_RGBPoint_v1976), ...
        sprintf('BestWhiteRatio = %.8f', r.Best_SingleWhiteRatio), ...
        sprintf('RGBWhiteRatio = %.8f', r.Best_RGBWhiteRatio), ...
        sprintf('Best Ra = %.6f', r.Best_Ra), ...
        sprintf('Best Rmin8 = %.6f', r.Best_Rmin_8), ...
        sprintf('RayT = %.8f', r.Best_RayT) ...
        };
end

function print_row_summary(r)
    fprintf('  Grid=(%g,%g,%g)\n', r.GridI, r.GridJ, r.GridK);
    fprintf('  Target xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        r.Target_x, r.Target_y, r.Target_u1976, r.Target_v1976);
    fprintf('  Best RGB xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        r.Best_RGBPoint_x, r.Best_RGBPoint_y, r.Best_RGBPoint_u1976, r.Best_RGBPoint_v1976);
    fprintf('  BestWhiteRatio=%.8f, RGBWhiteRatio=%.8f\n', ...
        r.Best_SingleWhiteRatio, r.Best_RGBWhiteRatio);
    fprintf('  Ra=%.6f, Rmin8=%.6f, RayT=%.8f\n', ...
        r.Best_Ra, r.Best_Rmin_8, r.Best_RayT);
end
