function result = test_RGB_delta_uv()
%TEST_RGB_DELTA_UV Compare fixed-primary RGB->xyY result with target xy.

    here = fileparts(mfilename('fullpath'));
    oldDir = pwd;
    cleanup = onCleanup(@() cd(oldDir));
    cd(here);

    rgbList = [ ...
        175 255  64;
        138 226  24;
        255   3   1;
        255  88   4;
        255 178   5;
        255 198   0;
        182 255   4;
        149 255   3;
        114 255   3;
         78 255   3;
         59 255   3;
          6 255   2;
         24 255  27;
         70 255  17;
         19 255  60;
          3 255  43;
          1 255 125;
         32 255 200;
         20 146 255;
          9  65 255;
          1  20 255;
         44  25 255;
         63  38 247;
        142  60 255;
        188  30 255;
        255  32 244;
        255   8  42;
        255  34  19;
        255  79  19;
        255 138  42];

    targetXY = [ ...
        0.3180 0.3180;
        0.3540 0.4100;
        0.6780 0.3090;
        0.5720 0.3720;
        0.5060 0.4220;
        0.5110 0.4510;
        0.4250 0.4900;
        0.4020 0.5130;
        0.3680 0.5370;
        0.3240 0.5690;
        0.2960 0.5890;
        0.1860 0.6790;
        0.2050 0.4550;
        0.2850 0.4840;
        0.1830 0.3310;
        0.1650 0.3890;
        0.1560 0.2180;
        0.1720 0.1660;
        0.1620 0.1000;
        0.1560 0.0630;
        0.1510 0.0400;
        0.1740 0.0538;
        0.1850 0.0650;
        0.2190 0.0900;
        0.2390 0.0880;
        0.2670 0.1030;
        0.4820 0.2110;
        0.5470 0.2810;
        0.5170 0.3160;
        0.4270 0.3000];

    xyYCalc = RGB_to_xyY(rgbList);
    calcUV = xy_to_uv(xyYCalc(:, 1:2));
    targetUV = xy_to_uv(targetXY);

    deltaUV2 = sum((calcUV - targetUV).^2, 2);
    deltaUV = sqrt(deltaUV2);

    result = table( ...
        (1:size(rgbList, 1)).', ...
        rgbList(:, 1), rgbList(:, 2), rgbList(:, 3), ...
        xyYCalc(:, 1), xyYCalc(:, 2), xyYCalc(:, 3), ...
        targetXY(:, 1), targetXY(:, 2), ...
        calcUV(:, 1), calcUV(:, 2), ...
        targetUV(:, 1), targetUV(:, 2), ...
        deltaUV2, deltaUV, ...
        'VariableNames', { ...
            '序号', '输入R', '输入G', '输入B', ...
            '计算x', '计算y', '计算Y', ...
            '目标x', '目标y', ...
            '计算u', '计算v', ...
            '目标u', '目标v', ...
            'DeltaUV平方', 'DeltaUV'});

    disp(result);

    [maxDeltaUV, maxIndex] = max(deltaUV);
    fprintf('max delta_uv = %.10f, index = %d\n', maxDeltaUV, maxIndex);
    fprintf('mean delta_uv = %.10f\n', mean(deltaUV));
end

function uv = xy_to_uv(xy)
    x = xy(:, 1);
    y = xy(:, 2);
    denominator = -2 * x + 12 * y + 3;
    uv = [4 * x ./ denominator, 9 * y ./ denominator];
end
