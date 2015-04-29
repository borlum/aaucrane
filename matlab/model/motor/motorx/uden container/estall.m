% Runs all motory files and estimates parameters using senstool


for i = 1:3
  str = sprintf('udenkontainer%d', i);
  run(str)
  mainest
  parb(i) = pare(1);
  parI(i) = pare(2);
  errx(i) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
