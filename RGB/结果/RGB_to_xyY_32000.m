function xyY = RGB_to_xyY_32000(varargin)
%RGB_TO_XYY_32000 Convert 0~32000 RGB/PWM counts back to xyY.
% Supported calls:
%   xyY = RGB_to_xyY_32000([R G B])
%   xyY = RGB_to_xyY_32000([R G B], primaries)
%   xyY = RGB_to_xyY_32000(R, G, B)
%   xyY = RGB_to_xyY_32000(R, G, B, primaries)
%
% primaries format:
%   [x_r y_r Y_r;
%    x_g y_g Y_g;
%    x_b y_b Y_b]

    primaries = [];

    switch nargin
        case 1
            RGB = normalize_rgb_matrix(varargin{1});
        case 2
            RGB = normalize_rgb_matrix(varargin{1});
            primaries = varargin{2};
        case 3
            RGB = normalize_rgb_channels(varargin{1}, varargin{2}, varargin{3});
        case 4
            RGB = normalize_rgb_channels(varargin{1}, varargin{2}, varargin{3});
            primaries = varargin{4};
        otherwise
            error('RGB_to_xyY_32000:InvalidInput', ...
                'Use RGB_to_xyY_32000([R G B]), RGB_to_xyY_32000([R G B], primaries), RGB_to_xyY_32000(R, G, B), or RGB_to_xyY_32000(R, G, B, primaries).');
    end

    [xyY_red, xyY_green, xyY_blue] = fixed_rgb_primaries(primaries);

    x_red = xyY_red(1);
    y_red = xyY_red(2);
    Y_red = xyY_red(3);

    x_green = xyY_green(1);
    y_green = xyY_green(2);
    Y_green = xyY_green(3);

    x_blue = xyY_blue(1);
    y_blue = xyY_blue(2);
    Y_blue = xyY_blue(3);

    R = RGB(:, 1);
    G = RGB(:, 2);
    B = RGB(:, 3);

    denominator = (Y_red / y_red) * R ...
                + (Y_green / y_green) * G ...
                + (Y_blue / y_blue) * B;

    x_numerator = (Y_red / y_red) * x_red * R ...
                + (Y_green / y_green) * x_green * G ...
                + (Y_blue / y_blue) * x_blue * B;

    luminance_numerator = Y_red * R + Y_green * G + Y_blue * B;

    x_target = x_numerator ./ denominator;
    y_target = luminance_numerator ./ denominator;
    Y_target = luminance_numerator ./ 32000;

    xyY = [x_target, y_target, Y_target];

    zeroRows = denominator == 0;
    xyY(zeroRows, 1:2) = NaN;
    xyY(zeroRows, 3) = 0;
end

function RGB = normalize_rgb_matrix(RGB)
    RGB = double(RGB);
    if size(RGB, 2) ~= 3
        if size(RGB, 1) == 3
            RGB = RGB.';
        else
            error('RGB_to_xyY_32000:InvalidRGB', ...
                'RGB must be a 1x3 vector or an Nx3 matrix.');
        end
    end
end

function RGB = normalize_rgb_channels(R, G, B)
    if numel(R) ~= numel(G) || numel(R) ~= numel(B)
        error('RGB_to_xyY_32000:InvalidRGB', ...
            'R, G, and B must have the same number of elements.');
    end
    RGB = [double(R(:)), double(G(:)), double(B(:))];
end
