% S01_PLOT_TEMPERATURE_BESTWHITERATIO_5SURFACES
% Plot five temperature BestWhiteRatio surfaces in one interactive 3D figure.
%
% Data source:
%   projectRoot/triangle_grid_lut_output/s15_temperature_on_25grid/
%
% Meaning:
%   S15 computes the same fixed 25-degree grid under T25/T45/T60/T80/T100
%   RGBW spectra. This S01 script reads those five result tables and overlays
%   five BestWhiteRatio surfaces. Use the buttons to hide/show each temperature.

if exist('s01TemperatureSurfaceMetricOverride', 'var')
    s01MetricOverride = s01TemperatureSurfaceMetricOverride;
else
    s01MetricOverride = '';
end

clearvars -except s01MetricOverride;
clc;

%% User options
temperatureLabels = {'T25', 'T45', 'T60', 'T80', 'T100'};
temperatureC = [21.4, 43.0, 60.0, 80.0, 100.0];
if isempty(s01MetricOverride)
    zMetric = 'BestWhiteRatio';  % 'BestWhiteRatio' or 'BestRa'
else
    zMetric = s01MetricOverride;
end
surfaceGridN = 90;               % interpolation resolution
surfaceAlpha = 0.34;             % smaller means easier to see overlapped surfaces
showScatterPoints = true;        % true: clickable points with full data tip
scatterSize = 18;
forceRunS15 = false;             % true: regenerate five-temperature S15 CSV before plotting
autoRunS15IfMissing = true;      % run test/s15.m if S15 output is missing
enableDataCursorAtStart = false; % false: mouse drag rotates the 3D view by default

%% Locate project and S15 output
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
s15Dir = fullfile(outputDir, 's15_temperature_on_25grid');
fullCsv = fullfile(s15Dir, 's15_fixed25grid_temperature_exact_and_delta_full.csv');

if forceRunS15 || (~exist(fullCsv, 'file') && autoRunS15IfMissing)
    s15File = fullfile(projectRoot, 'test', 's15.m');
    if exist(s15File, 'file')
        if forceRunS15
            fprintf('forceRunS15=true. Running:\n  %s\n\n', s15File);
        else
            fprintf('S15 full temperature CSV not found. Running:\n  %s\n\n', s15File);
        end
        run(s15File);
    end
end

caseData = read_temperature_case_data(s15Dir, fullCsv, temperatureLabels, temperatureC);

switch lower(zMetric)
    case 'bestwhiteratio'
        zField = 'BestWhiteRatio';
        zLabel = 'BestWhiteRatio';
        zTitle = 'BestWhiteRatio';
    case 'bestra'
        zField = 'BestRa';
        zLabel = 'Best Ra';
        zTitle = 'Best Ra';
    otherwise
        error('Unsupported zMetric: %s. Use BestWhiteRatio or BestRa.', zMetric);
end

fprintf('\n============================================================\n');
fprintf('S01 five-temperature %s 3D surfaces\n', zTitle);
fprintf('============================================================\n\n');

fprintf('Loaded temperature cases:\n');
for k = 1:numel(caseData)
    fprintf('  %s: %d valid points\n', caseData(k).Label, height(caseData(k).Table));
end

%% Common xy query grid
allX = vertcat(caseData.TableX);
allY = vertcat(caseData.TableY);
xGrid = linspace(min(allX), max(allX), surfaceGridN);
yGrid = linspace(min(allY), max(allY), surfaceGridN);
[Xq, Yq] = meshgrid(xGrid, yGrid);

baseX = caseData(1).Table.Target_x;
baseY = caseData(1).Table.Target_y;
hullIdx = convhull(baseX, baseY);
insideMask = inpolygon(Xq, Yq, baseX(hullIdx), baseY(hullIdx));

%% Figure
fig = figure('Name', sprintf('S01 Five-Temperature %s Surfaces', zTitle), ...
    'Color', 'w', 'NumberTitle', 'off', 'Position', [80 80 1280 760]);
ax = axes(fig);
hold(ax, 'on');
grid(ax, 'on');
box(ax, 'on');
set(ax, 'Position', [0.075 0.145 0.78 0.78]);

colors = temperature_colors(numel(caseData));
surfaceHandles = gobjects(numel(caseData), 1);
scatterHandles = gobjects(numel(caseData), 1);
buttonHandles = gobjects(numel(caseData), 1);
legendHandles = gobjects(numel(caseData), 1);

for k = 1:numel(caseData)
    T = caseData(k).Table;
    z0 = T.(zField);
    if strcmpi(zField, 'BestWhiteRatio')
        z0 = min(max(z0, 0), 1);
    end

    Zq = griddata(T.Target_x, T.Target_y, z0, Xq, Yq, 'natural');
    Zq(~insideMask) = NaN;

    surfaceHandles(k) = surf(ax, Xq, Yq, Zq, ...
        'EdgeColor', 'none', ...
        'FaceAlpha', surfaceAlpha, ...
        'FaceColor', colors(k, :), ...
        'DisplayName', sprintf('%s %.1f C', caseData(k).Label, caseData(k).TemperatureC));
    set(surfaceHandles(k), 'HitTest', 'off', 'PickableParts', 'none');

    if showScatterPoints
        scatterHandles(k) = scatter3(ax, T.Target_x, T.Target_y, z0, scatterSize, ...
            repmat(colors(k, :), height(T), 1), 'filled', ...
            'MarkerFaceAlpha', 0.82, ...
            'MarkerEdgeColor', [0.12 0.12 0.12], ...
            'MarkerEdgeAlpha', 0.18, ...
            'DisplayName', sprintf('%s points', caseData(k).Label));
        configure_persistent_datatips(scatterHandles(k), T, caseData(k), zField, zLabel);
        uistack(scatterHandles(k), 'top');
    else
        scatterHandles(k) = gobjects(1);
    end

    legendHandles(k) = plot3(ax, NaN, NaN, NaN, '-', ...
        'Color', colors(k, :), 'LineWidth', 6, ...
        'DisplayName', sprintf('%s %.1f C', caseData(k).Label, caseData(k).TemperatureC));
end

xlabel(ax, 'Target x');
ylabel(ax, 'Target y');
zlabel(ax, zLabel);
title(ax, sprintf('S01 Grid: Five-Temperature %s Surfaces', zTitle));
view(ax, 42, 30);
colormap(ax, turbo_or_parula());
legend(ax, legendHandles, 'Location', 'northeastoutside');

dcm = datacursormode(fig);
set(dcm, 'Enable', ternary_on_off(enableDataCursorAtStart));
rotate3d(fig, ternary_on_off(~enableDataCursorAtStart));

%% Toggle buttons
buttonPanelTitle = uicontrol(fig, 'Style', 'text', ...
    'Units', 'pixels', ...
    'Position', [20 48 150 20], ...
    'String', 'Hide / show surfaces', ...
    'BackgroundColor', get(fig, 'Color'), ...
    'HorizontalAlignment', 'left', ...
    'FontWeight', 'bold');

buttonW = 82;
buttonH = 28;
buttonGap = 8;
buttonY = 18;
buttonX0 = 20;
toggleCallbackText = [ ...
    'ud=get(gcbo,''UserData'');' ...
    'if get(gcbo,''Value'')~=0,' ...
    'set(ud.Surface,''Visible'',''on'');' ...
    'if isgraphics(ud.Scatter), set(ud.Scatter,''Visible'',''on''); end;' ...
    'set(gcbo,''BackgroundColor'',ud.Color,''ForegroundColor'',ud.TextColor);' ...
    'else,' ...
    'set(ud.Surface,''Visible'',''off'');' ...
    'if isgraphics(ud.Scatter), set(ud.Scatter,''Visible'',''off''); end;' ...
    'set(gcbo,''BackgroundColor'',[0.86 0.86 0.86],''ForegroundColor'',[0.25 0.25 0.25]);' ...
    'end' ...
    ];
for k = 1:numel(caseData)
    buttonHandles(k) = uicontrol(fig, 'Style', 'togglebutton', ...
        'Units', 'pixels', ...
        'Position', [buttonX0 + (k - 1) * (buttonW + buttonGap), buttonY, buttonW, buttonH], ...
        'String', caseData(k).Label, ...
        'Value', 1, ...
        'BackgroundColor', colors(k, :), ...
        'ForegroundColor', readable_text_color(colors(k, :)), ...
        'FontWeight', 'bold', ...
        'UserData', struct('Surface', surfaceHandles(k), ...
                           'Scatter', scatterHandles(k), ...
                           'Color', colors(k, :), ...
                           'TextColor', readable_text_color(colors(k, :))), ...
        'Callback', toggleCallbackText);
end

%% Export
plotOutDir = fullfile(outputDir, 's01_temperature_surfaces');
if ~exist(plotOutDir, 'dir')
    mkdir(plotOutDir);
end

pngFile = fullfile(plotOutDir, sprintf('s01_temperature_%s_5surfaces.png', lower(zMetric)));
figFile = fullfile(plotOutDir, sprintf('s01_temperature_%s_5surfaces.fig', lower(zMetric)));

try
    exportgraphics(fig, pngFile, 'Resolution', 200);
catch
    saveas(fig, pngFile);
end
savefig(fig, figFile);

fprintf('\nExported:\n  %s\n  %s\n', pngFile, figFile);
fprintf('\nTip: click a temperature button to hide/show its surface and points.\n');
fprintf('Tip: click scatter points to inspect exact grid values.\n');

%% Local functions
function rootDir = find_project_root(startDir, markerName)
    rootDir = startDir;
    while true
        if exist(fullfile(rootDir, markerName), 'dir') || exist(fullfile(rootDir, markerName), 'file')
            return;
        end

        parentDir = fileparts(rootDir);
        if isempty(parentDir) || strcmp(parentDir, rootDir)
            error('Cannot find project root containing %s from %s', markerName, startDir);
        end
        rootDir = parentDir;
    end
end

function caseData = read_temperature_case_data(s15Dir, fullCsv, labels, temperatureC)
    caseData = struct('Label', {}, 'TemperatureC', {}, 'Table', {}, 'TableX', {}, 'TableY', {});

    if exist(fullCsv, 'file')
        fullTable = readtable(fullCsv, 'TextType', 'string');
        for i = 1:numel(labels)
            T = build_case_table_from_full(fullTable, labels{i});
            caseData(i) = make_case_data(labels{i}, temperatureC(i), T);
        end
        return;
    end

    if ~exist(s15Dir, 'dir')
        error('S15 output folder not found: %s\nPlease run s15 first.', s15Dir);
    end

    for i = 1:numel(labels)
        csvFile = fullfile(s15Dir, sprintf('s15_fixed25grid_exact_%s.csv', labels{i}));
        if ~exist(csvFile, 'file')
            error('Missing S15 temperature CSV: %s\nPlease run s15 first.', csvFile);
        end
        rawT = readtable(csvFile, 'TextType', 'string');
        T = build_case_table_from_exact(rawT);
        caseData(i) = make_case_data(labels{i}, temperatureC(i), T);
    end
end

function item = make_case_data(label, tempC, T)
    validMask = T.Valid ~= 0 & isfinite(T.Target_x) & isfinite(T.Target_y);
    validMask = validMask & isfinite(T.BestWhiteRatio) & isfinite(T.BestRa);
    T = T(validMask, :);
    if isempty(T)
        error('%s has no valid rows.', label);
    end
    item.Label = label;
    item.TemperatureC = tempC;
    item.Table = T;
    item.TableX = T.Target_x;
    item.TableY = T.Target_y;
end

function T = build_case_table_from_full(F, label)
    requiredBase = {'GridI','GridJ','GridK','x','y','Y_lm','u1976','v1976'};
    assert_required_variables(F, requiredBase);

    prefix = [label '_'];
    requiredCase = strcat(prefix, {'Valid','BestRa','BestRmin8','BestWhiteRatio','BestRGBRatio', ...
        'BestRayT','BestRGBPoint_x','BestRGBPoint_y','R_weight','G_weight','B_weight','BoundaryEdge'});
    assert_required_variables(F, requiredCase);

    T = table();
    T.GridI = F.GridI;
    T.GridJ = F.GridJ;
    T.GridK = F.GridK;
    T.Target_x = F.x;
    T.Target_y = F.y;
    T.Target_Y_lm = F.Y_lm;
    T.Target_u1976 = F.u1976;
    T.Target_v1976 = F.v1976;
    T.Valid = F.([prefix 'Valid']);
    T.BestRa = F.([prefix 'BestRa']);
    T.BestRmin8 = F.([prefix 'BestRmin8']);
    T.BestWhiteRatio = F.([prefix 'BestWhiteRatio']);
    T.BestRGBRatio = F.([prefix 'BestRGBRatio']);
    T.BestRayT = F.([prefix 'BestRayT']);
    T.BestRGBPoint_x = F.([prefix 'BestRGBPoint_x']);
    T.BestRGBPoint_y = F.([prefix 'BestRGBPoint_y']);
    T.R_weight = F.([prefix 'R_weight']);
    T.G_weight = F.([prefix 'G_weight']);
    T.B_weight = F.([prefix 'B_weight']);
    T.BoundaryEdge = F.([prefix 'BoundaryEdge']);

    bestUv = xy_to_uv1976_array(T.BestRGBPoint_x, T.BestRGBPoint_y);
    T.BestRGBPoint_u1976 = bestUv(:, 1);
    T.BestRGBPoint_v1976 = bestUv(:, 2);
end

function T = build_case_table_from_exact(E)
    required = {'GridI','GridJ','GridK','Target_x','Target_y','Target_Y_lm','Target_u1976','Target_v1976', ...
        'Valid','Best_Ra','Best_Rmin_8','Best_SingleWhiteRatio','Best_RGBWhiteRatio','Best_RayT', ...
        'Best_RGBPoint_x','Best_RGBPoint_y','Best_RGBPoint_u1976','Best_RGBPoint_v1976', ...
        'Best_R_weight','Best_G_weight','Best_B_weight','BoundaryEdge'};
    assert_required_variables(E, required);

    T = table();
    T.GridI = E.GridI;
    T.GridJ = E.GridJ;
    T.GridK = E.GridK;
    T.Target_x = E.Target_x;
    T.Target_y = E.Target_y;
    T.Target_Y_lm = E.Target_Y_lm;
    T.Target_u1976 = E.Target_u1976;
    T.Target_v1976 = E.Target_v1976;
    T.Valid = E.Valid;
    T.BestRa = E.Best_Ra;
    T.BestRmin8 = E.Best_Rmin_8;
    T.BestWhiteRatio = E.Best_SingleWhiteRatio;
    T.BestRGBRatio = E.Best_RGBWhiteRatio;
    T.BestRayT = E.Best_RayT;
    T.BestRGBPoint_x = E.Best_RGBPoint_x;
    T.BestRGBPoint_y = E.Best_RGBPoint_y;
    T.BestRGBPoint_u1976 = E.Best_RGBPoint_u1976;
    T.BestRGBPoint_v1976 = E.Best_RGBPoint_v1976;
    T.R_weight = E.Best_R_weight;
    T.G_weight = E.Best_G_weight;
    T.B_weight = E.Best_B_weight;
    T.BoundaryEdge = E.BoundaryEdge;
end

function assert_required_variables(T, names)
    missing = setdiff(names, T.Properties.VariableNames);
    if ~isempty(missing)
        error('CSV missing required columns: %s', strjoin(missing, ', '));
    end
end

function uv = xy_to_uv1976_array(x, y)
    den = -2 .* x + 12 .* y + 3;
    uv = NaN(numel(x), 2);
    valid = abs(den) > 1e-12;
    uv(valid, 1) = 4 .* x(valid) ./ den(valid);
    uv(valid, 2) = 9 .* y(valid) ./ den(valid);
end

function colors = temperature_colors(n)
    base = [ ...
        0.067 0.333 0.800; ...
        0.000 0.600 0.700; ...
        0.120 0.620 0.260; ...
        0.950 0.560 0.110; ...
        0.800 0.120 0.090];
    if n <= size(base, 1)
        colors = base(1:n, :);
    else
        colors = lines(n);
    end
end

function c = readable_text_color(bg)
    luminance = 0.299 * bg(1) + 0.587 * bg(2) + 0.114 * bg(3);
    if luminance > 0.55
        c = [0 0 0];
    else
        c = [1 1 1];
    end
end

function s = ternary_on_off(tf)
    if tf
        s = 'on';
    else
        s = 'off';
    end
end

function cmap = turbo_or_parula()
    try
        cmap = turbo(256);
    catch
        cmap = parula(256);
    end
end

function configure_persistent_datatips(scatterObj, T, caseInfo, zField, zLabel)
    n = height(T);
    scatterObj.UserData = struct( ...
        'TemperatureLabel', char(caseInfo.Label), ...
        'TemperatureC', caseInfo.TemperatureC, ...
        'GridI', T.GridI, ...
        'GridJ', T.GridJ, ...
        'GridK', T.GridK, ...
        'Target_x', T.Target_x, ...
        'Target_y', T.Target_y, ...
        'Target_u1976', T.Target_u1976, ...
        'Target_v1976', T.Target_v1976, ...
        'BestWhiteRatio', T.BestWhiteRatio, ...
        'BestRGBRatio', T.BestRGBRatio, ...
        'BestRa', T.BestRa, ...
        'BestRmin8', T.BestRmin8, ...
        'BestRayT', T.BestRayT, ...
        'BestRGBPoint_x', T.BestRGBPoint_x, ...
        'BestRGBPoint_y', T.BestRGBPoint_y, ...
        'BestRGBPoint_u1976', T.BestRGBPoint_u1976, ...
        'BestRGBPoint_v1976', T.BestRGBPoint_v1976, ...
        'R_weight', T.R_weight, ...
        'G_weight', T.G_weight, ...
        'B_weight', T.B_weight);

    try
        scatterObj.DataTipTemplate.DataTipRows(1).Label = 'Target x';
        scatterObj.DataTipTemplate.DataTipRows(2).Label = 'Target y';
        scatterObj.DataTipTemplate.DataTipRows(3).Label = zLabel;
        scatterObj.DataTipTemplate.Interpreter = 'none';
    catch
        % Older MATLAB versions may not support DataTipTemplate.
    end

    add_datatip_row(scatterObj, 'Temp C', repmat(caseInfo.TemperatureC, n, 1));
    add_datatip_row(scatterObj, 'GridI', T.GridI);
    add_datatip_row(scatterObj, 'GridJ', T.GridJ);
    add_datatip_row(scatterObj, 'GridK', T.GridK);
    add_datatip_row(scatterObj, 'Target u''', T.Target_u1976);
    add_datatip_row(scatterObj, 'Target v''', T.Target_v1976);
    add_datatip_row(scatterObj, 'BestWhiteRatio', T.BestWhiteRatio);
    add_datatip_row(scatterObj, 'BestRGBRatio', T.BestRGBRatio);
    add_datatip_row(scatterObj, 'Best Ra', T.BestRa);
    add_datatip_row(scatterObj, 'Best Rmin8', T.BestRmin8);
    add_datatip_row(scatterObj, 'RayT', T.BestRayT);
    add_datatip_row(scatterObj, 'Best RGB x', T.BestRGBPoint_x);
    add_datatip_row(scatterObj, 'Best RGB y', T.BestRGBPoint_y);
    add_datatip_row(scatterObj, 'Best RGB u''', T.BestRGBPoint_u1976);
    add_datatip_row(scatterObj, 'Best RGB v''', T.BestRGBPoint_v1976);
    add_datatip_row(scatterObj, 'R weight', T.R_weight);
    add_datatip_row(scatterObj, 'G weight', T.G_weight);
    add_datatip_row(scatterObj, 'B weight', T.B_weight);
    add_datatip_row(scatterObj, 'Boundary', T.BoundaryEdge);

    if ~strcmpi(zField, 'BestWhiteRatio')
        add_datatip_row(scatterObj, 'Z metric', T.(zField));
    end
end

function add_datatip_row(scatterObj, label, values)
    try
        if isnumeric(values) || islogical(values)
            scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow(label, values);
        elseif isstring(values) || iscellstr(values) || ischar(values) || iscategorical(values)
            scatterObj.DataTipTemplate.DataTipRows(end + 1) = dataTipTextRow(label, cellstr(string(values)));
        end
    catch
        % Keep the remaining numeric rows persistent even if one text row is not supported.
    end
end
