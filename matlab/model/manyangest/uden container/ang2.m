% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429873806.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 50);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

v = DATA(:,8);
v = v(1:idx_end);
u = DATA(:,4) * -.4981 + 4.7931;
u = u(1:idx_end) - u(1);
y = DATA(:,3);
y = 0.7367 * y(1:idx_end) - 1.414;

save measang2 t u y

delete(TMP_FILE);

process = 'ang2'
plot(t, y)
grid on;
title('Wire angle at a 3 s 9.5 V square pulse')
ylabel('Wire angle [rad]')
xlabel('Time [s]')
%%
