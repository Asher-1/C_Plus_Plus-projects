// MeanShiftSegger.cpp: implementation of the MeanShiftSegger class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VideoDemo.h"
#include "MeanShiftSegger.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*int max(int x,int y)
{
	if(x > y)
		return x;

	return y;

}
int min(int x,int y)
{
	if(x > y)
		return y;

	return x;

}*/


/***********************************************************************
* 函数名称：
*     MeanShiftSegger()
*
*说明：构造函数 
***********************************************************************/
MeanShiftSegger::MeanShiftSegger():
imgWidth(0),imgHeight(0){
    for(int i = 0; i < HISTOGRAM_LENGTH; i ++)
		currHistogram[i] = 0;
    for(i = 0; i < HISTOGRAM_LENGTH; i ++)
		tempHistogram[i] = 0;
	//memset(currHistogram,0,sizeof(float) * HISTOGRAM_LENGTH);
	//memset(tempHistogram,0,sizeof(float) * HISTOGRAM_LENGTH);
	//SetZeroHistogram(currHistogram);
	//SetZeroHistogram(tempHistogram);
	return;

}
/***********************************************************************
* 函数名称：
*     ~MeanShiftSegger()
* 说明：
*     析构函数
***********************************************************************/
MeanShiftSegger::~MeanShiftSegger(){

}

/***********************************************************************
* 函数名称：
*     InitMeanShiftTracker()
* 参数：
*     unsigned char * firstFrame 第一帧缓冲
*     int frameWidth-帧宽度
*     int frameHeight-帧高度
*     int targetPosX-目标位置横坐标
*     int targetPosY-目标位置纵坐标
*     int targetWidth-目标宽度
*     int targetHeight-目标高度
* 说明：MeanShift过程跟踪初始化程序
***********************************************************************/

void MeanShiftSegger::InitMeanShiftTracker(unsigned char*  firstFrame,int frameWidth,int frameHeight,int targetPosX,int targetPosY,int targetWidth,int targetHeight){
	this->imgWidth = frameWidth;
	this->imgHeight = frameHeight;

	this->currentX = targetPosX;
	this->currentY = targetPosY;

	this->trackWinHeight = targetHeight;
	this->trackWinWidth = targetWidth;
}


/***********************************************************************
* 函数名称：
*     CalculateHistogramSp
* 参数：
*     image frame-当前输入帧
*     char kernel-核类型选择（目前只支持Uniform核）
*     float * histogram-直方图缓冲
* 说明：统计直方图，建立区域模型（无参数概率密度估计）
***********************************************************************/
int MeanShiftSegger::CalculateHistogramSp(unsigned char*  frame,char kernel, float *histogram){
	int pxValue = 0;

	for(int i = 0 ; i < HISTOGRAM_LENGTH; i++)
		histogram[i] = 0;

	if(kernel == 'U')
	{

		for(int j = max(0,currentY - trackWinHeight / 2);j < min(currentY + trackWinHeight / 2,imgHeight - 1); j ++){
			for(int i = max(0,currentX - trackWinWidth / 2); i < min(currentX + trackWinWidth / 2,imgWidth - 1);i ++){
				pixel r = frame[j * imgWidth * 3 + i * 3] / 16;
				pixel g = frame[j * imgWidth * 3 + i * 3 + 1] / 16;
				pixel b = frame[j * imgWidth * 3 + i * 3 + 2] / 16;

				histogram[256 * (int)r + 16 * (int)g + (int)b] += 1;
				pxValue ++;

			}

		}

		for(int i = 0; i < HISTOGRAM_LENGTH; i ++)
			histogram[i] /= pxValue;
	}
	else
	{
		return 0;
	}
	return pxValue;
}

/***********************************************************************
* 函数名称：
*     MeanShiftTrackerProcess
* 参数：
*     unsigned char *frame-当前帧缓冲
* 说明：跟踪中的MeanShift过程
***********************************************************************/
void MeanShiftSegger::MeanShiftProcessSp(unsigned char * frame){
	float weights[HISTOGRAM_LENGTH];

	float newX = 0.0;
	float newY = 0.0;

	for (int i=0;i<HISTOGRAM_LENGTH;i++)
	{
		if (currHistogram[i] >0.0 )
			weights[i] = (float)tempHistogram[i]/(float)currHistogram[i];
		//目标模板直方图/当前直方图
		else
			weights[i] = 0;
	}

	float sumOfWeights = 0.0;

		for(int j = max(0,currentY - trackWinHeight / 2);j < min(currentY + trackWinHeight / 2,imgHeight - 1); j ++){
			for(int i = max(0,currentX - trackWinWidth / 2); i < min(currentX + trackWinWidth / 2,imgWidth - 1);i ++){
			pixel r = frame[j * imgWidth * 3 + i * 3] / 16;
			pixel g = frame[j * imgWidth * 3 + i * 3 + 1] / 16;
			pixel b = frame[j * imgWidth * 3 + i * 3 + 2] / 16;

			int ptr = (int)(256 * (int)r + 16 * (int)g + (int)b);

			newX += (weights[ptr] * (float)i);
			newY += (weights[ptr] * (float)j);

			sumOfWeights += weights[ptr];


		}

	}
	if(sumOfWeights != 0){
		currentX = int((newX/sumOfWeights) + 0.5);
		currentY = int((newY/sumOfWeights) + 0.5);
	}
	return;

}


/***********************************************************************
* 函数名称
*     MeanShiftTrackProcess 
* 参数：
*     image frame-当前输入帧
*     int frameNumber-输入帧号
* 说明：MeanShift跟踪主调用过程
***********************************************************************/
void MeanShiftSegger::MeanShiftTrackProcess(unsigned char*  frame,int frameNumber)
{
	if(frameNumber == 0){
		//在指定位置建立目标模型
		CalculateHistogramSp(frame,'U',this->tempHistogram);
	}
	else
	{
		int stopThreshold = 10;
		int iteratorCoumt = 0;
		while(iteratorCoumt < stopThreshold){

			//在后续帧中检测目标新位置
			CalculateHistogramSp(frame,'U',this->currHistogram);
			MeanShiftProcessSp(frame); //获得新位置

			//更新直方图
			iteratorCoumt++;
		}
		
		DrawTrackBox(frame);

	}
}


/***********************************************************************
* 函数名称：
*     DrawTrackBox
* 参数：
*     unsigned char * frame-当前帧缓冲
* 说明：在输出中画跟踪窗口
***********************************************************************/
void MeanShiftSegger::DrawTrackBox(unsigned char*  frame)
{

	for(int i = currentX; i < min(imgWidth,currentX + trackWinWidth); i ++)
	{
		frame[currentY * imgWidth * 3 + i * 3 + 0] = 255;
		frame[currentY * imgWidth * 3 + i * 3 + 1] = 255;
		frame[currentY * imgWidth * 3 + i * 3 + 2] = 255;

		frame[min(imgHeight - 1,currentY + trackWinHeight) * imgWidth * 3 + i * 3 + 0] = 255;
		frame[min(imgHeight - 1,currentY + trackWinHeight) * imgWidth * 3 + i * 3 + 1] = 255;
		frame[min(imgHeight - 1,currentY + trackWinHeight) * imgWidth * 3 + i * 3 + 2] = 255;
	}

	for(int j = currentY; j < min(imgHeight - 1,currentY + trackWinHeight); j ++)
	{

		frame[j * imgWidth * 3 + currentX * 3 + 0] = 255;
		frame[j * imgWidth * 3 + currentX * 3 + 1] = 255;
		frame[j * imgWidth * 3 + currentX * 3 + 2] = 255;

		frame[j * imgWidth * 3 + min(imgWidth - 1,currentX + trackWinWidth) * 3 + 0] = 255;
		frame[j * imgWidth * 3 + min(imgWidth - 1,currentX + trackWinWidth) * 3 + 1] = 255;
		frame[j * imgWidth * 3 + min(imgWidth - 1,currentX + trackWinWidth) * 3 + 2] = 255;
	}


}

/***********************************************************************
* 函数名称：
*     CalculateBhattacharyya
* 说明：计算Bhattachryya
***********************************************************************/
float MeanShiftSegger::CalculateBhattacharyya(float initHistogram [HISTOGRAM_LENGTH],float tempHistogram[HISTOGRAM_LENGTH]){
	float dis = 0;

	for(int i = 0;i < HISTOGRAM_LENGTH ;i ++)
		dis += float(sqrt(double(currHistogram[i] * tempHistogram[i])));

	return dis;
}
