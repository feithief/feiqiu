function [x, y, Y] = sRGB_xyY(rgb)
% function [x, y] = claculate_color()
    % 将 sRGB 转换为 XYZ
%     rgb = [177, 255, 64];
    xyz = sRGB_to_XYZ(rgb);

    % 从 XYZ 计算 xy
    X = xyz(1);
    Y = xyz(2);
    Z = xyz(3);

    % 计算 xy 色度坐标
    total = X + Y + Z;
    
    if total == 0
        x = 0;
        y = 0;
    else
        x = X / total;
        y = Y / total;
    end
end

function xyz = sRGB_to_XYZ(rgb)
    % 确保 RGB 输入在 [0, 1] 范围内
    rgb = rgb / 255;
    rgb_linear = zeros(1, 3);  % 初始化线性 RGB
    % 转换 RGB 到线性空间
     for i = 1:3
        V = rgb(i);
        if V <= 0.04045
            rgb_linear(i) = V / 12.92;
        else
            rgb_linear(i) = ((V + 0.055) / 1.055) ^ 2.4;
%             rgb_linear(i) = 0.3031*V^3 + 0.6910*V^2 + 0.0052*V + 0.0016;
            
%             rgb_linear(i) = 0.2919*V^3 + 0.7106*V^2 - 0.0046*V + 0.0027;
        end 
     end 

    % sRGB 到 XYZ 转换矩阵
    M = [0.4124564 0.3575761 0.1804375; 
         0.2126729 0.7151522 0.0721750; 
         0.0193339 0.1192920 0.9503041];
    
    % 计算 XYZ
    xyz = M * rgb_linear(:);
end
