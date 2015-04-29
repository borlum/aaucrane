% Runs all ang files and estimates parameters using senstool
clear;

    
par0 = [0.001 0.001];

for i = 1:3
  str = sprintf('ang%d', i);
  run(str)
  mainest
  parb(i) = pare(1);
  parI(i) = pare(2);
  erra(i) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
