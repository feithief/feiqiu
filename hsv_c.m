
% 单个值
[R,G,B] = hsv2rgb_cstyle(0.5, 1.0, 1.0);     % 纯青 -> [0 1 1]

% 矩阵（尺寸一致）
h = [0.00 0.33 0.67; 0.10 0.50 0.90];
s = ones(size(h));
v = ones(size(h));
[R,G,B] = hsv2rgb_cstyle(h,s,v);
disp([R(:) G(:) B(:)]);



function [R, G, B] = hsv2rgb_cstyle(H, S, V)
%HSV2RGB_CSTYLE  C-like (loop-based) HSV to RGB for MATLAB
%   H,S,V in [0,1], size(H)==size(S)==size(V)
%   R,G,B in [0,1], same size as inputs

    % Input checks
    if ~isequal(size(H), size(S), size(V))
        error('Input size mismatch: H, S, V must be the same size.');
    end

    H = min(max(H,0),1);
    S = min(max(S,0),1);
    V = min(max(V,0),1);

    R = zeros(size(H));
    G = zeros(size(H));
    B = zeros(size(H));

    % Flatten for a simple loop, then reshape back
    Hf = H(:); Sf = S(:); Vf = V(:);
    Rf = R(:); Gf = G(:); Bf = B(:);

    for i = 1:numel(Hf)
        h = Hf(i); s = Sf(i); v = Vf(i);

        % Grayscale shortcut
        if s == 0
            Rf(i) = v; Gf(i) = v; Bf(i) = v;
            continue;
        end

        % Core sector logic (same as C)
        h6 = h * 6.0;
        k  = floor(h6);
        if k == 6, k = 0; end

        p  = h6 - k;            % fractional part
        t  = 1.0 - s;           % t = 1 - s
        n  = 1.0 - s * p;       % n = 1 - s*p
        pp = 1.0 - s*(1.0 - p); % pp = 1 - s*(1-p)

        rr = 0; gg = 0; bb = 0;
        switch k
            case 0
                rr = 1.0; gg = pp;  bb = t;
            case 1
                rr = n;   gg = 1.0; bb = t;
            case 2
                rr = t;   gg = 1.0; bb = pp;
            case 3
                rr = t;   gg = n;   bb = 1.0;
            case 4
                rr = pp;  gg = t;   bb = 1.0;
            otherwise % case 5
                rr = 1.0; gg = t;   bb = n;
        end

        m = max([rr, gg, bb]);
        if m > 0
            f = v / m;
            rr = rr * f; gg = gg * f; bb = bb * f;
        else
            rr = 0; gg = 0; bb = 0;
        end

        % Clamp
        rr = min(max(rr,0),1);
        gg = min(max(gg,0),1);
        bb = min(max(bb,0),1);

        Rf(i) = rr; Gf(i) = gg; Bf(i) = bb;
    end

    % Reshape back
    R = reshape(Rf, size(H));
    G = reshape(Gf, size(H));
    B = reshape(Bf, size(H));
end
