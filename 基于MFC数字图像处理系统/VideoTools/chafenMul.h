// chafenMul.h: interface for the chafenMul class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAFENMUL_H__3A0FE375_9E4F_481E_A5DD_D7080D40028E__INCLUDED_)
#define AFX_CHAFENMUL_H__3A0FE375_9E4F_481E_A5DD_D7080D40028E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ChafenMul{
public:

	ChafenMul(int width,int height,int targetThreshold);
	~ChafenMul();
public:

	unsigned char *frame1; //第1帧数据
	unsigned char *frame2; //第2帧数据
	unsigned char *frame3; //第3帧数据
	unsigned char *frame4; //第4帧数据

	int frameWidth; int frameHeight; //帧宽度和高度

	int readyCount;       //输入帧的数量

	int targetThreshold;  //目标检测阈值
public:
	void PrepareData(unsigned char *buffer,int width,int height);

	void process();
	

};
#endif // !defined(AFX_CHAFENMUL_H__3A0FE375_9E4F_481E_A5DD_D7080D40028E__INCLUDED_)
