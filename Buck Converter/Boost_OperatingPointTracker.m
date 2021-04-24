%% Column Definitions
GainCol = 9; %Operating Gain
PowerCol = 7; %Operating Power
FreqCol = 8; % Operating Frequency
GaNCondCol = 1; %GaN Conduction Loss
GaNSWCol = 2; % GaN Switching Loss
IndCondCol = 3; % Inductor Conduction Loss
IndSWCol = 4; % Inductor Switching Loss
CapLossCol = 5; % Capacitor total loss
ZVSCol = 6; % If ZVS (1) or Not (0)

%% Initial Definitions 
Vout = 400;
L = 6.8e-6;
freq = 100e3:10e3:1.5e6;
gain_min = 0;
gain_step = 0.01;
gain_max = 1;
gain_number = int16((gain_max - gain_min)/gain_step + 1);
power_min = 0;
power_step = 20;
power_max = 6000;
power_number = (power_max - power_min)/power_step + 1;
[~,size_freq] = size(freq);
results = zeros( gain_number*power_number, ZVSCol);

%% Calculate
ind = 1;
for power = power_min:power_step:power_max
    for gain = gain_min:gain_step:gain_max
        %
        Vin = Vout*gain;
        Iin = power/Vin;
        ILavg = Iin/2;
        duty = 1 - gain;
        %
        results(ind,GainCol) = gain;
        results(ind,PowerCol) = power;
        freq_opt_res = zeros(size_freq,ZVSCol+1);
        for freq_ind = 1:size_freq
            %
            ILpp = Vin*(1-gain)/(L*freq(freq_ind));
            %
            ZVSFlag = ZVSCheck(ILavg,ILpp);
            freq_opt_res(freq_ind,ZVSCol) = ZVSFlag;
            [freq_opt_res(freq_ind,GaNCondCol), freq_opt_res(freq_ind,GaNSWCol)]...
                = GaNLossCalculate(ILavg,ILpp,freq(freq_ind),ZVSFlag);
            [freq_opt_res(freq_ind,IndCondCol), freq_opt_res(freq_ind,IndSWCol)]...
                = IndLossCalculate(ILavg,ILpp,freq(freq_ind));
            freq_opt_res(freq_ind,CapLossCol) = CapLossCalculate(ILavg, ILpp, freq(freq_ind), gain);
            freq_opt_res(freq_ind,ZVSCol+1) = sum(freq_opt_res(freq_ind,:)) - ZVSFlag;
        end
        [~,min_index] = min(freq_opt_res(:,ZVSCol+1));
        results(ind,GaNCondCol) = freq_opt_res(min_index, GaNCondCol);
        results(ind,GaNSWCol) = freq_opt_res(min_index, GaNSWCol);
        results(ind,IndCondCol) = freq_opt_res(min_index, IndCondCol);
        results(ind,IndSWCol) = freq_opt_res(min_index, IndSWCol);
        results(ind,CapLossCol) = freq_opt_res(min_index, CapLossCol);
        results(ind,ZVSCol) = freq_opt_res(min_index, ZVSCol);
        results(ind,FreqCol) = 100e3 + 10e3*(min_index-1);
        clear freq_opt_res;
        ind = ind + 1;
    end
end
ind = 1;
for i=1:power_number
    for j=1:gain_number
        
        if sum(results((i-1)*gain_number+j,1:CapLossCol)) > 120
            results_hmp_loss(j,i) = NaN;
            results_hmp_eff(j,i) = NaN;
            results_hmp_zvs(j,i) = NaN;
            results_hmp_freq(j,i) = NaN;
            results((i-1)*gain_number+j,ZVSCol) = NaN;
        else
        results_hmp_loss(j,i) = sum(results((i-1)*gain_number+j,1:CapLossCol));
        results_hmp_eff(j,i) = results((i-1)*gain_number+j,PowerCol)/(results((i-1)*gain_number+j,PowerCol) + sum(results((i-1)*gain_number+j,1:CapLossCol)));
        results_hmp_zvs(j,i) = results((i-1)*gain_number+j,ZVSCol);
        results_hmp_freq(j,i) = results((i-1)*gain_number+j,FreqCol);
        end

    end
end

figure
h = heatmap(power_min:power_step:power_max,gain_min:gain_step:gain_max,results_hmp_loss,'ColorLimits',[0 120]);
xlabel('Output Power');
ylabel('Gain')
title('Power Loss')
h.MissingDataColor = [1 1 1];
h.GridVisible = 'off';
h.Colormap = jet;

figure
h = heatmap(power_min:power_step:power_max,gain_min:gain_step:gain_max,100*results_hmp_eff,'Colormap',jet,'ColorLimits',[75 100]);
xlabel('Output Power');
ylabel('Gain')
title('Efficiency')
h.MissingDataColor = [1 1 1];
h.GridVisible = 'off';
h.Colormap = jet;
% 
figure
h = heatmap(power_min:power_step:power_max,gain_min:gain_step:gain_max,results_hmp_freq,'Colormap',jet,'ColorLimits',[100e3 1.5e6]);
xlabel('Output Power');
ylabel('Gain')
title('Frequency')
h.MissingDataColor = [1 1 1];
h.GridVisible = 'off';
h.Colormap = jet;

% figure
% h = heatmap(power_min:power_step:power_max,gain_min:gain_step:gain_max,int8(a),'ColorLimits',[0 1]);
% xlabel('Output Power');
% ylabel('Gain')
% title('ZVS(0) or NOT(1)')


%% Functions
function [GaNCondLoss,GaNSWLoss] = GaNLossCalculate(ILavg,ILpp,freq,ZVSFlag)
% Conduction Loss
Rdson = 2.4*50e-3;
Ids2 = ILavg^2 + (ILpp/2/sqrt(3))^2;
GaNCondLoss = 2* (Ids2 * Rdson + sqrt(Ids2)*4*0.045);

% Switching Loss
Imax = ILavg + ILpp/2;
Eoff = 1.453*exp(0.05176*Imax); %muJ
Eon = 0;
    if ZVSFlag ~= 1
        Imin = ILavg - ILpp/2;
        Eon = 1.553*exp(0.1212*Imin); %muJ
    end
GaNSWLoss = 4*(Eon + Eoff)*freq*1e-6;
end

function [IndCondLoss, IndCoreLoss] = IndLossCalculate(ILavg,ILpp,freq)
% Core Loss

if freq < 500e3
    Cm = 6.83e-3;
    x = 1.439;
    y = 3.2672;
    Ct2 = 8.395e-5;
    Ct1 = 1.078e-2;
    Ct = 1.233;
elseif freq < 800e3
    Cm = 1.12e-7;
    x = 2.1952;
    y = 2.7199;
    Ct2 = 8.926e-5;
    Ct1 = 1.172e-2;
    Ct = 1.282;        
else
    Cm = 2.24e-10;
    x = 2.6105;
    y = 2.4977;
    Ct2 = 6.119e-5;
    Ct1 = 6.142e-3;
    Ct = 1.011;
end

N = 6;
mo = 4*pi*10^-7;
Lgap = 2*1.25e-3; % 1.16 mm and 1.33 mm measurements
Bac = N*ILpp*mo/Lgap;
Pv = Cm * freq^x * Bac^y * (Ct2*90^2 - Ct1*90 + Ct); %mW/cm3
Ve = 10200; % mm3
IndCoreLoss = 2*Pv*Ve*1e-6;

%Copper Loss
MLT = 101;
Rcable = (4/3)*(5/60); %mOhm per mm | measurement | 4/3 for 90C operation
RwindingDC = Rcable*MLT*N;
a = 0.268;
b = 3.102;
c = 0.9916;
depth = sqrt(1.678/(pi*freq*0.999991*mo));
x = 80/(depth*100);
Q = a*x^b + c;

RwindingAC = Q * RwindingDC;
PwindingDC = 4* (ILavg/2)^2*RwindingDC;
PwindingAC = 4* (ILpp/4/sqrt(3))^2*RwindingAC;
IndCondLoss = (PwindingAC + PwindingDC)*1e-3;
end

function [ZVSFlag] = ZVSCheck(ILavg,ILpp)
    if (ILpp/2 - ILavg) >= 4
        ZVSFlag = 1;
    else
        ZVSFlag = 0;
    end
end

function [CapLoss] = CapLossCalculate(ILavg,ILpp,fsw,duty)
    if duty <= 0.5
       RCoeff = -2.106*(duty) + 1.082;
    else
       RCoeff = -2.106*(1 - duty) + 1.082; 
    end
    
    SMD_esr = 2458*(fsw^-0.8446);
    DF_th = (0.0003829*fsw + 0.6488)*1e-4;
    TH_esr = DF_th/(2*pi*fsw*680e-9);
    
    % Input Capacitor Losses
    ILmin = ILavg - ILpp/2;
   
    Icaprms = sqrt(ILmin^2*duty + (ILpp)^2 *duty/3 + ILmin*ILpp*duty) * RCoeff;
    Icap_th = 680 / (680 + 2*220) * Icaprms; %single th capacitor
    Icap_smd = 220 / (680 + 2*220) * Icaprms ; %single smd capacitor
    
    OutCapLoss = Icap_th^2*TH_esr + 2*Icap_smd^2*SMD_esr;
 
    % Input Capacitor Losses
    Icaprms = ILpp/2/sqrt(3) * RCoeff;
    Icap_th = 680 / (680 + 2*220) * Icaprms; %single th capacitor
    Icap_smd = 220 / (680 + 2*220) * Icaprms ; %single smd capacitor
    InpCapLoss = Icap_th^2*TH_esr + 2*Icap_smd^2*SMD_esr;
    CapLoss = InpCapLoss + OutCapLoss;
end