% S13_CONTINUITY_SIM_EXACT_RA_DENSE_GRID
% Dense simulation for continuity evaluation of ExactRa and BestWhiteRatio.
%
% Purpose:
%   1. Generate a dense local grid around one input target color.
%   2. Calculate exact ray-scan Ra and BestWhiteRatio at every valid point.
%   3. Compare adjacent dense points to evaluate continuity behavior.
%   4. Export CSV/PNG results for later analysis.
%
% Interpretation:
%   - ExactRa continuity is evaluated by adjacent-point DeltaRa.
%   - BestWhiteRatio continuity is evaluated by adjacent-point DeltaWhiteRatio.
%   - Large DeltaWhiteRatio with small DeltaRa means possible argmax branch switching.
%   - If large jumps disappear after increasing pointCountPerAxis, the old LUT was too sparse.

clear;
clc;

%% ================= User config =================
centerUvDiv5 = [39, 110];       % MCU-style input, u'v' in Q1000/5
scanPlane = 'xy';               % 'xy' or 'uv1976'. Recommended: 'xy' after xy-grid LUT experiment.
halfSpan = 0.020;               % If scanPlane='xy': xy half-span. If 'uv1976': u'v' half-span.
pointCountPerAxis = 61;         % Dense grid count per axis. Increase to 81/101 for stronger verification.
targetY_lm = NaN;               % NaN -> use guangpu_case_config().target_xyY(3)
rayScanCount = 1001;            % Exact method ray scan count.

% Continuity evaluation thresholds.
raJumpThreshold = 2.0;           % Adjacent DeltaRa larger than this is suspicious for ExactRa.
whiteJumpThreshold = 0.050;      % Adjacent DeltaWhiteRatio larger than this is suspicious.
nearTieRaThreshold = 0.500;      % Large WhiteRatio jump with small DeltaRa suggests argmax branch switch.
reportTopN = 50;
plotEnable = true;
realtimeWindowEnable = true;

fprintf('\n============================================================\n');
fprintf('S13 Dense continuity simulation: ExactRa and BestWhiteRatio\n');
fprintf('Center uv(div5)     = [%u, %u]\n', centerUvDiv5(1), centerUvDiv5(2));
fprintf('Scan plane          = %s\n', scanPlane);
fprintf('Half span           = %.6f\n', halfSpan);
fprintf('Point count / axis  = %u\n', uint32(pointCountPerAxis));
fprintf('Ray scan count      = %u\n', uint32(rayScanCount));
fprintf('============================================================\n\n');

%% ================= Locate project =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

if ~exist(outputDir, 'dir')
    mkdir(outputDir);
end

outputPrefix = sprintf('s13_continuity_%s_center_%u_%u_N%u', scanPlane, uint32(centerUvDiv5(1)), uint32(centerUvDiv5(2)), uint32(pointCountPerAxis));
outputDenseCsv = fullfile(outputDir, [outputPrefix '_dense_results.csv']);
outputNeighborCsv = fullfile(outputDir, [outputPrefix '_neighbor_pairs.csv']);
outputSummaryCsv = fullfile(outputDir, [outputPrefix '_summary.csv']);
outputLogTxt = fullfile(outputDir, [outputPrefix '_log.txt']);
outputFigRa = fullfile(outputDir, [outputPrefix '_ExactRa.png']);
outputFigWhite = fullfile(outputDir, [outputPrefix '_BestWhiteRatio.png']);

%% ================= Load config and spectra =================
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

centerUvQ1000 = double(centerUvDiv5(:).') .* 5.0;
centerUv = centerUvQ1000 ./ 1000.0;
centerXY = uv1976_to_xy(centerUv);

fprintf('Center xyY = (%.6f, %.6f, %.6f)\n', centerXY(1), centerXY(2), targetY_lm);
fprintf('Center uv  = (%.6f, %.6f)\n', centerUv(1), centerUv(2));
fprintf('White xy   = (%.6f, %.6f)\n\n', singleWhiteXY(1), singleWhiteXY(2));

%% ================= Build dense local grid =================
axis1 = linspace(-halfSpan, halfSpan, pointCountPerAxis);
axis2 = linspace(-halfSpan, halfSpan, pointCountPerAxis);

ExactRaGrid = nan(pointCountPerAxis, pointCountPerAxis);
WhiteRatioGrid = nan(pointCountPerAxis, pointCountPerAxis);
RgbRatioGrid = nan(pointCountPerAxis, pointCountPerAxis);
ValidGrid = false(pointCountPerAxis, pointCountPerAxis);
TargetXGrid = nan(pointCountPerAxis, pointCountPerAxis);
TargetYGrid = nan(pointCountPerAxis, pointCountPerAxis);
TargetUGrid = nan(pointCountPerAxis, pointCountPerAxis);
TargetVGrid = nan(pointCountPerAxis, pointCountPerAxis);
RWeightGrid = nan(pointCountPerAxis, pointCountPerAxis);
GWeightGrid = nan(pointCountPerAxis, pointCountPerAxis);
BWeightGrid = nan(pointCountPerAxis, pointCountPerAxis);

[dialogHandle, editHandle] = create_continuity_result_window(realtimeWindowEnable);
logLines = strings(0, 1);
logLines = append_realtime_line(logLines, editHandle, '================ S13 Dense Continuity Simulation ================');
logLines = append_realtime_line(logLines, editHandle, sprintf('Center uv(div5)=[%u,%u], scanPlane=%s, N=%u', uint32(centerUvDiv5(1)), uint32(centerUvDiv5(2)), scanPlane, uint32(pointCountPerAxis)));
logLines = append_realtime_line(logLines, editHandle, sprintf('halfSpan=%.6f, rayScanCount=%u', halfSpan, uint32(rayScanCount)));
logLines = append_realtime_line(logLines, editHandle, ' ');

validCount = 0;
invalidCount = 0;
totalCount = pointCountPerAxis * pointCountPerAxis;
startTime = tic;

for rowIdx = 1:pointCountPerAxis
    for colIdx = 1:pointCountPerAxis
        linearIdx = (rowIdx - 1) * pointCountPerAxis + colIdx;

        if strcmpi(scanPlane, 'xy')
            targetXY = centerXY + [axis1(colIdx), axis2(rowIdx)];
            targetUv = xy_to_uv1976(targetXY);
        else
            targetUv = centerUv + [axis1(colIdx), axis2(rowIdx)];
            targetXY = uv1976_to_xy(targetUv);
        end

        TargetXGrid(rowIdx, colIdx) = targetXY(1);
        TargetYGrid(rowIdx, colIdx) = targetXY(2);
        TargetUGrid(rowIdx, colIdx) = targetUv(1);
        TargetVGrid(rowIdx, colIdx) = targetUv(2);

        if any(~isfinite(targetXY)) || any(~isfinite(targetUv)) || ~is_point_inside_triangle_xy(targetXY, rgbTriangleXY)
            invalidCount = invalidCount + 1;
            continue;
        end

        target_xyY = [targetXY, targetY_lm];

        try
            exact = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
                spd, singleNorm, std, wl, dLambda, rayScanCount);

            if exact.Valid
                ValidGrid(rowIdx, colIdx) = true;
                ExactRaGrid(rowIdx, colIdx) = exact.Ra;
                WhiteRatioGrid(rowIdx, colIdx) = exact.SingleWhiteRatio;
                RgbRatioGrid(rowIdx, colIdx) = exact.RGBWhiteRatio;
                RWeightGrid(rowIdx, colIdx) = exact.R_weight;
                GWeightGrid(rowIdx, colIdx) = exact.G_weight;
                BWeightGrid(rowIdx, colIdx) = exact.B_weight;
                validCount = validCount + 1;
            else
                invalidCount = invalidCount + 1;
            end
        catch
            invalidCount = invalidCount + 1;
        end

        if mod(linearIdx, max(1, floor(totalCount / 100))) == 0 || linearIdx == totalCount
            elapsed = toc(startTime);
            msg = sprintf('Progress %5u / %5u | valid=%5u | invalid=%5u | elapsed=%.1fs', ...
                uint32(linearIdx), uint32(totalCount), uint32(validCount), uint32(invalidCount), elapsed);
            logLines = append_realtime_line(logLines, editHandle, msg);
            fprintf('%s\n', msg);
        end
    end
end

%% ================= Build dense result table =================
denseRows = [];
for rowIdx = 1:pointCountPerAxis
    for colIdx = 1:pointCountPerAxis
        denseRows = [denseRows; ... %#ok<AGROW>
            rowIdx, colIdx, ValidGrid(rowIdx,colIdx), ...
            TargetXGrid(rowIdx,colIdx), TargetYGrid(rowIdx,colIdx), ...
            TargetUGrid(rowIdx,colIdx), TargetVGrid(rowIdx,colIdx), ...
            ExactRaGrid(rowIdx,colIdx), WhiteRatioGrid(rowIdx,colIdx), RgbRatioGrid(rowIdx,colIdx), ...
            RWeightGrid(rowIdx,colIdx), GWeightGrid(rowIdx,colIdx), BWeightGrid(rowIdx,colIdx)];
    end
end

denseTable = array2table(denseRows, 'VariableNames', ...
    {'Row','Col','Valid','Target_x','Target_y','Target_u1976','Target_v1976', ...
     'ExactRa','BestWhiteRatio','RGBRatio','R_weight','G_weight','B_weight'});
writetable(denseTable, outputDenseCsv);

%% ================= Neighbor continuity evaluation =================
neighborTable = build_neighbor_continuity_table(ExactRaGrid, WhiteRatioGrid, TargetXGrid, TargetYGrid, TargetUGrid, TargetVGrid, ValidGrid, scanPlane);

[summary, neighborTable] = summarize_continuity(neighborTable, validCount, invalidCount, pointCountPerAxis, ...
    raJumpThreshold, whiteJumpThreshold, nearTieRaThreshold);
writetable(neighborTable, outputNeighborCsv);
writetable(summary, outputSummaryCsv);

%% ================= Print summary =================
logLines = append_realtime_line(logLines, editHandle, ' ');
logLines = append_realtime_line(logLines, editHandle, '================ Continuity Summary ================');
summaryText = evalc('disp(summary)');
logLines = append_realtime_line(logLines, editHandle, summaryText);
fprintf('\n%s\n', summaryText);

if height(neighborTable) > 0
    suspicious = neighborTable(neighborTable.WhiteJumpFlag | neighborTable.RaJumpFlag | neighborTable.ArgmaxSwitchFlag, :);
    if height(suspicious) > 0
        suspicious = sortrows(suspicious, {'ArgmaxSwitchFlag','AbsDeltaWhiteRatio','AbsDeltaRa'}, {'descend','descend','descend'});
        showCount = min(reportTopN, height(suspicious));
        logLines = append_realtime_line(logLines, editHandle, sprintf('Top %u suspicious neighbor pairs:', uint32(showCount)));
        suspiciousText = evalc('disp(suspicious(1:showCount, :))');
        logLines = append_realtime_line(logLines, editHandle, suspiciousText);
        fprintf('\nTop %u suspicious neighbor pairs:\n%s\n', uint32(showCount), suspiciousText);
    else
        logLines = append_realtime_line(logLines, editHandle, 'No suspicious neighbor pair under current thresholds.');
        fprintf('\nNo suspicious neighbor pair under current thresholds.\n');
    end
end

%% ================= Plot =================
if plotEnable
    try
        figure('Name', 'ExactRa dense map');
        imagesc(ExactRaGrid);
        axis image;
        colorbar;
        title('ExactRa dense map');
        xlabel('Col');
        ylabel('Row');
        saveas(gcf, outputFigRa);

        figure('Name', 'BestWhiteRatio dense map');
        imagesc(WhiteRatioGrid);
        axis image;
        colorbar;
        title('BestWhiteRatio dense map');
        xlabel('Col');
        ylabel('Row');
        saveas(gcf, outputFigWhite);
    catch ME
        warning('Plot failed: %s', ME.message);
    end
end

%% ================= Export log =================
fid = fopen(outputLogTxt, 'w');
if fid >= 0
    fprintf(fid, '%s\n', char(strjoin(logLines, newline)));
    fclose(fid);
end

logLines = append_realtime_line(logLines, editHandle, ' ');
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported dense CSV    : %s', outputDenseCsv));
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported neighbor CSV : %s', outputNeighborCsv));
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported summary CSV  : %s', outputSummaryCsv));
logLines = append_realtime_line(logLines, editHandle, sprintf('Exported log TXT      : %s', outputLogTxt));
if plotEnable
    logLines = append_realtime_line(logLines, editHandle, sprintf('Exported ExactRa PNG  : %s', outputFigRa));
    logLines = append_realtime_line(logLines, editHandle, sprintf('Exported White PNG    : %s', outputFigWhite));
end
logLines = append_realtime_line(logLines, editHandle, 'Done.');

if ~isempty(dialogHandle) && ishghandle(dialogHandle)
    set(dialogHandle, 'Name', 'S13 Continuity Simulation - Done');
end

fprintf('\nExported dense CSV    : %s\n', outputDenseCsv);
fprintf('Exported neighbor CSV : %s\n', outputNeighborCsv);
fprintf('Exported summary CSV  : %s\n', outputSummaryCsv);
fprintf('Exported log TXT      : %s\n', outputLogTxt);
if plotEnable
    fprintf('Exported ExactRa PNG  : %s\n', outputFigRa);
    fprintf('Exported White PNG    : %s\n', outputFigWhite);
end
fprintf('Done.\n');

%% ================= Continuity helper functions =================
function T = build_neighbor_continuity_table(RaGrid, WhiteGrid, XGrid, YGrid, UGrid, VGrid, ValidGrid, scanPlane)
    pairData = [];
    [rowCount, colCount] = size(RaGrid);

    for r = 1:rowCount
        for c = 1:colCount
            if ~ValidGrid(r,c)
                continue;
            end

            if c < colCount && ValidGrid(r,c+1)
                pairData = append_pair(pairData, r, c, r, c+1, RaGrid, WhiteGrid, XGrid, YGrid, UGrid, VGrid, scanPlane); %#ok<AGROW>
            end

            if r < rowCount && ValidGrid(r+1,c)
                pairData = append_pair(pairData, r, c, r+1, c, RaGrid, WhiteGrid, XGrid, YGrid, UGrid, VGrid, scanPlane); %#ok<AGROW>
            end
        end
    end

    if isempty(pairData)
        T = table();
        return;
    end

    T = array2table(pairData, 'VariableNames', ...
        {'Row1','Col1','Row2','Col2','Distance','Ra1','Ra2','DeltaRa','AbsDeltaRa', ...
         'White1','White2','DeltaWhiteRatio','AbsDeltaWhiteRatio', ...
         'x1','y1','u1','v1','x2','y2','u2','v2'});

    T.RaSlope = T.AbsDeltaRa ./ max(T.Distance, eps);
    T.WhiteSlope = T.AbsDeltaWhiteRatio ./ max(T.Distance, eps);
    T.RaJumpFlag = false(height(T), 1);
    T.WhiteJumpFlag = false(height(T), 1);
    T.ArgmaxSwitchFlag = false(height(T), 1);
end

function pairData = append_pair(pairData, r1, c1, r2, c2, RaGrid, WhiteGrid, XGrid, YGrid, UGrid, VGrid, scanPlane)
    if strcmpi(scanPlane, 'xy')
        d = hypot(XGrid(r1,c1) - XGrid(r2,c2), YGrid(r1,c1) - YGrid(r2,c2));
    else
        d = hypot(UGrid(r1,c1) - UGrid(r2,c2), VGrid(r1,c1) - VGrid(r2,c2));
    end

    ra1 = RaGrid(r1,c1);
    ra2 = RaGrid(r2,c2);
    w1 = WhiteGrid(r1,c1);
    w2 = WhiteGrid(r2,c2);

    row = [r1, c1, r2, c2, d, ...
           ra1, ra2, ra2 - ra1, abs(ra2 - ra1), ...
           w1, w2, w2 - w1, abs(w2 - w1), ...
           XGrid(r1,c1), YGrid(r1,c1), UGrid(r1,c1), VGrid(r1,c1), ...
           XGrid(r2,c2), YGrid(r2,c2), UGrid(r2,c2), VGrid(r2,c2)];

    pairData = [pairData; row];
end

function [summary, neighborTable] = summarize_continuity(neighborTable, validCount, invalidCount, pointCountPerAxis, ...
    raJumpThreshold, whiteJumpThreshold, nearTieRaThreshold)

    if isempty(neighborTable) || height(neighborTable) == 0
        summary = table(validCount, invalidCount, pointCountPerAxis, 'VariableNames', {'ValidPointCount','InvalidPointCount','PointCountPerAxis'});
        return;
    end

    neighborTable.RaJumpFlag = neighborTable.AbsDeltaRa > raJumpThreshold;
    neighborTable.WhiteJumpFlag = neighborTable.AbsDeltaWhiteRatio > whiteJumpThreshold;
    neighborTable.ArgmaxSwitchFlag = (neighborTable.AbsDeltaWhiteRatio > whiteJumpThreshold) & (neighborTable.AbsDeltaRa < nearTieRaThreshold);

    maxAbsDeltaRa = max(neighborTable.AbsDeltaRa);
    p95AbsDeltaRa = local_percentile(neighborTable.AbsDeltaRa, 95);
    maxAbsDeltaWhiteRatio = max(neighborTable.AbsDeltaWhiteRatio);
    p95AbsDeltaWhiteRatio = local_percentile(neighborTable.AbsDeltaWhiteRatio, 95);
    maxRaSlope = max(neighborTable.RaSlope);
    p95RaSlope = local_percentile(neighborTable.RaSlope, 95);
    maxWhiteSlope = max(neighborTable.WhiteSlope);
    p95WhiteSlope = local_percentile(neighborTable.WhiteSlope, 95);

    raJumpPairCount = sum(neighborTable.RaJumpFlag);
    whiteJumpPairCount = sum(neighborTable.WhiteJumpFlag);
    argmaxSwitchPairCount = sum(neighborTable.ArgmaxSwitchFlag);

    if raJumpPairCount == 0
        exactRaContinuityStatus = "PASS_UNDER_THRESHOLD";
    else
        exactRaContinuityStatus = "SUSPICIOUS_RA_JUMPS";
    end

    if argmaxSwitchPairCount == 0
        whiteRatioArgmaxStatus = "NO_ARGMAX_SWITCH_FLAG";
    else
        whiteRatioArgmaxStatus = "POSSIBLE_ARGMAX_SWITCH_OR_UNDERSAMPLING";
    end

    summary = table(validCount, invalidCount, pointCountPerAxis, height(neighborTable), ...
        maxAbsDeltaRa, p95AbsDeltaRa, maxRaSlope, p95RaSlope, raJumpPairCount, ...
        maxAbsDeltaWhiteRatio, p95AbsDeltaWhiteRatio, maxWhiteSlope, p95WhiteSlope, whiteJumpPairCount, ...
        argmaxSwitchPairCount, exactRaContinuityStatus, whiteRatioArgmaxStatus, ...
        'VariableNames', {'ValidPointCount','InvalidPointCount','PointCountPerAxis','NeighborPairCount', ...
        'MaxAbsDeltaRa','P95AbsDeltaRa','MaxRaSlope','P95RaSlope','RaJumpPairCount', ...
        'MaxAbsDeltaWhiteRatio','P95AbsDeltaWhiteRatio','MaxWhiteSlope','P95WhiteSlope','WhiteJumpPairCount', ...
        'ArgmaxSwitchPairCount','ExactRaContinuityStatus','WhiteRatioArgmaxStatus'});
end

function p = local_percentile(x, q)
    x = x(isfinite(x));
    if isempty(x)
        p = NaN;
        return;
    end
    x = sort(x(:));
    pos = 1 + (numel(x) - 1) * q / 100;
    lo = floor(pos);
    hi = ceil(pos);
    if lo == hi
        p = x(lo);
    else
        p = x(lo) + (x(hi) - x(lo)) * (pos - lo);
    end
end

function inside = is_point_inside_triangle_xy(p, tri)
    p = p(:).';
    a = tri(1,:);
    b = tri(2,:);
    c = tri(3,:);
    v0 = c - a;
    v1 = b - a;
    v2 = p - a;

    dot00 = dot(v0, v0);
    dot01 = dot(v0, v1);
    dot02 = dot(v0, v2);
    dot11 = dot(v1, v1);
    dot12 = dot(v1, v2);

    invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    tol = 1e-12;
    inside = (u >= -tol) && (v >= -tol) && (u + v <= 1 + tol);
end

function [dialogHandle, editHandle] = create_continuity_result_window(enableWindow)
    dialogHandle = [];
    editHandle = [];

    if ~enableWindow
        return;
    end

    try
        dialogHandle = dialog('Name', 'S13 Continuity Simulation - Running', ...
                              'Position', [80, 60, 1280, 760], ...
                              'WindowStyle', 'normal');

        uicontrol('Parent', dialogHandle, ...
                  'Style', 'text', ...
                  'String', 'Dense exact-Ra continuity simulation. Output is appended during calculation.', ...
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
        fprintf('Realtime window open failed: %s\n', ME.message);
    end
end

function logLines = append_realtime_line(logLines, editHandle, newLine)
    parts = splitlines(string(newLine));
    for i = 1:numel(parts)
        logLines(end + 1, 1) = parts(i); %#ok<AGROW>
    end

    if ~isempty(editHandle) && ishghandle(editHandle)
        set(editHandle, 'String', char(strjoin(logLines, newline)));
        drawnow;
    end
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
