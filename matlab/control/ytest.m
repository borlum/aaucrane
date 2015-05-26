%#Y
%Op, container, calculated values
[t, x, y, theta] = grabData('http://172.26.12.144/data/acc/steps/1432127424-0.csv');
%Ned, container, calval 
[t1, x1, y1, theta1] = grabData('http://172.26.12.144/data/acc/steps/1432127283-0.csv');
%Op, no cont, calval
[t2, x2, y2, theta2] = grabData('http://172.26.12.144/data/acc/steps/1432127154-0.csv');
%Ned, no cont, calval
[t3, x3, y3, theta3] = grabData('http://172.26.12.144/data/acc/steps/1432127048-0.csv');
%Op, no cont, fifleval
[t4, x4, y4, theta4] = grabData('http://172.26.12.144/data/acc/steps/1432127868-0.csv'); y4 = y4+0.003;
%Ned, no cont, fifleval
[t5, x5, y5, theta5] = grabData('http://172.26.12.144/data/acc/steps/1432127815-0.csv');
%Op, cont, fifleval
[t6, x6, y6, theta6] = grabData('http://172.26.12.144/data/acc/steps/1432127680-0.csv');
%Ned, cont, fifleval
[t7, x7, y7, theta7] = grabData('http://172.26.12.144/data/acc/steps/1432127605-0.csv');
%
%OP no fiffel
plot(t,y,t2,y2); grid on; xlabel('Time[s]'); ylabel('y[m]'); title('Measured position response to 0.78 m step'); xlim([0 10]); hold on; plot([0 10],[0.128 0.128],'k--'); plot([5.5 5.5],[0 1],'k'); plot([0 10],[0.132 0.132],'k--'); legend('With container','Without container','Error band','Settle time','Location','northeast');
%NED no fiffel
%plot(t1,y1,t3,y3); grid on; xlabel('Time[s]'); ylabel('y[m]'); title('Measured position response to 0.78 m step'); xlim([0 10]); hold on; plot([0 10],[0.908 0.908],'k--'); plot([5.5 5.5],[0 1],'k'); plot([0 10],[0.912 0.912],'k--'); legend('With container','Without container','Error band','Settle time','Location','southeast');
%OP fiffel
%plot(t6,y6,t4,y4); grid on; xlabel('Time[s]'); ylabel('y[m]'); title('Measured position response to 0.78 m step'); xlim([0 10]); hold on; plot([0 10],[0.128 0.128],'k--'); plot([5.5 5.5],[0 1],'k'); plot([0 10],[0.132 0.132],'k--'); legend('With container','Without container','Error band','Settle time','Location','northeast');
%NED fiffel
%plot(t7,y7,t5,y5); grid on; xlabel('Time[s]'); ylabel('y[m]'); title('Measured position response to 0.78 m step'); xlim([0 10]); hold on; plot([0 10],[0.908 0.908],'k--'); plot([5.5 5.5],[0 1],'k'); plot([0 10],[0.912 0.912],'k--'); legend('With container','Without container','Error band','Settle time','Location','southeast');
