function y = simang2(u,t,par)

s = tf('s');

mp = 2.866 + 0.92;
l  = 0.61;
g  = 9.816;

b = par(1);
%mp = par(2);

W = -(s^2)/(l*s^2 + (b/(mp*l))*s + g);

y = lsim(W, u, t);