% S04_SHOW_MODEL_TABLE_RATIOK_STAT_FIXED
% Build grouped comparison tables from model_compare_results.csv.
% This script keeps the main grouped table/statistics behavior and
% additionally exports, for each target point, a compact 6-neighbor table
% using only xy coordinates and Delta uv, plus a separate Delta uv statistics table.

clear;
clc;

%% Locate folders
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
inputFile = fullfile(outputDir, 'model_compare_results.csv');
outputFile = fullfile(outputDir, 'model_compare_grouped_table_ratioK.csv');
statCsvFile = fullfile(outputDir, 'model_compare_ra_error_statistics.csv');
statPngFile = fullfile(outputDir, 'model_compare_ra_error_statistics.png');
lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
neighbor6CsvFile = fullfile(outputDir, 'model_compare_ratioK_neighbor6_compact.csv');
neighbor6XlsxFile = fullfile(outputDir, 'model_compare_ratioK_neighbor6_compact.xlsx');
deltaUvStatCsvFile = fullfile(outputDir, 'model_compare_ratioK_delta_uv_stats.csv');
deltaUvStatXlsxFile = fullfile(outputDir, 'model_compare_ratioK_delta_uv_stats.xlsx');
deltaUvOverallCsvFile = fullfile(outputDir, 'model_compare_ratioK_delta_uv_overall_summary.csv');
deltaUvOverallXlsxFile = fullfile(outputDir, 'model_compare_ratioK_delta_uv_overall_summary.xlsx');

if ~exist(inputFile, 'file')
    error('Result file not found: %s\nPlease run s02_run_model_compare_ratioK_fixed.m first.', inputFile);
end

T = readtable(inputFile, 'VariableNamingRule', 'preserve');

%% Optional independent output: readable 6-neighbor summary for each target
neighbor6CompactTable = table();
deltaUvStatTable = table();
deltaUvOverallTable = table();
neighbor6XlsxActual = neighbor6XlsxFile;
deltaUvStatXlsxActual = deltaUvStatXlsxFile;
deltaUvOverallXlsxActual = deltaUvOverallXlsxFile;

if exist(lutFile, 'file')
    lutTable = readtable(lutFile, 'VariableNamingRule', 'preserve');
    [neighbor6CompactTable, deltaUvStatTable, deltaUvOverallTable] = build_neighbor6_compact_table(T, lutTable, 6);
    neighbor6XlsxActual = resolve_writable_output_path(neighbor6XlsxFile);
    deltaUvStatXlsxActual = resolve_writable_output_path(deltaUvStatXlsxFile);
    deltaUvOverallXlsxActual = resolve_writable_output_path(deltaUvOverallXlsxFile);
    writetable(make_neighbor6_export_table(neighbor6CompactTable), neighbor6CsvFile);
    export_neighbor6_excel_formatted(neighbor6CompactTable, deltaUvOverallTable, neighbor6XlsxActual);
    writetable(make_delta_uv_export_table(deltaUvStatTable), deltaUvStatCsvFile);
    writetable(make_delta_uv_export_table(deltaUvStatTable), deltaUvStatXlsxActual);
    writetable(make_delta_uv_overall_export_table(deltaUvOverallTable), deltaUvOverallCsvFile);
    writetable(make_delta_uv_overall_export_table(deltaUvOverallTable), deltaUvOverallXlsxActual);
    fprintf('\n================ Neighbor-6 Compact Table ================\n');
    fprintf('Neighbor-6 compact CSV written to:\n%s\n', neighbor6CsvFile);
    fprintf('Neighbor-6 compact Excel written to:\n%s\n', neighbor6XlsxActual);
    fprintf('Delta uv statistics CSV written to:\n%s\n', deltaUvStatCsvFile);
    fprintf('Delta uv statistics Excel written to:\n%s\n', deltaUvStatXlsxActual);
    fprintf('300-color Delta uv overall summary CSV written to:\n%s\n', deltaUvOverallCsvFile);
    fprintf('300-color Delta uv overall summary Excel written to:\n%s\n', deltaUvOverallXlsxActual);
else
    warning('未找到 LUT 文件：%s\n跳过近邻6点明细表输出。', lutFile);
end

%% Ra error statistics (Exact as baseline)
statMethodNames = {'Projection1','ProjectionK','Ratio1','RatioK','PureRGB'};
statPrefixNames = {'Proj1','ProjK','Ratio1','RatioK','PureRGB'};

statCount = numel(statMethodNames);
statMeanDiff = nan(statCount, 1);
statMeanAbsDiff = nan(statCount, 1);
statMaxAbsDiff = nan(statCount, 1);
statValidCount = nan(statCount, 1);

for statIdx = 1:statCount
    prefix = statPrefixNames{statIdx};
    raCol = [prefix '_Ra'];

    if ~ismember(raCol, T.Properties.VariableNames)
        continue;
    end

    diffList = T.Exact_Ra - T.(raCol);
    validMask = isfinite(diffList);

    statValidCount(statIdx) = sum(validMask);
    statMeanDiff(statIdx) = mean(diffList(validMask), 'omitnan');
    statMeanAbsDiff(statIdx) = mean(abs(diffList(validMask)), 'omitnan');
    statMaxAbsDiff(statIdx) = max(abs(diffList(validMask)), [], 'omitnan');
end

statTable = table( ...
    string(statMethodNames(:)), ...
    statValidCount, ...
    statMeanDiff, ...
    statMeanAbsDiff, ...
    statMaxAbsDiff, ...
    'VariableNames', {'Method','ValidCount','MeanDiff_ExactMinusMethod','MeanAbsDiff','MaxAbsDiff'});

writetable(statTable, statCsvFile);

fprintf('\n================ Ra Error Statistics vs Exact ================\n');
disp(statTable);
fprintf('\nRa error statistics CSV written to:\n%s\n', statCsvFile);

plot_ra_error_statistics(statTable, statPngFile);

%% Build grouped comparison table
methodNames = {'Exact','Projection1','ProjectionK','Ratio1','RatioK','PureRGB'};
prefixNames = {'Exact','Proj1','ProjK','Ratio1','RatioK','PureRGB'};
methodCount = numel(methodNames);
targetCount = height(T);
rowCount = targetCount * methodCount;

bestApproxRowList = nan(targetCount, 1);
for targetIdx = 1:targetCount
    approxRaList = [T.Proj1_Ra(targetIdx), T.ProjK_Ra(targetIdx), ...
                    T.Ratio1_Ra(targetIdx), T.RatioK_Ra(targetIdx), ...
                    T.PureRGB_Ra(targetIdx)];
    validMask = isfinite(approxRaList);
    if any(validMask)
        validRaList = approxRaList(validMask);
        approxMethodIndexList = find(validMask) + 1; % +1 because row 1 is Exact
        [~, bestIdx] = max(validRaList);
        bestMethodIdx = approxMethodIndexList(bestIdx);
        bestApproxRowList(targetIdx) = (targetIdx - 1) * methodCount + bestMethodIdx;
    end
end

displayData = cell(rowCount, 12);
csvData = cell(rowCount + 1, 12);

columnNames = {'Index','Target xyY','Method','Ra','RGB Point','White_RGB Ratio','RGB Weights', ...
    'RaDiff_ExactMinusMethod','Note','WhiteRatio','RGBRatio','BestApproxSource'};
csvData(1, :) = columnNames;

for targetIdx = 1:targetCount
    exactRa = T.Exact_Ra(targetIdx);
    baseRow = (targetIdx - 1) * methodCount;
    targetText = sprintf('(%.4f, %.4f, %.4f)', ...
        T.Target_x(targetIdx), T.Target_y(targetIdx), T.Target_Y_lm(targetIdx));

    approxMaxSourceText = get_text_col(T, 'Approx_Max_Source', targetIdx);

    for methodIdx = 1:methodCount
        rowIdx = baseRow + methodIdx;
        methodName = methodNames{methodIdx};
        prefix = prefixNames{methodIdx};

        ra = get_col(T, [prefix '_Ra'], targetIdx);
        x = get_col(T, [prefix '_RGBPoint_x'], targetIdx);
        y = get_col(T, [prefix '_RGBPoint_y'], targetIdx);
        wRatio = get_col(T, [prefix '_SingleWhiteRatio'], targetIdx);
        rgbRatio = get_col(T, [prefix '_RGBWhiteRatio'], targetIdx);
        rWeight = get_col(T, [prefix '_R_weight'], targetIdx);
        gWeight = get_col(T, [prefix '_G_weight'], targetIdx);
        bWeight = get_col(T, [prefix '_B_weight'], targetIdx);

        if methodIdx == 1
            displayIndex = T.Index(targetIdx);
            displayTarget = targetText;
            noteText = 'Compare to Exact';
        else
            displayIndex = '';
            displayTarget = '';
            noteText = 'Compare to Exact';
        end

        rgbPointText = sprintf('(%.5f, %.5f)', x, y);
        ratioText = sprintf('W=%.4f, RGB=%.4f', wRatio, rgbRatio);
        weightText = sprintf('R=%.4f, G=%.4f, B=%.4f', rWeight, gWeight, bWeight);
        raDiff = exactRa - ra;

        displayData(rowIdx, :) = {displayIndex, displayTarget, methodName, ra, ...
            rgbPointText, ratioText, weightText, raDiff, noteText, ...
            wRatio, rgbRatio, approxMaxSourceText};

        csvData(rowIdx + 1, :) = displayData(rowIdx, :);
    end
end

writecell(csvData, outputFile);

fprintf('\n================ 6-Model Grouped Table ================\n');
fprintf('Grouped table prepared: %d targets x %d methods = %d rows.\n', targetCount, methodCount, rowCount);
fprintf('\nGrouped table CSV written to:\n%s\n', outputFile);

show_grouped_table(displayData, columnNames, targetCount, methodCount, outputFile, bestApproxRowList);
show_stat_table_window(statTable, statPngFile, statCsvFile);
if ~isempty(neighbor6CompactTable)
    show_neighbor6_summary_window(neighbor6CompactTable, deltaUvStatTable, neighbor6CsvFile, deltaUvStatCsvFile);
end

%% ================= Local functions =================

function value = get_col(T, colName, rowIdx)
    if ismember(colName, T.Properties.VariableNames)
        value = T.(colName)(rowIdx);
    else
        value = NaN;
    end
end

function textValue = get_text_col(T, colName, rowIdx)
    if ~ismember(colName, T.Properties.VariableNames)
        textValue = '';
        return;
    end

    value = T.(colName)(rowIdx);
    if iscell(value)
        value = value{1};
    end

    if isstring(value)
        textValue = char(value);
    elseif ischar(value)
        textValue = value;
    elseif iscategorical(value)
        textValue = char(value);
    elseif ismissing(value)
        textValue = '';
    else
        textValue = char(string(value));
    end
end

function [compactTable, statTable, overallTable] = build_neighbor6_compact_table(T, lutTable, neighborCount)
    if nargin < 3
        neighborCount = 6;
    end

    targetCount = height(T);
    lutCount = height(lutTable);
    useCount = min(neighborCount, lutCount);

    compactTable = table('Size', [targetCount * useCount, 15], ...
        'VariableTypes', {'double','string','string','string','string','double','string','double','string','string','string','double','string','string','string'}, ...
        'VariableNames', {'IndexId','TargetXY','TargetUvDecimal','TargetUvInt1000','TargetUvIntDiv5', ...
                          'NeighborRank','NeighborXY','DeltaUV','DeltaUVStats','TargetRatioKRa', ...
                          'TargetExactRa','NeighborBestRa','NeighborBestRGBPoint','NeighborBestRatio', ...
                          'NeighborBoundary'});

    deltaMatrix = nan(targetCount, useCount);
    outRow = 0;

    for targetIdx = 1:targetCount
        targetU = T.Target_u1976(targetIdx);
        targetV = T.Target_v1976(targetIdx);

        deltaU = lutTable.Target_u1976 - targetU;
        deltaV = lutTable.Target_v1976 - targetV;
        deltaUv = hypot(deltaU, deltaV);

        [~, order] = sort(deltaUv, 'ascend');
        order = order(1:useCount);

        for neighborIdx = 1:useCount
            lutIdx = order(neighborIdx);
            deltaMatrix(targetIdx, neighborIdx) = deltaUv(lutIdx);

            outRow = outRow + 1;
            compactTable.IndexId(outRow) = T.Index(targetIdx);
            compactTable.TargetXY(outRow) = string(sprintf('(%.5f, %.5f)', T.Target_x(targetIdx), T.Target_y(targetIdx)));
            compactTable.NeighborRank(outRow) = neighborIdx;
            compactTable.NeighborXY(outRow) = string(sprintf('(%.5f, %.5f)', ...
                lutTable.Target_x(lutIdx), lutTable.Target_y(lutIdx)));
            compactTable.DeltaUV(outRow) = deltaUv(lutIdx);

            compactTable.NeighborBestRa(outRow) = get_col(lutTable, 'Best_Ra', lutIdx);
            compactTable.NeighborBestRGBPoint(outRow) = string(sprintf('(%.5f, %.5f)', ...
                get_col(lutTable, 'Best_RGBPoint_x', lutIdx), get_col(lutTable, 'Best_RGBPoint_y', lutIdx)));
            compactTable.NeighborBestRatio(outRow) = string(sprintf('W=%.4f, RGB=%.4f', ...
                get_col(lutTable, 'Best_SingleWhiteRatio', lutIdx), get_col(lutTable, 'Best_RGBWhiteRatio', lutIdx)));
            compactTable.NeighborBoundary(outRow) = string(get_text_col(lutTable, 'BoundaryEdge', lutIdx));
        end

        summaryText = string(sprintf('dUV max=%.6f, min=%.6f, avg=%.6f', ...
            max(deltaMatrix(targetIdx, :), [], 'omitnan'), ...
            min(deltaMatrix(targetIdx, :), [], 'omitnan'), ...
            mean(deltaMatrix(targetIdx, :), 'omitnan')));
        rowStart = (targetIdx - 1) * useCount + 1;
        rowEnd = rowStart + useCount - 1;
        compactTable.TargetUvDecimal(rowStart:rowEnd) = "";
        compactTable.TargetUvInt1000(rowStart:rowEnd) = "";
        compactTable.TargetUvIntDiv5(rowStart:rowEnd) = "";
        compactTable.DeltaUVStats(rowStart:rowEnd) = "";
        compactTable.TargetRatioKRa(rowStart:rowEnd) = "";
        compactTable.TargetExactRa(rowStart:rowEnd) = "";
        compactTable.TargetUvDecimal(rowStart) = format_uv_decimal_text(targetU, targetV);
        compactTable.TargetUvInt1000(rowStart) = format_uv_int1000_text(targetU, targetV);
        compactTable.TargetUvIntDiv5(rowStart) = format_uv_int_div5_text(targetU, targetV);
        compactTable.DeltaUVStats(rowStart) = summaryText;
        compactTable.TargetRatioKRa(rowStart) = format_numeric_text(get_col(T, 'RatioK_Ra', targetIdx), 4);
        compactTable.TargetExactRa(rowStart) = format_numeric_text(get_col(T, 'Exact_Ra', targetIdx), 4);
    end

    statTable = build_delta_uv_stat_table(deltaMatrix, useCount);
    overallTable = build_delta_uv_overall_summary(deltaMatrix);
end

function statTable = build_delta_uv_stat_table(deltaMatrix, useCount)
    rowLabels = [arrayfun(@(k) sprintf('N%d', k), 1:useCount, 'UniformOutput', false), {'Overall'}];
    rowCount = numel(rowLabels);

    statTable = table('Size', [rowCount, 7], ...
        'VariableTypes', {'string','double','double','double','double','double','double'}, ...
        'VariableNames', {'Group','Count','MeanDeltaUV','MedianDeltaUV','MinDeltaUV','MaxDeltaUV','P95DeltaUV'});

    for k = 1:useCount
        vals = deltaMatrix(:, k);
        vals = vals(isfinite(vals));
        statTable.Group(k) = string(rowLabels{k});
        statTable.Count(k) = numel(vals);
        statTable.MeanDeltaUV(k) = mean(vals, 'omitnan');
        statTable.MedianDeltaUV(k) = median(vals, 'omitnan');
        statTable.MinDeltaUV(k) = min(vals, [], 'omitnan');
        statTable.MaxDeltaUV(k) = max(vals, [], 'omitnan');
        statTable.P95DeltaUV(k) = percentile95(vals);
    end

    allVals = deltaMatrix(:);
    allVals = allVals(isfinite(allVals));
    statTable.Group(end) = "整体";
    statTable.Count(end) = numel(allVals);
    statTable.MeanDeltaUV(end) = mean(allVals, 'omitnan');
    statTable.MedianDeltaUV(end) = median(allVals, 'omitnan');
    statTable.MinDeltaUV(end) = min(allVals, [], 'omitnan');
    statTable.MaxDeltaUV(end) = max(allVals, [], 'omitnan');
    statTable.P95DeltaUV(end) = percentile95(allVals);
end

function exportTable = make_neighbor6_export_table(compactTable)
    exportTable = compactTable;
    exportTable.Properties.VariableNames = {'序号','目标色坐标','目标色uv(小数)','目标色uv整数x1000','目标色uv整数/5', ...
        '近邻序号','近邻色坐标','Δuv','Δuv统计','K点比例法Ra','精确法最佳Ra', ...
        '近邻最佳Ra','近邻最佳RGB混色点','近邻最佳比例值','近邻边界'};
end

function exportTable = make_delta_uv_export_table(statTable)
    exportTable = statTable;
    exportTable.Properties.VariableNames = {'分组','数量','平均Δuv','中位Δuv','最小Δuv','最大Δuv','P95_Δuv'};
end

function overallTable = build_delta_uv_overall_summary(deltaMatrix)
    perTargetMax = max(deltaMatrix, [], 2, 'omitnan');
    perTargetMin = min(deltaMatrix, [], 2, 'omitnan');
    perTargetMean = mean(deltaMatrix, 2, 'omitnan');
    allVals = deltaMatrix(:);
    allVals = allVals(isfinite(allVals));

    overallTable = table('Size', [4, 5], ...
        'VariableTypes', {'string','double','double','double','double'}, ...
        'VariableNames', {'Item','Count','MinDeltaUV','MaxDeltaUV','MeanDeltaUV'});

    overallTable.Item(1) = "全部近邻点";
    overallTable.Count(1) = numel(allVals);
    overallTable.MinDeltaUV(1) = min(allVals, [], 'omitnan');
    overallTable.MaxDeltaUV(1) = max(allVals, [], 'omitnan');
    overallTable.MeanDeltaUV(1) = mean(allVals, 'omitnan');

    overallTable.Item(2) = "各目标色的最大Δuv";
    overallTable.Count(2) = numel(perTargetMax);
    overallTable.MinDeltaUV(2) = min(perTargetMax, [], 'omitnan');
    overallTable.MaxDeltaUV(2) = max(perTargetMax, [], 'omitnan');
    overallTable.MeanDeltaUV(2) = mean(perTargetMax, 'omitnan');

    overallTable.Item(3) = "各目标色的最小Δuv";
    overallTable.Count(3) = numel(perTargetMin);
    overallTable.MinDeltaUV(3) = min(perTargetMin, [], 'omitnan');
    overallTable.MaxDeltaUV(3) = max(perTargetMin, [], 'omitnan');
    overallTable.MeanDeltaUV(3) = mean(perTargetMin, 'omitnan');

    overallTable.Item(4) = "各目标色的平均Δuv";
    overallTable.Count(4) = numel(perTargetMean);
    overallTable.MinDeltaUV(4) = min(perTargetMean, [], 'omitnan');
    overallTable.MaxDeltaUV(4) = max(perTargetMean, [], 'omitnan');
    overallTable.MeanDeltaUV(4) = mean(perTargetMean, 'omitnan');
end

function exportTable = make_delta_uv_overall_export_table(overallTable)
    exportTable = overallTable;
    exportTable.Properties.VariableNames = {'项目','数量','最小Δuv','最大Δuv','平均Δuv'};
end

function export_neighbor6_excel_formatted(compactTable, overallTable, xlsxPath)
    exportCompact = make_neighbor6_export_table(compactTable);
    exportOverall = make_delta_uv_overall_export_table(overallTable);

    summaryTitle = {'300个颜色 Delta uv 总汇总'};
    detailTitle = {'近邻6点明细表'};
    blankRow = {''};

    writecell(summaryTitle, xlsxPath, 'Sheet', 1, 'Range', 'A1');
    writetable(exportOverall, xlsxPath, 'Sheet', 1, 'Range', 'A3', 'WriteVariableNames', true);
    writecell(blankRow, xlsxPath, 'Sheet', 1, 'Range', 'A9');
    writecell(detailTitle, xlsxPath, 'Sheet', 1, 'Range', 'A10');
    writetable(exportCompact, xlsxPath, 'Sheet', 1, 'Range', 'A12', 'WriteVariableNames', true);

    try
        excel = actxserver('Excel.Application');
        excel.Visible = false;
        workbook = excel.Workbooks.Open(xlsxPath);
        sheet = workbook.Worksheets.Item(1);

        % Basic widths
        sheet.Columns.Item('A').ColumnWidth = 8;
        sheet.Columns.Item('B').ColumnWidth = 20;
        sheet.Columns.Item('C').ColumnWidth = 18;
        sheet.Columns.Item('D').ColumnWidth = 18;
        sheet.Columns.Item('E').ColumnWidth = 16;
        sheet.Columns.Item('F').ColumnWidth = 10;
        sheet.Columns.Item('G').ColumnWidth = 20;
        sheet.Columns.Item('H').ColumnWidth = 12;
        sheet.Columns.Item('I').ColumnWidth = 30;
        sheet.Columns.Item('J').ColumnWidth = 12;
        sheet.Columns.Item('K').ColumnWidth = 12;
        sheet.Columns.Item('L').ColumnWidth = 12;
        sheet.Columns.Item('M').ColumnWidth = 20;
        sheet.Columns.Item('N').ColumnWidth = 22;
        sheet.Columns.Item('O').ColumnWidth = 12;

        % Summary title and table
        sheet.Range('A1:F1').Merge();
        sheet.Range('A1').Value = '300个颜色 Delta uv 总汇总';
        sheet.Range('A1').Font.Bold = true;
        sheet.Range('A1').Font.Size = 14;
        sheet.Range('A1').HorizontalAlignment = -4108; % xlCenter
        sheet.Range('A3:E3').Font.Bold = true;
        sheet.Range('A3:E7').Borders.LineStyle = 1;
        sheet.Range('A3:E3').Interior.Color = rgb_to_bgr(217, 225, 242);

        % Detail title and header
        sheet.Range('A10:O10').Merge();
        sheet.Range('A10').Value = '近邻6点明细表';
        sheet.Range('A10').Font.Bold = true;
        sheet.Range('A10').Font.Size = 13;
        sheet.Range('A10').HorizontalAlignment = -4108;
        sheet.Range('A12:O12').Font.Bold = true;
        sheet.Range('A12:O12').Interior.Color = rgb_to_bgr(221, 235, 247);
        sheet.Range('A12:O12').Borders.LineStyle = 1;

        % Freeze panes below header
        sheet.Range('A13').Select();
        excel.ActiveWindow.FreezePanes = true;

        dataStartRow = 13;
        dataEndRow = dataStartRow + height(exportCompact) - 1;
        if dataEndRow >= dataStartRow
            sheet.Range(sprintf('A%d:O%d', dataStartRow, dataEndRow)).Borders.LineStyle = 1;
            sheet.Range(sprintf('C%d:O%d', dataStartRow, dataEndRow)).WrapText = true;
        end

        groupColors = [ ...
            242 242 242; ...
            222 235 247; ...
            252 228 214; ...
            226 239 218];
        neighborColors = [ ...
            255 242 204; ...
            221 235 247; ...
            226 239 218; ...
            244 204 204; ...
            234 209 220; ...
            217 210 233];

        if ~isempty(exportCompact)
            groupStarts = find(exportCompact.("近邻序号") == 1);
            groupEnds = [groupStarts(2:end) - 1; height(exportCompact)];

            for g = 1:numel(groupStarts)
                rows = groupStarts(g):groupEnds(g);
                rowA = dataStartRow + rows(1) - 1;
                rowB = dataStartRow + rows(end) - 1;
                gc = groupColors(mod(g - 1, size(groupColors, 1)) + 1, :);

                % Whole target group background
                sheet.Range(sprintf('A%d:O%d', rowA, rowB)).Interior.Color = rgb_to_bgr(gc(1), gc(2), gc(3));

                % Neighbor rows with distinct colors on F:G:H
                for localIdx = 1:numel(rows)
                    excelRow = dataStartRow + rows(localIdx) - 1;
                    nc = neighborColors(mod(localIdx - 1, size(neighborColors, 1)) + 1, :);
                    sheet.Range(sprintf('F%d:H%d', excelRow, excelRow)).Interior.Color = rgb_to_bgr(nc(1), nc(2), nc(3));
                end

                % Summary cell highlight
                sheet.Range(sprintf('C%d:K%d', rowA, rowA)).Font.Bold = true;
                sheet.Range(sprintf('C%d:K%d', rowA, rowA)).Interior.Color = rgb_to_bgr(255, 230, 153);
            end
        end

        workbook.Save();
        workbook.Close(false);
        excel.Quit();
        delete(excel);
    catch
        % Fallback: plain xlsx already written above.
    end
end

function actualPath = resolve_writable_output_path(preferredPath)
    actualPath = preferredPath;
    if ~exist(preferredPath, 'file')
        return;
    end

    try
        fileInfo = dir(preferredPath);
        if isempty(fileInfo)
            return;
        end

        [fid, ~] = fopen(preferredPath, 'a');
        if fid ~= -1
            fclose(fid);
            warning('off', 'MATLAB:DELETE:Permission');
            delete(preferredPath);
            warning('on', 'MATLAB:DELETE:Permission');
            if ~exist(preferredPath, 'file')
                actualPath = preferredPath;
                return;
            end
        end
    catch
    end

    [folderPath, baseName, ext] = fileparts(preferredPath);
    stamp = datestr(now, 'yyyymmdd_HHMMSS');
    actualPath = fullfile(folderPath, sprintf('%s_%s%s', baseName, stamp, ext));
    suffix = 1;
    while exist(actualPath, 'file')
        actualPath = fullfile(folderPath, sprintf('%s_%s_%02d%s', baseName, stamp, suffix, ext));
        suffix = suffix + 1;
    end
    warning('目标文件正在使用，已改为输出到新文件：%s', actualPath);
end

function [bestSourceText, bestRgbPointText, maxRaValue, bestRatioText] = get_best_method_summary(T, rowIdx)
    bestSourceText = get_text_col(T, 'Overall_Max_Source', rowIdx);
    if isempty(bestSourceText)
        bestSourceText = get_text_col(T, 'Approx_Max_Source', rowIdx);
    end

    maxRaValue = get_col(T, 'Overall_Max_Ra', rowIdx);
    if ~isfinite(maxRaValue)
        maxRaValue = get_col(T, 'Approx_Max_Ra', rowIdx);
    end

    prefix = source_to_prefix(bestSourceText);
    if isempty(prefix)
        prefix = 'Exact';
        if isempty(bestSourceText)
            bestSourceText = 'Exact';
        end
    end

    x = get_col(T, [prefix '_RGBPoint_x'], rowIdx);
    y = get_col(T, [prefix '_RGBPoint_y'], rowIdx);
    whiteRatio = get_col(T, [prefix '_SingleWhiteRatio'], rowIdx);
    rgbRatio = get_col(T, [prefix '_RGBWhiteRatio'], rowIdx);

    bestRgbPointText = string(sprintf('(%.5f, %.5f)', x, y));
    bestRatioText = string(sprintf('W=%.4f, RGB=%.4f', whiteRatio, rgbRatio));
    bestSourceText = string(bestSourceText);
end

function textValue = format_numeric_text(value, precision)
    if nargin < 2
        precision = 4;
    end

    if ~isfinite(value)
        textValue = "";
        return;
    end

    textValue = string(sprintf(['%0.' num2str(precision) 'f'], value));
end

function textValue = format_uv_decimal_text(u1976, v1976)
    if ~isfinite(u1976) || ~isfinite(v1976)
        textValue = "";
        return;
    end

    textValue = string(sprintf('(%.5f, %.5f)', u1976, v1976));
end

function textValue = format_uv_int1000_text(u1976, v1976)
    if ~isfinite(u1976) || ~isfinite(v1976)
        textValue = "";
        return;
    end

    uInt = round(u1976 * 1000);
    vInt = round(v1976 * 1000);
    textValue = string(sprintf('(%d, %d)', uInt, vInt));
end

function textValue = format_uv_int_div5_text(u1976, v1976)
    if ~isfinite(u1976) || ~isfinite(v1976)
        textValue = "";
        return;
    end

    uIntDiv5 = round(round(u1976 * 1000) / 5);
    vIntDiv5 = round(round(v1976 * 1000) / 5);
    textValue = string(sprintf('(%d, %d)', uIntDiv5, vIntDiv5));
end

function prefix = source_to_prefix(sourceText)
    s = lower(strtrim(char(string(sourceText))));
    switch s
        case 'exact'
            prefix = 'Exact';
        case 'projection1'
            prefix = 'Proj1';
        case 'projectionk'
            prefix = 'ProjK';
        case 'ratio1'
            prefix = 'Ratio1';
        case 'ratiok'
            prefix = 'RatioK';
        case 'purergb'
            prefix = 'PureRGB';
        otherwise
            prefix = '';
    end
end

function bgr = rgb_to_bgr(r, g, b)
    bgr = double(r) + 256 * double(g) + 65536 * double(b);
end

function p = percentile95(x)
    if isempty(x)
        p = NaN;
        return;
    end
    x = sort(x(:));
    idx = max(1, min(numel(x), ceil(0.95 * numel(x))));
    p = x(idx);
end

function show_neighbor6_summary_window(compactTable, statTable, compactCsvFile, statCsvFile)
    compactDisplay = table_to_display_cell(compactTable);
    statDisplay = table_to_display_cell(statTable);
    compactHeaders = {'序号','目标色坐标','目标色uv(小数)','目标色uv整数x1000','目标色uv整数/5', ...
        '近邻序号','近邻色坐标','Δuv','Δuv统计','K点比例法Ra','精确法最佳Ra', ...
        '近邻最佳Ra','近邻最佳RGB混色点','近邻最佳比例值','近邻边界'};
    statHeaders = {'分组','数量','平均Δuv','中位Δuv','最小Δuv','最大Δuv','P95_Δuv'};
    targetRowColors = [ ...
        0.97 0.97 0.97; ...
        0.94 0.97 1.00; ...
        1.00 0.96 0.93; ...
        0.95 1.00 0.95];

    try
        fig = uifigure('Name','近邻6点与Delta uv统计', ...
            'Position',[80,80,1860,900], ...
            'Color',[0.97,0.97,0.97]);

        uilabel(fig, ...
            'Text','近邻6点明细：每个目标色占6行，并显示目标点u''v''、K点比例法Ra、精确法最佳Ra，以及每个近邻点自己的最优结果', ...
            'Position',[20,860,1280,24], ...
            'FontSize',14, ...
            'FontWeight','bold');

        uilabel(fig, ...
            'Text','近邻6点明细表', ...
            'Position',[20,830,220,22], ...
            'FontSize',12, ...
            'FontWeight','bold');

        uit1 = uitable(fig, ...
            'Data',compactDisplay, ...
            'ColumnName',compactHeaders, ...
            'Position',[20,420,1820,400], ...
            'FontSize',10, ...
            'ColumnWidth',{70,180,170,140,120,80,180,100,240,100,100,100,180,170,100});

        uilabel(fig, ...
            'Text','Delta uv 统计表', ...
            'Position',[20,380,220,22], ...
            'FontSize',12, ...
            'FontWeight','bold');

        uit2 = uitable(fig, ...
            'Data',statDisplay, ...
            'ColumnName',statHeaders, ...
            'Position',[20,190,920,170], ...
            'FontSize',11, ...
            'ColumnWidth',{100,90,120,120,120,120,120});

        ax = uiaxes(fig, ...
            'Position',[980,180,760,190], ...
            'Box','on');
        plot_delta_uv_bar(ax, statTable);

        uilabel(fig, ...
            'Text',['Compact CSV: ' compactCsvFile], ...
            'Position',[20,120,1600,20], ...
            'FontSize',10);

        uilabel(fig, ...
            'Text',['Delta uv stats CSV: ' statCsvFile], ...
            'Position',[20,95,1600,20], ...
            'FontSize',10);

        groupStarts = find(compactTable.NeighborRank == 1);
        groupEnds = [groupStarts(2:end) - 1; height(compactTable)];
        for g = 1:numel(groupStarts)
            bg = targetRowColors(mod(g - 1, size(targetRowColors, 1)) + 1, :);
            rows = groupStarts(g):groupEnds(g);
            addStyle(uit1, uistyle('BackgroundColor', bg), 'row', rows);
            addStyle(uit1, uistyle('FontWeight', 'bold'), 'cell', ...
                [groupStarts(g), 3; groupStarts(g), 4; groupStarts(g), 5; ...
                 groupStarts(g), 9; groupStarts(g), 10; groupStarts(g), 11]);
        end

        addStyle(uit2, uistyle('FontWeight', 'bold', 'BackgroundColor', [0.96 0.98 1.00]), 'row', height(statTable));
    catch
        fig = figure('Name','近邻6点与Delta uv统计', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.97,0.97,0.97], ...
            'Position',[80,80,1760,900]);

        uicontrol('Parent',fig,'Style','text', ...
            'String','近邻6点明细：每个目标色占6行，并显示目标点u''v''、K点比例法Ra、精确法最佳Ra，以及每个近邻点自己的最优结果', ...
            'Units','normalized','Position',[0.02,0.96,0.75,0.03], ...
            'BackgroundColor',[0.97,0.97,0.97], ...
            'FontSize',13,'FontWeight','bold','HorizontalAlignment','left');

        uicontrol('Parent',fig,'Style','text', ...
            'String','近邻6点明细表', ...
            'Units','normalized','Position',[0.02,0.92,0.20,0.03], ...
            'BackgroundColor',[0.97,0.97,0.97], ...
            'FontSize',12,'FontWeight','bold','HorizontalAlignment','left');

        uitable('Parent',fig, ...
            'Data',compactDisplay, ...
            'ColumnName',compactHeaders, ...
            'RowName',[], ...
            'Units','normalized', ...
            'Position',[0.02,0.40,0.96,0.50], ...
            'FontSize',9, ...
            'ColumnWidth',{70,180,170,140,120,80,180,100,240,100,100,100,180,170,100});

        uicontrol('Parent',fig,'Style','text', ...
            'String','Delta uv 统计表', ...
            'Units','normalized','Position',[0.02,0.36,0.20,0.03], ...
            'BackgroundColor',[0.97,0.97,0.97], ...
            'FontSize',12,'FontWeight','bold','HorizontalAlignment','left');

        uitable('Parent',fig, ...
            'Data',statDisplay, ...
            'ColumnName',statHeaders, ...
            'RowName',[], ...
            'Units','normalized', ...
            'Position',[0.02,0.15,0.60,0.18], ...
            'FontSize',10);

        ax = axes('Parent',fig, 'Units','normalized', 'Position',[0.66,0.16,0.31,0.18]);
        plot_delta_uv_bar(ax, statTable);

        uicontrol('Parent',fig,'Style','text', ...
            'String',['Compact CSV: ' compactCsvFile], ...
            'Units','normalized','Position',[0.02,0.08,0.96,0.03], ...
            'BackgroundColor',[0.97,0.97,0.97], ...
            'FontSize',9,'HorizontalAlignment','left');

        uicontrol('Parent',fig,'Style','text', ...
            'String',['Delta uv stats CSV: ' statCsvFile], ...
            'Units','normalized','Position',[0.02,0.04,0.96,0.03], ...
            'BackgroundColor',[0.97,0.97,0.97], ...
            'FontSize',9,'HorizontalAlignment','left');
    end
end

function plot_delta_uv_bar(ax, statTable)
    groups = cellstr(statTable.Group);
    y = [statTable.MeanDeltaUV, statTable.MedianDeltaUV, statTable.MaxDeltaUV];
    bar(ax, y, 'grouped');
    grid(ax, 'on');
    ax.XTick = 1:numel(groups);
    ax.XTickLabel = groups;
    ax.XTickLabelRotation = 0;
    ylabel(ax, 'Δuv');
    title(ax, 'Δuv 统计柱状图');
    legend(ax, {'平均Δuv','中位Δuv','最大Δuv'}, 'Location', 'best');
end

function show_grouped_table(displayData, columnNames, targetCount, methodCount, outputFile, bestApproxRowList)
    if nargin < 6
        bestApproxRowList = nan(targetCount, 1);
    end

    try
        fig = uifigure('Name','6-Model Grouped Table', ...
            'Position',[40,60,1850,820], ...
            'Color',[0.96,0.96,0.96]);

        uilabel(fig, ...
            'Text','Each target uses 6 rows: Exact / Projection1 / ProjectionK / Ratio1 / RatioK / PureRGB; red row = best approximate Ra', ...
            'Position',[20,780,1750,28], ...
            'FontSize',13, ...
            'FontWeight','bold');

        uit = uitable(fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'Position',[20,70,1810,700], ...
            'FontSize',10, ...
            'ColumnWidth',{55,150,90,85,135,165,220,165,120,90,90,140});

        colorA = [0.96, 0.985, 1.00];
        colorB = [1.00, 0.975, 0.93];

        for targetIdx = 1:targetCount
            rows = ((targetIdx - 1) * methodCount + 1):(targetIdx * methodCount);
            if mod(targetIdx, 2) == 1
                bg = colorA;
            else
                bg = colorB;
            end
            addStyle(uit, uistyle('BackgroundColor', bg), 'row', rows);
        end

        bestStyle = uistyle('BackgroundColor', [1.00, 0.78, 0.78], ...
                            'FontColor', [0.75, 0.00, 0.00], ...
                            'FontWeight', 'bold');
        for targetIdx = 1:targetCount
            bestRow = bestApproxRowList(targetIdx);
            if isfinite(bestRow)
                addStyle(uit, bestStyle, 'row', bestRow);
            end
        end

        uilabel(fig, ...
            'Text',['CSV output: ' outputFile], ...
            'Position',[20,25,1750,28], ...
            'FontSize',10);
    catch
        fig = figure('Name','6-Model Grouped Table', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.96,0.96,0.96], ...
            'Position',[40,60,1850,780]);

        uicontrol('Parent',fig,'Style','text', ...
            'String','Each target uses 6 rows: Exact / Projection1 / ProjectionK / Ratio1 / RatioK / PureRGB; red row = best approximate Ra', ...
            'Units','normalized','Position',[0.015,0.94,0.97,0.045], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',12,'FontWeight','bold','HorizontalAlignment','left');

        uitable('Parent',fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'RowName',[], ...
            'Units','normalized', ...
            'Position',[0.015,0.08,0.97,0.85], ...
            'FontSize',9, ...
            'ColumnWidth',{55,150,90,85,135,165,220,165,120,90,90,140});

        uicontrol('Parent',fig,'Style','text', ...
            'String',['CSV output: ' outputFile], ...
            'Units','normalized','Position',[0.015,0.01,0.97,0.05], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',9,'HorizontalAlignment','left');
    end
end

function plot_ra_error_statistics(statTable, statPngFile)
    fig = figure('Name', 'Ra Error Statistics vs Exact', ...
        'NumberTitle', 'off', ...
        'Color', 'w', ...
        'Position', [120, 120, 1100, 620]);

    methodName = string(statTable.Method);
    x = 1:height(statTable);

    meanDiff = statTable.MeanDiff_ExactMinusMethod;
    meanAbsDiff = statTable.MeanAbsDiff;
    maxAbsDiff = statTable.MaxAbsDiff;

    plot(x, meanAbsDiff, '-o', 'LineWidth', 1.8, 'MarkerSize', 7);
    hold on;
    plot(x, abs(meanDiff), '-^', 'LineWidth', 1.6, 'MarkerSize', 7);
    plot(x, maxAbsDiff, '-s', 'LineWidth', 1.6, 'MarkerSize', 7);
    hold off;

    grid on;
    xlim([1, height(statTable)]);
    xticks(x);
    xticklabels(methodName);
    xtickangle(20);

    xlabel('Approximation Method');
    ylabel('Ra Error');
    title('Ra Error Statistics Relative to Exact');
    legend({'Mean Absolute Error', 'Absolute Mean Error', 'Max Absolute Error'}, 'Location', 'best');

    saveas(fig, statPngFile);
    fprintf('\nRa error statistics figure written to:\n%s\n', statPngFile);
end

function show_stat_table_window(statTable, statPngFile, statCsvFile)
    try
        fig = figure('Name','Ra Error Statistics Table', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.96,0.96,0.96], ...
            'Position',[120,120,1180,560]);

        uicontrol('Parent',fig, ...
            'Style','text', ...
            'String','Each target uses 6 rows: Exact / Projection1 / ProjectionK / Ratio1 / RatioK / PureRGB; red row = best approximate Ra', ...
            'Units','normalized', ...
            'Position',[0.025,0.925,0.95,0.05], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',12, ...
            'FontWeight','bold', ...
            'HorizontalAlignment','left');

        statDisplayData = table_to_display_cell(statTable);

        uitable('Parent',fig, ...
            'Data',statDisplayData, ...
            'ColumnName',statTable.Properties.VariableNames, ...
            'RowName',[], ...
            'Units','normalized', ...
            'Position',[0.025,0.22,0.95,0.68], ...
            'FontSize',11, ...
            'ColumnWidth',{130,120,220,160,160});

        uicontrol('Parent',fig, ...
            'Style','text', ...
            'String',['Statistics CSV: ' statCsvFile], ...
            'Units','normalized', ...
            'Position',[0.025,0.125,0.95,0.04], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',10, ...
            'HorizontalAlignment','left');

        uicontrol('Parent',fig, ...
            'Style','text', ...
            'String',['Figure file: ' statPngFile], ...
            'Units','normalized', ...
            'Position',[0.025,0.075,0.95,0.04], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',10, ...
            'HorizontalAlignment','left');

        uicontrol('Parent',fig, ...
            'Style','pushbutton', ...
            'String','Close', ...
            'Units','normalized', ...
            'Position',[0.45,0.02,0.10,0.04], ...
            'Callback',@(~,~) close(fig));
    catch ME
        fprintf('\nFailed to open Ra statistics window: %s\n', ME.message);
    end
end

function C = table_to_display_cell(T)
    C = table2cell(T);

    for r = 1:size(C, 1)
        for c = 1:size(C, 2)
            value = C{r, c};

            if isstring(value)
                C{r, c} = char(value);
            elseif iscategorical(value)
                C{r, c} = char(value);
            elseif ischar(value)
                C{r, c} = value;
            elseif isnumeric(value)
                if isscalar(value)
                    if isnan(value)
                        C{r, c} = 'NaN';
                    else
                        C{r, c} = sprintf('%.6f', value);
                    end
                else
                    C{r, c} = mat2str(value);
                end
            elseif ismissing(value)
                C{r, c} = '';
            else
                C{r, c} = char(string(value));
            end
        end
    end
end

function projectRoot = find_project_root(startDir, inputFolderName)
    projectRoot = startDir;
    while true
        if exist(fullfile(projectRoot, inputFolderName), 'dir')
            return;
        end
        parentDir = fileparts(projectRoot);
        if strcmp(parentDir, projectRoot)
            error('Cannot find %s from script folder or its parent folders.', inputFolderName);
        end
        projectRoot = parentDir;
    end
end
