
%Vsense100af = lowpass(Vsense100a,45e6,5e9);
to = 9937;
from = 1500;

figure
set(gcf, 'Position',  [100, 100, 600, 400])
hold all
%plot(2e-10*(from:to), VrcoutSCP350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VrefDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VrcoutDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VscpDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VocpDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VrefDPT350_2(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VandoutDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), VcompoutDPT350(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), V5VISODPT320(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), V5VISODPT330(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), V5VISODPT340(from:to),'LineWidth',1.5)
plot(2e-10*(from:to), V5VISODPT350_2(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), V5VnfDPT350(from:to),'LineWidth',1.5)
%plot(2e-10*(from:to), V5VofDPT350(from:to),'LineWidth',1.5)
legend('320V','330V','340V','350V','Location','best')
ylim([0 6.5])
ylabel('Voltage (V)')
xlabel('time (\mus)')
title('Isolated 5V supply under different DC voltages')


