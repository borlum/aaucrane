% PLOT STEPS FROM ACC
clear all; close all;

%HAX: YES
cascade_wc_wh = 'http://172.26.12.144/data/acc/steps/1432049859-0.csv';
cascade_nc_wh = 'http://172.26.12.144/data/acc/steps/1432050002-0.csv';

%HAX: NO
cascade_wc_nh = 'http://172.26.12.144/data/acc/steps/1432050362-0.csv';
cascade_nc_nh = 'http://172.26.12.144/data/acc/steps/1432050189-0.csv';

%HAX: YES
parallel_wc_wh = 'http://172.26.12.144/data/acc/steps/1432123268-0.csv';
parallel_nc_wh = 'http://172.26.12.144/data/acc/steps/1432124208-0.csv';

%HAX: NO
parallel_wc_nh = 'http://172.26.12.144/data/acc/steps/1432050948-0.csv';
parallel_nc_nh = 'http://172.26.12.144/data/acc/steps/1432051059-0.csv';

%HAX + RAMP
parallel_wc_wh_wr = 'http://172.26.12.144/data/acc/steps/1432123706-0.csv';
parallel_nc_wh_wr = 'http://172.26.12.144/data/acc/steps/1432124494-0.csv';

[t  x  y  a] = grabData(parallel_nc_wh_wr);

[t1 x1 y1 a1] = grabData(parallel_nc_wh_wr);

xlim1 = 3.205 * ones(1, length(t));
xlim2 = 3.195 * ones(1, length(t));

alim1 = +0.087 * ones(1, length(t));
alim2 = -0.087 * ones(1, length(t));

alim3 = +0.004 * ones(1, length(t));
alim4 = -0.004 * ones(1, length(t));

ANG = 1;

if (ANG)
    p11 = plot(t, a);
    hold on;
    %p12 = plot(t1, a1 + 0.01);
    p2 = plot(t, alim1, 'k--');
    p3 = plot(t, alim2, 'k--');
    p4 = plot(t, alim3, 'r-');
    p5 = plot(t, alim4, 'r-');

    legend([p11 p2 p4], {'Measured \theta', 'Max. \theta when moving container', 'Max. \theta when picking/placing container'}, 'location', 'southeast');

    grid on;
    xlabel('t [s]');
    %ylabel('x [m]');
    ylabel('\theta [rad]');
    xlim([0 18]);
    title('Measured angle response to 3.2 m step');
    %title('Measured position response to 3.2 m step');
else
    p11 = plot(t, x - 0.405);
    hold on;
    %p12 = plot(t1, x1 - 0.4);
    p2 = plot(t, xlim1, 'k--');
    p3 = plot(t, xlim2, 'k--');

    legend([p11 p2], {'Measured x', 'Error band'}, 'location', 'southeast');

    grid on;
    xlabel('t [s]');
    ylabel('x [m]');
    xlim([0 18]);
    title('Measured position response to 3.2 m step');
end