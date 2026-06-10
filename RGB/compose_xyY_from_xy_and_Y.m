function xyY = compose_xyY_from_xy_and_Y(xy, Y)
%COMPOSE_XYY_FROM_XY_AND_Y 把 xy 坐标和亮度 Y 组合成一个 xyY 行向量。
%
% 输入：
%   xy : [x y]
%   Y  : 该色点对应的亮度
%
% 输出：
%   xyY : [x y Y]

    xy = double(xy(:).');
    Y = double(Y);

    if numel(xy) ~= 2
        error('compose_xyY_from_xy_and_Y:InvalidXY', ...
            'xy must be a 1x2 vector.');
    end

    xyY = [xy, Y];
end
