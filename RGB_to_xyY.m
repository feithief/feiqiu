function xyY = RGB_to_xyY(RGB,  Rx,Ry, RY,  Gx,Gy, GY,  Bx, By,BY)
    xyY(1) = (RY/Ry*Rx*RGB(1)+GY/Gy*Gx*RGB(2)+BY/By*Bx*RGB(3))/(RY/Ry*RGB(1)+GY/Gy*RGB(2)+BY/By*RGB(3));
    xyY(2) = (RY*RGB(1) + GY*RGB(2) + BY*RGB(3))/(RY/Ry*RGB(1)+GY/Gy*RGB(2)+BY/By*RGB(3));
    xyY(3) = (RY*RGB(1) + GY*RGB(2) + BY*RGB(3))/255*2.175543885971493;
end