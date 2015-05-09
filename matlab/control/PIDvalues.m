% MED CONTAINER
clear all;
close;
run('../model/cranemodel.m');
%rlocus(Mx)
Pvel = 11.6;
%loop3 = feedback(Pvel * Grx*rrx*Mx, 1);
%step(loop3*3.2);
X = Mx*Grx*rrx*(1/s);
%rlocus(X*1/(ip+s))
Ppos = 7.5;
ip = 2;
PIp = Ppos + (1/(ip+s));
dPIp = c2d(PIp, .01, 'tustin');
loop2 = feedback(Ppos *  X, 1);
%margin(loop2*3)
zp = 2;
%rlocus(loop2*-W*(zp + s)); grid on; %Cm = best gain with good damping
Cm = 3.6; PDa = Cm + (zp + s);
%rlocus(loop2*-W*(4+s)); grid on;
Pang = 9;
%loop1 = feedback(loop2 * -W, Pang);
%step(loop1*3.2); grid on;
%loop2 = feedback(P *  X, 1-W*PD);
dPDa = c2d(PDa, .01, 'tustin');