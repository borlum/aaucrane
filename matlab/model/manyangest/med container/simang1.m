function y = simang1(u,t,par)

s = tf('s');

mp = 2.866 + 0.92;
l  = 0.66;
g  = 9.816;

b = par(1);
l = par(2);
%mp = par(3);
W = -(s^2)/(3*l*s^2 + (b/(mp*l))*s + g);

y = lsim(W, u, t);
