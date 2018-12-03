// AVIHandle.h: interface for the AVIHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIHANDLE_H__A05DC085_F715_46FD_9568_DA9107583ED5__INCLUDED_)
#define AFX_AVIHANDLE_H__A05DC085_F715_46FD_9568_DA9107583ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vfw.h>

class AVIHandle  
{
public:
	AVIHandle(void);
	AVIHandle(CString filename);
public:
	~AVIHandle(void);
public:
	BOOL LoadVideo(CString filename);
private:
	int width; //图像宽度
	int height;//图象高度
	int length;//长度
	int defaultPad;
	BOOL loaded;
	CString AVIfilename; //AVI文件名
	LPBITMAPINFOHEADER lpbm;
	//未填充灰度缓冲（24Bit可彩色处理）
	unsigned char* gray_value;
	//边填充灰度缓冲（24Bit可彩色处理）
	unsigned char* gray_value_square;
	PGETFRAME pgf;
public:
	unsigned char* GetSingleFrame(int i);
	int GetFrameCount(void);
	int GetFrameWidth(void);
	int GetFrameHeight(void);
	LPBITMAPINFOHEADER GetLPBM(void);
	BOOL isLoaded(void);
};

#endif // !defined(AFX_AVIHANDLE_H__A05DC085_F715_46FD_9568_DA9107583ED5__INCLUDED_)
