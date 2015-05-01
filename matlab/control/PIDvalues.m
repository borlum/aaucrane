% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
X = Xm;
W = Wm;
%rlocus(Xm)
C1 = 15;
loop2 = feedback(C1 *  X, 1);
%------------------------------------------------------------------------------
% PID Control
%------------------------------------------------------------------------------
% PID = k * ( 1*(b*R(s)-Y(s)) + 1/Ti*(R(s)-Y(s)) + Td*Y(s) );
% I part ->
%bode(loop2*-Wm) %Ti = freq. at -45degrees
Ti = 4.01;
% D part ->
%step(loop2*4) %time below 0.01 amplitude
Td = 0.02; 
% P part ->
Tp = 0.5;     %from formular 0 < Tp <= 1 
%Gain ->
%rlocus((Tp+1+1/(Ti*s))*loop2*-Wm)
k = 10;     %Trail and error
Cs = k*(Tp+1+1/(Ti*s));
%% UDEN CONTAINER
clear all;
close;
run('../model/cranemodel.m');
X = Xu;
W = Wu;
%rlocus(X)
C1 = 15;
loop2 = feedback(C1 * X, 1);
%------------------------------------------------------------------------------
% PID Control
%------------------------------------------------------------------------------
% PID = k * ( 1*(b*R(s)-Y(s)) + 1/Ti*(R(s)-Y(s)) + Td*Y(s) );
% I part ->
%bode(loop2*-Wu) %Ti = freq. at -45degrees
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
C3 = 449;
%------------------------------------------------------------------------------
% Y Control DOWNWARDS
%------------------------------------------------------------------------------
%rlocus(Yd) to find C4
C4 = 94.9;