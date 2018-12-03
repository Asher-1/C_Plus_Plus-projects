function Y = show_noise_pdf(type, x, a, b)
% show_noise_pdf 显示不同噪声的概率密度函数.
% type:  字符串，取值随噪声种类而定
% 高斯噪声:     gaussian，参数为(x,y)，默认值为(0,10)
% 瑞利噪声:     rayleigh，参数为x，默认值为30
% 伽马噪声:     gamma，参数为(x,y),默认值为(2,10)
% 指数噪声：    exp，参数为x,默认值为15
% 均匀分布：    uniform，参数为(x,y)，默认值为(-20,20)
% 椒盐噪声：    salt & pepper: 强度为x，默认值为0.02
% example:
% x=0:.1:10;
% Y=show_noise_pdf('gamma',2,5,x);
% plot(x,Y)

% 设置默认噪声类型
if  nargin == 1
    type='gaussian';
end

% 开始处理
switch lower(type)
     %高斯噪声的情况
    case 'gaussian'
        if nargin<4
            b=10;
        end
        if nargin <3
            a=0;
        end
        Y=normpdf(x,a,b);
        
        %均匀噪声的情况
    case 'uniform'
        if nargin<4
            b=3;
        end
        if nargin <3
            a=-3;
        end
        Y=unifpdf(x,a,b);
        
        %椒盐噪声的情况
    case 'salt & pepper'
 
        % 调用imnoise函数
        Y=zeros(size(x));
        Y(1)=0.5;
        Y(end)=0.5;
        
        %瑞利噪声的情况
    case 'rayleigh'
        if nargin < 3
            a = 30;
        end
        Y=raylpdf(x,a);
        
        %指数噪声的情况
    case 'exp'
        if nargin < 3
            a = 15;
        end
        Y=exppdf(x,a);
        
        %伽马噪声的情况
    case 'gamma'
        if nargin <4
            b=10;
        end
        if nargin<3
            a=2;
        end
        Y=gampdf(x,a,b);
    
    otherwise
        error('Unknown distribution type.')
end
