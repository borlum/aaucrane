function y = simang(u,t,par)
s = tf('s');

% FROM SENSTOOLS: STEP INPUT
%b = 0.0014;
%I = 0.0017;

% FROM SENSTOOLS: SQUARE INPUT
b = 0.004;
I = 0.0013;

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + b + I*s);

Gr = 7/144;
rr = 0.08; 

X = M * Gr * rr * 1/s;

mt = 0.5;
mp = 3.5;
l  = 0.65;
g  = 9.82;

b = par(1);
Ip = par(2);

W = (-mp*l*s^2)/((Ip * mp*l^2)*s^2 - b*s + mp*g*l);

y = lsim(W, u, t);