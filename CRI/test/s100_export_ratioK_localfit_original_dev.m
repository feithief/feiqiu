% S100_EXPORT_RATIOK_LOCALFIT_ORIGINAL_DEV
% One-click export entry for the original packed32 localfit C file used by
% BLINKY_SIMPLE_EXAMPLE_TLD4020_DEV.
%
% Recommended workflow after changing spectra / chromaticity grid:
%   1. Run s01_make_lut_xy_grid_final.m
%   2. Run this script
%   3. Rebuild BLINKY_SIMPLE_EXAMPLE_TLD4020_DEV

clear;
clc;

fprintf('\n============================================================\n');
fprintf('S100 DEV original localfit export: one-click export C file\n');
fprintf('Step 1: export original packed32 localfit version\n');
fprintf('Step 2: rebuild BLINKY_SIMPLE_EXAMPLE_TLD4020_DEV\n');
fprintf('============================================================\n\n');

scriptDir = fileparts(mfilename('fullpath'));
exportScript = fullfile(scriptDir, 's06_export_ratioK_localfit_original_dev.m');
if ~exist(exportScript, 'file')
    error('Missing script:\n%s', exportScript);
end

fprintf('[1/1] Export original localfit packed32 variant...\n');
run(exportScript);

fprintf('\nDone.\n');
fprintf('After updating the LUT/grid, you only need:\n');
fprintf('  s01_make_lut_xy_grid_final.m\n');
fprintf('  s100_export_ratioK_localfit_original_dev.m\n');
fprintf('  Rebuild firmware project: BLINKY_SIMPLE_EXAMPLE_TLD4020_DEV\n');
