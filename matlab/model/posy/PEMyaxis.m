clc
clear
close all
load posydata % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(8200)); %Input voltage
y = y((975):(8200))-y(976); %Output potmeter; Makeing a calculation in change of angle to find a velocity.
t = t((975):(8200));
%%
%Create a data object to encapsulate the input/output data and their properties.
datay = iddata(y, u, Ts); % For the y-axis

%%
%For the y-axis
datay.Tstart = 0; % change starting time for first data
datay.TimeUnit = 's'; % Change the units of the time vector

%%
%Parameters
Gry = 1/24;
rry = 0.05;
kt = 0.03;
ra = 0.43;
n = 1/2;
x = kt/ra*Gry*rry*n;

%%
%The system identification
sysy = idproc('P3Z','InitialState','zero'); % For the y-axis

%%
%Determine the gain of TF.
sysy.Kp  = x; % For the y-axis

%%
% Here we run the Prediction error method.
estimatedDatay = pem(datay,sysy,'InitialState','zero') % For the y-axis

%%
% Compares the PEM results to the measured data.
compare(datay,estimatedDatay,'InitialState','zero'); % For the y-axis

%figure
%Plot u, motor Currert and potenciometer voltage
%plot(u);hold on
%plot(y);
%%
