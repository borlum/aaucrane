%%
% Fetch cascade results
[cascade_w_hax_w_container.t cascade_w_hax_w_container.x cascade_w_hax_w_container.y cascade_w_hax_w_container.a] = grabData('http://172.26.12.144/data/acc/steps/1432049859-0.csv');
[cascade_w_hax.t cascade_w_hax.x cascade_w_hax.y cascade_w_hax.a] = grabData('http://172.26.12.144/data/acc/steps/1432050002-0.csv');
[cascade.t cascade.x cascade.y cascade.a] = grabData('http://172.26.12.144/data/acc/steps/1432050189-0.csv');
[cascade_w_container.t cascade_w_container.x cascade_w_container.y cascade_w_container.a] = grabData('http://172.26.12.144/data/acc/steps/1432050362-0.csv');
%%
% Fetch parallel results
[parallel_w_container.t parallel_w_container.x parallel_w_container.y parallel_w_container.a] = grabData('http://172.26.12.144/data/acc/steps/1432050948-0.csv');
[parallel.t parallel.x parallel.y parallel.a] = grabData('http://172.26.12.144/data/acc/steps/1432051059-0.csv');
[parallel_w_hax.t parallel_w_hax.x parallel_w_hax.y parallel_w_hax.a] = grabData('http://172.26.12.144/data/acc/steps/1432051895-0.csv');
[parallel_w_hax_w_container.t parallel_w_hax_w_container.x parallel_w_hax_w_container.y parallel_w_hax_w_container.a] = grabData('http://172.26.12.144/data/acc/steps/1432051821-0.csv');

%%
% plot stuff

% With hax and container  - BAD
figure;
plot(cascade_w_hax_w_container.t, cascade_w_hax_w_container.a, parallel_w_hax_w_container.t, parallel_w_hax_w_container.a);
title('With container')
legend('Cascade design', 'Parallel design')
xlabel('Time [s]')
ylabel('Angle [rad]')
print('controller_test_w_hax_w_container_angle.pdf', '-dpdf');
figure;
plot(cascade_w_hax_w_container.t, cascade_w_hax_w_container.x, parallel_w_hax_w_container.t, parallel_w_hax_w_container.x);
title('With container')
legend('Cascade design', 'Parallel design')
xlabel('Time [s]')
ylabel('x position [m]')
print('controller_test_w_hax_w_container_pos.pdf', '-dpdf');

% With hax, no container - BAD
figure;
plot(cascade_w_hax.t, cascade_w_hax.a, parallel_w_hax.t, parallel_w_hax.a);
title('Without container')
legend('Cascade design', 'Parallel design')
xlabel('Time [s]')
ylabel('Angle [rad]')
print('controller_test_w_hax_wo_container_angle.pdf', '-dpdf');
figure;
plot(cascade_w_hax.t, cascade_w_hax.x, parallel_w_hax.t, parallel_w_hax.x);
title('Without container')
legend('Cascade design', 'Parallel design')
xlabel('Time [s]')
ylabel('x position [m]')
print('controller_test_w_hax_wo_container_pos.pdf', '-dpdf');

% % no hax, with container
% figure;
% plot(cascade_w_container.t, cascade_w_container.a, parallel_w_container.t, parallel_w_container.a);
% title('NO hax with container - ANGLE')
% figure;
% plot(cascade_w_container.t, cascade_w_container.x, parallel_w_container.t, parallel_w_container.x);
% title('NO hax with container - pos')

% % no hax, no container
% figure;
% plot(cascade.t, cascade.a, parallel.t, parallel.a);
% title('NO hax NO container - ANGLE')
% figure;
% plot(cascade.t, cascade.x, parallel.t, parallel.x);
% title('NO hax NO container - pos')
