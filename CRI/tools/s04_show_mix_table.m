% EXPORT_RGB_MIX_POINT_RATIO_ZH_LAYERED_MERGED
% 中文说明：
% 将 RGB混色点、白/RGB比例、权重、Ra 按“每个颜色三层”显示。
%
% 显示规则：
%   1. 每个颜色占 3 行：
%      第1行：精确法
%      第2行：网格法
%      第3行：纯RGB
%   2. 同一个颜色的“序号、目标xyY”只在第一行显示；
%      第二、三行留空，视觉上等效于合并单元格。
%   3. 每个颜色的三行使用同一种底色，相邻颜色底色交替，减少眼花。
%   4. 只输出一个表格界面，不再弹额外结果框。
%
% 使用顺序：
%   batch_compare_targets_grid_cn
%   calc_pure_rgb_ra_only_cn_v2
%   export_rgb_mix_point_ratio_zh_layered_merged
%
% 输出：
%   triangle_grid_lut_output/rgb_mix_point_ratio_layered_merged.csv

clear;
clc;

%% Step 0: locate files
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');

batchFile = fullfile(outputDir, 'batch_compare_targets_grid_results.csv');
pureFile  = fullfile(outputDir, 'pure_rgb_ra_only_results.csv');
outFile   = fullfile(outputDir, 'rgb_mix_point_ratio_layered_merged.csv');

if ~exist(batchFile, 'file')
    error('没有找到文件：%s\n请先运行 batch_compare_targets_grid_cn。', batchFile);
end

if ~exist(pureFile, 'file')
    error('没有找到文件：%s\n请先运行 calc_pure_rgb_ra_only_cn_v2。', pureFile);
end

batchData = readtable(batchFile);
pureData = readtable(pureFile);

%% Step 1: allocate display data
colorCount = height(batchData);
rowCount = colorCount * 3;

displayData = cell(rowCount, 12);

csvColorIndex = strings(rowCount, 1);
csvTargetXyY = strings(rowCount, 1);
csvLayer = strings(rowCount, 1);
csvMethod = strings(rowCount, 1);
csvRgbPoint = strings(rowCount, 1);
csvRatio = strings(rowCount, 1);
csvWeight = strings(rowCount, 1);
csvRa = nan(rowCount, 1);
csvRgbPointDiff = nan(rowCount, 1);
csvWhiteRatioDiff = nan(rowCount, 1);
csvRgbRatioDiff = nan(rowCount, 1);
csvRaDiff = nan(rowCount, 1);

%% Step 2: fill three rows for each color
for colorIdx = 1:colorCount
    baseRow = (colorIdx - 1) * 3;
    targetText = sprintf('(%.4f, %.4f, %.4f)', ...
        batchData.Target_x(colorIdx), batchData.Target_y(colorIdx), batchData.Target_Y(colorIdx));

    pureRow = find(pureData.Index == batchData.Index(colorIdx), 1, 'first');

    if isempty(pureRow)
        pureRa = NaN;
        pureRWeight = NaN;
        pureGWeight = NaN;
        pureBWeight = NaN;
    else
        pureRa = pureData.PureRGB_Ra(pureRow);
        pureRWeight = pureData.PureRGB_R_weight(pureRow);
        pureGWeight = pureData.PureRGB_G_weight(pureRow);
        pureBWeight = pureData.PureRGB_B_weight(pureRow);
    end

    % ---------- row 1: exact ----------
    rowIdx = baseRow + 1;

    methodText = '1-精确法';
    rgbPointText = sprintf('(%.5f, %.5f)', ...
        batchData.Exact_RGBPoint_x(colorIdx), batchData.Exact_RGBPoint_y(colorIdx));
    ratioText = sprintf('W=%.4f, RGB=%.4f', ...
        batchData.Exact_SingleWhiteRatio(colorIdx), batchData.Exact_RGBWhiteRatio(colorIdx));
    weightText = sprintf('R=%.4f, G=%.4f, B=%.4f', ...
        batchData.Exact_R_weight(colorIdx), batchData.Exact_G_weight(colorIdx), batchData.Exact_B_weight(colorIdx));

    displayData(rowIdx, :) = { ...
        batchData.Index(colorIdx), targetText, methodText, rgbPointText, ratioText, weightText, ...
        batchData.Exact_Ra(colorIdx), 0, 0, 0, 0, '基准'};

    csvColorIndex(rowIdx) = string(batchData.Index(colorIdx));
    csvTargetXyY(rowIdx) = targetText;
    csvLayer(rowIdx) = "1";
    csvMethod(rowIdx) = "精确法";
    csvRgbPoint(rowIdx) = rgbPointText;
    csvRatio(rowIdx) = ratioText;
    csvWeight(rowIdx) = weightText;
    csvRa(rowIdx) = batchData.Exact_Ra(colorIdx);
    csvRgbPointDiff(rowIdx) = 0;
    csvWhiteRatioDiff(rowIdx) = 0;
    csvRgbRatioDiff(rowIdx) = 0;
    csvRaDiff(rowIdx) = 0;

    % ---------- row 2: grid ----------
    rowIdx = baseRow + 2;

    methodText = '2-网格法';
    rgbPointText = sprintf('(%.5f, %.5f)', ...
        batchData.Grid_ProjectedRgb_x(colorIdx), batchData.Grid_ProjectedRgb_y(colorIdx));
    ratioText = sprintf('W=%.4f, RGB=%.4f', ...
        batchData.Grid_SingleWhiteRatio(colorIdx), batchData.Grid_RGBWhiteRatio(colorIdx));
    weightText = sprintf('R=%.4f, G=%.4f, B=%.4f', ...
        batchData.Grid_R_weight(colorIdx), batchData.Grid_G_weight(colorIdx), batchData.Grid_B_weight(colorIdx));

    rgbPointDiff = hypot( ...
        batchData.Grid_ProjectedRgb_x(colorIdx) - batchData.Exact_RGBPoint_x(colorIdx), ...
        batchData.Grid_ProjectedRgb_y(colorIdx) - batchData.Exact_RGBPoint_y(colorIdx));
    whiteRatioDiff = batchData.Grid_SingleWhiteRatio(colorIdx) - batchData.Exact_SingleWhiteRatio(colorIdx);
    rgbRatioDiff = batchData.Grid_RGBWhiteRatio(colorIdx) - batchData.Exact_RGBWhiteRatio(colorIdx);
    raDiff = batchData.Grid_Ra(colorIdx) - batchData.Exact_Ra(colorIdx);

    displayData(rowIdx, :) = { ...
        '', '', methodText, rgbPointText, ratioText, weightText, ...
        batchData.Grid_Ra(colorIdx), rgbPointDiff, whiteRatioDiff, rgbRatioDiff, raDiff, '对比精确法'};

    csvColorIndex(rowIdx) = "";
    csvTargetXyY(rowIdx) = "";
    csvLayer(rowIdx) = "2";
    csvMethod(rowIdx) = "网格法";
    csvRgbPoint(rowIdx) = rgbPointText;
    csvRatio(rowIdx) = ratioText;
    csvWeight(rowIdx) = weightText;
    csvRa(rowIdx) = batchData.Grid_Ra(colorIdx);
    csvRgbPointDiff(rowIdx) = rgbPointDiff;
    csvWhiteRatioDiff(rowIdx) = whiteRatioDiff;
    csvRgbRatioDiff(rowIdx) = rgbRatioDiff;
    csvRaDiff(rowIdx) = raDiff;

    % ---------- row 3: pure RGB ----------
    rowIdx = baseRow + 3;

    methodText = '3-纯RGB';
    rgbPointText = sprintf('(%.5f, %.5f)', ...
        batchData.Target_x(colorIdx), batchData.Target_y(colorIdx));
    ratioText = 'W=0.0000, RGB=1.0000';
    weightText = sprintf('R=%.4f, G=%.4f, B=%.4f', ...
        pureRWeight, pureGWeight, pureBWeight);

    rgbPointDiff = hypot( ...
        batchData.Target_x(colorIdx) - batchData.Exact_RGBPoint_x(colorIdx), ...
        batchData.Target_y(colorIdx) - batchData.Exact_RGBPoint_y(colorIdx));
    whiteRatioDiff = 0 - batchData.Exact_SingleWhiteRatio(colorIdx);
    rgbRatioDiff = 1 - batchData.Exact_RGBWhiteRatio(colorIdx);
    raDiff = pureRa - batchData.Exact_Ra(colorIdx);

    displayData(rowIdx, :) = { ...
        '', '', methodText, rgbPointText, ratioText, weightText, ...
        pureRa, rgbPointDiff, whiteRatioDiff, rgbRatioDiff, raDiff, '对比精确法'};

    csvColorIndex(rowIdx) = "";
    csvTargetXyY(rowIdx) = "";
    csvLayer(rowIdx) = "3";
    csvMethod(rowIdx) = "纯RGB";
    csvRgbPoint(rowIdx) = rgbPointText;
    csvRatio(rowIdx) = ratioText;
    csvWeight(rowIdx) = weightText;
    csvRa(rowIdx) = pureRa;
    csvRgbPointDiff(rowIdx) = rgbPointDiff;
    csvWhiteRatioDiff(rowIdx) = whiteRatioDiff;
    csvRgbRatioDiff(rowIdx) = rgbRatioDiff;
    csvRaDiff(rowIdx) = raDiff;
end

columnNames = { ...
    '颜色序号', ...
    '目标xyY', ...
    '方法', ...
    'RGB混色点', ...
    '白色_RGB比例', ...
    'RGB权重', ...
    'Ra', ...
    'RGB点距精确点_xy', ...
    '白色比例差', ...
    'RGB比例差', ...
    'Ra差', ...
    '说明'};

%% Step 3: export CSV
outputTable = table( ...
    csvColorIndex, csvTargetXyY, csvLayer, csvMethod, csvRgbPoint, csvRatio, csvWeight, ...
    csvRa, csvRgbPointDiff, csvWhiteRatioDiff, csvRgbRatioDiff, csvRaDiff, ...
    'VariableNames', { ...
        '颜色序号', '目标xyY', '层级', '方法', 'RGB混色点', '白色_RGB比例', 'RGB权重', ...
        'Ra', 'RGB点距精确点_xy', '白色比例差_相对精确', 'RGB比例差_相对精确', 'Ra差_相对精确'});

try
    writetable(outputTable, outFile);
catch
    outFile = fullfile(outputDir, ...
        ['rgb_mix_point_ratio_layered_merged_' datestr(now, 'yyyymmdd_HHMMSS') '.csv']);
    writetable(outputTable, outFile);
end

fprintf('\n================ RGB混色点与比例：合并显示三层表 ================\n');
disp(outputTable);
fprintf('\n合并显示三层表已导出：\n%s\n', outFile);

%% Step 4: show UI with color groups
show_grouped_table(displayData, columnNames, colorCount, outFile);

drawnow;

%% ================= Local functions =================
function projectRoot = find_project_root(startDir, inputFolderName)
    projectRoot = startDir;

    while true
        if exist(fullfile(projectRoot, inputFolderName), 'dir')
            return;
        end

        parentDir = fileparts(projectRoot);

        if strcmp(parentDir, projectRoot)
            error('Cannot find %s from script folder or parent folders.', inputFolderName);
        end

        projectRoot = parentDir;
    end
end

function show_grouped_table(displayData, columnNames, colorCount, outFile)
    % First try modern uifigure style, because it supports row background colors.
    try
        fig = uifigure('Name','RGB混色点与比例：合并显示三层表', ...
            'Position',[50,60,1780,820], ...
            'Color',[0.96,0.96,0.96]);

        uilabel(fig, ...
            'Text','每个颜色三层显示：第1层精确法 / 第2层网格法 / 第3层纯RGB；序号和目标xyY只显示一次', ...
            'Position',[20,780,1680,28], ...
            'FontSize',13, ...
            'FontWeight','bold');

        uit = uitable(fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'Position',[20,70,1740,700], ...
            'FontSize',11, ...
            'ColumnWidth',{70,145,80,130,150,200,80,150,120,120,95,100});

        % Group colors: every color has 3 rows with same background.
        colorA = [0.96, 0.985, 1.00];
        colorB = [1.00, 0.975, 0.93];

        for colorIdx = 1:colorCount
            rows = ((colorIdx - 1) * 3 + 1):(colorIdx * 3);

            if mod(colorIdx, 2) == 1
                bg = colorA;
            else
                bg = colorB;
            end

            style = uistyle('BackgroundColor', bg);
            addStyle(uit, style, 'row', rows);
        end

        % Highlight exact/grid/pure method cells lightly.
        exactStyle = uistyle('FontWeight','bold','FontColor',[0.0,0.25,0.65]);
        gridStyle = uistyle('FontWeight','bold','FontColor',[0.0,0.45,0.1]);
        pureStyle = uistyle('FontWeight','bold','FontColor',[0.65,0.25,0.0]);

        addStyle(uit, exactStyle, 'cell', [(1:3:size(displayData,1)).', repmat(3, colorCount, 1)]);
        addStyle(uit, gridStyle,  'cell', [(2:3:size(displayData,1)).', repmat(3, colorCount, 1)]);
        addStyle(uit, pureStyle,  'cell', [(3:3:size(displayData,1)).', repmat(3, colorCount, 1)]);

        uilabel(fig, ...
            'Text',['CSV输出路径：' outFile], ...
            'Position',[20,25,1680,28], ...
            'FontSize',10);

        return;
    catch
        % Fall through to classic figure.
    end

    % Fallback for older MATLAB: no per-row color style, but still keeps merged visual by blank cells.
    try
        fig = figure('Name','RGB混色点与比例：合并显示三层表', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.96,0.96,0.96], ...
            'Position',[40,60,1780,780]);

        uicontrol('Parent',fig, ...
            'Style','text', ...
            'String','每个颜色三层显示：第1层精确法 / 第2层网格法 / 第3层纯RGB；序号和目标xyY只显示一次', ...
            'Units','normalized', ...
            'Position',[0.01,0.94,0.98,0.045], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'HorizontalAlignment','left', ...
            'FontSize',12, ...
            'FontWeight','bold');

        uitable('Parent',fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'RowName',[], ...
            'Units','normalized', ...
            'Position',[0.01,0.08,0.98,0.85], ...
            'FontSize',10, ...
            'BackgroundColor',[0.96,0.985,1.00; 1.00,0.975,0.93], ...
            'ColumnWidth',{70,145,80,130,150,200,80,150,120,120,95,100});

        uicontrol('Parent',fig, ...
            'Style','text', ...
            'String',['CSV输出路径：' outFile], ...
            'Units','normalized', ...
            'Position',[0.01,0.01,0.98,0.05], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'HorizontalAlignment','left', ...
            'FontSize',9);
    catch ME
        fprintf('\n表格界面打开失败：%s\n', ME.message);
    end
end
