function y = simcrane(u,t,par)
s = tf('s');

bm = par(1);
I = par(2);

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + bm + I*s);

Gr = 7/144;
rr = 0.08; 

X = M * Gr * rr * 1/s;

mt = 0.5;
mp = 3.5;
l  = 0.65;
g  = 9.82;

b = par(3);
Ip = par(4);

W = (-mp*l*s^2)/((Ip + mp*l^2)*s^2 - b*s + mp*g*l);

y = lsim(X*W, u, t);