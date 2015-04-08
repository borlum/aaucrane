% GRAB DATA FROM CRANE
<<<<<<< HEAD
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, [CRANE_URL '/crane/manual/MANUAL_1427293912.csv']);
raw = csvread(TMP_FILE, 1, 0);
=======
y = [31.8 41.0 52.2 61.0 71.2 81.2 91.2 101.0 111.5 119.6];
>>>>>>> 74654106d28931d0a165584f1dac7ba37ab604f9

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
xlabel('Sensor output [V]');
ylabel('y [m]');
xlim([2 8]);
ylim([y(1) y(end)]);
legend('Measured', 'Linear regression', 'location', 'northeast');
grid on;