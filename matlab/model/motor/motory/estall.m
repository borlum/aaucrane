% Runs all motory files and estimates parameters using senstool


for i = 1:6
  str = sprintf('motory%d', i);
  run(str)
  mainest
  parb(i) = pare(1);
  parI(i) = pare(2);
  erry(i) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
