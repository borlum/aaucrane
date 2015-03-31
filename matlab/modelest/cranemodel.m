clear all; close all;
s = tf('s');

b = 0.004;
I = 0.0013;

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + b + I*s);

Gr = 7/144;
rr = 0.08; 

X = Gr * rr * 1/s;

mt = 0.5;
mp = 3.5;
l  = 0.65;
g  = 9.82;

% EST. STEP
%b = 0.25;
%Ip = 1.51;

% EST. SQUARE
b = -0.3113;
Ip = 0.9485;

W = (-mp*l*s^2)/((Ip * mp*l^2)*s^2 - b*s + mp*g*l);

C2 = 26.3;

inner = feedback(C2 * M * X, 1);


z = 0.0003;
wn = 3.9;

%C1 = (s^2 + 2*z*wn*s + wn^2)/(s + wn)^2;

C1 = (s^2 + wn^2)/(s + wn)^2;