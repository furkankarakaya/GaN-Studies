 %% Analytical Flux Linkage and Torque Calculation
% Assumption: No cogging torque
% Assumption: Operation in linear region even in fault conditions

close all; clear; clc; format shortEng; format compact;


t_s = 1e-6;
t_end = 2;
t_fault = 1;res = 0.1; %ohm
R = res*eye(5,5);

%% Define inductance matrix

L = [44.15,7.55,-25.17,-24.98,7.78;
    7.54,44.06,7.6,-24.87,-24.91;
    -25.17,7.61 ,44.12,7.73,-25.06;
    -24.98,-24.87,7.73,44.07,7.48;
    7.78,-24.9,-25.07,7.48,44.11]*1e-6;

L = (L+L')*0.5;

L_spatial1 = [1 cos(2*pi/5) cos(4*pi/5) cos(4*pi/5) cos(2*pi/5);
    cos(2*pi/5) 1 cos(2*pi/5) cos(4*pi/5) cos(4*pi/5);
    cos(4*pi/5) cos(2*pi/5) 1 cos(2*pi/5) cos(4*pi/5);
    cos(4*pi/5) cos(4*pi/5) cos(2*pi/5) 1 cos(2*pi/5);
    cos(2*pi/5) cos(4*pi/5) cos(4*pi/5) cos(2*pi/5) 1];


L_spatial3 = [1 cos(6*pi/5) cos(12*pi/5) cos(12*pi/5) cos(6*pi/5);
    cos(6*pi/5) 1 cos(6*pi/5) cos(12*pi/5) cos(12*pi/5);
    cos(12*pi/5) cos(6*pi/5) 1 cos(6*pi/5) cos(12*pi/5);
    cos(12*pi/5) cos(12*pi/5) cos(6*pi/5) 1 cos(6*pi/5);
    cos(6*pi/5) cos(12*pi/5) cos(12*pi/5) cos(6*pi/5) 1]; 

L_leakage = eye(5,5);

% L_leakage L_m1 and L_m3 values are obtained using Least Square Error
% method.
 
A = [L_leakage(:), L_spatial1(:), L_spatial3(:)];
b = L(:);

L_appx = (inv(A'*A)*A'*b);

L_l = L_appx(1);
L_m1 = L_appx(2);
L_m3 = L_appx(3);

% L_m1 = 38e-6;
% L_m3 = 4e-6;
% L_l = 2e-6;
L_an = L_l*L_leakage + L_m1*L_spatial1 + L_m3*L_spatial3;

Imp = R + L_an;
w_init = 70*pi;
w_set = 70*pi;
T_load = 28;
T_load_time = 0;
w_rev_time = 3;

%%
% A = (a,b,c) a: switches, b:time interval, c: sector

pulse = zeros(5,7,10);

v1 = [1 1 0 0 1];
v2 = [1 1 0 0 0];
v3 = [1 1 1 0 0];
v4 = [0 1 1 0 0];
v5 = [0 1 1 1 0];
v6 = [0 0 1 1 0];
v7 = [0 0 1 1 1];
v8 = [0 0 0 1 1];
v9 = [1 0 0 1 1];
v10 = [1 0 0 0 1];
v32 = [1 1 1 1 1];
v31 = [0 0 0 0 0];

%% Sector 1

pulse(:,1,1) = v31';
pulse(:,2,1) = v2';
pulse(:,3,1) = v1';
pulse(:,4,1) = v32';
pulse(:,5,1) = v1';
pulse(:,6,1) = v2';
pulse(:,7,1) = v31';

%% Sector 2

pulse(:,1,2) = v31';
pulse(:,2,2) = v2';
pulse(:,3,2) = v3';
pulse(:,4,2) = v32';
pulse(:,5,2) = v3';
pulse(:,6,2) = v2';
pulse(:,7,2) = v31';

%% Sector 3

pulse(:,1,3) = v31';
pulse(:,2,3) = v4';
pulse(:,3,3) = v3';
pulse(:,4,3) = v32';
pulse(:,5,3) = v3';
pulse(:,6,3) = v4';
pulse(:,7,3) = v31';

%% Sector 4

pulse(:,1,4) = v31';
pulse(:,2,4) = v4';
pulse(:,3,4) = v5';
pulse(:,4,4) = v32';
pulse(:,5,4) = v5';
pulse(:,6,4) = v4';
pulse(:,7,4) = v31';

%% Sector 5

pulse(:,1,5) = v31';
pulse(:,2,5) = v6';
pulse(:,3,5) = v5';
pulse(:,4,5) = v32';
pulse(:,5,5) = v5';
pulse(:,6,5) = v6';
pulse(:,7,5) = v31';

%% Sector 6

pulse(:,1,6) = v31';
pulse(:,2,6) = v6';
pulse(:,3,6) = v7';
pulse(:,4,6) = v32';
pulse(:,5,6) = v7';
pulse(:,6,6) = v6';
pulse(:,7,6) = v31';

%% Sector 7

pulse(:,1,7) = v31';
pulse(:,2,7) = v8';
pulse(:,3,7) = v7';
pulse(:,4,7) = v32';
pulse(:,5,7) = v7';
pulse(:,6,7) = v8';
pulse(:,7,7) = v31';

%% Sector 8

pulse(:,1,8) = v31';
pulse(:,2,8) = v8';
pulse(:,3,8) = v9';
pulse(:,4,8) = v32';
pulse(:,5,8) = v9';
pulse(:,6,8) = v8';
pulse(:,7,8) = v31';

%% Sector 9

pulse(:,1,9) = v31';
pulse(:,2,9) = v10';
pulse(:,3,9) = v9';
pulse(:,4,9) = v32';
pulse(:,5,9) = v9';
pulse(:,6,9) = v10';
pulse(:,7,9) = v31';

%% Sector 10

pulse(:,1,10) = v31';
pulse(:,2,10) = v10';
pulse(:,3,10) = v1';
pulse(:,4,10) = v32';
pulse(:,5,10) = v1';
pulse(:,6,10) = v10';
pulse(:,7,10) = v31';

pulse(:,:,11) = pulse(:,:,1);
pulse(:,:,12) = pulse(:,:,2);
pulse(:,:,13) = pulse(:,:,3);
pulse(:,:,14) = pulse(:,:,4);
pulse(:,:,15) = pulse(:,:,5);

%% RUN!

% simout = sim('fivephase.slx');
% 
% save results.mat;