clear;
close all;

run('../model/cranemodel');

%====================================
%			Med container
%====================================
% Tuning, lav settle time, lav overshoot
C1 = 3.75;

% Dæmp 4 rad/s
C2 = (s+4);

% Tuning - lav vinkel
k = 10;

XXm = feedback(C1 * Xm, 1-Wm*C2*k);

XVm = feedback(feedback(C1 * Xm, 1) * - Wm, C2 * k);	

dC2 = c2d(C2*k, .001, 'tustin');

%====================================
%			Uden container
%====================================
% Tuning, lav settle time, lav overshoot
C1 = 3.75;

% Dæmp 4 rad/s
C2 = (s+4);

% Tuning - lav vinkel
k = 10;

XXu = feedback(C1 * Xu, 1-Wu*C2*k);

XVu = feedback(feedback(C1 * Xu, 1) * - Wu, C2 * k);	

