// GetFeature.h: interface for the GetFeature class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETFEATURE_H__82AF2137_3B0D_45AA_BD6E_7FDF0669B22E__INCLUDED_)
#define AFX_GETFEATURE_H__82AF2137_3B0D_45AA_BD6E_7FDF0669B22E__INCLUDED_

#include "ImageDib.h"
#include "NumRecognition.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GetFeature : public ImageDib 
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;

	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;

	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;

	//数字位置
	int m_charleft,m_chartop,m_charright,m_charbottom;

	//输出图像的宽，像素为单位
	int m_imgWidthOut;

	//输出图像的高，像素为单位
	int m_imgHeightOut;
private:
	//输出图像颜色表长度
	int m_nColorTableLengthOut;

public:
	GetFeature();
	GetFeature(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	virtual ~GetFeature();
	CSize GetDimensions();
	GetPosition();
	SetFeature();
	ShowFeature();
	ShowPattern(double feature[]);
	double Cal(int row, int col);

};

#endif // !defined(AFX_GETFEATURE_H__82AF2137_3B0D_45AA_BD6E_7FDF0669B22E__INCLUDED_)
