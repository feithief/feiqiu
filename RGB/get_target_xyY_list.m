function targetxyY = get_target_xyY_list()
%GET_TARGET_XYY_LIST 返回 30 组目标色的 xyY。
%
% 这里直接读取项目根目录中的 xyY_target_get.m，
% 使白点参与混光的验证，能够使用每个目标点对应的真实亮度 Y。

    thisDir = fileparts(mfilename('fullpath'));
    projectDir = fileparts(thisDir);
    if exist('xyY_target_get', 'file') ~= 2
        addpath(projectDir);
    end

    n = 30;
    targetxyY = zeros(n, 3);
    for i = 1:n
        targetxyY(i, :) = xyY_target_get(i);
    end
end
