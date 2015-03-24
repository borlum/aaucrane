% Model of crane AXEM F9M2

s = tf('s');

jm = 2.9*10^-5;
ji = 0;
jw = 0;
jtot = jm+ji+jw;

Btot = 0;

r1 = 12;
r2 = 48;
r3 = 14;
r4 = 72;
rdrum = .08;

gr = 7/44;
mp = .3;
mt = .5

g = 9.82;

kt = 0.03;
ra = 0.43;
ke = kt;

H_x = (kt/ra) / ((jm + gr*rdrum^2*mp*mt)*s^2 + (kt^2/ra)*s)
H_y = (kt/ra)/(jm*s^2+kt*ke*1/ra*s -(-jtot*s^2+Btot*s+r1/r2*r3/r4*rdrum*s^2));


Hnl = (kt/ra) / (jm * s^2+kt*ke*(1/ra)*s);

step(14 * H_x);
