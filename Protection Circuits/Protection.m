% Protection   

Rpath = 0.243e-3;
Lpath = 1.11e-9;
C = 1e-9;
% Rc = 1e3;

hold on
for Rc = 880:3620:4500
G = tf([-1*Lpath -1*Rpath],[Lpath*C C*(Rpath + Rc) 1]);
bode(G)
end
legend('880 ohm','4.5 kohm')
hold off


