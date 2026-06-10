% S16_FIT_WEIGHTED_3SEG_TEMPERATURE_WHITERATIO_FORMULA
% Purpose:
%   Fit a compact temperature correction formula for BestWhiteRatio.
%
% Model:
%   DeltaW = WT - W25
%   t      = (Temp - baseTempC) / tempScaleC
%   DeltaW = t * F1_seg(x,y,W25) + t^2 * F2_seg(x,y,W25)
%
% W25 segments:
%   SEG_LOW  : W25 < 0.25
%   SEG_MID  : 0.25 <= W25 < 0.80
%   SEG_HIGH : W25 >= 0.80
%
% Basis:
%   [1, x, y, W25, x*y, x*W25, y*W25, W25^2, x^2, y^2]
%
% Note:
%   This version keeps C-side complexity low: only 3 segments and coefficient formulas.
%   Weighted least squares is used only during MATLAB fitting to focus more on larger DeltaW samples.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S16 fit WEIGHTED 3-SEGMENT temperature correction formula\n');
fprintf('Model: DeltaW = t*F1_seg(x,y,W25) + t^2*F2_seg(x,y,W25)\n');
fprintf('============================================================\n\n');

%% ================= User configurable parameters =================
baseLabel = 'T25';
fitTemperatureLabels = {'T60', 'T100'};     % train using T60/T100, validate T45/T80 in S17

temperatureLabelList = {'T25', 'T45', 'T60', 'T80', 'T100'};
temperatureValueList = [21.4, 43.0, 60.0, 80.0, 100.0];
baseTempC = 21.4;
tempScaleC = 100.0 - 21.4;

requireValidBoth = true;
ridgeLambda = 1e-6;

% Weighted least squares parameters.
fitWeightEnable = true;
fitWeightReferenceAbsDelta = 0.03;
fitWeightStrength = 1.0;
fitWeightPower = 1.0;
fitWeightMax = 10.0;

% No delta clamp by default. Keep fields exported for C compatibility.
deltaClampEnable = false;
deltaClampAbs = 0.15;
whiteRatioClampEnable = true;

segmentNames = {'SEG_LOW', 'SEG_MID', 'SEG_HIGH'};
segmentMin = [-1.0e-6, 0.25, 0.80];
segmentMax = [0.25, 0.80, 1.0000001];

basisNames = {'CONST','X','Y','W25','X_MUL_Y','X_MUL_W25','Y_MUL_W25','W25_SQ','X_SQ','Y_SQ'};
basisCount = numel(basisNames);
segmentCount = numel(segmentNames);

%% ================= Locate input =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'triangle_grid_lut_output');
s15Dir = fullfile(projectRoot, 'triangle_grid_lut_output', 's15_temperature_on_25grid');
inputCsv = fullfile(s15Dir, 's15_fixed25grid_temperature_exact_and_delta_full.csv');

if ~exist(inputCsv, 'file')
    error('Missing S15 full result CSV:\n%s\nPlease run S15 first.', inputCsv);
end

outputDir = fullfile(projectRoot, 'triangle_grid_lut_output', 's16_temperature_formula_fit');
if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

T = readtable(inputCsv, 'VariableNamingRule', 'preserve');

%% ================= Basic checks =================
allLabels = detect_temperature_labels(T.Properties.VariableNames);
if ~ismember(baseLabel, allLabels)
    error('Base label %s was not found in S15 CSV.', baseLabel);
end
for i = 1:numel(fitTemperatureLabels)
    if ~ismember(fitTemperatureLabels{i}, allLabels)
        error('Fit label %s was not found in S15 CSV.', fitTemperatureLabels{i});
    end
end

fprintf('Base label      : %s\n', baseLabel);
fprintf('Fit labels      : %s\n', strjoin(fitTemperatureLabels, ', '));
fprintf('Base temp C     : %.3f\n', baseTempC);
fprintf('Temp scale C    : %.3f\n', tempScaleC);
fprintf('Ridge lambda    : %.3g\n', ridgeLambda);
fprintf('Weighted LS     : enable=%u, ref=%.6f, strength=%.3f, power=%.3f, max=%.3f\n', ...
    uint32(fitWeightEnable), fitWeightReferenceAbsDelta, fitWeightStrength, fitWeightPower, fitWeightMax);
fprintf('Segment count   : %u\n\n', uint32(segmentCount));

baseWhiteCol = [baseLabel '_BestWhiteRatio'];
baseValidCol = [baseLabel '_Valid'];
assert_column(T, baseWhiteCol);
assert_column(T, baseValidCol);

xList = T.x;
yList = T.y;
W25List = T.(baseWhiteCol);
baseValidList = logical(T.(baseValidCol));

coefF1 = nan(segmentCount, basisCount);
coefF2 = nan(segmentCount, basisCount);
sampleRows = [];
sampleIndex = 0;

%% ================= Fit each W25 segment independently =================
for segIdx = 1:segmentCount
    A = [];
    yTarget = [];
    segmentSampleInfo = [];
    segmentSampleCount = 0;

    for labelIdx = 1:numel(fitTemperatureLabels)
        label = fitTemperatureLabels{labelIdx};
        tempC = get_temperature_for_label(label, temperatureLabelList, temperatureValueList);
        tNorm = (tempC - baseTempC) / tempScaleC;

        whiteCol = [label '_BestWhiteRatio'];
        validCol = [label '_Valid'];
        deltaCol = [label '_DeltaWhiteRatio_From25C'];
        assert_column(T, whiteCol);
        assert_column(T, validCol);
        assert_column(T, deltaCol);

        currentWhite = T.(whiteCol);
        currentValid = logical(T.(validCol));
        deltaW = T.(deltaCol);

        if requireValidBoth
            validMask = baseValidList & currentValid & isfinite(W25List) & isfinite(currentWhite) & isfinite(deltaW);
        else
            validMask = currentValid & isfinite(W25List) & isfinite(currentWhite) & isfinite(deltaW);
        end

        segMask = validMask & (W25List >= segmentMin(segIdx)) & (W25List < segmentMax(segIdx));
        idxList = find(segMask);
        fprintf('%s / %s: samples used = %u\n', segmentNames{segIdx}, label, uint32(numel(idxList)));

        for ii = 1:numel(idxList)
            rowIdx = idxList(ii);
            basis = make_basis_row(xList(rowIdx), yList(rowIdx), W25List(rowIdx));
            designRow = [tNorm .* basis, (tNorm.^2) .* basis];
            A = [A; designRow]; %#ok<AGROW>
            yTarget = [yTarget; deltaW(rowIdx)]; %#ok<AGROW>

            segmentSampleCount = segmentSampleCount + 1;
            segmentSampleInfo(segmentSampleCount).GridRow = rowIdx; %#ok<SAGROW>
            segmentSampleInfo(segmentSampleCount).Label = label;
            segmentSampleInfo(segmentSampleCount).Temperature_C = tempC;
            segmentSampleInfo(segmentSampleCount).t = tNorm;
            segmentSampleInfo(segmentSampleCount).x = xList(rowIdx);
            segmentSampleInfo(segmentSampleCount).y = yList(rowIdx);
            segmentSampleInfo(segmentSampleCount).W25 = W25List(rowIdx);
            segmentSampleInfo(segmentSampleCount).W_T = currentWhite(rowIdx);
            segmentSampleInfo(segmentSampleCount).DeltaW_Exact = deltaW(rowIdx);
        end
    end

    if size(A, 1) < size(A, 2)
        error('Segment %s has too few samples: %u samples for %u coefficients.', ...
            segmentNames{segIdx}, uint32(size(A,1)), uint32(size(A,2)));
    end

    fitWeight = make_fit_weight(yTarget, fitWeightEnable, fitWeightReferenceAbsDelta, ...
        fitWeightStrength, fitWeightPower, fitWeightMax);
    sqrtWeight = sqrt(fitWeight(:));
    Aw = A .* sqrtWeight;
    yw = yTarget .* sqrtWeight;

    if ridgeLambda > 0
        coef = (Aw.' * Aw + ridgeLambda .* eye(size(Aw, 2))) \ (Aw.' * yw);
    else
        coef = Aw \ yw;
    end

    coefF1(segIdx, :) = coef(1:basisCount).';
    coefF2(segIdx, :) = coef(basisCount+1:end).';

    pred = A * coef;
    if deltaClampEnable
        pred = min(max(pred, -deltaClampAbs), deltaClampAbs);
    end
    err = pred - yTarget;

    for i = 1:numel(segmentSampleInfo)
        sampleIndex = sampleIndex + 1;
        rowIdx = segmentSampleInfo(i).GridRow;
        item.SampleIndex = sampleIndex;
        item.GridRow = rowIdx;
        item.GridI = get_table_value_or_nan(T, 'GridI', rowIdx);
        item.GridJ = get_table_value_or_nan(T, 'GridJ', rowIdx);
        item.GridK = get_table_value_or_nan(T, 'GridK', rowIdx);
        item.SegmentIndex = segIdx;
        item.SegmentName = segmentNames{segIdx};
        item.Label = segmentSampleInfo(i).Label;
        item.Temperature_C = segmentSampleInfo(i).Temperature_C;
        item.t = segmentSampleInfo(i).t;
        item.x = segmentSampleInfo(i).x;
        item.y = segmentSampleInfo(i).y;
        item.W25 = segmentSampleInfo(i).W25;
        item.W_T = segmentSampleInfo(i).W_T;
        item.DeltaW_Exact = segmentSampleInfo(i).DeltaW_Exact;
        item.FitWeight = fitWeight(i);
        item.DeltaW_Pred = pred(i);
        item.DeltaW_Error = err(i);
        item.AbsDeltaW_Error = abs(err(i));
        item.W_Corrected = item.W25 + pred(i);
        if whiteRatioClampEnable
            item.W_Corrected = min(max(item.W_Corrected, 0), 1);
        end
        item.W_ErrorAfterCorrection = item.W_Corrected - item.W_T;
        item.AbsW_ErrorAfterCorrection = abs(item.W_ErrorAfterCorrection);
        item.AbsW_ErrorBeforeCorrection = abs(item.W25 - item.W_T);
        sampleRows = append_struct(sampleRows, item); %#ok<AGROW>
    end
end

%% ================= Export coefficient table and C header =================
coefTable = make_coeff_table(coefF1, coefF2, basisNames, segmentNames, segmentMin, segmentMax, ...
    baseTempC, tempScaleC, ridgeLambda, fitWeightEnable, fitWeightReferenceAbsDelta, ...
    fitWeightStrength, fitWeightPower, fitWeightMax, deltaClampEnable, deltaClampAbs);
coefCsv = fullfile(outputDir, 's16_temperature_formula_coefficients.csv');
writetable(coefTable, coefCsv);

export_c_header(fullfile(outputDir, 's16_temperature_formula_coefficients.h'), ...
    coefF1, coefF2, basisNames, segmentNames, segmentMin, segmentMax, baseTempC, tempScaleC, deltaClampEnable, deltaClampAbs);

%% ================= Export fit samples and summary =================
sampleTable = struct2table(sampleRows);
sampleCsv = fullfile(outputDir, 's16_temperature_formula_fit_samples.csv');
writetable(sampleTable, sampleCsv);

summaryTable = build_summary_table(sampleTable);
summaryCsv = fullfile(outputDir, 's16_temperature_formula_fit_summary.csv');
writetable(summaryTable, summaryCsv);

fprintf('\nFit finished. Output files:\n');
fprintf('  Coefficients CSV : %s\n', coefCsv);
fprintf('  C header         : %s\n', fullfile(outputDir, 's16_temperature_formula_coefficients.h'));
fprintf('  Fit samples      : %s\n', sampleCsv);
fprintf('  Fit summary      : %s\n', summaryCsv);

fprintf('\nOverall fitting error on DeltaWhiteRatio:\n');
fprintf('  MeanAbs = %.8f\n', mean(sampleTable.AbsDeltaW_Error));
fprintf('  P95Abs  = %.8f\n', local_percentile(sampleTable.AbsDeltaW_Error, 95));
fprintf('  MaxAbs  = %.8f\n', max(sampleTable.AbsDeltaW_Error));
fprintf('  RMSE    = %.8f\n', sqrt(mean(sampleTable.DeltaW_Error.^2)));

%% ================= Local functions =================
function basis = make_basis_row(x, y, W25)
    basis = [1, x, y, W25, x .* y, x .* W25, y .* W25, W25 .* W25, x .* x, y .* y];
end

function w = make_fit_weight(deltaW, enable, refAbsDelta, strength, powerVal, maxWeight)
    if ~enable
        w = ones(size(deltaW));
        return;
    end
    refAbsDelta = max(refAbsDelta, eps);
    z = abs(deltaW(:)) ./ refAbsDelta;
    w = 1.0 + strength .* (z .^ powerVal);
    w = min(max(w, 1.0), maxWeight);
end

function labels = detect_temperature_labels(varNames)
    labels = {};
    for i = 1:numel(varNames)
        token = regexp(varNames{i}, '^(T\d+)_BestWhiteRatio$', 'tokens', 'once');
        if ~isempty(token)
            labels{end+1} = token{1}; %#ok<AGROW>
        end
    end
    labels = sort_temperature_labels(labels);
end

function labels = sort_temperature_labels(labels)
    temps = zeros(numel(labels), 1);
    for i = 1:numel(labels)
        temps(i) = parse_temperature_from_label(labels{i});
    end
    [~, order] = sort(temps);
    labels = labels(order);
end

function t = parse_temperature_from_label(label)
    token = regexp(char(label), '^T(\d+)$', 'tokens', 'once');
    if isempty(token)
        error('Invalid temperature label: %s', char(label));
    end
    t = str2double(token{1});
end

function tempC = get_temperature_for_label(label, labelList, valueList)
    idx = find(strcmp(labelList, char(label)), 1);
    if isempty(idx)
        tempC = parse_temperature_from_label(label);
    else
        tempC = valueList(idx);
    end
end

function assert_column(T, name)
    if ~ismember(name, T.Properties.VariableNames)
        error('Missing required column: %s', name);
    end
end

function v = get_table_value_or_nan(T, colName, rowIdx)
    if ismember(colName, T.Properties.VariableNames)
        v = T.(colName)(rowIdx);
    else
        v = NaN;
    end
end

function out = append_struct(out, item)
    if isempty(out)
        out = item;
    else
        out(end + 1) = item;
    end
end

function coefTable = make_coeff_table(coefF1, coefF2, basisNames, segmentNames, segmentMin, segmentMax, ...
    baseTempC, tempScaleC, ridgeLambda, fitWeightEnable, fitWeightReferenceAbsDelta, ...
    fitWeightStrength, fitWeightPower, fitWeightMax, deltaClampEnable, deltaClampAbs)
    rows = [];
    idx = 0;
    for segIdx = 1:numel(segmentNames)
        for groupIdx = 1:2
            if groupIdx == 1
                groupName = 'F1';
                values = coefF1(segIdx, :);
            else
                groupName = 'F2';
                values = coefF2(segIdx, :);
            end
            for b = 1:numel(basisNames)
                idx = idx + 1;
                r.CoefficientIndex = idx;
                r.SegmentIndex = segIdx;
                r.SegmentName = segmentNames{segIdx};
                r.W25Min = segmentMin(segIdx);
                r.W25Max = segmentMax(segIdx);
                r.TermGroup = groupName;
                r.BasisIndex = b;
                r.BasisName = basisNames{b};
                r.Coefficient = values(b);
                r.BaseTempC = baseTempC;
                r.TempScaleC = tempScaleC;
                r.RidgeLambda = ridgeLambda;
                r.FitWeightEnable = logical(fitWeightEnable);
                r.FitWeightReferenceAbsDelta = fitWeightReferenceAbsDelta;
                r.FitWeightStrength = fitWeightStrength;
                r.FitWeightPower = fitWeightPower;
                r.FitWeightMax = fitWeightMax;
                r.DeltaClampEnable = logical(deltaClampEnable);
                r.DeltaClampAbs = deltaClampAbs;
                rows = append_struct(rows, r); %#ok<AGROW>
            end
        end
    end
    coefTable = struct2table(rows);
end

function export_c_header(filePath, coefF1, coefF2, basisNames, segmentNames, segmentMin, segmentMax, baseTempC, tempScaleC, deltaClampEnable, deltaClampAbs)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open C header for writing: %s', filePath);
    end
    cleanupObj = onCleanup(@() fclose(fid)); %#ok<NASGU>

    fprintf(fid, '/* Auto-generated by s16.m */\n');
    fprintf(fid, '#ifndef ALGO_TEMP_WHITERATIO_COEF_H\n');
    fprintf(fid, '#define ALGO_TEMP_WHITERATIO_COEF_H\n\n');
    fprintf(fid, '#define ALGO_TEMP_BASE_C       (%.9ff)\n', baseTempC);
    fprintf(fid, '#define ALGO_TEMP_SCALE_C      (%.9ff)\n', tempScaleC);
    fprintf(fid, '#define ALGO_TEMP_SEG_COUNT    (%uU)\n', uint32(size(coefF1,1)));
    fprintf(fid, '#define ALGO_TEMP_BASIS_COUNT  (%uU)\n', uint32(size(coefF1,2)));
    fprintf(fid, '#define ALGO_TEMP_DELTA_CLAMP_ENABLE (%uU)\n', uint32(deltaClampEnable));
    fprintf(fid, '#define ALGO_TEMP_DELTA_CLAMP_ABS    (%.9ff)\n\n', deltaClampAbs);
    fprintf(fid, '/* Basis: ');
    for i = 1:numel(basisNames)
        if i > 1
            fprintf(fid, ', ');
        end
        fprintf(fid, '%s', basisNames{i});
    end
    fprintf(fid, ' */\n\n');

    fprintf(fid, 'static const float g_AlgoTempW25Min[ALGO_TEMP_SEG_COUNT] = { ');
    for i = 1:numel(segmentMin)
        if i > 1, fprintf(fid, ', '); end
        fprintf(fid, '%.9ff', segmentMin(i));
    end
    fprintf(fid, ' };\n');

    fprintf(fid, 'static const float g_AlgoTempW25Max[ALGO_TEMP_SEG_COUNT] = { ');
    for i = 1:numel(segmentMax)
        if i > 1, fprintf(fid, ', '); end
        fprintf(fid, '%.9ff', segmentMax(i));
    end
    fprintf(fid, ' };\n\n');

    write_matrix(fid, 'g_AlgoTempCoefF1', coefF1, segmentNames);
    write_matrix(fid, 'g_AlgoTempCoefF2', coefF2, segmentNames);
    fprintf(fid, '#endif /* ALGO_TEMP_WHITERATIO_COEF_H */\n');
end

function write_matrix(fid, name, M, segmentNames)
    fprintf(fid, 'static const float %s[ALGO_TEMP_SEG_COUNT][ALGO_TEMP_BASIS_COUNT] =\n{\n', name);
    for r = 1:size(M,1)
        fprintf(fid, '    { ');
        for c = 1:size(M,2)
            if c > 1, fprintf(fid, ', '); end
            fprintf(fid, '% .9ef', M(r,c));
        end
        fprintf(fid, ' }');
        if r < size(M,1), fprintf(fid, ','); end
        fprintf(fid, ' /* %s */\n', segmentNames{r});
    end
    fprintf(fid, '};\n\n');
end

function summaryTable = build_summary_table(sampleTable)
    labels = unique(cellstr(sampleTable.Label), 'stable');
    segments = unique(sampleTable.SegmentIndex, 'stable');
    groupNames = {};
    labelNames = {};
    segmentIds = [];
    sampleCounts = [];
    meanBefore = [];
    meanAfter = [];
    meanDeltaAbs = [];
    p95DeltaAbs = [];
    maxDeltaAbs = [];
    rmseDelta = [];

    add_group('ALL', 'ALL', NaN, true(height(sampleTable),1));

    for i = 1:numel(labels)
        mask = strcmp(cellstr(sampleTable.Label), labels{i});
        add_group(['LABEL_' labels{i}], labels{i}, NaN, mask);
    end

    for i = 1:numel(segments)
        mask = sampleTable.SegmentIndex == segments(i);
        add_group(['SEGMENT_' num2str(segments(i))], 'ALL', segments(i), mask);
    end

    summaryTable = table(groupNames(:), labelNames(:), segmentIds(:), sampleCounts(:), ...
        meanBefore(:), meanAfter(:), meanDeltaAbs(:), p95DeltaAbs(:), maxDeltaAbs(:), rmseDelta(:), ...
        'VariableNames', {'Group','Label','SegmentIndex','SampleCount', ...
        'MeanAbsWhiteRatioErrorBefore','MeanAbsWhiteRatioErrorAfter', ...
        'MeanAbsDeltaFitError','P95AbsDeltaFitError','MaxAbsDeltaFitError','RMSE_DeltaFitError'});

    function add_group(groupName, labelName, segId, mask)
        sub = sampleTable(mask, :);
        groupNames{end+1} = groupName; %#ok<AGROW>
        labelNames{end+1} = labelName; %#ok<AGROW>
        segmentIds(end+1) = segId; %#ok<AGROW>
        sampleCounts(end+1) = height(sub); %#ok<AGROW>
        meanBefore(end+1) = mean(sub.AbsW_ErrorBeforeCorrection); %#ok<AGROW>
        meanAfter(end+1) = mean(sub.AbsW_ErrorAfterCorrection); %#ok<AGROW>
        meanDeltaAbs(end+1) = mean(sub.AbsDeltaW_Error); %#ok<AGROW>
        p95DeltaAbs(end+1) = local_percentile(sub.AbsDeltaW_Error, 95); %#ok<AGROW>
        maxDeltaAbs(end+1) = max(sub.AbsDeltaW_Error); %#ok<AGROW>
        rmseDelta(end+1) = sqrt(mean(sub.DeltaW_Error .^ 2)); %#ok<AGROW>
    end
end

function p = local_percentile(x, pct)
    x = sort(x(isfinite(x)));
    if isempty(x)
        p = NaN;
        return;
    end
    if numel(x) == 1
        p = x(1);
        return;
    end
    pos = 1 + (pct / 100) * (numel(x) - 1);
    lo = floor(pos);
    hi = ceil(pos);
    if lo == hi
        p = x(lo);
    else
        p = x(lo) + (pos - lo) * (x(hi) - x(lo));
    end
end

function projectRoot = find_project_root(startDir, folderName)
    projectRoot = startDir;
    while true
        if exist(fullfile(projectRoot, folderName), 'dir')
            return;
        end
        parentDir = fileparts(projectRoot);
        if strcmp(parentDir, projectRoot)
            error('Cannot find %s from script folder or its parent folders.', folderName);
        end
        projectRoot = parentDir;
    end
end
