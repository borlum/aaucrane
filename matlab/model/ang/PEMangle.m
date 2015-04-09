clc
clear
close all
load measang % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = u((975):(40000)); %Input voltage
A = y((975):(40000))-y(976); %Output photo; Makeing a calculation in change of angle to find a velocity.
%%
%Create a data object to encapsulate the input/output data and their properties.
data = iddata(A, u, Ts); % For the x-axis

%%
%For the x-axis
data.Tstart = 0; % change starting time for first data
data.TimeUnit = 's'; % Change the units of the time vector

%%
%Parameters
Mp = 3.5;
l = 0.65;
g = 9.805;

x = Mp*l;

%%
%The system identification
sysA = idproc('P2ZU','InitialState','zero'); % For the angle

%%
%Determine the gain of TF.
sysA.Kp  = -x; % For the angle

%%
% Here we run the Prediction error method.
estimatedDataA = pem(data,sysA,'InitialState','zero') % For the angle

%%
% Compares the PEM results to the measured data.
compare(data,estimatedDataA,'InitialState','zero'); % For the angle

%figure
%Plot u, motor Currert and potenciometer voltage
%plot(u);hold on
%plot(A);