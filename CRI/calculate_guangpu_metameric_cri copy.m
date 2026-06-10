% CALCULATE_GUANGPU_METAMERIC_CRI
% Fixed-single-white + RGB ray scan for guangpu spectra.
% The mixed output is constrained to cfg.target_xyY = [0.3169, 0.3310, 2.7000].

clear;
clc;

%% Step 0: locate input files
rootDir = fileparts(mfilename('fullpath'));
inputDir = fullfile(rootDir, 'guangpu_case_input');
addpath(inputDir);
caseCfg = guangpu_case_config();
spectraDir = caseCfg.inputDir;
singleWhiteFile = fullfile(spectraDir, 'W.CSV');
standardDir = fullfile(rootDir, 'standard_data');
if ~exist(standardDir, 'dir')
    mkdir(standardDir);
end

%% Step 1: target and channel xyY
% target_xyY = [0.3169, 0.3310, 2.7000];
target_xyY = [0.3174, 0.339, 2.7000];
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;
rayScanCount = 1001;

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

%% Step 5: scale each spectrum to its given photopic Y
spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

%% Step 6: use measured spectral xy as the fixed geometry
actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);
actual_xyY.W = spectrum_xyY(spd.W, std, dLambda);

singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda);
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
actual_xyY.SingleWhite = singleMetrics.xyY;
singleWhiteXY = singleMetrics.xyY(1:2);
targetXY = target_xyY(1:2);

rgbPrimary_xyY = [actual_xyY.R; actual_xyY.G; actual_xyY.B];
rgbTriangleXY = rgbPrimary_xyY(:, 1:2);
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', xyY_to_XYZ(actual_xyY.G).', xyY_to_XYZ(actual_xyY.B).'];

%% Step 7: find the ray segment from target to RGB gamut boundary
[boundaryXY, boundaryRayT, boundaryEdge, boundaryEdgeIndex] = ...
    find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
rayTList = linspace(1, boundaryRayT, rayScanCount).';

%% Step 8: pure RGB target endpoint, used as a baseline and plotting reference
[rgbWeights, spd.RGBTarget] = solve_rgb_point_spectrum(target_xyY, rgbSolveMatrix, spd);
rgbMetrics = spectrum_metrics(spd.RGBTarget, std, wl, dLambda);

%% Step 9: metrics of the fixed white baselines
wSheetMetrics = spectrum_metrics(spd.W, std, wl, dLambda);

%% Step 10: scan every RGB point on the single-white -> target -> gamut-boundary ray
n = numel(rayTList);
blendRows = table('Size', [n, 27], ...
    'VariableTypes', [repmat({'double'}, 1, 25), {'string','string'}], ...
    'VariableNames', {'RayT','WhiteXYCoeff','RGBXYCoeff','SingleWhiteRatio','RGBWhiteRatio', ...
    'RGBPoint_x','RGBPoint_y','RGBPoint_Y_lm','R_weight','G_weight','B_weight', ...
    'x','y','Y_lm','CCT_K','Ra','Rmin_8', ...
    'R1','R2','R3','R4','R5','R6','R7','R8','Worst_R','Edge'});

for i = 1:n
    rayT = rayTList(i);
    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);
    split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
    rgbPoint_xyY = [rgbPointXY, split.RGBY];
    [pointWeights, rgbPointSpd] = solve_rgb_point_spectrum(rgbPoint_xyY, rgbSolveMatrix, spd);
    singlePartSpd = split.WhiteYRatio .* singleNorm;
    mixSpd = singlePartSpd + rgbPointSpd;
    m = spectrum_metrics(mixSpd, std, wl, dLambda);

    blendRows.RayT(i) = rayT;
    blendRows.WhiteXYCoeff(i) = split.WhiteXYCoeff;
    blendRows.RGBXYCoeff(i) = split.RGBXYCoeff;
    blendRows.SingleWhiteRatio(i) = split.WhiteYRatio;
    blendRows.RGBWhiteRatio(i) = split.RGBYRatio;
    blendRows.RGBPoint_x(i) = rgbPointXY(1);
    blendRows.RGBPoint_y(i) = rgbPointXY(2);
    blendRows.RGBPoint_Y_lm(i) = split.RGBY;
    blendRows.R_weight(i) = pointWeights(1);
    blendRows.G_weight(i) = pointWeights(2);
    blendRows.B_weight(i) = pointWeights(3);
    blendRows.x(i) = m.xyY(1);
    blendRows.y(i) = m.xyY(2);
    blendRows.Y_lm(i) = m.photopicY;
    blendRows.CCT_K(i) = m.CCT;
    blendRows.Ra(i) = m.Ra;
    blendRows.Rmin_8(i) = m.Rmin8;
    blendRows{i, {'R1','R2','R3','R4','R5','R6','R7','R8'}} = m.Ri(:).';
    blendRows.Worst_R(i) = "R" + string(m.WorstIndex);
    if i == n
        blendRows.Edge(i) = string(boundaryEdge);
    else
        blendRows.Edge(i) = "";
    end
end

%% Step 11: select best blends
[~, idxBestRa] = max(blendRows.Ra);
[~, idxBestRmin] = max(blendRows.Rmin_8);

%% Step 12: export summary tables
weightsTable = table( ...
    ["R"; "G"; "B"], rgbWeights(:), ...
    'VariableNames', {'Channel', 'Scale'});

bestWeightsTable = table( ...
    ["RGB target"; "Best Ra blend"; "Best Rmin8 blend"], ...
    [rgbWeights(1); blendRows.R_weight(idxBestRa); blendRows.R_weight(idxBestRmin)], ...
    [rgbWeights(2); blendRows.G_weight(idxBestRa); blendRows.G_weight(idxBestRmin)], ...
    [rgbWeights(3); blendRows.B_weight(idxBestRa); blendRows.B_weight(idxBestRmin)], ...
    'VariableNames', {'Case','R_weight','G_weight','B_weight'});

actualXyYTable = table( ...
    ["R"; "G"; "B"; "SingleWhite"; "W channel"], ...
    [actual_xyY.R(1); actual_xyY.G(1); actual_xyY.B(1); actual_xyY.SingleWhite(1); actual_xyY.W(1)], ...
    [actual_xyY.R(2); actual_xyY.G(2); actual_xyY.B(2); actual_xyY.SingleWhite(2); actual_xyY.W(2)], ...
    [actual_xyY.R(3); actual_xyY.G(3); actual_xyY.B(3); actual_xyY.SingleWhite(3); actual_xyY.W(3)], ...
    'VariableNames', {'Source','x','y','Y_lm'});

summary = table( ...
    ["Single white fixed spectrum"; "RGB target point"; "W channel"; "Best Ra target blend"; "Best Rmin8 target blend"], ...
    [singleMetrics.xyY(1); rgbMetrics.xyY(1); wSheetMetrics.xyY(1); blendRows.x(idxBestRa); blendRows.x(idxBestRmin)], ...
    [singleMetrics.xyY(2); rgbMetrics.xyY(2); wSheetMetrics.xyY(2); blendRows.y(idxBestRa); blendRows.y(idxBestRmin)], ...
    [singleMetrics.photopicY; rgbMetrics.photopicY; wSheetMetrics.photopicY; blendRows.Y_lm(idxBestRa); blendRows.Y_lm(idxBestRmin)], ...
    [singleMetrics.CCT; rgbMetrics.CCT; wSheetMetrics.CCT; blendRows.CCT_K(idxBestRa); blendRows.CCT_K(idxBestRmin)], ...
    [singleMetrics.Ra; rgbMetrics.Ra; wSheetMetrics.Ra; blendRows.Ra(idxBestRa); blendRows.Ra(idxBestRmin)], ...
    [singleMetrics.Rmin8; rgbMetrics.Rmin8; wSheetMetrics.Rmin8; blendRows.Rmin_8(idxBestRa); blendRows.Rmin_8(idxBestRmin)], ...
    ["-"; "-"; "-"; string(format_blend_note(blendRows(idxBestRa, :))); string(format_blend_note(blendRows(idxBestRmin, :)))], ...
    'VariableNames', {'Case','x','y','Y_lm','CCT_K','Ra','Rmin_8','BlendRatio'});

writetable(weightsTable, fullfile(rootDir, 'guangpu_rgb_target_mix_weights.csv'));
writetable(bestWeightsTable, fullfile(rootDir, 'guangpu_best_rgb_point_mix_weights.csv'));
writetable(actualXyYTable, fullfile(rootDir, 'guangpu_actual_spectral_xyY.csv'));
writetable(blendRows, fullfile(rootDir, 'guangpu_metameric_blend_results.csv'));
writetable(summary, fullfile(rootDir, 'guangpu_cri_summary.csv'));

%% Step 13: export spectra
rgbSpectrumTable = table( ...
    wl, rgbWeights(1) .* spd.R, rgbWeights(2) .* spd.G, rgbWeights(3) .* spd.B, ...
     spd.RGBTarget, singleNorm, ...
     'VariableNames', {'wavelength_nm','R_component_W_nm','G_component_W_nm','B_component_W_nm','S_RGB_target_W_nm','S_single_white_W_nm'});
writetable(rgbSpectrumTable, fullfile(rootDir, 'guangpu_rgb_target_spectrum_0p2nm.csv'));

bestRaSingleSpd = blendRows.SingleWhiteRatio(idxBestRa) .* singleNorm;
bestRaRgbSpd = blendRows.R_weight(idxBestRa) .* spd.R + ...
               blendRows.G_weight(idxBestRa) .* spd.G + ...
               blendRows.B_weight(idxBestRa) .* spd.B;
bestRaSpd = bestRaSingleSpd + bestRaRgbSpd;
bestRminSingleSpd = blendRows.SingleWhiteRatio(idxBestRmin) .* singleNorm;
bestRminRgbSpd = blendRows.R_weight(idxBestRmin) .* spd.R + ...
                 blendRows.G_weight(idxBestRmin) .* spd.G + ...
                 blendRows.B_weight(idxBestRmin) .* spd.B;
bestRminSpd = bestRminSingleSpd + bestRminRgbSpd;
bestBlendSpectrumTable = table( ...
    wl, singleNorm, spd.RGBTarget, bestRaSingleSpd, bestRaRgbSpd, bestRaSpd, ...
    bestRminSingleSpd, bestRminRgbSpd, bestRminSpd, ...
    'VariableNames', {'wavelength_nm','S_single_white_W_nm','S_RGB_target_W_nm', ...
    'S_best_Ra_single_white_W_nm','S_best_Ra_RGB_point_W_nm','S_best_Ra_blend_W_nm', ...
    'S_best_Rmin8_single_white_W_nm','S_best_Rmin8_RGB_point_W_nm','S_best_Rmin8_blend_W_nm'});
writetable(bestBlendSpectrumTable, fullfile(rootDir, 'guangpu_best_blend_spectrum_0p2nm.csv'));

%% Step 14: print key results
fprintf('\nTarget xyY = (%.4f, %.4f, %.4f)\n', target_xyY);
fprintf('Fixed single-white xy = (%.6f, %.6f)\n', singleWhiteXY);
fprintf('Ray boundary = (%.6f, %.6f), RayT=%.6f, Edge=%s, EdgeIndex=%d\n', ...
    boundaryXY(1), boundaryXY(2), boundaryRayT, boundaryEdge, boundaryEdgeIndex);
fprintf('Pure RGB target weights: R=%.12f, G=%.12f, B=%.12f\n', rgbWeights(1), rgbWeights(2), rgbWeights(3));
print_metrics('Single white fixed spectrum', singleMetrics);
print_metrics('RGB target point', rgbMetrics);
print_metrics('W channel', wSheetMetrics);
print_best_row('Best Ra target blend', blendRows(idxBestRa, :));
print_best_row('Best Rmin8 target blend', blendRows(idxBestRmin, :));

function note = format_blend_note(row)
    note = sprintf('rayT=%.4f,singleY=%.3f,rgbY=%.3f,rgbXY=(%.5f,%.5f)', ...
        row.RayT, row.SingleWhiteRatio, row.RGBWhiteRatio, row.RGBPoint_x, row.RGBPoint_y);
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
        error('split_target_on_ray:InvalidTarget', ...
            'Target y is too small for luminance splitting.');
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

function print_best_row(name, row)
    fprintf(['\n%s: rayT=%.6f, singleYRatio=%.4f, RGBYRatio=%.4f, ', ...
        'Ra=%.4f, Rmin8=%.4f, xy=(%.5f, %.5f)\n'], ...
        name, row.RayT, row.SingleWhiteRatio, row.RGBWhiteRatio, ...
        row.Ra, row.Rmin_8, row.x, row.y);
    fprintf('  RGB point xyY=(%.6f, %.6f, %.4f lm)\n', ...
        row.RGBPoint_x, row.RGBPoint_y, row.RGBPoint_Y_lm);
    fprintf('  RGB weights: R=%.12f, G=%.12f, B=%.12f\n', ...
        row.R_weight, row.G_weight, row.B_weight);
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
