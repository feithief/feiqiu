clc; clear; close all;

%——— 三基色（校光值）———
xr=0.710424; yr=0.289462; Yr=1.4;
xg=0.149830; yg=0.716825; Yg=2.2;
xb=0.133977; yb=0.053097; Yb=0.5;

%——— 将公式数值化（方便高效计算）———
aR = (Yr/yr)*xr;  aG = (Yg/yg)*xg;  aB = (Yb/yb)*xb;  % x 分子系数
dR = (Yr/yr);     dG = (Yg/yg);     dB = (Yb/yb);     % 公分母系数
bR = Yr;          bG = Yg;          bB = Yb;          % y 分子系数 (=亮度权重)

% 采样步长（越小越细）：
t = linspace(0,255,65);   % 比如 65 点 ~ 步长约4
[U,V] = meshgrid(t,t);

% 小工具：根据 R,G,B 网格算 x,y,Y（三个都是二维阵列，可用 surf）
calc_xyY = @(R,G,B) deal( ...
    (aR.*R + aG.*G + aB.*B) ./ (dR.*R + dG.*G + dB.*B), ...
    (bR.*R + bG.*G + bB.*B) ./ (dR.*R + dG.*G + dB.*B), ...
    (bR.*R + bG.*G + bB.*B)/255 );

%——— 6 个表面：R/G/B 固定为 0 或 255（RGB 立方体的 6 个面）———
faces = { ...
   struct('R',0,   'G',U, 'B',V, 'name','R=0'), ...
   struct('R',255, 'G',U, 'B',V, 'name','R=255'), ...
   struct('R',U,   'G',0, 'B',V, 'name','G=0'), ...
   struct('R',U,   'G',255,'B',V, 'name','G=255'), ...
   struct('R',U,   'G',V, 'B',0, 'name','B=0'), ...
   struct('R',U,   'G',V, 'B',255,'name','B=255')};

figure('Color','w'); hold on;
for k = 1:numel(faces)
    R = double(faces{k}.R); G = double(faces{k}.G); B = double(faces{k}.B);
    den = dR.*R + dG.*G + dB.*B;
    [x,y,Y] = calc_xyY(R,G,B);

    % 去掉无效点（分母=0 的位置设为 NaN，surf 会自动忽略）
    x(den<=0) = NaN; y(den<=0) = NaN; Y(den<=0) = NaN;

    % 画参数曲面（xy 为平面坐标，Y 为高度）
    s = surf(x, y, Y, Y, 'EdgeColor','none', 'FaceAlpha',0.55);
end

% 轴与交互
xlabel('x'); ylabel('y'); zlabel('Y'); grid on; box on;
axis vis3d; view(42,26); colorbar; title('xyY 空间边界表面（由 RGB 立方体 6 面映射）');
rotate3d on;

%——— 高亮三基点（固定的 xyY）———
plot3(xr, yr, Yr, 'rp', 'MarkerSize',14, 'MarkerFaceColor','r'); % Red
plot3(xg, yg, Yg, 'gp', 'MarkerSize',14, 'MarkerFaceColor','g'); % Green
plot3(xb, yb, Yb, 'bp', 'MarkerSize',14, 'MarkerFaceColor','b'); % Blue

%（可选）白点：R=G=B=255
Rw=255; Gw=255; Bw=255;
xw = (aR*Rw + aG*Gw + aB*Bw)/(dR*Rw + dG*Gw + dB*Bw);
yw = (bR*Rw + bG*Gw + bB*Bw)/(dR*Rw + dG*Gw + dB*Bw);
Yw = (bR*Rw + bG*Gw + bB*Bw)/255;    % = 1.4+2.2+0.5 = 4.1
plot3(xw, yw, Yw, 'kp', 'MarkerSize',14, 'MarkerFaceColor','k'); % white
legend({'R=0','R=255','G=0','G=255','B=0','B=255','Red','Green','Blue','White'}, ...
       'Location','northeastoutside');
