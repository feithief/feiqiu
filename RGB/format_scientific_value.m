function textValue = format_scientific_value(value, precision, mode)
%FORMAT_SCIENTIFIC_VALUE 将数值格式化为 a × 10^{b} 的科学计数法字符串。
%
% 用法：
%   textValue = format_scientific_value(value)
%   textValue = format_scientific_value(value, precision)
%   textValue = format_scientific_value(value, precision, mode)
%
% 输入：
%   value     : 标量数值
%   precision : 尾数保留小数位，默认 4
%   mode      : 'plain' 或 'latex'
%
% 输出示例：
%   plain : 5.5511 × 10^{-17}
%   latex : 5.5511 \times 10^{-17}

    if nargin < 2 || isempty(precision)
        precision = 4;
    end
    if nargin < 3 || isempty(mode)
        mode = 'plain';
    end

    if isnan(value)
        textValue = 'NaN';
        return;
    end

    if isinf(value)
        if value > 0
            textValue = 'Inf';
        else
            textValue = '-Inf';
        end
        return;
    end

    if value == 0
        mantissa = 0;
        exponent = 0;
    else
        exponent = floor(log10(abs(value)));
        mantissa = value / (10 ^ exponent);
    end

    switch lower(mode)
        case 'latex'
            textValue = sprintf(['%0.', num2str(precision), 'f \\times 10^{%d}'], ...
                mantissa, exponent);
        otherwise
            textValue = sprintf(['%0.', num2str(precision), 'f × 10^{%d}'], ...
                mantissa, exponent);
    end
end
