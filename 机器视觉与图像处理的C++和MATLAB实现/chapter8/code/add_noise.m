function IJ = add_noise(I, type, x, y)
% add_noise 函数用以产生前面所述几种噪声的随机序列.
% input:
% I：输入图像矩阵，为灰度图像
% type:  字符串，取值随噪声种类而定
% 高斯噪声:     gaussian，参数为(x,y)，默认值为(0,10)
% 瑞利噪声:     rayleigh，参数为x，默认值为30
% 伽马噪声:     gamma，参数为(x,y),默认值为(2,10)
% 指数噪声：    exp，参数为x,默认值为15
% 均匀分布：    uniform，参数为(x,y)，默认值为(-20,20)
% 椒盐噪声：    salt & pepper: 强度为x，默认值为0.02
% output:
% IJ:添加噪声后的图像
% example:
% I=imread('a.bmp');
% IJ=add_noise(I,'salt & pepper',0.1);
% imshow(IJ)

% 预处理
if ndims(I)>=3
    I=rgb2gray(I);
end

[M,N]=size(I);

% 设置默认噪声类型
if  nargin == 1
    type='gaussian';
end

% 开始处理
switch lower(type)
    
    %高斯噪声的情况
    case 'gaussian'
        if nargin<4
            y=10;
        end
        if nargin <3
            x=0;
        end
        
        % 产生高斯分布随机数
        R = normrnd(x,y,M,N);
        IJ=double(I)+R;
        IJ=uint8(round(IJ));
        
        %均匀噪声的情况
    case 'uniform'
        if nargin<4
            y=3;
        end
        if nargin <3
            x=-3;
        end
        
        % 产生均匀分布随机数
        R = unifrnd(x,y,M,N);
        IJ=double(I)+R;
        IJ=uint8(round(IJ));
        
        %椒盐噪声的情况
    case 'salt & pepper'
        if nargin < 3
            x= 0.02;
        end
        
        % 调用imnoise函数
        IJ=imnoise(I,'salt & pepper', x);      
        
        %瑞利噪声的情况
    case 'rayleigh'
        if nargin < 3
            x = 30;
        end
        
        % 产生瑞利分布随机数
        R = raylrnd(x,M,N);
        IJ=double(I)+R;
        IJ=uint8(round(IJ));
        
        %指数噪声的情况
    case 'exp'
        if nargin < 3
            x = 15;
        end
        R=exprnd(x,M,N);
        IJ=double(I)+R;
        IJ=uint8(round(IJ));
        
        %伽马噪声的情况
    case 'gamma'
        if nargin <4
            y=10;
        end
        if nargin<3
            x=2;
        end
        
        R=gamrnd(x,y,M,N);
        IJ=double(I)+R;
        IJ=uint8(round(IJ));
    otherwise
        error('Unknown distribution type.')
end
