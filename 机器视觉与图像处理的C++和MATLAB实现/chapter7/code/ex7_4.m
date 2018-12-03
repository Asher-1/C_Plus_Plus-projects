%装入图像
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
