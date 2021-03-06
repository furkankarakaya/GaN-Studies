%% Protection   
clear all
% Path Parameters
Lpath = 0.660973e-9;
C = 1e-9;
Rpath20 = 238e-6;
T = 25:5:150;
Rpath = Rpath20.*(1 + 0.004041.*(T-20));
%plot(T,Rpath)

% NTC Parameters
Rntc25 = 10e3;
B = 3380;
Rntc = Rntc25 .* exp(B.*((1./(T+273.5))-(1/(25+273.5))));
% plot(T,Rntc)

% Equivalent Resistance
i = 0;
Rmin = 1e3;
Rchange = 100;
for Rparallel = Rmin:Rchange:50e3
i= i + 1;
Rseries = Lpath/C/243e-6 - Rparallel*Rntc25./(Rparallel + Rntc25); % Rseries in 25C
Req = Rntc.*Rparallel./(Rntc+Rparallel) + Rseries;
Delta(i) = max(Req.*Rpath)-min(Req.*Rpath);
end

[DVal, index] = min(Delta);
Rparallel =  Rmin + index*Rchange;
Rseries = Lpath/C/243e-6 - Rparallel*Rntc25./(Rparallel + Rntc25); 
Req = Rntc.*Rparallel./(Rntc+Rparallel) + Rseries;

%% Bode Plots
Rseries = 1500;
Rparallel = 1400;
figure
hold on
for T = 25:50:125
if T <= 60
    B = 3380;
elseif T >= 95
    B = 3455;
else
    B = 3435;
end
Rpath = Rpath20.*(1 + 0.004041.*(T-20));
Rntc = Rntc25 .* exp(B.*((1./(T+273.5))-(1/(25+273.5))));
Rc = Rseries + Rparallel*Rntc/(Rntc+Rparallel);
G = tf([Lpath Rpath],[Lpath*C C*(Rpath + Rc) 1]);
bode(G)
end
set(findall(gcf,'type','line'),'linewidth',3)

% ylim([-65 -85]);
% xlim([0 20]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
% ax.XTick = [0 5 10 15 20];
% ax.YTick = [-10 -5 0 5 10];
legend('25{\circ}C','75{\circ}C','125{\circ}C','Location','best');
hold off

