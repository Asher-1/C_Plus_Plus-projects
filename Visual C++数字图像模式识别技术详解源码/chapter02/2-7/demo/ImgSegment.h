// ImgSegment.h: interface for the ImgSegment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_)
#define AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class ImgSegment : public ImageDib  
{
public:
	int m_nBitCountOut;	//输出图像每像素位数
	unsigned char * m_pImgDataOut;	//输出图像位图数据指针
	LPRGBQUAD m_lpColorTableOut;	//输出图像颜色表
private:
	int m_imgWidthOut;	//输出图像的宽
	int m_imgHeightOut;	//输出图像的高
	int m_nColorTableLengthOut;	//输出图像颜色表长度
public:
	ImgSegment();	//不带参数的构造函数
	ImgSegment(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);	//带参数的构造函数
	virtual ~ImgSegment();	//析构函数
	CSize GetDimensions();	//以像素为单位返回输出图像的尺寸
public:
	int threshOtus(int histArray[256]);	//大津阈值分割
	void AdaptThreshSeg(unsigned char *pImgData);
	void Roberts();	//Roberts算子
	void Sobel();	//Sobel算子
	void Prewitt();	//Prewitt算子
	void Laplacian();	//Laplacian算子
public:
	void RegionGrow(CPoint SeedPos, int thresh);	//区域生长
	void EdgeTrace();	//曲线跟踪
private:
	//模板卷积函数
	void TemplateEdge(unsigned char *imgIn, int width, int height, int nBitCount,
		int *mask, int maskW, int maskH, unsigned char *imgOut);
	
	//极坐标形式下的线检测hough变换
	void HoughTransform(unsigned char *imgBinaryIn, int width, int height, int *houghBuf,
		int  houghWidth, int houghHeight,float radiusResolution, float angleResolution,
					 float *radius, float *angle);

};

#endif // !defined(AFX_IMGSEGMENT_H__8745C184_7B7A_4116_9351_36AEA5427282__INCLUDED_)
