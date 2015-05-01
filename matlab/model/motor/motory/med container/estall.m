% Runs all motory files and estimates parameters using senstool
clear all;
close;

par0 = [0.01 0.01];
for i = 4:6
  str = sprintf('motory%d', i);
  run(str)
  mainest
  parb(i-3) = pare(1);
  parI(i-3) = pare(2);
  erry(i-3) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
