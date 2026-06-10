function [xyY_red, xyY_green, xyY_blue] = generate_random_xyY(div)
    % 分辨率（LSB）
    LSB_Rx = 0.0012086614173228;
    LSB_Ry = 0.0014984126984127;
    LSB_RY = 0.19685/div;

    LSB_Gx = 0.0021858267716535;
    LSB_Gy = 0.0055746031746032;
    LSB_GY = 0.19685/div;

    LSB_Bx = 0.0005396078431372549;
    LSB_By = 0.0002545098039215686;
    LSB_BY = 0.086614/div;

    % 最小值
    R_x_min = 0.6279;   R_y_min = 0.2674;   R_Y_min = 25/div;
    G_x_min = 0.0517;   G_y_min = 0.5294;   G_Y_min = 25/div;
    B_x_min = 0.0743;   B_y_min = 0.0092;   B_Y_min = 7/div;

    % 随机步数 (0–127)
    step_Rx = randi([0,127]);
    step_Ry = randi([0,63]);
    step_RY = randi([0,127]);

    step_Gx = randi([0,127]);
    step_Gy = randi([0,63]);
    step_GY = randi([0,127]);

    step_Bx = randi([0,255]);
    step_By = randi([0,255]);
    step_BY = randi([0,127]);

    % 生成随机 xyY
    xyY_red   = [R_x_min + step_Rx * LSB_Rx, ...
                 R_y_min + step_Ry * LSB_Ry, ...
                 R_Y_min + step_RY * LSB_RY];

    xyY_green = [G_x_min + step_Gx * LSB_Gx, ...
                 G_y_min + step_Gy * LSB_Gy, ...
                 G_Y_min + step_GY * LSB_GY];

    xyY_blue  = [B_x_min + step_Bx * LSB_Bx, ...
                 B_y_min + step_By * LSB_By, ...
                 B_Y_min + step_BY * LSB_BY];
end
