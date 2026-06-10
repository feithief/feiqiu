% S16_OUTPUT
% One-click sync of SF2 temperature-correction coefficients into C code.
%
% Workflow:
%   1. Run s16_all.m (preferred) or generate the latest coefficient files.
%   2. Run s16_output.m
%   3. Rebuild the Xiaoli firmware project
%
% Priority:
%   - Use s16_all header if it exists
%   - Otherwise fall back to s16_all CSV
%
% Scope:
%   - Update BaseTempC
%   - Update TempScaleC
%   - Update segment count / basis count
%   - Update F1/F2 coefficient arrays
%
% It does not change the 25C LUT or the S14 local-fit body.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S16_OUTPUT: sync s16_all temperature-correction parameters\n');
fprintf('Source : triangle_grid_lut_output/s16_all_temperature_formula_fit\n');
fprintf('============================================================\n\n');

scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
repoRoot = fileparts(projectRoot);
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');

coeffHeaderFile = fullfile(outputDir, 's16_all_temperature_formula_fit', ...
    's16_all_temperature_formula_coefficients.h');
coeffCsvFile = fullfile(outputDir, 's16_all_temperature_formula_fit', ...
    's16_all_temperature_formula_coefficients.csv');
firmwareRoot = find_xiaoli_firmware_root(repoRoot);
algorithmFile = fullfile(firmwareRoot, 'Algorithm', 'Algorithm.c');

if ~exist(algorithmFile, 'file')
    error('Missing target Algorithm.c:\n%s', algorithmFile);
end

fprintf('Resolved target:\n  %s\n\n', algorithmFile);

model = load_temperature_formula_model(coeffHeaderFile, coeffCsvFile);

fprintf('Loaded source model:\n');
fprintf('  BaseTempC   = %.12g\n', model.BaseTempC);
fprintf('  TempScaleC  = %.12g\n', model.TempScaleC);
fprintf('  Segments    = %u\n', model.SegmentCount);
fprintf('  Basis count = %u\n', model.BasisCount);
fprintf('  Source      = %s\n\n', model.SourcePath);

algorithmText = fileread(algorithmFile);
updatedText = update_algorithm_temperature_coefficients(algorithmText, model);

backupFile = [algorithmFile, '.bak_s16_output_last'];
copyfile(algorithmFile, backupFile);
write_text_file(algorithmFile, updatedText);

fprintf('Updated C file:\n  %s\n', algorithmFile);
fprintf('Backup saved:\n  %s\n', backupFile);
fprintf('\nDone. Next step: Rebuild the Xiaoli firmware project.\n');

function model = load_temperature_formula_model(coeffHeaderFile, coeffCsvFile)
    if exist(coeffHeaderFile, 'file')
        model = parse_temperature_formula_header(coeffHeaderFile);
        return;
    end

    if exist(coeffCsvFile, 'file')
        model = parse_temperature_formula_csv(coeffCsvFile);
        return;
    end

    error(['Cannot find s16_all coefficient source.\n' ...
        'Checked:\n%s\n%s'], coeffHeaderFile, coeffCsvFile);
end

function model = parse_temperature_formula_header(coeffHeaderFile)
    headerText = fileread(coeffHeaderFile);

    model = struct();
    model.SourcePath = coeffHeaderFile;
    model.BaseTempC = parse_define_number(headerText, 'S16_TEMP_BASE_C');
    model.TempScaleC = parse_define_number(headerText, 'S16_TEMP_SCALE_C');
    model.SegmentCount = uint32(parse_define_integer(headerText, 'S16_TEMP_SEGMENT_COUNT'));
    model.BasisCount = uint32(parse_define_integer(headerText, 'S16_TEMP_BASIS_COUNT'));
    model.F1 = parse_double_matrix_block(headerText, ...
        'g_s16_temp_f1_coef', model.SegmentCount, model.BasisCount);
    model.F2 = parse_double_matrix_block(headerText, ...
        'g_s16_temp_f2_coef', model.SegmentCount, model.BasisCount);
end

function model = parse_temperature_formula_csv(coeffCsvFile)
    C = readtable(coeffCsvFile, 'VariableNamingRule', 'preserve');
    requiredCols = {'SegmentIndex','Term','Basis','Coefficient'};
    for i = 1:numel(requiredCols)
        if ~ismember(requiredCols{i}, C.Properties.VariableNames)
            error('Coefficient CSV missing column: %s', requiredCols{i});
        end
    end

    basisOrder = {'CONST','X','Y','W25','X_MUL_Y','X_MUL_W25','Y_MUL_W25','W25_SQ','X_SQ','Y_SQ'};
    segmentOrder = unique(C.SegmentIndex, 'stable');

    model = struct();
    model.SourcePath = coeffCsvFile;
    model.BaseTempC = 21.4;
    model.TempScaleC = 78.6;
    if ismember('BaseTempC', C.Properties.VariableNames) && ~isempty(C.BaseTempC)
        model.BaseTempC = C.BaseTempC(find(~isnan(C.BaseTempC), 1, 'first'));
    end
    if ismember('TempScaleC', C.Properties.VariableNames) && ~isempty(C.TempScaleC)
        model.TempScaleC = C.TempScaleC(find(~isnan(C.TempScaleC), 1, 'first'));
    end

    model.SegmentCount = uint32(numel(segmentOrder));
    model.BasisCount = uint32(numel(basisOrder));
    model.F1 = zeros(double(model.SegmentCount), double(model.BasisCount));
    model.F2 = zeros(double(model.SegmentCount), double(model.BasisCount));

    for segIdx = 1:numel(segmentOrder)
        segValue = segmentOrder(segIdx);
        for basisIdx = 1:numel(basisOrder)
            basisName = basisOrder{basisIdx};

            f1Mask = (C.SegmentIndex == segValue) & strcmp(C.Term, 'F1') & strcmp(C.Basis, basisName);
            f2Mask = (C.SegmentIndex == segValue) & strcmp(C.Term, 'F2') & strcmp(C.Basis, basisName);

            if nnz(f1Mask) ~= 1
                error('Cannot uniquely find F1 coeff for segment=%d basis=%s', segValue, basisName);
            end
            if nnz(f2Mask) ~= 1
                error('Cannot uniquely find F2 coeff for segment=%d basis=%s', segValue, basisName);
            end

            model.F1(segIdx, basisIdx) = C.Coefficient(find(f1Mask, 1, 'first'));
            model.F2(segIdx, basisIdx) = C.Coefficient(find(f2Mask, 1, 'first'));
        end
    end
end

function value = parse_define_number(text, macroName)
    pattern = ['#define\s+', regexptranslate('escape', macroName), '\s+\(([-+0-9.eE]+)(?:u)?\)'];
    tokens = regexp(text, pattern, 'tokens', 'once');
    if isempty(tokens)
        error('Cannot parse macro %s', macroName);
    end
    value = str2double(tokens{1});
end

function value = parse_define_integer(text, macroName)
    value = round(parse_define_number(text, macroName));
end

function matrix = parse_double_matrix_block(text, arrayName, rowCount, colCount)
    pattern = ['static const double\s+', regexptranslate('escape', arrayName), ...
        '\[[^\]]+\]\[[^\]]+\]\s*=\s*\{([\s\S]*?)\n\};'];
    tokens = regexp(text, pattern, 'tokens', 'once');
    if isempty(tokens)
        error('Cannot parse array block: %s', arrayName);
    end

    blockText = tokens{1};
    blockText = regexprep(blockText, '/\*.*?\*/', '');
    numberTokens = regexp(blockText, '[-+]?(?:\d+\.\d*|\d+|\.\d+)(?:[eE][-+]?\d+)?', 'match');

    expectedCount = double(rowCount) * double(colCount);
    if numel(numberTokens) ~= expectedCount
        error('Array %s expected %d numbers but got %d.', arrayName, expectedCount, numel(numberTokens));
    end

    values = str2double(numberTokens);
    matrix = reshape(values, double(colCount), double(rowCount)).';
end

function updatedText = update_algorithm_temperature_coefficients(algorithmText, model)
    updatedText = algorithmText;

    updatedText = regexprep(updatedText, ...
        '#define ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT\s+\(\d+u\)', ...
        sprintf('#define ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT      (%uu)', uint32(model.SegmentCount)), ...
        'once');

    updatedText = regexprep(updatedText, ...
        '#define ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT\s+\(\d+u\)', ...
        sprintf('#define ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT        (%uu)', uint32(model.BasisCount)), ...
        'once');

    updatedText = regexprep(updatedText, ...
        'static const fix16_t gAlgoSonderfunktion2TempBaseC = F16\([^\)]+\);', ...
        sprintf('static const fix16_t gAlgoSonderfunktion2TempBaseC = F16(%s);', format_number(model.BaseTempC)), ...
        'once');

    updatedText = regexprep(updatedText, ...
        'static const fix16_t gAlgoSonderfunktion2TempScaleC = F16\([^\)]+\);', ...
        sprintf('static const fix16_t gAlgoSonderfunktion2TempScaleC = F16(%s);', format_number(model.TempScaleC)), ...
        'once');

    f1Pattern = ['static const fix16_t gAlgoSonderfunktion2TempCoefF1\[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT\]' ...
        '\[ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT\]\s*=\s*\{\r?\n[\s\S]*?\r?\n\};'];
    f1Replacement = sprintf(['static const fix16_t gAlgoSonderfunktion2TempCoefF1[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT]' ...
        '[ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT] =\r\n{\r\n%s\r\n};'], ...
        build_fix16_matrix_rows(model.F1));
    updatedText = regexprep(updatedText, f1Pattern, f1Replacement, 'once');

    f2Pattern = ['static const fix16_t gAlgoSonderfunktion2TempCoefF2\[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT\]' ...
        '\[ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT\]\s*=\s*\{\r?\n[\s\S]*?\r?\n\};'];
    f2Replacement = sprintf(['static const fix16_t gAlgoSonderfunktion2TempCoefF2[ALGO_SONDERFUNKTION2_TEMP_SEGMENT_COUNT]' ...
        '[ALGO_SONDERFUNKTION2_TEMP_BASIS_COUNT] =\r\n{\r\n%s\r\n};'], ...
        build_fix16_matrix_rows(model.F2));
    updatedText = regexprep(updatedText, f2Pattern, f2Replacement, 'once');
end

function rowsText = build_fix16_matrix_rows(matrix)
    rowCount = size(matrix, 1);
    colCount = size(matrix, 2);
    lines = strings(rowCount, 1);

    for rowIdx = 1:rowCount
        items = strings(1, colCount);
        for colIdx = 1:colCount
            items(colIdx) = sprintf('F16(%s)', format_number(matrix(rowIdx, colIdx)));
        end
        if rowIdx < rowCount
            lines(rowIdx) = sprintf('    { %s },', strjoin(cellstr(items), ', '));
        else
            lines(rowIdx) = sprintf('    { %s }', strjoin(cellstr(items), ', '));
        end
    end

    rowsText = strjoin(cellstr(lines), sprintf('\r\n'));
end

function out = format_number(value)
    out = sprintf('%.17g', value);
    out = regexprep(out, 'e([+-])0*(\d+)', 'e$1$2');
end

function write_text_file(filePath, outputText)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open output file: %s', filePath);
    end
    cleanupObj = onCleanup(@() fclose(fid)); %#ok<NASGU>
    fwrite(fid, outputText, 'char');
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

function firmwareRoot = find_xiaoli_firmware_root(repoRoot)
    rgbRoot = fullfile(repoRoot, 'RGB');
    candidates = dir(fullfile(rgbRoot, '20260528*'));
    firmwareRoot = '';

    for i = 1:numel(candidates)
        if ~candidates(i).isdir
            continue;
        end

        probeDir = fullfile(candidates(i).folder, candidates(i).name, 'BLINKY_SIMPLE_EXAMPLE_TLD4020');
        if exist(fullfile(probeDir, 'Algorithm', 'Algorithm.c'), 'file')
            firmwareRoot = probeDir;
            return;
        end
    end

    error(['Cannot locate Xiaoli firmware directory under:\n%s\n' ...
        'Expected something like RGB/20260528*/BLINKY_SIMPLE_EXAMPLE_TLD4020'], rgbRoot);
end
