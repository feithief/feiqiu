% S17_VALIDATE_WEIGHTED_3SEG_TEMPERATURE_WHITERATIO_FORMULA
% Purpose:
%   Validate the segmented temperature correction formula fitted by S16.

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S17 validate WEIGHTED 3-SEGMENT temperature WhiteRatio correction formula\n');
fprintf('============================================================\n\n');

%% ================= User configurable parameters =================
baseLabel = 'T25';
validationTemperatureLabels = {'T45', 'T80'};
clampWhiteRatio = true;

%% ================= Locate files =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'triangle_grid_lut_output');

s15Dir = fullfile(projectRoot, 'triangle_grid_lut_output', 's15_temperature_on_25grid');
s16Dir = fullfile(projectRoot, 'triangle_grid_lut_output', 's16_temperature_formula_fit');

s15Csv = fullfile(s15Dir, 's15_fixed25grid_temperature_exact_and_delta_full.csv');
coefCsv = fullfile(s16Dir, 's16_temperature_formula_coefficients.csv');

if ~exist(s15Csv, 'file')
    error('Missing S15 full CSV:\n%s', s15Csv);
end
if ~exist(coefCsv, 'file')
    error('Missing S16 coefficient CSV:\n%s\nPlease run S16 first.', coefCsv);
end

outputDir = fullfile(projectRoot, 'triangle_grid_lut_output', 's17_temperature_formula_validation');
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
for i = 1:numel(validationTemperatureLabels)
    if ~ismember(validationTemperatureLabels{i}, allLabels)
        error('Validation label %s was not found in S15 CSV.', validationTemperatureLabels{i});
    end
end

fprintf('Base label       : %s\n', baseLabel);
fprintf('Validate labels  : %s\n', strjoin(validationTemperatureLabels, ', '));
fprintf('Base temp C      : %.3f\n', model.BaseTempC);
fprintf('Temp scale C     : %.3f\n', model.TempScaleC);
fprintf('Segment count    : %u\n', uint32(model.SegmentCount));
fprintf('Basis count      : %u\n', uint32(model.BasisCount));
fprintf('Delta clamp      : enable=%u, abs=%.6f\n\n', uint32(model.DeltaClampEnable), model.DeltaClampAbs);

%% ================= Validate =================
baseWhiteCol = [baseLabel '_BestWhiteRatio'];
baseValidCol = [baseLabel '_Valid'];
assert_column(T, baseWhiteCol);
assert_column(T, baseValidCol);

xList = T.x;
yList = T.y;
W25List = T.(baseWhiteCol);
baseValidList = logical(T.(baseValidCol));

rows = [];
rowIdx = 0;

for labelIdx = 1:numel(validationTemperatureLabels)
    label = validationTemperatureLabels{labelIdx};
    tempC = get_temperature_for_label(label, model);
    tNorm = (tempC - model.BaseTempC) / model.TempScaleC;

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
        [predDelta, segIdx, segName] = predict_delta_w(xList(i), yList(i), W25List(i), tNorm, model);
        Wcorr = W25List(i) + predDelta;
        if clampWhiteRatio
            Wcorr = min(max(Wcorr, 0), 1);
        end

        rowIdx = rowIdx + 1;
        item.Index = rowIdx;
        item.GridRow = i;
        item.GridI = get_table_value_or_nan(T, 'GridI', i);
        item.GridJ = get_table_value_or_nan(T, 'GridJ', i);
        item.GridK = get_table_value_or_nan(T, 'GridK', i);
        item.SegmentIndex = segIdx;
        item.SegmentName = segName;
        item.Label = label;
        item.Temperature_C = tempC;
        item.t = tNorm;
        item.x = xList(i);
        item.y = yList(i);
        item.W25 = W25List(i);
        item.W_Exact = WExact(i);
        item.DeltaW_Exact = T.(deltaCol)(i);
        item.DeltaW_Pred = predDelta;
        item.W_Corrected = Wcorr;
        item.Error_Before = W25List(i) - WExact(i);
        item.AbsError_Before = abs(item.Error_Before);
        item.Error_After = Wcorr - WExact(i);
        item.AbsError_After = abs(item.Error_After);
        item.Improvement = item.AbsError_Before - item.AbsError_After;
        rows = append_struct(rows, item); %#ok<AGROW>
    end
end

if isempty(rows)
    error('No validation rows were generated.');
end

pointTable = struct2table(rows);
pointCsv = fullfile(outputDir, 's17_temperature_formula_validation_points.csv');
writetable(pointTable, pointCsv);

summaryTable = build_summary_table(pointTable);
summaryCsv = fullfile(outputDir, 's17_temperature_formula_validation_summary.csv');
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
    assert_column(coefTable, 'SegmentIndex');
    assert_column(coefTable, 'SegmentName');
    assert_column(coefTable, 'W25Min');
    assert_column(coefTable, 'W25Max');
    assert_column(coefTable, 'TermGroup');
    assert_column(coefTable, 'BasisIndex');
    assert_column(coefTable, 'Coefficient');

    segmentIndexList = unique(coefTable.SegmentIndex, 'stable');
    segmentCount = numel(segmentIndexList);
    basisCount = max(coefTable.BasisIndex);
    coefF1 = zeros(segmentCount, basisCount);
    coefF2 = zeros(segmentCount, basisCount);
    segmentNames = cell(segmentCount, 1);
    segmentMin = zeros(segmentCount, 1);
    segmentMax = zeros(segmentCount, 1);

    for si = 1:segmentCount
        segId = segmentIndexList(si);
        maskSeg = coefTable.SegmentIndex == segId;
        firstRow = find(maskSeg, 1);
        segmentNames{si} = char(coefTable.SegmentName(firstRow));
        segmentMin(si) = coefTable.W25Min(firstRow);
        segmentMax(si) = coefTable.W25Max(firstRow);

        for bi = 1:basisCount
            maskF1 = maskSeg & strcmp(cellstr(coefTable.TermGroup), 'F1') & coefTable.BasisIndex == bi;
            maskF2 = maskSeg & strcmp(cellstr(coefTable.TermGroup), 'F2') & coefTable.BasisIndex == bi;
            coefF1(si, bi) = coefTable.Coefficient(find(maskF1, 1));
            coefF2(si, bi) = coefTable.Coefficient(find(maskF2, 1));
        end
    end

    model.SegmentIndexList = segmentIndexList;
    model.SegmentCount = segmentCount;
    model.BasisCount = basisCount;
    model.SegmentNames = segmentNames;
    model.SegmentMin = segmentMin;
    model.SegmentMax = segmentMax;
    model.CoefF1 = coefF1;
    model.CoefF2 = coefF2;
    model.BaseTempC = get_first_or_default(coefTable, 'BaseTempC', 21.4);
    model.TempScaleC = get_first_or_default(coefTable, 'TempScaleC', 78.6);
    model.DeltaClampEnable = logical(get_first_or_default(coefTable, 'DeltaClampEnable', false));
    model.DeltaClampAbs = get_first_or_default(coefTable, 'DeltaClampAbs', 0.15);
    model.TemperatureLabelList = {'T25', 'T45', 'T60', 'T80', 'T100'};
    model.TemperatureValueList = [21.4, 43.0, 60.0, 80.0, 100.0];
end

function v = get_first_or_default(T, colName, defaultValue)
    if ismember(colName, T.Properties.VariableNames)
        v = T.(colName)(1);
    else
        v = defaultValue;
    end
end

function [deltaW, segIdx, segName] = predict_delta_w(x, y, W25, tNorm, model)
    segIdx = find_segment(W25, model);
    segName = model.SegmentNames{segIdx};
    basis = make_basis_row(x, y, W25);
    f1 = sum(model.CoefF1(segIdx, :) .* basis);
    f2 = sum(model.CoefF2(segIdx, :) .* basis);
    deltaW = tNorm .* f1 + (tNorm .^ 2) .* f2;
    if model.DeltaClampEnable
        deltaW = min(max(deltaW, -model.DeltaClampAbs), model.DeltaClampAbs);
    end
end

function segIdx = find_segment(W25, model)
    segIdx = model.SegmentCount;
    for i = 1:model.SegmentCount
        if W25 >= model.SegmentMin(i) && W25 < model.SegmentMax(i)
            segIdx = i;
            return;
        end
    end
end

function basis = make_basis_row(x, y, W25)
    basis = [1, x, y, W25, x .* y, x .* W25, y .* W25, W25 .* W25, x .* x, y .* y];
end

function tempC = get_temperature_for_label(label, model)
    idx = find(strcmp(model.TemperatureLabelList, char(label)), 1);
    if isempty(idx)
        tempC = parse_temperature_from_label(label);
    else
        tempC = model.TemperatureValueList(idx);
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

function summaryTable = build_summary_table(pointTable)
    labels = unique(cellstr(pointTable.Label), 'stable');
    segments = unique(pointTable.SegmentIndex, 'stable');
    groupNames = {};
    labelNames = {};
    segmentIds = [];
    sampleCounts = [];
    beforeMean = [];
    beforeP95 = [];
    beforeMax = [];
    afterMean = [];
    afterP95 = [];
    afterMax = [];
    improveMean = [];

    add_group('ALL', 'ALL', NaN, true(height(pointTable),1));
    for i = 1:numel(labels)
        mask = strcmp(cellstr(pointTable.Label), labels{i});
        add_group(['LABEL_' labels{i}], labels{i}, NaN, mask);
    end
    for i = 1:numel(segments)
        mask = pointTable.SegmentIndex == segments(i);
        add_group(['SEGMENT_' num2str(segments(i))], 'ALL', segments(i), mask);
    end

    summaryTable = table(groupNames(:), labelNames(:), segmentIds(:), sampleCounts(:), ...
        beforeMean(:), beforeP95(:), beforeMax(:), afterMean(:), afterP95(:), afterMax(:), improveMean(:), ...
        'VariableNames', {'Group','Label','SegmentIndex','SampleCount', ...
        'MeanAbsError_Before','P95AbsError_Before','MaxAbsError_Before', ...
        'MeanAbsError_After','P95AbsError_After','MaxAbsError_After','MeanImprovement'});

    function add_group(groupName, labelName, segId, mask)
        sub = pointTable(mask, :);
        groupNames{end+1} = groupName; %#ok<AGROW>
        labelNames{end+1} = labelName; %#ok<AGROW>
        segmentIds(end+1) = segId; %#ok<AGROW>
        sampleCounts(end+1) = height(sub); %#ok<AGROW>
        beforeMean(end+1) = mean(sub.AbsError_Before); %#ok<AGROW>
        beforeP95(end+1) = local_percentile(sub.AbsError_Before, 95); %#ok<AGROW>
        beforeMax(end+1) = max(sub.AbsError_Before); %#ok<AGROW>
        afterMean(end+1) = mean(sub.AbsError_After); %#ok<AGROW>
        afterP95(end+1) = local_percentile(sub.AbsError_After, 95); %#ok<AGROW>
        afterMax(end+1) = max(sub.AbsError_After); %#ok<AGROW>
        improveMean(end+1) = mean(sub.Improvement); %#ok<AGROW>
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
