% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429008979.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 60);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

u = DATA(:,4) * -.4981 + 4.7931;
u = u(1:idx_end) - u(1);
y = DATA(:,3);
y = 0.7367 * y(1:idx_end) - 1.411;

save measang4 t u y

delete(TMP_FILE);

process = 'ang4'
plot(t, y)
title('Wire angle at a 3 s 11 V square pulse')
ylabel('Wire angle [rad]')
xlabel('Time [s]')
%%
