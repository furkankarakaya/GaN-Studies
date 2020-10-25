r = 0.1778e-3/2;
d = 10.135e-3;
alpha = 0;
phi=0;

fun = @(theta,phi) (r*r*cos(phi-theta))./(sqrt(r^2+r^2-2*r*r*cos(phi-theta)+d^2));
q=integral2(fun,0,2*pi,0,2*pi);
q = q*1e-7;

total = q*15e-3/(2*r);
total = total*1e12*84*85/2

sum=0;
for i=0:83
for delta_yx=0:15e-3/(2*r):15e-3
    delta_y=delta_yx-15e-3/(2*r)*i;
num= @(theta) (r*r*(sin(theta)*sin(phi)+cos(alpha)*cos(theta)*cos(phi))) ...
    / (sqrt((r*cos(theta)-r*cos(phi))^2+(r*sin(theta)-r*sin(phi)*cos(alpha)-delta_y)^2+(r*sin(phi)*sin(alpha)-d)^2));

q=integral2(num,theta,[0,2*pi]);

sum =sum+q;
end
end