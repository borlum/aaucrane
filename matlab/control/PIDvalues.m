% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
Mx = Mxm;
%rlocus(Mx)
Pvel = 0.15;
pp = 1/(10+s);
PIv = Pvel * pp;
dPIv = c2d(PIv, .001, 'tustin');
loop3 = feedback(PIv * Mx, 1);
X = gearx*(1/s);
%rlocus(loop3*X)
Ppos = 700; %P = 4.5;
loop2 = feedback(Ppos * loop3 *  X, 1);
W = Wm;
%step(loop2*3.2)
zp = 4;
%rlocus(loop2*-W*(zp + s)); grid on; %Cm = best gain with good damping
%Cm = 1.22; PDa = Cm * (zp + s);
rlocus(loop2*-W); grid on;
Pang = 22.9;
loop1 = feedback(loop2 * -W, PDa);
%step(loop1*3.2); grid on;
%loop2 = feedback(P *  X, 1-W*PD);
%loop1 = feedback(loop2 * W, PD);
dPDa = c2d(PDa, .001, 'tustin');

%% UDEN CONTAINER
clear all;
close;
run('../model/cranemodel.m');
X = Xu;
W = Wu;

%rlocus(X)
P = 3.75;
loop2 = feedback(P *  X, 1);
%bode(loop2*-W) %zp = freq. at -45degrees
zp = 4;
preloop1 = feedback(loop2 * -W, (zp + s));
%rlocus(loop2*-W*(zp + s)); grid on; %C2 = best gain with good damping
Cm = 10;
sv = 0.1;
PD = Cm * (zp + sv*s);
loop1 = feedback(loop2 * -W, PD);
%step(loop2*3.2); grid on;
%step(loop1*3.2); grid on;
dPD = c2d(PD, .01, 'tustin');
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