% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

% http://172.26.12.144/data/crane/xsteps/1431161920.csv
websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1431161920.csv']);


DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 15);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

u = DATA(:,4);
u = u(1:idx_end) - u(1);
y = DATA(:,3);
y = y(1:idx_end);

save measang1 t u y

plot(t, y)
grid on;

delete(TMP_FILE);

process = 'ang1'

%%
