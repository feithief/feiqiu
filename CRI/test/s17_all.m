% S17_ALL_VALIDATE_TEMPERATURE_WHITERATIO_FORMULA
% Purpose:
%   Validate coefficients fitted by s16_all on another product/sample.
%
% Usage:
%   1. Run s16_all on product #1 and keep its coefficient file.
%   2. Replace input spectra and xyY arrays with product #2, then run s15.
%   3. Run this script without running s16/s16_all again.
%
% This is cross-product validation: product #1 formula -> product #2 S15 answers.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S17_ALL cross-product validation of temperature WhiteRatio formula\n');
fprintf('============================================================\n\n');

%% ================= User configurable parameters =================
baseLabel = 'T25';
validationTemperatureLabels = {'T45','T60','T80','T100'};
clampWhiteRatio = true;

% Optional DeltaW clamp. Disabled by default because the current best model did
% not need clamp. Enable only if validation shows rare formula blow-up.
deltaClampEnable = false;
deltaClampAbs = 0.15;

%% ================= Locate files =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'triangle_grid_lut_output');

s15Dir = fullfile(projectRoot, 'triangle_grid_lut_output', 's15_temperature_on_25grid');
s16AllDir = fullfile(projectRoot, 'triangle_grid_lut_output', 's16_all_temperature_formula_fit');

s15Csv = fullfile(s15Dir, 's15_fixed25grid_temperature_exact_and_delta_full.csv');
coefCsv = fullfile(s16AllDir, 's16_all_temperature_formula_coefficients.csv');

if ~exist(s15Csv, 'file')
    error('Missing S15 full CSV:\n%s\nRun s15 on the validation product first.', s15Csv);
end
if ~exist(coefCsv, 'file')
    error('Missing S16_ALL coefficient CSV:\n%s\nRun s16_all on the fitting product first, and keep its coefficients.', coefCsv);
end

outputDir = fullfile(projectRoot, 'triangle_grid_lut_output', 's17_all_temperature_formula_validation');
if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

T = readtable(s15Csv, 'VariableNamingRule', 'preserve');
coefTable = readtable(coefCsv, 'VariableNamingRule', 'preserve');
model = load_segmented_coefficients(coefTable);

%% ================= Check labels =================
allLabels = detect_temperature_labels(T.Properties.VariableNames);
if ~ismember(baseLabel, allLabels)
    error('Base label %s was not found in S15 CSV.', baseLabel);
end

validateLabels = validationTemperatureLabels;
for i = 1:numel(validateLabels)
    if ~ismember(validateLabels{i}, allLabels)
        error('Validation label %s was not found in S15 CSV.', validateLabels{i});
    end
end

fprintf('Base label       : %s\n', baseLabel);
fprintf('Validate labels  : %s\n', strjoin(validateLabels, ', '));
fprintf('Base temp C      : %.3f\n', model.baseTempC);
fprintf('Temp scale C     : %.3f\n', model.tempScaleC);
fprintf('Segment count    : %u\n', uint32(numel(model.segmentNames)));
fprintf('Basis count      : %u\n', uint32(numel(model.basisNames)));
fprintf('Delta clamp      : enable=%u, abs=%.6f\n\n', uint32(deltaClampEnable), deltaClampAbs);

%% ================= Validate =================
baseWhiteCol = [baseLabel '_BestWhiteRatio'];
baseValidCol = [baseLabel '_Valid'];
assert_column(T, baseWhiteCol);
assert_column(T, baseValidCol);

xList = T.x;
yList = T.y;
W25List = T.(baseWhiteCol);
baseValidList = logical(T.(baseValidCol));

Index = [];
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
W_Exact = [];
DeltaW_Exact = [];
DeltaW_Pred = [];
W_Corrected = [];
Error_Before = [];
AbsError_Before = [];
Error_After = [];
AbsError_After = [];
Improvement = [];
SegmentIndex = [];
SegmentName = {};

rowCount = 0;
for labelIdx = 1:numel(validateLabels)
    label = validateLabels{labelIdx};
    tempC = actual_temperature_from_label(label);
    tNorm = (tempC - model.baseTempC) / model.tempScaleC;

    whiteCol = [label '_BestWhiteRatio'];
    validCol = [label '_Valid'];
    deltaCol = [label '_DeltaWhiteRatio_From25C'];

    assert_column(T, whiteCol);
    assert_column(T, validCol);
    assert_column(T, deltaCol);

    WExact = T.(whiteCol);
    validMask = baseValidList & logical(T.(validCol)) & isfinite(W25List) & isfinite(WExact) & isfinite(T.(deltaCol));
    idxList = find(validMask);
    fprintf('%s: validation samples = %u\n', label, uint32(numel(idxList)));

    for ii = 1:numel(idxList)
        i = idxList(ii);
        % Floating-point noise can make an exact zero ratio become a tiny
        % negative value (for example -1e-16). Clamp W25 before segment
        % selection, basis construction, and correction application so that
        % near-zero RGB-only points are not misclassified as HIGH segment.
        W25Safe = clamp01(W25List(i));
        [predDelta, segIdx] = predict_delta_w(xList(i), yList(i), W25Safe, tNorm, model);
        if deltaClampEnable
            predDelta = min(max(predDelta, -deltaClampAbs), deltaClampAbs);
        end
        Wcorr = W25Safe + predDelta;
        if clampWhiteRatio
            Wcorr = clamp01(Wcorr);
        end

        rowCount = rowCount + 1;
        Index(end+1,1) = rowCount; %#ok<SAGROW>
        GridRow(end+1,1) = i; %#ok<SAGROW>
        GridI(end+1,1) = get_table_value_or_nan(T, 'GridI', i); %#ok<SAGROW>
        GridJ(end+1,1) = get_table_value_or_nan(T, 'GridJ', i); %#ok<SAGROW>
        GridK(end+1,1) = get_table_value_or_nan(T, 'GridK', i); %#ok<SAGROW>
        Label{end+1,1} = label; %#ok<SAGROW>
        Temperature_C(end+1,1) = tempC; %#ok<SAGROW>
        tNormList(end+1,1) = tNorm; %#ok<SAGROW>
        X(end+1,1) = xList(i); %#ok<SAGROW>
        Y(end+1,1) = yList(i); %#ok<SAGROW>
        W25(end+1,1) = W25Safe; %#ok<SAGROW>
        W_Exact(end+1,1) = WExact(i); %#ok<SAGROW>
        DeltaW_Exact(end+1,1) = T.(deltaCol)(i); %#ok<SAGROW>
        DeltaW_Pred(end+1,1) = predDelta; %#ok<SAGROW>
        W_Corrected(end+1,1) = Wcorr; %#ok<SAGROW>
        Error_Before(end+1,1) = W25Safe - WExact(i); %#ok<SAGROW>
        AbsError_Before(end+1,1) = abs(W25Safe - WExact(i)); %#ok<SAGROW>
        Error_After(end+1,1) = Wcorr - WExact(i); %#ok<SAGROW>
        AbsError_After(end+1,1) = abs(Wcorr - WExact(i)); %#ok<SAGROW>
        Improvement(end+1,1) = AbsError_Before(end) - AbsError_After(end); %#ok<SAGROW>
        SegmentIndex(end+1,1) = segIdx; %#ok<SAGROW>
        SegmentName{end+1,1} = model.segmentNames{segIdx}; %#ok<SAGROW>
    end
end

if rowCount == 0
    error('No validation rows were generated.');
end

pointTable = table(Index, GridRow, GridI, GridJ, GridK, string(Label), Temperature_C, tNormList, X, Y, W25, W_Exact, DeltaW_Exact, DeltaW_Pred, W_Corrected, Error_Before, AbsError_Before, Error_After, AbsError_After, Improvement, SegmentIndex, string(SegmentName), ...
    'VariableNames', {'Index','GridRow','GridI','GridJ','GridK','Label','Temperature_C','t','x','y','W25','W_Exact','DeltaW_Exact','DeltaW_Pred','W_Corrected','Error_Before','AbsError_Before','Error_After','AbsError_After','Improvement','SegmentIndex','SegmentName'});

pointCsv = fullfile(outputDir, 's17_all_temperature_formula_validation_points.csv');
writetable(pointTable, pointCsv);

summaryTable = build_summary_table(pointTable);
summaryCsv = fullfile(outputDir, 's17_all_temperature_formula_validation_summary.csv');
writetable(summaryTable, summaryCsv);

fprintf('\nValidation finished. Output files:\n');
fprintf('  Points  : %s\n', pointCsv);
fprintf('  Summary : %s\n', summaryCsv);

fprintf('\nOverall WhiteRatio error:\n');
fprintf('  Before MeanAbs = %.8f, P95Abs = %.8f, MaxAbs = %.8f\n', ...
    mean(pointTable.AbsError_Before), local_percentile(pointTable.AbsError_Before, 95), max(pointTable.AbsError_Before));
fprintf('  After  MeanAbs = %.8f, P95Abs = %.8f, MaxAbs = %.8f\n', ...
    mean(pointTable.AbsError_After), local_percentile(pointTable.AbsError_After, 95), max(pointTable.AbsError_After));

%% ================= Local functions =================
function model = load_segmented_coefficients(coefTable)
    required = {'SegmentIndex','SegmentName','W25_Min','W25_Max','Term','Basis','Coefficient'};
    for i = 1:numel(required)
        assert_column(coefTable, required{i});
    end

    model.baseTempC = get_first_or_default(coefTable, 'BaseTempC', 21.4);
    model.tempScaleC = get_first_or_default(coefTable, 'TempScaleC', 78.6);
    model.basisNames = {'CONST','X','Y','W25','X_MUL_Y','X_MUL_W25','Y_MUL_W25','W25_SQ','X_SQ','Y_SQ'};
    model.segmentIds = unique(coefTable.SegmentIndex, 'stable');
    nSeg = numel(model.segmentIds);
    nBasis = numel(model.basisNames);
    model.segmentNames = cell(nSeg,1);
    model.segmentLower = zeros(nSeg,1);
    model.segmentUpper = zeros(nSeg,1);
    model.coefF1 = zeros(nSeg,nBasis);
    model.coefF2 = zeros(nSeg,nBasis);

    for s = 1:nSeg
        segId = model.segmentIds(s);
        maskSeg = coefTable.SegmentIndex == segId;
        model.segmentNames{s} = char(coefTable.SegmentName(find(maskSeg,1,'first')));
        model.segmentLower(s) = coefTable.W25_Min(find(maskSeg,1,'first'));
        model.segmentUpper(s) = coefTable.W25_Max(find(maskSeg,1,'first'));
        for b = 1:nBasis
            basisName = model.basisNames{b};
            maskF1 = maskSeg & strcmp(cellstr(coefTable.Term), 'F1') & strcmp(cellstr(coefTable.Basis), basisName);
            maskF2 = maskSeg & strcmp(cellstr(coefTable.Term), 'F2') & strcmp(cellstr(coefTable.Basis), basisName);
            if ~any(maskF1) || ~any(maskF2)
                error('Missing coefficient for segment %s basis %s.', model.segmentNames{s}, basisName);
            end
            model.coefF1(s,b) = coefTable.Coefficient(find(maskF1,1,'first'));
            model.coefF2(s,b) = coefTable.Coefficient(find(maskF2,1,'first'));
        end
    end
end

function [deltaW, segIdx] = predict_delta_w(x, y, W25, tNorm, model)
    W25 = clamp01(W25);
    segIdx = find_segment(W25, model.segmentLower, model.segmentUpper);
    basis = make_basis_row(x, y, W25);
    f1 = sum(model.coefF1(segIdx,:) .* basis);
    f2 = sum(model.coefF2(segIdx,:) .* basis);
    deltaW = tNorm .* f1 + (tNorm.^2) .* f2;
end

function basis = make_basis_row(x, y, W25)
    W25 = clamp01(W25);
    basis = [1, x, y, W25, x.*y, x.*W25, y.*W25, W25.*W25, x.*x, y.*y];
end

function idx = find_segment(W25, lower, upper)
    W25 = clamp01(W25);

    % Default to the first segment for any below-range value after numeric
    % cleanup. This prevents tiny negative zeros from falling through to the
    % last segment.
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

function y = clamp01(x)
    y = min(max(x, 0), 1);
end

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

function assert_column(T, columnName)
    if ~ismember(columnName, T.Properties.VariableNames)
        error('Missing column in table: %s', columnName);
    end
end

function value = get_first_or_default(T, columnName, defaultValue)
    if ismember(columnName, T.Properties.VariableNames)
        value = T.(columnName)(1);
    else
        value = defaultValue;
    end
end

function value = get_table_value_or_nan(T, col, rowIdx)
    if ismember(col, T.Properties.VariableNames)
        value = T.(col)(rowIdx);
    else
        value = NaN;
    end
end

function summaryTable = build_summary_table(pointTable)
    labels = unique(pointTable.Label, 'stable');
    segNames = unique(pointTable.SegmentName, 'stable');
    rows = {};
    rows(end+1,:) = summarize_one_group(pointTable, true(height(pointTable),1), 'ALL', 'ALL', NaN); %#ok<AGROW>
    for i = 1:numel(labels)
        mask = pointTable.Label == labels(i);
        tempC = pointTable.Temperature_C(find(mask,1,'first'));
        rows(end+1,:) = summarize_one_group(pointTable, mask, char(labels(i)), 'ALL', tempC); %#ok<AGROW>
    end
    for s = 1:numel(segNames)
        mask = pointTable.SegmentName == segNames(s);
        rows(end+1,:) = summarize_one_group(pointTable, mask, 'ALL', char(segNames(s)), NaN); %#ok<AGROW>
    end
    summaryTable = cell2table(rows, 'VariableNames', {'Label','SegmentName','Temperature_C','SampleCount','MeanBefore','P95Before','MaxBefore','MeanAfter','P95After','MaxAfter','MeanImprovement','P95Improvement','MaxImprovement'});
end

function row = summarize_one_group(T, mask, label, segmentName, tempC)
    beforeErr = T.AbsError_Before(mask);
    afterErr = T.AbsError_After(mask);
    improve = T.Improvement(mask);
    row = {string(label), string(segmentName), tempC, sum(mask), ...
        mean(beforeErr), local_percentile(beforeErr,95), max(beforeErr), ...
        mean(afterErr), local_percentile(afterErr,95), max(afterErr), ...
        mean(improve), local_percentile(improve,95), max(improve)};
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
