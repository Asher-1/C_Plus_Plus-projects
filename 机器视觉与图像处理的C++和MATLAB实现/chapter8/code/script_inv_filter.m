% script_inv_filter
%%
% 逆滤波
I0=imread('lena_t.bmp');

% 阈值为128
I_new1 = rev_filter(I0, H, 128);

% 阈值为108
I_new2 = rev_filter(I0, H, 108);

% 阈值为78
I_new3 = rev_filter(I0, H, 78);

% 阈值为48
I_new4 = rev_filter(I0, H, 48);
si=zeros(M,N,1,4,'uint8');
si(:,:,1)=I_new1;
si(:,:,2)=I_new2;
si(:,:,3)=I_new3;
si(:,:,4)=I_new4;

% 绘图
figure
montage(si)
title('阈值分别为128,108,78,48')
