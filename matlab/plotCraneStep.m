% PLOT CRANE STEP
function [] = plotCraneStep(test)
    close all;
    if nargin < 1
        fprintf('No test given => expected: plotCraneStep("testnumber")\n');
        return;
    end

    plotAngleStep(grabData(test));
    %plotXVStep(grabData(test));

    function [] = plotXStep(stepData)
        plot(stepData.t, stepData.x);
        xlabel('Time [s]');
        ylabel('Voltage [V]');
        tmp = ['Step response (' num2str(stepData.xin) ' V input, x-axis)'];
        title(tmp);
        xlim([0 stepData.t(end)]);
        ylim([min(stepData.x) max(stepData.x)]);
        grid on;
    end

    function [] = plotXVStep(stepData)
        plot(stepData.t, stepData.xv);
        xlabel('Time [s]');
        ylabel('Voltage [V]');
        tmp = ['Step response (' num2str(stepData.xin) ' V input, dx/dt)'];
        title(tmp);
        xlim([0 stepData.t(end)]);
        ylim([min(stepData.xv) max(stepData.xv)]);
        grid on;
    end

    function [] = plotYStep(stepData)
        plot(stepData.t, stepData.y);
        xlabel('Time [s]');
        ylabel('Voltage [V]');
        tmp = ['Step response (' num2str(stepData.yin) ' V input, y-axis)'];
        title(tmp);
        xlim([0 stepData.t(end)]);
        ylim([min(stepData.y) max(stepData.y)]);
        grid on;
    end

    function [] = plotAngleStep(stepData)
        plot(stepData.t, stepData.phi);
        xlabel('Time [s]');
        ylabel('Voltage [V]');
        tmp = ['Step response (' num2str(stepData.xin) ' V input, angle)'];
        title(tmp);
        xlim([0 stepData.t(end)]);
        ylim([min(stepData.phi) max(stepData.phi)]);
        grid on;
    end

    function [data] = grabData(test)
        CRANE_URL = 'http://172.26.12.144/data';
        TMP_FILE  = 'tmp.csv';
        websave(TMP_FILE, [CRANE_URL '/crane/xsteps/' num2str(test) '.csv']);
        raw = csvread(TMP_FILE, 1, 0);
        data.t   = raw(:,1) * 1e-6;
        data.phi = raw(:,3);
        data.x   = raw(:,4);
        data.y   = raw(:,5);
        data.xv  = raw(:,6);
        data.yv  = raw(:,7);
        data.xin = raw(:,8);
        data.xin = abs(data.xin(end-5) * 2);
        data.yin = raw(:,9);
        data.yin = abs(data.yin(end-5) * 2);
        delete(TMP_FILE);
    end
end