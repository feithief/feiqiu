% CALCULATE_GUANGPU_METAMERIC_CRI
% Search the maximum CRI while the target chromaticity is allowed to move
% within DeltaUV <= targetDeltaUvLimit from cfg.target_xyY. DeltaUV here uses CIE 1976 u'v'.

clear;
clc;
%% User configurable parameters
targetDeltaUvLimit = 0.002;  % CIE 1976 u'v' DeltaUV search radius

%% Step 0: locate input files
rootDir = fileparts(mfilename('fullpath'));
inputDir = fullfile(rootDir, 'guangpu_case_input');
addpath(inputDir);
caseCfg = guangpu_case_config();
if isfield(caseCfg, 'inputDir') && ~isempty(caseCfg.inputDir)
    spectraDir = caseCfg.inputDir;
else
    spectraDir = inputDir;
end
singleWhiteFile = fullfile(spectraDir, 'W.CSV');
standardDir = fullfile(rootDir, 'standard_data');
if ~exist(standardDir, 'dir')
    mkdir(standardDir);
end

%% Step 1: target, channels, and search settings
target_xyY = caseCfg.target_xyY;
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;
searchCfg = default_search_config(caseCfg, targetDeltaUvLimit);

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

%% Step 5: scale each measured spectrum to its measured photopic Y
spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

%% Step 6: use actual spectral xyY for the RGB solver
sourceMetrics.R = spectrum_metrics(spd.R, std, wl, dLambda);
sourceMetrics.G = spectrum_metrics(spd.G, std, wl, dLambda);
sourceMetrics.B = spectrum_metrics(spd.B, std, wl, dLambda);
sourceMetrics.SingleWhite = spectrum_metrics(scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda), std, wl, dLambda);
sourceMetrics.W = spectrum_metrics(spd.W, std, wl, dLambda);

actual_xyY.R = sourceMetrics.R.xyY;
actual_xyY.G = sourceMetrics.G.xyY;
actual_xyY.B = sourceMetrics.B.xyY;
actual_xyY.SingleWhite = sourceMetrics.SingleWhite.xyY;
actual_xyY.W = sourceMetrics.W.xyY;

singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda);

%% Step 7: nominal RGB target point, used as the zero-offset reference row
nominalRgbWeights = solve_rgb_weights(target_xyY, actual_xyY);
nominalRgbSpd = nominalRgbWeights(1) .* spd.R + ...
                nominalRgbWeights(2) .* spd.G + ...
                nominalRgbWeights(3) .* spd.B;
nominalRgbMetrics = spectrum_metrics(nominalRgbSpd, std, wl, dLambda);

%% Step 8: coarse and fine search inside target DeltaUV limit
coarseTargets = make_target_candidates(target_xyY, searchCfg.deltaUvLimit, ...
    searchCfg.coarseUvStep, xy_to_uv1976(target_xyY(1:2)), searchCfg.deltaUvLimit);
coarseRows = scan_targets(coarseTargets, "coarse", target_xyY, actual_xyY, ...
    spd, singleNorm, std, wl, dLambda, searchCfg.coarseRaySamples);

if height(coarseRows) == 0
    error('No valid coarse-search candidate was found inside DeltaUV <= %.6f.', searchCfg.deltaUvLimit);
end
[~, coarseBestIdx] = max(coarseRows.Ra);
fineCenterUv = xy_to_uv1976([coarseRows.Target_x(coarseBestIdx), coarseRows.Target_y(coarseBestIdx)]);
fineTargets = make_target_candidates(target_xyY, searchCfg.deltaUvLimit, ...
    searchCfg.fineUvStep, fineCenterUv, searchCfg.fineUvRadius);
fineRows = scan_targets(fineTargets, "fine", target_xyY, actual_xyY, ...
    spd, singleNorm, std, wl, dLambda, searchCfg.fineRaySamples);

blendRows = [coarseRows; fineRows];
[~, idxBestRa] = max(blendRows.Ra);
[~, idxBestRmin] = max(blendRows.Rmin_8);
bestRa = blendRows(idxBestRa, :);
bestRmin = blendRows(idxBestRmin, :);

%% Step 9: export summary tables
actualTable = table( ...
    ["R"; "G"; "B"; "SingleWhite"; "W channel"], ...
    [actual_xyY.R(1); actual_xyY.G(1); actual_xyY.B(1); actual_xyY.SingleWhite(1); actual_xyY.W(1)], ...
    [actual_xyY.R(2); actual_xyY.G(2); actual_xyY.B(2); actual_xyY.SingleWhite(2); actual_xyY.W(2)], ...
    [actual_xyY.R(3); actual_xyY.G(3); actual_xyY.B(3); actual_xyY.SingleWhite(3); actual_xyY.W(3)], ...
    'VariableNames', {'Source','x','y','Y_lm'});

weightsTable = table( ...
    ["R"; "G"; "B"], nominalRgbWeights(:), ...
    'VariableNames', {'Channel', 'Scale'});

bestWeightsTable = table( ...
    ["RGB target"; "Best Ra blend"; "Best Rmin8 blend"], ...
    [nominalRgbWeights(1); bestRa.R_weight; bestRmin.R_weight], ...
    [nominalRgbWeights(2); bestRa.G_weight; bestRmin.G_weight], ...
    [nominalRgbWeights(3); bestRa.B_weight; bestRmin.B_weight], ...
    'VariableNames', {'Case','R_weight','G_weight','B_weight'});

summary = table( ...
    ["Single white fixed spectrum"; "Nominal RGB target point"; "W channel"; "Best Ra within DeltaUV"; "Best Rmin8 within DeltaUV"], ...
    [sourceMetrics.SingleWhite.xyY(1); nominalRgbMetrics.xyY(1); sourceMetrics.W.xyY(1); bestRa.x; bestRmin.x], ...
    [sourceMetrics.SingleWhite.xyY(2); nominalRgbMetrics.xyY(2); sourceMetrics.W.xyY(2); bestRa.y; bestRmin.y], ...
    [sourceMetrics.SingleWhite.photopicY; nominalRgbMetrics.photopicY; sourceMetrics.W.photopicY; bestRa.Y_lm; bestRmin.Y_lm], ...
    [sourceMetrics.SingleWhite.CCT; nominalRgbMetrics.CCT; sourceMetrics.W.CCT; bestRa.CCT_K; bestRmin.CCT_K], ...
    [sourceMetrics.SingleWhite.Ra; nominalRgbMetrics.Ra; sourceMetrics.W.Ra; bestRa.Ra; bestRmin.Ra], ...
    [sourceMetrics.SingleWhite.Rmin8; nominalRgbMetrics.Rmin8; sourceMetrics.W.Rmin8; bestRa.Rmin_8; bestRmin.Rmin_8], ...
    [0; delta_uv_from_xy(nominalRgbMetrics.xyY(1:2), target_xyY(1:2)); delta_uv_from_xy(sourceMetrics.W.xyY(1:2), target_xyY(1:2)); bestRa.TargetDelta_uv; bestRmin.TargetDelta_uv], ...
    ["-"; "-"; "-"; string(format_blend_ratio(bestRa)); string(format_blend_ratio(bestRmin))], ...
    'VariableNames', {'Case','x','y','Y_lm','CCT_K','Ra','Rmin_8','TargetDelta_uv','BlendRatio'});

writetable(actualTable, fullfile(rootDir, 'guangpu_actual_spectral_xyY.csv'));
writetable(weightsTable, fullfile(rootDir, 'guangpu_rgb_target_mix_weights.csv'));
writetable(bestWeightsTable, fullfile(rootDir, 'guangpu_best_rgb_point_mix_weights.csv'));
writetable(blendRows, fullfile(rootDir, 'guangpu_metameric_blend_results.csv'));
writetable(summary, fullfile(rootDir, 'guangpu_cri_summary.csv'));

%% Step 10: export spectra
rgbSpectrumTable = table( ...
    wl, nominalRgbWeights(1) .* spd.R, nominalRgbWeights(2) .* spd.G, nominalRgbWeights(3) .* spd.B, ...
    nominalRgbSpd, singleNorm, ...
    'VariableNames', {'wavelength_nm','R_component_W_nm','G_component_W_nm','B_component_W_nm','S_RGB_target_W_nm','S_single_white_W_nm'});
writetable(rgbSpectrumTable, fullfile(rootDir, 'guangpu_rgb_target_spectrum_0p2nm.csv'));

bestRaSpd = row_to_spectrum(bestRa, spd, singleNorm);
bestRminSpd = row_to_spectrum(bestRmin, spd, singleNorm);
bestBlendSpectrumTable = table( ...
    wl, singleNorm, nominalRgbSpd, bestRaSpd, bestRminSpd, ...
    'VariableNames', {'wavelength_nm','S_single_white_W_nm','S_RGB_target_W_nm','S_best_Ra_blend_W_nm','S_best_Rmin8_blend_W_nm'});
writetable(bestBlendSpectrumTable, fullfile(rootDir, 'guangpu_best_blend_spectrum_0p2nm.csv'));

%% Step 11: print key results
fprintf('\nNominal target xyY = (%.4f, %.4f, %.4f)\n', target_xyY);
fprintf('Target DeltaUV limit = %.6f, coarse targets = %d, fine targets = %d, evaluated rows = %d\n', ...
    searchCfg.deltaUvLimit, height(coarseTargets), height(fineTargets), height(blendRows));
fprintf('Nominal RGB weights: R=%.12f, G=%.12f, B=%.12f\n', nominalRgbWeights(1), nominalRgbWeights(2), nominalRgbWeights(3));
print_metrics('Single white fixed spectrum', sourceMetrics.SingleWhite);
print_metrics('Nominal RGB target point', nominalRgbMetrics);
print_metrics('W channel', sourceMetrics.W);
print_best_row('Best Ra within DeltaUV', bestRa);
print_best_row('Best Rmin8 within DeltaUV', bestRmin);

function searchCfg = default_search_config(caseCfg, targetDeltaUvLimit)
    searchCfg.deltaUvLimit = targetDeltaUvLimit;
    searchCfg.coarseUvStep = get_optional_scalar(caseCfg, 'targetCoarseUvStep', 0.0015);
    searchCfg.fineUvStep = get_optional_scalar(caseCfg, 'targetFineUvStep', 0.0005);
    searchCfg.fineUvRadius = get_optional_scalar(caseCfg, 'targetFineUvRadius', 0.0010);
    searchCfg.coarseRaySamples = get_optional_scalar(caseCfg, 'coarseRaySamples', 61);
    searchCfg.fineRaySamples = get_optional_scalar(caseCfg, 'fineRaySamples', 151);
end

function value = get_optional_scalar(s, fieldName, defaultValue)
    if isstruct(s) && isfield(s, fieldName) && isscalar(s.(fieldName)) && isfinite(s.(fieldName))
        value = s.(fieldName);
    else
        value = defaultValue;
    end
end

function targets = make_target_candidates(nominal_xyY, deltaLimit, uvStep, centerUv, radius)
    nominalUv = xy_to_uv1976(nominal_xyY(1:2));
    offsets = (-radius:uvStep:radius).';
    maxN = numel(offsets) ^ 2 + 1;
    vals = nan(maxN, 4);
    n = 0;
    for iu = 1:numel(offsets)
        for iv = 1:numel(offsets)
            du = offsets(iu);
            dv = offsets(iv);
            if hypot(du, dv) > radius + eps
                continue;
            end
            uv = centerUv + [du, dv];
            deltaUv = norm(uv - nominalUv);
            if deltaUv > deltaLimit + eps
                continue;
            end
            xy = uv1976_to_xy(uv);
            if all(isfinite(xy)) && xy(1) > 0 && xy(2) > 0 && (xy(1) + xy(2)) < 1
                n = n + 1;
                vals(n, :) = [xy(1), xy(2), nominal_xyY(3), deltaUv];
            end
        end
    end
    n = n + 1;
    vals(n, :) = [nominal_xyY(1), nominal_xyY(2), nominal_xyY(3), 0];
    vals = vals(1:n, :);
    rounded = round(vals(:, 1:2) * 1e8) / 1e8;
    [~, keep] = unique(rounded, 'rows', 'stable');
    vals = vals(keep, :);
    targets = array2table(vals, 'VariableNames', {'x','y','Y_lm','TargetDelta_uv'});
end

function rows = scan_targets(targets, stageName, nominalTarget, actual_xyY, spd, singleNorm, std, wl, dLambda, raySamples)
    varNames = {'SearchStage','Target_x','Target_y','TargetDelta_uv','RayT','WhiteXYCoeff','RGBXYCoeff', ...
        'SingleWhiteRatio','RGBWhiteRatio','RGBPoint_x','RGBPoint_y','RGBPoint_Y_lm','R_weight','G_weight','B_weight', ...
        'x','y','Y_lm','CCT_K','Ra','Rmin_8','R1','R2','R3','R4','R5','R6','R7','R8','Worst_R','Edge'};
    maxRows = max(1, height(targets) * raySamples);
    stage = strings(maxRows, 1);
    nums = nan(maxRows, 28);
    worst = strings(maxRows, 1);
    edgeOut = strings(maxRows, 1);
    row = 0;

    whiteXY = actual_xyY.SingleWhite(1:2);
    vertices = [actual_xyY.R(1:2); actual_xyY.G(1:2); actual_xyY.B(1:2)];

    for it = 1:height(targets)
        targetXY = [targets.x(it), targets.y(it)];
        targetY = targets.Y_lm(it);
        [rayTMax, edgeAtMax] = ray_to_gamut_boundary(whiteXY, targetXY, vertices);
        if ~isfinite(rayTMax) || rayTMax < 1
            continue;
        end
        tList = linspace(1, rayTMax, raySamples).';
        for ir = 1:numel(tList)
            rayT = tList(ir);
            rgbXY = whiteXY + rayT .* (targetXY - whiteXY);
            lambda = 1 / rayT;
            whiteXYCoeff = 1 - lambda;
            rgbXYCoeff = lambda;
            whiteRatio = whiteXYCoeff * whiteXY(2) / targetXY(2);
            rgbRatio = rgbXYCoeff * rgbXY(2) / targetXY(2);
            rgbPointY = targetY * rgbRatio;
            rgbPoint_xyY = [rgbXY, rgbPointY];
            [rgbWeights, ok] = solve_rgb_weights_safe(rgbPoint_xyY, actual_xyY);
            if ~ok
                continue;
            end
            mixSpd = whiteRatio .* singleNorm + ...
                     rgbWeights(1) .* spd.R + ...
                     rgbWeights(2) .* spd.G + ...
                     rgbWeights(3) .* spd.B;
            m = spectrum_metrics(mixSpd, std, wl, dLambda);
            row = row + 1;
            stage(row) = stageName;
            nums(row, :) = [targetXY(1), targetXY(2), targets.TargetDelta_uv(it), rayT, whiteXYCoeff, rgbXYCoeff, ...
                whiteRatio, rgbRatio, rgbXY(1), rgbXY(2), rgbPointY, rgbWeights(1), rgbWeights(2), rgbWeights(3), ...
                m.xyY(1), m.xyY(2), m.photopicY, m.CCT, m.Ra, m.Rmin8, m.Ri(:).'];
            worst(row) = "R" + string(m.WorstIndex);
            if abs(rayT - rayTMax) <= max(1e-10, abs(rayTMax) * 1e-10)
                edgeOut(row) = edgeAtMax;
            else
                edgeOut(row) = "";
            end
        end
    end

    stage = stage(1:row);
    nums = nums(1:row, :);
    worst = worst(1:row);
    edgeOut = edgeOut(1:row);
    rows = table(stage, nums(:,1), nums(:,2), nums(:,3), nums(:,4), nums(:,5), nums(:,6), nums(:,7), nums(:,8), ...
        nums(:,9), nums(:,10), nums(:,11), nums(:,12), nums(:,13), nums(:,14), nums(:,15), nums(:,16), nums(:,17), ...
        nums(:,18), nums(:,19), nums(:,20), nums(:,21), nums(:,22), nums(:,23), nums(:,24), nums(:,25), nums(:,26), ...
        nums(:,27), nums(:,28), worst, edgeOut, 'VariableNames', varNames);
end

function [rayTMax, edgeName] = ray_to_gamut_boundary(whiteXY, targetXY, vertices)
    dir = targetXY - whiteXY;
    rayTMax = NaN;
    edgeName = "";
    if norm(dir) <= eps
        return;
    end
    edgeLabels = ["R-G", "G-B", "B-R"];
    for i = 1:3
        a = vertices(i, :);
        b = vertices(mod(i, 3) + 1, :);
        edge = b - a;
        den = cross2d(dir, edge);
        if abs(den) <= 1e-12
            continue;
        end
        rel = a - whiteXY;
        t = cross2d(rel, edge) / den;
        u = cross2d(rel, dir) / den;
        if t >= 1 - 1e-10 && u >= -1e-10 && u <= 1 + 1e-10
            if ~isfinite(rayTMax) || t < rayTMax
                rayTMax = t;
                edgeName = edgeLabels(i);
            end
        end
    end
end

function z = cross2d(a, b)
    z = a(1) * b(2) - a(2) * b(1);
end

function [weights, ok] = solve_rgb_weights_safe(rgbPoint_xyY, actual_xyY)
    weights = solve_rgb_weights(rgbPoint_xyY, actual_xyY);
    tol = 1e-8;
    ok = all(isfinite(weights)) && all(weights >= -tol);
    weights(abs(weights) < tol) = 0;
end

function weights = solve_rgb_weights(targetPoint_xyY, actual_xyY)
    A = [xyY_to_XYZ(actual_xyY.R).', xyY_to_XYZ(actual_xyY.G).', xyY_to_XYZ(actual_xyY.B).'];
    b = xyY_to_XYZ(targetPoint_xyY).';
    weights = A \ b;
end

function spdOut = row_to_spectrum(row, spd, singleNorm)
    spdOut = row.SingleWhiteRatio .* singleNorm + ...
             row.R_weight .* spd.R + ...
             row.G_weight .* spd.G + ...
             row.B_weight .* spd.B;
end

function s = format_blend_ratio(row)
    s = sprintf('targetDeltaUV=%.6f,rayT=%.4f,singleY=%.3f,rgbY=%.3f,rgbXY=(%.5f,%.5f)', ...
        row.TargetDelta_uv, row.RayT, row.SingleWhiteRatio, row.RGBWhiteRatio, row.RGBPoint_x, row.RGBPoint_y);
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

function deltaUv = delta_uv_from_xy(actualXY, targetXY)
    deltaUv = norm(xy_to_uv1976(actualXY) - xy_to_uv1976(targetXY));
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

function print_metrics(name, m)
    fprintf('\n%s\n', name);
    fprintf('  xyY=(%.5f, %.5f, %.4f lm), CCT=%.1f K\n', m.xyY(1), m.xyY(2), m.xyY(3), m.CCT);
    fprintf('  Ra=%.4f, Rmin8=%.4f, worst=R%d\n', m.Ra, m.Rmin8, m.WorstIndex);
    fprintf('  R1-R8: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n', m.Ri);
end

function print_best_row(name, row)
    fprintf('\n%s\n', name);
    fprintf('  target xy=(%.5f, %.5f), TargetDeltaUV=%.6f\n', row.Target_x, row.Target_y, row.TargetDelta_uv);
    fprintf('  mix xyY=(%.5f, %.5f, %.4f lm), CCT=%.1f K\n', row.x, row.y, row.Y_lm, row.CCT_K);
    fprintf('  Ra=%.4f, Rmin8=%.4f, worst=%s\n', row.Ra, row.Rmin_8, char(row.Worst_R));
    fprintf('  rayT=%.6f, singleYRatio=%.6f, rgbYRatio=%.6f\n', row.RayT, row.SingleWhiteRatio, row.RGBWhiteRatio);
    fprintf('  RGB point xy=(%.5f, %.5f), RGB point Y=%.4f lm\n', row.RGBPoint_x, row.RGBPoint_y, row.RGBPoint_Y_lm);
    fprintf('  RGB weights: R=%.12f, G=%.12f, B=%.12f\n', row.R_weight, row.G_weight, row.B_weight);
end
