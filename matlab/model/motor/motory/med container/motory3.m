%%
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1431433123.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 6);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,5);
y = y(1:idx_end) - y(1);
u = DATA(:,9);
u = u(1:idx_end);

save measmotory3 t u y

process = 'motory3'

delete(TMP_FILE);

% Senstool params: pare = [.0028 .0013]
