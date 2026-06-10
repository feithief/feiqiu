% S101
% One-click export for:
%   RGB/20260528小莉测试版/BLINKY_SIMPLE_EXAMPLE_TLD4020
%
% Workflow:
%   1. Run s01_make_lut_xy_grid_final.m
%   2. Run s101.m
%   3. Rebuild the Xiaoli firmware project
%
% This script is self-contained.
% It does not run any other custom export .m files.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S101 Xiaoli export: update localfit C files\n');
fprintf('Input : triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv\n');
fprintf('Target: RGB/20260528小莉测试版/BLINKY_SIMPLE_EXAMPLE_TLD4020\n');
fprintf('Logic : identical export flow, single entry only\n');
fprintf('============================================================\n\n');

scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
repoRoot = fileparts(projectRoot);
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
firmwareDir = fullfile(repoRoot, 'RGB', '20260528小莉测试版', ...
    'BLINKY_SIMPLE_EXAMPLE_TLD4020', 'Algorithm', 'AlgoRatioK_localfit');
headerFile = fullfile(firmwareDir, 'AlgoRatioK_localfit.h');

inputFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
ratio16OutputFile = fullfile(outputDir, 'AlgoRatioK_localfit_ratio16_runtime.c');
ratio16FirmwareFile = fullfile(firmwareDir, 'AlgoRatioK_localfit_ratio16_runtime.c');
packedOutputFile = fullfile(outputDir, 'AlgoRatioK_localfit_packed32_compat.c');
packedFirmwareFile = fullfile(firmwareDir, 'AlgoRatioK_localfit_packed32_compat.c');

if ~exist(inputFile, 'file')
    error('Missing LUT result file:\n%s\nPlease run s01_make_lut_xy_grid_final.m first.', inputFile);
end
if ~exist(firmwareDir, 'dir')
    error('Missing firmware directory:\n%s', firmwareDir);
end
if ~exist(ratio16FirmwareFile, 'file')
    error('Missing runtime-grid template file:\n%s', ratio16FirmwareFile);
end
if ~exist(packedFirmwareFile, 'file')
    error('Missing packed32 template file:\n%s', packedFirmwareFile);
end
if ~exist(headerFile, 'file')
    error('Missing header file:\n%s', headerFile);
end

T = readtable(inputFile, 'VariableNamingRule', 'preserve');
needCols = {'GridI','GridJ','GridK','Target_x','Target_y', ...
    'Target_u1976','Target_v1976','Best_SingleWhiteRatio'};
for i = 1:numel(needCols)
    if ~ismember(needCols{i}, T.Properties.VariableNames)
        error('Missing column: %s', needCols{i});
    end
end

T = sortrows(T, {'GridI','GridJ','GridK'});
gridMeta = get_grid_meta(T);
validate_runtime_grid_generation(T, gridMeta);

fprintf('[1/3] Export runtime-grid ratio16 version...\n');
ratio16TemplateText = fileread(ratio16FirmwareFile);
ratio16OutputText = build_ratio16_output_text(ratio16TemplateText, T, gridMeta);
write_text_file(ratio16OutputFile, ratio16OutputText);
write_text_file(ratio16FirmwareFile, ratio16OutputText);
fprintf('Generated : %s\n', ratio16OutputFile);
fprintf('Mirrored  : %s\n\n', ratio16FirmwareFile);

fprintf('[2/3] Export packed32 compatibility version...\n');
packedTemplateText = fileread(packedFirmwareFile);
packedOutputText = build_packed_output_text(packedTemplateText, T, gridMeta);
write_text_file(packedOutputFile, packedOutputText);
write_text_file(packedFirmwareFile, packedOutputText);
fprintf('Generated : %s\n', packedOutputFile);
fprintf('Mirrored  : %s\n\n', packedFirmwareFile);

fprintf('[3/3] Read current compile-time macro selection...\n');
show_current_macro_selection(headerFile);

fprintf('\nDone.\n');
fprintf('Use only these two steps next time:\n');
fprintf('  1. s01_make_lut_xy_grid_final.m\n');
fprintf('  2. s101.m\n');

function write_text_file(filePath, outputText)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open output file: %s', filePath);
    end
    cleanupObj = onCleanup(@() fclose(fid)); %#ok<NASGU>
    fwrite(fid, outputText, 'char');
end

function outputText = build_ratio16_output_text(templateText, T, gridMeta)
    lutCount = height(T);
    ratioRowsText = build_ratio_rows_text(T);

    outputText = regexprep(templateText, ...
        '#define ALGO_RATIO_K_LUT_COUNT\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_LUT_COUNT           (%uu)', uint32(lutCount)), ...
        'once');

    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_GRID_ORDER\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_GRID_ORDER          (%uu)', uint32(gridMeta.GridOrder)), ...
        'once');

    macroPairs = {
        'ALGO_RATIO_K_XY_SCALE', sprintf('(%uu)', uint32(gridMeta.XYScale))
        'ALGO_RATIO_K_VERTEX_K_X_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexKXY(1)))
        'ALGO_RATIO_K_VERTEX_K_Y_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexKXY(2)))
        'ALGO_RATIO_K_VERTEX_I_X_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexIXY(1)))
        'ALGO_RATIO_K_VERTEX_I_Y_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexIXY(2)))
        'ALGO_RATIO_K_VERTEX_J_X_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexJXY(1)))
        'ALGO_RATIO_K_VERTEX_J_Y_QSCALE', sprintf('(%uu)', uint32(gridMeta.VertexJXY(2)))
        };

    for i = 1:size(macroPairs, 1)
        pattern = sprintf('#define %s\\s+\\([^\\r\\n]+\\)', macroPairs{i, 1});
        replacement = sprintf('#define %-32s %s', macroPairs{i, 1}, macroPairs{i, 2});
        outputText = regexprep(outputText, pattern, replacement, 'once');
    end

    ratioPattern = ['static const uint16_t g_algoRatioKRatioQ10000Lut\[ALGO_RATIO_K_LUT_COUNT\] =\r?\n' ...
        '\{\r?\n[\s\S]*?\r?\n\};'];
    ratioReplacement = sprintf(['static const uint16_t g_algoRatioKRatioQ10000Lut[ALGO_RATIO_K_LUT_COUNT] =\r\n' ...
        '{\r\n%s\r\n};'], ratioRowsText);
    outputText = regexprep(outputText, ratioPattern, ratioReplacement, 'once');
end

function ratioRowsText = build_ratio_rows_text(T)
    lines = strings(height(T), 1);
    for i = 1:height(T)
        ratioQ10000 = round(T.Best_SingleWhiteRatio(i) * 10000);
        ratioQ10000 = min(max(ratioQ10000, 0), 10000);
        lines(i) = sprintf('    %5du,  /* idx=%3d, ratio=%.6f */', ...
            ratioQ10000, i - 1, T.Best_SingleWhiteRatio(i));
    end
    ratioRowsText = strjoin(cellstr(lines), sprintf('\r\n'));
end

function outputText = build_packed_output_text(templateText, T, gridMeta)
    lutCount = height(T);
    lutRowsText = build_lut_rows_text(T);

    outputText = regexprep(templateText, ...
        '#define ALGO_RATIO_K_LUT_COUNT\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_LUT_COUNT           (%uu)', uint32(lutCount)), ...
        'once');

    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_GRID_ORDER\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_GRID_ORDER          (%uu)', uint32(gridMeta.GridOrder)), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_K_U\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_K_U          (%uu)', uint32(gridMeta.VertexKQ1000(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_K_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_K_V          (%uu)', uint32(gridMeta.VertexKQ1000(2))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_I_U\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_I_U          (%uu)', uint32(gridMeta.VertexIQ1000(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_I_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_I_V          (%uu)', uint32(gridMeta.VertexIQ1000(2))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_J_U\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_J_U          (%uu)', uint32(gridMeta.VertexJQ1000(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_J_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_J_V          (%uu)', uint32(gridMeta.VertexJQ1000(2))), ...
        'once');

    lutPattern = ['static const uint32_t g_algoRatioKPackedLut\[ALGO_RATIO_K_LUT_COUNT\] =\r?\n' ...
        '\{\r?\n[\s\S]*?\r?\n\};'];
    lutReplacement = sprintf(['static const uint32_t g_algoRatioKPackedLut[ALGO_RATIO_K_LUT_COUNT] =\r\n' ...
        '{\r\n%s\r\n};'], lutRowsText);
    outputText = regexprep(outputText, lutPattern, lutReplacement, 'once');
end

function lutRowsText = build_lut_rows_text(T)
    lines = strings(height(T), 1);
    for i = 1:height(T)
        u = round(T.Target_u1976(i) * 1000);
        v = round(T.Target_v1976(i) * 1000);
        ratio12 = round(T.Best_SingleWhiteRatio(i) * 4095);

        u = min(max(u, 0), 1023);
        v = min(max(v, 0), 1023);
        ratio12 = min(max(ratio12, 0), 4095);

        lines(i) = sprintf('    ALGO_RATIO_K_PACK(%4du, %4du, %4du),  /* idx=%3d, ratio=%.6f */', ...
            u, v, ratio12, i - 1, T.Best_SingleWhiteRatio(i));
    end
    lutRowsText = strjoin(cellstr(lines), sprintf('\r\n'));
end

function gridMeta = get_grid_meta(T)
    gridOrderValues = T.GridI + T.GridJ + T.GridK;
    gridOrderUnique = unique(gridOrderValues);
    if numel(gridOrderUnique) ~= 1
        error('GridI + GridJ + GridK must be constant for all LUT rows.');
    end

    gridOrder = gridOrderUnique(1);
    expectedCount = (gridOrder + 1) * (gridOrder + 2) / 2;
    if height(T) ~= expectedCount
        error('LUT row count %d does not match triangular grid count %d.', height(T), expectedCount);
    end

    vertexKRow = T((T.GridI == 0) & (T.GridJ == 0) & (T.GridK == gridOrder), :);
    vertexIRow = T((T.GridI == gridOrder) & (T.GridJ == 0) & (T.GridK == 0), :);
    vertexJRow = T((T.GridI == 0) & (T.GridJ == gridOrder) & (T.GridK == 0), :);
    if (height(vertexKRow) ~= 1) || (height(vertexIRow) ~= 1) || (height(vertexJRow) ~= 1)
        error('Cannot uniquely determine grid triangle vertices from GridI/GridJ/GridK.');
    end

    gridMeta = struct();
    gridMeta.GridOrder = gridOrder;
    % 1e6 precision is occasionally short by 1 LSB in regenerated u'v'.
    % 2e6 is the smallest scale that eliminates the mismatch on current LUTs
    % while keeping the runtime math comfortably within int64 range.
    gridMeta.XYScale = 2000000;
    gridMeta.VertexKQ1000 = [round(vertexKRow.Target_u1976(1) * 1000), round(vertexKRow.Target_v1976(1) * 1000)];
    gridMeta.VertexIQ1000 = [round(vertexIRow.Target_u1976(1) * 1000), round(vertexIRow.Target_v1976(1) * 1000)];
    gridMeta.VertexJQ1000 = [round(vertexJRow.Target_u1976(1) * 1000), round(vertexJRow.Target_v1976(1) * 1000)];
    gridMeta.VertexKXY = [round(vertexKRow.Target_x(1) * gridMeta.XYScale), round(vertexKRow.Target_y(1) * gridMeta.XYScale)];
    gridMeta.VertexIXY = [round(vertexIRow.Target_x(1) * gridMeta.XYScale), round(vertexIRow.Target_y(1) * gridMeta.XYScale)];
    gridMeta.VertexJXY = [round(vertexJRow.Target_x(1) * gridMeta.XYScale), round(vertexJRow.Target_y(1) * gridMeta.XYScale)];
end

function validate_runtime_grid_generation(T, gridMeta)
    for i = 1:height(T)
        gridI = int64(T.GridI(i));
        gridJ = int64(T.GridJ(i));
        gridK = int64(T.GridK(i));

        xNum = (gridK * int64(gridMeta.VertexKXY(1))) + ...
            (gridI * int64(gridMeta.VertexIXY(1))) + ...
            (gridJ * int64(gridMeta.VertexJXY(1)));
        yNum = (gridK * int64(gridMeta.VertexKXY(2))) + ...
            (gridI * int64(gridMeta.VertexIXY(2))) + ...
            (gridJ * int64(gridMeta.VertexJXY(2)));

        denNum = (int64(3) * int64(gridMeta.GridOrder) * int64(gridMeta.XYScale)) ...
            - (int64(2) * xNum) + (int64(12) * yNum);

        uCalcQ1000 = round_div_positive_int64(int64(4000) * xNum, denNum);
        vCalcQ1000 = round_div_positive_int64(int64(9000) * yNum, denNum);

        uActualQ1000 = round(T.Target_u1976(i) * 1000);
        vActualQ1000 = round(T.Target_v1976(i) * 1000);

        if (uCalcQ1000 ~= uActualQ1000) || (vCalcQ1000 ~= vActualQ1000)
            error(['Runtime grid regeneration mismatch at LUT idx=%d (GridI=%d, GridJ=%d, GridK=%d): ' ...
                'actual=(%d,%d), calc=(%d,%d).'], ...
                i - 1, T.GridI(i), T.GridJ(i), T.GridK(i), ...
                uActualQ1000, vActualQ1000, uCalcQ1000, vCalcQ1000);
        end
    end
end

function value = round_div_positive_int64(numerator, denominator)
    if denominator <= 0
        error('round_div_positive_int64 requires denominator > 0.');
    end
    numerator = int64(numerator);
    denominator = int64(denominator);
    value = idivide(numerator + idivide(denominator, int64(2), 'floor'), denominator, 'floor');
end

function show_current_macro_selection(headerFile)
    headerText = fileread(headerFile);
    runtimeGridEnable = parse_macro_value(headerText, 'ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE');
    fastNearestEnable = parse_macro_value(headerText, 'ALGO_RATIO_K_FAST_NEAREST_ENABLE');

    fprintf('Header: %s\n', headerFile);
    fprintf('ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE = %d\n', runtimeGridEnable);
    fprintf('ALGO_RATIO_K_FAST_NEAREST_ENABLE        = %d\n', fastNearestEnable);

    if runtimeGridEnable ~= 0
        fprintf('Current firmware branch: runtime-grid + uint16 ratio LUT\n');
    else
        fprintf('Current firmware branch: packed32 compatibility LUT\n');
    end

    if fastNearestEnable ~= 0
        fprintf('Nearest-point search : FAST local neighborhood search\n');
    else
        fprintf('Nearest-point search : FULL SCAN over all LUT points\n');
    end
end

function value = parse_macro_value(headerText, macroName)
    pattern = ['#define\s+', regexptranslate('escape', macroName), '\s+\((\d+)u\)'];
    tokens = regexp(headerText, pattern, 'tokens', 'once');
    if isempty(tokens)
        error('Cannot parse macro %s from header.', macroName);
    end
    value = str2double(tokens{1});
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
