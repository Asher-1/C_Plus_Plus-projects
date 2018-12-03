%装载信号
load nelec;  
indx=1:1024;
x=nelec(indx);

% 用小波haar对信号进行三层分解
[c,l]=wavedec(x,3,'haar');  

alpha=1.5;
% 获取信号压缩的阈值
[thr,nkeep]=wdcbm(c,l,alpha);

% 对信号进行压缩
[xd,cxd,lxd,perf0,perfl2]=wdencmp('lvd',c,l,'haar',3,thr,'s');  
subplot(2,1,1);  
plot(indx,x);  
title('原始信号');  

subplot(2,1,2);  
plot(indx,xd);  
title('压缩后的信号');
