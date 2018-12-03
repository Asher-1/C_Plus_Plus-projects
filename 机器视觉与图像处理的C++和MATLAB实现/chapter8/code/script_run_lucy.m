% script_run_lucy
% L-R算法图像复原

%%
clear,clc
close all

%%
% 棋盘格图像
I=checkerboard(8);

%点扩散函数
PSF=fspecial('gaussian',7,10);

% 方差为0.0001
SD=0.01;
In=imnoise(imfilter(I,PSF),'gaussian',0,SD^2);

%%
% 使用Lucy-Richardson算法对图像复原
Dampar=10*SD;
LIM=ceil(size(PSF,1)/2);
Weight=zeros(size(In));

% 权值weight数组的大小是64*64
% 并且有值为0的4像素宽的边界，其余像素都是1
Weight(LIM+1:end-LIM,LIM+1:end-LIM)=1;

% 迭代次数为5
NumIt=5;
% 利用deconvlucy来实现复原
J1=deconvlucy(In,PSF,NumIt,Dampar,Weight);

% 迭代次数为10
NumIt=10;
J2=deconvlucy(In,PSF,NumIt,Dampar,Weight);

% 迭代次数为20
NumIt=20;
J3=deconvlucy(In,PSF,NumIt,Dampar,Weight);

% 迭代次数为100
NumIt=100;
J4=deconvlucy(In,PSF,NumIt,Dampar,Weight);

%% 绘图
subplot(231);
imshow(I);
title('原图')

subplot(232);
imshow(In);
title('退化图像')

subplot(233);
imshow(J1);
title('迭代5次')

subplot(234)
imshow(J2);
title('迭代10次')

subplot(235);
imshow(J3);
title('迭代20次')

subplot(236);
imshow(J4);
title('迭代100次')
