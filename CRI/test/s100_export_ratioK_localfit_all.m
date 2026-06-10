% S100_EXPORT_RATIOK_LOCALFIT_ALL
% One-click export entry for localfit C files.
%
% Recommended workflow after changing spectra / chromaticity grid:
%   1. Run s01_make_lut_xy_grid_final.m
%   2. Run this script
%   3. Rebuild firmware project
%
% This script exports both localfit implementations:
%   - runtime-grid + uint16 ratio LUT
%   - packed32 compatibility LUT
%
% The actual firmware implementation selected at compile time is controlled by:
%   ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE
%   ALGO_RATIO_K_FAST_NEAREST_ENABLE

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S100 localfit export: one-click export all C variants\n');
fprintf('Step 1: export runtime-grid ratio16 version\n');
fprintf('        (this also exports packed32 compatibility version)\n');
fprintf('Step 2: show current firmware macro selection\n');
fprintf('============================================================\n\n');

scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
repoRoot = fileparts(projectRoot);
headerFile = fullfile(repoRoot, 'RGB', 'BLINKY_SIMPLE_EXAMPLE_TLD4020', ...
    'Algorithm', 'AlgoRatioK_localfit', 'AlgoRatioK_localfit.h');

ratio16Script = fullfile(scriptDir, 's06_export_ratioK_localfit_ratio16.m');
if ~exist(ratio16Script, 'file')
    error('Missing script:\n%s', ratio16Script);
end
if ~exist(headerFile, 'file')
    error('Missing header file:\n%s', headerFile);
end

fprintf('[1/2] Export runtime-grid ratio16 variant...\n');
run(ratio16Script);

fprintf('\n[2/2] Read current compile-time macro selection...\n');
scriptDir = fileparts(mfilename('fullpath'));
projectRoot = find_project_root(scriptDir, 'guangpu_case_input');
repoRoot = fileparts(projectRoot);
headerFile = fullfile(repoRoot, 'RGB', 'BLINKY_SIMPLE_EXAMPLE_TLD4020', ...
    'Algorithm', 'AlgoRatioK_localfit', 'AlgoRatioK_localfit.h');
show_current_macro_selection(headerFile);

fprintf('\nDone.\n');
fprintf('After updating the LUT/grid, you only need:\n');
fprintf('  s01_make_lut_xy_grid_final.m\n');
fprintf('  s100_export_ratioK_localfit_all.m\n');
fprintf('  Rebuild firmware project\n');

function show_current_macro_selection(headerFile)
    headerText = fileread(headerFile);
    runtimeGridEnable = parse_macro_value(headerText, 'ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE');
    fastNearestEnable = parse_macro_value(headerText, 'ALGO_RATIO_K_FAST_NEAREST_ENABLE');

    fprintf('Header: %s\n', headerFile);
    fprintf('ALGO_RATIO_K_RUNTIME_GRID_RATIO16_ENABLE = %d\n', runtimeGridEnable);
    fprintf('ALGO_RATIO_K_FAST_NEAREST_ENABLE        = %d\n', fastNearestEnable);

    if runtimeGridEnable ~= 0
        fprintf('Current firmware branch: runtime-grid + uint16 ratio LUT\n');
    else
        fprintf('Current firmware branch: packed32 compatibility LUT\n');
    end

    if fastNearestEnable ~= 0
        fprintf('Nearest-point search : FAST local neighborhood search\n');
    else
        fprintf('Nearest-point search : FULL SCAN over all LUT points\n');
    end
end

function value = parse_macro_value(headerText, macroName)
    pattern = ['#define\s+', regexptranslate('escape', macroName), '\s+\((\d+)u\)'];
    tokens = regexp(headerText, pattern, 'tokens', 'once');
    if isempty(tokens)
        error('Cannot parse macro %s from header.', macroName);
    end
    value = str2double(tokens{1});
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
