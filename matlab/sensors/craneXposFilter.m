function [] = craneFilter()
    Fs = 1000;
    [t1, x1, y1, theta1] = grabData('1430816355.csv'); 
    [t2, x2, y2, theta2] = grabData('1430816567.csv');

    [fx, Yx] = onesidedFFT(t1, x1, Fs);
    [fy, Yy] = onesidedFFT(t2, x2, Fs);
    [fa, Ya] = onesidedFFT(t1, theta1, Fs);

    plot(fx, Yx);

    [buttNum buttDenom] = butter(2, 2*pi*0.5, 's');
    Filter = tf(buttNum, buttDenom)
    bode(Filter);

    zeta = 0.707;
    wn = 0.3948;
    gain = 1;

    function [f, Y] = onesidedFFT(t, y, fs)
        NFFT = 2^nextpow2(length(y));
        Y = fft(y,NFFT)/length(y);
        f = fs/2*linspace(0,1,NFFT/2+1);
        Y = 2*abs(Y(1:NFFT/2+1));
    end

    function [t,x,y,theta] = grabData(test)
        CRANE_URL = 'http://172.26.12.144/data';
        TMP_FILE  = 'tmp.csv';
        websave(TMP_FILE, [CRANE_URL '/crane/xsteps/' test]);
        
        raw   = csvread(TMP_FILE, 2, 0);
        t     = raw(:,1) * 1e-6;
        theta = raw(:,3);
        x     = raw(:,4);
        y     = raw(:,5);
        v     = raw(:,8);

        delete(TMP_FILE);
    end
end