% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
%rlocus(Grx*rrx*Mx)
Pvel = 10;
%pp = 1/(4+s);
%PIv = Pvel * pp;
%dPIv = c2d(PIv, .01, 'tustin');
loop3 = feedback(Pvel * Grx*rrx*Mx, 1);
X = (1/s);
%rlocus(loop3*X)
Ppos = 5; %P = 4.5;
loop2 = feedback(Ppos * loop3 *  X, 1);
%step(loop2*3.2)
%zp = 4;
%rlocus(loop2*-W*(zp + s); grid on; %Cm = best gain with good damping
Cm = 1; PDa = Cm * (1 + s);
%rlocus(loop2*-W); grid on;
Pang = 2.5;
%loop1 = feedback(loop2 * -W, PDa);
%step(loop1*3.2); grid on;
%loop2 = feedback(P *  X, 1-W*PD);
%loop1 = feedback(loop2 * W, PD);
dPDa = c2d(PDa, .01, 'tustin');