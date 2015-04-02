clear all; close all;

s = tf('s');

b = 0.0015;
I = 0.0009;

kt = 0.03;
ra = 0.43;

M = (kt/ra)/(kt^2/ra + b + I*s);

Gr = 1/24; %?
rr = 0.05; %?

Y = M * Gr * rr * 1/2 * 1/s;

CL = feedback(80 * Y, 1);