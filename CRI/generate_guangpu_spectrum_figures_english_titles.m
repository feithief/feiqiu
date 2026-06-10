% GENERATE_GUANGPU_SPECTRUM_FIGURES
% 对应 calculate_guangpu_metameric_cri.m，
% 生成输入光谱、合成结果、比例扫描结果，以及简洁的公式说明页。

clear; clc; close all;

cfg = figure_config();
data = load_report_data(cfg);
model = load_report_model(cfg);
cleanup_legacy_outputs(cfg);

plot_input_spectra(data, cfg);
plot_synthesized(data, model, cfg);
plot_formula_page_1(model, cfg);
plot_formula_page_2(model, cfg);
plot_formula_page_3(data, model, cfg);

disp("Figures exported to: " + cfg.outDir);

function cfg = figure_config()
    cfg.rootDir = fileparts(mfilename('fullpath'));
    cfg.outDir = fullfile(cfg.rootDir, 'figures_guangpu');
    inputDir = fullfile(cfg.rootDir, 'guangpu_case_input');
    addpath(inputDir);
    caseCfg = guangpu_case_config();
    cfg.spectraDir = caseCfg.inputDir;

    cfg.rgbTargetCsv = fullfile(cfg.rootDir, 'guangpu_rgb_target_spectrum_0p2nm.csv');
    cfg.bestBlendCsv = fullfile(cfg.rootDir, 'guangpu_best_blend_spectrum_0p2nm.csv');
    cfg.blendRowsCsv = fullfile(cfg.rootDir, 'guangpu_metameric_blend_results.csv');
    cfg.summaryCsv = fullfile(cfg.rootDir, 'guangpu_cri_summary.csv');
    cfg.weightsCsv = fullfile(cfg.rootDir, 'guangpu_rgb_target_mix_weights.csv');

    cfg.target_xyY = caseCfg.target_xyY;
    cfg.R_xyY = caseCfg.channel_xyY.R;
    cfg.G_xyY = caseCfg.channel_xyY.G;
    cfg.B_xyY = caseCfg.channel_xyY.B;
    cfg.W_xyY = caseCfg.channel_xyY.W;

    cfg.fontName = choose_font();
    if ~exist(cfg.outDir, 'dir')
        mkdir(cfg.outDir);
    end
end

function data = load_report_data(cfg)
    require_file(cfg.rgbTargetCsv);
    require_file(cfg.bestBlendCsv);

    [wl, rawR] = read_spd_native(fullfile(cfg.spectraDir, 'R.CSV'));
    [~, rawG] = read_spd_native(fullfile(cfg.spectraDir, 'G.CSV'));
    [~, rawB] = read_spd_native(fullfile(cfg.spectraDir, 'B.CSV'));
    [~, rawW] = read_spd_native(fullfile(cfg.spectraDir, 'W.CSV'));

    data.wavelength = wl;
    data.rawR = rawR;
    data.rawG = rawG;
    data.rawB = rawB;
    data.rawW = rawW;
    data.rawSingleWhite = rawW;

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

    model.summary = readtable(cfg.summaryCsv, 'TextType', 'string');
    model.blendRows = readtable(cfg.blendRowsCsv);
    [~, model.idxBestRa] = max(model.blendRows.Ra);
    [~, model.idxBestRmin] = max(model.blendRows.Rmin_8);
end

function cleanup_legacy_outputs(cfg)
    baseNames = {
        '01_input_spectra'
        '02_synthesis_and_scan'
        '03_formula_page1'
        '04_formula_page2'
        '05_formula_page3'
        };
    extensions = {'.png', '.pdf', '.fig'};
    for i = 1:numel(baseNames)
        for j = 1:numel(extensions)
            fileName = fullfile(cfg.outDir, [baseNames{i} extensions{j}]);
            if exist(fileName, 'file')
                try
                    delete(fileName);
                catch
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
    plot(data.wavelength, normalize_curve(data.rawR), 'r', 'LineWidth', 1.6);
    plot(data.wavelength, normalize_curve(data.rawG), 'g', 'LineWidth', 1.6);
    plot(data.wavelength, normalize_curve(data.rawB), 'b', 'LineWidth', 1.6);
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 12);
    title('Input R/G/B Raw Spectra', 'FontName', cfg.fontName, ...
        'FontSize', 16, 'FontWeight', 'bold');
    legend({'R channel','G channel','B channel'}, 'Location', 'northeast', 'FontSize', 10.5);
    xlim([380 780]);
    ylim([0 1.08]);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.rawW), 'Color', [0.10 0.36 0.72], 'LineWidth', 1.8);
    xlabel('Wavelength  \lambda  / nm', 'FontName', cfg.fontName, 'FontSize', 12);
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 12);
    title('Input W-Channel Raw Spectrum', 'FontName', cfg.fontName, ...
        'FontSize', 16, 'FontWeight', 'bold');
    legend({'W channel'}, 'Location', 'northeast', 'FontSize', 10.5);
    xlim([380 780]);
    ylim([0 1.08]);

    title(t, 'Overview of Input Spectra', 'FontName', cfg.fontName, 'FontSize', 20, 'FontWeight', 'bold');
    export_all(f, cfg, '01_input_spectra');
end

function plot_synthesized(data, model, cfg)
    f = make_figure([100 80 1900 1300]);
    t = tiledlayout(2, 2, 'TileSpacing', 'compact', 'Padding', 'compact');
    bestRa = model.blendRows(model.idxBestRa, :);
    bestRmin = model.blendRows(model.idxBestRmin, :);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.R_component), 'r', 'LineWidth', 1.35);
    plot(data.wavelength, normalize_curve(data.G_component), 'g', 'LineWidth', 1.35);
    plot(data.wavelength, normalize_curve(data.B_component), 'b', 'LineWidth', 1.35);
    plot(data.wavelength, normalize_curve(data.rgbTarget), 'k', 'LineWidth', 1.9);
    title(sprintf('RGB Target White: r=%.6f, g=%.6f, b=%.6f', model.rgbWeight), ...
        'FontName', cfg.fontName, 'FontSize', 13.5, 'FontWeight', 'bold');
    ylabel('Normalized power', 'FontName', cfg.fontName, 'FontSize', 10.5);
    legend({'rS_R','gS_G','bS_B','S_{RGB}'}, 'Location', 'northeast', 'FontSize', 9.5);
    xlim([380 780]);
    ylim([0 1.08]);

    nexttile;
    hold on; grid on; box on;
    plot(data.wavelength, normalize_curve(data.singleWhite), 'Color', [0.35 0.35 0.35], 'LineWidth', 1.4);
    plot(data.wavelength, normalize_curve(data.rgbTarget), 'Color', [0 0.30 0.85], 'LineWidth', 1.4);
    plot(data.wavelength, normalize_curve(data.bestRaBlend), 'Color', [0.85 0.15 0], 'LineWidth', 1.9);
    plot(data.wavelength, normalize_curve(data.bestRminBlend), 'Color', [0.12 0.62 0.38], 'LineWidth', 1.9);
    title('W Channel, RGB White, and Optimized Blended Spectra', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'W channel','RGB target','Best Ra','Best Rmin8'}, ...
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
    xlabel('W ratio p', 'FontName', cfg.fontName, 'FontSize', 10.5);
    ylabel('CRI score', 'FontName', cfg.fontName, 'FontSize', 10.5);
    title('Blend Ratio Sweep Results', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'Ra','Rmin8','Best Ra','Best Rmin8'}, 'Location', 'best', 'FontSize', 9.5);
    xlim([0 1]);

    nexttile;
    bar([bestRi(bestRa); bestRi(bestRmin)].');
    grid on; box on;
    xticks(1:8);
    xticklabels({'R1','R2','R3','R4','R5','R6','R7','R8'});
    ylabel('Score', 'FontName', cfg.fontName, 'FontSize', 10.5);
    title('R1-R8 Comparison of Optimized Blends', 'FontName', cfg.fontName, ...
        'FontSize', 13.5, 'FontWeight', 'bold');
    legend({'Best Ra','Best Rmin8'}, 'Location', 'southoutside', ...
        'Orientation', 'horizontal', 'FontSize', 9.5);

    title(t, 'Overview of Synthesized Spectra and CRI Sweep', 'FontName', cfg.fontName, ...
        'FontSize', 20, 'FontWeight', 'bold');
    export_all(f, cfg, '02_synthesis_and_scan');
end

function plot_formula_page_1(model, cfg)
    f = text_page('Formula Page 1: Inputs, XYZ Conversion, RGB Inversion, and Spectral Synthesis', ...
        'Corresponding to Steps 1-8 in calculate_guangpu_metameric_cri.m.', cfg);
    ax = page_axis();

    lines = { ...
        sprintf('Target white point: (%.4f, %.4f, %.4f)', cfg.target_xyY), ...
        sprintf('R = (%.4f, %.4f, %.4f)', cfg.R_xyY), ...
        sprintf('G = (%.4f, %.4f, %.4f)', cfg.G_xyY), ...
        sprintf('B = (%.4f, %.4f, %.4f)', cfg.B_xyY), ...
        sprintf('W = (%.4f, %.4f, %.4f)', cfg.W_xyY), ...
        'Wavelength range: 380-780 nm', ...
        'Spectral sampling interval: 0.2 nm', ...
        '', ...
        '1) Calibrate each channel spectrum to the specified Y value', ...
        '2) Solve the RGB weights for the target white point using xyY-to-XYZ conversion', ...
        '3) Synthesize the RGB target white spectrum', ...
        '4) Normalize the W channel and RGB target white to the same luminance'};

    draw_lines(ax, 0.08, 0.86, lines, cfg);
    add_formula(ax, 0.08, 0.22, 'A=[XYZ_R, XYZ_G, XYZ_B], \quad b=XYZ_{target}, \quad q=A^{-1}b', cfg);
    export_all(f, cfg, '03_formula_page1');
end

function plot_formula_page_2(model, cfg)
    f = text_page('Formula Page 2: Metameric Blend Ratio Sweep', ...
        'Corresponding to Steps 9-12 in calculate_guangpu_metameric_cri.m.', cfg);
    ax = page_axis();

    lines = { ...
        'After normalizing the W channel and RGB target white to the same luminance, sweep the blend ratio p:', ...
        '', ...
        'p = W-channel ratio', ...
        '1-p = RGB-white ratio', ...
        '', ...
        'For each p, compute:', ...
        '- xy', ...
        '- Y', ...
        '- CCT', ...
        '- R1-R8', ...
        '- Ra', ...
        '- Rmin8', ...
        '', ...
        'Final outputs:', ...
        '- Blend ratio with maximum Ra', ...
        '- Blend ratio with maximum Rmin8'};

    draw_lines(ax, 0.08, 0.86, lines, cfg);
    add_formula(ax, 0.08, 0.30, 'S_{mix}=pS_{W}+(1-p)S_{RGB}', cfg);
    add_formula(ax, 0.08, 0.22, 'p_{Ra}=\\arg\\max R_a(p), \quad p_{Rmin}=\\arg\\max R_{min8}(p)', cfg);
    export_all(f, cfg, '04_formula_page2');
end

function plot_formula_page_3(data, model, cfg)
    f = text_page('Formula Page 3: Summary of Key Results', ...
        'Generated from the CSV outputs of the Guangpu workflow.', cfg);
    ax = page_axis();

    S = model.summary;
    bestRa = model.blendRows(model.idxBestRa, :);
    bestRmin = model.blendRows(model.idxBestRmin, :);

    lines = {
        sprintf('RGB weights: R=%.8f, G=%.8f, B=%.8f', model.rgbWeight)
        sprintf('W channel: xy=(%.5f, %.5f), Y=%.4f lm, Ra=%.4f, Rmin8=%.4f', S.x(1), S.y(1), S.Y_lm(1), S.Ra(1), S.Rmin_8(1))
        sprintf('RGB target white: xy=(%.5f, %.5f), Y=%.4f lm, Ra=%.4f, Rmin8=%.4f', S.x(2), S.y(2), S.Y_lm(2), S.Ra(2), S.Rmin_8(2))
        ''
        sprintf('Best Ra: W=%.3f, RGB=%.3f', bestRa.SingleWhiteRatio, bestRa.RGBWhiteRatio)
        sprintf('        xy=(%.5f, %.5f), Y=%.4f, Ra=%.4f, Rmin8=%.4f', bestRa.x, bestRa.y, bestRa.Y_lm, bestRa.Ra, bestRa.Rmin_8)
        ''
        sprintf('Best Rmin8: W=%.3f, RGB=%.3f', bestRmin.SingleWhiteRatio, bestRmin.RGBWhiteRatio)
        sprintf('            xy=(%.5f, %.5f), Y=%.4f, Ra=%.4f, Rmin8=%.4f', bestRmin.x, bestRmin.y, bestRmin.Y_lm, bestRmin.Ra, bestRmin.Rmin_8)
        ''
        sprintf('Number of spectral samples: %d', numel(data.wavelength))
        sprintf('Spectral sampling interval: %.1f nm', median(diff(data.wavelength)))
        };

    draw_lines(ax, 0.08, 0.86, lines, cfg);
    export_all(f, cfg, '05_formula_page3');
end

function ax = page_axis()
    ax = axes('Position', [0 0 1 1], 'Visible', 'off');
    xlim(ax, [0 1]);
    ylim(ax, [0 1]);
end

function f = text_page(titleText, subtitleText, cfg)
    f = make_figure([100 80 1700 1100]);
    annotation(f, 'textbox', [0.05 0.91 0.90 0.06], ...
        'String', titleText, ...
        'EdgeColor', 'none', ...
        'HorizontalAlignment', 'center', ...
        'FontName', cfg.fontName, ...
        'FontSize', 20, ...
        'FontWeight', 'bold');
    annotation(f, 'textbox', [0.07 0.86 0.86 0.05], ...
        'String', subtitleText, ...
        'EdgeColor', 'none', ...
        'HorizontalAlignment', 'center', ...
        'FontName', cfg.fontName, ...
        'FontSize', 11);
end

function f = make_figure(position)
    screen = get(0, 'ScreenSize');
    maxWidth = min([position(3), 1200, max(760, screen(3) - 160)]);
    maxHeight = min([position(4), 760, max(520, screen(4) - 180)]);
    left = max(40, min(80, screen(3) - maxWidth - 40));
    bottom = max(50, min(90, screen(4) - maxHeight - 80));
    f = figure('Color', 'w', 'Units', 'pixels', 'Position', [left bottom maxWidth maxHeight]);
end

function draw_lines(ax, x0, y0, lines, cfg)
    dy = 0.048;
    for i = 1:numel(lines)
        text(ax, x0, y0 - (i-1) * dy, lines{i}, ...
            'FontName', cfg.fontName, ...
            'FontSize', 15, ...
            'Interpreter', 'none', ...
            'VerticalAlignment', 'top');
    end
end

function add_formula(ax, x0, y0, formulaText, cfg)
    text(ax, x0, y0, ['$' formulaText '$'], ...
        'Interpreter', 'latex', ...
        'FontSize', 22, ...
        'FontName', cfg.fontName, ...
        'VerticalAlignment', 'top');
end

function export_all(f, cfg, baseName)
    saveas(f, fullfile(cfg.outDir, [baseName '.png']));
    saveas(f, fullfile(cfg.outDir, [baseName '.fig']));
    try
        exportgraphics(f, fullfile(cfg.outDir, [baseName '.pdf']), 'ContentType', 'vector');
    catch
    end
end

function c = normalize_curve(v)
    vmax = max(v);
    if vmax <= 0
        c = v;
    else
        c = v ./ vmax;
    end
end

function r = bestRi(row)
    r = [row.R1, row.R2, row.R3, row.R4, row.R5, row.R6, row.R7, row.R8];
end

function require_file(fileName)
    if ~exist(fileName, 'file')
        error('Missing file: %s. Run calculate_guangpu_metameric_cri.m first.', fileName);
    end
end

function [wl, spd] = read_spd_native(fileName)
    M = readmatrix(fileName);
    wl = M(:, 1);
    if size(M, 2) >= 3
        spd = M(:, 3);
    else
        spd = M(:, 2);
    end
    ok = isfinite(wl) & isfinite(spd);
    wl = wl(ok);
    spd = spd(ok);
    [wl, order] = sort(wl);
    spd = spd(order);
    spd(spd < 0) = 0;
end

function fontName = choose_font()
    candidates = {'Microsoft YaHei UI', 'Microsoft YaHei', 'SimHei', 'Arial'};
    available = listfonts;
    fontName = 'Arial';
    for i = 1:numel(candidates)
        if any(strcmpi(available, candidates{i}))
            fontName = candidates{i};
            break;
        end
    end
end
