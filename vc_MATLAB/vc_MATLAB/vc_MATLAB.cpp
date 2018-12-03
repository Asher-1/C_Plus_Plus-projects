// vc_MATLAB.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include "engine.h"
#include <string.h> 
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

void Opencv_Matlab_C()
{

	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		printf("Cannot open MATLAB Engine.");
	}

	const char *pstrimagename = "17.jpg";
	const char *pstrwindowstitle1 = "目标图方法1";
	const char *pstrwindowstitle2 = "目标图方法2";
	const char *sdtwindowstitle = "原图";
	//从文件中读取图像  
	IplImage *img;
	IplImage *gray;
	if (!(img = cvLoadImage(pstrimagename, 1)))
		printf("can't load image");
	gray = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);

	int width = cvGetSize(img).width;
	int height = cvGetSize(img).height;

	// 定义903*600大小的mxREAL实双精度矩阵
	mxArray *gray_MATLAB = mxCreateDoubleMatrix(width, height, mxREAL);
	// 给gray_MATLAB赋值
	memcpy(mxGetPr(gray_MATLAB), gray, width * height * sizeof(double));
	engPutVariable(ep, "gray_MATLAB", gray_MATLAB);
	engEvalString(ep, "figure");
	engEvalString(ep, "imshow(gray_MATLAB)");
	printf("press any key to exit!");
	getchar();
	mxDestroyArray(gray_MATLAB);
	engClose(ep);
	cvReleaseImage(&img);
	cvReleaseImage(&gray);
	cvDestroyWindow("gray_MATLAB");

}


void curveChart()
{
	const int N = 20;
	double x[N], y[N];//定义变量
	double z[256][256];
	//第7行—第9行：创建初始化变量，这里需要根据实际情况给出变量的维数和大小
	for (int i = 0; i < N; i++)
	{
		x[i] = i + 1;
		y[i] = x[i] * x[i];
	}
	//第12行—第16行：得到x平方的20个点坐标，用于之后绘图
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		printf("Cannot open MATLAB Engine.");
	}
	//第19行—第23行：定义MATLAB外部调用引擎的指针，并判定引擎指针创建是否成功
	mxArray *xx = mxCreateDoubleMatrix(1, N, mxREAL);//1行N列
	mxArray *yy = mxCreateDoubleMatrix(1, N, mxREAL);//1行N列
	mxArray *zz = mxCreateDoubleMatrix(256, 256, mxREAL);//256行256列
														 //第26—第28行：创建需要传递给MATLAB的实数变量指针，并根据上面创建的初始化变量指定该指针的维度和大小
	memcpy(mxGetPr(xx), x, N * sizeof(double));
	memcpy(mxGetPr(yy), y, N * sizeof(double));
	memcpy(mxGetPr(zz), z, 256 * 256 * sizeof(double));
	//第31行—第33行：将MATLAB的变量指针指向初始化变量地址
	engPutVariable(ep, "xx", xx);
	engPutVariable(ep, "yy", yy);
	engPutVariable(ep, "zz", zz);
	//第36行—第38行：将矩阵写入MATLAB空间，并为其在MATLAB空间中命名
	//调用MATLAB中plot函数，绘制曲线
	engEvalString(ep, "plot(xx,yy)");
	//调用MATLAB中的imread函数，读取一张图片
	engEvalString(ep, "zz=imread('cameraman.tif')");
	//调用MATLAB中的figure函数及imshow函数显示这张图片
	engEvalString(ep, "figure");
	engEvalString(ep, "imshow(zz)");
	printf("press any key to exit!");
	getchar();
	mxDestroyArray(xx);
	mxDestroyArray(yy);
	mxDestroyArray(zz);
	engClose(ep);
	// 第52行—第55行：释放内存空间，并关闭MATLAB引擎
}

void log_transformation()
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		printf("Cannot open MATLAB Engine.");
	}
	// mxCreateDoubleMatrix函数生成矩阵，本则例子需要创建3个256x256的矩阵
	mxArray *I = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *J = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *H = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
														//下面通过MATLAB引擎调用MATLAB算法
	engPutVariable(ep, "I", I);
	engPutVariable(ep, "J", J);
	engPutVariable(ep, "H", H);
	//第18行—第20行：将三个矩阵写入MATLAB空间，并为其在MATLAB空间中命名
	engEvalString(ep, "I=imread('cameraman.tif')");
	engEvalString(ep, "J=double(I)");
	//进行对数变换
	engEvalString(ep, "J=40*(log(J+1))");
	engEvalString(ep, "H=uint8(J)");
	engEvalString(ep, "subplot(1,2,1), imshow(I)");
	engEvalString(ep, "subplot(1,2,2), imshow(H)");
	printf("press any key to exit!");
	getchar();
	mxDestroyArray(I);
	mxDestroyArray(J);
	mxDestroyArray(H);
	engClose(ep);
}

void smooth_sharpness()
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		printf("Cannot open MATLAB Engine.");
	}
	// mxCreateDoubleMatrix函数生成矩阵	
	mxArray *I = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *J = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *A = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *B = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *C = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	engPutVariable(ep, "I", I);
	engPutVariable(ep, "J", J);
	engPutVariable(ep, "A", A);
	engPutVariable(ep, "B", B);
	engPutVariable(ep, "C", C);
	//下面通过MATLAB引擎调用MATLAB算法
	engEvalString(ep, "I=imread('cameraman.tif')");
	engEvalString(ep, "J=im2double(I)");
	engEvalString(ep, "subplot ( 2,2,1 ), imshow ( J, [ ] )");
	//采用的高通滤波方阵模板为h1、h2、h3
	engEvalString(ep, "h1=[ 0  -1   0,  -1   5  -1,  0  -1   0]");
	engEvalString(ep, "h2=[-1  -1  -1,  -1   9  -1, -1  -1  -1]");
	engEvalString(ep, "h3=[ 1  -2   1,  -2   5  -2,  1  -2   1]");
	engEvalString(ep, "A = conv2 (J,h1,'same')");
	engEvalString(ep, "subplot (2,2,2), imshow (A, [ ])");
	engEvalString(ep, "B= conv2 ( J, h2, 'same' )");
	engEvalString(ep, "subplot(2,2,3 ), imshow (B, [ ])");
	engEvalString(ep, "C = conv2 (J, h3, 'same' )");
	engEvalString(ep, "subplot (2,2,4), imshow (C, [ ])");
	printf("press any key to exit!");
	getchar();
	mxDestroyArray(I);
	mxDestroyArray(J);
	mxDestroyArray(A);
	mxDestroyArray(B);
	mxDestroyArray(C);
	engClose(ep);
}



//第1行—第3行：包含了C语言运行的必要文件和外部调用MATLAB函数的引擎头文件engine.h 
int main()
{
	smooth_sharpness();
	//Opencv_Matlab_C();
	return 0;
}



