function [t,x,y,theta, velx, vely, vx, vy] = grabData(file)
    TMP_FILE  = 'tmp.csv';
    websave(TMP_FILE, file);
    
    raw   = csvread(TMP_FILE, 2, 0);
    t     = raw(:,1) * 1e-6;
    theta = raw(:,3);
    x     = raw(:,4);
    y     = raw(:,5);
    velx    = raw(:,6);
    vely    = raw(:,7);
    vx    = raw(:,8);
    vy    = raw(:,9);
    
    delete(TMP_FILE);
end
