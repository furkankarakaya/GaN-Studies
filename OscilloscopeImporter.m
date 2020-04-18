


figure
set(gcf, 'Position',  [100, 100, 600, 400])
hold all
plot(time,S5V)
plot(time,DSPSCTrig)
ylabel('Voltage (V)')
%ylim([-0.4 1])
yyaxis right
plot(time,LoadCurrent)
%ylim([0 6.5])
ylabel('Current (A)')
%plot(2e-10*(from:to), V5VnfDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), V5VofDPT350(from:to),'LineWidth',1.5)
legend('S5V','DSP SC Input','Load Current','Location','best')
xlabel('time (\mus)')
title('All isolated converters desoldered except gate drivers, 350V')


