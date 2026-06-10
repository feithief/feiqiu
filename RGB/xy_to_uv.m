function uv = xy_to_uv(xy)
%XY_TO_UV 将 CIE 1931 xy 转换为 CIE 1976 u'v'。

    x = xy(:, 1);
    y = xy(:, 2);
    denominator = -2 * x + 12 * y + 3;

    uv = [4 * x ./ denominator, 9 * y ./ denominator];
end
