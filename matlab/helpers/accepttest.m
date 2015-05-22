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
xlabel('Time [s]'); 
ylabel('\theta [rad]');
title('Acceptance test of ACC (angle)'); 
grid on;
hold on;
plot(t, ones(length(t),1) * 0.087, '--k'); %Max \theta when moving container
plot(t, ones(length(t),1) * 0.004, 'r'); %Max \theta when picking/placing container
plot([14.86 14.86],[-0.01 0.01],'r'); %Y-action
plot([20 20],[-0.01 0.01],'r'); %Y-done
plot([42.8 42.8],[-0.01 0.01],'r'); %Y-action
plot([48.24 48.24],[-0.01 0.01],'r'); %Y-done
plot(t, ones(length(t),1) * -0.087, '--k'); %Max \theta when moving container
plot(t, ones(length(t),1) * -0.004, 'r'); %Max \theta when picking/placing container
legend('Measured \theta','Max \theta when moving container','Max \theta when picking/placing container','Location','northeast');
xlim([0 53.61]);
