function plot_target_result(step, whiteXY, targetIndex)
%PLOT_TARGET_RESULT Plot the formula view and the xy geometry view.

    if nargin < 2 || isempty(whiteXY)
        whiteXY = [0.3090 0.3176];
    end
    if nargin < 3 || isempty(targetIndex)
        targetIndex = 2;
    end

    plot_process_figure(step, whiteXY, targetIndex);
    plot_xy_geometry(step, whiteXY, targetIndex);
end

function plot_process_figure(step, whiteXY, targetIndex)
    targetxyY = step.Target_xyY;
    whitePart = step.WhiteComponent_xyY;
    pointPart = step.PointComponent_xyY;
    verifyxyY = step.Verify_xyY;
    coeff = step.Coeff;

    whiteInfo = step.WhitePWMInfo;
    pointInfo = step.PointPWMInfo;

    hasPoint = all(isfinite(step.IntersectXY));
    if hasPoint
        pointXY = step.IntersectXY;
    else
        pointXY = [NaN NaN];
    end

    deltaXText = format_scientific_value(step.ErrorXY(1), 4, 'latex');
    deltaYxyText = format_scientific_value(step.ErrorXY(2), 4, 'latex');
    deltaYText = format_scientific_value(step.ErrorY, 4, 'latex');
    deltaUVText = format_scientific_value(step.Delta_uv, 4, 'latex');
    relativeDeltaYText = format_scientific_value(step.RelativeDeltaY, 4, 'latex');
    splitErrorText = format_scientific_value(step.SplitErrorY, 4, 'latex');

    figure('Name', sprintf('第%d个目标颜色: 计算过程', targetIndex), ...
        'Color', 'w', 'Position', [60 40 1450 920]);

    ax = axes('Position', [0 0 1 1]);
    xlim(ax, [0 1]);
    ylim(ax, [0 1]);
    axis(ax, 'off');
    hold(ax, 'on');

    draw_rect(ax, [0.04 0.73 0.92 0.22], [0.96 0.98 1.00], [0.75 0.80 0.90]);
    draw_rect(ax, [0.04 0.46 0.44 0.22], [0.97 0.99 1.00], [0.75 0.85 0.90]);
    draw_rect(ax, [0.52 0.46 0.44 0.22], [0.97 1.00 0.97], [0.75 0.90 0.75]);
    draw_rect(ax, [0.04 0.22 0.92 0.19], [1.00 0.98 0.95], [0.90 0.82 0.72]);
    draw_rect(ax, [0.04 0.05 0.92 0.12], [1.00 0.96 0.96], [0.90 0.78 0.78]);

    add_plain_line(0.05, 0.965, sprintf('第 %d 个目标颜色计算过程', targetIndex), 16, 'bold');

    add_plain_line(0.06, 0.92, '1. 目标颜色、交点、共线参数与亮度分解', 13, 'bold');
    add_latex_line(0.06, 0.885, sprintf('T=(%.4f,\\ %.4f,\\ %.6f)', ...
        targetxyY(1), targetxyY(2), targetxyY(3)), 16);
    add_latex_line(0.06, 0.850, sprintf('W=(%.4f,\\ %.4f)', whiteXY(1), whiteXY(2)), 16);
    if hasPoint
        add_latex_line(0.36, 0.850, sprintf('P=(%.4f,\\ %.4f)', pointXY(1), pointXY(2)), 16);
    else
        add_plain_line(0.36, 0.850, 'P 不需要: 当前目标点与白点重合', 13, 'normal');
    end
    add_latex_line(0.06, 0.815, sprintf('\\lambda = \\frac{k_2}{k_3} = \\frac{%.6f}{%.6f} = %.6f', ...
        coeff.k2, coeff.k3, coeff.lambda), 16);
    add_latex_line(0.52, 0.815, sprintf('1-\\lambda = \\frac{k_1}{k_3} = \\frac{%.6f}{%.6f} = %.6f', ...
        coeff.k1, coeff.k3, coeff.oneMinusLambda), 16);
    add_latex_line(0.06, 0.780, sprintf('Y_{W}=Y_{T}\\frac{(1-\\lambda)y_{W}}{y_{T}}=%.6f', whitePart(3)), 16);
    if hasPoint
        add_latex_line(0.52, 0.780, sprintf('Y_{P}=Y_{T}\\frac{\\lambda y_{P}}{y_{T}}=%.6f', pointPart(3)), 16);
    else
        add_latex_line(0.52, 0.780, 'Y_{P}=0', 16);
    end

    add_plain_line(0.06, 0.655, '2. 白点分量的公式与结果', 13, 'bold');
    add_latex_line(0.06, 0.620, sprintf('W''=(%.4f,\\ %.4f,\\ %.6f)', ...
        whitePart(1), whitePart(2), whitePart(3)), 16);
    add_latex_line(0.06, 0.585, sprintf('z_{W}=1-x_{W}-y_{W}=%.6f', whiteInfo.z_target), 15);
    add_latex_line(0.06, 0.550, sprintf('D_{W}=M^{-1}[x_{W},\\ y_{W},\\ z_{W}]^{T}=[%.6f,\\ %.6f,\\ %.6f]^{T}', ...
        whiteInfo.D(1), whiteInfo.D(2), whiteInfo.D(3)), 14);
    add_latex_line(0.06, 0.515, ...
        'PWM_{W}=\frac{Y_{W}}{Y_{r}D_{r}+Y_{g}D_{g}+Y_{b}D_{b}}\cdot 32000 \cdot D_{W}', 14);
    add_latex_line(0.06, 0.480, sprintf('PWM_{W}=[%.6f,\\ %.6f,\\ %.6f]^{T}', ...
        step.WhitePWM(1), step.WhitePWM(2), step.WhitePWM(3)), 14);

    add_plain_line(0.54, 0.655, '3. 交点分量的公式与结果', 13, 'bold');
    if hasPoint
        add_latex_line(0.54, 0.620, sprintf('P''=(%.4f,\\ %.4f,\\ %.6f)', ...
            pointPart(1), pointPart(2), pointPart(3)), 16);
        add_latex_line(0.54, 0.585, sprintf('z_{P}=1-x_{P}-y_{P}=%.6f', pointInfo.z_target), 15);
        add_latex_line(0.54, 0.550, sprintf('D_{P}=M^{-1}[x_{P},\\ y_{P},\\ z_{P}]^{T}=[%.6f,\\ %.6f,\\ %.6f]^{T}', ...
            pointInfo.D(1), pointInfo.D(2), pointInfo.D(3)), 14);
        add_latex_line(0.54, 0.515, ...
            'PWM_{P}=\frac{Y_{P}}{Y_{r}D_{r}+Y_{g}D_{g}+Y_{b}D_{b}}\cdot 32000 \cdot D_{P}', 14);
        add_latex_line(0.54, 0.480, sprintf('PWM_{P}=[%.6f,\\ %.6f,\\ %.6f]^{T}', ...
            step.PointPWM(1), step.PointPWM(2), step.PointPWM(3)), 14);
    else
        add_plain_line(0.54, 0.620, '当前目标点与白点重合，因此没有交点分量。', 12, 'normal');
        add_latex_line(0.54, 0.575, 'P''=[NaN,\\ NaN,\\ 0]', 15);
        add_latex_line(0.54, 0.535, 'PWM_{P}=[0,\\ 0,\\ 0]^{T}', 15);
    end

    add_plain_line(0.06, 0.385, '4. PWM 合成与 RGB_to_xyY_32000 回算', 13, 'bold');
    add_latex_line(0.06, 0.350, sprintf('PWM_{mix}=PWM_{W}+PWM_{P}=[%.6f,\\ %.6f,\\ %.6f]^{T}', ...
        step.MixPWM(1), step.MixPWM(2), step.MixPWM(3)), 15);
    add_latex_line(0.06, 0.315, ...
        'x_{V}=\frac{(Y_{r}/y_{r})x_{r}R+(Y_{g}/y_{g})x_{g}G+(Y_{b}/y_{b})x_{b}B}{(Y_{r}/y_{r})R+(Y_{g}/y_{g})G+(Y_{b}/y_{b})B}', 14);
    add_latex_line(0.06, 0.282, sprintf('x_{V}=%.6f', verifyxyY(1)), 15);
    add_latex_line(0.46, 0.315, ...
        'y_{V}=\frac{Y_{r}R+Y_{g}G+Y_{b}B}{(Y_{r}/y_{r})R+(Y_{g}/y_{g})G+(Y_{b}/y_{b})B}', 14);
    add_latex_line(0.46, 0.282, sprintf('y_{V}=%.6f', verifyxyY(2)), 15);
    add_latex_line(0.06, 0.248, 'Y_{V}=\frac{Y_{r}R+Y_{g}G+Y_{b}B}{32000}', 14);
    add_latex_line(0.29, 0.248, sprintf('Y_{V}=%.6f', verifyxyY(3)), 15);

    add_plain_line(0.06, 0.145, '5. 误差', 13, 'bold');
    add_latex_line(0.06, 0.112, sprintf('\\Delta x = %s,\\ \\Delta y = %s,\\ \\Delta Y = %s', ...
        deltaXText, deltaYxyText, deltaYText), 15);
    add_latex_line(0.52, 0.112, sprintf('\\Delta uv = %s,\\ \\delta_Y = %s', ...
        deltaUVText, relativeDeltaYText), 15);
    add_latex_line(0.06, 0.078, sprintf('Y_{W}+Y_{P}-Y_{T}=%s', splitErrorText), 15);
end

function plot_xy_geometry(step, whiteXY, targetIndex)
    lambdaChar = char(955);
    [verifyRed, verifyGreen, verifyBlue, verifyTriangleXY] = fixed_rgb_primaries(step.VerifyPrimaries);
    intersectionTriangleXY = normalize_intersection_triangle(step.IntersectionPrimaries);

    targetXY = step.TargetXY;
    verifyXY = step.VerifyXY;
    hasPoint = all(isfinite(step.IntersectXY));

    figure('Name', sprintf('第%d个目标颜色: 验算色域与求交三角形', targetIndex), ...
        'Color', 'w', 'Position', [100 100 1020 780]);
    hold on;
    grid on;
    axis equal;

    verifyClosed = [verifyTriangleXY; verifyTriangleXY(1, :)];
    intersectionClosed = [intersectionTriangleXY; intersectionTriangleXY(1, :)];

    hVerifyArea = fill(verifyClosed(:, 1), verifyClosed(:, 2), [0.92 0.95 1.00], ...
        'FaceAlpha', 0.35, 'EdgeColor', 'none');
    hVerifyBoundary = plot(verifyClosed(:, 1), verifyClosed(:, 2), 'k-', 'LineWidth', 1.4);
    hIntersectionBoundary = plot(intersectionClosed(:, 1), intersectionClosed(:, 2), ...
        '-', 'Color', [0.85 0.35 0.10], 'LineWidth', 1.8);

    hWT = plot([whiteXY(1), targetXY(1)], [whiteXY(2), targetXY(2)], ...
        'b-', 'LineWidth', 2.0);

    if hasPoint
        intersectXY = step.IntersectXY;
        hTP = plot([targetXY(1), intersectXY(1)], [targetXY(2), intersectXY(2)], ...
            'm-', 'LineWidth', 2.0);
        plot([whiteXY(1), intersectXY(1)], [whiteXY(2), intersectXY(2)], ...
            'k--', 'LineWidth', 0.8);
        rayDirection = intersectXY - whiteXY;
    else
        intersectXY = [NaN NaN];
        hTP = plot(NaN, NaN, 'm-');
        rayDirection = targetXY - whiteXY;
    end

    plot_point_with_y(verifyRed(1:2), verifyRed(3), 'ro', '验算R', [0.010 0.010], false);
    plot_point_with_y(verifyGreen(1:2), verifyGreen(3), 'go', '验算G', [0.010 0.010], false);
    plot_point_with_y(verifyBlue(1:2), verifyBlue(3), 'bo', '验算B', [0.010 0.010], false);

    plot_point_xy(intersectionTriangleXY(1, :), '求交R', [0.008 -0.018], [0.85 0.35 0.10]);
    plot_point_xy(intersectionTriangleXY(2, :), '求交G', [0.008 0.010], [0.85 0.35 0.10]);
    plot_point_xy(intersectionTriangleXY(3, :), '求交B', [0.008 -0.020], [0.85 0.35 0.10]);

    plot_point_with_y(whiteXY, step.Luminance.WhiteY, 'ks', '白点W', [0.010 -0.020], false);
    if hasPoint
        plot_point_with_y(intersectXY, step.Luminance.PointY, 'cp', '交点P', [0.012 0.000], false);
    end

    normalVec = upper_side_normal(rayDirection);
    pointLabelMag = 0.030;
    segmentLabelMag = 0.020;

    plot_point_with_y(targetXY, step.TargetY, 'md', '目标T', -pointLabelMag * normalVec, true);
    plot_point_with_y(verifyXY, step.VerifyY, 'yx', '验证V', pointLabelMag * normalVec, true);

    if hasPoint
        label_segment_ratio(whiteXY, targetXY, ...
            sprintf('WT/WP = %s = k2/k3 = %.6f', lambdaChar, step.Coeff.lambda), ...
            -segmentLabelMag * normalVec, true);
        label_segment_ratio(targetXY, intersectXY, ...
            sprintf('TP/WP = 1-%s = k1/k3 = %.6f', lambdaChar, step.Coeff.oneMinusLambda), ...
            segmentLabelMag * normalVec, true);
        pointInfo = sprintf('相交边: %s\n射线 t = %.6f\n', step.Edge, step.Ray_t);
    else
        pointInfo = '当前目标点就是白点，不需要交点。\n';
    end

    deltaUVText = format_scientific_value(step.Delta_uv);
    deltaYText = format_scientific_value(step.DeltaY);
    relativeDeltaYText = format_scientific_value(step.RelativeDeltaY);

    infoText = sprintf([ ...
        '%s', ...
        'k1 = %.6f, k2 = %.6f, k3 = %.6f\n', ...
        '%s = %.6f, 1-%s = %.6f\n', ...
        '目标Y = %.6f\n白点分量Y = %.6f\n交点分量Y = %.6f\n', ...
        '验证Y = %.6f\n', ...
        'DeltaUV = %s\nDeltaY = %s\n相对DeltaY = %s'], ...
        pointInfo, ...
        step.Coeff.k1, step.Coeff.k2, step.Coeff.k3, ...
        lambdaChar, step.Coeff.lambda, lambdaChar, step.Coeff.oneMinusLambda, ...
        step.TargetY, step.Luminance.WhiteY, step.Luminance.PointY, ...
        step.VerifyY, deltaUVText, deltaYText, relativeDeltaYText);
    text(0.03, 0.97, infoText, 'Units', 'normalized', ...
        'VerticalAlignment', 'top', 'BackgroundColor', 'w', ...
        'EdgeColor', [0.7 0.7 0.7], 'Margin', 6);

    xlabel('x');
    ylabel('y');
    title(sprintf('第 %d 个目标颜色: 验算色域与求交三角形', targetIndex));
    legend([hVerifyBoundary, hVerifyArea, hIntersectionBoundary, hWT, hTP], ...
        {'验算色域边界', '验算色域区域', '求交三角形', 'W-T 线段', 'T-P 线段'}, ...
        'Location', 'best');
end

function draw_rect(ax, position, faceColor, edgeColor)
    rectangle(ax, 'Position', position, ...
        'FaceColor', faceColor, 'EdgeColor', edgeColor, 'LineWidth', 1.0);
end

function add_plain_line(x, y, textValue, fontSize, fontWeight)
    text(x, y, textValue, 'Units', 'normalized', ...
        'FontSize', fontSize, 'FontWeight', fontWeight, ...
        'Interpreter', 'none', 'Clipping', 'off');
end

function add_latex_line(x, y, latexValue, fontSize)
    text(x, y, ['$' latexValue '$'], 'Units', 'normalized', ...
        'FontSize', fontSize, 'Interpreter', 'latex', ...
        'Clipping', 'off');
end

function plot_point_xy(xy, labelText, offset, colorValue)
    plot(xy(1), xy(2), 'o', 'LineWidth', 1.6, 'MarkerSize', 8, ...
        'MarkerEdgeColor', colorValue);
    [hAlign, vAlign] = label_alignment(offset);
    text(xy(1) + offset(1), xy(2) + offset(2), ...
        sprintf('%s (%.4f, %.4f)', labelText, xy(1), xy(2)), ...
        'FontSize', 8, 'FontWeight', 'bold', 'Color', colorValue, ...
        'HorizontalAlignment', hAlign, ...
        'VerticalAlignment', vAlign);
end

function plot_point_with_y(xy, yValue, style, labelText, offset, forceCentered)
    if nargin < 6
        forceCentered = false;
    end

    plot(xy(1), xy(2), style, 'LineWidth', 1.8, 'MarkerSize', 9);
    if forceCentered
        hAlign = 'center';
        vAlign = centered_vertical_alignment(offset);
    else
        [hAlign, vAlign] = label_alignment(offset);
    end

    text(xy(1) + offset(1), xy(2) + offset(2), ...
        sprintf('%s (%.4f, %.4f, %.6f)', labelText, xy(1), xy(2), yValue), ...
        'FontSize', 9, 'FontWeight', 'bold', ...
        'HorizontalAlignment', hAlign, ...
        'VerticalAlignment', vAlign);
end

function label_segment_ratio(p1, p2, labelText, offset, forceCentered)
    if nargin < 5
        forceCentered = false;
    end

    mid = (p1 + p2) / 2;
    if forceCentered
        hAlign = 'center';
        vAlign = centered_vertical_alignment(offset);
    else
        [hAlign, vAlign] = label_alignment(offset);
    end

    text(mid(1) + offset(1), mid(2) + offset(2), labelText, ...
        'FontSize', 9, 'FontWeight', 'bold', ...
        'BackgroundColor', 'w', 'EdgeColor', [0.75 0.75 0.75], ...
        'Margin', 4, ...
        'HorizontalAlignment', hAlign, ...
        'VerticalAlignment', vAlign);
end

function [hAlign, vAlign] = label_alignment(offset)
    if offset(1) > 0.004
        hAlign = 'left';
    elseif offset(1) < -0.004
        hAlign = 'right';
    else
        hAlign = 'center';
    end

    if offset(2) > 0.004
        vAlign = 'bottom';
    elseif offset(2) < -0.004
        vAlign = 'top';
    else
        vAlign = 'middle';
    end
end

function vAlign = centered_vertical_alignment(offset)
    if offset(2) > 0
        vAlign = 'bottom';
    elseif offset(2) < 0
        vAlign = 'top';
    else
        vAlign = 'middle';
    end
end

function normalVec = upper_side_normal(directionVec)
    directionVec = double(directionVec(:).');
    if norm(directionVec) <= eps
        normalVec = [0 1];
        return;
    end

    normalVec = [-directionVec(2), directionVec(1)];
    normalVec = normalVec / norm(normalVec);

    if normalVec(2) < 0
        normalVec = -normalVec;
    end
end
