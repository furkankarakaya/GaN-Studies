
%Vsense100af = lowpass(Vsense100a,45e6,5e9);
to = 6300;
from = 1500;

figure
set(gcf, 'Position',  [100, 100, 600, 400])
hold all
%plot(2e-10*(from:to), VrcoutSCP350(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), VrefSCP300(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), VfiltoutSCP300(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VscpDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VocpDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VrefDPT350_2(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VandoutDPT350(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), VcompoutSCP300(from:to),'LineWidth',1.5)
% plot(2e-10*(from:to), VgsBotSCP300(from:to),'LineWidth',1.5)
% plot(2e-10*(from:to), V5VISODPT320(from:to),'LineWidth',1.5)
% plot(2e-10*(from:to), P5VISOSCP300(from:to),'LineWidth',1.5)
% plot(2e-10*(from:to), V5VISODPT340(from:to),'LineWidth',1.5)
% yyaxis left
% plot(2e-10*(from:to), CommonMCurr350_ES(from:to),'LineWidth',1.5)
% plot(2e-10*(from:to), CommonMCurr350_Adapter(from:to),'LineWidth',1.5)
ylabel('Voltage (V)')
% ylim([-0.4 1])
% yyaxis right
% plot(2e-10*(from:to), P5V_wSS_350(from:to),'LineWidth',1.5)
% ylim([0 6.5])
% ylabel('Voltage (V)')
%plot(2e-10*(from:to), V5VnfDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), V5VofDPT350(from:to),'LineWidth',1.5)
legend('Reference','Filter Output','Comparator Output','5V','best')
xlabel('time (\mus)')
title('SCP Waveforms under 300V')


