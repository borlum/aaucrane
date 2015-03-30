% XPOS MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427464633.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 8);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0,t(idx_end), idx_end);

y = 2*(0.5279 * DATA(:,2) - 3.85);
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = -u(1:idx_end);

delete(TMP_FILE);
