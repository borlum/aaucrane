% GRAB DATA FROM CRANE
y = [31.8 41.0 52.2 61.0 71.2 81.2 91.2 101.0 111.5 119.6];

y = (y - y(1)) / 100;

u = [7.90 7.28 6.55 5.97 5.32 4.67 4.01 3.380 2.700 2.170];


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