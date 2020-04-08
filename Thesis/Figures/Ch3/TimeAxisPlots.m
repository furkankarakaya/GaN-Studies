%% Time Axis Plots
%% Model 1 - 2 - 3
load('Model120A125C10and1ohm.mat');
load('Model220A125C10and1ohm.mat');
load('Model320A125C10and1ohm.mat');
%% Model 1 Plots
%Bottom Switch
getElement(Model120A125C10and1ohm,'IdsB_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model120A125C10and1ohm,'VdsB_O');
DSVoltage = ans.Values.Data;
clear ans;
getElement(Model120A125C10and1ohm,'IdsB_I');
CHCurrent = ans.Values.Data;
clear ans;
getElement(Model120A125C10and1ohm,'VdsB_I');
CHVoltage = ans.Values.Data;
clear ans;
SampleTime = 5e-13;
% Bottom Turn ON
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f1 = figure('Name','Bottom Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1)
figure('Position', [10 10 900 600])
% f1.Position = [100, 100, 600, 400];
 %set(gca, 'Position',  [100, 100, 600, 400])
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.04]);
ylim([-5 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.04];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off
% Bottom Turn OFF
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f1 = figure('Name','Bottom Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1)
figure('Position', [10 10 900 600])
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.06 0.15]);
ylim([-5 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.03:0.15];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%Top Switch
getElement(Model120A125C10and1ohm,'IdsT_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model120A125C10and1ohm,'VdsT_O');
DSVoltage = downsample(ans.Values.Data,2);
clear ans;
getElement(Model120A125C10and1ohm,'IdsT_I');
CHCurrent = downsample(ans.Values.Data,2);
clear ans;
getElement(Model120A125C10and1ohm,'VdsT_I');
CHVoltage = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
% Top Turn ON
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f1 = figure('Name','Top Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1)
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.06 0.15]);
ylim([-25 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.03:0.15];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

% Top Turn OFF
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f1 = figure('Name','Top Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1)
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.04]);
ylim([-25 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.04];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%% Model 2 Plots
%Bottom Switch
getElement(Model220A125C10and1ohm,'IdsB_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model220A125C10and1ohm,'VdsB_O');
DSVoltage = ans.Values.Data;
clear ans;
getElement(Model220A125C10and1ohm,'IdsB_I');
CHCurrent = ans.Values.Data;
clear ans;
getElement(Model220A125C10and1ohm,'VdsB_I');
CHVoltage = ans.Values.Data;
clear ans;
SampleTime = 5e-13;
% Bottom Turn ON
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f5 = figure('Name','Bottom Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.06]);
ylim([-5 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.06];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off
% Bottom Turn OFF
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f6 = figure('Name','Bottom Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f6);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.06 0.15]);
ylim([-5 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.03:0.15];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%Top Switch
getElement(Model220A125C10and1ohm,'IdsT_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model220A125C10and1ohm,'VdsT_O');
DSVoltage = downsample(ans.Values.Data,2);
clear ans;
getElement(Model220A125C10and1ohm,'IdsT_I');
CHCurrent = downsample(ans.Values.Data,2);
clear ans;
getElement(Model220A125C10and1ohm,'VdsT_I');
CHVoltage = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
% Top Turn ON
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f1 = figure('Name','Top Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.06 0.15]);
ylim([-25 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.03:0.15];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

% Top Turn OFF
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f1 = figure('Name','Top Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.06]);
ylim([-25 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.06];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%% Model 3 Plots
%Bottom Switch
getElement(Model320A125C10and1ohm,'IdsB_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model320A125C10and1ohm,'VdsB_O');
DSVoltage = ans.Values.Data;
clear ans;
getElement(Model320A125C10and1ohm,'IdsB_I');
CHCurrent = ans.Values.Data;
clear ans;
getElement(Model320A125C10and1ohm,'VdsB_I');
CHVoltage = ans.Values.Data;
clear ans;
SampleTime = 5e-13;
% Bottom Turn ON
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f5 = figure('Name','Bottom Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.1]);
ylim([-5 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.02:0.1];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off
% Bottom Turn OFF
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f6 = figure('Name','Bottom Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f6);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.09 0.3]);
ylim([-5 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.05:0.3];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%Top Switch
getElement(Model320A125C10and1ohm,'IdsT_O');
DSCurrent = ans.Values.Data;
clear ans;
getElement(Model320A125C10and1ohm,'VdsT_O');
DSVoltage = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and1ohm,'IdsT_I');
CHCurrent = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and1ohm,'VdsT_I');
CHVoltage = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
% Top Turn ON
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f1 = figure('Name','Top Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0.09 0.3]);
ylim([-25 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.05:0.3];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

% Top Turn OFF
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f1 = figure('Name','Top Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f1);
figure('Position', [10 10 900 600]) 
hold all
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*CHVoltage(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, CHCurrent(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
xlim([0 0.1]);
ylim([-25 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.02:0.1];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('V_d_s(1/10)','I_d_s','V_c_h(1/10)','I_c_h','northeast');
hold off

%% Ron Roff Plots
clear all
load('Model320A125C10and1ohm.mat');
load('Model320A125C20and1ohm.mat');
load('Model320A125C30and1ohm.mat');
load('Model320A125C10and5ohm.mat');
load('Model320A125C10and10ohm.mat');
%% Ron
% Bottom Switch
getElement(Model320A125C10and1ohm,'IdsB_O');
DSCurrentR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and1ohm,'VdsB_O');
DSVoltageR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C20and1ohm,'IdsB_O');
DSCurrentR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C20and1ohm,'VdsB_O');
DSVoltageR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C30and1ohm,'IdsB_O');
DSCurrentR3 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C30and1ohm,'VdsB_O');
DSVoltageR3 = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
% Bottom Turn ON
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f5 = figure('Name','Bottom Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
h1 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
h2 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
h3 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','b','Linewidth',2.0);
set(get(get(h1, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h2, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h3, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
xlim([0 0.1]);
ylim([-5 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.02:0.1];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('R_o_n = 10 ohm','R_o_n = 20 ohm','R_o_n = 30 ohm','none','northeast');
hold off
% Top Turn ON
clear DSCurrentR1;
clear DSVotlageR1;
clear DSCurrentR2;
clear DSVotlageR2;
clear DSCurrentR3;
clear DSVotlageR3;
getElement(Model320A125C10and1ohm,'IdsT_O');
DSCurrentR1 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C10and1ohm,'VdsT_O');
DSVoltageR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C20and1ohm,'IdsT_O');
DSCurrentR2 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C20and1ohm,'VdsT_O');
DSVoltageR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C30and1ohm,'IdsT_O');
DSCurrentR3 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C30and1ohm,'VdsT_O');
DSVoltageR3 = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f5 = figure('Name','Top Switch Turn ON','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
h1 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
h2 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
h3 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','b','Linewidth',2.0);
set(get(get(h1, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h2, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h3, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
xlim([0.09 0.12]);
ylim([-25 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.12];
ax.YTick = [-20:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('R_o_n = 10 ohm','R_o_n = 20 ohm','R_o_n = 30 ohm','none','northeast');
hold off
%% Roff Plot
clear DSCurrentR1;
clear DSVotlageR1;
clear DSCurrentR2;
clear DSVotlageR2;
clear DSCurrentR3;
clear DSVotlageR3;
getElement(Model320A125C10and1ohm,'IdsB_O');
DSCurrentR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and1ohm,'VdsB_O');
DSVoltageR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and5ohm,'IdsB_O');
DSCurrentR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and5ohm,'VdsB_O');
DSVoltageR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and10ohm,'IdsB_O');
DSCurrentR3 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C10and10ohm,'VdsB_O');
DSVoltageR3 = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
FirstDataBeginIndex = 9e-7/SampleTime;
FirstDataEndIndex = 12e-7/SampleTime;
f5 = figure('Name','Bot Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
h1 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
h2 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
h3 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','b','Linewidth',2.0);
set(get(get(h1, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h2, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h3, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
xlim([0.09 0.12]);
ylim([-5 45]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.01:0.12];
ax.YTick = [0:10:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('R_o_f_f = 1 ohm','R_o_f_f = 5 ohm','R_o_f_f = 10 ohm','none','northeast');
hold off
clear DSCurrentR1;
clear DSVotlageR1;
clear DSCurrentR2;
clear DSVotlageR2;
clear DSCurrentR3;
clear DSVotlageR3;
getElement(Model320A125C10and1ohm,'IdsT_O');
DSCurrentR1 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C10and1ohm,'VdsT_O');
DSVoltageR1 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C20and1ohm,'IdsT_O');
DSCurrentR2 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C20and1ohm,'VdsT_O');
DSVoltageR2 = downsample(ans.Values.Data,2);
clear ans;
getElement(Model320A125C30and1ohm,'IdsT_O');
DSCurrentR3 = downsample(ans.Values.Data,1);
clear ans;
getElement(Model320A125C30and1ohm,'VdsT_O');
DSVoltageR3 = downsample(ans.Values.Data,2);
clear ans;
SampleTime = 1e-12;
FirstDataBeginIndex = 4.9e-7/SampleTime;
FirstDataEndIndex = 6e-7/SampleTime;
f5 = figure('Name','Top Switch Turn OFF','units','normalized','outerposition',[1/4 1/4 1/2 1/2]);
figure(f5);
figure('Position', [10 10 900 600]) 
hold all
h1 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR1(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'Color','b','Linewidth',2.0);
h2 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR2(FirstDataBeginIndex:FirstDataEndIndex)...
     ,'--','Color','b','Linewidth',2.0);
h3 = plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, 0.1*DSVoltageR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','r','Linewidth',2.0);
plot((0:FirstDataEndIndex-FirstDataBeginIndex)*SampleTime*1e6, DSCurrentR3(FirstDataBeginIndex:FirstDataEndIndex)...
     ,':','Color','b','Linewidth',2.0);
set(get(get(h1, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h2, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
set(get(get(h3, 'Annotation'), 'LegendInformation'), 'IconDisplayStyle', 'off');
xlim([0 0.1]);
ylim([-25 50]);
ax = gca;
ax.FontSize = 22;
ax.XTick = [0:0.02:0.1];
ax.YTick = [-20:20:50];
grid off;
xlabel('Time(\musec)','FontSize',22,'FontWeight','bold','Color','k');
ylabel('Current(A), Voltage(V)','FontSize',22,'FontWeight','bold','Color','k');
% title({'I_d_s vs V_d_s Curve of Top Switch During Turn ON'},'FontSize',22,'FontWeight','bold','Color','k')
legend ('R_o_f_f = 1 ohm','R_o_f_f = 5 ohm','R_o_f_f = 10 ohm','none','northeast');
hold off