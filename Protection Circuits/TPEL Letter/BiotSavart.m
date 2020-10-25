ymin = 0;
ymax = 15e-3;
ysteplength = (ymax-ymin)/1000;

xmin = 0;
xmax = 0.1778e-3;
xsteplength = (xmax-xmin)/1000;

z_p = 10.135e-3;

simpmu0 = 1e-7;
%BiotSavart = @(y) sqrt(Rkare)/sqrt(Rkare + (y_p-y)^2);

Bsum = 0;
for x_p = xmin:xsteplength:xmax
    Rkare = x_p^2 + z_p^2;
for y_p = ymin:ysteplength:ymax
for y_s = ymin:ysteplength:ymax

    Bp = simpmu0*sqrt(Rkare)/sqrt(Rkare + (y_p-y_s)^2)*ysteplength;
    Bsum = Bp + Bsum;

end
end
end