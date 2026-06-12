% S01_PLOT_TEMPERATURE_RA_5SURFACES
% Plot five temperature Best Ra surfaces in one interactive 3D figure.

s01TemperatureSurfaceMetricOverride = 'BestRa';
run(fullfile(fileparts(mfilename('fullpath')), 's01_plot_temperature_bestwhiteratio_5surfaces.m'));
clear s01TemperatureSurfaceMetricOverride;
