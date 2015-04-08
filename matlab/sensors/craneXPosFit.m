% GRAB DATA FROM CRANE
x = [0 0.4 0.8 1.2 1.6 2.0 2.4 2.8 3.2 3.6 4.0];
u = [9.624 8.818 8.022 7.221 6.415 5.590 4.798 4.002 3.200 2.400 1.595];


[p, s] = polyfit(u, x, 1);
% x = -0.4981*u + 4.7931

x_est = polyval(p, u);

%x_est = p(1) * u + p(2);

r2 = 1 - s.normr^2 / norm(x-mean(x))^2

% PLOT
plot(u, x, '*', u, x_est);
title('Position sensor, x');
xlabel('Voltage [V]');
ylabel('x [m]');
xlim([0 10]);
ylim([x(1) x(end)]);
grid on;
legend('Measured', 'Linear regression', 'location', 'northeast');