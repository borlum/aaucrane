CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1431686817.csv']);
DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 6);
idx_end = idx_end(1);

t = t(1:idx_end);

t1 = linspace(0,t(idx_end), idx_end);

y = DATA(:,5);
y1 = y(1:idx_end) - y(1);
u = DATA(:,9);
u = u(1:idx_end);

save measmotory6 t u y

process = 'motory6'

delete(TMP_FILE);
