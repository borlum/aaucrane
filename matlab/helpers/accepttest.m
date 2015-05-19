[t x y a] = grabData('http://172.26.12.144/data/acc/acc/1431681590-0.csv');
t = t - t(1);

pickup_x = [(ones(1,length(t)) * 13)];
pickup_y = [(ones(1,length(t)) * 1.05)];

pickup_y = linspace(0,4,length(t));

dropdown_x = [(ones(1,length(t)) * 36.5)];
dropdown_y = [(ones(1,length(t)) * 1.05)];

dropdown_y = linspace(0,4,length(t));

plot(t,x, t,y, pickup_x,pickup_y,'k', dropdown_x,dropdown_y,'k');

legend('x position', 'y position');
xlabel('Time [s]');
ylabel('Position [m]');
grid on;
