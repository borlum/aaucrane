 clear;
%close all;

run('../model/cranemodel');

% Angle control, as damped as possible!

C1 = 5;

Vcl = feedback(C1 * Mx * Grx * rrx, 1);

C2 = 80 * (s + 6) * 1/(s+20);

dC2 = c2d(C2, .01, 'tustin');

Acl = feedback(C2 * Vcl * 1/s * -W,1);

C3 = 10;
