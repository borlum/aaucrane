function y = simposx(u,t)
s = tf('s');

% FROM SENSTOOLS...
b = 0.0014;
I = 0.0009;

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + b + I*s);

Gr = 7/144; %?
rr = 0.08; %?

Y = M * Gr * rr * 1/s;

y = lsim(Y, u, t);