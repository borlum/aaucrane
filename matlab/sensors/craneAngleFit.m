% GRAB DATA FROM CRANE
ZERO_PIXEL_M = 277;
ZERO_PIXEL_U = 292;

% PIXELS, CONVERTED TO ANGLE + RESPECTIVE COMEDI-READ VOLTAGES!
p = [188   208   228   248   268   288   308   328   348   368   376];
u = [0.720 0.950 1.187 1.418 1.653 1.887 2.125 2.361 2.590 2.825 2.923];

a = asin( ( (p-ZERO_PIXEL_M) / 15 ) / 25 );


[p, s] = polyfit(u, a, 1);
% x = -0.4981*u + 4.7931

a_est = polyval(p, u);

%x_est = p(1) * u + p(2);

r2 = 1 - s.normr^2 / norm(a-mean(a))^2

% PLOT
plot(u, a, '*', u, a_est);

xlabel('Sensor voltage [V]');
ylabel('\theta [rad]');
xlim([0.5 7.5]);
ylim([-1 1]);

grid on;
legend('Measured', 'Linear regression', 'location', 'southeast');
