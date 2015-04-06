% GRAB DATA FROM CRANE
y = [23.5 33.0 43.0 53.3 62.6 73.1 83.0 91.6 103.0 114.7 121.8];
y = (y - 23.5) / 100;
u = [7.406 6.762 6.087 5.448 4.827 4.150 3.518 2.942 2.205 1.453 0.970];


[p, s] = polyfit(u, y, 1);
%y = -0.1534*u + 1.1331

y_est = polyval(p, u);

%y = p(1) * Vy + p(2);

r2 = 1 - s.normr^2 / norm(y_est-mean(y_est))^2

% PLOT
plot(u, y, '*', u, y_est);
title('Position sensor, y');
xlabel('Voltage [V]');
ylabel('y [m]');
xlim([0 10]);
ylim([y(1) y(end)]);
legend('Measured', 'Linear regression', 'location', 'northeast');
grid on;