clc
clear
close all
load posxdata % loads in the data
s = tf('s');
Ts = 0.01; % specifies time between data
%%
%u Input voltage
%y Output potmeter
%Vel Velocity from position
%%

u = -u;
y = y-y(1);
%%

%data = iddata(Curr_neg_A_filtered, u, Ts);
data = iddata(y, u, Ts);
%%

data.Tstart = 0; % change starting time for first data
data.TimeUnit = 's'; % Change the units of the time vector

Gr = 7/144;
rr = 0.08;
kt = 0.03;
ra = 0.43;

%sys = idproc('P3Z','InitialState','zero');
sys1 = idproc('P2U','InitialState','zero');
sys2 = idproc('P2Z','InitialState','zero');
%sys3 = idproc('P3ZU','InitialState','zero');
%sys.Kp  =Gr*kt*rr/ra;
sys1.Kp  =Gr*kt*rr/ra;
sys2.Kp  =Gr*kt*rr;
%sys3.Kp  =kt*Gr*rr/ra;
%%
% Here we run the Prediction error method.
%estimatedData = pem(data,sys,'InitialState','zero');
estimatedData1 = pem(data,sys1,'InitialState','zero');
estimatedData2 = pem(data,sys2,'InitialState','zero');
%estimatedData3 = pem(data,sys3,'InitialState','zero');
%%
% Compares the PEM results to the measured data.
compare(data,estimatedData,'InitialState','zero');
compare(data,estimatedData1,'InitialState','zero');
compare(data,estimatedData2,'InitialState','zero');
compare(data,estimatedData3,'InitialState','zero');

%Plot u, motor Currert and potenciometer voltage

%%
