% GRAB DATA FROM CRANE
%a = [-25 -20 -10 -5 0 5 10 15 20];
%u = [1.301 1.350 1.604 1.799 1.917 2.088 2.205 2.500 2.689];

a1 = [-15 -10 -5 0 5 10 15]; a1 = a1 * (pi/180);
u1 = [1.27 1.38 1.57 1.85 2.07 2.12 2.52];

a2 = [-15 -10 -5 0 5 10 15]; a2 = a2 * (pi/180);
u2 = [1.23 1.38 1.57 1.84 2.06 2.11 2.49];

a = (a1 + a2) / 2;
u = (u1 + u2) / 2;


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