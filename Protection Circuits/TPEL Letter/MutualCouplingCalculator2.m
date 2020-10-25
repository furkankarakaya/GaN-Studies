%https://www.fhwa.dot.gov/publications/research/operations/its/06139/appenda.cfm
A = 15e-3;
B = 1.4e-3;
H = 10.135e-3;

M13 = mutual(A,sqrt(B^2+H^2));
M11 = mutual(A,H);
M24 = mutual(B,sqrt(A^2+H^2));
M22 = mutual(B,H);

Mtotal = abs(-2*(M13-M11+M24-M22))/1e-9 %nH

function M = mutual(t,d)
    mu0 = 4*pi*1e-7;
    M = mu0*t/(2*pi)*(log((t/d)+sqrt(1+(t/d)^2))-1 + sqrt((d/t)^2 + d/t));
end
