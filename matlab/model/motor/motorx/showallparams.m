par = [0.0065 0.002;
       .0028 .0013;
       .0015 .0012;
       .0011 .0011];

s = tf('s');

kt = 0.03;
ra = 0.43;

Gr = 7/144;
rr = 0.08;

for i = 1:4
  bm = par(i, 1);
  I = par(i, 2);
  M = (kt/ra)/(kt^2/ra + bm + I*s);
  X(i) = M * Gr * rr * 1/s;
  bode(X(i));hold on
  grid on;
end
