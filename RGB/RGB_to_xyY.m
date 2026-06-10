function xyY = RGB_to_xyY(RGB, G, B)
%RGB_TO_XYY 使用固定三基色参数，把 RGB 驱动值换算成 xyY。
%
% 调用方式：
%   xyY = RGB_to_xyY([R G B])
%   xyY = RGB_to_xyY(R, G, B)
%
% 固定三基色顶点：
%   red   = [x y Y] = [0.69 0.31 1.40]
%   green = [x y Y] = [0.17 0.72 2.20]
%   blue  = [x y Y] = [0.15 0.03 0.50]

    % 支持两种输入：
    % 1) RGB_to_xyY([R G B])，可以一次输入 Nx3 矩阵
    % 2) RGB_to_xyY(R, G, B)，三个通道分开输入
    if nargin == 1
        RGB = double(RGB);
        if size(RGB, 2) ~= 3
            if size(RGB, 1) == 3
                RGB = RGB.';
            else
                error('RGB_to_xyY:InvalidRGB', ...
                    'RGB must be a 1x3 vector or an Nx3 matrix.');
            end
        end
    elseif nargin == 3
        if numel(RGB) ~= numel(G) || numel(RGB) ~= numel(B)
            error('RGB_to_xyY:InvalidRGB', ...
                'R, G, and B must have the same number of elements.');
        end
        RGB = [double(RGB(:)), double(G(:)), double(B(:))];
    else
        error('RGB_to_xyY:InvalidInput', ...
            'Use RGB_to_xyY([R G B]) or RGB_to_xyY(R, G, B).');
    end

    % 三个基色的色坐标和亮度。这里已经按你的要求定死。
    x_red = 0.69;
    y_red = 0.31;
    Y_red = 1.40;

    x_green = 0.17;
    y_green = 0.72;
    Y_green = 2.20;

    x_blue = 0.15;
    y_blue = 0.03;
    Y_blue = 0.50;

    R = RGB(:, 1);
    G = RGB(:, 2);
    B = RGB(:, 3);

    % 公式分母：
    % Yred/yred*R + Ygreen/ygreen*G + Yblue/yblue*B
    % 它等价于混光后的 X+Y+Z 总量。
    denominator = Y_red / y_red * R ...
                + Y_green / y_green * G ...
                + Y_blue / y_blue * B;

    % 公式中 x_target 的分子。
    % 每个基色先用 Y/y 换成 XYZ 权重，再乘自己的 x 坐标。
    x_numerator = Y_red / y_red * x_red * R ...
                + Y_green / y_green * x_green * G ...
                + Y_blue / y_blue * x_blue * B;

    % 公式中 y_target 的分子，同时也是混光后的亮度加权和。
    y_numerator = Y_red * R + Y_green * G + Y_blue * B;

    % 最终输出：
    % x = x_numerator / denominator
    % y = y_numerator / denominator
    % Y = y_numerator / 255
    xyY = [x_numerator ./ denominator, ...
           y_numerator ./ denominator, ...
           y_numerator ./ 255];

    % 如果 RGB 全为 0，分母为 0，没有有效色坐标；亮度置 0。
    zeroRows = denominator == 0;
    xyY(zeroRows, 1:2) = NaN;
    xyY(zeroRows, 3) = 0;
end
