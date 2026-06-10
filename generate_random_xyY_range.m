function [xyY_red, xyY_green, xyY_blue] = generate_random_xyY_range()
    % 范围定义（来自表格）
    R_x_range = [0.6279, 0.7814];
    R_y_range = [0.2674, 0.3618];
    R_Y_range = [25, 50];

    G_x_range = [0.0517, 0.3293];
    G_y_range = [0.5294, 0.8806];
    G_Y_range = [25, 50];

    B_x_range = [0.0743, 0.2119];
    B_y_range = [0.0092, 0.0741];
    B_Y_range = [7, 18];

    % 随机生成 (均匀分布)
    xyY_red   = [rand_range(R_x_range), rand_range(R_y_range), rand_range(R_Y_range)];
    xyY_green = [rand_range(G_x_range), rand_range(G_y_range), rand_range(G_Y_range)];
    xyY_blue  = [rand_range(B_x_range), rand_range(B_y_range), rand_range(B_Y_range)];
end

function val = rand_range(range)
    % 在 [range(1), range(2)] 内均匀生成一个随机数
    val = range(1) + (range(2)-range(1))*rand();
end
