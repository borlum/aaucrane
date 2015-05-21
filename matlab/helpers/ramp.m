function [time ramp step] = ramp(step_size)
  milisec_before = 500;
  speed = 0.0025;
  s = 0;
  j = milisec_before;

  ramp(1:j) = 0;
  step(1:j) = 0;
  time = linspace(-500,1500, 2000/1) / 100;
  while ramp(end) < step_size
    ramp(j) = s;
    s = s + speed;
    j = j + 1;
  end
  ramp(end) = step_size;

  
  for i = j:milisec_before + 1500
    ramp(i) = ramp(end);
  end

  step(milisec_before:2000) = step_size;

  plot(time,ramp, time,step);
  legend('Ramp reference', 'Step reference');
  xlabel('Time [s]');
  ylabel('Distance [m]');
  title('Reference types');
  
end
