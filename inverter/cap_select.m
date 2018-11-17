ma = 0.9;
Is = 12;
fsw = 50e3;
Vdcrip = 1;
pf = 0.9;

Cdcreq = 1e6*(ma*Is/(16*fsw*Vdcrip))*...
    sqrt( (6 - (96*sqrt(3)*ma)/(5*pi) +...
    (9*ma^2/2) )*pf^2 + (8*sqrt(3)*ma)/(5*pi) )


Idcrms = Is*sqrt( 2*ma*(sqrt(3)/(4*pi) +...
    pf^2*(sqrt(3)/pi-9*ma/16)) )

