% XPOS MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, 'http://172.26.12.144/data/crane/xsteps/1429009375.csv');

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 20);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0,t(idx_end), idx_end);

y = -DATA(:,4)*0.4981 + 4.7931;
y = y(1:idx_end);
y = (y - y(1));
u = DATA(:,8);
u = -u(1:idx_end);
u = u - u(length(u));

save posxdata t y u;
%%


% load posxdata % loads in the data
% s = tf('s');
% Ts = 0.01; % specifies time between data
% %%
% %Deciding what input and output parameters that are used
% u = -u((975):(17000)); %Input voltage
% x = y((975):(17000))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
% t = t((975):(17000));
% plot(t, u);hold on
% plot(t, x);
% %%
delete(TMP_FILE);
