function cfg = guangpu_case_config()
%GUANGPU_CASE_CONFIG Editable product configuration for LUT generation.
%
% This file is the single source that s01_make_lut_xy_grid_final.m reads.
% When switching to another product, update channel_xyY / single_white_xyY
% here, then rerun:
%   1. s01_make_lut_xy_grid_final.m
%   2. s101.m

cfg = struct();

cfg.inputDir = fileparts(mfilename('fullpath'));
cfg.singleWhiteFile = 'W.CSV';

% Common wavelength axis used by the CRI scripts.
cfg.wavelength_nm = (380:780).';
cfg.dLambda = 1;

% Target color used by scripts that need a default target.
% Only the Y component is consumed by s01_make_lut_xy_grid_final.m.
cfg.target_xyY = [0.3169, 0.3310, 4.0000];

% -------------------------------------------------------------------------
% Active product set
% -------------------------------------------------------------------------
% 1
% Current BLINKY runtime RGBW channel parameters, T25 / MCU temp 21.4 C.
cfg.channel_xyY.R = [0.7025, 0.2967, 2.9000];
cfg.channel_xyY.G = [0.1370, 0.7069, 4.7060];
cfg.channel_xyY.B = [0.1521, 0.0284, 0.8733];
cfg.channel_xyY.W = [0.3333, 0.3491, 5.5580];
cfg.single_white_xyY = [0.3333, 0.3491, 5.5580];

% -------------------------------------------------------------------------
% Alternate product set reference
% -------------------------------------------------------------------------
% 2
% cfg.channel_xyY.R = [0.7030, 0.2965, 2.8030];
% cfg.channel_xyY.G = [0.1472, 0.7211, 5.1110];
% cfg.channel_xyY.B = [0.1519, 0.0297, 0.8438];
% cfg.channel_xyY.W = [0.3360, 0.3473, 5.3070];
% cfg.single_white_xyY = [0.3360, 0.3473, 5.3070];

end
