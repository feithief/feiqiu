%GENERATE_CRI_SPECTRUM_FIGURES
% 生成原始 RGB 光谱、单白灯光谱、合成光谱，以及与代码一致的数学模型图。

clear; clc; close all;

cfg = figure_config();
data = load_report_data(cfg);
model = load_report_model(cfg);
cleanup_legacy_outputs(cfg);

plot_input_spectra(data, cfg);
plot_synthesized(data, model, cfg);
plot_formula_page_1(data, model, cfg);
plot_formula_page_2(cfg);
plot_formula_page_3(model, cfg);

disp("图片已输出到: " + cfg.outDir);

function cfg = figure_config()
    cfg.rootDir = fileparts(mfilename('fullpath'));
    cfg.outDir = fullfile(cfg.rootDir, 'figures');
    cfg.rgbFile = fullfile(cfg.rootDir, 'RGB.xlsx');
    cfg.singleWhiteFile = fullfile(cfg.rootDir, '2.4lm.CSV');
    cfg.standardDir = fullfile(cfg.rootDir, 'standard_data');
    cfg.rgbToolDir = fullfile(cfg.rootDir, '..', 'RGB');

    cfg.rgbTargetCsv = fullfile(cfg.rootDir, 'rgb_target_spectrum_5nm.csv');
    cfg.bestBlendCsv = fullfile(cfg.rootDir, 'best_blend_spectrum_5nm.csv');
    cfg.blendRowsCsv = fullfile(cfg.rootDir, 'metameric_blend_results.csv');
    cfg.summaryCsv = fullfile(cfg.rootDir, 'cri_summary.csv');
    cfg.weightsCsv = fullfile(cfg.rootDir, 'rgb_target_mix_weights.csv');

    cfg.wavelength = (380:5:780).';
    cfg.dLambda = 5;
    cfg.target_xyY = [0.3090, 0.3176, 2.39];
    cfg.R_xyY = [0.7013, 0.2979, 1.02];
    cfg.G_xyY = [0.1432, 0.7249, 1.8628];
    cfg.B_xyY = [0.1510, 0.0303, 0.35];
    cfg.W_xyY = [0.3169, 0.3310, 3.2];
    cfg.singleWhite_xyY = [0.3090, 0.3176, 2.39];

    cfg.fontName = choose_font();
    if ~exist(cfg.outDir, 'dir')
        mkdir(cfg.outDir);
    end
end

function data = load_report_data(cfg)
    require_file(cfg.rgbTargetCsv);
    require_file(cfg.bestBlendCsv);

    wl = cfg.wavelength;
    data.wavelength = wl;
    data.rawR = read_spd(cfg.rgbFile, 'R', wl);
    data.rawG = read_spd(cfg.rgbFile, 'G', wl);
    data.rawB = read_spd(cfg.rgbFile, 'B', wl);
    data.rawW = read_spd(cfg.rgbFile, 'W', wl);
    data.rawSingleWhite = read_spd(cfg.singleWhiteFile, '', wl);
    data.cmf = read_cmf(cfg, wl);

    T = readtable(cfg.rgbTargetCsv);
    data.R_component = T.R_component_W_nm;
    data.G_component = T.G_component_W_nm;
    data.B_component = T.B_component_W_nm;
    data.rgbTarget = T.S_RGB_target_W_nm;
    data.singleWhite = T.S_single_white_W_nm;

    B = readtable(cfg.bestBlendCsv);
    data.bestRaBlend = B.S_best_Ra_blend_W_nm;
    data.bestRminBlend = B.S_best_Rmin8_blend_W_nm;
end

function model = load_report_model(cfg)
    require_file(cfg.summaryCsv);
    require_file(cfg.weightsCsv);
    require_file(cfg.blendRowsCsv);

    W = readtable(cfg.weightsCsv);
    model.rgbWeight = W.Scale(:).';
    model.rgb32000 = round(model.rgbWeight ./ max(model.rgbWeight) .* 32000);

    primaries = [cfg.R_xyY; cfg.G_xyY; cfg.B_xyY];
    addpath(cfg.rgbToolDir);
    xyY32000 = RGB_to_xyY_32000(model.rgb32000, primaries);
    model.rgb32000_xy = xyY32000(1, 1:2);

    model.summary = readtable(cfg.summaryCsv, 'TextType', 'string');
    model.blendRows = readtable(cfg.blendRowsCsv);
    [~, model.idxBestRa] = max(model.blendRows.Ra);
    [~, model.idxBestRmin] = max(model.blendRows.Rmin_8);
end

function cleanup_legacy_outputs(cfg)
    legacyBaseNames = {
        '01_original_rgb_spectrum'
        '02_single_white_spectrum'
        '03_synthesized_spectrum'
        '04_formula_page1_human_functions'
        '05_formula_page2_tristimulus_original'
        '06_formula_page3_rgb_solve_xy_check'
        '07_formula_page4_spectrum_xyz'
        '08_formula_page5_cri_chain'
        '09_formula_page6_results'
        };
    extensions = {'.png', '.pdf', '.svg', '.fig'};
    for i = 1:numel(legacyBaseNames)
        for j = 1:numel(extensions)
            fileName = fullfile(cfg.outDir, [legacyBaseNames{i} extensions{j}]);
            if exist(fileName, 'file')
                try
                    delete(fileName);
                catch
                    warning('无法删除旧输出文件: %s', fileName);
                end
            end
        end
    end
end

function plot_input_spectra(data, cfg)
    f = make_figure([100 100 1800 1100]);
    t = tiledlayout(2, 1, 'TileSpacing', 'compact', 'Padding', 'compact');

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.rawR), 'r', 'LineWidth', 2.0);
    plot(data.wavelength, normalize_curve(data.rawG), 'g', 'LineWidth', 2.0);
    plot(data.wavelength, normalize_curve(data.rawB), 'b', 'LineWidth', 2.0);
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 12);
    title('RGB.xlsx 输入的 R/G/B 原始光谱', 'FontName', cfg.fontName, ...
        'FontSize', 16, 'FontWeight', 'bold');
    legend({'R channel','G channel','B channel'}, 'Location', 'northeast', 'FontSize', 10.5);
    xlim([380 780]);
    ylim([0 1.08]);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.rawW), 'Color', [0.10 0.36 0.72], 'LineWidth', 2.0);
    plot(data.wavelength, normalize_curve(data.rawSingleWhite), 'Color', [0.08 0.08 0.08], 'LineWidth', 2.0);
    xlabel('Wavelength  \lambda  / nm', 'FontName', cfg.fontName, 'FontSize', 12);
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 12);
    title('RGB.xlsx W sheet 与单白灯输入光谱', 'FontName', cfg.fontName, ...
        'FontSize', 16, 'FontWeight', 'bold');
    legend({'RGB.xlsx W sheet','Single white 2.4lm'}, 'Location', 'northeast', 'FontSize', 10.5);
    xlim([380 780]);
    ylim([0 1.08]);

    title(t, '输入光谱总览', 'FontName', cfg.fontName, 'FontSize', 20, 'FontWeight', 'bold');
    export_all(f, cfg, '01_input_spectra');
end

function plot_synthesized(data, model, cfg)
    f = make_figure([100 80 1900 1300]);
    t = tiledlayout(2, 2, 'TileSpacing', 'compact', 'Padding', 'compact');
    bestRa = model.blendRows(model.idxBestRa, :);
    bestRmin = model.blendRows(model.idxBestRmin, :);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.R_component), 'r', 'LineWidth', 1.45);
    plot(data.wavelength, normalize_curve(data.G_component), 'g', 'LineWidth', 1.45);
    plot(data.wavelength, normalize_curve(data.B_component), 'b', 'LineWidth', 1.45);
    plot(data.wavelength, normalize_curve(data.rgbTarget), 'k', 'LineWidth', 2.1);
    title(sprintf('RGB 目标白光: r=%.6f, g=%.6f, b=%.6f', model.rgbWeight), ...
        'FontName', cfg.fontName, 'FontSize', 13.5, 'FontWeight', 'bold');
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 10.5);
    legend({'rS_R','gS_G','bS_B','S_{RGB}'}, 'Location', 'northeast', 'FontSize', 9.5);
    xlim([380 780]);
    ylim([0 1.08]);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.singleWhite), 'Color', [0.35 0.35 0.35], 'LineWidth', 1.5);
    plot(data.wavelength, normalize_curve(data.rgbTarget), 'Color', [0 0.30 0.85], 'LineWidth', 1.5);
    plot(data.wavelength, normalize_curve(data.bestRaBlend), 'Color', [0.85 0.15 0], 'LineWidth', 2.0);
    plot(data.wavelength, normalize_curve(data.bestRminBlend), 'Color', [0.12 0.62 0.38], 'LineWidth', 2.0);
    title('单白灯、RGB 白光与最优混合光谱', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'Single white','RGB target','Best Ra','Best Rmin8'}, ...
        'Location', 'northeast', 'FontSize', 9.5);
    xlim([380 780]);
    ylim([0 1.08]);

    nexttile;
    hold on; grid on; box on;
    plot(model.blendRows.SingleWhiteRatio, model.blendRows.Ra, ...
        'Color', [0.85 0.15 0], 'LineWidth', 1.7);
    plot(model.blendRows.SingleWhiteRatio, model.blendRows.Rmin_8, ...
        'Color', [0.12 0.62 0.38], 'LineWidth', 1.7);
    scatter(bestRa.SingleWhiteRatio, bestRa.Ra, 45, [0.85 0.15 0], 'filled');
    scatter(bestRmin.SingleWhiteRatio, bestRmin.Rmin_8, 45, [0.12 0.62 0.38], 'filled');
    xlabel('Single white ratio p', 'FontName', cfg.fontName, 'FontSize', 10.5);
    ylabel('CRI score', 'FontName', cfg.fontName, 'FontSize', 10.5);
    title('混合比例扫描结果', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'Ra','Rmin8','Best Ra','Best Rmin8'}, 'Location', 'best', 'FontSize', 9.5);
    xlim([0 1]);

    nexttile;
    bar([bestRi(bestRa); bestRi(bestRmin)].');
    grid on; box on;
    xticks(1:8);
    xticklabels({'R1','R2','R3','R4','R5','R6','R7','R8'});
    ylabel('Score', 'FontName', cfg.fontName, 'FontSize', 10.5);
    title('最佳组合的 R1-R8 对比', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'Best Ra','Best Rmin8'}, 'Location', 'southoutside', ...
        'Orientation', 'horizontal', 'FontSize', 9.5);

    title(t, '合成光谱与 CRI 扫描总览', 'FontName', cfg.fontName, ...
        'FontSize', 20, 'FontWeight', 'bold');
    export_all(f, cfg, '02_synthesis_and_scan');
end

function plot_formula_page_1(data, model, cfg)
    f = text_page('公式第 1 页：输入、XYZ、RGB 反解与光谱合成', ...
        '对应 calculate_rgb_metameric_cri.m 步骤 1 到 8：标准数据、xyY 到 XYZ、RGB 权重、光谱校准和混光。', cfg);
    ax = page_axis();

    axPlot = axes('Position', [0.075 0.62 0.39 0.23]);
    plot(data.wavelength, data.cmf(:,1), 'r', 'LineWidth', 1.6); hold on;
    plot(data.wavelength, data.cmf(:,2), 'g', 'LineWidth', 1.6);
    plot(data.wavelength, data.cmf(:,3), 'b', 'LineWidth', 1.6);
    grid on; box on;
    title('CIE 1931 2° xbar / ybar / zbar', ...
        'FontName', cfg.fontName, 'FontSize', 11.5, 'FontWeight', 'bold');
    xlabel('\lambda / nm', 'FontName', cfg.fontName, 'FontSize', 9.5);
    ylabel('Value', 'FontName', cfg.fontName, 'FontSize', 9.5);
    legend({'$\overline{x}$','$\overline{y}$','$\overline{z}$'}, ...
        'Interpreter', 'latex', 'Location', 'northeast', 'FontSize', 8.8);
    xlim([380 780]);

    axes(ax);
    draw_block(ax, 0.545, 0.84, 0.40, '步骤 1-4：输入与标准数据', ...
        '所有光谱统一插值到 380-780 nm、5 nm 步进，并使用 CIE 标准数据。', {
        '$\lambda_k=380+5k,\quad k=0,1,\cdots,80$'
        '$\Delta\lambda=5\ {\rm nm}$'
        'std.cmf = xbar/ybar/zbar'
        'std.tcs = CIE CRI TCS01-TCS08'
        'std.daylightComponents = Dxx daylight basis'
        }, cfg);

    draw_block(ax, 0.055, 0.54, 0.42, '步骤 5-7：校准、反解与 RGB 合成', ...
        '先把光谱校准到给定 Y，再用 xyY 到 XYZ 的线性关系反解 RGB 权重。', {
        '$Y_{lm}=683\sum S(\lambda)\overline{y}(\lambda)\Delta\lambda$'
        '$S_c(\lambda)=\frac{Y_c}{Y_{lm,c}}S_c^{raw}(\lambda)$'
        '$X=\frac{x}{y}Y,\quad Z=\frac{1-x-y}{y}Y$'
        '$\mathbf{A}=[\mathbf{X}_R,\mathbf{X}_G,\mathbf{X}_B]$'
        '$\mathbf{q}=[r,g,b]^T=\mathbf{A}^{-1}\mathbf{X}_T$'
        '$S_{RGB0}=rS_R+gS_G+bS_B$'
        sprintf('Target Y=%.2f lm', cfg.target_xyY(3))
        sprintf('r=%.12f', model.rgbWeight(1))
        sprintf('g=%.12f', model.rgbWeight(2))
        sprintf('b=%.12f', model.rgbWeight(3))
        }, cfg);

    draw_block(ax, 0.545, 0.54, 0.40, '步骤 8：混光扫描与 32000 码值验算', ...
        'p 是单白灯比例，1-p 是 RGB 白光比例；右侧公式用于和嵌入式侧 xy 计算对齐。', {
        '$S_{mix}(\lambda,p)=pS_{Single}(\lambda)$'
        '$\quad +(1-p)S_{RGB}(\lambda)$'
        '$p\in\{0,0.001,0.002,\cdots,1\}$'
        '$D=\sum_c\frac{Y_c}{y_c}Code_c$'
        '$x=\frac{\sum_c\frac{Y_c}{y_c}x_cCode_c}{D}$'
        '$y=\frac{\sum_cY_cCode_c}{D}$'
        sprintf('RGB32000=[%d, %d, %d]', model.rgb32000)
        sprintf('xy check=(%.5f, %.5f)', model.rgb32000_xy)
        }, cfg);

    export_all(f, cfg, '03_formula_page1_rgb_model');
end

function plot_formula_page_2(cfg)
    f = text_page('公式第 2 页：CRI 评价主链路', ...
        '对应 calculate_rgb_metameric_cri.m 的 spectrum_metrics、reference_spd、cri_R1_R8 和 tcs_uvw。', cfg);
    ax = page_axis();

    y = 0.82;
    y = draw_block(ax, 0.055, y, 0.42, '步骤 9-10：混合光谱进入指标计算', ...
        '每一个 p 都先得到光源 XYZ、xy、Y、uv，再估算 CCT 并生成参考光源。', {
        '$[X,Y,Z]=\sum S_{mix}(\lambda,p)\,CMF(\lambda)\Delta\lambda$'
        '$x=\frac{X}{X+Y+Z},\quad y=\frac{Y}{X+Y+Z}$'
        '$u=\frac{4X}{X+15Y+3Z}$'
        '$v=\frac{6Y}{X+15Y+3Z}$'
        '$CCT<5000K:\ S_{ref}=S_{BB}(T)$'
        '$CCT\geq5000K:\ S_{ref}=S_D(T)$'
        }, cfg);

    draw_block(ax, 0.055, y, 0.42, 'CRI 色样反射与归一化', ...
        '测试光源和参考光源都照射同一组 TCS01-TCS08 标准色样。', {
        '$S_{i,j}(\lambda)=S_j(\lambda)\rho_i(\lambda)$'
        '$k_j=\frac{100}{\sum S_j(\lambda)\overline{y}(\lambda)\Delta\lambda}$'
        '$[X_{i,j},Y_{i,j},Z_{i,j}]=k_j\sum S_{i,j}(\lambda)CMF(\lambda)\Delta\lambda$'
        '$i=1,\cdots,8,\quad j\in\{test,ref\}$'
        }, cfg);

    y = 0.82;
    y = draw_block(ax, 0.545, y, 0.40, 'CIE CRI 色适应与 UVW', ...
        '测试色样先做色适应，参考色样保持在参考白点下计算。', {
        '$c=\frac{4-u-10v}{v}$'
        '$d=\frac{1.708v+0.404-1.481u}{v}$'
        '$D_i=16.518+1.481(c_r/c_t)c_i-(d_r/d_t)d_i$'
        '$u_i^a=\frac{10.872+0.404(c_r/c_t)c_i-4(d_r/d_t)d_i}{D_i}$'
        '$v_i^a=\frac{5.520}{D_i}$'
        }, cfg);

    draw_block(ax, 0.545, y, 0.40, 'Ri、Ra、Rmin8 与最优筛选', ...
        '计算脚本把每个比例的 R1-R8、Ra 和 Rmin8 全部写入结果表，再取最优行。', {
        '$W_i^*=25Y_i^{1/3}-17$'
        '$U_i^*=13W_i^*(u_i^a-u_r),\quad V_i^*=13W_i^*(v_i^a-v_r)$'
        '$\Delta E_i=\sqrt{(\Delta U_i^*)^2+(\Delta V_i^*)^2+(\Delta W_i^*)^2}$'
        '$R_i=100-4.6\Delta E_i$'
        '$R_a(p)=\frac{1}{8}\sum_{i=1}^{8}R_i(p)$'
        '$R_{min,8}(p)=\min(R_1,\ldots,R_8)$'
        '$p_{Ra}=\arg\max R_a(p)$'
        '$p_{min}=\arg\max R_{min,8}(p)$'
        }, cfg);

    export_all(f, cfg, '04_formula_page2_cri_chain');
end

function plot_formula_page_3(model, cfg)
    f = text_page('公式第 3 页：本次计算结果汇总', ...
        '结果来自 rgb_target_mix_weights.csv、cri_summary.csv 和 metameric_blend_results.csv。', cfg);
    ax = page_axis();
    bestRa = model.blendRows(model.idxBestRa, :);
    bestRmin = model.blendRows(model.idxBestRmin, :);

    y = 0.82;
    y = draw_block(ax, 0.055, y, 0.42, 'RGB 权重与白点验算', ...
        'RGB 目标白光权重来自 A\\b，32000 码值用于和 RGB_to_xyY_32000.m 对齐。', {
        sprintf('r=%.12f', model.rgbWeight(1))
        sprintf('g=%.12f', model.rgbWeight(2))
        sprintf('b=%.12f', model.rgbWeight(3))
        sprintf('RGB32000=[%d, %d, %d]', model.rgb32000)
        sprintf('xy check=(%.5f, %.5f)', model.rgb32000_xy)
        }, cfg);

    draw_block(ax, 0.055, y, 0.42, '三个基准光谱指标', ...
        '这些行对应 summary 表中的前三个 case。', baseline_result_lines(model), cfg);

    y = 0.82;
    y = draw_block(ax, 0.545, y, 0.40, 'Best Ra blend', ...
        '以 Ra 最大为目标筛选得到的混光比例。', blend_result_lines(bestRa), cfg);

    draw_block(ax, 0.545, y, 0.40, 'Best Rmin8 blend', ...
        '以 R1-R8 中最差项尽量高为目标筛选得到的混光比例。', blend_result_lines(bestRmin), cfg);

    export_all(f, cfg, '05_results_summary');
end

function r = bestRi(row)
    r = [row.R1, row.R2, row.R3, row.R4, row.R5, row.R6, row.R7, row.R8];
end

function resultLines = baseline_result_lines(model)
    resultLines = {
        summary_line(model.summary, 'Single white 2.4lm')
        summary_line(model.summary, 'RGB target white')
        summary_line(model.summary, 'RGB.xlsx W sheet')
        };
end

function resultLines = blend_result_lines(row)
    r = bestRi(row);
    resultLines = {
        sprintf('single=%.3f, RGB=%.3f', row.SingleWhiteRatio, row.RGBWhiteRatio)
        sprintf('xy=(%.5f, %.5f), Y=%.4f lm', row.x, row.y, row.Y_lm)
        sprintf('CCT=%.1f K, Ra=%.4f, Rmin8=%.4f', row.CCT_K, row.Ra, row.Rmin_8)
        sprintf('Worst=%s', char(row.Worst_R))
        sprintf('R1-R4: %.2f, %.2f, %.2f, %.2f', r(1), r(2), r(3), r(4))
        sprintf('R5-R8: %.2f, %.2f, %.2f, %.2f', r(5), r(6), r(7), r(8))
        };
end

function line = summary_line(T, caseName)
    idx = find(T.Case == caseName, 1);
    if isempty(idx)
        line = [caseName ': not found'];
        return;
    end
    line = sprintf('%s: xy=(%.5f, %.5f), Y=%.4f lm, CCT=%.1f K, Ra=%.4f, Rmin8=%.4f', ...
        char(T.Case(idx)), T.x(idx), T.y(idx), T.Y_lm(idx), T.CCT_K(idx), T.Ra(idx), T.Rmin_8(idx));
end

function f = text_page(titleText, subtitleText, cfg)
    f = make_figure([100 80 2200 1500]);
    ax = page_axis();
    text(0.04, 0.955, titleText, 'FontName', cfg.fontName, ...
        'FontSize', 20, 'FontWeight', 'bold', 'Interpreter', 'none');
    text(0.04, 0.915, subtitleText, 'FontName', cfg.fontName, ...
        'FontSize', 11.8, 'Interpreter', 'none');
end

function yNext = draw_block(ax, x, y, width, titleText, explainText, lines, cfg)
    axes(ax);
    titleFont = 13.2;
    textFont = 10.0;
    formulaFont = 10.8;
    titleStep = 0.030;
    textStep = 0.029;
    formulaStep = 0.039;
    blankStep = 0.014;

    maxChars = chars_for_width(width);
    titleLines = wrap_text(titleText, maxChars + 8);
    explainLines = wrap_text(explainText, maxChars);
    bodyLines = wrap_block_lines(lines, maxChars);

    blockHeight = estimate_block_height(titleLines, explainLines, bodyLines, ...
        titleStep, textStep, formulaStep, blankStep);
    bottom = max(0.018, y - blockHeight);
    if x < 0.5
        faceColor = [0.955 0.975 1.000];
        edgeColor = [0.60 0.73 0.88];
    else
        faceColor = [0.975 0.965 1.000];
        edgeColor = [0.70 0.64 0.86];
    end
    rectangle(ax, 'Position', [x - 0.018, bottom - 0.006, width + 0.036, y - bottom + 0.024], ...
        'Curvature', [0.025 0.025], 'FaceColor', faceColor, 'EdgeColor', edgeColor, ...
        'LineWidth', 1.0);

    for i = 1:numel(titleLines)
        text(x, y, titleLines{i}, 'FontName', cfg.fontName, 'FontSize', titleFont, ...
            'FontWeight', 'bold', 'Interpreter', 'none', 'Clipping', 'on');
        y = y - titleStep;
    end
    y = y - 0.004;

    for i = 1:numel(explainLines)
        text(x, y, explainLines{i}, 'FontName', cfg.fontName, ...
            'FontSize', textFont, 'Interpreter', 'none', 'Clipping', 'on');
        y = y - textStep;
    end
    y = y - 0.008;

    for i = 1:numel(bodyLines)
        lineText = bodyLines{i};
        if isempty(lineText)
            y = y - blankStep;
        elseif startsWith(string(lineText), "$")
            text(x + 0.010, y, lineText, 'Interpreter', 'latex', ...
                'FontSize', formulaFont, 'Clipping', 'on');
            y = y - formulaStep;
        else
            text(x + 0.010, y, lineText, 'FontName', cfg.fontName, ...
                'FontSize', textFont, 'Interpreter', 'none', 'Clipping', 'on');
            y = y - textStep;
        end
    end
    yNext = y - 0.018;
end

function h = estimate_block_height(titleLines, explainLines, bodyLines, titleStep, textStep, formulaStep, blankStep)
    h = numel(titleLines) * titleStep + 0.004 + numel(explainLines) * textStep + 0.008 + 0.018;
    for i = 1:numel(bodyLines)
        lineText = bodyLines{i};
        if isempty(lineText)
            h = h + blankStep;
        elseif startsWith(string(lineText), "$")
            h = h + formulaStep;
        else
            h = h + textStep;
        end
    end
end

function wrapped = wrap_block_lines(lines, maxChars)
    wrapped = {};
    for i = 1:numel(lines)
        lineText = lines{i};
        if isempty(lineText)
            wrapped{end + 1, 1} = ''; %#ok<AGROW>
        elseif startsWith(string(lineText), "$")
            wrapped{end + 1, 1} = char(lineText); %#ok<AGROW>
        else
            splitLines = wrap_text(lineText, maxChars);
            for j = 1:numel(splitLines)
                wrapped{end + 1, 1} = splitLines{j}; %#ok<AGROW>
            end
        end
    end
end

function wrapped = wrap_text(textValue, maxChars)
    s = char(string(textValue));
    if isempty(strtrim(s))
        wrapped = {};
        return;
    end
    wrapped = {};
    while length(s) > maxChars
        cut = find_wrap_position(s, maxChars);
        wrapped{end + 1, 1} = strtrim(s(1:cut)); %#ok<AGROW>
        s = strtrim(s(cut + 1:end));
    end
    wrapped{end + 1, 1} = s;
end

function cut = find_wrap_position(s, maxChars)
    candidates = find(isspace(s(1:maxChars)) | ismember(s(1:maxChars), ',，;；、'));
    if isempty(candidates)
        cut = maxChars;
    else
        cut = candidates(end);
    end
end

function n = chars_for_width(width)
    n = max(26, floor(width * 78));
end

function ax = page_axis()
    ax = axes('Position', [0 0 1 1], 'Visible', 'off');
    axis(ax, [0 1 0 1]);
    hold(ax, 'on');
end

function f = make_figure(position)
    screen = get(0, 'ScreenSize');
    maxWidth = min([position(3), 1200, max(760, screen(3) - 160)]);
    maxHeight = min([position(4), 760, max(520, screen(4) - 180)]);
    left = max(40, min(80, screen(3) - maxWidth - 40));
    bottom = max(50, min(90, screen(4) - maxHeight - 80));
    f = figure('Color', 'w', 'Units', 'pixels', 'Position', [left bottom maxWidth maxHeight]);
end

function export_all(f, cfg, baseName)
    drawnow;
    axs = findall(f, 'Type', 'axes');
    for i = 1:numel(axs)
        try
            axs(i).Toolbar.Visible = 'off';
        catch
        end
        try
            disableDefaultInteractivity(axs(i));
        catch
        end
    end
    exportgraphics(f, fullfile(cfg.outDir, [baseName '.png']), 'Resolution', 300);
    exportgraphics(f, fullfile(cfg.outDir, [baseName '.pdf']), 'ContentType', 'vector');
    export_svg(f, fullfile(cfg.outDir, [baseName '.svg']));
    savefig(f, fullfile(cfg.outDir, [baseName '.fig']));

    for i = 1:numel(axs)
        try
            axs(i).Toolbar.Visible = 'on';
        catch
        end
        try
            enableDefaultInteractivity(axs(i));
        catch
        end
    end
    try
        z = zoom(f);
        z.Enable = 'on';
        z.Motion = 'both';
    catch
        zoom(f, 'on');
    end
end

function export_svg(f, fileName)
    try
        exportgraphics(f, fileName, 'ContentType', 'vector');
    catch
        oldRenderer = get(f, 'Renderer');
        cleanupObj = onCleanup(@() set(f, 'Renderer', oldRenderer));
        set(f, 'Renderer', 'painters');
        saveas(f, fileName, 'svg');
        clear cleanupObj;
    end
end

function s = read_spd(fileName, sheetName, wlOut)
    if isempty(sheetName)
        M = readmatrix(fileName);
    else
        M = readmatrix(fileName, 'Sheet', sheetName);
    end
    wl = M(:,1);
    if size(M, 2) >= 3
        v = M(:,3);
    else
        v = M(:,2);
    end
    ok = isfinite(wl) & isfinite(v);
    wl = wl(ok);
    v = v(ok);
    [wl, order] = sort(wl);
    v = v(order);
    s = interp1(wl, v, wlOut, 'linear', 0);
    s(s < 0) = 0;
end

function cmf = read_cmf(cfg, wlOut)
    cmfFile = fullfile(cfg.standardDir, 'CIE_xyz_1931_2deg.csv');
    require_file(cmfFile);
    M = readmatrix(cmfFile);
    cmf = interp1(M(:,1), M(:,2:4), wlOut, 'linear', 'extrap');
end

function require_file(fileName)
    if ~exist(fileName, 'file')
        error('缺少文件: %s。请先运行 calculate_rgb_metameric_cri.m。', fileName);
    end
end

function y = normalize_curve(x)
    m = max(abs(x(:)));
    if m <= 0 || ~isfinite(m)
        y = x;
    else
        y = x ./ m;
    end
end

function fontName = choose_font()
    fonts = listfonts;
    candidates = {'Microsoft YaHei','SimHei','Microsoft JhengHei','Arial Unicode MS','Arial'};
    fontName = 'Arial';
    for i = 1:numel(candidates)
        if any(strcmpi(fonts, candidates{i}))
            fontName = candidates{i};
            return;
        end
    end
end
