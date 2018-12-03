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

>> %装入图像
load tire

%下面进行噪声的产生
init=3718025452;
rand('seed',init);
Xnoise=X+18*(rand(size(X)));

%显示原始图像及它的含噪声的图像
colormap(map);
subplot(2,2,1);image(wcodemat(X,192));
title('原始图像')
axis square
subplot(2,2,2);image(wcodemat(Xnoise,192));
title('含噪声的图像');
axis square

%用sym5小波对图像信号进行二层的小波分解
[c,s]=wavedec2(X,2,'sym5');

%下面进行图像的去噪处理
%使用ddencmp函数来计算去噪的默认阈值和熵标准
%使用wdencmp函数来实现图像的压缩
[thr,sorh,keepapp]=ddencmp('den','wv',Xnoise);
[Xdenoise,cxc,lxc,perf0,perfl2]=wdencmp('gbl',c,s,'sym5',2,thr,sorh,keepapp);
%显示去噪后的图像
subplot(223);image(Xdenoise);
title('去噪后的图像');
axis square

>> %装入图像
load wbarb;
%显示图像
subplot(221);image(X);colormap(map)
title('原始图像');
axis square

disp('压缩前图像X的大小：');
whos('X')

%对图像用bior3.7小波进行2层小波分解
[c,s]=wavedec2(X,2,'bior3.7');

%提取小波分解结构中第一层低频系数和高频系数
ca1=appcoef2(c,s,'bior3.7',1);
ch1=detcoef2('h',c,s,1);
cv1=detcoef2('v',c,s,1);
cd1=detcoef2('d',c,s,1);

%分别对各频率成分进行重构
a1=wrcoef2('a',c,s,'bior3.7',1);
h1=wrcoef2('h',c,s,'bior3.7',1);
v1=wrcoef2('v',c,s,'bior3.7',1);
d1=wrcoef2('d',c,s,'bior3.7',1);
c1=[a1,h1;v1,d1];

%显示分解后各频率成分的信息
subplot(222);image(c1);
axis square
title('分解后低频和高频信息');

%下面进行图像压缩处理
%保留小波分解第一层低频信息，进行图像的压缩
%第一层的低频信息即为ca1，显示第一层的低频信息
%首先对第一层信息进行量化编码
ca1=appcoef2(c,s,'bior3.7',1);
ca1=wcodemat(ca1,440,'mat',0);

%改变图像的高度
ca1=0.5*ca1;
subplot(223);image(ca1);colormap(map);
axis square
title('第一次压缩');
disp('第一次压缩图像的大小为：');
whos('ca1')

%保留小波分解第二层低频信息，进行图像的压缩，此时压缩比更大
%第二层的低频信息即为ca2，显示第二层的低频信息
ca2=appcoef2(c,s,'bior3.7',2);

%首先对第二层信息进行量化编码
ca2=wcodemat(ca2,440,'mat',0);
%改变图像的高度
ca2=0.25*ca2;
subplot(224);image(ca2);colormap(map);
axis square
title('第二次压缩');
disp('第二次压缩图像的大小为：');
whos('ca2')
