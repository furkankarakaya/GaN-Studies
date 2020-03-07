figure
hold all
%Vsense100af = lowpass(Vsense100a,45e6,5e9);
% Vsense100bf = lowpass(Vsense100b,15e6,5e9);
Vsense400f = lowpass(Vsense400,45e6,5e9);
%Vsense300f = lowpass(Vsense300,45e6,5e9);
%Vsense200f = lowpass(Vsense200,45e6,5e9);
yyaxis left
plot(Time,VgsBot_200ns,'LineWidth',2);
% plot(Time,VgsTop_200ns,'LineWidth',2);
ylabel('Vgs Voltage (V)')
% legend('Bot Switch','Top Switch')
yyaxis right
ylabel('Induced Voltage (V)')
plot(Time,Vsense400_200ns,'LineWidth',2,'Color','r');
plot(Time,Vsense400f,'LineWidth',2,'Color','g');
% plot(Time,Vsense200f,'LineWidth',2,'Color','k');
legend('Vgs','200ns','100ns')
%plot(time,Vsense150,'LineWidth',2);
%plot(time,Vsense200,'LineWidth',2);
% plot(time,Vtrig200,'LineWidth',2);
xlabel('time (s)')
title('Induced voltages for different SC durations under 400V')

