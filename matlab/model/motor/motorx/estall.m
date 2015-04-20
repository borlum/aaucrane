% Runs all motory files and estimates parameters using senstool


for i = 3:5
  str = sprintf('motorx%d', i);
  run(str)
  mainest
  parb(i-2) = pare(1);
  parI(i-2) = pare(2);
  errx(i-2) = errn;
  par0 = pare;
end

meanb = mean(parb);
meanI = mean(parI);
