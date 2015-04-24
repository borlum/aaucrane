% Load models
run('../model/cranemodel.m');

C1_p = 25;
C1_i = 10;

C1 = C1_p + C1_i/s;

inner = feedback(Mx, .02);
outer = feedback(C1 * inner * Grx * rrx * 1/s,1);

OL = outer * W;

C2 = 5;

%C2 = 500 * (s^2 + 4.01^2) / (s^2 + 2*s + 4.01^2)
