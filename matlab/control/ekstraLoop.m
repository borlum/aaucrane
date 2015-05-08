clear;
%close all;

run('../model/cranemodel');

%====================================
%			Med container
%====================================
% Tuning, lav settle time, lav overshoot

Hv = Mx * Grx*rrx;

C1 = 3;

Vcl = feedback(C1 * Hv ,1);

C2 = 1.65;

Xcl = feedback(C2 * Vcl * 1/s, 1);

k = 3.46;

p = 4;

d = 1;

C3 = k * (s * d + p);

dC3 = c2d(C3, .01, 'tustin');

C3 = 10;

Acl = feedback(-C3 * Vcl * 1/s * W ,1);






