function y = simang(u,t,par)
s = tf('s');

% Magnet + krog
mm = 0.8;
% Container + magnet + krog (Hele 'pendul' massen)
mp = 2.866 + mm;
% Længde af wire
l  = 0.65;
% Tyngdekraften
g  = 9.82;

b = par(1);
Ip = par(2);


%W = (-mp*l*s^2)/((mp*l^2 - Ip)*s^2 + b*s + mp*g*l);

%y = lsim(W, u, t);

assignin('base', 'us',u);
assignin('base', 'ts',t);
assignin('base', 'jp', Ip);
assignin('base', 'B', b);

%assignin('base', 'W', W);

set_param('anglesimu','AlgebraicLoopSolver','LineSearch');

sim('anglesimu');
y = ysimu;