function [pointXY, rayT, edgeName, edgeIndex] = find_gamut_intersection(whiteXY, targetXY, intersectionTriangle)
%FIND_GAMUT_INTERSECTION Find the ray intersection with a configurable triangle.
% Ray:
%   pointXY = whiteXY + rayT * (targetXY - whiteXY)
%
% Requirement:
%   rayT >= 1, so the intersection lies beyond targetXY on the same ray.

    if nargin < 3
        intersectionTriangle = [];
    end

    triangleXY = normalize_intersection_triangle(intersectionTriangle);
    edgeNames = {'R-G'; 'G-B'; 'B-R'};

    whiteXY = double(whiteXY(:).');
    targetXY = double(targetXY(:).');
    direction = targetXY - whiteXY;

    if norm(direction) <= eps
        error('find_gamut_intersection:SamePoint', ...
            'whiteXY and targetXY are the same point.');
    end

    tol = 1e-12;
    rayT = Inf;
    edgeIndex = NaN;
    pointXY = [NaN NaN];

    for i = 1:3
        edgeStart = triangleXY(i, :);
        edgeEnd = triangleXY(mod(i, 3) + 1, :);
        edgeVector = edgeEnd - edgeStart;

        systemMatrix = [direction(:), -edgeVector(:)];
        if abs(det(systemMatrix)) < tol
            continue;
        end

        params = systemMatrix \ (edgeStart - whiteXY).';
        t = params(1);
        s = params(2);

        if t >= 1 - tol && s >= -tol && s <= 1 + tol && t < rayT
            rayT = t;
            edgeIndex = i;
            pointXY = whiteXY + t * direction;
        end
    end

    if ~isfinite(rayT)
        error('find_gamut_intersection:NoIntersection', ...
            'The ray does not intersect the configured gamut triangle after targetXY.');
    end

    edgeName = edgeNames{edgeIndex};
end
