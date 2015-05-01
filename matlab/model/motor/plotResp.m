% SQUARE PULSE MEASUREMENTS
function [] = plotResp()
    
    % X POS RESPONSE, COMPARISON
    % --------------------------
    %[t_m, y_m, u_m] = getData('1429008757.csv', 10);
    %[t_u, y_u, u_u] = getData('1429874033.csv', 10);

    %y_u = -y_u;
    %u_u = -u_u;

    %plot(t_m, y_m, t_u, y_u);
    %title('Trolley position to square pulse @ 6.5 V');
    %ylabel('x [m]');
    %xlabel('t [s]');
    %xlim([t_m(1) t_m(end)]);
    %grid on;
    %legend('Container: yes', 'Container: no', 'location', 'southeast');

    % ANGLE RESPONSE, COMPARISON
    % --------------------------
    %[t_m, y_m, u_m] = getData('1429008757.csv', 50);
    %[t_u, y_u, u_u] = getData('1429874033.csv', 50);

    %y_u = -y_u;
    %u_u = -u_u;

    %plot(t_m, y_m, t_u, y_u);
    %title('Wire angle to square pulse @ 6.5 V');
    %ylabel('\theta [rad]');
    %xlabel('t [s]');
    %xlim([t_m(1) t_m(end)]);
    %grid on;
    %legend('Container: yes', 'Container: no', 'location', 'southeast');

    % Y POS RESPONSE, COMPARISON
    % --------------------------
    [t_n, y_n, u_n] = getData('1429087361.csv', 5);
    [t_o, y_o, u_o] = getData('1429087470.csv', 5);

    y_o = -y_o;
    u_o = -u_o;

    plot(t_n, y_n, t_o, y_o);
    title('Magnet position to square pulse @ 6.5 V');
    ylabel('y [m]');
    xlabel('t [s]');
    xlim([t_n(1) t_n(end)]);
    grid on;
    legend('Direction: down', 'Direction: up', 'location', 'southeast');


    function [t, y, u] = getData(file, t_stop)
        CRANE_URL = 'http://172.26.12.144/data';
        TMP_FILE  = 'tmp.csv';

        websave(TMP_FILE, [CRANE_URL '/crane/ysteps/' file]);

        DATA = csvread(TMP_FILE, 2, 0);
        t = DATA(:,1) * 1e-6;

        idx_end = find(t > t_stop);
        idx_end = idx_end(1);

        t = t(1:idx_end);

        t = linspace(0,t(idx_end), idx_end);

        %XPOS
        %y = DATA(:,4) * -.4981 + 4.7931;
        %y = y(1:idx_end) - y(1);

        %YPOS
        y = DATA(:,5) *(-0.1536) + 1.2106;
        y = y(1:idx_end) - y(1);

        %ANGLE
        %y = 0.7367*DATA(:,3) - 1.3835;
        %y = y(1:idx_end) - y(1);

        %MOTORX
        %u = DATA(:,8);
        %u = (-u(1:idx_end)) + u(idx_end);

        %MOTORY
        u = DATA(:,9);
        u = (-u(1:idx_end)) + u(idx_end);

        delete(TMP_FILE);
    end

end