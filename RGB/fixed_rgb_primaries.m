function [xyY_red, xyY_green, xyY_blue, triangleXY, edgeNames, primaries] = fixed_rgb_primaries(primaries)
%FIXED_RGB_PRIMARIES Return a 3-primary xyY triangle.
% primaries format:
%   [x_r y_r Y_r;
%    x_g y_g Y_g;
%    x_b y_b Y_b]

    if nargin < 1 || isempty(primaries)
        primaries = [ ...
            0.7040 0.2970 1.5474; ...
            0.1568 0.7411 3.5194; ...
            0.1532 0.0310 0.5766];
    end

    primaries = double(primaries);
    if ~isequal(size(primaries), [3 3])
        error('fixed_rgb_primaries:InvalidPrimaries', ...
            'primaries must be a 3x3 matrix: [x y Y] for R, G, and B.');
    end

    xyY_red = primaries(1, :);
    xyY_green = primaries(2, :);
    xyY_blue = primaries(3, :);

    triangleXY = primaries(:, 1:2);
    edgeNames = {'R-G'; 'G-B'; 'B-R'};
end
