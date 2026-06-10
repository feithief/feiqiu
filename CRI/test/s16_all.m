% S16_ALL_FIT_TEMPERATURE_WHITERATIO_FORMULA
% Purpose:
%   Use ALL non-base temperatures of sample/product #1 to fit a compact
%   temperature correction formula for BestWhiteRatio.
%
% Model:
%   DeltaW = WT - W25
%   t      = (Temp - baseTempC) / tempScaleC
%   DeltaW = t*F1(x,y,W25) + t^2*F2(x,y,W25)
%
% Engineering version:
%   - 3 W25 segments
%   - 10 basis terms
%   - weighted least squares
%   - no high-temperature LUT array
%
% Usage:
%   1. Put product #1 data in guangpu_temperature_input and run s15 first.
%   2. Run this script.
%   3. Keep the generated coefficients. Then replace data with product #2,
%      run s15, and run s17_all to cross-validate product #2.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S16_ALL fit temperature correction formula using ALL temperatures\n');
fprintf('Model: DeltaW = t*F1(x,y,W25) + t^2*F2(x,y,W25)\n');
fprintf('Formula: 3 W25 segments + 10 basis + weighted least squares\n');
fprintf('Version       : W25 clamp before segmentation and basis, 2026-05-29 fix\n');
fprintf('============================================================\n\n');

%% ================= User configurable parameters =================
baseLabel = 'T25';
fitTemperatureLabels = {'T45','T60','T80','T100'};  % all non-base temperatures

% Actual MCU temperature mapping. Keep labels for folder/column names, but use
% the real MCU temperature values for formula fitting.
baseTempC = 21.4;
tempScaleC = 100.0 - 21.4;

% W25 segmentation. Segment 1: LOW, 2: MID, 3: HIGH.
segmentNames = {'LOW','MID','HIGH'};
segmentLower = [0.00, 0.25, 0.80];
segmentUpper = [0.25, 0.80, 1.00];

% Weighted least squares: larger |DeltaW| gets larger weight, to improve P95/Max.
useWeightedFit = true;
weightGain = 3.0;
weightScale = 0.05;
weightMax = 5.0;

% Small ridge improves numerical stability without adding C-side complexity.
ridgeLambda = 1e-8;
requireValidBoth = true;

%% ================= Locate project and input =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'triangle_grid_lut_output');
s15Dir = fullfile(projectRoot, 'triangle_grid_lut_output', 's15_temperature_on_25grid');
inputCsv = fullfile(s15Dir, 's15_fixed25grid_temperature_exact_and_delta_full.csv');

if ~exist(inputCsv, 'file')
    error('Missing S15 full result CSV:\n%s\nPlease run s15 first.', inputCsv);
end

outputDir = fullfile(projectRoot, 'triangle_grid_lut_output', 's16_all_temperature_formula_fit');
if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

T = readtable(inputCsv, 'VariableNamingRule', 'preserve');

%% ================= Check labels =================
allLabels = detect_temperature_labels(T.Properties.VariableNames);
if ~ismember(baseLabel, allLabels)
    error('Base label %s was not found in S15 CSV.', baseLabel);
end

fitLabels = fitTemperatureLabels;
for i = 1:numel(fitLabels)
    if ~ismember(fitLabels{i}, allLabels)
        error('Fit label %s was not found in S15 CSV.', fitLabels{i});
    end
end

fprintf('Base label      : %s\n', baseLabel);
fprintf('Fit labels      : %s\n', strjoin(fitLabels, ', '));
fprintf('Base temp C     : %.3f\n', baseTempC);
fprintf('Temp scale C    : %.3f\n', tempScaleC);
fprintf('Segment count   : %u\n', uint32(numel(segmentNames)));
fprintf('Basis count     : 10\n');
fprintf('Weighted fit    : %u\n', uint32(useWeightedFit));
fprintf('Ridge lambda    : %.3g\n\n', ridgeLambda);

%% ================= Build fitting samples =================
baseWhiteCol = [baseLabel '_BestWhiteRatio'];
baseValidCol = [baseLabel '_Valid'];
assert_column(T, baseWhiteCol);
assert_column(T, baseValidCol);

xList = T.x;
yList = T.y;
W25List = T.(baseWhiteCol);
baseValidList = logical(T.(baseValidCol));

SampleIndex = [];
GridRow = [];
GridI = [];
GridJ = [];
GridK = [];
Label = {};
Temperature_C = [];
tNormList = [];
X = [];
Y = [];
W25 = [];
W_T = [];
DeltaW_Exact = [];
SegmentIndex = [];
SegmentName = {};
SampleWeight = [];

sampleCount = 0;
for labelIdx = 1:numel(fitLabels)
    label = fitLabels{labelIdx};
    tempC = actual_temperature_from_label(label);
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

    idxList = find(validMask);
    fprintf('%s: samples used = %u\n', label, uint32(numel(idxList)));

    for ii = 1:numel(idxList)
        rowIdx = idxList(ii);
        sampleCount = sampleCount + 1;
        W25Safe = clamp01(W25List(rowIdx));
        segIdx = find_segment(W25Safe, segmentLower, segmentUpper);
        wt = calc_sample_weight(deltaW(rowIdx), useWeightedFit, weightGain, weightScale, weightMax);

        SampleIndex(end+1,1) = sampleCount; %#ok<SAGROW>
        GridRow(end+1,1) = rowIdx; %#ok<SAGROW>
        GridI(end+1,1) = get_table_value_or_nan(T, 'GridI', rowIdx); %#ok<SAGROW>
        GridJ(end+1,1) = get_table_value_or_nan(T, 'GridJ', rowIdx); %#ok<SAGROW>
        GridK(end+1,1) = get_table_value_or_nan(T, 'GridK', rowIdx); %#ok<SAGROW>
        Label{end+1,1} = label; %#ok<SAGROW>
        Temperature_C(end+1,1) = tempC; %#ok<SAGROW>
        tNormList(end+1,1) = tNorm; %#ok<SAGROW>
        X(end+1,1) = xList(rowIdx); %#ok<SAGROW>
        Y(end+1,1) = yList(rowIdx); %#ok<SAGROW>
        W25(end+1,1) = W25Safe; %#ok<SAGROW>
        W_T(end+1,1) = currentWhite(rowIdx); %#ok<SAGROW>
        DeltaW_Exact(end+1,1) = deltaW(rowIdx); %#ok<SAGROW>
        SegmentIndex(end+1,1) = segIdx; %#ok<SAGROW>
        SegmentName{end+1,1} = segmentNames{segIdx}; %#ok<SAGROW>
        SampleWeight(end+1,1) = wt; %#ok<SAGROW>
    end
end

if sampleCount == 0
    error('No valid fitting samples were found.');
end

sampleTable = table(SampleIndex, GridRow, GridI, GridJ, GridK, string(Label), Temperature_C, tNormList, X, Y, W25, W_T, DeltaW_Exact, SegmentIndex, string(SegmentName), SampleWeight, ...
    'VariableNames', {'SampleIndex','GridRow','GridI','GridJ','GridK','Label','Temperature_C','t','x','y','W25','W_T','DeltaW_Exact','SegmentIndex','SegmentName','SampleWeight'});

%% ================= Fit coefficients segment by segment =================
basisNames = {'CONST','X','Y','W25','X_MUL_Y','X_MUL_W25','Y_MUL_W25','W25_SQ','X_SQ','Y_SQ'};
nBasis = numel(basisNames);
nSeg = numel(segmentNames);
coefF1 = zeros(nSeg, nBasis);
coefF2 = zeros(nSeg, nBasis);

predDelta = nan(height(sampleTable), 1);

for segIdx = 1:nSeg
    mask = sampleTable.SegmentIndex == segIdx;
    if ~any(mask)
        warning('Segment %s has no samples. Coefficients remain zero.', segmentNames{segIdx});
        continue;
    end

    A = zeros(sum(mask), 2*nBasis);
    yTarget = sampleTable.DeltaW_Exact(mask);
    weights = sampleTable.SampleWeight(mask);
    rows = find(mask);

    for r = 1:numel(rows)
        i = rows(r);
        basis = make_basis_row(sampleTable.x(i), sampleTable.y(i), sampleTable.W25(i));
        t = sampleTable.t(i);
        A(r, :) = [t .* basis, (t.^2) .* basis];
    end

    Aw = A .* sqrt(weights);
    yw = yTarget .* sqrt(weights);

    if ridgeLambda > 0
        coef = (Aw.' * Aw + ridgeLambda .* eye(size(Aw, 2))) \ (Aw.' * yw);
    else
        coef = Aw \ yw;
    end

    coefF1(segIdx, :) = coef(1:nBasis).';
    coefF2(segIdx, :) = coef(nBasis+1:end).';
    predDelta(mask) = A * coef;

    fprintf('Segment %-4s: samples=%u, mean|err|=%.8f, p95|err|=%.8f, max|err|=%.8f\n', ...
        segmentNames{segIdx}, uint32(sum(mask)), mean(abs(predDelta(mask) - yTarget)), ...
        local_percentile(abs(predDelta(mask) - yTarget), 95), max(abs(predDelta(mask) - yTarget)));
end

%% ================= Add prediction columns =================
sampleTable.DeltaW_Pred = predDelta;
sampleTable.DeltaW_Error = sampleTable.DeltaW_Pred - sampleTable.DeltaW_Exact;
sampleTable.AbsDeltaW_Error = abs(sampleTable.DeltaW_Error);
sampleTable.W_Corrected = min(max(sampleTable.W25 + sampleTable.DeltaW_Pred, 0), 1);
sampleTable.W_ErrorBeforeCorrection = sampleTable.W25 - sampleTable.W_T;
sampleTable.AbsW_ErrorBeforeCorrection = abs(sampleTable.W_ErrorBeforeCorrection);
sampleTable.W_ErrorAfterCorrection = sampleTable.W_Corrected - sampleTable.W_T;
sampleTable.AbsW_ErrorAfterCorrection = abs(sampleTable.W_ErrorAfterCorrection);

%% ================= Export coefficients =================
coefTable = make_coeff_table(coefF1, coefF2, basisNames, segmentNames, segmentLower, segmentUpper, baseTempC, tempScaleC, ridgeLambda, useWeightedFit, weightGain, weightScale, weightMax);
coefCsv = fullfile(outputDir, 's16_all_temperature_formula_coefficients.csv');
writetable(coefTable, coefCsv);

export_c_header(fullfile(outputDir, 's16_all_temperature_formula_coefficients.h'), coefF1, coefF2, basisNames, segmentNames, segmentLower, segmentUpper, baseTempC, tempScaleC);

%% ================= Export samples and summary =================
sampleCsv = fullfile(outputDir, 's16_all_temperature_formula_fit_samples.csv');
writetable(sampleTable, sampleCsv);

summaryTable = build_summary_table(sampleTable);
summaryCsv = fullfile(outputDir, 's16_all_temperature_formula_fit_summary.csv');
writetable(summaryTable, summaryCsv);

fprintf('\nFit finished. Output files:\n');
fprintf('  Coefficients CSV : %s\n', coefCsv);
fprintf('  C header         : %s\n', fullfile(outputDir, 's16_all_temperature_formula_coefficients.h'));
fprintf('  Fit samples      : %s\n', sampleCsv);
fprintf('  Fit summary      : %s\n', summaryCsv);

fprintf('\nOverall fitting error on DeltaWhiteRatio:\n');
fprintf('  MeanAbs = %.8f\n', mean(sampleTable.AbsDeltaW_Error));
fprintf('  P95Abs  = %.8f\n', local_percentile(sampleTable.AbsDeltaW_Error, 95));
fprintf('  MaxAbs  = %.8f\n', max(sampleTable.AbsDeltaW_Error));
fprintf('  RMSE    = %.8f\n', sqrt(mean(sampleTable.DeltaW_Error.^2)));

%% ================= Local functions =================
function tempC = actual_temperature_from_label(label)
    switch char(label)
        case 'T25'
            tempC = 21.4;
        case 'T45'
            tempC = 43.0;
        case 'T60'
            tempC = 60.0;
        case 'T80'
            tempC = 80.0;
        case 'T100'
            tempC = 100.0;
        otherwise
            token = regexp(char(label), '^T(-?\d+(?:\.\d+)?)$', 'tokens', 'once');
            if isempty(token)
                error('Cannot parse temperature from label: %s', char(label));
            end
            tempC = str2double(token{1});
    end
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
        temps(i) = actual_temperature_from_label(labels{i});
    end
    [~, order] = sort(temps, 'ascend');
    labels = labels(order);
end

function idx = find_segment(W25, lower, upper)
    W25 = clamp01(W25);
    idx = 1;
    for i = 1:numel(lower)
        isLast = (i == numel(lower));
        if (W25 >= lower(i) && W25 < upper(i)) || (isLast && W25 >= lower(i) && W25 <= upper(i))
            idx = i;
            return;
        end
    end
    if W25 >= upper(end)
        idx = numel(lower);
    end
end

function wt = calc_sample_weight(deltaW, enabled, gain, scale, maxWeight)
    if ~enabled
        wt = 1.0;
        return;
    end
    wt = 1.0 + gain .* min(abs(deltaW) ./ scale, maxWeight - 1.0);
    wt = min(max(wt, 1.0), maxWeight);
end

function basis = make_basis_row(x, y, W25)
    W25 = clamp01(W25);
    basis = [1, x, y, W25, x.*y, x.*W25, y.*W25, W25.*W25, x.*x, y.*y];
end

function y = clamp01(x)
    y = min(max(x, 0), 1);
end

function assert_column(T, columnName)
    if ~ismember(columnName, T.Properties.VariableNames)
        error('Missing column in table: %s', columnName);
    end
end

function value = get_table_value_or_nan(T, col, rowIdx)
    if ismember(col, T.Properties.VariableNames)
        value = T.(col)(rowIdx);
    else
        value = NaN;
    end
end

function coefTable = make_coeff_table(coefF1, coefF2, basisNames, segmentNames, segmentLower, segmentUpper, baseTempC, tempScaleC, ridgeLambda, useWeightedFit, weightGain, weightScale, weightMax)
    SegmentIndex = [];
    SegmentName = {};
    W25_Min = [];
    W25_Max = [];
    Term = {};
    Basis = {};
    Coefficient = [];
    nSeg = numel(segmentNames);
    nBasis = numel(basisNames);
    for s = 1:nSeg
        for termIdx = 1:2
            for b = 1:nBasis
                SegmentIndex(end+1,1) = s; %#ok<SAGROW>
                SegmentName{end+1,1} = segmentNames{s}; %#ok<SAGROW>
                W25_Min(end+1,1) = segmentLower(s); %#ok<SAGROW>
                W25_Max(end+1,1) = segmentUpper(s); %#ok<SAGROW>
                if termIdx == 1
                    Term{end+1,1} = 'F1'; %#ok<SAGROW>
                    Coefficient(end+1,1) = coefF1(s,b); %#ok<SAGROW>
                else
                    Term{end+1,1} = 'F2'; %#ok<SAGROW>
                    Coefficient(end+1,1) = coefF2(s,b); %#ok<SAGROW>
                end
                Basis{end+1,1} = basisNames{b}; %#ok<SAGROW>
            end
        end
    end
    coefTable = table(SegmentIndex, string(SegmentName), W25_Min, W25_Max, string(Term), string(Basis), Coefficient, ...
        'VariableNames', {'SegmentIndex','SegmentName','W25_Min','W25_Max','Term','Basis','Coefficient'});
    coefTable.BaseTempC(:) = baseTempC;
    coefTable.TempScaleC(:) = tempScaleC;
    coefTable.RidgeLambda(:) = ridgeLambda;
    coefTable.UseWeightedFit(:) = logical(useWeightedFit);
    coefTable.WeightGain(:) = weightGain;
    coefTable.WeightScale(:) = weightScale;
    coefTable.WeightMax(:) = weightMax;
end

function export_c_header(filePath, coefF1, coefF2, basisNames, segmentNames, segmentLower, segmentUpper, baseTempC, tempScaleC)
    fid = fopen(filePath, 'w');
    if fid < 0
        error('Cannot open C header for writing: %s', filePath);
    end
    cleanupObj = onCleanup(@() fclose(fid)); %#ok<NASGU>
    nSeg = numel(segmentNames);
    nBasis = numel(basisNames);
    fprintf(fid, '/* Auto-generated by s16_all.m */\n');
    fprintf(fid, '#ifndef S16_ALL_TEMPERATURE_WHITERATIO_FORMULA_COEFFICIENTS_H\n');
    fprintf(fid, '#define S16_ALL_TEMPERATURE_WHITERATIO_FORMULA_COEFFICIENTS_H\n\n');
    fprintf(fid, '#define S16_TEMP_BASE_C        (%.12f)\n', baseTempC);
    fprintf(fid, '#define S16_TEMP_SCALE_C       (%.12f)\n', tempScaleC);
    fprintf(fid, '#define S16_TEMP_SEGMENT_COUNT (%uu)\n', uint32(nSeg));
    fprintf(fid, '#define S16_TEMP_BASIS_COUNT   (%uu)\n\n', uint32(nBasis));
    fprintf(fid, '/* Basis order: 1, x, y, W25, x*y, x*W25, y*W25, W25^2, x^2, y^2 */\n');
    fprintf(fid, 'static const double g_s16_temp_segment_lower[S16_TEMP_SEGMENT_COUNT] = {');
    for s = 1:nSeg
        fprintf(fid, ' %.17g%s', segmentLower(s), comma_if_needed(s, nSeg));
    end
    fprintf(fid, ' };\n');
    fprintf(fid, 'static const double g_s16_temp_segment_upper[S16_TEMP_SEGMENT_COUNT] = {');
    for s = 1:nSeg
        fprintf(fid, ' %.17g%s', segmentUpper(s), comma_if_needed(s, nSeg));
    end
    fprintf(fid, ' };\n\n');
    fprintf(fid, 'static const double g_s16_temp_f1_coef[S16_TEMP_SEGMENT_COUNT][S16_TEMP_BASIS_COUNT] =\n{\n');
    for s = 1:nSeg
        fprintf(fid, '    {');
        for b = 1:nBasis
            fprintf(fid, ' %.17g%s', coefF1(s,b), comma_if_needed(b, nBasis));
        end
        fprintf(fid, ' }%s /* %s */\n', comma_if_needed(s, nSeg), segmentNames{s});
    end
    fprintf(fid, '};\n\n');
    fprintf(fid, 'static const double g_s16_temp_f2_coef[S16_TEMP_SEGMENT_COUNT][S16_TEMP_BASIS_COUNT] =\n{\n');
    for s = 1:nSeg
        fprintf(fid, '    {');
        for b = 1:nBasis
            fprintf(fid, ' %.17g%s', coefF2(s,b), comma_if_needed(b, nBasis));
        end
        fprintf(fid, ' }%s /* %s */\n', comma_if_needed(s, nSeg), segmentNames{s});
    end
    fprintf(fid, '};\n\n');
    fprintf(fid, '#endif /* S16_ALL_TEMPERATURE_WHITERATIO_FORMULA_COEFFICIENTS_H */\n');
end

function s = comma_if_needed(i, n)
    if i < n
        s = ',';
    else
        s = ' ';
    end
end

function summaryTable = build_summary_table(sampleTable)
    labels = unique(sampleTable.Label, 'stable');
    segNames = unique(sampleTable.SegmentName, 'stable');
    rows = {};
    rows(end+1,:) = summarize_one_group(sampleTable, true(height(sampleTable),1), 'ALL', 'ALL', NaN); %#ok<AGROW>
    for i = 1:numel(labels)
        mask = sampleTable.Label == labels(i);
        tempC = sampleTable.Temperature_C(find(mask,1,'first'));
        rows(end+1,:) = summarize_one_group(sampleTable, mask, char(labels(i)), 'ALL', tempC); %#ok<AGROW>
    end
    for s = 1:numel(segNames)
        mask = sampleTable.SegmentName == segNames(s);
        rows(end+1,:) = summarize_one_group(sampleTable, mask, 'ALL', char(segNames(s)), NaN); %#ok<AGROW>
    end
    summaryTable = cell2table(rows, 'VariableNames', {'Label','SegmentName','Temperature_C','SampleCount','MeanBefore','P95Before','MaxBefore','MeanAfter','P95After','MaxAfter','MeanDeltaFit','P95DeltaFit','MaxDeltaFit','RMSEDeltaFit'});
end

function row = summarize_one_group(T, mask, label, segmentName, tempC)
    beforeErr = T.AbsW_ErrorBeforeCorrection(mask);
    afterErr = T.AbsW_ErrorAfterCorrection(mask);
    deltaErr = T.AbsDeltaW_Error(mask);
    row = {string(label), string(segmentName), tempC, sum(mask), ...
        mean(beforeErr), local_percentile(beforeErr,95), max(beforeErr), ...
        mean(afterErr), local_percentile(afterErr,95), max(afterErr), ...
        mean(deltaErr), local_percentile(deltaErr,95), max(deltaErr), sqrt(mean(T.DeltaW_Error(mask).^2))};
end

function p = local_percentile(x, percentValue)
    x = x(isfinite(x));
    if isempty(x)
        p = NaN;
        return;
    end
    x = sort(x(:));
    if numel(x) == 1
        p = x(1);
        return;
    end
    pos = 1 + (percentValue / 100) * (numel(x) - 1);
    lo = floor(pos);
    hi = ceil(pos);
    if lo == hi
        p = x(lo);
    else
        p = x(lo) + (x(hi) - x(lo)) * (pos - lo);
    end
end

function projectRoot = find_project_root(startDir, markerFolderName)
    projectRoot = startDir;
    while true
        if exist(fullfile(projectRoot, markerFolderName), 'dir')
            return;
        end
        parentDir = fileparts(projectRoot);
        if strcmp(parentDir, projectRoot)
            error('Cannot find project root containing folder: %s', markerFolderName);
        end
        projectRoot = parentDir;
    end
end
