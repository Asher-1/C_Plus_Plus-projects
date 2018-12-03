% script_adp_median
I=imread('lena.bmp');
I0=imnoise(I,'salt & pepper',0.01);
I1=medfilt2(I0,[3,3]);
I2=adp_median(I0,7);
subplot(221);
imshow(I);
title('原图')
subplot(222);
imshow(I0);
title('椒盐噪声')
subplot(223);
imshow(I1);
title('中值滤波')
subplot(224);
imshow(I2);
title('自适应中值滤波')
