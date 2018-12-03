// GetFeature.cpp: implementation of the GetFeature class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "GetFeature.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

double m_charsample[25];//待测的数字
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GetFeature::GetFeature()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

GetFeature::GetFeature(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData):
ImageDib(size, nBitCount, lpColorTable, pImgData)
{

    m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

GetFeature::~GetFeature()
{
	//释放输出图像位图数据缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
    	m_pImgDataOut=NULL;
	}

	//释放输出图像颜色表
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}
}

/***************************************************************
*	函数名称：GetPosition()
*   函数类型：void 
*   函数功能：搜索数字的位置，赋值给bottom,top,right,left
****************************************************************/
GetFeature::GetPosition()
{

	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	int i,j;
	BOOL flag;
	for(j=0;j<m_imgHeight;j++)
	{
		flag=FALSE;
		for(i=0;i<m_imgWidth;i++)
			if(*(m_pImgData+j*lineByte+i)==0)
			{
				flag=TRUE;
				break;
			}
		if(flag)
			break;
	}
	m_charbottom=j;
	for(j=m_imgHeight-1;j>0;j--)
	{
		flag=FALSE;
		for(i=0;i<m_imgWidth;i++)
			if(*(m_pImgData+j*lineByte+i)==0)
			{
				flag=TRUE;
				break;
			}
		if(flag)
			break;
	}
	m_chartop=j;
	for(i=0;i<m_imgWidth;i++)
	{
		flag=FALSE;
		for(j=0;j<m_imgHeight;j++)
			if(*(m_pImgData+j*lineByte+i)==0)
			{
				flag=TRUE;
				break;
			}
		if(flag)
			break;
	}
	m_charleft=i;
	for(i=m_imgWidth-1;i>0;i--)
	{
		flag=FALSE;
		for(j=0;j<m_imgHeight;j++)
			if(*(m_pImgData+j*lineByte+i)==0)
			{
				flag=TRUE;
				break;
			}
		if(flag)
			break;
	}
	m_charright=i;
}

/****************************************************************
*   函数名称：Cal(int row, int col)
*   函数类型：double 
*   参数说明：int row, int col:第row行，第col个区域
*   函数功能：计算某一小区域内黑像素所占比例,返回某一小区域内黑像素所占比例
****************************************************************/
double GetFeature::Cal(int row, int col)
{
	double w,h,count;
	w=(m_charright-m_charleft)/5;
	h=(m_chartop-m_charbottom)/5;
	count=0;

	int lineByte=(m_imgWidth*m_nBitCount/8+3)/4*4;
	int i,j;
	for(j=m_charbottom+row*h;j<m_charbottom+(row+1)*h;j++)
	for(i=m_charleft+col*w;i<m_charleft+(col+1)*w;i++)
	{
		if(*(m_pImgData+j*lineByte+i)==0)
			count++;
	}

	return (double)count/(w*h);
}

/***************************************************************
*   函数名称：SetFeature()
*   函数类型：void 
*   函数功能：将数字特征保存在变量testsample中
****************************************************************/
GetFeature::SetFeature()
{
	int i,j;
	for(j=0;j<5;j++)
	{
		for(i=0;i<5;i++)
		{
			m_charsample[5*(4-j)+i]=Cal(j,i);
		}
	}
	i=1;
}

CSize GetFeature::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

GetFeature::ShowFeature()
{

	//输出图像为标记图象的输出,此处为灰度类型
	m_nBitCountOut=8;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=250;
	m_imgHeightOut=250;
	
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];
	memset(m_pImgDataOut,255,lineByte*m_imgHeightOut);

	//循环变量，图像的坐标
	int i,j,m,n;

	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			if(m_charsample[5*i+j]>0.1){
				for(m=5;m<45;m++)
					for(n=5;n<45;n++)
						*(m_pImgDataOut+(250-i*50-m)*lineByte+j*50+n)=0;
	}
}

GetFeature::ShowPattern(double feature[])
{
	//输出图像为标记图象的输出,此处为灰度类型
	m_nBitCountOut=8;
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=250;
	m_imgHeightOut=250;
	
	int lineByte=(m_imgWidthOut*m_nBitCountOut/8+3)/4*4;
	m_pImgDataOut=new unsigned char[lineByte*m_imgHeightOut];
	memset(m_pImgDataOut,255,lineByte*m_imgHeightOut);

	//循环变量，图像的坐标
	int i,j,m,n;

	for(i=0;i<5;i++)
		for(j=0;j<5;j++)
			if(feature[5*i+j]>0.1){
				for(m=5;m<45;m++)
					for(n=5;n<45;n++)
						*(m_pImgDataOut+(250-i*50-m)*lineByte+j*50+n)=0;
	}


}
