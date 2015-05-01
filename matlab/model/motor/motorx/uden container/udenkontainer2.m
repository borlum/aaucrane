CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429873806.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * -.4981 + 4.7931;
y = y(1:idx_end) - y(1);
u = DATA(:,8);
u = (-u(1:idx_end)) + u(idx_end);

save measudenkontainer2 t u y

process = 'udenkontainer2'

% plot(t, y)
% grid on;
% title('Movement response to 9.5 V');
% ylabel('X position [m]');
% xlabel('Time [s]');
% Found parameters: pare = [0.0065 0.002]
delete(TMP_FILE);