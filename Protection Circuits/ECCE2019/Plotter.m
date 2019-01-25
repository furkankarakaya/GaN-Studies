%% Cap Currents
fplot = figure;
figure(fplot)
plot(time,ACapCurr,time,BCapCurr,time,CCapCurr,'Linewidth',3.0);
ylim([-12 12]);
xlim([0 20]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.XTick = [0 5 10 15 20];
ax.YTick = [-10 -5 0 5 10];
legend('Phase A','Phase B','Phase C');
xlabel('Time(ms)','FontSize',12);
ylabel('Capacitor Current(A)','FontSize',12);

%% Cap Voltages
fplot2 = figure;
figure(fplot2)
hold all;
p1 = plot(time,ACapVolt,'Linewidth',3.0);
p2 = plot(time,BCapVolt,'Linewidth',3.0);
p3 = plot(time,CCapVolt,'Linewidth',3.0);
p1.Color(4) = 1;
p2.Color(4) = 1;
p3.Color(4) = 1;
ylim([268 273]);
xlim([0 20]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.XTick = [0 5 10 15 20];
ax.YTick = [268 269 270 271 272 273];
legend('Phase A','Phase B','Phase C');
xlabel('Time(ms)','FontSize',12);
ylabel('Capacitor Voltage(V)','FontSize',12);

%% Cap Curr with and without comm inductances
fplot2 = figure;
figure(fplot2)
hold all;
p1 = plot(time,ACapCurrWithCommInductances,'Linewidth',2.0);
p2 = plot(time,ACapCurrWithOnlyESL,'Linewidth',2.0);
p1.Color(4) = 1;
p2.Color(4) = 1;
ylim([-12 12]);
xlim([0 0.08]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.XTick = [0 0.02 0.04 0.06 0.08];
ax.YTick = [-10 -5 0 5 10];
legend('With Comm. Ind.','Without Comm. Ind.');
xlabel('Time(ms)','FontSize',12);
ylabel('Capacitor Current(A)','FontSize',12);

%% Cap Curr RMS with and without comm inductances
fplot2 = figure;
figure(fplot2)
hold all;
plot(time,ACapCurrRMSWithInd,'Linewidth',2.0);
plot(time,BCapCurrRMSWithInd,'Linewidth',2.0);
plot(time,CCapCurrRMSWithInd,'Linewidth',2.0);
plot(time,ACapCurrRMSWithoutInd,'Linewidth',2.0,'Color','g');

ylim([0 3]);
xlim([0 20]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.XTick = [0 5 10 15 20];
ax.YTick = [0 1 2 3];
legend('Phase A','Phase B','Phase C','Ideal','Location','best');
xlabel('Time(ms)','FontSize',12);
ylabel('Capacitor Current RMS(A)','FontSize',12);

%% Series Connected with Interlaving Voltages
fplot2 = figure;
figure(fplot2)
hold all;
plot(time,Module1DCVolt,'Linewidth',2.0);
plot(time,Module2DCVolt,'Linewidth',2.0);
plot(time,DCBusVoltage/2,'Linewidth',2.0,'Color','g');
ylim([268 273]);
xlim([0 0.05]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.YTick = [268 269 270 271 272 273];
ax.XTick = [0 0.01 0.02 0.03 0.04 0.05];
legend('Module S1','Module S2','DC Supply (x1/2)','Ideal','Location','best');
xlabel('Time(ms)','FontSize',12);
ylabel('Voltage (V)','FontSize',12);

%% Paralel Connected Module Current Waveforms
fplot2 = figure;
figure(fplot2)
hold all;
plot(time,Module1CurrentNoIndNoInt,'Linewidth',2.0,'Color','m');
plot(time,Module1CurrentNoIndYesInt,'Linewidth',2.0,'Color','r');
plot(time,Module1CurrentYesIndYesInt,'Linewidth',2.0,'Color','g');
plot(time,Module2CurrentYesIndYesInt,'Linewidth',2.0,'Color','b');


ylim([-5 25]);
xlim([0 0.05]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.YTick = [-5 0 5 10 15 20 25];
ax.XTick = [0 0.01 0.02 0.03 0.04 0.05];
legend('Mod. P1 - No Conn. Ind. and No Interlaving',...
    'Mod. P1 - No Conn. Ind. and With Interlaving',...
        'Mod. P1 - With Conn. Ind. and With Interlaving',...
        'Mod. P2 - With Conn. Ind. and With Interlaving','Location','best');
xlabel('Time(ms)','FontSize',12);
ylabel('Current (A)','FontSize',12);

%% Paralel Connected Module Voltage Waveforms
fplot2 = figure;
figure(fplot2)
hold all;
plot(time,DCBusVoltNoInt,'Linewidth',2.0);
plot(time,DCBusVoltYesInt,'Linewidth',2.0);
ylim([268 273]);
xlim([0 0.05]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.YTick = [268 269 270 271 272 273];
ax.XTick = [0 0.01 0.02 0.03 0.04 0.05];
legend('No Interleaving','With Interleaving','Ideal','Location','best');
xlabel('Time(ms)','FontSize',12);
ylabel('Voltage (V)','FontSize',12);

%% Paralel Connected Capacitor RMS Waveforms
fplot2 = figure;
figure(fplot2)
hold all;
plot(time,CurrRMS_YesConnInd_NoInt,'Linewidth',2.0,'Color','g');
plot(time,CurrRMS_YesConnInd_YesInt,'Linewidth',2.0,'Color','r');
plot(time,CurrRMS_NoConnIndYesComm_YesInt,'Linewidth',2.0,'Color','b');
plot(time,CurrRMS_YesConnIndNoComm_YesInt,'Linewidth',2.0,'Color','m');



ylim([0 3.1]);
xlim([0 20]);
ax = gca;
set(findobj(gcf,'type','axes'),'FontName','Arial','FontSize',12,'LineWidth', 1);
ax.YTick = [0 1 2 3];
ax.XTick = [0 5 10 15 20];
legend('With All Ind. and No Interleaving',...
    'With All Ind. and With Interlaving',...
        'No Connection Ind. and With Interlaving',...
        'No Commutation Ind. and With Interlaving','Location','best');
xlabel('Time(ms)','FontSize',12);
ylabel('Capacitor Current RMS (A)','FontSize',12);