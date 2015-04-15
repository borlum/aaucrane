s = tf('s');

wn = .5;
zeta = .95;

cref = wn^2/(s^2+2*zeta*wn*s+wn^2);

step(4*cref);

%tau = 3;

%cref = 1/(tau*s+1)
