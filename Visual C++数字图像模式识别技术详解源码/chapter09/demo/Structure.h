////Structure.h//
//包含程序中的所有数据结构，类和结构体组成

#ifndef STRUCTURE_H
#define STRUCTURE_H
//正常c++
#include <vector>
#include <list>
#include <string>

using namespace std;

#define pi 3.1415927
const int Swh=24;                            //字符归一化大小28
const int Feature=13;//22                        //汉字的特征数//重新个数大小
const int SUMCHINESE=3768;                   //一级汉字样本数（要加入四种字体）
//const int SUMCHINESE=40;
const int SUMCHAR=567;                      //字符数样本数
//根据连通体
const int CHARHOLE0=362;                    //字符0孔洞数
const int CHARHOLE1=141;                     //字符1孔洞数
const int CHARHOLE2=53;                     //字符2孔洞数
const int CHARHOLE3=11;                      //字符3孔洞数+大于3孔洞数
//const double REFUSE=0.025;                   //汉字拒识率


//公式行特征
struct h
{
	int m_iX1;//任意两行
	int m_iX2;
	int m_iH;//行高
	int m_ias;//上行间距
	int m_ibs;//下行间距	
};

///聚类的特征（用于切分）
struct cluster
{
	int first;//高度
	int second;//个数
	int third;//均值
};

///位置特征（矩形区域的通用结构）
class Position
{
public:
	int m_left;
	int m_top;
	int m_bottom;
	int m_right;

	Position():m_left(0),m_top(0),m_bottom(0),m_right(0){}

};


//文档中各个对象属性一个类,即每个对象就是一个字符，其特征可以增减！
enum Type{Chinese,Formular};//Type t=0/1/2

struct coordinate 
{
	int x;
	int y;
};//任意字符的坐标位置


//汉字字符
class CFtChinese
{
public:	
	Type m_type;//确定是否为汉字
	Position m_pos;
	int m_iImage[48][48];//汉字归一化大小48（可调，根据特征数）//归一化大小可变32
    //////////////////////////////////////////////////////////////////////////
    //string m_Chinese;//原字符无法获取
	//////////////////////////////////////////////////////////////////////////
	//features
	double m_iFtCrude[129];//汉字特征数（可变）
	//double m_iCenter[129];//质心特征
    //double m_iFtPer[78];//细分特征(65)
	int m_iHole;

	//double m_dIndex;
	int m_dIndex;
	int m_iCrude;
	double e;
	
	int m_iSumBlack;
	double m_iDensity;
	int i_row;
	//mine
	//int m_storke;//笔划密度特征
	//int m_wavelet;//小波
	//enum Font{Song,FangSong,Hei,Kai};//字体选择（先选择字体，再识别）
	list<coordinate> m_FtXY;//汉字特征点坐标
    //CFtFormular m_Formular;//行特征
	CFtChinese()
	{
		for (int i=0;i!=48;++i)
		{
			for (int j=0;j!=48;++j)
			{
				m_iImage[i][j]=0;
			}
		}
		m_iSumBlack=0;
		m_iDensity=0.0;
	}

};
//字符特征
class CFtCharacter
{
public:
   int m_iSinF;//独立行行值
   int m_iInFX;//内嵌行行值
   int m_iInFY;//内嵌行中字符的序号（列号）
   Position m_pos;//字符区域

   //////////////////////////////////////////////////////////////////////////
   //string m_Char;//原字符无法获取
   //////////////////////////////////////////////////////////////////////////
   
   //features
   int m_iMiddleX;//归一化中心特征X
   int m_iMiddleY;//归一化中心特征Y
   int m_iCenterX;//质心X特征
   int m_iCenterY;//质心Y特征

   //int m_projectX;//投影特征X
   //int m_projectY;//投影特征Y
   double m_dRationWH;//高宽比
   int m_iHole;//孔洞数
   int m_iFeature[Feature];//特征向量
   int m_iImage[Swh][Swh];//归一化模板
   int m_iIndex;//在样本集中的序号（样本需要写入）
   //识别完后
   int m_iType;//字符属于哪一类特征字符（识别之后，结构分析）
   //后标型1 上下型2 包含型3 （下标型4） 括号型5 独立型6 （多行型7） 普通型8 多重型9 合并型0
   int m_iType2;   //up=1 center=2 down=3 辅助标号=4 合并型标号=5

   CFtCharacter()
   {
	   for (int i=0;i!=Swh;++i)
	   {
		   for (int j=0;j!=Swh;++j)
		   {
			   m_iImage[i][j]=0;
		   }
	   }
   }
   //overload
   bool operator<(CFtCharacter &item)
   {
	   if (m_iMiddleY<item.m_iMiddleY)
	   {
		   return TRUE;
	   }
	   else
		   return FALSE;
   }
   
};
///*
class CFtCharactor
{
public:
   int m_iSinF;//独立行行值
   int m_iInFX;//内嵌行行值
   int m_iInFY;//内嵌行中字符的序号（列号）
   Position m_pos;//字符区域

   //////////////////////////////////////////////////////////////////////////
   //string m_Char;//原字符无法获取
   //////////////////////////////////////////////////////////////////////////
   
   //features
   int m_iMiddleX;//归一化中心特征X
   int m_iMiddleY;//归一化中心特征Y
   int m_iCenterX;//质心X特征
   int m_iCenterY;//质心Y特征

   //int m_projectX;//投影特征X
   //int m_projectY;//投影特征Y
   double m_dRationWH;//高宽比
   int m_iHole;//孔洞数
   int m_iFeature[Feature];//特征向量
   int m_iImage[Swh][Swh];//归一化模板
   int m_iIndex;//在样本集中的序号（样本需要写入）
   //识别完后
   int m_iType;//字符属于哪一类特征字符（识别之后，结构分析）
   //后标型1 上下型2 包含型3 （下标型4） 括号型5 独立型6 （多行型7） 普通型8 多重型9 合并型0
   int m_iType2;   //up=1 center=2 down=3 辅助标号=4 合并型标号=5

   CFtCharactor()
   {
	   for (int i=0;i!=Swh;++i)
	   {
		   for (int j=0;j!=Swh;++j)
		   {
			   m_iImage[i][j]=0;
		   }
	   }
   }
   //overload
   bool operator<(CFtCharactor &item)
   {
	   if (m_iMiddleY<item.m_iMiddleY)
	   {
		   return TRUE;
	   }
	   else
		   return FALSE;
   }
   
};//*/

class CCharInit
{
public:
	int m_iCenterX;                           //质心X坐标
	int m_iCenterY;                           //质心Y坐标
	double m_dRatioWH;                        //宽高比
	int m_iFeature[Feature];                  //特征向量
	int m_iIndex;	                          //序数
	int m_iType;		                      //特征字符的类别（结构分析使用） 
	//后标型1 上下型2 包含型3 (下标型4) 括号型5 独立型6 (多行型7) 普通型8 多重型9 合并型0
	int m_iType2;		                      // up=1  center=2  down=3 辅助标号＝4 合并型标号＝5

};//特征（字符）
//公式行（定位,切分处理问题）
struct CFtFormular
{
	int iRow;//行号
	double HT;//归一化行高
	double AS;//上行间距=到上行的距离/行段平均高
	double BS;//下行间距=到下行的距离/行段平均高
	double LI;//左缩进=左边距/行宽
	double RI;//右缩进=右边距/行宽
	double LD;//公式编号和公式的距离归一化=公式编号和公式的距离/行段平均高
	double DE;//密度=区域黑像素的数目/区域像素总数

	//CFtChinese m_Chinese;//汉字
};
//////////////////////////////////////////////////////////////////////////
//algorithms 的结构
//汉字,字符
class match
{
public:
	int index;
	double e;
	int InitChar;
};//匹配算法

//Layout Analyse
enum Type2{Table,Image};
class layoutStru
{
public:
	Position m_pos;
	double area;
    
	Type2 m_type2;
	
};

//文本块
/*class CText
{
public:
   int m_BL;//块所在行号
   Position t_pos;

   CFtChinese *p_Chines;
   CFtCharacter *p_Char;

};*/
//表格块
class CForm
{
public:
	Position m_Fpos;
	double area;

	CFtChinese *p_Chinese;
	//表格处理函数
};
//图形块
class CImage
{
public:
	Position b_pos;
    //压缩图像区域
};

//字符结构分析
class StruAna
{
public:
	Position m_pos;
	int m_iMiddleY;//竖直方向上的
	int m_iMiddleX;//水平方向上的
	int m_iIndex;
	int m_iType;
	int m_iType2;
};
//////////////////////////////////////////////////////////////////////////
typedef struct{
	int Value;
	int Dist;
	int AngleNumber;
}	MaxValue;//结构体(Hough)
//////////////////////////////////////////////////////////////////////////
class Number
{
public:
   int m_value;
   int m_count;

   Number():m_value(0),m_count(0){}

};//表格区分
class Line 
{
public:
	int line1;
	int line2;
	int line;

	Line():line1(0),line2(0),line(0){}
};

#endif