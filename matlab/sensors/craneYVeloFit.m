% COHERENCE BETWEEN VELOCITY SENSOR OUTPUT AND ACTUAL ROTATIONAL VELOCITY
% GRAB DATA FROM CRANE
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427376971.csv']);
meas1.data = csvread(TMP_FILE, 2, 0);
meas1.V    = meas1.data(:,6);
meas1.mean = mean(meas1.V(3000:4000));
meas1.in   = 5.90 * 2;
meas1.tac  = (2184 / 60) * 2*pi;


websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427377048.csv']);
meas2.data = csvread(TMP_FILE, 2, 0);
meas2.V    = meas2.data(:,6);
meas2.mean = mean(meas2.V(3000:4000));
meas2.in   = 5.10 * 2;
meas2.tac  = (1745 / 60) * 2*pi;

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427377122.csv']);
meas3.data = csvread(TMP_FILE, 2, 0);
meas3.V    = meas3.data(:,6);
meas3.mean = mean(meas3.V(3000:4000));
meas3.in   = 4.40 * 2;
meas3.tac  = (1338 / 60) * 2*pi;

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427377189.csv']);
meas4.data = csvread(TMP_FILE, 2, 0);
meas4.V    = meas4.data(:,6);
meas4.mean = mean(meas4.V(3000:4000));
meas4.in   = 3.71 * 2;
meas4.tac  = (880 / 60) * 2*pi;

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427377279.csv']);
meas5.data = csvread(TMP_FILE, 2, 0);
meas5.V    = meas5.data(:,6);
meas5.mean = mean(meas5.V(3000:4000));
meas5.in   = 3.00 * 2;
meas5.tac  = (442 / 60) * 2*pi;

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427377353.csv']);
meas6.data = csvread(TMP_FILE, 2, 0);
meas6.V    = meas6.data(:,6);
meas6.mean = mean(meas6.V(3000:4000));
meas6.in   = 2.28 * 2;
meas6.tac  = (197 / 60) * 2*pi;

out = [meas1.mean meas2.mean meas3.mean meas4.mean meas5.mean meas6.mean];
tac = [meas1.tac meas2.tac meas3.tac meas4.tac meas5.tac meas6.tac];

[p, s] = polyfit(abs(out), tac, 1)
tac_est = polyval(p, abs(out));

[p, s] = polyfit(abs(out), tac, 1);
% x = -0.4981*u + 4.7931

tac_est = polyval(p, abs(out));

r2 = 1 - s.normr^2 / norm(tac-mean(tac))^2

% PLOT
plot(abs(out), tac, '*', abs(out), tac_est);
title('Velocity sensor, y');
xlabel('Sensor output [V]');
ylabel('\omega [rad/s]');
xlim([0 10]);
ylim([0 250]);
grid on;
legend('Measured', 'Linear regression', 'location', 'southeast');