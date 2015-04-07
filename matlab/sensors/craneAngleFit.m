% GRAB DATA FROM CRANE
ZERO_PIXEL = 285;

% PIXELS, CONVERTED TO ANGLE + RESPECTIVE COMEDI-READ VOLTAGES!
p = [0     50    100   150   200   250   285   300   350   400   450   500   550];
a = asin(((p-ZERO_PIXEL) / 15)/25);
u = [0.674 0.869 1.067 1.263 1.458 1.653 1.7899 1.849 2.044 2.239 2.440 2.630 2.830];

[p, s] = polyfit(u, a, 1);
% x = -0.4981*u + 4.7931

a_est = polyval(p, u);

%x_est = p(1) * u + p(2);

r2 = 1 - s.normr^2 / norm(a-mean(a))^2

% PLOT
plot(u, a, '*', u, a_est);
title('Angle sensor');
xlabel('Voltage [V]');
ylabel('\theta [rad]');
xlim([0 3.3]);
ylim([a(1) a(end)]);
grid on;
legend('Measured', 'Linear regression', 'location', 'southeast');