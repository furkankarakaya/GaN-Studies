clear VgsBotf
clear LoadCurrentf
clear VdsBotf

VgsBotf = lowpass(VgsBot,45e6,5e9);
VdsBotf = lowpass(VdsBot,45e6,5e9);
LoadCurrentf = lowpass(LoadCurrent,500e6,5e9);
from = 1561;
to = 9040;
figure
set(gcf, 'Position',  [100, 100, 600, 400])
hold all
yyaxis left
ylabel('Voltage (V)')
plot(2e-4*(1:(to-from+1)),VgsBotf(from:to),'LineWidth',2,'Color',[0, 0.4470, 0.7410])
%plot(Time*1e6,VgsBot,'LineWidth',2,'Color',[0.9290, 0.6940, 0.1250])
plot(2e-4*(1:(-from+to+1)),VdsBotf(from:to)/50,'-','LineWidth',2,'Color',[0.9290, 0.6940, 0.1250])
yyaxis right
plot(2e-4*(1:(-from+to+1)),LoadCurrentf(from:to),'LineWidth',2,'Color',[0.8500, 0.3250, 0.0980])
%plot(Time*1e6,LoadCurrent,'LineWidth',2,'Color','k')
legend('Vgs','Vsw (x1/50)','Load Current','Location','SouthEast','Orientation','horizontal')
ylabel('Current (A)')
xlabel('time (\mus)')
%title('SC Mechanism Timing Waveform')

