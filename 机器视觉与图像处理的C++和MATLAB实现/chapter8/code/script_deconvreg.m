% script_deconvreg.m
% 约束最小二乘复原
%%
clear,clc
close all

%% 产生退化图像
I=checkerboard(8);

% 运动模糊的点扩散函数
PSF = fspecial('motion', 7, 45);

fprintf('点扩散函数:\n');
disp(PSF)

% 对图像进行运动模糊滤波
Im1 = imfilter(I, PSF, 'circular');

% 添加高斯噪声
noise = imnoise(zeros(size(I)), 'gaussian', 0, 0.001);
Im = Im1 + noise;

%% 维纳滤波
Iw = deconvwnr(Im,PSF,0.02);

%% 约束最小二乘滤波
I=edgetaper(I,PSF);
Iz = deconvreg(Im, PSF, 0.2,[1e-7, 1e7]);

%% 绘图
subplot(221);
imshow(I,[])
title('原始图像');

subplot(222);
imshow(Im, [])
title('退化图像');

subplot(223)
imshow(Iw,[])
title('维纳滤波')

subplot(224)
imshow(Iz,[])
title('约束最小二乘滤波')
