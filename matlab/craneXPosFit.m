% GRAB DATA FROM CRANE
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427291280.csv']);
raw = csvread(TMP_FILE, 2, 0);

% GRAB DATA FOR X POS. SENSOR
Vx = raw(:,4);

% GRAB ONLY LINEAR PART
Vx = Vx(100:18177);

x_lin = linspace(0, 4.03, length(Vx));

[p, s] = polyfit(Vx, x_lin', 1)

x_est = polyval(p, Vx);

x = p(1) * Vx + p(2);

r2 = 1 - s.normr^2 / norm(x_lin-mean(x_lin))^2

% PLOT
plot(Vx, x, Vx, x_lin);
title('Position sensor, x');
xlabel('Voltage [V]');
ylabel('x [m]');
<<<<<<< HEAD
xlim([Vx(1) Vx(end)]);
ylim([x(1) x(end)]);
grid on;
legend('Linear regression', 'Measured', 'location', 'southeast');