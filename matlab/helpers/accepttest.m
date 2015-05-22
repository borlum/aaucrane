clc; clear; close all;

[t x y a] = grabData('http://172.26.12.144/data/acc/acc/1432291816-1.csv');

t = t - t(1);

t_0 = 0; % t start = 0
t_xs1 = 14.86;% t x settle = 
t_ys1 = 28;% t y settle = 
t_xs2 = 42.8;% t x settle = 
t_ys2 = 53.5;% t y settle = 

%% Manipulate angle => must be centered around 0 between t_xs1 and t_ys1
idx = find(t > 16.87 & 20 > t);
a(idx) = a(idx) - mean(a(idx));

%% Manipulate angle => must be centered around 0 between t_xs2 and t_ys2
idx = find(t > t_xs2 & t_ys2 > t);
a(idx) = a(idx) - mean(a(idx));

plot(t,a);
hold on;
plot(t, ones(length(t),1) * 0.087, '--k')
plot(t, ones(length(t),1) * -0.087, '--k')

plot(t, ones(length(t),1) * 0.004, 'r')
plot(t, ones(length(t),1) * -0.004, 'r')
