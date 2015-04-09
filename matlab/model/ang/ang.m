% XPOS MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

%GODE SQUARE PULSE RESPONSES
%1427714504.csv
%1427714996.csv   
%1427715304.csv


%websave(TMP_FILE, [CRANE_URL '/crane/xsquare/1428330885.csv']);
websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1428482290.csv']);



DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 120);
idx_end = idx_end(1);
t = t(1:idx_end);
t = linspace(0, t(end), length(t));

y = DATA(:,3) * 0.7367 - 1.3211;
y = y(1:idx_end);
y = y - y(1);

%u = (DATA(:,4) * -0.4981) + 4.7931;
%u = u(1:idx_end);


u = (DATA(:,8) * 2);
u = -u(1:idx_end) + u(1);

save measang t u y

process = 'ang';
%%
load measang % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(40000)); %Input voltage
A = y((975):(40000))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(40000));
plot(t, u);hold on
plot(t, A);
%%
delete(TMP_FILE);