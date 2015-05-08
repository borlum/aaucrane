clear;
close all;

run('../model/cranemodel');

%====================================
%			Med container
%====================================
% Tuning, lav settle time, lav overshoot
C1 = 5;

tp = 1;

td = 0;

% Dæmp 4 rad/s
C2 = (td * s+ tp);

% Tuning - lav vinkel
k = 20;

XXm = feedback(C1 * Hx, 1-W*C2*k);

XVm = feedback(feedback(C1 * Hx, 1) * -W, C2 * k);	

dC2 = c2d(C2*k, .01, 'tustin');

%====================================
%			Uden container
%====================================
% Tuning, lav settle time, lav overshoot
% C1 = 3.75;

% tp = .4;

% td = 1;

% % Dæmp 4 rad/s
% C2 = (s*td+tp);

% % Tuning - lav vinkel
% k = 10;

% XXu = feedback(C1 * Xu, 1-Wu*C2*k);

% XVu = feedback(feedback(C1 * Xu, 1) * - Wu, C2 * k);