% S10_BATCH_31_COLORS_NEAREST3_RATIO_WEIGHTED_REALTIME
% Batch calculate multiple MCU-style u'v' input colors in one project.
%
% Method:
%   1. Exact ray-scan best-Ra method
%   2. S10 nearest-3 ratio-weighted method
%
% Realtime output:
%   - Each color is calculated one by one.
%   - The result text window is updated immediately after each color finishes.
%   - A CSV and TXT summary are exported to triangle_grid_lut_output.

clear;
clc;

%% User config
% Note:
% The image provided contains 31 rows. All rows are included below.
% If only 30 rows are needed, delete the last row [41, 92].
targetUvDiv5List = [
     40,  94;
     39,  96;
     39, 110;
     41, 110;
     43, 110;
     48, 109;
     54, 109;
     61, 108;
     67, 107;
     76, 106;
    103, 104;
    101, 101;
     98,  99;
     88,  88;
     49,  36;
     39,  17;
     32,  48;
     29,  58;
     27,  64;
     25,  72;
     24,  78;
     22,  84;
     21,  91;
     20,  95;
     19,  98;
     17, 105;
     15, 112;
     19, 112;
     27, 111;
     34, 111;
     41,  92
];

targetY_lm = NaN;              % NaN -> use guangpu_case_config().target_xyY(3)
nearestGridMetric = 'uv1976';  % 'uv1976' or 'xy'
nearestGridK = 3;              % S10: nearest 3 points
nearestGridWeightPower = 2;    % weight = 1 / distance^power
rayScanCount = 1001;
pwmMax = 255;

fprintf('\n============================================================\n');
fprintf('S10 batch verification: Exact vs nearest-3 ratio weighting\n');
fprintf('Color count          = %u\n', uint32(size(targetUvDiv5List, 1)));
fprintf('Nearest grid metric  = %s\n', nearestGridMetric);
fprintf('Nearest grid K       = %u\n', uint32(nearestGridK));
fprintf('Weight power         = %.6f\n', nearestGridWeightPower);
fprintf('============================================================\n\n');

%% Locate project
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');
lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');

if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

if ~exist(lutFile, 'file')
    error('Missing LUT file:\n%s\nPlease run s01_make_lut_24_final.m first.', lutFile);
end

outputCsvFile = fullfile(outputDir, 's10_batch_31_colors_nearest3_ratio_weighted_results.csv');
outputTxtFile = fullfile(outputDir, 's10_batch_31_colors_nearest3_ratio_weighted_realtime_log.txt');

%% Load config and spectra once
addpath(inputDir);
caseCfg = guangpu_case_config();

wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;

std = load_standard_data(standardDir, wl);

raw.R = read_spd(fullfile(inputDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(inputDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(inputDir, 'B.CSV'), '', wl);
raw.W = read_spd(fullfile(inputDir, 'W.CSV'), '', wl);
raw.SingleWhite = read_spd(fullfile(inputDir, 'W.CSV'), '', wl);

spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, dLambda);
spd.W = scale_to_photopic_Y(raw.W, channel_xyY.W(3), std, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, dLambda);

actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);

rgbTriangleXY = [actual_xyY.R(1:2); actual_xyY.G(1:2); actual_xyY.B(1:2)];
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', ...
                  xyY_to_XYZ(actual_xyY.G).', ...
                  xyY_to_XYZ(actual_xyY.B).'];

if isnan(targetY_lm)
    targetY_lm = caseCfg.target_xyY(3);
end

singleNorm = scale_to_photopic_Y(spd.SingleWhite, targetY_lm, std, dLambda);
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
singleWhiteXY = singleMetrics.xyY(1:2);

lut = readtable(lutFile, 'VariableNamingRule', 'preserve');
requiredCols = {'Target_x','Target_y','Target_Y_lm','Target_u1976','Target_v1976', ...
    'Best_Ra','Best_SingleWhiteRatio','Best_RGBPoint_x','Best_RGBPoint_y', ...
    'Best_RGBPoint_Y_lm','Best_R_weight','Best_G_weight','Best_B_weight','BoundaryEdge'};
for iCol = 1:numel(requiredCols)
    if ~ismember(requiredCols{iCol}, lut.Properties.VariableNames)
        error('LUT missing column: %s', requiredCols{iCol});
    end
end

%% Realtime result window
[dialogHandle, editHandle] = create_realtime_result_window();

logLines = strings(0, 1);
logLines = append_realtime_line(logLines, editHandle, '================ S10 Batch Realtime Result ================');
logLines = append_realtime_line(logLines, editHandle, sprintf('Color count = %u', uint32(size(targetUvDiv5List, 1))));
logLines = append_realtime_line(logLines, editHandle, sprintf('Y = %.6f, K = %u, power = %.6f', targetY_lm, uint32(nearestGridK), nearestGridWeightPower));
logLines = append_realtime_line(logLines, editHandle, sprintf('White xy = (%.6f, %.6f)', singleWhiteXY(1), singleWhiteXY(2)));
logLines = append_realtime_line(logLines, editHandle, ' ');
logLines = append_realtime_line(logLines, editHandle, sprintf('%-4s %-5s %-5s %-9s %-9s %-10s %-10s %-10s %-10s %-10s %-12s %-18s', ...
    'No', 'U', 'V', 'ExactRa', 'S10Ra', 'RaDiff', 'ExactW', 'S10W', 'WDiff', 'OutDuv', 'S10RGBPWM', 'Status'));
logLines = append_realtime_line(logLines, editHandle, string(repmat('-', 1, 150)));

%% Preallocate results
n = size(targetUvDiv5List, 1);
resultRows = repmat(make_batch_empty_row(), n, 1);

%% Batch calculation
for idxColor = 1:n
    rowOut = make_batch_empty_row();
    rowOut.Index = idxColor;
    rowOut.IntSig_u = targetUvDiv5List(idxColor, 1);
    rowOut.IntSig_v = targetUvDiv5List(idxColor, 2);

    try
        targetUvDiv5 = double(targetUvDiv5List(idxColor, :));
        targetUvQ1000 = targetUvDiv5 .* 5.0;
        targetUv = targetUvQ1000 ./ 1000.0;
        targetXY = uv1976_to_xy(targetUv);

        if any(~isfinite(targetXY))
            error('Invalid target uv: [%.6f, %.6f].', targetUv(1), targetUv(2));
        end

        target_xyY = [targetXY, targetY_lm];
        targetXYZ = xyY_to_XYZ(target_xyY); %#ok<NASGU>

        exact = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
            spd, singleNorm, std, wl, dLambda, rayScanCount);

        if exact.Valid
            targetBestRGBPointXY = exact.RGBPoint_xyY(1:2);
        else
            targetBestRGBPointXY = [NaN, NaN];
        end

        s10 = find_stored_ratio_result(target_xyY, targetUv, targetBestRGBPointXY, singleWhiteXY, rgbTriangleXY, ...
            rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, lut, ...
            nearestGridMetric, nearestGridK, nearestGridWeightPower);

        rowOut.Target_u1976 = targetUv(1);
        rowOut.Target_v1976 = targetUv(2);
        rowOut.Target_x = target_xyY(1);
        rowOut.Target_y = target_xyY(2);
        rowOut.Target_Y_lm = target_xyY(3);

        rowOut.ExactValid = double(exact.Valid);
        rowOut.S10Valid = double(s10.Valid);

        if exact.Valid
            rowOut.ExactRa = exact.Ra;
            rowOut.ExactWhiteRatio = exact.SingleWhiteRatio;
            rowOut.ExactRGBRatio = exact.RGBWhiteRatio;
            rowOut.ExactRGBPoint_x = exact.RGBPoint_xyY(1);
            rowOut.ExactRGBPoint_y = exact.RGBPoint_xyY(2);
            rowOut.ExactRGBPoint_Y_lm = exact.RGBPoint_xyY(3);
            rowOut.Exact_R_weight = exact.R_weight;
            rowOut.Exact_G_weight = exact.G_weight;
            rowOut.Exact_B_weight = exact.B_weight;
        end

        if s10.Valid
            rowOut.S10Ra = s10.Ra;
            rowOut.S10WhiteRatio = s10.SingleWhiteRatio;
            rowOut.S10WeightedWhiteRatio = s10.WeightedWhiteRatio;
            rowOut.S10RGBRatio = s10.RGBWhiteRatio;
            rowOut.S10OutputDeltaUv = s10.OutputDeltaUv;
            rowOut.S10RGBPoint_x = s10.RGBPoint_xyY(1);
            rowOut.S10RGBPoint_y = s10.RGBPoint_xyY(2);
            rowOut.S10RGBPoint_Y_lm = s10.RGBPoint_xyY(3);
            rowOut.S10_R_weight = s10.R_weight;
            rowOut.S10_G_weight = s10.G_weight;
            rowOut.S10_B_weight = s10.B_weight;
            rowOut.S10NearestLutIndex0 = s10.LutIndex0;
            rowOut.S10CandidateCount = s10.CandidateCount;
        end

        if exact.Valid && s10.Valid
            rowOut.RaDiff_ExactMinusS10 = exact.Ra - s10.Ra;
            rowOut.WhiteRatioDiff_ExactMinusS10 = exact.SingleWhiteRatio - s10.SingleWhiteRatio;
        end

        pwm = batch_calc_pwm([rowOut.S10_R_weight, rowOut.S10_G_weight, rowOut.S10_B_weight, rowOut.S10WhiteRatio], pwmMax);
        rowText = sprintf('%-4u %-5u %-5u %-9.4f %-9.4f %-10.4f %-10.4f %-10.4f %-10.4f %-10.6f (%3u,%3u,%3u,%3u) %-18s', ...
            uint32(idxColor), uint32(rowOut.IntSig_u), uint32(rowOut.IntSig_v), ...
            rowOut.ExactRa, rowOut.S10Ra, rowOut.RaDiff_ExactMinusS10, ...
            rowOut.ExactWhiteRatio, rowOut.S10WhiteRatio, rowOut.WhiteRatioDiff_ExactMinusS10, ...
            rowOut.S10OutputDeltaUv, pwm(1), pwm(2), pwm(3), pwm(4), 'OK');

        logLines = append_realtime_line(logLines, editHandle, rowText);
        fprintf('%s\n', rowText);
    catch ME
        rowOut.ErrorMessage = string(ME.message);
        rowText = sprintf('%-4u %-5u %-5u %-9s %-9s %-10s %-10s %-10s %-10s %-10s %-12s ERROR: %s', ...
            uint32(idxColor), uint32(rowOut.IntSig_u), uint32(rowOut.IntSig_v), ...
            '-', '-', '-', '-', '-', '-', '-', '-', ME.message);
        logLines = append_realtime_line(logLines, editHandle, rowText);
        fprintf('%s\n', rowText);
    end

    resultRows(idxColor) = rowOut;
end

%% Export results
resultTable = struct2table(resultRows);
writetable(resultTable, outputCsvFile);

fid = fopen(outputTxtFile, 'w');
if fid >= 0
    fprintf(fid, '%s\n', char(strjoin(logLines, newline)));
    fclose(fid);
end

logLines = append_realtime_line(logLines, editHandle, ' ');
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported CSV: %s', outputCsvFile));
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported TXT: %s', outputTxtFile));
logLines = append_realtime_line(logLines, editHandle, 'Done.');

if ishghandle(dialogHandle)
    set(dialogHandle, 'Name', 'S10 Batch Result - Done');
end

fprintf('\nExported CSV: %s\n', outputCsvFile);
fprintf('Exported TXT: %s\n', outputTxtFile);
fprintf('Done.\n');

%% ================= Batch helper functions =================
function row = make_batch_empty_row()
    row.Index = NaN;
    row.IntSig_u = NaN;
    row.IntSig_v = NaN;
    row.Target_u1976 = NaN;
    row.Target_v1976 = NaN;
    row.Target_x = NaN;
    row.Target_y = NaN;
    row.Target_Y_lm = NaN;

    row.ExactValid = NaN;
    row.S10Valid = NaN;

    row.ExactRa = NaN;
    row.S10Ra = NaN;
    row.RaDiff_ExactMinusS10 = NaN;

    row.ExactWhiteRatio = NaN;
    row.S10WhiteRatio = NaN;
    row.S10WeightedWhiteRatio = NaN;
    row.WhiteRatioDiff_ExactMinusS10 = NaN;

    row.ExactRGBRatio = NaN;
    row.S10RGBRatio = NaN;
    row.S10OutputDeltaUv = NaN;

    row.ExactRGBPoint_x = NaN;
    row.ExactRGBPoint_y = NaN;
    row.ExactRGBPoint_Y_lm = NaN;
    row.S10RGBPoint_x = NaN;
    row.S10RGBPoint_y = NaN;
    row.S10RGBPoint_Y_lm = NaN;

    row.Exact_R_weight = NaN;
    row.Exact_G_weight = NaN;
    row.Exact_B_weight = NaN;
    row.S10_R_weight = NaN;
    row.S10_G_weight = NaN;
    row.S10_B_weight = NaN;

    row.S10NearestLutIndex0 = NaN;
    row.S10CandidateCount = NaN;
    row.ErrorMessage = "";
end

function [dialogHandle, editHandle] = create_realtime_result_window()
    dialogHandle = [];
    editHandle = [];

    try
        dialogHandle = dialog('Name', 'S10 Batch Result - Running', ...
                              'Position', [80, 60, 1320, 780], ...
                              'WindowStyle', 'normal');

        uicontrol('Parent', dialogHandle, ...
                  'Style', 'text', ...
                  'String', 'S10 batch result, one line is appended immediately after each color finishes.', ...
                  'Units', 'normalized', ...
                  'Position', [0.03, 0.94, 0.94, 0.04], ...
                  'FontSize', 11, ...
                  'FontWeight', 'bold', ...
                  'HorizontalAlignment', 'left');

        editHandle = uicontrol('Parent', dialogHandle, ...
                               'Style', 'edit', ...
                               'String', '', ...
                               'Units', 'normalized', ...
                               'Position', [0.03, 0.10, 0.94, 0.83], ...
                               'Max', 2, ...
                               'Min', 0, ...
                               'HorizontalAlignment', 'left', ...
                               'FontName', 'Consolas', ...
                               'FontSize', 9);

        uicontrol('Parent', dialogHandle, ...
                  'Style', 'pushbutton', ...
                  'String', '关闭', ...
                  'Units', 'normalized', ...
                  'Position', [0.84, 0.035, 0.10, 0.045], ...
                  'Callback', @(~,~) close(dialogHandle));
    catch ME
        fprintf('Realtime result window open failed: %s\n', ME.message);
    end
end

function logLines = append_realtime_line(logLines, editHandle, newLine)
    logLines(end + 1, 1) = string(newLine);

    if ~isempty(editHandle) && ishghandle(editHandle)
        set(editHandle, 'String', char(strjoin(logLines, newline)));
        drawnow;
    end
end

function pwm = batch_calc_pwm(values, pwmMax)
    values(~isfinite(values)) = 0;
    values = min(max(values, 0), 1);
    pwm = uint16(round(values .* pwmMax));
end


%% ================= Local functions =================

function result = find_stored_ratio_result(target_xyY, targetUv, targetBestRGBPointXY, singleWhiteXY, rgbTriangleXY, ...
    rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, lut, ...
    nearestGridMetric, nearestGridK, nearestGridWeightPower)

    result = empty_stored_ratio_result();
    targetXY = target_xyY(1:2);

    [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric);

    candidateStruct = repmat(empty_stored_ratio_candidate(), nearestGridK, 1);
    validCount = 0;

    % S10 picking rule:
    % distance sorting -> first 3 valid LUT rows.
    for i = 1:numel(sortedIndex)
        lutIndex = sortedIndex(i);
        row = lut(lutIndex, :);
        storedWhiteRatio = row.Best_SingleWhiteRatio;
        storedBestRGBPointXY = [row.Best_RGBPoint_x, row.Best_RGBPoint_y];

        if ~isfinite(storedWhiteRatio) || any(~isfinite(storedBestRGBPointXY))
            continue;
        end

        validCount = validCount + 1;
        candidate = empty_stored_ratio_candidate();
        candidate.Valid = true;
        candidate.Rank = validCount;
        candidate.LutIndex0 = lutIndex - 1;
        candidate.CandidateDeltaUv = sortedDistanceList(i);
        candidate.StoredWhiteRatio = storedWhiteRatio;
        candidate.StoredBestRa = row.Best_Ra;
        candidate.StoredBestRGBPoint_xyY = [storedBestRGBPointXY, row.Best_RGBPoint_Y_lm];
        candidate.SelectedGridTarget_xyY = [row.Target_x, row.Target_y, row.Target_Y_lm];
        candidate.SelectedGridTarget_uv = [row.Target_u1976, row.Target_v1976];
        candidate.BoundaryEdge = string(row.BoundaryEdge);

        if all(isfinite(targetBestRGBPointXY)) && all(isfinite(storedBestRGBPointXY))
            storedBestRGBPointUv = xy_to_uv1976(storedBestRGBPointXY);
            targetBestRGBPointUv = xy_to_uv1976(targetBestRGBPointXY);
            candidate.BestRGBPointDeltaUvToTargetBest = norm(storedBestRGBPointUv - targetBestRGBPointUv);
        end

        singleRatioResult = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
            rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, storedWhiteRatio);

        if singleRatioResult.Valid
            candidate.Ra = singleRatioResult.Ra;
            candidate.Rmin8 = singleRatioResult.Rmin8;
            candidate.SingleWhiteRatio = singleRatioResult.SingleWhiteRatio;
            candidate.RGBWhiteRatio = singleRatioResult.RGBWhiteRatio;
            candidate.RGBPoint_xyY = singleRatioResult.RGBPoint_xyY;
            candidate.R_weight = singleRatioResult.R_weight;
            candidate.G_weight = singleRatioResult.G_weight;
            candidate.B_weight = singleRatioResult.B_weight;
            candidate.OutputDeltaUv = 0;
            candidate.Output_xyY = target_xyY;
            candidate.ResidualXYZ = 0;
        end

        candidateStruct(validCount) = candidate;

        if validCount >= nearestGridK
            break;
        end
    end

    if validCount <= 0
        return;
    end

    candidateStruct = candidateStruct(1:validCount);
    distanceList = [candidateStruct.CandidateDeltaUv].';
    whiteRatioList = [candidateStruct.StoredWhiteRatio].';

    zeroIdx = find(distanceList <= 1e-12, 1, 'first');
    if ~isempty(zeroIdx)
        weights = zeros(validCount, 1);
        weights(zeroIdx) = 1;
    else
        weights = 1 ./ (distanceList .^ nearestGridWeightPower);
        weights = weights ./ sum(weights);
    end

    weightedWhiteRatio = sum(weights .* whiteRatioList);

    for i = 1:validCount
        candidateStruct(i).DistanceWeight = weights(i);
        candidateStruct(i).WeightedContribution = weights(i) * candidateStruct(i).StoredWhiteRatio;
    end

    finalRatioResult = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
        rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, weightedWhiteRatio);

    result.CandidateCount = validCount;
    result.SearchDeltaUvLimit = NaN;
    result.UsedFallback = validCount < nearestGridK;
    result.CandidateTable = build_stored_ratio_candidate_table(candidateStruct);
    result.WeightedWhiteRatio = weightedWhiteRatio;

    if ~finalRatioResult.Valid
        return;
    end

    result.Valid = true;
    result.Ra = finalRatioResult.Ra;
    result.Rmin8 = finalRatioResult.Rmin8;
    result.SingleWhiteRatio = finalRatioResult.SingleWhiteRatio;
    result.RGBWhiteRatio = finalRatioResult.RGBWhiteRatio;
    result.RGBPoint_xyY = finalRatioResult.RGBPoint_xyY;
    result.R_weight = finalRatioResult.R_weight;
    result.G_weight = finalRatioResult.G_weight;
    result.B_weight = finalRatioResult.B_weight;
    result.ResidualXYZ = 0;
    result.OutputDeltaUv = 0;
    result.Output_xyY = target_xyY;
    result.StoredWhiteRatio = weightedWhiteRatio;
    result.StoredBestRa = NaN;
    result.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    result.BestRGBPointDeltaUvToTargetBest = NaN;
    result.LutIndex0 = candidateStruct(1).LutIndex0;
    result.SelectedGridTarget_xyY = candidateStruct(1).SelectedGridTarget_xyY;
    result.SelectedGridTarget_uv = candidateStruct(1).SelectedGridTarget_uv;
    result.BoundaryEdge = "";
end

function result = evaluate_ratio_on_current_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
    rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, storedWhiteRatio)

    result = empty_best_result();

    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    maxWhiteRatio = white_ratio_from_rayt(target_xyY, singleWhiteXY, boundaryRayT);
    whiteRatio = min(max(storedWhiteRatio, 0), maxWhiteRatio);

    rayT = rayt_from_white_ratio(target_xyY, singleWhiteXY, whiteRatio);
    rayT = min(max(rayT, 1), boundaryRayT);

    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);
end

function T = build_stored_ratio_candidate_table(candidateStruct)
    validMask = [candidateStruct.Valid];
    if ~any(validMask)
        T = table();
        return;
    end

    c = candidateStruct(validMask);
    T = table( ...
        [c.Rank].', ...
        [c.LutIndex0].', ...
        [c.CandidateDeltaUv].', ...
        reshape([c.SelectedGridTarget_uv], 2, []).', ...
        reshape([c.SelectedGridTarget_xyY], 3, []).', ...
        [c.StoredWhiteRatio].', ...
        [c.DistanceWeight].', ...
        [c.WeightedContribution].', ...
        [c.StoredBestRa].', ...
        reshape([c.StoredBestRGBPoint_xyY], 3, []).', ...
        [c.BestRGBPointDeltaUvToTargetBest].', ...
        [c.Ra].', ...
        'VariableNames', { ...
            'Rank', ...
            'LutIndex0', ...
            'DistanceUv', ...
            'GridTarget_uv', ...
            'GridTarget_xyY', ...
            'StoredWhiteRatio', ...
            'DistanceWeight', ...
            'WeightedContribution', ...
            'OwnBestRa', ...
            'OwnBestRGBPoint_xyY', ...
            'BestRGBPointDeltaUvToTargetBestRGBPoint', ...
            'SingleRatioAppliedToTargetRa'});

    T = sortrows(T, {'Rank'}, {'ascend'});
end

function print_result_summary(result, targetUv, label)
    if ~result.Valid
        fprintf('%s: invalid result.\n', label);
        return;
    end

    outputUv = xy_to_uv1976(result.RGBPoint_xyY(1:2));
    fprintf('%s Ra                = %.6f\n', label, result.Ra);
    fprintf('%s WhiteRatio        = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s RGBWhiteRatio     = %.6f\n', label, result.RGBWhiteRatio);
    fprintf('%s RGBPoint xyY      = (%.6f, %.6f, %.6f)\n', label, ...
        result.RGBPoint_xyY(1), result.RGBPoint_xyY(2), result.RGBPoint_xyY(3));
    fprintf('%s RGB weights       = (R=%.6f, G=%.6f, B=%.6f)\n', label, ...
        result.R_weight, result.G_weight, result.B_weight);
    fprintf('%s RGBPoint uv       = (%.6f, %.6f)\n', label, outputUv(1), outputUv(2));
    fprintf('%s OutputDeltaUv     = %.6f (constructed target ray)\n', label, 0.0);
end

function print_stored_ratio_summary(result, targetUv, label)
    if ~result.Valid
        fprintf('%s: invalid result.\n', label);
        return;
    end

    outputUv = xy_to_uv1976(result.Output_xyY(1:2));
    fprintf('%s Ra                  = %.6f\n', label, result.Ra);
    fprintf('%s WhiteRatio          = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s WeightedWhiteRatio  = %.6f\n', label, result.WeightedWhiteRatio);
    fprintf('%s UsedWhiteRatio      = %.6f\n', label, result.SingleWhiteRatio);
    fprintf('%s RGBWhiteRatio       = %.6f\n', label, result.RGBWhiteRatio);
    fprintf('%s ResidualXYZ         = %.9f\n', label, result.ResidualXYZ);
    fprintf('%s OutputDeltaUv       = %.9f\n', label, result.OutputDeltaUv);
    fprintf('%s LUT index           = %d\n', label, int32(result.LutIndex0));
    fprintf('%s Candidate count     = %u\n', label, uint32(result.CandidateCount));
    fprintf('%s Used fallback       = %u\n', label, uint32(result.UsedFallback));
    fprintf('%s Grid target uv      = (%.6f, %.6f)\n', label, ...
        result.SelectedGridTarget_uv(1), result.SelectedGridTarget_uv(2));
    fprintf('%s RatioK RGB xyY      = (%.6f, %.6f, %.6f)\n', label, ...
        result.RGBPoint_xyY(1), result.RGBPoint_xyY(2), result.RGBPoint_xyY(3));
    fprintf('%s Output xyY          = (%.6f, %.6f, %.6f)\n', label, ...
        result.Output_xyY(1), result.Output_xyY(2), result.Output_xyY(3));
    fprintf('%s Output uv           = (%.6f, %.6f)\n', label, outputUv(1), outputUv(2));
    fprintf('%s RGB weights         = (R=%.6f, G=%.6f, B=%.6f)\n', label, ...
        result.R_weight, result.G_weight, result.B_weight);
    fprintf('%s BoundaryEdge        = %s\n', label, char(result.BoundaryEdge));
    fprintf('%s Delta uv to target  = %.9f\n', label, norm(outputUv - targetUv));
end

function s = empty_best_result()
    s.Valid = false;
    s.Ra = NaN;
    s.Rmin8 = NaN;
    s.RGBPoint_xyY = [NaN, NaN, NaN];
    s.RayT = NaN;
    s.SingleWhiteRatio = NaN;
    s.RGBWhiteRatio = NaN;
    s.R_weight = NaN;
    s.G_weight = NaN;
    s.B_weight = NaN;
end

function s = empty_stored_ratio_candidate()
    s.Valid = false;
    s.Rank = NaN;
    s.LutIndex0 = NaN;
    s.CandidateDeltaUv = NaN;
    s.DistanceWeight = NaN;
    s.WeightedContribution = NaN;
    s.StoredWhiteRatio = NaN;
    s.StoredBestRa = NaN;
    s.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    s.BestRGBPointDeltaUvToTargetBest = NaN;
    s.SelectedGridTarget_xyY = [NaN, NaN, NaN];
    s.SelectedGridTarget_uv = [NaN, NaN];
    s.BoundaryEdge = "";
    s.ResidualXYZ = NaN;
    s.OutputDeltaUv = NaN;
    s.Output_xyY = [NaN, NaN, NaN];
    s.Ra = NaN;
    s.Rmin8 = NaN;
    s.SingleWhiteRatio = NaN;
    s.RGBWhiteRatio = NaN;
    s.RGBPoint_xyY = [NaN, NaN, NaN];
    s.R_weight = NaN;
    s.G_weight = NaN;
    s.B_weight = NaN;
end

function s = empty_stored_ratio_result()
    s = empty_best_result();
    s.ResidualXYZ = NaN;
    s.OutputDeltaUv = NaN;
    s.Output_xyY = [NaN, NaN, NaN];
    s.StoredWhiteRatio = NaN;
    s.StoredBestRa = NaN;
    s.StoredBestRGBPoint_xyY = [NaN, NaN, NaN];
    s.BestRGBPointDeltaUvToTargetBest = NaN;
    s.LutIndex0 = NaN;
    s.SelectedGridTarget_xyY = [NaN, NaN, NaN];
    s.SelectedGridTarget_uv = [NaN, NaN];
    s.BoundaryEdge = "";
    s.UsedFallback = false;
    s.CandidateCount = 0;
    s.SearchDeltaUvLimit = NaN;
    s.WeightedWhiteRatio = NaN;
    s.CandidateTable = table();
end

function best = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount)

    best = empty_best_result();
    targetXY = target_xyY(1:2);
    [pointXY, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);

    rayTList = linspace(1, boundaryRayT, rayScanCount);
    rayTList(end) = boundaryRayT;

    for i = 1:numel(rayTList)
        rayT = rayTList(i);
        rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);
        result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
            singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

        if ~result.Valid
            continue;
        end

        if ~best.Valid || result.Ra > best.Ra
            best = result;
        end
    end
end

function result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
    singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda)

    result = empty_best_result();

    split = split_target_on_ray(target_xyY, singleWhiteXY, rgbPointXY, rayT);
    point_xyY = [rgbPointXY, split.RGBY];

    try
        [weights, rgbSpd] = solve_rgb_point_spectrum(point_xyY, rgbSolveMatrix, spd);
    catch
        return;
    end

    mixSpd = split.WhiteYRatio .* singleNorm + rgbSpd;
    metrics = spectrum_metrics(mixSpd, std, wl, dLambda);

    result.Valid = true;
    result.Ra = metrics.Ra;
    result.Rmin8 = metrics.Rmin8;
    result.RGBPoint_xyY = point_xyY;
    result.RayT = rayT;
    result.SingleWhiteRatio = split.WhiteYRatio;
    result.RGBWhiteRatio = split.RGBYRatio;
    result.R_weight = weights(1);
    result.G_weight = weights(2);
    result.B_weight = weights(3);
end

function [pointXY, rayT, edgeName, edgeIndex] = find_ray_gamut_intersection(whiteXY, targetXY, triangleXY)
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    triangleXY = double(triangleXY(:, 1:2));
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_ray_gamut_intersection:SamePoint', 'White and target have the same xy.');
    end

    tol = 1e-12;
    rayT = Inf;
    edgeIndex = NaN;
    pointXY = [NaN, NaN];

    for i = 1:3
        edgeStart = triangleXY(i, :);
        edgeEnd = triangleXY(mod(i, 3) + 1, :);
        edgeVector = edgeEnd - edgeStart;
        systemMatrix = [direction(:), -edgeVector(:)];
        if abs(det(systemMatrix)) < tol
            continue;
        end

        params = systemMatrix \ (edgeStart - whiteXY).';
        t = params(1);
        s = params(2);

        if t >= 1 - tol && s >= -tol && s <= 1 + tol && t < rayT
            rayT = t;
            edgeIndex = i;
            pointXY = whiteXY + t .* direction;
        end
    end

    if ~isfinite(rayT)
        error('find_ray_gamut_intersection:NoIntersection', ...
            'The fixed-white to target ray does not hit the RGB gamut boundary after the target.');
    end

    edgeName = edgeNames{edgeIndex};
end

function split = split_target_on_ray(target_xyY, whiteXY, pointXY, rayT)
    targetY = target_xyY(3);
    target_y = target_xyY(2);

    lambda = 1 ./ rayT;
    oneMinusLambda = 1 - lambda;

    split.WhiteXYCoeff = oneMinusLambda;
    split.RGBXYCoeff = lambda;
    split.WhiteY = targetY * oneMinusLambda * whiteXY(2) / target_y;
    split.RGBY = targetY * lambda * pointXY(2) / target_y;
    split.WhiteYRatio = split.WhiteY / targetY;
    split.RGBYRatio = split.RGBY / targetY;
end

function rayT = rayt_from_white_ratio(target_xyY, whiteXY, whiteRatio)
    targetYCoord = target_xyY(2);
    whiteYCoord = whiteXY(2);

    if whiteRatio <= 0
        rayT = 1;
        return;
    end

    a = whiteRatio * targetYCoord / whiteYCoord;
    if a >= 1
        rayT = Inf;
    else
        rayT = 1 / (1 - a);
    end
end

function whiteRatio = white_ratio_from_rayt(target_xyY, whiteXY, rayT)
    targetYCoord = target_xyY(2);
    whiteYCoord = whiteXY(2);

    if rayT <= 1
        whiteRatio = 0;
        return;
    end

    whiteRatio = (1 - 1 / rayT) * whiteYCoord / targetYCoord;
end

function [sortedDistanceList, sortedIndex] = sort_grid_by_distance(lut, targetXY, nearestGridMetric)
    if strcmpi(nearestGridMetric, 'xy')
        lutTarget = [lut.Target_x, lut.Target_y];
        distanceList = vecnorm(lutTarget - targetXY, 2, 2);
    else
        targetUv = xy_to_uv1976(targetXY);
        lutTarget = [lut.Target_u1976, lut.Target_v1976];
        distanceList = vecnorm(lutTarget - targetUv, 2, 2);
    end

    [sortedDistanceList, sortedIndex] = sort(distanceList, 'ascend');
end

function [weights, rgbSpd] = solve_rgb_point_spectrum(point_xyY, rgbSolveMatrix, spd)
    b = xyY_to_XYZ(point_xyY).';
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;

    if any(weights < -1e-8)
        error('solve_rgb_point_spectrum:OutsideGamut', ...
            'RGB candidate xy=(%.6f, %.6f) is outside the measured RGB gamut.', ...
            point_xyY(1), point_xyY(2));
    end

    rgbSpd = weights(1) .* spd.R + weights(2) .* spd.G + weights(3) .* spd.B;
end

function [weights, rgbSpd] = solve_rgb_xyz_spectrum(rgbXYZ, rgbSolveMatrix, spd)
    b = rgbXYZ(:);
    weights = rgbSolveMatrix \ b;
    weights(abs(weights) < 1e-10) = 0;

    if any(weights < -1e-8)
        error('solve_rgb_xyz_spectrum:OutsideGamut', ...
            'The residual RGB XYZ is outside the measured RGB gamut.');
    end

    rgbSpd = weights(1) .* spd.R + weights(2) .* spd.G + weights(3) .* spd.B;
end

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xy = XYZ_to_xyY(XYZ);
    xyY = [xy(1), xy(2), 683 * XYZ(2)];
end

function std = load_standard_data(standardDir, wl)
    xyzPath = ensure_file(standardDir, 'CIE_xyz_1931_2deg.csv', 'https://files.cie.co.at/CIE_xyz_1931_2deg.csv');
    tcsPath = ensure_file(standardDir, 'CIE_srf_cri.csv', 'https://files.cie.co.at/CIE_srf_cri.csv');
    dPath = ensure_file(standardDir, 'CIE_illum_Dxx_comp.csv', 'https://files.cie.co.at/CIE_illum_Dxx_comp.csv');

    xyz = readmatrix(xyzPath);
    tcs = readmatrix(tcsPath);
    d = readmatrix(dPath);

    std.cmf = interp1(xyz(:,1), xyz(:,2:4), wl, 'linear', 'extrap');
    std.tcs = interp1(tcs(:,1), tcs(:,2:9), wl, 'linear', 'extrap');
    std.daylightComponents = interp1(d(:,1), d(:,2:4), wl, 'linear', 'extrap');
end

function path = ensure_file(folder, fileName, url)
    path = fullfile(folder, fileName);
    if ~exist(path, 'file')
        websave(path, url);
    end
end

function spd = read_spd(fileName, sheetName, wlOut)
    if isempty(sheetName)
        M = readmatrix(fileName);
    else
        M = readmatrix(fileName, 'Sheet', sheetName);
    end
    wl = M(:,1);
    if size(M, 2) >= 3
        s = M(:,3);
    else
        s = M(:,2);
    end
    ok = isfinite(wl) & isfinite(s);
    wl = wl(ok);
    s = s(ok);
    [wl, order] = sort(wl);
    s = s(order);
    spd = interp1(wl, s, wlOut, 'linear', 0);
    spd(spd < 0) = 0;
end

function XYZ = xyY_to_XYZ(xyY)
    x = xyY(1);
    y = xyY(2);
    Y = xyY(3);
    if Y == 0
        XYZ = [0, 0, 0];
        return;
    end
    X = x / y * Y;
    Z = (1 - x - y) / y * Y;
    XYZ = [X, Y, Z];
end

function spdScaled = scale_to_photopic_Y(spd, targetYlm, std, dLambda)
    Ylm = 683 * sum(spd(:) .* std.cmf(:,2)) * dLambda;
    if Ylm <= 0
        error('The spectrum photopic Y is 0 and cannot be normalized.');
    end
    spdScaled = spd(:) .* (targetYlm / Ylm);
end

function m = spectrum_metrics(spd, std, wl, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xyY = XYZ_to_xyY(XYZ);
    photopicY = 683 * XYZ(2);
    uv = XYZ_to_uv1960(XYZ);
    [CCT, ~] = estimate_cct_uv(uv, std.cmf, wl, dLambda);
    refSpd = reference_spd(CCT, std, wl);
    Ri = cri_R1_R8(spd, refSpd, std.cmf, std.tcs, dLambda);
    [Rmin8, ~] = min(Ri);
    m.xyY = [xyY(1), xyY(2), photopicY];
    m.photopicY = photopicY;
    m.CCT = CCT;
    m.Ri = Ri;
    m.Ra = mean(Ri);
    m.Rmin8 = Rmin8;
end

function XYZ = source_XYZ(spd, cmf, dLambda)
    XYZ = sum(spd(:) .* cmf, 1) .* dLambda;
end

function xyY = XYZ_to_xyY(XYZ)
    s = sum(XYZ);
    if s <= 0
        xyY = [NaN, NaN, NaN];
    else
        xyY = [XYZ(1) / s, XYZ(2) / s, XYZ(2)];
    end
end

function uv = XYZ_to_uv1960(XYZ)
    den = XYZ(1) + 15 * XYZ(2) + 3 * XYZ(3);
    uv = [4 * XYZ(1) / den, 6 * XYZ(2) / den];
end

function uv = xy_to_uv1976(xy)
    x = xy(1);
    y = xy(2);
    den = -2 * x + 12 * y + 3;
    uv = [4 * x / den, 9 * y / den];
end

function [CCT, Duv] = estimate_cct_uv(uv, cmf, wl, dLambda)
    gridT = linspace(1000, 25000, 481);
    dist2 = zeros(size(gridT));
    for i = 1:numel(gridT)
        bb = blackbody_spd(gridT(i), wl);
        bbXYZ = source_XYZ(bb, cmf, dLambda);
        bbuv = XYZ_to_uv1960(bbXYZ);
        dist2(i) = sum((uv - bbuv) .^ 2);
    end
    [~, idx] = min(dist2);
    lo = max(1000, gridT(max(1, idx - 2)));
    hi = min(25000, gridT(min(numel(gridT), idx + 2)));
    CCT = fminbnd(@(T) blackbody_uv_distance2(T, uv, cmf, wl, dLambda), lo, hi);
    Duv = sqrt(blackbody_uv_distance2(CCT, uv, cmf, wl, dLambda));
end

function d2 = blackbody_uv_distance2(T, uv, cmf, wl, dLambda)
    bb = blackbody_spd(T, wl);
    bbXYZ = source_XYZ(bb, cmf, dLambda);
    bbuv = XYZ_to_uv1960(bbXYZ);
    d2 = sum((uv - bbuv) .^ 2);
end

function spd = blackbody_spd(T, wl)
    c2 = 1.438776877e-2;
    lambda = wl(:) * 1e-9;
    spd = 1 ./ (lambda .^ 5 .* (exp(c2 ./ (lambda .* T)) - 1));
    spd = spd ./ max(spd);
end

function spd = reference_spd(CCT, std, wl)
    if CCT < 5000
        spd = blackbody_spd(CCT, wl);
        return;
    end

    T = min(max(CCT, 4000), 25000);
    if T <= 7000
        xD = 0.244063 + 0.09911e3 / T + 2.9678e6 / T^2 - 4.6070e9 / T^3;
    else
        xD = 0.237040 + 0.24748e3 / T + 1.9018e6 / T^2 - 2.0064e9 / T^3;
    end
    yD = -3.000 * xD^2 + 2.870 * xD - 0.275;
    den = 0.0241 + 0.2562 * xD - 0.7341 * yD;
    M1 = (-1.3515 - 1.7703 * xD + 5.9114 * yD) / den;
    M2 = (0.0300 - 31.4424 * xD + 30.0717 * yD) / den;
    spd = std.daylightComponents(:,1) + M1 .* std.daylightComponents(:,2) + M2 .* std.daylightComponents(:,3);
    spd = max(spd, 0);
end

function Ri = cri_R1_R8(testSpd, refSpd, cmf, tcs, dLambda)
    testXYZ = source_XYZ(testSpd, cmf, dLambda);
    refXYZ = source_XYZ(refSpd, cmf, dLambda);
    testUV = XYZ_to_uv1960(testXYZ);
    refUV = XYZ_to_uv1960(refXYZ);

    testUVW = tcs_uvw(testSpd, cmf, tcs, dLambda, testUV, refUV, true);
    refUVW = tcs_uvw(refSpd, cmf, tcs, dLambda, refUV, refUV, false);

    Ri = zeros(8, 1);
    for i = 1:8
        deltaE = norm(refUVW(i,:) - testUVW(i,:));
        Ri(i) = 100 - 4.6 * deltaE;
    end
end

function UVW = tcs_uvw(sourceSpd, cmf, tcs, dLambda, sourceUV, refUV, doAdapt)
    sourceY = sum(sourceSpd(:) .* cmf(:,2)) * dLambda;
    k = 100 / sourceY;
    UVW = zeros(size(tcs, 2), 3);

    cfun = @(u, v) (4 - u - 10 * v) / v;
    dfun = @(u, v) (1.708 * v + 0.404 - 1.481 * u) / v;
    sourceC = cfun(sourceUV(1), sourceUV(2));
    sourceD = dfun(sourceUV(1), sourceUV(2));
    refC = cfun(refUV(1), refUV(2));
    refD = dfun(refUV(1), refUV(2));

    for i = 1:size(tcs, 2)
        sampleXYZ = k .* sum(sourceSpd(:) .* tcs(:,i) .* cmf, 1) .* dLambda;
        sampleXY = XYZ_to_xyY(sampleXYZ);
        sampleUV = XYZ_to_uv1960(sampleXYZ);
        u = sampleUV(1);
        v = sampleUV(2);
        if doAdapt
            sampleC = cfun(u, v);
            sampleD = dfun(u, v);
            denominator = 16.518 + 1.481 * refC / sourceC * sampleC - refD / sourceD * sampleD;
            u = (10.872 + 0.404 * refC / sourceC * sampleC - 4 * refD / sourceD * sampleD) / denominator;
            v = 5.520 / denominator;
        end
        W = 25 * sampleXY(3)^(1/3) - 17;
        U = 13 * W * (u - refUV(1));
        V = 13 * W * (v - refUV(2));
        UVW(i,:) = [U, V, W];
    end
end

function xy = uv1976_to_xy(uv)
    u = uv(1);
    v = uv(2);
    den = 6 * u - 16 * v + 12;

    if abs(den) < eps
        xy = [NaN, NaN];
    else
        x = 9 * u / den;
        y = 4 * v / den;
        xy = [x, y];
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
            error('find_project_root:NotFound', ...
                'Cannot find %s from script folder or its parent folders.', inputFolderName);
        end

        projectRoot = parentDir;
    end
end
