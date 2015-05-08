% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
%rlocus(Mx)
Pvel = 1;
pp = 1/(20+s);
PIv = Pvel * pp;
dPIv = c2d(PIv, .01, 'tustin');
loop3 = feedback(PIv * Mx, 1);
X = Grx*rrx*(1/s);
%rlocus(loop3*X)
Ppos = 300; %P = 4.5;
loop2 = feedback(Ppos * loop3 *  X, 1);
%step(loop2*3.2)
zp = .02;
%rlocus(loop2*-W*(zp + s)); grid on; %Cm = best gain with good damping
Cm = 1; PDa = Cm * (zp + s);
%rlocus(loop2*-W); grid on;
Pang = 7;
%loop1 = feedback(loop2 * -W, PDa);
%step(loop1*3.2); grid on;
%loop2 = feedback(P *  X, 1-W*PD);
%loop1 = feedback(loop2 * W, PD);
dPDa = c2d(PDa, .01, 'tustin');