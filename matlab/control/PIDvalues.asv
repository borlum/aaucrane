run('../model/cranemodel.m');
%rlocus(X)
C2 = 11.6;
loop2 = feedback(C2 *  Mx * gear * (1/s), 1);
%------------------------------------------------------------------------------
% PID Control
%------------------------------------------------------------------------------
% PID = k * ( 1*(b*R(s)-Y(s)) + 1/Ti*(R(s)-Y(s)) + Td*Y(s) );
% I part ->
%bode(loop2*-W) %Ti = freq. at -45degrees
Ti = 4;
% D part ->
%step(loop2*4) %time below 0.01 amplitude
Td = 0.06; 
% P part ->
Tp = 1;     %from formular 0 < Tp <= 1 
%Gain ->
rlocus((Tp+1+1/Ti*s+Tp*s)*loop2*C2*-W)
k = 20;     %Trail and error