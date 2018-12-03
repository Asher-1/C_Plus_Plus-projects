% de_motion.m
clear, clc
close all

%% 读入图片
I=imread('bicycle.bmp');
if ndims(I)>=3
    I=rgb2gray(I);
end
figure(1);
imshow(I,[])
title('运动模糊图像');

%% 去除运动模糊

% 水平方向上运动20像素
PSF=fspecial('motion', 20,0);

figure(2);
% 估计噪声方差
noise_var = 0.0001;
estimated_nsr = noise_var / var(double(I(:)));

% 维纳滤波
I2 = deconvwnr(I,PSF,0.00005);
imshow(I2,[]);
title('维纳滤波复原');
