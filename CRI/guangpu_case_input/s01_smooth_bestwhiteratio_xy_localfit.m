% S01_SMOOTH_BESTWHITERATIO_XY_LOCALFIT
% MATLAB-only experiment for a differentiable BestWhiteRatio surface.
%
% Input:
%   projectRoot/triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv
%
% Purpose:
%   The firmware-side finite-neighbor local plane model can show crease lines
%   because the active neighbor set changes. This script keeps S01 data
%   unchanged and tests a smooth alternative:
%
%       fixed-bandwidth Gaussian weights + moving local quadratic regression
%
%   Since every grid point contributes with a continuous Gaussian weight, the
%   fitted surface is smooth unless the source data itself has a true active
%   constraint transition.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S01 smooth BestWhiteRatio xy localfit experiment\n');
fprintf('Model: fixed Gaussian weights + local quadratic regression\n');
fprintf('============================================================\n\n');

%% User options
surfaceGridN = 90;              % Smooth display grid resolution
bandwidthGridStepFactor = 3.0;  % Larger = smoother, smaller = closer to raw data
ridgeLambda = 1e-8;             % Stabilizes the local normal equation
compareRawSurface = true;       % Plot raw natural-neighbor interpolation too
enableDataCursorAtStart = false;

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

x0 = T.Target_x(:);
y0 = T.Target_y(:);
z0 = min(max(T.Best_SingleWhiteRatio(:), 0), 1);

gridStep = estimate_triangular_grid_step(x0, y0);
bandwidth = bandwidthGridStepFactor * gridStep;

fprintf('Input CSV              : %s\n', csvFile);
fprintf('Valid grid point count : %d\n', height(T));
fprintf('Estimated grid step    : %.10f xy\n', gridStep);
fprintf('Gaussian bandwidth     : %.10f xy = %.3f * grid step\n', bandwidth, bandwidthGridStepFactor);
fprintf('Local model            : z = c0 + c1*dx + c2*dy + c3*dx^2 + c4*dx*dy + c5*dy^2\n\n');

%% Build query grid inside the triangular target domain
xGrid = linspace(min(x0), max(x0), surfaceGridN);
yGrid = linspace(min(y0), max(y0), surfaceGridN);
[Xq, Yq] = meshgrid(xGrid, yGrid);
insideMask = points_inside_convex_domain(Xq, Yq, x0, y0);

fprintf('Fitting smooth surface on %d x %d query grid...\n', surfaceGridN, surfaceGridN);
Zsmooth = moving_local_quadratic_gaussian(x0, y0, z0, Xq, Yq, insideMask, bandwidth, ridgeLambda);
Zsmooth = min(max(Zsmooth, 0), 1);

if compareRawSurface
    Zraw = griddata(x0, y0, z0, Xq, Yq, 'natural');
    Zraw(~insideMask) = NaN;
else
    Zraw = NaN(size(Zsmooth));
end

%% Leave-one-out diagnostic at original grid points
fprintf('Running leave-one-out diagnostic on original grid points...\n');
zLoo = moving_local_quadratic_gaussian_loo(x0, y0, z0, bandwidth, ridgeLambda);
err = zLoo - z0;
absErr = abs(err(isfinite(err)));
fprintf('LOO abs error: mean=%.8f, p95=%.8f, max=%.8f\n', ...
    mean(absErr), percentile_no_toolbox(absErr, 95), max(absErr));

%% Figure 1: smooth differentiable surface
fig1 = figure('Name', 'S01 Smooth BestWhiteRatio Surface', ...
    'Color', 'w', 'NumberTitle', 'off');
ax1 = axes(fig1);
hold(ax1, 'on');
grid(ax1, 'on');
box(ax1, 'on');

surfObj = surf(ax1, Xq, Yq, Zsmooth, ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.62, ...
    'FaceColor', 'interp');
set(surfObj, 'HitTest', 'off', 'PickableParts', 'none');

scatterObj = scatter3(ax1, x0, y0, z0, 22, z0, 'filled', ...
    'MarkerEdgeColor', [0.12 0.12 0.12], ...
    'MarkerEdgeAlpha', 0.20);
uistack(scatterObj, 'top');

xlabel(ax1, 'Target x');
ylabel(ax1, 'Target y');
zlabel(ax1, 'BestWhiteRatio');
title(ax1, sprintf('Smooth BestWhiteRatio Surface, Gaussian bandwidth = %.2f grid steps', bandwidthGridStepFactor));
cb1 = colorbar(ax1);
cb1.Label.String = 'BestWhiteRatio';
colormap(ax1, turbo_or_parula());
view(ax1, 42, 30);

configure_persistent_datatips(scatterObj, T, z0);
dcm1 = datacursormode(fig1);
set(dcm1, 'Enable', ternary_on_off(enableDataCursorAtStart));
rotate3d(fig1, ternary_on_off(~enableDataCursorAtStart));

%% Figure 2: raw vs smooth residual map
fig2 = figure('Name', 'S01 Raw minus Smooth BestWhiteRatio', ...
    'Color', 'w', 'NumberTitle', 'off');
ax2 = axes(fig2);
hold(ax2, 'on');
grid(ax2, 'on');
box(ax2, 'on');

if compareRawSurface
    residual = Zraw - Zsmooth;
    surf(ax2, Xq, Yq, residual, ...
        'EdgeColor', 'none', ...
        'FaceAlpha', 0.72, ...
        'FaceColor', 'interp');
    zlabel(ax2, 'Raw - Smooth');
    title(ax2, 'Raw Natural-Neighbor Surface minus Smooth Local-Quadratic Surface');
else
    scatter3(ax2, x0, y0, err, 22, err, 'filled');
    zlabel(ax2, 'Leave-one-out error');
    title(ax2, 'Leave-One-Out Error at Original Grid Points');
end
xlabel(ax2, 'Target x');
ylabel(ax2, 'Target y');
cb2 = colorbar(ax2);
cb2.Label.String = 'Difference';
colormap(ax2, turbo_or_parula());
view(ax2, 42, 30);
rotate3d(fig2, 'on');

%% Figure 3: gradient magnitude of smooth surface
[dZdy, dZdx] = gradient(Zsmooth, mean(diff(yGrid)), mean(diff(xGrid)));
gradMag = sqrt(dZdx.^2 + dZdy.^2);
gradMag(~insideMask) = NaN;

fig3 = figure('Name', 'S01 Smooth BestWhiteRatio Gradient Magnitude', ...
    'Color', 'w', 'NumberTitle', 'off');
ax3 = axes(fig3);
hold(ax3, 'on');
grid(ax3, 'on');
box(ax3, 'on');
surf(ax3, Xq, Yq, gradMag, ...
    'EdgeColor', 'none', ...
    'FaceAlpha', 0.72, ...
    'FaceColor', 'interp');
xlabel(ax3, 'Target x');
ylabel(ax3, 'Target y');
zlabel(ax3, '|grad BestWhiteRatio|');
title(ax3, 'Gradient Magnitude of Smooth BestWhiteRatio Surface');
cb3 = colorbar(ax3);
cb3.Label.String = '|grad BestWhiteRatio|';
colormap(ax3, turbo_or_parula());
view(ax3, 42, 30);
rotate3d(fig3, 'on');

%% Export figures and diagnostic table
smoothCsv = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_localfit_loo.csv');
outTable = T(:, {'GridI','GridJ','GridK','Target_x','Target_y','Target_u1976','Target_v1976','Best_SingleWhiteRatio'});
outTable.Smooth_LOO_BestWhiteRatio = zLoo;
outTable.Smooth_LOO_Error = err;
writetable(outTable, smoothCsv);

png1 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_surface.png');
png2 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_raw_minus_smooth.png');
png3 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_gradient.png');
figFile1 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_surface.fig');
figFile2 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_raw_minus_smooth.fig');
figFile3 = fullfile(outputDir, 's01_smooth_bestwhiteratio_xy_gradient.fig');

try
    exportgraphics(fig1, png1, 'Resolution', 200);
    exportgraphics(fig2, png2, 'Resolution', 200);
    exportgraphics(fig3, png3, 'Resolution', 200);
catch
    saveas(fig1, png1);
    saveas(fig2, png2);
    saveas(fig3, png3);
end
savefig(fig1, figFile1);
savefig(fig2, figFile2);
savefig(fig3, figFile3);

fprintf('\nExported:\n');
fprintf('  %s\n', smoothCsv);
fprintf('  %s\n  %s\n', png1, figFile1);
fprintf('  %s\n  %s\n', png2, figFile2);
fprintf('  %s\n  %s\n', png3, figFile3);
fprintf('\nTip: adjust bandwidthGridStepFactor. Larger values suppress creases but may reduce local accuracy.\n');

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

function gridStep = estimate_triangular_grid_step(x, y)
    n = numel(x);
    nearestDist = zeros(n, 1);
    for i = 1:n
        d2 = (x - x(i)).^2 + (y - y(i)).^2;
        d2(i) = inf;
        nearestDist(i) = sqrt(min(d2));
    end
    gridStep = median(nearestDist(isfinite(nearestDist) & nearestDist > 0));
    if ~isfinite(gridStep) || gridStep <= 0
        gridStep = max(max(x) - min(x), max(y) - min(y)) / 50;
    end
end

function insideMask = points_inside_convex_domain(Xq, Yq, x, y)
    hullIdx = convhull(x, y);
    insideMask = inpolygon(Xq, Yq, x(hullIdx), y(hullIdx));
end

function Zq = moving_local_quadratic_gaussian(x, y, z, Xq, Yq, insideMask, h, ridgeLambda)
    Zq = NaN(size(Xq));
    queryIndex = find(insideMask);
    for k = 1:numel(queryIndex)
        idx = queryIndex(k);
        Zq(idx) = moving_local_quadratic_one(x, y, z, Xq(idx), Yq(idx), h, ridgeLambda, []);
    end
end

function zPred = moving_local_quadratic_gaussian_loo(x, y, z, h, ridgeLambda)
    n = numel(x);
    zPred = NaN(n, 1);
    for i = 1:n
        zPred(i) = moving_local_quadratic_one(x, y, z, x(i), y(i), h, ridgeLambda, i);
    end
end

function zPred = moving_local_quadratic_one(x, y, z, xq, yq, h, ridgeLambda, excludeIndex)
    dx = (x - xq) ./ h;
    dy = (y - yq) ./ h;
    d2 = dx.^2 + dy.^2;
    w = exp(-0.5 .* d2);

    if ~isempty(excludeIndex)
        w(excludeIndex) = 0;
    end

    Phi = [ones(size(dx)), dx, dy, dx.^2, dx .* dy, dy.^2];
    sqrtW = sqrt(w);
    PhiW = Phi .* sqrtW;
    zW = z .* sqrtW;

    A = PhiW.' * PhiW;
    b = PhiW.' * zW;

    scaleA = trace(A) / size(A, 1);
    if ~isfinite(scaleA) || scaleA <= 0
        zPred = weighted_average_fallback(z, w);
        return;
    end

    A = A + ridgeLambda * scaleA * eye(size(A));

    if rcond(A) < 1e-12
        zPred = weighted_average_fallback(z, w);
        return;
    end

    coef = A \ b;
    zPred = coef(1);
end

function zPred = weighted_average_fallback(z, w)
    s = sum(w);
    if s <= 0 || ~isfinite(s)
        zPred = NaN;
    else
        zPred = sum(w .* z) ./ s;
    end
end

function p = percentile_no_toolbox(x, pct)
    x = sort(x(:));
    x = x(isfinite(x));
    if isempty(x)
        p = NaN;
        return;
    end
    pos = 1 + (numel(x) - 1) * pct / 100;
    lo = floor(pos);
    hi = ceil(pos);
    if lo == hi
        p = x(lo);
    else
        p = x(lo) + (x(hi) - x(lo)) * (pos - lo);
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

function configure_persistent_datatips(scatterObj, T, z0)
    try
        scatterObj.DataTipTemplate.DataTipRows(1).Label = 'Target x';
        scatterObj.DataTipTemplate.DataTipRows(2).Label = 'Target y';
        scatterObj.DataTipTemplate.DataTipRows(3).Label = 'BestWhiteRatio';

        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('GridI', T.GridI);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('GridJ', T.GridJ);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('GridK', T.GridK);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Target u''', T.Target_u1976);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Target v''', T.Target_v1976);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Best RGB x', T.Best_RGBPoint_x);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Best RGB y', T.Best_RGBPoint_y);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('RGBWhiteRatio', T.Best_RGBWhiteRatio);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Best Ra', T.Best_Ra);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Best Rmin8', T.Best_Rmin_8);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('RayT', T.Best_RayT);
        scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow('Clamped ratio', z0);
    catch
        % Older MATLAB versions may not support DataTipTemplate.
    end
end
