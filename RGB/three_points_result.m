clc;
clear;
close all;

% Read three points
P1 = readPoint('请输入第1个点坐标，例如 1 2 或 1,2 ：');
P2 = readPoint('请输入第2个点坐标，例如 3 4 或 3,4 ：');
P3 = readPoint('请输入第3个点坐标，例如 5 6 或 5,6 ：');

tol = 1e-10;

% Check repeated points
if norm(P1 - P2) < tol || norm(P1 - P3) < tol || norm(P2 - P3) < tol
    error('存在重复点，无法进行分段比例计算。');
end

% Collinearity check
V1 = P2 - P1;
V2 = P3 - P1;
crossVal = V1(1) * V2(2) - V1(2) * V2(1);
isCollinear = abs(crossVal) < tol;

% Draw figure
figure;
hold on;
grid on;
axis equal;

plot(P1(1), P1(2), 'ro', 'MarkerSize', 8, 'LineWidth', 2);
plot(P2(1), P2(2), 'go', 'MarkerSize', 8, 'LineWidth', 2);
plot(P3(1), P3(2), 'bo', 'MarkerSize', 8, 'LineWidth', 2);

text(P1(1), P1(2), '  P1', 'FontSize', 12);
text(P2(1), P2(2), '  P2', 'FontSize', 12);
text(P3(1), P3(2), '  P3', 'FontSize', 12);

if isCollinear
    pts = [P1; P2; P3];
    names = {'P1', 'P2', 'P3'};

    % Sort points along the line
    if range(pts(:,1)) >= range(pts(:,2))
        [~, idx] = sort(pts(:,1));
    else
        [~, idx] = sort(pts(:,2));
    end

    pts_sorted = pts(idx, :);
    names_sorted = names(idx);

    A = pts_sorted(1, :);
    M = pts_sorted(2, :);
    B = pts_sorted(3, :);

    nameA = names_sorted{1};
    nameM = names_sorted{2};
    nameB = names_sorted{3};

    % Draw line
    plot(pts_sorted(:,1), pts_sorted(:,2), 'k-', 'LineWidth', 1.5);
    title('三个点共线');

    % Segment ratio
    d1 = norm(M - A);
    d2 = norm(B - M);
    d  = norm(B - A);

    r1 = d1 / d;
    r2 = d2 / d;

    disp('判断结果：三个点共线');
    fprintf('中间点是 %s\n', nameM);
    fprintf('分段比例 %s%s : %s%s = %.6f : %.6f\n', ...
        nameA, nameM, nameM, nameB, r1, r2);
    fprintf('校验：%.6f + %.6f = %.6f\n', r1, r2, r1 + r2);

    % Show ratio on figure
    mid1 = (A + M) / 2;
    mid2 = (M + B) / 2;
    text(mid1(1), mid1(2), sprintf('  %.4f', r1), 'FontSize', 11);
    text(mid2(1), mid2(2), sprintf('  %.4f', r2), 'FontSize', 11);

else
    pts = [P1; P2; P3; P1];
    plot(pts(:,1), pts(:,2), 'k-', 'LineWidth', 1.5);
    title('三个点不共线');

    disp('判断结果：三个点不共线');
    disp('由于不能形成一条直线上的两段，无法计算“分段比例”。');
end

xlabel('X');
ylabel('Y');

% Save image
saveas(gcf, 'three_points_result.png');
disp('图片已保存为 three_points_result.png');

% =========================
% Local function
% =========================
function P = readPoint(promptText)
    str = input(promptText, 's');
    str = strrep(str, ',', ' ');
    nums = sscanf(str, '%f');

    if numel(nums) ~= 2
        error('输入格式错误，请输入两个数，例如 1 2 或 1,2');
    end

    P = nums(:).';
end