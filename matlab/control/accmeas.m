% CRANE MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/acc/steps/1428517047.csv']);

DATA = csvread(TMP_FILE, 2, 0);

DATA(end,:) = [];

t = DATA(:,1) * 1e-6;

t = linspace(0, t(end), length(t));

a = DATA(:,3) * 0.7367   - 1.3211;
x = DATA(:,4) *(-0.4981) + 4.7931;

u = (DATA(:,8) * 2);
u = -u + u(1);

delete(TMP_FILE);