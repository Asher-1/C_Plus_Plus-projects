% script_wiener2
%%
clear,clc

% 读入退化图像
I1=imread('lena_wn.bmp');

[m,n]=size(I1); 
k=0.0025; 
u=1:m;
v=1:n;
[u,v]=meshgrid(u,v);

% 退化函数
H=exp((-k).*(((u-m/2).^2+(v-n/2).^2).^(5/6)));

% 退化图像对应的点扩散函数
PSF=ifftshift(ifft2(H));

% 维纳滤波
I2 = deconvwnr(I1,abs(PSF),0.08);
imshow(I2);
title('使用deconvwnr滤波')
