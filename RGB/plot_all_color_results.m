function plot_all_color_results(allResult, whiteXY, config, selectedIndex)
%PLOT_ALL_COLOR_RESULTS Show one overview figure for all color results.

    if nargin < 2 || isempty(whiteXY)
        whiteXY = [0.3090 0.3176];
    end
    if nargin < 3 || isempty(config)
        config = default_white_mix_config();
    end
    if nargin < 4 || isempty(selectedIndex)
        selectedIndex = 4;
    end

    [verifyRed, verifyGreen, verifyBlue, verifyTriangleXY] = fixed_rgb_primaries(config.verifyPrimaries);
    intersectionTriangleXY = normalize_intersection_triangle(config.intersectionPrimaries);

    indexValue = allResult{:, '序号'};
    isValid = allResult{:, '是否有效'};
    statusText = allResult{:, '状态'};
    targetX = allResult{:, '目标x'};
    targetY = allResult{:, '目标y'};
    targetL = allResult{:, '目标Y'};
    verifyX = allResult{:, '验证x'};
    verifyY = allResult{:, '验证y'};
    verifyL = allResult{:, '验证Y'};
    deltaUV = allResult{:, '验证DeltaUV'};

    figure('Name', 'All Color Results Overview', ...
        'Color', 'w', 'Position', [80 60 1600 920]);

    ax1 = axes('Position', [0.06 0.10 0.52 0.82]);
    hold(ax1, 'on');
    grid(ax1, 'on');
    axis(ax1, 'equal');

    verifyClosed = [verifyTriangleXY; verifyTriangleXY(1, :)];
    intersectionClosed = [intersectionTriangleXY; intersectionTriangleXY(1, :)];

    fill(ax1, verifyClosed(:, 1), verifyClosed(:, 2), [0.92 0.95 1.00], ...
        'FaceAlpha', 0.35, 'EdgeColor', 'none');
    hVerifyBoundary = plot(ax1, verifyClosed(:, 1), verifyClosed(:, 2), ...
        'k-', 'LineWidth', 1.4);
    hIntersectionBoundary = plot(ax1, intersectionClosed(:, 1), intersectionClosed(:, 2), ...
        '-', 'Color', [0.85 0.35 0.10], 'LineWidth', 1.8);

    hTarget = scatter(ax1, targetX, targetY, 42, 'm', 'd', 'filled');
    hVerify = scatter(ax1, verifyX, verifyY, 36, [0.95 0.75 0.05], 'x', 'LineWidth', 1.4);
    hWhite = plot(ax1, whiteXY(1), whiteXY(2), 'ks', 'LineWidth', 1.8, 'MarkerSize', 8);

    plot(ax1, verifyRed(1), verifyRed(2), 'ro', 'LineWidth', 1.6, 'MarkerSize', 8);
    plot(ax1, verifyGreen(1), verifyGreen(2), 'go', 'LineWidth', 1.6, 'MarkerSize', 8);
    plot(ax1, verifyBlue(1), verifyBlue(2), 'bo', 'LineWidth', 1.6, 'MarkerSize', 8);

    text(ax1, verifyRed(1) + 0.010, verifyRed(2) + 0.010, ...
        sprintf('R (%.4f, %.4f, %.3f)', verifyRed(1), verifyRed(2), verifyRed(3)), ...
        'FontSize', 8, 'FontWeight', 'bold');
    text(ax1, verifyGreen(1) + 0.010, verifyGreen(2) + 0.010, ...
        sprintf('G (%.4f, %.4f, %.3f)', verifyGreen(1), verifyGreen(2), verifyGreen(3)), ...
        'FontSize', 8, 'FontWeight', 'bold');
    text(ax1, verifyBlue(1) + 0.010, verifyBlue(2) + 0.010, ...
        sprintf('B (%.4f, %.4f, %.3f)', verifyBlue(1), verifyBlue(2), verifyBlue(3)), ...
        'FontSize', 8, 'FontWeight', 'bold');
    text(ax1, whiteXY(1) + 0.010, whiteXY(2) - 0.018, ...
        sprintf('W (%.4f, %.4f)', whiteXY(1), whiteXY(2)), ...
        'FontSize', 8, 'FontWeight', 'bold');

    for i = 1:numel(indexValue)
        if indexValue(i) == selectedIndex
            markerSize = 80;
            lineWidth = 1.8;
            fontWeight = 'bold';
            textColor = [0.75 0.00 0.00];
        else
            markerSize = 42;
            lineWidth = 1.0;
            fontWeight = 'normal';
            textColor = [0.35 0.00 0.35];
        end

        scatter(ax1, targetX(i), targetY(i), markerSize, 'm', 'd', ...
            'LineWidth', lineWidth);
        scatter(ax1, verifyX(i), verifyY(i), markerSize * 0.7, [0.95 0.75 0.05], 'x', ...
            'LineWidth', lineWidth);

        text(ax1, targetX(i) + 0.004, targetY(i) + 0.004, sprintf('%02d', indexValue(i)), ...
            'FontSize', 8, 'FontWeight', fontWeight, 'Color', textColor);
    end

    xlabel(ax1, 'x');
    ylabel(ax1, 'y');
    title(ax1, sprintf('All 30 colors: target vs verify (highlight #%d)', selectedIndex));
    legend(ax1, [hVerifyBoundary, hIntersectionBoundary, hTarget, hVerify, hWhite], ...
        {'Verify gamut', 'Intersection triangle', 'Target T', 'Verify V', 'White W'}, ...
        'Location', 'best');

    ax2 = axes('Position', [0.62 0.10 0.34 0.82]);
    axis(ax2, 'off');
    hold(ax2, 'on');

    text(ax2, 0.00, 0.98, 'All color results', 'FontSize', 13, 'FontWeight', 'bold');
    text(ax2, 0.00, 0.94, ...
        sprintf('Highlighted target: #%d  T = (%.12f, %.12f, %.3f)', ...
        selectedIndex, targetX(selectedIndex), targetY(selectedIndex), targetL(selectedIndex)), ...
        'FontSize', 9, 'FontWeight', 'bold');

    rowsPerColumn = 15;
    leftX = 0.00;
    rightX = 0.50;
    topY = 0.90;
    rowStep = 0.056;

    for i = 1:numel(indexValue)
        if i <= rowsPerColumn
            xPos = leftX;
            yPos = topY - (i - 1) * rowStep;
        else
            xPos = rightX;
            yPos = topY - (i - rowsPerColumn - 1) * rowStep;
        end

        if isValid(i)
            lineText = sprintf([ ...
                '%02d  T(%.4f, %.4f, %.3f)\n', ...
                '    V(%.4f, %.4f, %.3f)  dUV=%s'], ...
                indexValue(i), targetX(i), targetY(i), targetL(i), ...
                verifyX(i), verifyY(i), verifyL(i), ...
                format_scientific_value(deltaUV(i), 3));
        else
            lineText = sprintf([ ...
                '%02d  T(%.4f, %.4f, %.3f)\n', ...
                '    invalid: %s'], ...
                indexValue(i), targetX(i), targetY(i), targetL(i), ...
                statusText{i});
        end

        if indexValue(i) == selectedIndex
            textColor = [0.75 0.00 0.00];
            fontWeight = 'bold';
        else
            textColor = [0 0 0];
            fontWeight = 'normal';
        end

        text(ax2, xPos, yPos, lineText, ...
            'Units', 'normalized', ...
            'FontName', 'Consolas', ...
            'FontSize', 8.5, ...
            'FontWeight', fontWeight, ...
            'Color', textColor, ...
            'VerticalAlignment', 'top');
    end
end
