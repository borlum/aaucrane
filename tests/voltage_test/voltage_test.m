data = csvread('data1.csv',1,0);
P1 = polyfit(data(:,3), data(:,1), 1);
P2 = polyfit(data(:,2), data(:,1), 1);

test_data = linspace(-14, 14, 100);

hold on;
plot(test_data, test_data * P1(1) + P1(2));
plot(test_data, test_data * P2(1) + P2(2));
plot(data(:,3), data(:,1), 'o');
plot(data(:,2), data(:,1), 'x');


FLUKE = polyfit(data(:,2), data(:,3), 1);
