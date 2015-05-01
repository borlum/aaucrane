function [] = simvsacc()

    % Plot ACC step
    [t, x, y, theta] = grabData('1430469544.csv');

    plot(t, theta);

    function [t,x,y,theta] = grabData(test)
        CRANE_URL = 'http://172.26.12.144/data';
        TMP_FILE  = 'tmp.csv';
        websave(TMP_FILE, [CRANE_URL '/acc/steps/' test]);
        
        raw   = csvread(TMP_FILE, 2, 0);
        t     = raw(:,1) * 1e-6;
        theta = raw(:,3)*(0.7367)  - 1.3835;
        x     = raw(:,4)*(-0.4981) + 4.7931;
        y     = raw(:,5)*(-0.1536) + 1.2106;

        delete(TMP_FILE);
    end
end