function [t,x,y,theta, vx, vy] = grabData(file)
    TMP_FILE  = 'tmp.csv';
    websave(TMP_FILE, file);
    
    raw   = csvread(TMP_FILE, 2, 0);
    t     = raw(:,1) * 1e-6;
    theta = raw(:,3);
    x     = raw(:,4);
    y     = raw(:,5);
    vx    = raw(:,8);
    vy    = raw(:,9);
    
    delete(TMP_FILE);
end
