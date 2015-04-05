% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsquare/1427800240.csv']);
%websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427451475.csv']);
DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,6) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx t u y

process = 'motorx';

delete(TMP_FILE);