% CALC_PURE_RGB_RA_ONLY_CN_V2
% 中文说明：
% 只额外计算“纯RGB亮、单白光不亮”时的 Ra。
% 不修改任何已有脚本。
%
% 纯RGB定义：
%   white Y = 0
%   RGB 等效点 = 当前目标色 target_xyY
%   直接由 R/G/B 三通道解出目标色，并计算该 RGB 光谱的 Ra。
%
% 输出：
%   triangle_grid_lut_output/pure_rgb_ra_only_results.csv

clear;
clc;

%% ================= 用户可配置参数 =================
target_xyY_list = [
    0.3169, 0.3310, 3.2310;
    0.3197, 0.3497, 3.1980;
    0.4016, 0.5034, 2.9180;
    0.4165, 0.4966, 2.8880;
    0.4310, 0.4900, 2.8510;
    0.4576, 0.4619, 2.7920;
    0.4959, 0.4449, 2.6830;
    0.5289, 0.4162, 2.5930;
    0.5532, 0.3927, 2.5130;
    0.5897, 0.3655, 2.3750;
    0.6846, 0.3072, 1.9420;
    0.6540, 0.2906, 1.9070;
    0.6282, 0.2821, 1.9150;
    0.5211, 0.2316, 1.8350;
    0.2082, 0.0680, 1.2650;
    0.1486, 0.0288, 0.5840;
    0.1579, 0.1053, 1.7290;
    0.1586, 0.1409, 1.9570;
    0.1580, 0.1664, 2.0940;
    0.1609, 0.2060, 2.2930;
    0.1667, 0.2407, 2.4490;
    0.1667, 0.2828, 2.5860;
    0.1766, 0.3402, 2.7720;
    0.1800, 0.3800, 2.8720;
    0.1808, 0.4144, 2.9310;
    0.1861, 0.5109, 3.1060;
    0.1934, 0.6418, 3.2690;
    0.2368, 0.6205, 3.2180;
    0.3092, 0.5649, 3.1110;
    0.3696, 0.5362, 3.0010];

outputFileName = 'pure_rgb_ra_only_results.csv';

%% ================= 定位工程目录 =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

addpath(inputDir);
caseCfg = guangpu_case_config();

if isfield(caseCfg, 'inputDir') && ~isempty(caseCfg.inputDir)
    spectraDir = caseCfg.inputDir;
else
    spectraDir = inputDir;
end

%% ================= 一次性初始化 =================
wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;
channel_xyY = caseCfg.channel_xyY;

std = load_standard_data(standardDir, wl);

raw.R = read_spd(fullfile(spectraDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(spectraDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(spectraDir, 'B.CSV'), '', wl);

spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);

actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);

rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', ...
                  xyY_to_XYZ(actual_xyY.G).', ...
                  xyY_to_XYZ(actual_xyY.B).'];

%% ================= 逐目标点计算纯RGB Ra =================
nTarget = size(target_xyY_list, 1);

Index = (1:nTarget).';
Target_x = target_xyY_list(:, 1);
Target_y = target_xyY_list(:, 2);
Target_Y = target_xyY_list(:, 3);

PureRGB_Valid = zeros(nTarget, 1);
PureRGB_Ra = nan(nTarget, 1);
PureRGB_Rmin8 = nan(nTarget, 1);
PureRGB_WorstR = strings(nTarget, 1);
PureRGB_R_weight = nan(nTarget, 1);
PureRGB_G_weight = nan(nTarget, 1);
PureRGB_B_weight = nan(nTarget, 1);
PureRGB_x = nan(nTarget, 1);
PureRGB_y = nan(nTarget, 1);
PureRGB_Y = nan(nTarget, 1);
PureRGB_CCT = nan(nTarget, 1);

fprintf('\n开始计算纯RGB Ra，共 %d 个目标颜色点...\n', nTarget);

for k = 1:nTarget
    target_xyY = target_xyY_list(k, :);

    fprintf('处理中 %02d/%02d: target=(%.4f, %.4f, %.4f)\n', ...
        k, nTarget, target_xyY(1), target_xyY(2), target_xyY(3));

    try
        [weights, rgbSpd] = solve_rgb_point_spectrum(target_xyY, rgbSolveMatrix, spd);
        m = spectrum_metrics(rgbSpd, std, wl, dLambda);

        PureRGB_Valid(k) = 1;
        PureRGB_Ra(k) = m.Ra;
        PureRGB_Rmin8(k) = m.Rmin8;
        PureRGB_WorstR(k) = "R" + string(m.WorstIndex);
        PureRGB_R_weight(k) = weights(1);
        PureRGB_G_weight(k) = weights(2);
        PureRGB_B_weight(k) = weights(3);
        PureRGB_x(k) = m.xyY(1);
        PureRGB_y(k) = m.xyY(2);
        PureRGB_Y(k) = m.xyY(3);
        PureRGB_CCT(k) = m.CCT;
    catch ME
        PureRGB_Valid(k) = 0;
        PureRGB_WorstR(k) = "Invalid";
        fprintf('  目标点 %d 纯RGB计算失败：%s\n', k, ME.message);
    end
end

PureRgbTable = table( ...
    Index, Target_x, Target_y, Target_Y, ...
    PureRGB_Valid, PureRGB_Ra, PureRGB_Rmin8, PureRGB_WorstR, ...
    PureRGB_R_weight, PureRGB_G_weight, PureRGB_B_weight, ...
    PureRGB_x, PureRGB_y, PureRGB_Y, PureRGB_CCT);

outputFile = fullfile(outputDir, outputFileName);

try
    writetable(PureRgbTable, outputFile);
catch ME
    fprintf('\n写入失败：%s\n', outputFile);
    fprintf('常见原因：CSV正在被Excel/WPS打开。\n');
    fprintf('错误信息：%s\n', ME.message);
    outputFile = fullfile(outputDir, ['pure_rgb_ra_only_results_' datestr(now, 'yyyymmdd_HHMMSS') '.csv']);
    writetable(PureRgbTable, outputFile);
end

fprintf('\n================ 纯RGB Ra 结果 ================\n');
disp(PureRgbTable(:, {'Index','Target_x','Target_y','Target_Y','PureRGB_Valid','PureRGB_Ra','PureRGB_Rmin8','PureRGB_R_weight','PureRGB_G_weight','PureRGB_B_weight'}));
fprintf('\n纯RGB Ra结果已导出：\n%s\n', outputFile);

% 用 msgbox 显示摘要，避免 uitable 在部分 MATLAB 环境下出现空白窗口。
validCount = sum(PureRGB_Valid);
summaryText = sprintf(['纯RGB Ra计算完成。\n\n' ...
                       '目标点数量：%d\n' ...
                       '有效纯RGB点数量：%d\n' ...
                       '结果文件：\n%s\n\n' ...
                       '完整数据请查看命令行表格或CSV文件。'], ...
                       nTarget, validCount, outputFile);
try
    msgbox(summaryText, '纯RGB Ra计算结果', 'help');
catch
end

%% ================= Local functions =================
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

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xy = XYZ_to_xyY(XYZ);
    xyY = [xy(1), xy(2), 683 * XYZ(2)];
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
    if ~exist(folder, 'dir')
        mkdir(folder);
    end

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

    if y <= 0
        error('xyY_to_XYZ:InvalidY', 'xyY y is zero or negative.');
    end

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
