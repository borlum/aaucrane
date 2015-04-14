CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/manual/1429012849.csv']);

DATA = csvread(TMP_FILE, 2, 0);

t = DATA(:,1) * 1e-6;
t = linspace(0, t(end), length(t));

a = DATA(:,3) * 0.7367    - 1.3211;
x = DATA(:,4) * (-0.4981) + 4.7931;
y = DATA(:,5) * (-0.1536) + 1.2106;

plot(t,a);