% script_run_blind
% 盲去卷积复原
%%
clear,clc
close all

%% 产生棋盘格图像并进行退化
I = checkerboard(8);
PSF = fspecial('gaussian',7,10);
V = .0001;

% 退化
BlurredNoisy = imnoise(imfilter(I,PSF),'gaussian',0,V);

%% 复原
% 权值
Weight = zeros(size(I));
Weight(5:end-4,5:end-4) = 1;

% 点扩展函数的估计值
InitPSF = ones(size(PSF));
[J P] = deconvblind(BlurredNoisy,InitPSF,20,10*sqrt(V),Weight);
subplot(221);
imshow(BlurredNoisy);
title('退化图像');
subplot(222);
imshow(PSF,[]);
title('点扩展函数');
subplot(223);
imshow(J);
title('盲去卷积复原结果');
subplot(224);
imshow(P,[]);
title('输出的估计点扩展函数');
