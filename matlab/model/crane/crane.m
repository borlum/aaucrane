% CRANE MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1428482290.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 120);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0, t(end), length(t));

y = DATA(:,3) * 0.7367 - 1.3211;
y = y(1:idx_end);
y = y - y(1);

u = (DATA(:,8) * 2);
u = -u(1:idx_end) + u(1);

save meascrane t u y

process = 'crane';

delete(TMP_FILE);