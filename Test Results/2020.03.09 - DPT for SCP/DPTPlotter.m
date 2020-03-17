% clear VgsBotf
% clear LoadCurrentf
% clear VdsBotf

% VgsBotf = lowpass(VgsBot,45e6,5e9);
% VdsBotf = lowpass(VdsBot,45e6,5e9);
% LoadCurrentf = lowpass(LoadCurrent,500e6,5e9);
from = 1561;
to = 9040;
figure
set(gcf, 'Position',  [100, 100, 600, 400])

%Top plot
subplot(2,1,1);
hold all
plot(2e-4*(1:(to-from+1)),VgsBotDPTof(from:to),'LineWidth',1,'Color',[0.9290, 0.6940, 0.1250])
plot(2e-4*(1:(to-from+1)),VgsTopDPTof(from:to),'LineWidth',1,'Color',[0.4660, 0.6740, 0.1880]	)
ylabel('Voltage (V)')
legend('V_{gs,bot}','V_{gs,top}','Location','NorthEast')
xticks([0,0.25,0.5,0.75,1,1.25,1.5,2])
yticks([-8,-4,-0,4,8])
ylim([-8 8])
%Bot plot
subplot(2,1,2);
yyaxis left
plot(2e-4*(1:(-from+to+1)),VdsBotDPTof(from:to),'-','LineWidth',2,'Color',[0, 0.4470, 0.7410])
ylim([-40 400])
yticks([-100,-0,100,200,300,400])
ylabel('Voltage (V)')
yyaxis right
plot(2e-4*(1:(-from+to+1)),LoadCurrentDPTof(from:to),'LineWidth',2,'Color',[0.8500, 0.3250, 0.0980]	)
legend('V_{Load}','I_{Load}','Location','SouthEast')
ylabel('Current (A)')
xlabel('time (\mus)')
xticks([0,0.25,0.5,0.75,1,1.25,1.5,2])
%title('SC Mechanism Timing Waveform')

