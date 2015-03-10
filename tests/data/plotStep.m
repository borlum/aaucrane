function [] = plotStep(file)
    if nargin < 1
        fprintf('No file given => expected input: plotStep("filename")\n');
        return;
    end

    stepData = grabData(file);

    plot(stepData.t / 1e9, stepData.phi);
    xlabel('Time [s]');
    ylabel('Voltage [V]');

    function [data] = grabData(file)
        raw = csvread(file, 1, 0);
        data.t = raw(:,1);
        data.phi = raw(:,2);
        data.x = raw(:,3);
    end
end