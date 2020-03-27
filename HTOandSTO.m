from = 2500;
to = 7000;
figure
set(gcf, 'Position',  [100, 100, 600, 400])

hold all
plot(2e-4*(1:(to-from+1)),VdsHTO(from:to),'LineWidth',1,'Color',[0.9290, 0.6940, 0.1250])
plot(2e-4*(1:(to-from+1)),VdsSTO(from:to),'LineWidth',1,'Color',[0.4660, 0.6740, 0.1880])
ylabel('Voltage (V)')
xlabel('Time (\mus)')
legend('Hard Turn-Off','Soft Turn-Off','Location','NorthEast')
xlim([0 0.9])
