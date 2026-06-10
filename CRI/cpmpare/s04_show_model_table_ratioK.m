% S04_SHOW_MODEL_TABLE_RATIOK
% 中文说明：
% 一个颜色占 6 行：
%   第1行：精确法
%   第2行：投影1点
%   第3行：投影K点
%   第4行：同比例1点
%   第5行：同比例K点
%   第6行：纯RGB
%
% 序号和目标xyY只在第一行显示，后面5行留空，视觉上等效合并。
% 相邻目标颜色用不同底色区分。

clear;
clc;

%% 定位目录
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');
inputFile = fullfile(outputDir, 'model_compare_results.csv');
outputFile = fullfile(outputDir, 'model_compare_grouped_table_ratioK.csv');

if ~exist(inputFile, 'file')
    error('没有找到结果文件：%s\n请先运行 s02_run_model_compare_ratioK.m。', inputFile);
end

T = readtable(inputFile, 'VariableNamingRule', 'preserve');

methodNames = {'精确法','投影1点','投影K点','同比例1点','同比例K点','纯RGB'};
prefixNames = {'Exact','Proj1','ProjK','Ratio1','RatioK','PureRGB'};
methodCount = numel(methodNames);
targetCount = height(T);
rowCount = targetCount * methodCount;

displayData = cell(rowCount, 12);
csvData = cell(rowCount + 1, 12);

columnNames = {'序号','目标xyY','方法','Ra','RGB点','白色_RGB比例','RGB权重', ...
    'Ra差_精确减当前','说明','白色比例','RGB比例','该目标近似最优来源'};
csvData(1, :) = columnNames;

for targetIdx = 1:targetCount
    exactRa = T.Exact_Ra(targetIdx);
    baseRow = (targetIdx - 1) * methodCount;
    targetText = sprintf('(%.4f, %.4f, %.4f)', ...
        T.Target_x(targetIdx), T.Target_y(targetIdx), T.Target_Y_lm(targetIdx));

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
            noteText = '基准';
        else
            displayIndex = '';
            displayTarget = '';
            noteText = '对比精确法';
        end

        rgbPointText = sprintf('(%.5f, %.5f)', x, y);
        ratioText = sprintf('W=%.4f, RGB=%.4f', wRatio, rgbRatio);
        weightText = sprintf('R=%.4f, G=%.4f, B=%.4f', rWeight, gWeight, bWeight);
        raDiff = exactRa - ra;

        displayData(rowIdx, :) = {displayIndex, displayTarget, methodName, ra, ...
            rgbPointText, ratioText, weightText, raDiff, noteText, ...
            wRatio, rgbRatio, char(string(T.Approx_Max_Source(targetIdx)))};

        csvData(rowIdx + 1, :) = displayData(rowIdx, :);
    end
end

writecell(csvData, outputFile);

fprintf('\n================ 6模型分组表 ================\n');
safeColumnNames = matlab.lang.makeUniqueStrings(matlab.lang.makeValidName(columnNames));
disp(cell2table(displayData, 'VariableNames', safeColumnNames));
fprintf('\n分组表CSV已输出：\n%s\n', outputFile);

show_grouped_table(displayData, columnNames, targetCount, methodCount, outputFile);

%% ================= Local functions =================
function value = get_col(T, colName, rowIdx)
    if ismember(colName, T.Properties.VariableNames)
        value = T.(colName)(rowIdx);
    else
        value = NaN;
    end
end

function show_grouped_table(displayData, columnNames, targetCount, methodCount, outputFile)
    try
        fig = uifigure('Name','6模型分组表：每个颜色6行', ...
            'Position',[40,60,1850,820], ...
            'Color',[0.96,0.96,0.96]);

        uilabel(fig, ...
            'Text','每个颜色6行：精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB；序号和目标xyY只显示一次', ...
            'Position',[20,780,1750,28], ...
            'FontSize',13, ...
            'FontWeight','bold');

        uit = uitable(fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'Position',[20,70,1810,700], ...
            'FontSize',10, ...
            'ColumnWidth',{55,150,90,85,135,165,220,135,100,90,90,140});

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

        uilabel(fig, ...
            'Text',['CSV输出路径：' outputFile], ...
            'Position',[20,25,1750,28], ...
            'FontSize',10);
    catch
        fig = figure('Name','6模型分组表：每个颜色6行', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.96,0.96,0.96], ...
            'Position',[40,60,1850,780]);

        uicontrol('Parent',fig,'Style','text', ...
            'String','每个颜色6行：精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB；序号和目标xyY只显示一次', ...
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
            'ColumnWidth',{55,150,90,85,135,165,220,135,100,90,90,140});

        uicontrol('Parent',fig,'Style','text', ...
            'String',['CSV输出路径：' outputFile], ...
            'Units','normalized','Position',[0.015,0.01,0.97,0.05], ...
            'BackgroundColor',[0.96,0.96,0.96], ...
            'FontSize',9,'HorizontalAlignment','left');
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
