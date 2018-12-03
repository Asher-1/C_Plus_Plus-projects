%信号函数定义
	%  1.正弦波定义
	f1=50;  		%  频率1
	f2=100; 		%  频率2
	fs=2*(f1+f2);  	%  采样频率
	Ts=1/fs;  		%  采样间隔
	N=120;    		%  采样点数
	n=1:N;
	y=sin(2*pi*f1*n*Ts)+sin(2*pi*f2*n*Ts);  %  信号函数

% 2小波滤波器谱分析
	h=wfilters('db30','l');  %  低通
	g=wfilters('db30','h');  %  高通

	h=[h,zeros(1,N-length(h))];  %  补零（圆周卷积，且增大分辨率便于观察）
	g=[g,zeros(1,N-length(g))];  	%  补零（圆周卷积，且增大分辨率便于观察）

% 3 MALLAT分解算法(圆周卷积的快速傅里叶变换实现)
	sig1=ifft(fft(y).*fft(h));  	%  低通(低频分量)
	sig2=ifft(fft(y).*fft(g));  	%  高通(高频分量)

%4 MALLAT重构算法
	sig1=dyaddown(sig1); 		%  2抽取
	sig2=dyaddown(sig2); 		%  2抽取

	sig1=dyadup(sig1);   		%  2插值
	sig2=dyadup(sig2);   		%  2插值

	sig1=sig1(1,[1:N]);  		%  去掉最后一个零
	sig2=sig2(1,[1:N]);  		%  去掉最后一个零

	hr=h(end:-1:1);         		%  重构低通
	gr=g(end:-1:1);         		%  重构高通

	hr=circshift(hr',1)';   		%  位置调整圆周右移一位
	gr=circshift(gr',1)';   		%  位置调整圆周右移一位

	sig1=ifft(fft(hr).*fft(sig1));  	%  低频
	sig2=ifft(fft(gr).*fft(sig2));  	%  高频

	sig=sig1+sig2; 		%  源信号
