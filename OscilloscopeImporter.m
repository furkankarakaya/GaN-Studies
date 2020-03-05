figure
hold all
Vsense400f = lowpass(Vsense400,15e6,5e9);
Vsense350f = lowpass(Vsense350,15e6,5e9);
Vsense300f = lowpass(Vsense300,15e6,5e9);
yyaxis left
ylabel('GS voltage (V)')
plot(Time,VgsBot300,'LineWidth',2);

% legend('300V','350V','400V')
yyaxis right
ylabel('Digital Signals')
plot(Time,SCTrig300,'LineWidth',2,'Color','g');
plot(Time,STO300,'LineWidth',2);
legend('GS Voltage','SC Input','STO Command')
%plot(time,Vsense150,'LineWidth',2);
%plot(time,Vsense200,'LineWidth',2);
% plot(time,Vtrig200,'LineWidth',2);
xlabel('time (s)')
title('SCP Timing')

