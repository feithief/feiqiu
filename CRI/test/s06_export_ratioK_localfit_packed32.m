% S06_EXPORT_RATIOK_LOCALFIT_PACKED32
% Export the MATLAB-generated LUT directly as AlgoRatioK_localfit_packed32_compat.c.
%
% Input:
%   triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv
%
% Output:
%   triangle_grid_lut_output/AlgoRatioK_localfit_packed32_compat.c
%   RGB/BLINKY_SIMPLE_EXAMPLE_TLD4020/Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit_packed32_compat.c
%
% Packed LUT format:
%   bit[ 0: 9] = U       = round(Target_u1976 * 1000)
%   bit[10:19] = V       = round(Target_v1976 * 1000)
%   bit[20:31] = ratio12 = round(Best_SingleWhiteRatio * 4095)
%
% Public C interface:
%   uint8_t AlgoRatioK_GetWhiteRatioQ10000(uint16_t targetU,
%                                          uint16_t targetV,
%                                          uint16_t *whiteRatioQ10000);

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S06 localfit packed32 compat version: export LUT to AlgoRatioK_localfit_packed32_compat.c\n');
fprintf('Input : triangle_grid_lut_output/guangpu_triangle_grid_lut_results.csv\n');
fprintf('Output: triangle_grid_lut_output/AlgoRatioK_localfit_packed32_compat.c\n');
fprintf('Mirror: RGB/BLINKY_SIMPLE_EXAMPLE_TLD4020/Algorithm/AlgoRatioK_localfit/AlgoRatioK_localfit_packed32_compat.c\n');
fprintf('============================================================\n\n');

scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
repoRoot = fileparts(projectRoot);
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
firmwareDir = fullfile(repoRoot, 'RGB', 'BLINKY_SIMPLE_EXAMPLE_TLD4020', ...
    'Algorithm', 'AlgoRatioK_localfit');

inputFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
outputFile = fullfile(outputDir, 'AlgoRatioK_localfit_packed32_compat.c');
firmwareOutputFile = fullfile(firmwareDir, 'AlgoRatioK_localfit_packed32_compat.c');

if ~exist(inputFile, 'file')
    error('Missing LUT result file:\n%s\nPlease run s01_make_lut_xy_grid_final.m first.', inputFile);
end

if ~exist(firmwareDir, 'dir')
    mkdir(firmwareDir);
end

T = readtable(inputFile, 'VariableNamingRule', 'preserve');
needCols = {'GridI','GridJ','GridK','Target_u1976','Target_v1976','Best_SingleWhiteRatio'};
for i = 1:numel(needCols)
    if ~ismember(needCols{i}, T.Properties.VariableNames)
        error('Missing column: %s', needCols{i});
    end
end

T = sortrows(T, {'GridI','GridJ','GridK'});

if ~exist(firmwareOutputFile, 'file')
    error('Missing localfit template file:\n%s', firmwareOutputFile);
end

templateText = fileread(firmwareOutputFile);

write_c_file(outputFile, T, templateText);
write_c_file(firmwareOutputFile, T, templateText);

fprintf('Generated C file:\n%s\n', outputFile);
fprintf('Mirrored firmware file:\n%s\n', firmwareOutputFile);

function write_c_file(filePath, T, templateText)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open output file: %s', filePath);
    end
    cleanupObj = onCleanup(@() fclose(fid)); %#ok<NASGU>

    outputText = build_output_text(templateText, T);
    fwrite(fid, outputText, 'char');
end

function outputText = build_output_text(templateText, T)
    lutCount = height(T);
    gridMeta = get_grid_meta(T);
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
        sprintf('#define ALGO_RATIO_K_VERTEX_K_U          (%uu)', uint32(gridMeta.VertexK(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_K_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_K_V          (%uu)', uint32(gridMeta.VertexK(2))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_I_U\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_I_U          (%uu)', uint32(gridMeta.VertexI(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_I_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_I_V          (%uu)', uint32(gridMeta.VertexI(2))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_J_U\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_J_U          (%uu)', uint32(gridMeta.VertexJ(1))), ...
        'once');
    outputText = regexprep(outputText, ...
        '#define ALGO_RATIO_K_VERTEX_J_V\s+\(\d+u\)', ...
        sprintf('#define ALGO_RATIO_K_VERTEX_J_V          (%uu)', uint32(gridMeta.VertexJ(2))), ...
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

    vertexKRow = T((T.GridI == 0) & (T.GridJ == 0) & (T.GridK == gridOrder), :);
    vertexIRow = T((T.GridI == gridOrder) & (T.GridJ == 0) & (T.GridK == 0), :);
    vertexJRow = T((T.GridI == 0) & (T.GridJ == gridOrder) & (T.GridK == 0), :);
    if (height(vertexKRow) ~= 1) || (height(vertexIRow) ~= 1) || (height(vertexJRow) ~= 1)
        error('Cannot uniquely determine grid triangle vertices from GridI/GridJ/GridK.');
    end

    gridMeta = struct();
    gridMeta.GridOrder = gridOrder;
    gridMeta.VertexK = [round(vertexKRow.Target_u1976(1) * 1000), round(vertexKRow.Target_v1976(1) * 1000)];
    gridMeta.VertexI = [round(vertexIRow.Target_u1976(1) * 1000), round(vertexIRow.Target_v1976(1) * 1000)];
    gridMeta.VertexJ = [round(vertexJRow.Target_u1976(1) * 1000), round(vertexJRow.Target_v1976(1) * 1000)];
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
