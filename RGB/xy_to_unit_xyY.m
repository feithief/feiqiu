function xyY = xy_to_unit_xyY(xy)
%XY_TO_UNIT_XYY 把 xy 扩展成用于旧版归一化验证的 xyY。
%
% 说明：
% 这个函数保留给旧版“归一化验证”使用。
% 当前白点参与混光的主流程，已经改成：
%   1) 先用 lambda 计算白点分量亮度 Y1 和交点分量亮度 Y2
%   2) 再分别构造 [x_w, y_w, Y1] 和 [x_p, y_p, Y2]
%
% 这里取 Y = y。
% 这样 xyY 转成 XYZ 后会得到：
%   X = x, Y = y, Z = 1 - x - y
%
% 这个写法的好处是，两个色点按线段权重相加时，
% XYZ 相加后的 xy 会严格落在线段上的对应位置。

    xy = double(xy(:).');
    xyY = [xy, xy(2)];
end
