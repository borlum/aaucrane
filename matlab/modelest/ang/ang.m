% XPOS MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

%GODE SQUARE PULSE RESPONSES
%1427714504.csv
%1427714996.csv   
%1427715304.csv

websave(TMP_FILE, [CRANE_URL '/crane/x_square/1427800240.csv']);
%websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1427719295.csv']);


DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0, t(end), length(t));

y = (DATA(:,3) - 1.85) * 0.31;
y = y(1:idx_end);

u = (DATA(:,8) * 2);
u = -1*u(1:idx_end) + u(1);

save measang t u y

process = 'ang';

delete(TMP_FILE);