% S01_MAKE_LUT_24_FINAL
% Build an offline triangular-grid LUT using the same ray-scan logic,
% with fast precomputed CRI evaluation.
%
% Core idea:
% 1. Split the RGB chromaticity triangle into a triangular grid in CIE 1976 u'v'.
% 2. For every target grid point, keep the original ray-scan method:
%       SingleWhite -> Target -> RGB gamut boundary
% 3. Scan every candidate RGB equivalent point on this ray.
% 4. Compute Ra for each mixed spectrum and store the best-Ra RGB point.
% 5. Export CSV results and an optional compact C header LUT.
%
% The online C code can then:
%   input target xy/u'v' -> find nearest triangle-grid point -> read best RGB point
%   -> project it to the current target ray -> mix with single white -> output PWM.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S01 当前版本：24阶基础 u''v'' 三角网格，不启用自适应加密\n');
fprintf('三角网格点数 = (24+1)*(24+2)/2 = 325 点\n');
fprintf('============================================================\n\n');

%% ================= User configurable parameters =================
triangleGridDivCount = 43;      % 24 equal divisions on each RGB triangle edge, gives (24+1)*(24+2)/2 = 325 points
rayScanCount = 1001;            % Candidate RGB points per target ray
exportCHeader = true;           % Export compact C LUT header
cHeaderFileName = 'guangpu_triangle_grid_best_rgb_uv_lut_24.h';
validateFastEvaluation = true;   % Compare fast CRI with original spectrum_metrics on a few samples
validationRaTolerance = 1e-4;    % Allowed Ra difference during validation

% Adaptive refinement settings
adaptiveEnable = false;                     % true：启用自适应加密
adaptiveKeepAllCenters = false;            % true：保留所有小三角形中心点，更准但点数更多
adaptiveWhiteRatioDiffThreshold = 0.025;   % 中心点白光比例与顶点插值差值超过该值则加密
adaptiveRaErrorThreshold = 2.0;             % 中心点Ra与顶点插值差值超过该值则加密
adaptiveWhiteRatioMin = 0.005;             % 中心点白光比例超过该值，且顶点接近0时加密

%% Step 0: locate input files
% This script can be placed in the project root folder or in a subfolder.
% The code searches upward until it finds the guangpu_case_input folder.
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
addpath(inputDir);
caseCfg = guangpu_case_config();

if isfield(caseCfg, 'inputDir') && ~isempty(caseCfg.inputDir)
    spectraDir = caseCfg.inputDir;
else
    spectraDir = inputDir;
end

% Keep the same single-white file behavior as calculate_guangpu_metameric_cri_backup_20260509_before_duv005.m.
singleWhiteFile = fullfile(spectraDir, 'W.CSV');

standardDir = fullfile(projectRoot, 'standard_data');
if ~exist(standardDir, 'dir')
    mkdir(standardDir);
end

outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

%% Step 1: target luminance and channel xyY
targetY_lm = caseCfg.target_xyY(3);
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;

%% Step 2: common wavelength axis
wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;

%% Step 3: load standard data
std = load_standard_data(standardDir, wl);

%% Step 4: read spectra and interpolate to the common axis
raw.R = read_spd(fullfile(spectraDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(spectraDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(spectraDir, 'B.CSV'), '', wl);
raw.W = read_spd(fullfile(spectraDir, 'W.CSV'), '', wl);
raw.SingleWhite = read_spd(singleWhiteFile, '', wl);

%% Step 5: scale each spectrum to its measured photopic Y
spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

%% Step 6: calculate actual spectral xyY for geometry
actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);
actual_xyY.W = spectrum_xyY(spd.W, std, dLambda);

singleNorm = scale_to_photopic_Y(spd.SingleWhite, targetY_lm, std, wl, dLambda);
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
actual_xyY.SingleWhite = singleMetrics.xyY;
singleWhiteXY = actual_xyY.SingleWhite(1:2);

rgbPrimary_xyY = [actual_xyY.R; actual_xyY.G; actual_xyY.B];
rgbTriangleXY = rgbPrimary_xyY(:, 1:2);
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', xyY_to_XYZ(actual_xyY.G).', xyY_to_XYZ(actual_xyY.B).'];

%% Step 6.1: precompute fixed channel integrals for faster CRI evaluation
fastPre = build_fast_cri_precompute(spd, singleNorm, std, dLambda);

%% Step 7: build adaptive triangular target grid in CIE 1976 u'v'
baseGrid = make_triangle_target_grid_uv1976(actual_xyY.R(1:2), actual_xyY.G(1:2), actual_xyY.B(1:2), ...
    targetY_lm, triangleGridDivCount);

fprintf('\nBase uniform-u''v'' triangle grid divisions = %d, point count = %d\n', ...
    triangleGridDivCount, height(baseGrid));
fprintf('Ray scan count per target = %d\n', rayScanCount);

if validateFastEvaluation
    validate_fast_cri_evaluation(baseGrid, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
        spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre, validationRaTolerance);
end

if adaptiveEnable
    fprintf('\nAdaptive refinement enabled: scanning base grid first...\n');

    baseRows = scan_target_grid_to_lut(baseGrid, targetY_lm, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
        spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre);

    centerGrid = make_triangle_cell_center_grid_uv1976(baseGrid, triangleGridDivCount, targetY_lm);

    fprintf('Scanning %d triangle cell centers for refinement decision...\n', height(centerGrid));

    centerRows = scan_target_grid_to_lut(centerGrid, targetY_lm, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
        spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre);

    refineMask = select_adaptive_center_points(baseRows, centerRows, centerGrid, ...
        adaptiveKeepAllCenters, adaptiveWhiteRatioDiffThreshold, adaptiveRaErrorThreshold, adaptiveWhiteRatioMin);

    refineGrid = centerGrid(refineMask, {'GridI','GridJ','GridK','x','y','Y_lm','u1976','v1976'});

    fprintf('Adaptive center points kept = %d / %d\n', height(refineGrid), height(centerGrid));

    targetGrid = unique_target_grid([baseGrid; refineGrid]);
else
    targetGrid = baseGrid;
end

pointCount = height(targetGrid);
fprintf('\nFinal LUT target point count = %d\n', pointCount);

%% Step 8: scan every final target grid point and find best-Ra RGB point
lutRows = scan_target_grid_to_lut(targetGrid, targetY_lm, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre);

%% Step 9: export CSV files
resultCsv = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
compactCsv = fullfile(outputDir, 'guangpu_triangle_grid_best_rgb_point_lut.csv');
writetable(lutRows, resultCsv);

compactRows = lutRows(:, {'GridI','GridJ','GridK', ...
    'Target_u1976','Target_v1976','Best_RGBPoint_u1976','Best_RGBPoint_v1976','Best_Ra','Valid'});
writetable(compactRows, compactCsv);

%% Step 10: export optional C header
if exportCHeader
    export_best_rgb_uv_c_header(fullfile(outputDir, cHeaderFileName), lutRows, triangleGridDivCount);
end

%% Step 11: print summary
validRows = lutRows(lutRows.Valid, :);
fprintf('\nExported full LUT CSV: %s\n', resultCsv);
fprintf('Exported compact LUT CSV: %s\n', compactCsv);
if exportCHeader
    fprintf('Exported compact C LUT header: %s\n', fullfile(outputDir, cHeaderFileName));
end

if height(validRows) > 0
    [~, idxBest] = max(validRows.Best_Ra);
    row = validRows(idxBest, :);
    fprintf('\nGlobal best grid target by Ra:\n');
    fprintf('  Target xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        row.Target_x, row.Target_y, row.Target_u1976, row.Target_v1976);
    fprintf('  Best RGB point xy=(%.6f, %.6f), u''v''=(%.6f, %.6f)\n', ...
        row.Best_RGBPoint_x, row.Best_RGBPoint_y, row.Best_RGBPoint_u1976, row.Best_RGBPoint_v1976);
    fprintf('  Ra=%.4f, Rmin8=%.4f, RayT=%.6f\n', row.Best_Ra, row.Best_Rmin_8, row.Best_RayT);
    fprintf('  Weights: Single=%.6f, R=%.12f, G=%.12f, B=%.12f\n', ...
        row.Best_SingleWhiteRatio, row.Best_R_weight, row.Best_G_weight, row.Best_B_weight);
end

%% ================= Local functions =================

function lutRows = scan_target_grid_to_lut(targetGrid, targetY_lm, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre)

    pointCount = height(targetGrid);

    varNames = {'GridI','GridJ','GridK', ...
        'Target_x','Target_y','Target_Y_lm','Target_u1976','Target_v1976', ...
        'Best_Ra','Best_Rmin_8','Best_Worst_R','Best_RayT', ...
        'Best_SingleWhiteRatio','Best_RGBWhiteRatio', ...
        'Best_RGBPoint_x','Best_RGBPoint_y','Best_RGBPoint_Y_lm','Best_RGBPoint_u1976','Best_RGBPoint_v1976', ...
        'Best_R_weight','Best_G_weight','Best_B_weight', ...
        'Mix_x','Mix_y','Mix_Y_lm','Mix_CCT_K','BoundaryEdge','Valid'};

    varTypes = {'double','double','double', ...
        'double','double','double','double','double', ...
        'double','double','string','double', ...
        'double','double', ...
        'double','double','double','double','double', ...
        'double','double','double', ...
        'double','double','double','double','string','logical'};

    lutRows = table('Size', [pointCount, numel(varNames)], ...
        'VariableTypes', varTypes, 'VariableNames', varNames);

    for i = 1:pointCount
        target_xyY = [targetGrid.x(i), targetGrid.y(i), targetY_lm];

        best = find_best_ra_for_target_optimized(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre);

        lutRows.GridI(i) = targetGrid.GridI(i);
        lutRows.GridJ(i) = targetGrid.GridJ(i);
        lutRows.GridK(i) = targetGrid.GridK(i);
        lutRows.Target_x(i) = targetGrid.x(i);
        lutRows.Target_y(i) = targetGrid.y(i);
        lutRows.Target_Y_lm(i) = targetY_lm;
        lutRows.Target_u1976(i) = targetGrid.u1976(i);
        lutRows.Target_v1976(i) = targetGrid.v1976(i);

        if best.Valid
            lutRows.Best_Ra(i) = best.Ra;
            lutRows.Best_Rmin_8(i) = best.Rmin8;
            lutRows.Best_Worst_R(i) = "R" + string(best.WorstIndex);
            lutRows.Best_RayT(i) = best.RayT;
            lutRows.Best_SingleWhiteRatio(i) = best.SingleWhiteRatio;
            lutRows.Best_RGBWhiteRatio(i) = best.RGBWhiteRatio;
            lutRows.Best_RGBPoint_x(i) = best.RGBPoint_xyY(1);
            lutRows.Best_RGBPoint_y(i) = best.RGBPoint_xyY(2);
            lutRows.Best_RGBPoint_Y_lm(i) = best.RGBPoint_xyY(3);
            bestRgbUv = xy_to_uv1976(best.RGBPoint_xyY(1:2));
            lutRows.Best_RGBPoint_u1976(i) = bestRgbUv(1);
            lutRows.Best_RGBPoint_v1976(i) = bestRgbUv(2);
            lutRows.Best_R_weight(i) = best.R_weight;
            lutRows.Best_G_weight(i) = best.G_weight;
            lutRows.Best_B_weight(i) = best.B_weight;
            lutRows.Mix_x(i) = best.Mix_xyY(1);
            lutRows.Mix_y(i) = best.Mix_xyY(2);
            lutRows.Mix_Y_lm(i) = best.Mix_xyY(3);
            lutRows.Mix_CCT_K(i) = best.CCT;
            lutRows.BoundaryEdge(i) = best.BoundaryEdge;
            lutRows.Valid(i) = true;
        else
            lutRows.Best_Worst_R(i) = "";
            lutRows.BoundaryEdge(i) = "";
            lutRows.Valid(i) = false;
        end
    end
end

function centerGrid = make_triangle_cell_center_grid_uv1976(baseGrid, divCount, targetY_lm)
    % 每个基础小三角形取一个中心点，用于判断是否需要自适应加密。
    baseKey = make_grid_key(baseGrid.GridI, baseGrid.GridJ, baseGrid.GridK);
    baseIndex = containers.Map(baseKey, num2cell(1:height(baseGrid)));

    maxCenterCount = divCount * divCount;
    vals = nan(maxCenterCount, 11);
    idx = 0;

    for i = 0:(divCount - 1)
        for j = 0:(divCount - i - 1)
            k = divCount - i - j;

            % Lower triangle: (i,j,k), (i+1,j,k-1), (i,j+1,k-1)
            v1 = get_grid_row(baseGrid, baseIndex, i, j, k);
            v2 = get_grid_row(baseGrid, baseIndex, i + 1, j, k - 1);
            v3 = get_grid_row(baseGrid, baseIndex, i, j + 1, k - 1);
            idx = idx + 1;
            vals(idx, :) = make_center_row(baseGrid, [v1, v2, v3], targetY_lm);

            % Upper triangle exists when k >= 2:
            % (i+1,j,k-1), (i+1,j+1,k-2), (i,j+1,k-1)
            if k >= 2
                v1 = get_grid_row(baseGrid, baseIndex, i + 1, j, k - 1);
                v2 = get_grid_row(baseGrid, baseIndex, i + 1, j + 1, k - 2);
                v3 = get_grid_row(baseGrid, baseIndex, i, j + 1, k - 1);
                idx = idx + 1;
                vals(idx, :) = make_center_row(baseGrid, [v1, v2, v3], targetY_lm);
            end
        end
    end

    vals = vals(1:idx, :);
    centerGrid = array2table(vals, 'VariableNames', ...
        {'GridI','GridJ','GridK','x','y','Y_lm','u1976','v1976','V1','V2','V3'});
end

function row = make_center_row(baseGrid, vertexRows, targetY_lm)
    uv = mean([baseGrid.u1976(vertexRows), baseGrid.v1976(vertexRows)], 1);
    xy = uv1976_to_xy(uv);
    gridI = mean(baseGrid.GridI(vertexRows));
    gridJ = mean(baseGrid.GridJ(vertexRows));
    gridK = mean(baseGrid.GridK(vertexRows));
    row = [gridI, gridJ, gridK, xy(1), xy(2), targetY_lm, uv(1), uv(2), vertexRows(1), vertexRows(2), vertexRows(3)];
end

function rowIndex = get_grid_row(baseGrid, baseIndex, i, j, k)
    key = make_grid_key(i, j, k);
    if ~isKey(baseIndex, key)
        error('Cannot find base grid vertex (%g,%g,%g).', i, j, k);
    end
    rowIndex = baseIndex(key);
end

function key = make_grid_key(i, j, k)
    key = string(i) + "_" + string(j) + "_" + string(k);
    key = cellstr(key);
    if numel(key) == 1
        key = key{1};
    end
end

function refineMask = select_adaptive_center_points(baseRows, centerRows, centerGrid, ...
    keepAllCenters, whiteRatioDiffThreshold, raErrorThreshold, whiteRatioMin)

    if keepAllCenters
        refineMask = true(height(centerGrid), 1);
        return;
    end

    refineMask = false(height(centerGrid), 1);

    for i = 1:height(centerGrid)
        if ~centerRows.Valid(i)
            continue;
        end

        v = [centerGrid.V1(i), centerGrid.V2(i), centerGrid.V3(i)];
        vertexValid = baseRows.Valid(v);
        if ~all(vertexValid)
            refineMask(i) = true;
            continue;
        end

        vertexWhite = baseRows.Best_SingleWhiteRatio(v);
        vertexRa = baseRows.Best_Ra(v);

        interpWhite = mean(vertexWhite);
        interpRa = mean(vertexRa);

        centerWhite = centerRows.Best_SingleWhiteRatio(i);
        centerRa = centerRows.Best_Ra(i);

        whiteError = abs(centerWhite - interpWhite);
        raError = abs(centerRa - interpRa);

        transitionHidden = (centerWhite > whiteRatioMin) && (max(vertexWhite) <= whiteRatioMin);

        if whiteError > whiteRatioDiffThreshold || raError > raErrorThreshold || transitionHidden
            refineMask(i) = true;
        end
    end
end

function outGrid = unique_target_grid(inGrid)
    core = inGrid(:, {'GridI','GridJ','GridK','x','y','Y_lm','u1976','v1976'});
    keyU = round(core.u1976 .* 1e10);
    keyV = round(core.v1976 .* 1e10);
    [~, uniqueIndex] = unique([keyU, keyV], 'rows', 'stable');
    outGrid = core(uniqueIndex, :);
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

function grid = make_triangle_target_grid_uv1976(Rxy, Gxy, Bxy, targetY_lm, divCount)
    Ruv = xy_to_uv1976(Rxy);
    Guv = xy_to_uv1976(Gxy);
    Buv = xy_to_uv1976(Bxy);

    pointCount = (divCount + 1) * (divCount + 2) / 2;
    vals = nan(pointCount, 8);
    idx = 0;

    for i = 0:divCount
        for j = 0:(divCount - i)
            k = divCount - i - j;
            a = i / divCount;
            b = j / divCount;
            c = k / divCount;
            uv = a .* Ruv + b .* Guv + c .* Buv;
            xy = uv1976_to_xy(uv);
            idx = idx + 1;
            vals(idx, :) = [i, j, k, xy(1), xy(2), targetY_lm, uv(1), uv(2)];
        end
    end

    grid = array2table(vals, 'VariableNames', ...
        {'GridI','GridJ','GridK','x','y','Y_lm','u1976','v1976'});
end

function best = find_best_ra_for_target_optimized(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre)
% Same geometric ray-scan logic as calculate_guangpu_metameric_cri_backup_20260509_before_duv005.m.
% Optimization: do not build a per-target blendRows table and do not redo fixed
% channel integrals for every rayT. The mixed spectrum math is still linear and
% equivalent to the original calculation.

    best.Valid = false;
    best.Ra = -Inf;
    best.Rmin8 = -Inf;
    best.WorstIndex = NaN;
    best.RayT = NaN;
    best.SingleWhiteRatio = NaN;
    best.RGBWhiteRatio = NaN;
    best.RGBPoint_xyY = [NaN NaN NaN];
    best.R_weight = NaN;
    best.G_weight = NaN;
    best.B_weight = NaN;
    best.Mix_xyY = [NaN NaN NaN];
    best.CCT = NaN;
    best.BoundaryEdge = "";

    targetXY = target_xyY(1:2);

    try
        [~, boundaryRayT, boundaryEdge, ~] = ...
            find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
        rayTList = linspace(1, boundaryRayT, rayScanCount).';

        % Reference source is fixed for this target because every valid rayT
        % is constrained to the same target xyY.
        targetXYZ = xyY_to_XYZ(target_xyY);
        targetUV = XYZ_to_uv1960(targetXYZ);
        [targetCCT, ~] = estimate_cct_uv(targetUV, std.cmf, wl, dLambda);
        refSpd = reference_spd(targetCCT, std, wl);
        refXYZ = source_XYZ(refSpd, std.cmf, dLambda);
        refUV = XYZ_to_uv1960(refXYZ);
        refUVW = tcs_uvw(refSpd, std.cmf, std.tcs, dLambda, refUV, refUV, false);

        for i = 1:numel(rayTList)
            rayT = rayTList(i);
            rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);
            split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
            rgbPoint_xyY = [rgbPointXY, split.RGBY];
            pointWeights = solve_rgb_point_weights(rgbPoint_xyY, rgbSolveMatrix);

            % Fast equivalent of:
            % rgbPointSpd = Rw*spd.R + Gw*spd.G + Bw*spd.B;
            % mixSpd = split.WhiteYRatio*singleNorm + rgbPointSpd;
            m = spectrum_metrics_fast(split.WhiteYRatio, pointWeights, fastPre, targetCCT, refUV, refUVW);

            if m.Ra > best.Ra
                best.Valid = true;
                best.Ra = m.Ra;
                best.Rmin8 = m.Rmin8;
                best.WorstIndex = m.WorstIndex;
                best.RayT = rayT;
                best.SingleWhiteRatio = split.WhiteYRatio;
                best.RGBWhiteRatio = split.RGBYRatio;
                best.RGBPoint_xyY = [rgbPointXY, split.RGBY];
                best.R_weight = pointWeights(1);
                best.G_weight = pointWeights(2);
                best.B_weight = pointWeights(3);
                best.Mix_xyY = m.xyY;
                best.CCT = m.CCT;
                if i == numel(rayTList)
                    best.BoundaryEdge = string(boundaryEdge);
                else
                    best.BoundaryEdge = "";
                end
            end
        end
    catch ME
        warning('find_best_ra_for_target_optimized:InvalidTarget', ...
            'Skip target xy=(%.6f, %.6f): %s', target_xyY(1), target_xyY(2), ME.message);
    end
end

function weights = solve_rgb_point_weights(point_xyY, rgbSolveMatrix)
    b = xyY_to_XYZ(point_xyY).';
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;

    if any(weights < -1e-8)
        error('solve_rgb_point_weights:OutsideGamut', ...
            'RGB candidate xy=(%.6f, %.6f) is outside the measured RGB gamut.', ...
            point_xyY(1), point_xyY(2));
    end
end

function fastPre = build_fast_cri_precompute(spd, singleNorm, std, dLambda)
    fastPre.cmf = std.cmf;
    fastPre.tcs = std.tcs;
    fastPre.dLambda = dLambda;

    fastPre.XYZ.SingleWhite = source_XYZ(singleNorm, std.cmf, dLambda);
    fastPre.XYZ.R = source_XYZ(spd.R, std.cmf, dLambda);
    fastPre.XYZ.G = source_XYZ(spd.G, std.cmf, dLambda);
    fastPre.XYZ.B = source_XYZ(spd.B, std.cmf, dLambda);

    fastPre.TCS.SingleWhite = channel_tcs_xyz_unscaled(singleNorm, std.cmf, std.tcs, dLambda);
    fastPre.TCS.R = channel_tcs_xyz_unscaled(spd.R, std.cmf, std.tcs, dLambda);
    fastPre.TCS.G = channel_tcs_xyz_unscaled(spd.G, std.cmf, std.tcs, dLambda);
    fastPre.TCS.B = channel_tcs_xyz_unscaled(spd.B, std.cmf, std.tcs, dLambda);
end

function tcsXYZ = channel_tcs_xyz_unscaled(channelSpd, cmf, tcs, dLambda)
    sampleCount = size(tcs, 2);
    tcsXYZ = zeros(sampleCount, 3);
    for i = 1:sampleCount
        tcsXYZ(i, :) = sum(channelSpd(:) .* tcs(:, i) .* cmf, 1) .* dLambda;
    end
end

function m = spectrum_metrics_fast(singleWhiteRatio, weights, fastPre, CCT, refUV, refUVW)
    mixXYZ = singleWhiteRatio .* fastPre.XYZ.SingleWhite + ...
             weights(1) .* fastPre.XYZ.R + ...
             weights(2) .* fastPre.XYZ.G + ...
             weights(3) .* fastPre.XYZ.B;

    xyY = XYZ_to_xyY(mixXYZ);
    photopicY = 683 * mixXYZ(2);
    sourceUV = XYZ_to_uv1960(mixXYZ);

    mixTcsXYZ = singleWhiteRatio .* fastPre.TCS.SingleWhite + ...
                weights(1) .* fastPre.TCS.R + ...
                weights(2) .* fastPre.TCS.G + ...
                weights(3) .* fastPre.TCS.B;

    testUVW = tcs_uvw_from_unscaled_tcs_xyz(mixTcsXYZ, mixXYZ(2), sourceUV, refUV, true);

    Ri = zeros(8, 1);
    for i = 1:8
        deltaE = norm(refUVW(i,:) - testUVW(i,:));
        Ri(i) = 100 - 4.6 * deltaE;
    end

    [Rmin8, worstIndex] = min(Ri);
    m.XYZ = mixXYZ;
    m.xyY = [xyY(1), xyY(2), photopicY];
    m.photopicY = photopicY;
    m.uv1960 = sourceUV;
    m.CCT = CCT;
    m.Ri = Ri;
    m.Ra = mean(Ri);
    m.Rmin8 = Rmin8;
    m.WorstIndex = worstIndex;
end

function UVW = tcs_uvw_from_unscaled_tcs_xyz(unscaledTcsXYZ, sourceY, sourceUV, refUV, doAdapt)
    k = 100 / sourceY;
    UVW = zeros(size(unscaledTcsXYZ, 1), 3);

    cfun = @(u, v) (4 - u - 10 * v) / v;
    dfun = @(u, v) (1.708 * v + 0.404 - 1.481 * u) / v;
    sourceC = cfun(sourceUV(1), sourceUV(2));
    sourceD = dfun(sourceUV(1), sourceUV(2));
    refC = cfun(refUV(1), refUV(2));
    refD = dfun(refUV(1), refUV(2));

    for i = 1:size(unscaledTcsXYZ, 1)
        sampleXYZ = k .* unscaledTcsXYZ(i, :);
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

function validate_fast_cri_evaluation(targetGrid, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount, fastPre, toleranceRa)
    fprintf('\nValidating fast CRI evaluation against original spectrum_metrics...\n');
    if height(targetGrid) == 0
        return;
    end

    targetIndices = unique(round(linspace(1, height(targetGrid), min(5, height(targetGrid)))));
    maxRaDiff = 0;

    for ti = targetIndices(:).'
        target_xyY = [targetGrid.x(ti), targetGrid.y(ti), targetGrid.Y_lm(ti)];
        try
            [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, target_xyY(1:2), rgbTriangleXY);
            rayList = linspace(1, boundaryRayT, min(9, rayScanCount)).';
            targetXYZ = xyY_to_XYZ(target_xyY);
            targetUV = XYZ_to_uv1960(targetXYZ);
            [targetCCT, ~] = estimate_cct_uv(targetUV, std.cmf, wl, dLambda);
            refSpd = reference_spd(targetCCT, std, wl);
            refXYZ = source_XYZ(refSpd, std.cmf, dLambda);
            refUV = XYZ_to_uv1960(refXYZ);
            refUVW = tcs_uvw(refSpd, std.cmf, std.tcs, dLambda, refUV, refUV, false);

            for ri = 1:numel(rayList)
                rayT = rayList(ri);
                rgbPointXY = singleWhiteXY + rayT .* (target_xyY(1:2) - singleWhiteXY);
                split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
                rgbPoint_xyY = [rgbPointXY, split.RGBY];
                [weights, rgbPointSpd] = solve_rgb_point_spectrum(rgbPoint_xyY, rgbSolveMatrix, spd);
                mixSpd = split.WhiteYRatio .* singleNorm + rgbPointSpd;
                exactM = spectrum_metrics(mixSpd, std, wl, dLambda);
                fastM = spectrum_metrics_fast(split.WhiteYRatio, weights, fastPre, targetCCT, refUV, refUVW);
                maxRaDiff = max(maxRaDiff, abs(exactM.Ra - fastM.Ra));
            end
        catch ME
            warning('validate_fast_cri_evaluation:SkipTarget', ...
                'Skip validation target xy=(%.6f, %.6f): %s', target_xyY(1), target_xyY(2), ME.message);
        end
    end

    fprintf('  Max Ra difference = %.10f\n', maxRaDiff);
    if maxRaDiff > toleranceRa
        error('Fast CRI validation failed: max Ra difference %.10f > tolerance %.10f.', maxRaDiff, toleranceRa);
    end
    fprintf('  Fast CRI validation passed.\n');
end

function [pointXY, rayT, edgeName, edgeIndex] = find_ray_gamut_intersection(whiteXY, targetXY, triangleXY)
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    triangleXY = double(triangleXY(:, 1:2));
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_ray_gamut_intersection:SamePoint', ...
            'The fixed single white and target have the same xy.');
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
            'RGB candidate xy=(%.6f, %.6f) is outside the measured RGB gamut.', ...
            point_xyY(1), point_xyY(2));
    end

    rgbSpd = weights(1) .* spd.R + weights(2) .* spd.G + weights(3) .* spd.B;
end

function export_best_rgb_uv_c_header(filePath, rows, divCount)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open C header for writing: %s', filePath);
    end

    cleanupObj = onCleanup(@() fclose(fid));
    fprintf(fid, '/* Auto-generated by calculate_guangpu_triangle_grid_lut.m */\n');
    fprintf(fid, '#ifndef GUANGPU_TRIANGLE_GRID_BEST_RGB_UV_LUT_H\n');
    fprintf(fid, '#define GUANGPU_TRIANGLE_GRID_BEST_RGB_UV_LUT_H\n\n');
    fprintf(fid, '#include <stdint.h>\n\n');
    fprintf(fid, '#define GUANGPU_TRI_GRID_DIV_COUNT   %uU\n', uint32(divCount));
    fprintf(fid, '#define GUANGPU_TRI_GRID_POINT_COUNT %uU\n\n', uint32(height(rows)));
    fprintf(fid, 'typedef struct\n{\n    uint16_t u_q16;\n    uint16_t v_q16;\n} ST_GuangpuBestRgbUv;\n\n');
    fprintf(fid, 'static const ST_GuangpuBestRgbUv g_GuangpuBestRgbUvLut[GUANGPU_TRI_GRID_POINT_COUNT] =\n{\n');

    for i = 1:height(rows)
        uq = q0_1_to_u16(rows.Best_RGBPoint_u1976(i));
        vq = q0_1_to_u16(rows.Best_RGBPoint_v1976(i));
        if ~rows.Valid(i)
            uq = uint16(0);
            vq = uint16(0);
        end
        fprintf(fid, '    { %5uU, %5uU }, /* %4u: grid=(%u,%u,%u) */\n', ...
            uq, vq, uint32(i - 1), uint32(rows.GridI(i)), uint32(rows.GridJ(i)), uint32(rows.GridK(i)));
    end

    fprintf(fid, '};\n\n');
    fprintf(fid, '#endif /* GUANGPU_TRIANGLE_GRID_BEST_RGB_UV_LUT_H */\n');
end

function q = q0_1_to_u16(x)
    if ~isfinite(x)
        x = 0;
    end
    x = min(max(x, 0), 1);
    q = uint16(round(x * 65535));
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
    spd = spd(:);
    spd(spd < 0) = 0;
end

function XYZ = xyY_to_XYZ(xyY)
    x = xyY(1);
    y = xyY(2);
    Y = xyY(3);
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
    x = xy(1);
    y = xy(2);
    den = -2 * x + 12 * y + 3;
    uv = [4 * x / den, 9 * y / den];
end

function xy = uv1976_to_xy(uv)
    u = uv(1);
    v = uv(2);
    den = 6 * u - 16 * v + 12;
    xy = [9 * u / den, 4 * v / den];
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
