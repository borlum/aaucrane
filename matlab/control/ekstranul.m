s = tf('s');

k = 2;

dD = (s+4) * k;

dD = c2d(dD, .001, 'tustin');

C1 = 7.5;