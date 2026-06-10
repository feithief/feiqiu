% S05_PLOT_MODEL_RA_LINE_RATIOK
% 中文说明：
% 画 6 种模型的 Ra 对比折线：
%   精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB
%
% 另外输出“每个颜色6行”的分组表：
%   第1行：精确法
%   第2行：投影1点
%   第3行：投影K点
%   第4行：同比例1点
%   第5行：同比例K点
%   第6行：纯RGB

clear;
clc;

%% 定位目录
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');

inputFile = fullfile(outputDir, 'model_compare_results.csv');
outputPng = fullfile(outputDir, 'model_ra_compare_ratioK.png');

if ~exist(inputFile, 'file')
    error('没有找到结果文件：%s\n请先运行 s02_run_model_compare_ratioK.m。', inputFile);
end

T = readtable(inputFile, 'VariableNamingRule', 'preserve');

idx = T.Index;
exactRa = T.Exact_Ra;
proj1Ra = T.Proj1_Ra;
projKRa = T.ProjK_Ra;
ratio1Ra = T.Ratio1_Ra;
ratioKRa = T.RatioK_Ra;
pureRgbRa = T.PureRGB_Ra;

%% 画图
fig = figure('Name', '6模型 Ra 折线对比', ...
             'NumberTitle', 'off', ...
             'Position', [80, 80, 1500, 720]);

plot(idx, exactRa, '-o', 'LineWidth', 1.8, 'MarkerSize', 5);
hold on;
plot(idx, proj1Ra, '-d', 'LineWidth', 1.5, 'MarkerSize', 5);
plot(idx, projKRa, '-^', 'LineWidth', 1.5, 'MarkerSize', 5);
plot(idx, ratio1Ra, '-h', 'LineWidth', 1.5, 'MarkerSize', 5);
plot(idx, ratioKRa, '->', 'LineWidth', 1.5, 'MarkerSize', 5);
plot(idx, pureRgbRa, '-s', 'LineWidth', 1.5, 'MarkerSize', 5);
hold off;

grid on;
xlabel('目标颜色序号');
ylabel('Ra');
title('6模型 Ra 折线对比：精确法 / 投影1点 / 投影K点 / 同比例1点 / 同比例K点 / 纯RGB');
legend({'精确法','投影1点','投影K点','同比例1点','同比例K点','纯RGB'}, 'Location', 'best');

xlim([min(idx), max(idx)]);
xticks(idx);

saveas(fig, outputPng);

fprintf('\n折线图已生成：\n%s\n', outputPng);

%% 构造每个颜色6行的分组表
methodNames = {'精确法','投影1点','投影K点','同比例1点','同比例K点','纯RGB'};
prefixNames = {'Exact','Proj1','ProjK','Ratio1','RatioK','PureRGB'};
methodCount = numel(methodNames);
targetCount = height(T);
rowCount = targetCount * methodCount;

displayData = cell(rowCount, 9);

for targetIdx = 1:targetCount
    exact = T.Exact_Ra(targetIdx);
    targetText = sprintf('(%.4f, %.4f, %.4f)', ...
        T.Target_x(targetIdx), T.Target_y(targetIdx), T.Target_Y_lm(targetIdx));
    baseRow = (targetIdx - 1) * methodCount;

    for methodIdx = 1:methodCount
        rowIdx = baseRow + methodIdx;
        prefix = prefixNames{methodIdx};

        if methodIdx == 1
            idxDisplay = T.Index(targetIdx);
            targetDisplay = targetText;
        else
            idxDisplay = '';
            targetDisplay = '';
        end

        ra = T.([prefix '_Ra'])(targetIdx);
        diff = exact - ra;
        wRatio = T.([prefix '_SingleWhiteRatio'])(targetIdx);
        rgbRatio = T.([prefix '_RGBWhiteRatio'])(targetIdx);
        x = T.([prefix '_RGBPoint_x'])(targetIdx);
        y = T.([prefix '_RGBPoint_y'])(targetIdx);

        displayData(rowIdx, :) = {idxDisplay, targetDisplay, methodNames{methodIdx}, ...
            ra, diff, sprintf('(%.5f, %.5f)', x, y), ...
            sprintf('W=%.4f, RGB=%.4f', wRatio, rgbRatio), ...
            T.Approx_Max_Source(targetIdx), T.Approx_Max_Ra(targetIdx)};
    end
end

columnNames = {'序号','目标xyY','方法','Ra','Ra差_精确减当前','RGB点','白色_RGB比例','该目标近似最优来源','近似最大Ra'};

fprintf('\n================ 6模型 Ra 分组数值表 ================\n');
safeColumnNames = matlab.lang.makeUniqueStrings(matlab.lang.makeValidName(columnNames));
disp(cell2table(displayData, 'VariableNames', safeColumnNames));

show_grouped_table(displayData, columnNames, targetCount, methodCount, outputPng);

%% ================= Local functions =================
function show_grouped_table(displayData, columnNames, targetCount, methodCount, outputPng)
    try
        fig = uifigure('Name','6模型Ra分组表：每个颜色6行', ...
            'Position',[60,70,1700,820], ...
            'Color',[0.96,0.96,0.96]);

        uilabel(fig, ...
            'Text','6模型Ra分组表：每个颜色6行，序号和目标xyY只显示一次', ...
            'Position',[20,780,1600,28], ...
            'FontSize',13, ...
            'FontWeight','bold');

        uit = uitable(fig, ...
            'Data',displayData, ...
            'ColumnName',columnNames, ...
            'Position',[20,70,1660,700], ...
            'FontSize',10, ...
            'ColumnWidth',{55,150,90,90,135,135,160,150,110});

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
            'Text',['图片输出路径：' outputPng], ...
            'Position',[20,25,1600,28], ...
            'FontSize',10);
    catch
        fig = figure('Name','6模型Ra分组表：每个颜色6行', ...
            'NumberTitle','off', ...
            'MenuBar','none', ...
            'ToolBar','none', ...
            'Color',[0.96,0.96,0.96], ...
            'Position',[60,70,1700,780]);

        uicontrol('Parent',fig,'Style','text', ...
            'String','6模型Ra分组表：每个颜色6行，序号和目标xyY只显示一次', ...
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
            'ColumnWidth',{55,150,90,90,135,135,160,150,110});

        uicontrol('Parent',fig,'Style','text', ...
            'String',['图片输出路径：' outputPng], ...
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
