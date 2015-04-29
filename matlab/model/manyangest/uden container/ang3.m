% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429874033.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 40);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

u = DATA(:,4) * -.4981 + 4.7931;
u = u(1:idx_end) - u(1);
y = DATA(:,3);
y = 0.7367 * y(1:idx_end) - 1.411;

save measang3 t u y

delete(TMP_FILE);

process = 'ang3'

%%
