% Runs all ang files and estimates parameters using senstool
clear;

    
par0 = [-0.1677 3.0413];

for i = 5:5
  str = sprintf('ang%d', i);
  run(str)
  mainest
  parb(i-4) = pare(1);
  parI(i-4) = pare(2);
  erra(i-4) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
