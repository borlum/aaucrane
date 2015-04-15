% Runs all motory files and estimates parameters using senstool


for i = 1:5
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
