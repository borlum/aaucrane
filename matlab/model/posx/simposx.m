function y = simposx(u,t)
s = tf('s');

% FROM SENSTOOLS...
b = 0.0040;
I = 0.0013;

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + b + I*s);

Gr = 7/144;
rr = 0.08;

X = M * Gr * rr * 1/s;

y = lsim(X, u, t);