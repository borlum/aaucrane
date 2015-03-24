mp = 2.8;
mt = .5;

plot_time = 30;

l = .5;

I = mp * l^2 ;

b = .1;

g = 9.82;

s = tf('s');

h = ((-mp*l)/(mt+mp))/(s^2*(I+l^2-(mp^2*l^2)/(mt+mp))+b*s+mp*g*l)

impulse(h, plot_time)
hold on;
step(h, plot_time)
