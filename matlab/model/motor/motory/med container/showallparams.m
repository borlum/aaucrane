par = [.0028 .0013;
       .0015 .0012;
       .0011 .0011];

s = tf('s');

kt = 0.03;
ra = 0.43;

Gr = 1/24;
rr = 0.05;

for i = 1:3
  bm = par(i, 1);
  I = par(i, 2);
  M = (kt/ra)/(kt^2/ra + bm + I*s);
  Y(i) = M * Gr * rr * 1/s * 1/2;
  bode(X(i));hold on
  grid on;
end
