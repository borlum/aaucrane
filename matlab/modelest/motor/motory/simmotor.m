function y = simmotor(u,t,par)
b = par(1);
I = par(2);

kt = 0.03;
ra = 0.43;

s = tf('s');

M = (kt/ra)/(kt^2/ra + b + I * s);

y = lsim(M, u, t);
