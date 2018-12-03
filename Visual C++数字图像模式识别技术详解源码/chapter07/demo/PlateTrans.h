// PlateTrans.h: interface for the PlateTrans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLATETRANS_H__22078346_C360_4BAB_966C_FAC26E9959BF__INCLUDED_)
#define AFX_PLATETRANS_H__22078346_C360_4BAB_966C_FAC26E9959BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
class PlateTrans : public ImageDib  
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;

	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	
	unsigned char * m_pImgDataIn1;
	unsigned char * m_pImgDataIn2;

	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;

private:
	void initmodule();
	int BeWhite(char *Col);
	int BeBlue(char *Col);
	//输出图像的宽，像素为单位
	int m_imgWidthOut;

	//输出图像的高，像素为单位
	int m_imgHeightOut;

	//输出图像颜色表长度
	int m_nColorTableLengthOut;

	int m_modules[40][20][40];
	char m_chars[40];


public:
	void charrecognize();
	
	//不带参数的构造函数
	PlateTrans();

	//带参数的构造函数
	PlateTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData1,unsigned char *pImgData2);

	//析构函数
	~PlateTrans();
	

	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();

	void ColorPairSearch();
};

#endif // !defined(AFX_PLATETRANS_H__22078346_C360_4BAB_966C_FAC26E9959BF__INCLUDED_)
