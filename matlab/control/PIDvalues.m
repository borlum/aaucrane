% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
%rlocus(Grx*rrx*Mx)
Pvel = 3;
%loop3 = feedback(Pvel * Grx*rrx*Mx, 1);
%step(loop3*3.2);
X = Mx*Grx*rrx*(1/s);
%rlocus(X)
Ppos = 9;
loop2 = feedback(Ppos *  X, 1);
%margin(loop2*3)
zp = 2;
%rlocus(loop2*-W*(zp + s)); grid on; %Cm = best gain with good damping
Cm = 2; PDa = Cm * (zp + s);
%rlocus(loop2*-W*(4+s)); grid on;
Pang = 9;
%loop1 = feedback(loop2 * -W, Pang);
%step(loop1*3.2); grid on;
%loop2 = feedback(P *  X, 1-W*PD);
dPDa = c2d(PDa, .01, 'tustin');