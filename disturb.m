function [xyY_red_true, xyY_green_true, xyY_blue_true] = disturb(xyY_red, xyY_green, xyY_blue, div)
%% LSB
xLSB.R = 0.0012086614173228;    yLSB.R = 0.0014984126984127;    YLSB.R = 0.19685/div;
xLSB.G = 0.0021858267716535;    yLSB.G = 0.0055746031746032;    YLSB.G = 0.19685/div;
xLSB.B = 0.0005396078431372549; yLSB.B = 0.0002545098039215686;   YLSB.B = 0.086614/div;

%% 小工具
sgn = @() (2*randi([0,1]) - 1);   % 随机取 +1 或 -1

rng('shuffle');  % 随机种子

%% Red
xyY_red_true = [ ...
    xyY_red(1) + sgn()*xLSB.R, ...
    xyY_red(2) + sgn()*yLSB.R, ...
    xyY_red(3) + sgn()*YLSB.R ];

%% Green
xyY_green_true = [ ...
    xyY_green(1) + sgn()*xLSB.G, ...
    xyY_green(2) + sgn()*yLSB.G, ...
    xyY_green(3) + sgn()*YLSB.G ];

%% Blue
xyY_blue_true = [ ...
    xyY_blue(1) + sgn()*xLSB.B, ...
    xyY_blue(2) + sgn()*yLSB.B, ...
    xyY_blue(3) + sgn()*YLSB.B ];

end

