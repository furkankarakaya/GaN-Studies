% Protection   

Rpath = 0.243e-3;
Lpath = 1.11e-9;
C = 1e-9;
Rc = 4.5e3;

G = tf([Lpath Rpath],[Lpath*C C*(Rpath + Rc) 1]);
bode(G)
hold off


