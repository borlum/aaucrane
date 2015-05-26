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
plot([14.86 14.86],[-0.087 0.087],'m'); %Y-action
plot([27 27],[-0.087 0.087],'m'); %Y-done
plot([42.8 42.8],[-0.087 0.087],'m'); %Y-action
plot([53.5 53.5],[-0.087 0.087],'m'); %Y-done
plot(t, ones(length(t),1) * -0.087, '--k'); %Max \theta when moving container
plot(t, ones(length(t),1) * -0.004, 'r'); %Max \theta when picking/placing container
legend('Measured \theta','Max \theta when moving container','Max \theta when picking/placing container','Location','northeast');
xlim([0 53.61]);
ylim([-0.15 0.15]);
%% Plot of the X-axis movement
% idx = find(t > 40);
% x(idx) = x(idx) - 0.03;
% 
% plot(t,x);
% xlabel('Time [s]'); 
% ylabel('Position [m]');
% title('Acceptance test of ACC (x-axis)'); 
% grid on;
% hold on;
% plot(t, ones(length(t),1) * (3.59-0.05+0.0055), '--k');
% plot([15 15],[3.25 3.75],'r'); %X-Done
% plot([27 27],[3.25 3.75],'r'); %X-action
% plot([42.8 42.8],[0.25 0.75],'r'); %X-done
% plot(t, ones(length(t),1) * 0.4445, '--k');
% plot(t, ones(length(t),1) * (3.59-0.05-0.0055), '--k'); 
% plot(t, ones(length(t),1) * 0.4555, '--k');
% legend('Measured Position','Error band','Action indicator','Location','northeast');
% xlim([0 53.61]);
% %ylim([-0.15 0.15]);
%% Plot of the Y-axis movement
%idx = find(t > 40);
%x(idx) = x(idx) - 0.03;

% plot(t,y);
% xlabel('Time [s]'); 
% ylabel('Position [m]');
% title('Acceptance test of ACC (y-axis)'); 
% grid on;
% hold on;
% plot(t, ones(length(t),1) * 0.268, '--k');
% plot([15 15],[0.24 0.3],'r'); %X-Done
% plot([20 20],[1 1.07],'r'); %X-Done
% plot([27 27],[0.24 0.3],'r'); %X-action
% plot([42.8 42.8],[0.24 0.3],'r'); %X-done
% plot([49 49],[1 1.07],'r'); %X-Done
% plot(t, ones(length(t),1) * 0.272, '--k');
% plot(t, ones(length(t),1) * 1.052, '--k'); 
% plot(t, ones(length(t),1) * 1.048, '--k');
% legend('Measured Position','Error band','Action indicator','Location','southwest');
% xlim([0 53.61]);
%ylim([-0.15 0.15]);