s = tf('s');

wn = .5;
zeta = .95;

cref = wn^2/(s^2+2*zeta*wn*s+wn^2);

step(4*cref);
