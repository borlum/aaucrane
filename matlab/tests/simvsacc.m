function [] = simvsacc()

    % Plot ACC step
    [t1, x1, y1, theta1] = grabData('1430483951.csv');
    [t2, x2, y2, theta2] = grabData('1430485866.csv');

    plot(t1, theta1);
    hold on;
    plot(t2, theta2);

    function [t,x,y,theta] = grabData(test)
        CRANE_URL = 'http://172.26.12.144/data';
        TMP_FILE  = 'tmp.csv';
        websave(TMP_FILE, [CRANE_URL '/acc/steps/' test]);
        
        raw   = csvread(TMP_FILE, 2, 0);
        t     = raw(:,1) * 1e-6;
        theta = raw(:,3);
        x     = raw(:,4);
        y     = raw(:,5);
        v     = raw(:,8);

        delete(TMP_FILE);
    end
end