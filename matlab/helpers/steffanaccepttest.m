%ACCEPTANCE TEST
[t, x, y, theta] = grabData('http://172.26.12.144/data/acc/acc/1432291816-1.csv'); t=t-t(1);
%plot(t,x); grid on;
plot(t,y); grid on;
% hold off; plot(t,theta); 
% grid on; 
% xlabel('Time[s]'); 
% ylabel('Theta[rad]'); 
% title('Acceptance test of ACC'); 
% hold on; 
% plot([0 53.6],[-0.087 -0.087],'k--'); %Error band outer
% plot([14.86 14.86],[-0.1 0.1],'m'); %Y-action
% plot([20.14 20.14],[-0.1 0.1],'g'); %Y-done
% plot([0 53.6],[-0.004 -0.004],'r'); %Error band inner
% plot([0 53.6],[0.004 0.004],'y'); %Error band inner
% plot([20.76 20.76],[-0.1 0.1],'r'); %Y-action
% plot([28 28],[-0.1 0.1],'g'); %Y-done
% plot([42.8 42.8],[-0.1 0.1],'r'); %Y-action
% plot([48.7 48.7],[-0.1 0.1],'g'); %Y-done
% plot([49.15 49.15],[-0.1 0.1],'r'); %Y-action
% plot([53.6 53.6],[-0.1 0.1],'g'); %Y-done
% plot([0 53.6],[0.087 0.087],'k--'); %Error band outer
% legend('With container','Error band outer','y-action','y-done','Error band outer','Location','northeast');
