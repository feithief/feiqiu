% EXPORT_RA_COMPARE_ONLY_CN
% 中文说明：
% 只输出 Ra 前后对比表，不改变前面任何计算逻辑。
%
% 使用前提：
% 先运行 batch_compare_targets_grid_cn.m，生成：
% triangle_grid_lut_output/batch_compare_targets_grid_results.csv
%
% 本脚本只读取该结果文件，并导出只包含 Ra 对比的表格。

clear;
clc;

%% Step 0: locate project folders
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
outputDir = fullfile(projectRoot, 'triangle_grid_lut_output');

inputFile = fullfile(outputDir, 'batch_compare_targets_grid_results.csv');
outputFile = fullfile(outputDir, 'ra_compare_only.csv');

if ~exist(inputFile, 'file')
    error(['没有找到批量结果文件：\n%s\n\n' ...
           '请先运行 batch_compare_targets_grid_cn.m。'], inputFile);
end

%% Step 1: read batch result
T = readtable(inputFile);

%% Step 2: export only Ra compare columns
RaCompare = table( ...
    T.Index, ...
    T.Target_x, ...
    T.Target_y, ...
    T.Target_Y, ...
    T.Exact_Ra, ...
    T.Grid_Ra, ...
    T.Ra_Diff, ...
    T.Abs_Ra_Diff, ...
    'VariableNames', { ...
        '序号', ...
        '目标_x', ...
        '目标_y', ...
        '目标_Y', ...
        '精确法_Ra', ...
        '网格法_Ra', ...
        'Ra差值_精确减网格', ...
        'Ra绝对差值'});

writetable(RaCompare, outputFile);

%% Step 3: print table
fprintf('\n================ Ra 前后对比表 ================\n');
disp(RaCompare);
fprintf('\nRa对比表已导出：\n%s\n', outputFile);

try
    f = figure('Name', 'Ra前后对比表', 'NumberTitle', 'off', ...
               'Position', [200, 200, 980, 520]);
    uit = uitable(f, 'Data', table2cell(RaCompare), ...
        'ColumnName', RaCompare.Properties.VariableNames, ...
        'Units', 'normalized', ...
        'Position', [0, 0, 1, 1]);
    uit.FontSize = 10;
catch
    % 如果当前 MATLAB 环境不支持 uitable，只保留命令行输出。
end

%% ================= Local functions =================
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
