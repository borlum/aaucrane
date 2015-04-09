% XPOS MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';
websave(TMP_FILE, [CRANE_URL '/crane/xsquare/1428330885.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0,t(idx_end), idx_end);

y = DATA(:,5)*(-0.15) + 1.85;
y = y(1:idx_end);
u = DATA(:,9);
u = (u(1:idx_end)) + u(20);

save posydata t u y
%%
load posydata % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8200)); %Input voltage
y = y((975):(8200))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8200));
plot(t, u);hold on
plot(t, y);
%%
delete(TMP_FILE);