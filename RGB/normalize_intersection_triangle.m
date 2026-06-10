function triangleXY = normalize_intersection_triangle(intersectionTriangle)
%NORMALIZE_INTERSECTION_TRIANGLE Normalize the geometric intersection triangle.
% Supported input:
%   3x2 : [x y] for R/G/B
%   3x3 : [x y Y] for R/G/B, only the first two columns are used

    if nargin < 1 || isempty(intersectionTriangle)
        [~, ~, ~, ~, ~, primaries] = fixed_rgb_primaries();
        triangleXY = primaries(:, 1:2);
        return;
    end

    intersectionTriangle = double(intersectionTriangle);

    if isequal(size(intersectionTriangle), [3 2])
        triangleXY = intersectionTriangle;
    elseif isequal(size(intersectionTriangle), [3 3])
        triangleXY = intersectionTriangle(:, 1:2);
    else
        error('normalize_intersection_triangle:InvalidTriangle', ...
            'The intersection triangle must be a 3x2 [x y] matrix or a 3x3 [x y Y] matrix.');
    end
end
