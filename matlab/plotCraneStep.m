% PLOT CRANE STEP
function [] = plotCraneStep(test)
    close all;
    if nargin < 1
        fprintf('No test given => expected: plotCraneStep("testnumber")\n');
        return;
    end

    plotYStep(grabData(test))
    %plotAngleAndX(grabData(test));
    %plotCraneMovement(grabData(test));
    %plotAngleStep(grabData(test));
    %plotXStep(grabData(test));

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
        %xlim([0 stepData.t(end)]);
        ylim([min(stepData.phi) max(stepData.phi)]);
        grid on;
    end

    function [] = plotCraneMovement(stepData)
      x_pos = 0.5 * stepData.x - 0.8;
      y_pos = -0.1348 * stepData.y + 0.2395 + 10*0.1348;
      %figure;

      %subplot(2,1,1);
      %plot(stepData.t, stepData.x, stepData.t, stepData.y, stepData.t, stepData.phi);
      %legend('X_{pos}', 'Y_{pos}', 'Load angle');
      %title('Crane sensor data');

      %subplot(2,1,2)
      plot(x_pos, y_pos, stepData.t, stepData.phi);
      title('Movement of the crane');
      set(gca,'YDir','reverse');
      ylim([0, 10*0.1348]);
      xlim([0, 4]);
    end

    function [] = plotAngleAndX(stepData);
      x_pos = 0.4841 * stepData.x - 0.6302;
      y_pos = -0.1348 * stepData.y + 0.2395 + 10*0.1348;
      theta = (0.31 * stepData.phi - 1.63);

      [ax,h1,h2] = plotyy(stepData.t, [x_pos, y_pos], stepData.t, theta);
      set(gca,'YDir','reverse');
      legend('x', 'y', '\theta');
      title('Crane sensor data');
      set(ax(1), 'YLim', [0 4])
      ylabel(ax(1), '[m]');
      set(ax(2), 'YLim', [-1.3 0.5])
      ylabel(ax(2), '[rad]');
      xlabel('[s]');

    end
    
    function [data] = grabData(test)
        TMP_FILE  = 'tmp.csv';
        websave(TMP_FILE, test);
        raw = csvread(TMP_FILE, 2, 0);
        data.t   = raw(:,1) * 1e-6;
        data.theta = raw(:,2);
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
