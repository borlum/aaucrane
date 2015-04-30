% Runs all ang files and estimates parameters using senstool
clear;

    
par0 = [0.01 0.01];

for i = 3:5
  str = sprintf('ang%d', i);
  run(str)
  mainest
  parb(i-2) = pare(1);
  parI(i-2) = pare(2);
  erra(i-2) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
