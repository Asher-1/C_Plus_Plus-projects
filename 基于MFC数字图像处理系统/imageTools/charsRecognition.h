// charsRecognition.h: interface for the CharsRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARSRECOGNITION_H__851ED4A3_186E_447B_B380_B10FCC9AC344__INCLUDED_)
#define AFX_CHARSRECOGNITION_H__851ED4A3_186E_447B_B380_B10FCC9AC344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
#include "Structure.h"

class charsRecognition  : public ImageDib 
{
public:
	//输出图像每像素位数
	int m_nBitCountOut;
	//输出图像位图数据指针
	unsigned char * m_pImgDataOut;
	unsigned char * m_pImgDataIn;
	//输出图像颜色表
	LPRGBQUAD m_lpColorTableOut;
	list<h> m_ProjX;//向垂直方向上的投影X结果
	list<CFtChinese> m_fChinese;//汉字
	string m_strResult;//字符结果
    match L[20];         //汉字近邻候选值
	match Lm[10];
	match Lp[10];
private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
public:
	//不带参数的构造函数
	charsRecognition();
	//带参数的构造函数
	charsRecognition(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	//析构函数
	~charsRecognition();
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();
	void HprojX();
	int LineCluster();
	void MergeProjX(int lWm);
	void CutImage(int lWm);
	LONG RoundCut(LONG iy,h ix,int iW);
	void GetMinRect(Position &pos);
	void BlockNormalization();
	void Norm(CFtChinese &itx);
	int MyRound(double x);
	void Thinning(int SWH,CFtChinese &it);
	void Normalization(Position &pos,int SWH,int *iImage);
	void GetCrudeFeature();
	void Density();
	void CrossMesh(CFtChinese &it);
	void CrossPeriphery(CFtChinese &it);
	void CrudeMatchChinese();
	BOOL CrudeMatch(CFtChinese &it);
	void DrawRect();
};

#endif // !defined(AFX_CHARSRECOGNITION_H__851ED4A3_186E_447B_B380_B10FCC9AC344__INCLUDED_)

inline int charsRecognition::MyRound(double x)
{
	if ((x-(int)x)>=0.5)
	{
		return (int)(x+0.5);
	}
	else
	{
		return (int)x;
	}
}
