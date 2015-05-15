clear all; close all;
run('../model/cranemodel.m');
bwd = 0.00078; Iwd = 0.0014; Mywd = (kt/ra)/(kt^2/ra + bwd + Iwd*s); Hywd = Mywd * Gry * rry * (1/2) * (1/s);
bwu = 0.0019; Iwu = 0.0013; Mywu = (kt/ra)/(kt^2/ra + bwu + Iwu*s); Hywu = Mywu * Gry * rry * (1/2) * (1/s);
bnd = 0.00103; Ind = 0.00085; Mynd = (kt/ra)/(kt^2/ra + bnd + Ind*s); Hynd = Mynd * Gry * rry * (1/2) * (1/s);
bnu = 0.00105; Inu = 0.0011; Mynu = (kt/ra)/(kt^2/ra + bnu + Inu*s); Hynu = Mynu * Gry * rry * (1/2) * (1/s);
%%
% FIND THE RIGHT SAMPLINGS FREQ. BY: TS = 4.6/(SIGMA*OMEGA_N); 
% WHERE TS IS THE WANTED TIME
% SIGMA IS THE OVERSHOOT LVL (IF 0% SIGMA = 1)
% OMEGA_N IS THE SAMPLING FREQUENCY 
Ts = 3.5;
Mp = 0.27;
Sigma_u = sqrt((log(Mp)^2)/(pi^2+log(Mp)^2))
Sigma_d = 1;
omega_u = 4.6/(Ts*Sigma_u); % = 1.4148
omega_d = 4.6/(Ts*Sigma_d); % = 0.8364
omega = [omega_d omega_u]
%margin(Hywd); = GM = inf, PM = 89.3
%rlocus(Hywd);
Pywd = 20; %WITH CONTAINER DOWN
wcd = bandwidth(feedback(Pywd*Hywd, 1));
%hold off; plot(WCD1); hold on; grid on; plot(WCD);

%margin(Hywu); = GM = inf, PM = 89.7
%rlocus(Hywu);
Pywu = 86.5; %WITH CONTAINER UP
wcu = bandwidth(feedback(Pywu*Hywu, 1));
%hold off; plot(WCU1); hold on; grid on; plot(WCU);

%margin(Hynd); = GM = inf, PM = 89.6
%rlocus(Hynd);
Pynd = 43.2; %NO CONTAINER DOWN
ncd = bandwidth(feedback(Pynd*Hynd, 1));
%hold off; plot(NCD1); hold on; grid on; plot(NCD);

%margin(Hynu); = GM = inf, PM = 89.5
%rlocus(Hynu);
Pynu = 68; %NO CONTAINER UP
ncu = bandwidth(feedback(Pynu*Hynu, 1));
%hold off; plot(NCU1); hold on; grid on; plot(NCU);

BWwc = [wcd wcu]
BWnc = [ncd ncu]