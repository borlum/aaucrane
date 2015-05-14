bwd = 0.0115;
Iwd = 0.0067;
Mywd = (kt/ra)/(kt^2/ra + bwd + Iwd*s);
Hywd = Mywd * Gry * rry * (1/2) * (1/s);
bwu = 0.0322;
Iwu = 0.0090;
Mywu = (kt/ra)/(kt^2/ra + bwu + Iwu*s);
Hywu = Mywu * Gry * rry * (1/2) * (1/s);
bnd = 0.0042;
Ind = 0.0025;
Mynd = (kt/ra)/(kt^2/ra + bnd + Ind*s);
Hynd = Mynd * Gry * rry * (1/2) * (1/s);
bnu = 0.0051;
Inu = 0.0022;
Mynu = (kt/ra)/(kt^2/ra + bnu + Inu*s);
Hynu = Mynu * Gry * rry * (1/2) * (1/s);
%%
% FIND THE RIGHT SAMPLINGS FREQ. BY: TS = 4.6/(SIGMA*OMEGA_N); 
% WHERE TS IS THE WANTED TIME
% SIGMA IS THE OVERSHOOT LVL (IF 0% SIGMA = 1)
% OMEGA_N IS THE SAMPLING FREQUENCY 
Ts = 5.5;
Sigma = 0.0139;
omega_n = (Ts*Sigma)/4.6 % = 1.957
% USING THIS IN margin(Hy**) YOU FIND THE RIGHT x = GAIN(dB) VALUE
%PyXX = 10^(x/20);
%rlocus(Hywu);
%margin(Hywu);
PywuX = 10^(55/20);
Pywu = 370; %WITH CONTAINER UP
%step(feedback(PywuX*Hywu, 1)*-0.78); grid on; ylabel('y[m]'); title('Step response of y-axis @ 0.78 m'); xlim([0 7]);
%rlocus(Hywd);
%margin(Hywd);
PywdX = 10^(48/20);
Pywd = 132; %WITH CONTAINER DOWN
%step(feedback(Pywd*Hywd, 1)*0.78); grid on; ylabel('y[m]'); title('Step response of y-axis @ 0.78 m'); xlim([0 7]);
%rlocus(Hynu);
%margin(Hynd);
PynuX = 10^(41/20);
Pynu = 75; %NO CONTAINER UP
%step(feedback(Pynu*Hynu, 1)*-0.78); grid on; ylabel('y[m]'); title('Step response of y-axis @ 0.78 m'); xlim([0 7]);
%rlocus(Hynd);
%margin(Hynu);
PyndX = 10^(42/20);
Pynd = 54.3; %NO CONTAINER DOWN
step(feedback(Pynd*Hynd, 1)*0.78); grid on; ylabel('y[m]'); title('Step response of y-axis @ 0.78 m'); xlim([0 7]);