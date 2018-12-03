// chafenMul.cpp: implementation of the chafenMul class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoDemo.h"
#include "chafenMul.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/************************************************************************
* 函数名称：
*     ChafenMul
*
* 参数：
*     int width-帧宽度
*     int height-帧高度
*
* 说明：
*     构造函数，并分配内存空间，参数为帧的高度和宽度
************************************************************************/
ChafenMul::ChafenMul(int width,int height,int _targetThreshold):
targetThreshold(_targetThreshold),frameWidth(width),frameHeight(height),
frame1(NULL),frame2(NULL),frame3(NULL),frame4(NULL)
{
	frame1 = new unsigned char[width * height];
	frame2 = new unsigned char[width * height];
	frame3 = new unsigned char[width * height];
	frame4 = new unsigned char[width * height];

	memset(frame1,0,sizeof(unsigned char) * width * height);
	memset(frame2,0,sizeof(unsigned char) * width * height);
	memset(frame3,0,sizeof(unsigned char) * width * height);
	memset(frame4,0,sizeof(unsigned char) * width * height);

	readyCount = 0;

}


/************************************************************************
* 函数名称：
*     ~ChafenMul
*
* 说明：
*     析构函数，释放内存空间
************************************************************************/
ChafenMul::~ChafenMul()
{
	if(frame1 != NULL) delete[] frame1;
	if(frame2 != NULL) delete[] frame2;
	if(frame3 != NULL) delete[] frame3;
	if(frame4 != NULL) delete[] frame4;
}

/************************************************************************
* 函数名称：
*     PrepareData
*
* 参数：
*     unsigned char *buffer-输入图像数据
*     int width-帧宽度
*     int height-帧高度
*
* 说明：
*  输入帧图像数据，并分别将帧图像数据拷贝至缓存中
************************************************************************/
void ChafenMul::PrepareData(unsigned char *buffer,int width,int height)
{	
	if(readyCount == 0)
		memcpy(frame1,buffer,width * height * sizeof(unsigned char));

	if(readyCount == 1)
		memcpy(frame2,buffer,width * height * sizeof(unsigned char));

	if(readyCount == 2)
		memcpy(frame3,buffer,width * height * sizeof(unsigned char));

	if(readyCount == 3)
		memcpy(frame4,buffer,width * height * sizeof(unsigned char))
		;

	readyCount ++;

	if(readyCount == 4)
		readyCount = 0;

}

/************************************************************************
* 函数名称：
*     process
*
* 说明：
*     差分相乘法检测目标
************************************************************************/
void ChafenMul::process()
{
	int * mul = new int[frameWidth * frameHeight];
	
	for(int j = 0; j < frameHeight; j ++)
	{
		for(int i = 0; i < frameWidth; i ++)
		{
			frame1[j*frameWidth+i]=abs(frame3[j*frameWidth+i] - frame1[j * frameWidth + i]);
			frame2[j*frameWidth+i]=abs(frame4[j*frameWidth+ i] - frame2[j * frameWidth + i]);
			mul[j*frameWidth +i] = frame1[j * frameWidth + i] * frame2[j * frameWidth + i];
			if(mul[j*frameWidth +i] > targetThreshold)
			{
				frame1[j * frameWidth + i] = 255;
			}else
				frame1[j * frameWidth + i] = 0;
		}
	}
	
	
	//对frame1运用静态背景检测的方法并计算目标形心的位置
	

	//释放空间
	delete[] mul;
}
