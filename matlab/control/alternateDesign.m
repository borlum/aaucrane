clear;
%close all;

run('../model/cranemodel');

%====================================
%			Med container
%====================================
% Tuning, lav settle time, lav overshoot

C1 = 5;

k = 5;

p = 4;

d = 1;

C2 = k * (s * d + p);

dC2 = c2d(C2, .01, 'tustin');

XX = feedback(C1*feedback(Hx,-W*C2), 1);

XV = XX * -W;

