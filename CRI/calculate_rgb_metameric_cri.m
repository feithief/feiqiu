% CALCULATE_RGB_METAMERIC_CRI
% 目标：用 RGB 三通道合成目标白光，再与单白灯按比例叠加，计算 R1-R8、Ra 和 Rmin8。
% 说明：重要步骤都写在对应代码旁边，方便和公式图逐步对应。

clear; clc;

%% 步骤 0：定位输入文件和标准数据文件夹
rootDir = fileparts(mfilename('fullpath'));
rgbFile = fullfile(rootDir, 'RGB.xlsx');              % RGB.xlsx: R/G/B/W 四个 sheet 的光谱
singleWhiteFile = fullfile(rootDir, '2.4lm.CSV');     % 2.4lm.CSV: 单白灯光谱
standardDir = fullfile(rootDir, 'standard_data');     % CIE 标准数据保存位置
if ~exist(standardDir, 'dir')
    mkdir(standardDir);
end

%% 步骤 1：输入目标白点和 RGB 三通道的 xyY
target_xyY = [0.3090, 0.3176, 2.39];                  % 客户目标白点 xyY
channel_xyY.R = [0.7013, 0.2979, 1.02];               % 红通道 xyY
channel_xyY.G = [0.1432, 0.7249, 1.8628];             % 绿通道 xyY
channel_xyY.B = [0.1510, 0.0303, 0.35];               % 蓝通道 xyY
channel_xyY.W = [0.3169, 0.3310, 3.2];                % RGB.xlsx 中 W sheet 白光 xyY
single_white_xyY = [0.3090, 0.3176, 2.39];            % 单白灯目标校准 xyY

%% 步骤 2：统一波长范围和步进
wl = (380:5:780).';                                   % 所有光谱统一到 380-780 nm
dLambda = 5;                                          % 光谱步进为 5 nm

%% 步骤 3：加载 CIE 标准数据
std = load_standard_data(standardDir, wl);             % 包含 xbar/ybar/zbar、TCS01-TCS08、日光基函数

%% 步骤 4：读取原始光谱，并插值到统一的 5 nm 波长轴
raw.R = read_spd(rgbFile, 'R', wl);                    % 红通道原始光谱
raw.G = read_spd(rgbFile, 'G', wl);                    % 绿通道原始光谱
raw.B = read_spd(rgbFile, 'B', wl);                    % 蓝通道原始光谱
raw.W = read_spd(rgbFile, 'W', wl);                    % RGB.xlsx 中 W sheet 白光光谱
raw.SingleWhite = read_spd(singleWhiteFile, '', wl);   % 单白灯原始光谱

%% 步骤 5：把每个通道光谱校准到给定光通量 Y
% 这里的 683*sum(S*ybar)*dLambda 是光通量 lm。
% CRI 本身与绝对倍率无关，但同色异谱混光比例需要统一亮度尺度。
spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, wl, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, wl, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, wl, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, wl, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, wl, dLambda);

%% 步骤 6：用 RGB 的 xyY 反解目标白点需要的 R/G/B 权重
% xyY 先转 XYZ，因为 XYZ 可以线性叠加。
% A 的三列分别是 R、G、B 的 XYZ，b 是目标白点 XYZ。
A = [xyY_to_XYZ(channel_xyY.R).', xyY_to_XYZ(channel_xyY.G).', xyY_to_XYZ(channel_xyY.B).'];
b = xyY_to_XYZ(target_xyY).';
rgbWeights = A \ b;                                    % 等价于 q=[r,g,b]^T=A^{-1}b

%% 步骤 7：按权重合成 RGB 目标白光光谱
spd.RGBTarget = rgbWeights(1) .* spd.R + ...
                rgbWeights(2) .* spd.G + ...
                rgbWeights(3) .* spd.B;                % S_RGB0 = r*S_R + g*S_G + b*S_B
spd.RGBTarget = scale_to_photopic_Y(spd.RGBTarget, target_xyY(3), std, wl, dLambda);

%% 步骤 8：把单白灯和 RGB 目标白光都归一到同一亮度
% 后面 p 才能表示“同亮度白光之间的混合比例”。
% p=1 表示纯单白灯，p=0 表示纯 RGB 目标白光。
singleNorm = scale_to_photopic_Y(spd.SingleWhite, target_xyY(3), std, wl, dLambda);
rgbNorm = scale_to_photopic_Y(spd.RGBTarget, target_xyY(3), std, wl, dLambda);

%% 步骤 9：先分别计算三个基准光谱的指标
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);   % 单白灯指标
rgbMetrics = spectrum_metrics(rgbNorm, std, wl, dLambda);         % RGB 目标白光指标
wSheetMetrics = spectrum_metrics(spd.W, std, wl, dLambda);        % RGB.xlsx W sheet 指标

%% 步骤 10：扫描单白灯和 RGB 白光叠加比例
pList = (0:0.001:1).';                                  % 扫描步长 0.001
n = numel(pList);
blendRows = table('Size', [n, 17], ...
    'VariableTypes', [{'double','double','double','double','double','double','double','double'}, repmat({'double'}, 1, 8), {'string'}], ...
    'VariableNames', {'SingleWhiteRatio','RGBWhiteRatio','x','y','Y_lm','CCT_K','Ra','Rmin_8', ...
    'R1','R2','R3','R4','R5','R6','R7','R8','Worst_R'});

for i = 1:n
    p = pList(i);                                      % p 是单白灯比例
    mixSpd = p .* singleNorm + (1 - p) .* rgbNorm;     % S_mix = p*S_single + (1-p)*S_RGB
    m = spectrum_metrics(mixSpd, std, wl, dLambda);    % 每一个比例都完整计算 xy、CCT、R1-R8、Ra

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

%% 步骤 11：筛选最优组合
[~, idxBestRa] = max(blendRows.Ra);                    % Ra 最大的比例
[~, idxBestRmin] = max(blendRows.Rmin_8);              % R1-R8 中最小值最大的比例

%% 步骤 12：整理输出表格
weightsTable = table( ...
    ["R"; "G"; "B"], rgbWeights(:), ...
    'VariableNames', {'Channel', 'Scale'});             % RGB 合成目标白光的通道权重

summary = table( ...
    ["Single white 2.4lm"; "RGB target white"; "RGB.xlsx W sheet"; "Best Ra blend"; "Best Rmin8 blend"], ...
    [singleMetrics.xyY(1); rgbMetrics.xyY(1); wSheetMetrics.xyY(1); blendRows.x(idxBestRa); blendRows.x(idxBestRmin)], ...
    [singleMetrics.xyY(2); rgbMetrics.xyY(2); wSheetMetrics.xyY(2); blendRows.y(idxBestRa); blendRows.y(idxBestRmin)], ...
    [singleMetrics.photopicY; rgbMetrics.photopicY; wSheetMetrics.photopicY; blendRows.Y_lm(idxBestRa); blendRows.Y_lm(idxBestRmin)], ...
    [singleMetrics.CCT; rgbMetrics.CCT; wSheetMetrics.CCT; blendRows.CCT_K(idxBestRa); blendRows.CCT_K(idxBestRmin)], ...
    [singleMetrics.Ra; rgbMetrics.Ra; wSheetMetrics.Ra; blendRows.Ra(idxBestRa); blendRows.Ra(idxBestRmin)], ...
    [singleMetrics.Rmin8; rgbMetrics.Rmin8; wSheetMetrics.Rmin8; blendRows.Rmin_8(idxBestRa); blendRows.Rmin_8(idxBestRmin)], ...
    ["-"; "-"; "-"; sprintf('single=%.3f,rgb=%.3f', blendRows.SingleWhiteRatio(idxBestRa), blendRows.RGBWhiteRatio(idxBestRa)); sprintf('single=%.3f,rgb=%.3f', blendRows.SingleWhiteRatio(idxBestRmin), blendRows.RGBWhiteRatio(idxBestRmin))], ...
    'VariableNames', {'Case','x','y','Y_lm','CCT_K','Ra','Rmin_8','BlendRatio'});

writetable(weightsTable, fullfile(rootDir, 'rgb_target_mix_weights.csv'));      % 输出 RGB 权重
writetable(blendRows, fullfile(rootDir, 'metameric_blend_results.csv'));        % 输出所有比例扫描结果
writetable(summary, fullfile(rootDir, 'cri_summary.csv'));                      % 输出关键结果汇总

%% 步骤 13：输出 RGB 目标白光和最佳合成光谱
rgbSpectrumTable = table( ...
    wl, rgbWeights(1) .* spd.R, rgbWeights(2) .* spd.G, rgbWeights(3) .* spd.B, ...
    spd.RGBTarget, singleNorm, ...
    'VariableNames', {'wavelength_nm','R_component_W_nm','G_component_W_nm','B_component_W_nm','S_RGB_target_W_nm','S_single_white_W_nm'});
writetable(rgbSpectrumTable, fullfile(rootDir, 'rgb_target_spectrum_5nm.csv'));

bestRaSpd = blendRows.SingleWhiteRatio(idxBestRa) .* singleNorm + blendRows.RGBWhiteRatio(idxBestRa) .* rgbNorm;
bestRminSpd = blendRows.SingleWhiteRatio(idxBestRmin) .* singleNorm + blendRows.RGBWhiteRatio(idxBestRmin) .* rgbNorm;
bestBlendSpectrumTable = table( ...
    wl, singleNorm, rgbNorm, bestRaSpd, bestRminSpd, ...
    'VariableNames', {'wavelength_nm','S_single_white_W_nm','S_RGB_target_W_nm','S_best_Ra_blend_W_nm','S_best_Rmin8_blend_W_nm'});
writetable(bestBlendSpectrumTable, fullfile(rootDir, 'best_blend_spectrum_5nm.csv'));

%% 步骤 14：在命令行打印关键结果，便于快速检查
fprintf('\nRGB target xyY = (%.4f, %.4f, %.4f)\n', target_xyY);
fprintf('RGB mix weights: R=%.12f, G=%.12f, B=%.12f\n', rgbWeights(1), rgbWeights(2), rgbWeights(3));
print_metrics('Single white 2.4lm', singleMetrics);
print_metrics('RGB target white', rgbMetrics);
print_metrics('RGB.xlsx W sheet', wSheetMetrics);
fprintf('\nBest Ra blend: single=%.3f, RGB=%.3f, Ra=%.4f, Rmin8=%.4f, xy=(%.5f, %.5f)\n', ...
    blendRows.SingleWhiteRatio(idxBestRa), blendRows.RGBWhiteRatio(idxBestRa), ...
    blendRows.Ra(idxBestRa), blendRows.Rmin_8(idxBestRa), blendRows.x(idxBestRa), blendRows.y(idxBestRa));
fprintf('Best Rmin8 blend: single=%.3f, RGB=%.3f, Ra=%.4f, Rmin8=%.4f, xy=(%.5f, %.5f)\n', ...
    blendRows.SingleWhiteRatio(idxBestRmin), blendRows.RGBWhiteRatio(idxBestRmin), ...
    blendRows.Ra(idxBestRmin), blendRows.Rmin_8(idxBestRmin), blendRows.x(idxBestRmin), blendRows.y(idxBestRmin));

%% 本文件下面都是局部函数，主流程不需要改这里

function std = load_standard_data(standardDir, wl)
    % 加载 CIE 标准数据，并统一插值到本项目使用的 5 nm 波长轴。
    % cmf: CIE 1931 2° xbar/ybar/zbar
    % tcs: CIE CRI TCS01-TCS08 标准色样反射率
    % daylightComponents: CIE 日光参考光源基函数
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
    % 如果标准数据不存在，就从 CIE 官方地址下载一次。
    path = fullfile(folder, fileName);
    if ~exist(path, 'file')
        websave(path, url);
    end
end

function spd = read_spd(fileName, sheetName, wlOut)
    % 读取光谱文件，并把原始 0.2 nm 等间隔数据插值到 5 nm。
    if isempty(sheetName)
        M = readmatrix(fileName);
    else
        M = readmatrix(fileName, 'Sheet', sheetName);
    end
    wl = M(:,1);
    if size(M, 2) >= 3
        s = M(:,3);                                    % 优先使用 Spectrum(W/nm)
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
    % 把 xyY 转成可线性叠加的 XYZ。
    x = xyY(1);
    y = xyY(2);
    Y = xyY(3);
    X = x / y * Y;
    Z = (1 - x - y) / y * Y;
    XYZ = [X, Y, Z];
end

function spdScaled = scale_to_photopic_Y(spd, targetYlm, std, wl, dLambda)
    % 把光谱按目标光通量 Y(lm) 做整体缩放。
    % Ylm = 683 * integral(S*ybar)
    Ylm = 683 * sum(spd(:) .* std.cmf(:,2)) * dLambda;
    if Ylm <= 0
        error('光谱的 photopic Y 为 0，不能归一化。');
    end
    spdScaled = spd(:) .* (targetYlm / Ylm);
end

function m = spectrum_metrics(spd, std, wl, dLambda)
    % 一个光谱进入这里后，会计算 xy、Y、CCT、Duv、R1-R8、Ra、Rmin8。
    XYZ = source_XYZ(spd, std.cmf, dLambda);           % 光源本身 XYZ
    xyY = XYZ_to_xyY(XYZ);                             % 光源白点 xy
    photopicY = 683 * XYZ(2);                          % 光通量 lm
    uv = XYZ_to_uv1960(XYZ);                           % CIE 1960 uv，用于估算 CCT
    [CCT, Duv] = estimate_cct_uv(uv, std.cmf, wl, dLambda);
    refSpd = reference_spd(CCT, std, wl);              % 根据 CCT 生成参考光源
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
    % 光谱到 XYZ 的 5 nm 离散求和。
    XYZ = sum(spd(:) .* cmf, 1) .* dLambda;
end

function xyY = XYZ_to_xyY(XYZ)
    % XYZ 转 xyY，其中第三个值保留 Y。
    s = sum(XYZ);
    if s <= 0
        xyY = [NaN, NaN, NaN];
    else
        xyY = [XYZ(1) / s, XYZ(2) / s, XYZ(2)];
    end
end

function uv = XYZ_to_uv1960(XYZ)
    % XYZ 转 CIE 1960 uv。
    den = XYZ(1) + 15 * XYZ(2) + 3 * XYZ(3);
    uv = [4 * XYZ(1) / den, 6 * XYZ(2) / den];
end

function [CCT, Duv] = estimate_cct_uv(uv, cmf, wl, dLambda)
    % 用黑体轨迹搜索近似 CCT，并用最小 uv 距离作为 Duv 的近似值。
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
    % 计算某个温度黑体光源与测试光源 uv 的距离平方。
    bb = blackbody_spd(T, wl);
    bbXYZ = source_XYZ(bb, cmf, dLambda);
    bbuv = XYZ_to_uv1960(bbXYZ);
    d2 = sum((uv - bbuv).^2);
end

function spd = blackbody_spd(T, wl)
    % 生成相对黑体光谱，用于 CCT<5000 K 的参考光源。
    c2 = 1.438776877e-2;                               % m*K
    lambda = wl(:) * 1e-9;
    spd = 1 ./ (lambda .^ 5 .* (exp(c2 ./ (lambda .* T)) - 1));
    spd = spd ./ max(spd);
end

function spd = reference_spd(CCT, std, wl)
    % CRI 参考光源规则：低于 5000 K 用黑体，高于等于 5000 K 用 CIE 日光。
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
    % 计算 R1-R8：测试光源和参考光源分别照标准色样，再比较 UVW 色差。
    testXYZ = source_XYZ(testSpd, cmf, dLambda);
    refXYZ = source_XYZ(refSpd, cmf, dLambda);
    testUV = XYZ_to_uv1960(testXYZ);
    refUV = XYZ_to_uv1960(refXYZ);

    testUVW = tcs_uvw(testSpd, cmf, tcs, dLambda, testUV, refUV, true);
    refUVW = tcs_uvw(refSpd, cmf, tcs, dLambda, refUV, refUV, false);

    Ri = zeros(8, 1);
    for i = 1:8
        deltaE = norm(refUVW(i,:) - testUVW(i,:));     % UVW 空间色差
        Ri(i) = 100 - 4.6 * deltaE;                    % 单个色样显色指数
    end
end

function UVW = tcs_uvw(sourceSpd, cmf, tcs, dLambda, sourceUV, refUV, doAdapt)
    % 标准色样反射光谱 -> XYZ -> uv 色适应 -> UVW。
    sourceY = sum(sourceSpd(:) .* cmf(:,2)) * dLambda;
    k = 100 / sourceY;                                 % 把光源白场 Y 归一到 100
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
    % 命令行输出一个光谱的核心指标，方便人工核对。
    fprintf('\n%s\n', name);
    fprintf('  xyY=(%.5f, %.5f, %.4f lm), CCT=%.1f K\n', m.xyY(1), m.xyY(2), m.xyY(3), m.CCT);
    fprintf('  Ra=%.4f, Rmin8=%.4f, worst=R%d\n', m.Ra, m.Rmin8, m.WorstIndex);
    fprintf('  R1-R8: %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n', m.Ri);
end
