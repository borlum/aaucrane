% Load models (Mx, My, X, Y, W)
run('../model/cranemodel.m');


wn = .5;
zeta = .95;

cref = wn^2/(s^2+2*zeta*wn*s+wn^2);

%step(4*cref);

%tau = 3;

%cref = 1/(tau*s+1)

C1 = 50;

inner = feedback(Mx * C1,1);

C2 = 1/(s+1);

cl = feedback(inner * C2, 1) * Grx*rrx*1/s;
