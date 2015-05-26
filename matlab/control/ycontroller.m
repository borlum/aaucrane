clear all; close all;
run('../model/cranemodel.m');
bwd = 0.00078; Iwd = 0.0014; Mywd = (kt/ra)/(kt^2/ra + bwd + Iwd*s); Hywd = Mywd * Gry * rry * (1/2) * (1/s);
bwu = 0.0019; Iwu = 0.0013; Mywu = (kt/ra)/(kt^2/ra + bwu + Iwu*s); Hywu = Mywu * Gry * rry * (1/2) * (1/s);
bnd = 0.00103; Ind = 0.00085; Mynd = (kt/ra)/(kt^2/ra + bnd + Ind*s); Hynd = Mynd * Gry * rry * (1/2) * (1/s);
bnu = 0.00105; Inu = 0.0011; Mynu = (kt/ra)/(kt^2/ra + bnu + Inu*s); Hynu = Mynu * Gry * rry * (1/2) * (1/s);

% FIND THE RIGHT SAMPLINGS FREQ. BY: TS = 4.6/(SIGMA*OMEGA_N); 
% WHERE TS IS THE WANTED TIME
% SIGMA IS THE OVERSHOOT LVL (IF 0% SIGMA = 1)
% OMEGA_N IS THE SAMPLING FREQUENCY 
Ts = 4;
Mp = 0.1;
Sigma_u = sqrt((log(Mp)^2)/(pi^2+log(Mp)^2));
Sigma_d = 1;
omega_u = 4.6/(Ts*Sigma_u); % = 1.9453
omega_d = 4.6/(Ts*Sigma_d); % = 1.1500
%omega = [omega_d omega_u]

%margin(Hywu); = GM = inf, PM = 89.7
X = 42; % Find the dB at the calculated bandwidth frequency 
Pywu =  10^(X/20);
Pywu = 75.9;
%Pywu = 64; %WITH CONTAINER UP
wcu = bandwidth(feedback(Pywu*Hywu, 1));
step(feedback(Pywu*Hywu, 1)*0.78); grid on; xlabel('t'); ylabel('y[m]'); title('Step response of y-axis @ 0.78'); xlim([0 7]); hold on; plot([0 7],[0.7855 0.7855],'k--');plot([0 7],[0.7745 0.7745],'k--');stepinfo(feedback(Pywu*Hywu, 1)*0.78)
%hold off; plot(WCU); grid on; xlabel('t[seconds]'); ylabel('y[m]'); title('Zoomed step response of y-axis @ -0.78'); hold on; plot([0 10],[-0.778 -0.778],'k--'); plot([0 10],[-0.782 -0.782],'k--')

%margin(Hywd); = GM = inf, PM = 89.3
X = 34.3; % Find the dB at the calculated bandwidth frequency 
Pywd =  10^(X/20); 
%Pywd = 26.9;
Pywd = 26.9; %WITH CONTAINER DOWN
wcd = bandwidth(feedback(Pywd*Hywd, 1));
%step(feedback(Hywd*Pywd, 1)*0.78); grid on; xlabel('t'); ylabel('y[m]'); title('Step response of y-axis @ 0.78'); xlim([0 7]); stepinfo(feedback(Hywd*Pywd, 1)*0.78)
%hold off; plot(WCD); grid on; xlabel('t[seconds]'); ylabel('y[m]'); title('Zoomed step response of y-axis @ 0.78'); hold on; plot([0 7],[0.778 0.778],'k--'); plot([0 7],[0.782 0.782],'k--')

%margin(Hynu); = GM = inf, PM = 89.5
X = 34.6; % Find the dB at the calculated bandwidth frequency 
Pynu =  10^(X/20); 
%Pynu = 40;
Pynu = 42; %NO CONTAINER UP
ncu = bandwidth(feedback(Pynu*Hynu, 1));
%step(feedback(Pynu*Hynu, 1)*0.78); grid on; xlabel('t'); ylabel('y[m]'); title('Step response of y-axis @ 0.78'); xlim([0 7]); stepinfo(feedback(Pynu*Hynu, 1)*0.78)
%hold off; plot(NCU); grid on; xlabel('t[seconds]'); ylabel('y[m]'); title('Zoomed step response of y-axis @ -0.78'); hold on; plot([0 8],[-0.778 -0.778],'k--'); plot([0 8],[-0.782 -0.782],'k--')









%margin(Hynd); = GM = inf, PM = 89.6
X = 34.3; % Find the dB at the calculated bandwidth frequency 
Pynd =  10^(X/20);
Pynd = 50;
%Pynd = 37.27;
%Pynd = 55; %NO CONTAINER DOWN
ncd = bandwidth(feedback(Pynd*Hynd, 1));
%step(feedback(Pynd*Hynd, 1)*0.78); grid on; xlabel('t'); ylabel('y[m]'); title('Step response of y-axis @ 0.78'); xlim([0 7]); stepinfo(feedback(Pynd*Hynd, 1)*0.78)
%hold off; plot(NCD); grid on; xlabel('t[seconds]'); ylabel('y[m]'); title('Zoomed step response of y-axis @ 0.78'); hold on; plot([0 10],[0.778 0.778],'k--'); plot([0 10],[0.782 0.782],'k--')

%BWwc = [wcd wcu]
%BWnc = [0 ncu]