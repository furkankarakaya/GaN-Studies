
%Vsense100af = lowpass(Vsense100a,45e6,5e9);
% Vsense100bf = lowpass(Vsense100b,15e6,5e9);
%Vsense400f = lowpass(Vsense400,45e6,5e9);
%Vsense300f = lowpass(Vsense300,45e6,5e9);
%Vsense200f = lowpass(Vsense200,45e6,5e9);
Vcompisof = lowpass(Vcompiso,45e6,5e9);
Vcompoutf = lowpass(Vcompout,1e6,5e9);
Vdspsoftf = lowpass(Vdspsoft,45e6,5e9);
VgsBotScpf = lowpass(VgsBotScp,45e6,5e9);
Visobjtf = lowpass(Visobjt,45e6,5e9);
Vsense350f = lowpass(Vsense350,1e6,5e9);
threshold = 3.2*ones(1,(6061-2750+1),'uint16');

figure
hold all
%yyaxis left
% plot(Time,Vcompisof,'LineWidth',2);
plot(2e-4*(1:(6061-2750+1)),VgsBotScpf(2750:6061),'LineWidth',2,'Color','r');
plot(2e-4*(1:(6061-2750+1)),Vsense350f(2750:6061),'LineWidth',2,'Color','b');
% plot(Time,Vdspsoftf,'LineWidth',2);
plot(2e-4*(1:(6061-2750+1)),threshold','LineWidth',2,'Color','k');
% plot(Time,Visobjtf,'LineWidth',2);
plot(2e-4*(1:(6061-2750+1)),Vcompoutf(2750:6061),'LineWidth',2,'Color','g');
plot(2e-4*(1:(6061-2750+1)),VgsBotOriginal(2750:6061),'LineWidth',2);

% plot(Time,VgsTop_200ns,'LineWidth',2);
%ylabel('Vgs Voltage (V)')
legend('Vgs','Induced voltage','Threshold','Comparator output','Location','best')
%yyaxis right
ylabel('Voltage (V)')
%plot(Time,Vsense400_200ns,'LineWidth',2,'Color','r');
%plot(Time,Vsense400f,'LineWidth',2,'Color','g');
% plot(Time,Vsense200f,'LineWidth',2,'Color','k');
% legend('Vgs','200ns','100ns')
%plot(time,Vsense150,'LineWidth',2);
%plot(time,Vsense200,'LineWidth',2);
% plot(time,Vtrig200,'LineWidth',2);
xlabel('time (\mus)')
title('SC Mechanism Timing Waveform')

