% S08_VERIFY_SINGLE_COLOR_EXACT_VS_MIN_ANGLE
% Single-color verification for:
%   1. Exact method
%   2. Minimum-angle method
%
% Added angle report logic:
%   - configurable Delta uv threshold
%   - count how many LUT points fall inside the radius
%   - print every candidate angle

clear;
clc;

%% ================= User Config =================
targetUvDiv5 = [39, 110];      % firmware-style input, q1000 divided by 5
targetYlm = NaN;              % NaN -> use guangpu_case_config().target_xyY(3)
rayScanCount = 1001;          % exact method scan count

minAngleDeltaUvLimit = 0.01;  % min-angle method search radius
reportDeltaUvLimit = 0.01;    % angle report radius

%% ================= Basic Validation =================
if ~isnumeric(targetUvDiv5) || numel(targetUvDiv5) ~= 2
    error('targetUvDiv5 must be a numeric 1x2 vector.');
end
if ~isfinite(minAngleDeltaUvLimit) || minAngleDeltaUvLimit <= 0
    error('minAngleDeltaUvLimit must be a positive scalar.');
end
if ~isfinite(reportDeltaUvLimit) || reportDeltaUvLimit <= 0
    error('reportDeltaUvLimit must be a positive scalar.');
end

targetUvDiv5 = round(double(targetUvDiv5(:).'));
targetUvQ1000 = targetUvDiv5 .* 5;
targetUv = targetUvQ1000 ./ 1000;

minAngleDeltaUvLimitQ1000 = round(minAngleDeltaUvLimit * 1000);
reportDeltaUvLimitQ1000 = round(reportDeltaUvLimit * 1000);

fprintf('\n============================================================\n');
fprintf('S08 Single-Color Verify: Exact vs Min-Angle\n');
fprintf('============================================================\n');
fprintf('Target uv q1000/5   : (%d, %d)\n', targetUvDiv5(1), targetUvDiv5(2));
fprintf('Target uv q1000     : (%d, %d)\n', targetUvQ1000(1), targetUvQ1000(2));
fprintf('Target uv decimal   : (%.4f, %.4f)\n', targetUv(1), targetUv(2));
fprintf('Min-angle radius    : %.4f (q1000 = %d)\n', ...
    minAngleDeltaUvLimit, minAngleDeltaUvLimitQ1000);
fprintf('Angle report radius : %.4f (q1000 = %d)\n', ...
    reportDeltaUvLimit, reportDeltaUvLimitQ1000);
fprintf('============================================================\n\n');

%% ================= Locate Project Paths =================
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
inputDir = fullfile(projectRoot, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
standardDir = fullfile(projectRoot, 'standard_data');

lutFile = fullfile(outputDir, 'guangpu_triangle_grid_lut_results.csv');
if ~exist(lutFile, 'file')
    error('LUT file not found: %s', lutFile);
end

%% ================= Load Config and Spectra =================
addpath(inputDir);
caseCfg = guangpu_case_config();

wl = caseCfg.wavelength_nm;
dLambda = caseCfg.dLambda;
channel_xyY = caseCfg.channel_xyY;
single_white_xyY = caseCfg.single_white_xyY;

if isnan(targetYlm)
    targetYlm = caseCfg.target_xyY(3);
end

targetXY = uv1976_to_xy(targetUv);
if any(~isfinite(targetXY))
    error('Target uv cannot be converted to xy.');
end
target_xyY = [targetXY, targetYlm];

std = load_standard_data(standardDir, wl);

raw.R = read_spd(fullfile(inputDir, 'R.CSV'), '', wl);
raw.G = read_spd(fullfile(inputDir, 'G.CSV'), '', wl);
raw.B = read_spd(fullfile(inputDir, 'B.CSV'), '', wl);
raw.SingleWhite = read_spd(fullfile(inputDir, 'W.CSV'), '', wl);

spd.R = scale_to_photopic_Y(raw.R, channel_xyY.R(3), std, dLambda);
spd.G = scale_to_photopic_Y(raw.G, channel_xyY.G(3), std, dLambda);
spd.B = scale_to_photopic_Y(raw.B, channel_xyY.B(3), std, dLambda);
spd.SingleWhite = scale_to_photopic_Y(raw.SingleWhite, single_white_xyY(3), std, dLambda);

actual_xyY.R = spectrum_xyY(spd.R, std, dLambda);
actual_xyY.G = spectrum_xyY(spd.G, std, dLambda);
actual_xyY.B = spectrum_xyY(spd.B, std, dLambda);

rgbTriangleXY = [actual_xyY.R(1:2); actual_xyY.G(1:2); actual_xyY.B(1:2)];
rgbSolveMatrix = [xyY_to_XYZ(actual_xyY.R).', ...
                  xyY_to_XYZ(actual_xyY.G).', ...
                  xyY_to_XYZ(actual_xyY.B).'];

singleNorm = scale_to_photopic_Y(spd.SingleWhite, targetYlm, std, dLambda);
singleMetrics = spectrum_metrics(singleNorm, std, wl, dLambda);
singleWhiteXY = singleMetrics.xyY(1:2);
singleWhiteUv = xy_to_uv1976(singleWhiteXY);
singleWhiteUvQ1000 = round(singleWhiteUv .* 1000);

fprintf('Target xyY         : (%.6f, %.6f, %.4f)\n', ...
    target_xyY(1), target_xyY(2), target_xyY(3));
fprintf('Single white xy    : (%.6f, %.6f)\n', singleWhiteXY(1), singleWhiteXY(2));
fprintf('Single white uv    : (%.6f, %.6f)\n', singleWhiteUv(1), singleWhiteUv(2));
fprintf('Single white q1000 : (%d, %d)\n\n', ...
    singleWhiteUvQ1000(1), singleWhiteUvQ1000(2));

%% ================= Load LUT =================
lut = readtable(lutFile, 'VariableNamingRule', 'preserve');

requiredCols = {'GridI', 'GridJ', 'GridK', ...
    'Target_x', 'Target_y', 'Target_u1976', 'Target_v1976', ...
    'Best_Ra', 'Best_SingleWhiteRatio', 'Best_RGBPoint_x', 'Best_RGBPoint_y'};
for iCol = 1:numel(requiredCols)
    if ~ismember(requiredCols{iCol}, lut.Properties.VariableNames)
        error('LUT is missing required column: %s', requiredCols{iCol});
    end
end

%% ================= Exact Method =================
exactResult = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, ...
    rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, rayScanCount);

%% ================= Min-Angle Method =================
[minAngleResult, minAngleInfo, minAngleCandidateTable] = find_min_angle_same_ratio_result( ...
    target_xyY, targetUvQ1000, singleWhiteXY, singleWhiteUvQ1000, ...
    rgbTriangleXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, ...
    lut, minAngleDeltaUvLimitQ1000);

%% ================= Angle Report =================
angleReportTable = build_angle_report_table( ...
    targetUvQ1000, singleWhiteUvQ1000, lut, reportDeltaUvLimitQ1000);

%% ================= Print Summary =================
fprintf('---------------- Exact ----------------\n');
if exactResult.Valid
    fprintf('Exact Ra            : %.6f\n', exactResult.Ra);
    fprintf('Exact WhiteRatio    : %.6f\n', exactResult.SingleWhiteRatio);
    fprintf('Exact RGBRatio      : %.6f\n', exactResult.RGBWhiteRatio);
    fprintf('Exact RayT          : %.6f\n', exactResult.RayT);
    fprintf('Exact RGB point xy  : (%.6f, %.6f)\n', ...
        exactResult.RGBPoint_xyY(1), exactResult.RGBPoint_xyY(2));
else
    fprintf('Exact result        : invalid\n');
end
fprintf('\n');

fprintf('-------------- Min-Angle --------------\n');
if minAngleResult.Valid
    fprintf('Selection mode      : %s\n', char(minAngleInfo.SelectionMode));
    fprintf('Chosen LUT index0   : %d\n', minAngleInfo.LutIndex0);
    fprintf('Chosen LUT q1000    : (%d, %d)\n', ...
        minAngleInfo.PointUQ1000, minAngleInfo.PointVQ1000);
    fprintf('Chosen LUT q1000/5  : (%d, %d)\n', ...
        round(minAngleInfo.PointUQ1000 / 5), round(minAngleInfo.PointVQ1000 / 5));
    fprintf('Chosen Delta uv     : %.6f\n', minAngleInfo.DeltaUv);
    fprintf('Chosen Angle deg    : %.6f\n', minAngleInfo.AngleDeg);
    fprintf('MinAngle Ra         : %.6f\n', minAngleResult.Ra);
    fprintf('MinAngle WhiteRatio : %.6f\n', minAngleResult.SingleWhiteRatio);
    fprintf('MinAngle RGBRatio   : %.6f\n', minAngleResult.RGBWhiteRatio);
    fprintf('MinAngle RayT       : %.6f\n', minAngleResult.RayT);
else
    fprintf('Min-angle result    : invalid\n');
end
fprintf('\n');

fprintf('--------- Min-Angle Radius Candidates (Delta uv < %.4f) ---------\n', ...
    minAngleDeltaUvLimit);
fprintf('Candidate count     : %d\n', height(minAngleCandidateTable));
if ~isempty(minAngleCandidateTable)
    disp(minAngleCandidateTable);
end
fprintf('\n');

fprintf('----------- Angle Report (Delta uv < %.4f) -----------\n', reportDeltaUvLimit);
fprintf('Point count         : %d\n', height(angleReportTable));
if ~isempty(angleReportTable)
    disp(angleReportTable);
end
fprintf('\nDone.\n');

%% ================= Local Functions =================
function [result, info, candidateTable] = find_min_angle_same_ratio_result( ...
    target_xyY, targetUvQ1000, singleWhiteXY, singleWhiteUvQ1000, ...
    rgbTriangleXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda, ...
    lut, deltaUvLimitQ1000)

    targetXY = target_xyY(1:2);
    targetU = targetUvQ1000(1);
    targetV = targetUvQ1000(2);
    whiteU = singleWhiteUvQ1000(1);
    whiteV = singleWhiteUvQ1000(2);
    limit2 = double(deltaUvLimitQ1000) * double(deltaUvLimitQ1000);

    validMask = true(height(lut), 1);
    if ismember('Valid', lut.Properties.VariableNames)
        validMask = isfinite(lut.Valid) & (lut.Valid ~= 0);
    end

    awU = double(targetU) - double(whiteU);
    awV = double(targetV) - double(whiteV);
    awLen2 = (awU * awU) + (awV * awV);

    nearestFound = false;
    radiusNearestFound = false;
    bestFound = false;

    nearestIdx = NaN;
    radiusNearestIdx = NaN;
    bestIdx = NaN;

    nearestDistance2 = inf;
    radiusNearestDistance2 = inf;
    bestDistance2 = inf;
    bestBwLen2 = 0;
    bestDot = 0;

    for lutIdx1 = 1:height(lut)
        if ~validMask(lutIdx1) || ~isfinite(lut.Best_SingleWhiteRatio(lutIdx1))
            continue;
        end

        pointU = round(lut.Target_u1976(lutIdx1) * 1000);
        pointV = round(lut.Target_v1976(lutIdx1) * 1000);
        distance2 = calc_distance2_q1000(targetU, targetV, pointU, pointV);

        if (~nearestFound) || (distance2 < nearestDistance2)
            nearestFound = true;
            nearestDistance2 = distance2;
            nearestIdx = lutIdx1;
        end

        if distance2 == 0
            bestIdx = lutIdx1;
            bestDistance2 = 0;
            bestBwLen2 = 0;
            bestDot = 0;
            bestFound = true;
            break;
        end

        if distance2 >= limit2
            continue;
        end

        if (~radiusNearestFound) || (distance2 < radiusNearestDistance2)
            radiusNearestFound = true;
            radiusNearestDistance2 = distance2;
            radiusNearestIdx = lutIdx1;
        end

        bwU = double(pointU) - double(whiteU);
        bwV = double(pointV) - double(whiteV);
        bwLen2 = (bwU * bwU) + (bwV * bwV);
        if (awLen2 == 0) || (bwLen2 == 0)
            continue;
        end

        dotValue = (awU * bwU) + (awV * bwV);
        if (~bestFound) || is_candidate_better(dotValue, bwLen2, distance2, bestDot, bestBwLen2, bestDistance2)
            bestFound = true;
            bestIdx = lutIdx1;
            bestDot = dotValue;
            bestBwLen2 = bwLen2;
            bestDistance2 = distance2;
        end
    end

    if bestFound
        chosenIdx = bestIdx;
        selectionMode = "min-angle-in-radius";
    elseif radiusNearestFound
        chosenIdx = radiusNearestIdx;
        selectionMode = "nearest-in-radius";
    elseif nearestFound
        chosenIdx = nearestIdx;
        selectionMode = "nearest-overall";
    else
        chosenIdx = NaN;
        selectionMode = "not-found";
    end

    candidateTable = build_angle_report_table(targetUvQ1000, singleWhiteUvQ1000, lut, deltaUvLimitQ1000);
    result = empty_best_result();
    info = make_min_angle_info();
    info.SelectionMode = selectionMode;

    if ~isfinite(chosenIdx)
        return;
    end

    row = lut(chosenIdx, :);
    storedWhiteRatio = row.Best_SingleWhiteRatio;

    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    maxWhiteRatio = white_ratio_from_rayt(target_xyY, singleWhiteXY, boundaryRayT);
    whiteRatio = min(max(storedWhiteRatio, 0), maxWhiteRatio);

    rayT = rayt_from_white_ratio(target_xyY, singleWhiteXY, whiteRatio);
    rayT = min(max(rayT, 1), boundaryRayT);
    rgbPointXY = singleWhiteXY + rayT .* (targetXY - singleWhiteXY);

    result = calculate_result_from_rgb_point(target_xyY, rgbPointXY, rayT, ...
        singleWhiteXY, rgbSolveMatrix, spd, singleNorm, std, wl, dLambda);

    pointU = round(row.Target_u1976 * 1000);
    pointV = round(row.Target_v1976 * 1000);
    [dotValue, bwLen2, angleDeg] = calc_angle_metrics(targetUvQ1000, singleWhiteUvQ1000, [pointU, pointV]);

    info.LutIndex0 = chosenIdx - 1;
    info.GridI = row.GridI;
    info.GridJ = row.GridJ;
    info.GridK = row.GridK;
    info.PointUQ1000 = pointU;
    info.PointVQ1000 = pointV;
    info.PointU = row.Target_u1976;
    info.PointV = row.Target_v1976;
    info.DeltaUv = sqrt(calc_distance2_q1000(targetU, targetV, pointU, pointV)) / 1000;
    info.AngleDeg = angleDeg;
    info.Dot = dotValue;
    info.BwLen2 = bwLen2;
    info.StoredWhiteRatio = storedWhiteRatio;
    info.StoredBestRa = row.Best_Ra;

    if result.Valid
        result.StoredBestRgbXY = [row.Best_RGBPoint_x, row.Best_RGBPoint_y];
        result.SelectedGridTargetXY = [row.Target_x, row.Target_y];
        result.KUsed = 1;
    end
end

function info = make_min_angle_info()
    info.SelectionMode = "not-found";
    info.LutIndex0 = NaN;
    info.GridI = NaN;
    info.GridJ = NaN;
    info.GridK = NaN;
    info.PointUQ1000 = NaN;
    info.PointVQ1000 = NaN;
    info.PointU = NaN;
    info.PointV = NaN;
    info.DeltaUv = NaN;
    info.AngleDeg = NaN;
    info.Dot = NaN;
    info.BwLen2 = NaN;
    info.StoredWhiteRatio = NaN;
    info.StoredBestRa = NaN;
end

function tableOut = build_angle_report_table(targetUvQ1000, singleWhiteUvQ1000, lut, deltaUvLimitQ1000)
    limit2 = double(deltaUvLimitQ1000) * double(deltaUvLimitQ1000);
    validMask = true(height(lut), 1);
    if ismember('Valid', lut.Properties.VariableNames)
        validMask = isfinite(lut.Valid) & (lut.Valid ~= 0);
    end

    rows = struct( ...
        'LutIndex0', {}, 'GridI', {}, 'GridJ', {}, 'GridK', {}, ...
        'PointU_div5', {}, 'PointV_div5', {}, ...
        'PointU', {}, 'PointV', {}, 'DeltaUv', {}, ...
        'AngleDeg', {}, 'Dot', {}, 'BestRa', {}, 'BestWhiteRatio', {});

    writeIdx = 0;
    targetU = targetUvQ1000(1);
    targetV = targetUvQ1000(2);

    for lutIdx1 = 1:height(lut)
        if ~validMask(lutIdx1)
            continue;
        end

        pointU = round(lut.Target_u1976(lutIdx1) * 1000);
        pointV = round(lut.Target_v1976(lutIdx1) * 1000);
        distance2 = calc_distance2_q1000(targetU, targetV, pointU, pointV);
        if distance2 >= limit2
            continue;
        end

        [dotValue, ~, angleDeg] = calc_angle_metrics(targetUvQ1000, singleWhiteUvQ1000, [pointU, pointV]);

        writeIdx = writeIdx + 1;
        rows(writeIdx).LutIndex0 = lutIdx1 - 1;
        rows(writeIdx).GridI = lut.GridI(lutIdx1);
        rows(writeIdx).GridJ = lut.GridJ(lutIdx1);
        rows(writeIdx).GridK = lut.GridK(lutIdx1);
        rows(writeIdx).PointU_div5 = round(pointU / 5);
        rows(writeIdx).PointV_div5 = round(pointV / 5);
        rows(writeIdx).PointU = lut.Target_u1976(lutIdx1);
        rows(writeIdx).PointV = lut.Target_v1976(lutIdx1);
        rows(writeIdx).DeltaUv = sqrt(distance2) / 1000;
        rows(writeIdx).AngleDeg = angleDeg;
        rows(writeIdx).Dot = dotValue;
        rows(writeIdx).BestRa = lut.Best_Ra(lutIdx1);
        rows(writeIdx).BestWhiteRatio = lut.Best_SingleWhiteRatio(lutIdx1);
    end

    if isempty(rows)
        tableOut = table();
        return;
    end

    tableOut = struct2table(rows);
    tableOut = sortrows(tableOut, {'AngleDeg', 'DeltaUv', 'LutIndex0'}, {'ascend', 'ascend', 'ascend'});
end

function distance2 = calc_distance2_q1000(u1, v1, u2, v2)
    du = double(u1) - double(u2);
    dv = double(v1) - double(v2);
    distance2 = (du * du) + (dv * dv);
end

function [dotValue, bwLen2, angleDeg] = calc_angle_metrics(targetUvQ1000, singleWhiteUvQ1000, pointUvQ1000)
    aw = double(targetUvQ1000) - double(singleWhiteUvQ1000);
    bw = double(pointUvQ1000) - double(singleWhiteUvQ1000);
    awLen2 = sum(aw .^ 2);
    bwLen2 = sum(bw .^ 2);
    dotValue = sum(aw .* bw);

    if (awLen2 <= 0) || (bwLen2 <= 0)
        angleDeg = NaN;
        return;
    end

    cosValue = dotValue / sqrt(awLen2 * bwLen2);
    cosValue = min(max(cosValue, -1), 1);
    angleDeg = acosd(cosValue);
end

function signValue = get_dot_sign(dotValue)
    if dotValue > 0
        signValue = 2;
    elseif dotValue == 0
        signValue = 1;
    else
        signValue = 0;
    end
end

function tf = is_candidate_better(candidateDot, candidateLen2, candidateDistance2, ...
        bestDot, bestLen2, bestDistance2)
    candidateSign = get_dot_sign(candidateDot);
    bestSign = get_dot_sign(bestDot);

    if candidateSign ~= bestSign
        tf = candidateSign > bestSign;
        return;
    end

    if candidateSign == 1
        if candidateDistance2 ~= bestDistance2
            tf = candidateDistance2 < bestDistance2;
        else
            tf = candidateLen2 < bestLen2;
        end
        return;
    end

    lhs = (abs(candidateDot)^2) * bestLen2;
    rhs = (abs(bestDot)^2) * candidateLen2;

    if candidateSign == 2
        if lhs ~= rhs
            tf = lhs > rhs;
            return;
        end
    else
        if lhs ~= rhs
            tf = lhs < rhs;
            return;
        end
    end

    if candidateDistance2 ~= bestDistance2
        tf = candidateDistance2 < bestDistance2;
    else
        tf = candidateLen2 < bestLen2;
    end
end

function best = find_best_ra_for_target(target_xyY, singleWhiteXY, rgbTriangleXY, rgbSolveMatrix, ...
    spd, singleNorm, std, wl, dLambda, rayScanCount)

    targetXY = target_xyY(1:2);
    [~, boundaryRayT, ~, ~] = find_ray_gamut_intersection(singleWhiteXY, targetXY, rgbTriangleXY);
    rayTList = linspace(1, boundaryRayT, rayScanCount).';

    best = empty_best_result();

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
    s.StoredBestRgbXY = [NaN, NaN];
    s.SelectedGridTargetXY = [NaN, NaN];
    s.KUsed = NaN;
end

function xyY = spectrum_xyY(spd, std, dLambda)
    XYZ = source_XYZ(spd, std.cmf, dLambda);
    xy = XYZ_to_xyY(XYZ);
    xyY = [xy(1), xy(2), 683 * XYZ(2)];
end

function [pointXY, rayT, edgeName, edgeIndex] = find_ray_gamut_intersection(whiteXY, targetXY, triangleXY)
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    triangleXY = double(triangleXY(:, 1:2));
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_ray_gamut_intersection:SamePoint', ...
            'The fixed single white and target have the same xy.');
    end

    tol = 1e-12;
    rayT = Inf;
    edgeIndex = NaN;
    pointXY = [NaN NaN];

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

function pathOut = ensure_file(folder, fileName, url)
    pathOut = fullfile(folder, fileName);
    if ~exist(pathOut, 'file')
        websave(pathOut, url);
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
        dist2(i) = sum((uv - bbuv).^2);
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
    d2 = sum((uv - bbuv).^2);
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
