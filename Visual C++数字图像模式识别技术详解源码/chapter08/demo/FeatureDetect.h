// FeatureDetect.h: interface for the FeatureDetect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEATUREDETECT_H__07B031B4_F2C2_4C25_BE7D_FE9272432868__INCLUDED_)
#define AFX_FEATUREDETECT_H__07B031B4_F2C2_4C25_BE7D_FE9272432868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageDib.h"
class FeatureDetect : public ImageDib
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

		//变量
	double FeatureLocalCalmRD;	//局部平稳性－45度方向
	double FeatureLocalCalmLD;	//局部平稳性－135度方向
	double FeatureLocalCalmV;	//局部平稳性－90度方向度方向
	double FeatureLocalCalmH;	//局部平稳性－0度方向
	double FeatureCorrelationRD;	//相关性－45度方向
	double FeatureCorrelationLD;	//相关性－135度方向
	double FeatureCorrelationV;		//相关性－90度方向
	double FeatureCorrelationH;		//相关性－0度方向
	double FeatureInertiaQuadratureRD;	//惯性矩－45度方向
	double FeatureInertiaQuadratureLD;	//惯性矩－135度方向
	double FeatureInertiaQuadratureV;	//惯性矩－90度方向
	double FeatureInertiaQuadratureH;	//惯性矩－0度方向
	double FeatureEntropyRD;	//熵－45度方向
	double FeatureEntropyLD;	//熵－135度方向
	double FeatureEntropyV;		//熵－90度方向
	double FeatureEntropyH;		//熵－0度方向
	double FeatureEnergyRD;		//能量－45度方向
	double FeatureEnergyLD;		//能量－135度方向
	double FeatureEnergyV;		//能量－90度方向
	double FeatureEnergyH;		//能量－0度方向
	int FilterWindowWidth;	//纹理区域块的大小，通常将图像划分成若干个纹理块计算
	
	int distance;	//距离，依据不同的应用选取	
	int GrayLayerNum;	//灰度分级
	int** PMatrixRD;	//45度方向上的灰度共现矩阵
	int** PMatrixLD;	//135度方向上的灰度共现矩阵
	int** PMatrixV;		//90度方向上的灰度共现矩阵
	int** PMatrixH;		//0度方向上的灰度共现矩阵
private:
	//输出图像的宽，像素为单位
	int m_imgWidthOut;
	//输出图像的高，像素为单位
	int m_imgHeightOut;
	//输出图像颜色表长度
	int m_nColorTableLengthOut;
public:
	//不带参数的构造函数
	FeatureDetect();
	//带参数的构造函数
	FeatureDetect(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
		unsigned char *pImgData);
	//析构函数
	~FeatureDetect();
	//以像素为单位返回输出图像的宽和高
	CSize GetDimensions();

	void ComputeMatrix(BYTE **LocalImage, int LocalImageWidth);
	void ComputeFeature(double &FeatureEnergy, double &FeatureEntropy, 
	double &FeatureInertiaQuadrature, double &FeatureCorrelation, 
	double &FeatureLocalCalm, int** pMatrix, int dim);

};

#endif // !defined(AFX_FEATUREDETECT_H__07B031B4_F2C2_4C25_BE7D_FE9272432868__INCLUDED_)
