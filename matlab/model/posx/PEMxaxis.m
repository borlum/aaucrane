clc
clear
close all
load posxdata % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%Deciding what input and output parameters that are used
u = -u((975):(17000)); %Input voltage
x = y((975):(17000))-y(976); %Output potmeter; Makeing a calculation in change of angle to find a velocity.
%%
%Create a data object to encapsulate the input/output data and their properties.
ups = 0.0036909978715412545386*Ts+0.53572818329785902091;
datax = iddata(x, u-ups, Ts); % For the x-axis

%%
%For the x-axis
datax.Tstart = 0; % change starting time for first data
datax.TimeUnit = 's'; % Change the units of the time vector

%%
%Parameters
kt = 0.03;
ra = 0.43;
Grx = 7/144;
rrx = 0.08;
mm = 0.8; % Magnet + krog
mp = 2.866 + mm;
g  = 9.82;


%%
%The system identification
sysx = idproc('P2ZU','InitialState','zero'); % For the x-axis

%%
%Determine the gain of TF.
sysx.Kp  =((-1)/(3+g+(1/mp)))*Grx*rrx*(kt/ra); % For the x-axis
%sysx.Kp  =kt/ra; % For the x-axis

%%
% Here we run the Prediction error method.
estimatedDatax = pem(datax,sysx,'InitialState','zero') % For the x-axis

%%
% Compares the PEM results to the measured data.
compare(datax,estimatedDatax,'InitialState','zero'); % For the x-axis
