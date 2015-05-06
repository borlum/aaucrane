function [] = simvsacc()
    %RAMP, P KONTROL, K = 10
    %[t2, x2, y2, theta2] = grabData('1430727416.csv');  
    %STEP, P KONTROL, K = 10
    %[t1, x1, y1, theta1] = grabData('1430726870.csv');

    %RAMP, PID KONTROL
    %[t3, x3, y3, theta3] = grabData('1430726940.csv');
    %STEP, PID KONTROL
    %[t4, x4, y4, theta4] = grabData('1430727000.csv'); 

    % Step P, 7.5 - NO RAMPT
    [t, x, y, theta] = grabData('1430913702-0.csv');    

    plot(t, x);

    %plot(t2, x2, t3, x3);
    %legend('P x', 'PID x');

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