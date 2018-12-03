// MeanShiftSegger.h: interface for the MeanShiftSegger class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEANSHIFTSEGGER_H__0CB542C8_6C53_49C1_BFA4_7124A4712E47__INCLUDED_)
#define AFX_MEANSHIFTSEGGER_H__0CB542C8_6C53_49C1_BFA4_7124A4712E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HISTOGRAM_LENGTH 4096   //直方图长度
typedef unsigned char pixel;

class MeanShiftSegger  
{
public:
	MeanShiftSegger();
	~MeanShiftSegger();
private:
	int imgWidth;   //图像宽度
	int imgHeight;  //图像高度

	int trackWinWidth;  //窗口宽度
	int trackWinHeight;  //窗口高度		
	int currentX;       //当前目标横坐标
	int currentY;       //当前目标纵坐标

private:
	float currHistogram[HISTOGRAM_LENGTH];  //当前直方图
	float tempHistogram[HISTOGRAM_LENGTH];  //目标直方图摸板
public:

    //计算Bhattacharyya系数
	float CalculateBhattacharyya(float initHistogram[HISTOGRAM_LENGTH],float tempHistogram[HISTOGRAM_LENGTH]);

    //设置目标位置
	void InitMeanShiftTracker(unsigned char* firstFrame,int frameWidth,int frameHeight,int targetPosX,int targetPosY,int targetWidth,int targetHeight);
    //计算目标直方图	
	int CalculateHistogramSp(unsigned char*  frame,char kernel,float histogram[HISTOGRAM_LENGTH]);

	//Mean Shift 迭代过程
	void MeanShiftProcessSp(unsigned char * frame);
    //Mean Shift跟踪操作函数
	void MeanShiftTrackProcess(unsigned char*  frame,int frameNumber);
	//根据当前跟踪窗口的大小和目标位置绘制跟踪窗
	void DrawTrackBox(unsigned char*  frame);
};

#endif // !defined(AFX_MEANSHIFTSEGGER_H__0CB542C8_6C53_49C1_BFA4_7124A4712E47__INCLUDED_)
