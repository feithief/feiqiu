function config = default_white_mix_config(intersectionTriangle, verifyPrimaries)
%DEFAULT_WHITE_MIX_CONFIG Build the configurable triangles used by white mixing.
% intersectionTriangle:
%   triangle used only for geometric ray/edge intersection.
%   It can be 3x2 [x y] or 3x3 [x y Y]. Only x and y are used.
% verifyPrimaries:
%   triangle used for xyY -> PWM decomposition and PWM -> xyY validation.

    if nargin < 2 || isempty(verifyPrimaries)
        [~, ~, ~, ~, ~, verifyPrimaries] = fixed_rgb_primaries();
    end
    if nargin < 1 || isempty(intersectionTriangle)
        intersectionTriangle = verifyPrimaries(:, 1:2);
    end

    intersectionTriangleXY = normalize_intersection_triangle(intersectionTriangle);
    intersectionEdgeNames = {'R-G'; 'G-B'; 'B-R'};
    [~, ~, ~, verifyTriangleXY, verifyEdgeNames, verifyPrimaries] = ...
        fixed_rgb_primaries(verifyPrimaries);

    config = struct();
    config.intersectionPrimaries = intersectionTriangleXY;
    config.intersectionTriangleXY = intersectionTriangleXY;
    config.intersectionEdgeNames = intersectionEdgeNames;

    config.verifyPrimaries = verifyPrimaries;
    config.verifyTriangleXY = verifyTriangleXY;
    config.verifyEdgeNames = verifyEdgeNames;
end
