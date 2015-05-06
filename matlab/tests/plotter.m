function [] = plotter()
  webpage = urlread('http://172.26.12.144/data/acc/steps/');
  pattern = '([0-9]{10}-[0-9]{0,2}.csv)';
  files = regexp(webpage, pattern, 'match');

  file = char(files(length(files)));

  [t x y theta] = grabData(file);

  figure;
  subplot(2,1,1);
  plot(t, x);
  title([file ' - x pos']);
  xlabel('Time [s]');
  ylabel('Position [m]');
  subplot(2,1,2);
  plot(t, theta);
  title([file ' - angle']);
  xlabel('Time [s]');
  ylabel('Angle [rad]');

  function [t,x,y,theta] = grabData(file)
    CRANE_URL = 'http://172.26.12.144/data';
    TMP_FILE  = 'tmp.csv';
    websave(TMP_FILE, [CRANE_URL '/acc/steps/' file]);
    
    raw   = csvread(TMP_FILE, 2, 0);
    t     = raw(:,1) * 1e-6;
    theta = raw(:,3);
    x     = raw(:,4);
    y     = raw(:,5);
    v     = raw(:,8);
    
    delete(TMP_FILE);
  end
end
