% MOTOR MODEL ESTIMATION
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429008454.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx1 t u y

process = 'motorx1';
%%
load measmotorx1 % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8600)); %Input voltage
x = y((975):(8600))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8600));
plot(t, x); hold on
%%
delete(TMP_FILE);
%%
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429008580.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx2 t u y

process = 'motorx2';
%%
load measmotorx2 % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8600)); %Input voltage
x = y((975):(8600))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8600));
plot(t, x); hold on
%%
delete(TMP_FILE);
%%
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429008757.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx3 t u y

process = 'motorx3';
%%
load measmotorx3 % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8600)); %Input voltage
x = y((975):(8600))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8600));
plot(t, x); hold on
%%
delete(TMP_FILE);
%%
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429008979.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx4 t u y

process = 'motorx4';
%%
load measmotorx4 % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8600)); %Input voltage
x = y((975):(8600))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8600));
plot(t, x); hold on
%%
delete(TMP_FILE);
%%
CRANE_URL = 'http://172.26.12.144/data';
TMP_FILE  = 'tmp.csv';

websave(TMP_FILE, [CRANE_URL '/crane/xsteps/1429009375.csv']);

DATA = csvread(TMP_FILE, 2, 0);
t = DATA(:,1) * 1e-6;

idx_end = find(t > 10);
idx_end = idx_end(1);

t = t(1:idx_end);

t = linspace(0,t(idx_end), idx_end);

y = DATA(:,4) * 34.8 - 4.0;
y = y(1:idx_end);
u = DATA(:,8) * 2;
u = (-u(1:idx_end)) + u(1);

save measmotorx5 t u y

process = 'motorx5';
%%
load measmotorx5 % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8600)); %Input voltage
x = y((975):(8600))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8600));
plot(t, x); hold on
%%
delete(TMP_FILE);