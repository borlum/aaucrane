% Runs all motory files and estimates parameters using senstool


for i = 2:10
  str = sprintf('motory%d', i);
  run(str)
  mainest
  parb(i) = pare(1);
  parI(i) = pare(2);
  erry = errn;
end

meanb = mean(parb);
meanI = mean(parI);
