// NumRecognitin.cpp: implementation of the NumRecognition class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "demo1.h"
#include "NumRecognition.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern double m_charsample[25];//待测的数字
extern pattern pattern[10];//数字模板
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NumRecognition::NumRecognition()
{

}

NumRecognition::~NumRecognition()
{

}


number_no NumRecognition::LeastDistance()
{
	double min=10000000000;
	number_no number;
	int i,n;
	for(n=0;n<10;n++)
	{
		for(i=0;i<pattern[n].number;i++)
		{
			if(match(pattern[n].feature[i],m_charsample)<min)
			{
				//匹配的最小值
				min=match(pattern[n].feature[i],m_charsample);
				number.number  =n;//样品类别
				number.no=i;//样品序号
			}
		}
	}
	return number;//返回数字的类别和序号
}


/****************************************************************
*   函数名称：pipei(double s1[], double s2[])
*   函数类型：double 
*   参数说明：double s1[], double s2[]:两个样品的特征
*   函数功能：计算两个样品的匹配程度 ,返回两各样品的的匹配程度。
****************************************************************/
double NumRecognition::match(double s1[], double s2[])
{
	double count=0.0;
	for(int i=0;i<25;i++)
	{
		count+=(s1[i]-s2[i])*(s1[i]-s2[i]);
	}
	return count;
}
