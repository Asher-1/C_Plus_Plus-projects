function I_new = wn_filter(I, H, threshold, K)
% 维纳滤波复原函数
% I_new = rev_filter(I, H, threshold)
% I：原始图像
% H：传递函数
% K:噪声-图像功率比
% threshold:逆滤波的半径
% I_new：复原图像

% 若为彩色图像，转为为灰度图像
if ndims(I)>=3
    I=rgb2gray(I);
end

Id=im2double(I);

% 傅里叶变换
f_Id=fft2(Id);
f_Id=fftshift(f_Id);
fH_Id=f_Id;
D=abs(H);
D=D.^2;

[M,N]=size(fH_Id);
% 逆滤波
if threshold>M/2
    % 全滤波
    fH_Id=fH_Id./(H+eps);
else
    %对一定半径范围内进行滤波
    for i=1:M
        for j=1:N
            if sqrt((i-M/2).^2+(j-N/2).^2)<threshold
                % 维纳滤波公式
                fH_Id(i,j)=fH_Id(i,j)./(H(i,j)) .* (D(i,j)./(D(i,j)+K));
            end
        end
    end
end


% 执行傅立叶逆变换
fH_Id1=ifftshift(fH_Id);
I_new=ifft2(fH_Id1);
I_new=uint8(abs(I_new)*255);
