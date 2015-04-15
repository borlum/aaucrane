function y = simang4(u,t,par)

s = tf('s');

kt = par(3);%0.03;
ra = 0.43;
mp = 2.866 + 0.8;
l  = 0.65;
g  = 9.82;

b = par(1);
Ip = par(2);

W = (-mp*l*s^2)/((Ip + mp*l^2)*s^2 - b*s + mp*g*l);

y = lsim(W, u, t);
