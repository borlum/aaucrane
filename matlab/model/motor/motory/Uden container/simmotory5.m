function y = simmotorx5(u,t,par)
s = tf('s');

bm = par(1);
I = par(2);

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + bm + I*s);

Gr = 1/24;
rr = 0.05;

X = M * Gr * rr * 1/s;

y = lsim(X, u, t);
