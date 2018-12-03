// DlgArith.cpp : 实现文件
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgArith.h"
#include "afxdialogex.h"
#include <math.h>


// CDlgArith 对话框

IMPLEMENT_DYNAMIC(CDlgArith, CDialogEx)

CDlgArith::CDlgArith(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgArith::IDD, pParent)
	, m_sInput(_T(""))
	, m_sEncode(_T(""))
	, m_sDecode(_T(""))
	, m_showResult(_T(""))
	, m_p(0)
{

}

CDlgArith::~CDlgArith()
{
}

void CDlgArith::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_, m_sInput);
	DDX_Text(pDX, IDC_EDIT_EN, m_sEncode);
	DDX_Text(pDX, IDC_EDIT_DE, m_sDecode);
	DDX_Text(pDX, IDC_STATIC_EN, m_showResult);
	DDX_Text(pDX, IDC_P, m_p);
}


BEGIN_MESSAGE_MAP(CDlgArith, CDialogEx)
	ON_BN_CLICKED(IDC_BUT_ENCODE, &CDlgArith::OnBnClickedButEncode)
	ON_BN_CLICKED(IDC_BUT_DECODE, &CDlgArith::OnBnClickedButDecode)
END_MESSAGE_MAP()


// CDlgArith 消息处理程序


BOOL CDlgArith::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sInput="011011101101111101110101";
	p=0.0;
	m_p=-1.0;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgArith::OnBnClickedButEncode()
{
	UpdateData(true);
	char *in=(LPSTR)(LPCTSTR)m_sInput;
	int n=strlen(in);
	int i;
	if(m_p>0 && m_p<1)
		p=m_p;
	else
	{	p=0.0;
		for (i=0; i<n; i++)
		{
			if (in[i]!='0' && in[i]!='1')
			{
				AfxMessageBox("只能输入0或1组成的串！");
				return;
			}
			if(in[i]=='0')
				p++;
		}
		p/=n;
	}
	char out[512];
	int outN;
	arith_Encode(in,  n, p, out, outN);
	out[outN]='\0';
	m_sEncode.Format("%s", out);
	m_showResult.Format("编码结果（压缩率: %f%%）：", double(outN)/n*100);
	UpdateData(false);
}

void CDlgArith::OnBnClickedButDecode()
{
	UpdateData(true);
	char *en=(LPSTR)(LPCTSTR)m_sEncode;
	int n_en=strlen(en);
	if(n_en==0)
	{
		AfxMessageBox("先编码再解码！");
		return;
	}
	int n_de=m_sInput.GetLength();
	char de[512];
	arith_Decode(en, n_en, p, de,  n_de);
	de[n_de]='\0';
	m_sDecode.Format("%s", de);
	UpdateData(false);
}



/*******************************
void CDlgArith::arith_Encode(const char *in, int n, double p, char *out, int &outN)
功能：输入长为n的二进制序列in，输出编码结果out，out长度为outN。
参数：
const char *in：	输入01序列如“001100110”
int n：			输入01序列的长度
double p：		0的先验概率
char *out：		输出的编码结果，也是二进制序列
int &outN：		输出结果的长度
返回值：
无
*******************************/
void CDlgArith::arith_Encode(const char *in, int n, double p, char *out, int &outN)
{
	if(in==NULL || n<=0)
		return ;

	double low=0.0;
	double high=1.0;
	double range=high-low;
	double low_0=0.0, high_0=p;
	double low_1=p, high_1=1.0;
	double fp=1.0;
	int i;
	for (i=0; i<n; i++)
	{
		if(in[i]=='0')
		{
			high=low + range * high_0;
			low=low + range * low_0;
			range = high - low;
			fp *= (high_0 - low_0);
		}else
		{
			high=low + range * high_1;
			low=low + range * low_1;
			range = high - low;
			fp *= (high_1 - low_1);
		}
		printf("low: %f\n",low);
	}
	outN = (int)(floor(-log10(fp)/log10(2.0))+1);
	for(i=0; i<outN; i++)
	{
		double dpi=pow(0.5, i+1);
		if (low >= dpi)
		{
			out[i] = '1';
			low -= dpi;
		}
		else
		{
			out[i]='0';
		}
	}
	// 处理余数,进位
	if (low>0)
	{
		for(i=outN-1; i>=0; i--)
		{
			if(out[i]=='1')
			{
				out[i]='0';
			}else
			{
				out[i]='1';
				break;
			}
		}
	}
}

/*******************************
void CDlgArith::arith_Decode(const char *en, int n_en, double p, char *de, int n_de)
功能：输入长为n_en的二进制序列en，输出算术编码解码结果de，de长度为n_de。
参数：
const char *en：	输入01序列如“001100110”
int n_en：		输入01序列的长度
double p：		0的先验概率
char *de：		输出的解码结果，也是二进制序列
int n_de：		输出结果的长度
返回值：
无
*******************************/
void CDlgArith::arith_Decode(const char *en, int n_en, double p, char *de, int n_de)
{
	if(en==NULL || n_en<=0 || n_de<=0)
		return ;

	double low_0=0.0, high_0=p;
	double low_1=p, high_1=1.0;

	// 计算浮点数
	double f=0.0;
	int i;
	for(i=0; i<n_en; i++)
	{
		double dpi=pow(0.5, i+1);
		if (en[i]=='1')
		{
			f += dpi;
		}
	}

	// 解码 
	for (i=0; i<n_de; i++)
	{
		if(f<p) // 输出0
		{
			de[i]='0';
			f = (f-low_0)/(high_0-low_0);
		}else
		{
			de[i]='1';
			f = (f-low_1)/(high_1-low_1);
		}
	}
	
}



