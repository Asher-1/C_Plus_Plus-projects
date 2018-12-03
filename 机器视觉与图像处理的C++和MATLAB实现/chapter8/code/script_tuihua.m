%script_tuihua
% 读进原始图像
I = imread('lena.bmp');
figure(1);
subplot(121)
imshow(I)
title('原始图像');
f=im2double(I);

% 傅立叶变换
F = fft2(f);
F=fftshift(F);

% 执行退化
[M,N]=size(F);
[u,v]=meshgrid(1:M,1:N);
H=exp(-0.0025* ( (u-M/2).^2 + (v-N/2).^2 ) .^(5/6) );
F=F.*H;

% 傅立叶反变换
X=ifftshift(F);
x=ifft2(X);
subplot(122)
x=uint8(abs(x)*256);
imshow(x)
imwrite(x,'lena_t.bmp');
title('退化图像')
