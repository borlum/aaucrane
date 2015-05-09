function y = simang1(u,t,par)

s = tf('s');

mp = 2.866 + 1;
l  = 0.48;
g  = 9.82;

b = par(1);

W = (-1/l*s^2)/(s^2 + (b/(mp*l^2)*s + g/l));

y = lsim(W, u, t);
