% GRAB DATA FROM CRANE
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, [CRANE_URL '/crane/xsteps/MANUAL_1427293912.csv']);
raw = csvread(TMP_FILE, 1, 0);

% GRAB DATA FOR X POS. SENSOR
Vy = raw(:,5);

% GRAB ONLY LINEAR PART
Vy = Vy(1:6000);

y_lin = linspace(0.02, 0.85, length(Vy));

[p, s] = polyfit(Vy, y_lin', 1);

p

y_est = polyval(p, Vy);

y = p(1) * Vy + p(2);

r2 = 1 - s.normr^2 / norm(y_lin-mean(y_lin))^2

% PLOT
plot(Vy, y, Vy, y_lin);
title('Position sensor, y');
xlabel('Voltage [V]');
ylabel('y [m]');
xlim([Vy(1) Vy(end)]);
ylim([y(1) y(end)]);
legend('Linear regression', 'Measured', 'location', 'southeast');
grid on;