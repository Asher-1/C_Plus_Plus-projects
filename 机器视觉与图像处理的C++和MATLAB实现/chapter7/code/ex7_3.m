f=imread('D:\1.jpg');%读入一幅图
imshow(f);%显示读入的灰度图

[c,s]=wavefast(f,1,'sym4');%利用‘sym4’小波作快速小波变换
figure;wave2gray(c,s,-6);%利用小波工具箱的函数显示变换后灰度图

[nc,y]=wavecut('a',c,s);%利用小波工具箱的函数将近似系数置0
figure; wave2gray(nc,s,-6);
edges=abs(waveback(nc,s,'sym4'));%边缘图像的重构
figure; imshow(mat2gray(edges));
