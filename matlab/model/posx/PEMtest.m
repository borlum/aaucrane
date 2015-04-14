clc
clear
close all
load posxdata % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%u Input voltage
%y Output potmeter

%%

u = -u((975):(16000));
x = y((975):(16000))-y(976); %Makeing a calculation in change of angle to find a velocity.
%%

datax = iddata(x, u, Ts); % For the x-axis
datay = iddata(y, u, Ts); % For the y-axis
dataA = iddata(A, u, Ts); % For the angle
%%
%For the x-axis
datax.Tstart = 0; % change starting time for first data
datax.TimeUnit = 's'; % Change the units of the time vector
%For the y-axis
datay.Tstart = 0; % change starting time for first data
datay.TimeUnit = 's'; % Change the units of the time vectordatax.Tstart = 0; % change starting time for first data
%For the angle
dataA.Tstart = 0; % change starting time for first data
dataA.TimeUnit = 's'; % Change the units of the time vector
%%
%Parameters
Grx = 7/144;
rrx = 0.08;
Gry = 1/24;
rry = 0.05;
kt = 0.03;
ra = 0.43;
x = -1;
%%

sysx = idproc('P3Z','InitialState','zero'); % For the x-axis
sysy = idproc('P2U','InitialState','zero'); % For the y-axis
sysA = idproc('P2Z','InitialState','zero'); % For the angle

%%
%Determine the gain of each TF.
sysx.Kp  =Gr*kt*rr/ra; % For the x-axis
sysy.Kp  =Gr*kt*rr/ra; % For the y-axis
sysA.Kp  =-1; % For the angle

%%
% Here we run the Prediction error method.
estimatedDatax = pem(datax,sysx,'InitialState','zero'); % For the x-axis
estimatedDatay = pem(datax,sysy,'InitialState','zero'); % For the y-axis
estimatedDataA = pem(datax,sysA,'InitialState','zero'); % For the angle

%%
% Compares the PEM results to the measured data.
compare(datax,estimatedDatax,'InitialState','zero'); % For the x-axis
compare(datax,estimatedDatay,'InitialState','zero'); % For the y-axis
compare(datax,estimatedDataA,'InitialState','zero'); % For the angle

%figure
%Plot u, motor Currert and potenciometer voltage
plot(u);hold on
plot(x);
%%
