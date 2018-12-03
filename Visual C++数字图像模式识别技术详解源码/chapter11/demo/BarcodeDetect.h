// BarcodeDetect.h: interface for the BarcodeDetect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODEDETECT_H__6D80A444_19EE_4D6B_9300_F9C1F0ED87AF__INCLUDED_)
#define AFX_BARCODEDETECT_H__6D80A444_19EE_4D6B_9300_F9C1F0ED87AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"

class BarcodeDetect : public ImageDib  
{

	public:
		//输出图像每像素位数
		int m_nBitCountOut;
		//输出图像位图数据指针
		unsigned char * m_pImgDataOut;
		//输出图像颜色表
		LPRGBQUAD m_lpColorTableOut;
		//输入图像位图数据指针
		unsigned char * m_pImgDataIn;

		BYTE** ImageArray;				//原始的图像数据数组


		int arPixelH[1000];				//为了水平方向直方图统计用
		int arPixelV[1000];				//为了垂直方向直方图统计用
		int arWidth[200];				//保存宽度序列：从最左边的黑条开始
		int arDifference[1000];			//差分
		bool arMark[1000];				//标记
		int ImageLeft;					//图像最左边的座标
		int ImageRight;					//图像最右边的座标
		int ImageTop;					//条形码顶部的座标
		int ImageBottom;				//条形码下部的座标

	private:
		//输出图像的宽，像素为单位
		int m_imgWidthOut;
		//输出图像的高，像素为单位
		int m_imgHeightOut;
		//输出图像颜色表长度
		int m_nColorTableLengthOut;
	public:
		//不带参数的构造函数
		BarcodeDetect();
		//带参数的构造函数
		BarcodeDetect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
			unsigned char *pImgData);
		~BarcodeDetect();

		BOOL PreProcess(void);
		BOOL Recognize();
		int JudgNum(int w1, int w2, int w3, int w4, double mx);
		CString strCodeNumber;
		CString strCodeStyle;

	protected:	
		int arDelta[1000];
		int arLeftEdge1[1000];
		int arLeftEdge2[1000];
		int arLeftEdge[1000];
};

#endif // !defined(AFX_BARCODEDETECT_H__6D80A444_19EE_4D6B_9300_F9C1F0ED87AF__INCLUDED_)
