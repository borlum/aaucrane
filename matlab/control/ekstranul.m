s = tf('s');

k = 2;

PD = (s+4) * k;

dD = c2d(dD, .001, 'tustin');

C1 = 7.5;

Ti = 4.15;
% D part ->
%step(loop2*4) %time below 0.01 amplitude
Td = 0.02; 
% P part ->
Tp = 0.5;     %from formular 0 < Tp <= 1 
%Gain ->
%rlocus(-(Tp+1+1/(Ti*s))*loop2*Wu)
k = 10;     %Trail and error
Cs = k*(Tp+1+1/(Ti*s));
%% Y-AXIS
%------------------------------------------------------------------------------
% Y Control UPWARDS
%------------------------------------------------------------------------------
%rlocus(Yu) to find C3
C3 = 49;
%------------------------------------------------------------------------------
% Y Control DOWNWARDS
%------------------------------------------------------------------------------
%rlocus(Yd) to find C4
C4 = 94.9;
%%
PID = k*(1+1/s*1/Ti+s*Td);
% Discrete PID
dPID = c2d(PID, .001, 'tustin');
