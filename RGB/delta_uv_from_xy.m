function deltaUV = delta_uv_from_xy(actualXY, targetXY)
%DELTA_UV_FROM_XY 按 test.m 的方式计算两个 xy 坐标的 Delta_uv。

    actualUV = xy_to_uv(actualXY);
    targetUV = xy_to_uv(targetXY);

    deltaUV = sqrt(sum((actualUV - targetUV).^2, 2));
end
