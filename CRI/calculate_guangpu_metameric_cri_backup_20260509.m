% CALCULATE_GUANGPU_METAMERIC_CRI
% Same workflow as calculate_rgb_metameric_cri.m.
% Only the input spectra source and wavelength step are changed to guangpu + 0.2 nm.

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
target_xyY = caseCfg.target_xyY;
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

%% Step 5: scale each spectrum to its given photopic Y
spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

%% Step 6: solve RGB weights for the target white
A = [xyY_to_XYZ(channel_xyY.R).', xyY_to_XYZ(channel_xyY.G).', xyY_to_XYZ(channel_xyY.B).'];
b = xyY_to_XYZ(target_xyY).';
rgbWeights = A \ b;

%% Step 7: synthesize RGB target white spectrum
spd.RGBTarget = rgbWeights(1) .* spd.R + ...
                rgbWeights(2) .* spd.G + ...
                rgbWeights(3) .* spd.B;
spd.RGBTarget = scale_to_photopic_Y(spd.RGBTarget, target_xyY(3), std, wl, dLambda);

%% Step 8: normalize W and RGB white to the same luminance
singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda);
rgbNorm = scale_to_photopic_Y(spd.RGBTarget, target_xyY(3), std, wl, dLambda);

%% Step 9: metrics of the three baseline spectra
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
rgbMetrics = spectrum_metrics(rgbNorm, std, wl, dLambda);
wSheetMetrics = spectrum_metrics(spd.W, std, wl, dLambda);

%% Step 10: scan W/RGB blend ratio
pList = (0:0.001:1).';
n = numel(pList);
blendRows = table('Size', [n, 17], ...
    'VariableTypes', [{'double','double','double','double','double','double','double','double'}, repmat({'double'}, 1, 8), {'string'}], ...
    'VariableNames', {'SingleWhiteRatio','RGBWhiteRatio','x','y','Y_lm','CCT_K','Ra','Rmin_8', ...
    'R1','R2','R3','R4','R5','R6','R7','R8','Worst_R'});

for i = 1:n
    p = pList(i);
    mixSpd = p .* singleNorm + (1 - p) .* rgbNorm;
    m = spectrum_metrics(mixSpd, std, wl, dLambda);

    blendRows.SingleWhiteRatio(i) = p;
    blendRows.RGBWhiteRatio(i) = 1 - p;
    blendRows.x(i) = m.xyY(1);
    blendRows.y(i) = m.xyY(2);
    blendRows.Y_lm(i) = m.photopicY;
    blendRows.CCT_K(i) = m.CCT;
    blendRows.Ra(i) = m.Ra;
    blendRows.Rmin_8(i) = m.Rmin8;
    blendRows{i, {'R1','R2','R3','R4','R5','R6','R7','R8'}} = m.Ri(:).';
    blendRows.Worst_R(i) = "R" + string(m.WorstIndex);
end

%% Step 11: select best blends
[~, idxBestRa] = max(blendRows.Ra);
[~, idxBestRmin] = max(blendRows.Rmin_8);

%% Step 12: export summary tables
weightsTable = table( ...
    ["R"; "G"; "B"], rgbWeights(:), ...
    'VariableNames', {'Channel', 'Scale'});

summary = table( ...
    ["Single white (W channel)"; "RGB target white"; "W channel"; "Best Ra blend"; "Best Rmin8 blend"], ...
    [singleMetrics.xyY(1); rgbMetrics.xyY(1); wSheetMetrics.xyY(1); blendRows.x(idxBestRa); blendRows.x(idxBestRmin)], ...
    [singleMetrics.xyY(2); rgbMetrics.xyY(2); wSheetMetrics.xyY(2); blendRows.y(idxBestRa); blendRows.y(idxBestRmin)], ...
    [singleMetrics.photopicY; rgbMetrics.photopicY; wSheetMetrics.photopicY; blendRows.Y_lm(idxBestRa); blendRows.Y_lm(idxBestRmin)], ...
    [singleMetrics.CCT; rgbMetrics.CCT; wSheetMetrics.CCT; blendRows.CCT_K(idxBestRa); blendRows.CCT_K(idxBestRmin)], ...
    [singleMetrics.Ra; rgbMetrics.Ra; wSheetMetrics.Ra; blendRows.Ra(idxBestRa); blendRows.Ra(idxBestRmin)], ...
    [singleMetrics.Rmin8; rgbMetrics.Rmin8; wSheetMetrics.Rmin8; blendRows.Rmin_8(idxBestRa); blendRows.Rmin_8(idxBestRmin)], ...
    ["-"; "-"; "-"; sprintf('single=%.3f,rgb=%.3f', blendRows.SingleWhiteRatio(idxBestRa), blendRows.RGBWhiteRatio(idxBestRa)); sprintf('single=%.3f,rgb=%.3f', blendRows.SingleWhiteRatio(idxBestRmin), blendRows.RGBWhiteRatio(idxBestRmin))], ...
    'VariableNames', {'Case','x','y','Y_lm','CCT_K','Ra','Rmin_8','BlendRatio'});

writetable(weightsTable, fullfile(rootDir, 'guangpu_rgb_target_mix_weights.csv'));
writetable(blendRows, fullfile(rootDir, 'guangpu_metameric_blend_results.csv'));
writetable(summary, fullfile(rootDir, 'guangpu_cri_summary.csv'));

%% Step 13: export spectra
rgbSpectrumTable = table( ...
    wl, rgbWeights(1) .* spd.R, rgbWeights(2) .* spd.G, rgbWeights(3) .* spd.B, ...
    spd.RGBTarget, singleNorm, ...
    'VariableNames', {'wavelength_nm','R_component_W_nm','G_component_W_nm','B_component_W_nm','S_RGB_target_W_nm','S_single_white_W_nm'});
writetable(rgbSpectrumTable, fullfile(rootDir, 'guangpu_rgb_target_spectrum_0p2nm.csv'));

bestRaSpd = blendRows.SingleWhiteRatio(idxBestRa) .* singleNorm + blendRows.RGBWhiteRatio(idxBestRa) .* rgbNorm;
bestRminSpd = blendRows.SingleWhiteRatio(idxBestRmin) .* singleNorm + blendRows.RGBWhiteRatio(idxBestRmin) .* rgbNorm;
bestBlendSpectrumTable = table( ...
    wl, singleNorm, rgbNorm, bestRaSpd, bestRminSpd, ...
    'VariableNames', {'wavelength_nm','S_single_white_W_nm','S_RGB_target_W_nm','S_best_Ra_blend_W_nm','S_best_Rmin8_blend_W_nm'});
writetable(bestBlendSpectrumTable, fullfile(rootDir, 'guangpu_best_blend_spectrum_0p2nm.csv'));

%% Step 14: print key results
fprintf('\nRGB target xyY = (%.4f, %.4f, %.4f)\n', target_xyY);
fprintf('RGB mix weights: R=%.12f, G=%.12f, B=%.12f\n', rgbWeights(1), rgbWeights(2), rgbWeights(3));
print_metrics('Single white (W channel)', singleMetrics);
print_metrics('RGB target white', rgbMetrics);
print_metrics('W channel', wSheetMetrics);
fprintf('\nBest Ra blend: single=%.3f, RGB=%.3f, Ra=%.4f, Rmin8=%.4f, xy=(%.5f, %.5f)\n', ...
    blendRows.SingleWhiteRatio(idxBestRa), blendRows.RGBWhiteRatio(idxBestRa), ...
    blendRows.Ra(idxBestRa), blendRows.Rmin_8(idxBestRa), blendRows.x(idxBestRa), blendRows.y(idxBestRa));
fprintf('Best Rmin8 blend: single=%.3f, RGB=%.3f, Ra=%.4f, Rmin8=%.4f, xy=(%.5f, %.5f)\n', ...
    blendRows.SingleWhiteRatio(idxBestRmin), blendRows.RGBWhiteRatio(idxBestRmin), ...
    blendRows.Ra(idxBestRmin), blendRows.Rmin_8(idxBestRmin), blendRows.x(idxBestRmin), blendRows.y(idxBestRmin));

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
