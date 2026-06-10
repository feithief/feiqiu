% Example primaries (xyY) and target
div=10;
% xyY_red    = [0.710424, 0.289462, 25/div];
% xyY_green  = [0.149830, 0.716825, 25/div];
% xyY_blue   = [0.133977, 0.053097, 7/div];
% [xyY_red, xyY_green, xyY_blue] = generate_random_xyY(div);
[xyY_red, xyY_green, xyY_blue] = generate_random_xyY_range();
% xyY_red    = [0.710424, 0.289462, 1.4];
% xyY_green  = [0.149830, 0.716825, 2.2];
% xyY_blue   = [0.133977, 0.053097, 0.5];
delta_uv_list = zeros(30, 1);  % 初始化结果记录数组
delta_Y_list = zeros(30, 1);  % 初始化结果记录数组

%% 扰动
[xyY_red_true, xyY_green_true, xyY_blue_true] = disturb(xyY_red, xyY_green, xyY_blue, div);

%% 遍历所有颜色
for i = 1 : 30
    
    %% 获取目标颜色
    xyY_target = xyY_target_get(i);
    
    %% 计算扰动坐标下的PWM
    [Dred, Dgreen, Dblue] = xyY_PWM(xyY_red_true, xyY_green_true, xyY_blue_true, xyY_target);
    D_all = [Dred, Dgreen, Dblue];
    Dmax  = max(D_all);   % Dmax 就是三者中的最大值

    fprintf('Dred=%.6f, Dgreen=%.6f, Dblue=%.6f\n', Dred/Dmax*255, Dgreen/Dmax*255, Dblue/Dmax*255);
    PWM=[Dred/Dmax*255, Dgreen/Dmax*255, Dblue/Dmax*255];
    
    %% 计算扰动PWM的实际色坐标
    PWMxyY=RGB_to_xyY(PWM, xyY_red(1),xyY_red(2),xyY_red(3),...
                   xyY_green(1),xyY_green(2),xyY_green(3),...
                   xyY_blue(1),xyY_blue(2),xyY_blue(3));
    fprintf('PWMx=%.6f, PWMy=%.6f\n', PWMxyY(1), PWMxyY(2));
    
    %% 计算标准uv
    RGBuv = [4 * xyY_target(1) / (-2 * xyY_target(1) + 12 * xyY_target(2) + 3), ...
                       9 * xyY_target(2) / (-2 * xyY_target(1) + 12 * xyY_target(2) + 3)];         
                   
    %% 计算扰动uv
    actual_color_coordinate = PWMxyY;
    actualCoord_u_v = [4 * actual_color_coordinate(1) / (-2 * actual_color_coordinate(1) + 12 * actual_color_coordinate(2) + 3), ...
                       9 * actual_color_coordinate(2) / (-2 * actual_color_coordinate(1) + 12 * actual_color_coordinate(2) + 3)];
 
    %% 计算delta_uv
    delta_uv_2 = sum((RGBuv - actualCoord_u_v).^2);
    delta_uv = sqrt(sum((RGBuv - actualCoord_u_v).^2));
    % disp(delta_uv);
    fprintf('delta_uv_2=%.10f\n', delta_uv_2);
    fprintf('delta_uv=%.10f\n', delta_uv);
    delta_uv_list(i) = delta_uv;

    %% 计算标准亮度Y
    [R, G, B] = xyY_PWM(xyY_red, xyY_green, xyY_blue, xyY_target);
    DRGB = [R, G, B];
    RGBmax  = max(DRGB);   % Dmax 就是三者中的最大值

    fprintf('Dred=%.6f, Dgreen=%.6f, Dblue=%.6f\n', R/RGBmax*255, G/RGBmax*255, B/RGBmax*255);
    RGB=[R/RGBmax*255, G/RGBmax*255, B/RGBmax*255];
    xyY_target(3) = (RGB(1)*xyY_red(3) + RGB(2)*xyY_green(3) + RGB(3)*xyY_blue(3))/255;
    %% 计算delta_Y
    delta_Y = abs(PWMxyY(3) - xyY_target(3))/xyY_target(3);
    fprintf('delta_Y=%.10f\n', delta_Y);
    delta_Y_list(i) = delta_Y;
end

%% 输出所有结果
for i = 1:30
    fprintf('delta_uv=%.6f\tdelta_Y=%.6f\n', delta_uv_list(i), delta_Y_list(i));
end

%% 输出最大值
    [max_uv, idx_uv] = max(delta_uv_list);
    [max_Y,  idx_Y ] = max(delta_Y_list);
    fprintf('delta_uv_list 最大值 = %.6f (第 %d 个)\n', max_uv, idx_uv);
    fprintf('delta_Y_list  最大值 = %.6f%% (第 %d 个)\n', max_Y,  idx_Y);
    