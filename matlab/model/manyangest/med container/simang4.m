function y = simang4(u,t,par)

s = tf('s');

mp = 2.866 + 0.8;
l  = 0.65;
g  = 9.82;

b = par(1);
Ip = par(2);

W = (-mp*l*s^2)/((mp*l^2 - Ip)*s^2 + b*s + mp*g*l);

y = lsim(W, u, t);
