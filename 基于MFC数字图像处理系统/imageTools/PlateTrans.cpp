// PlateTrans.cpp: implementation of the PlateTrans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlateTrans.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PlateTrans::PlateTrans()
{
	m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0	

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0
}

PlateTrans::PlateTrans(CSize size, int nBitCount, LPRGBQUAD lpColorTable, 
					 unsigned char *pImgData1,unsigned char *pImgData2):
ImageDib(size, nBitCount, lpColorTable, pImgData1)
{

    m_pImgDataOut=NULL;//输出图像位图数据指针为空

	m_lpColorTableOut=NULL;//输出图像颜色表指针为空
	
	m_nColorTableLengthOut=0;//输出图像颜色表长度为0

    m_nBitCountOut=0;//输出图像每像素位数为0

	m_imgWidthOut=0;//输出图像的宽为0

	m_imgHeightOut=0;//输出图像的高为0

	m_pImgDataIn1=pImgData1;//m_pImgData;
	m_pImgDataIn2=pImgData2;
}

PlateTrans::~PlateTrans()
{

}

void PlateTrans::ColorPairSearch()
{

	//若灰度图像,则返回
	if(m_nBitCount==8) return;

	//释放旧的输出图像数据及颜色表缓冲区
	if(m_pImgDataOut!=NULL){
		delete []m_pImgDataOut;
		m_pImgDataOut=NULL;
	}
	if(m_lpColorTableOut!=NULL){
		delete []m_lpColorTableOut;
		m_lpColorTableOut=NULL;
	}

	//灰值化后,每像素位数为8比特
	m_nBitCountOut=8;

	//颜色表长度
	m_nColorTableLengthOut=ComputeColorTabalLength(m_nBitCountOut);

	//申请颜色表缓冲区,生成灰度图像的颜色表
	if(m_nColorTableLengthOut!=0){
    	m_lpColorTableOut=new RGBQUAD[m_nColorTableLengthOut];
		for(int i=0; i<m_nColorTableLengthOut;i++){
			m_lpColorTableOut[i].rgbBlue=i;
			m_lpColorTableOut[i].rgbGreen=i;
			m_lpColorTableOut[i].rgbRed=i;
			m_lpColorTableOut[i].rgbReserved=0;
		}
	}

	//输入图像每像素字节数,彩色图像为3字节/像素
	int pixelByteIn=3;
	
	//输入图像每行像素所占字节数,必须是4的倍数
	int lineByteIn=(m_imgWidth*pixelByteIn+3)/4*4;

	//输出图像的宽高,与输入图像相等
	m_imgWidthOut=m_imgWidth;
	m_imgHeightOut=m_imgHeight;

	//输出图像每行像素所占字节数,必须是4的倍数
	int lineByteOut=(m_imgWidth*m_nBitCountOut/8+3)/4*4;

	//申请输出图像位图数据缓冲区
	m_pImgDataOut=new unsigned char[lineByteOut*m_imgHeight];

	//循环变量,图像的坐标
	int i,j,k,j1,j2,flag;
	char c_t1[4],c_t2[4],c_t3[4],c_t4[4];

	for(i=0;i<m_imgHeight;i++){
		for(j=0;j<m_imgWidth;j++){
			*(m_pImgDataOut+i*lineByteOut+j)=255;
		}
	}
			
	//根据灰值化公式为输出图像赋值
	for(i=20;i<m_imgHeight-20;i++){
		flag=0;
		for(j=20;j<m_imgWidth-20;j++){
			if(flag==0){
				if(*(m_pImgDataIn2+i*lineByteOut+j)==255){
					for(k=0;k<3;k++){
						c_t1[k]=*(m_pImgDataIn1+i*lineByteIn+j*pixelByteIn+k);
						c_t2[k]=*(m_pImgDataIn1+i*lineByteIn+(j-1)*pixelByteIn+k);
						c_t3[k]=*(m_pImgDataIn1+i*lineByteIn+(j-2)*pixelByteIn+k);
						c_t4[k]=*(m_pImgDataIn1+i*lineByteIn+(j-3)*pixelByteIn+k);
					}
					if(BeBlue(c_t1)||BeBlue(c_t2)||BeBlue(c_t3)||BeBlue(c_t4)){
						j1=j;
	                    j2=0;
		                flag=1;
					}
				}
			}
			else{
				for(k=0;k<3;k++){
					c_t1[k]=*(m_pImgDataIn1+i*lineByteIn+(j+1)*pixelByteIn+k);
					c_t2[k]=*(m_pImgDataIn1+i*lineByteIn+(j+2)*pixelByteIn+k);
					c_t3[k]=*(m_pImgDataIn1+i*lineByteIn+(j+3)*pixelByteIn+k);
				}
				if(BeWhite(c_t1)||BeWhite(c_t2)||BeWhite(c_t3)){
					j2=j;
				}
				else{
					if(BeBlue(c_t1)||BeBlue(c_t2)||BeBlue(c_t3)){
						if(j2-j1<50){
							for(k=j1;k<j2;k++){
								*(m_pImgDataOut+i*lineByteOut+k)=0;
							}
						}
						flag=0;
						j1=0;
					}
					else{
						flag=0;
						j1=0;
					}
				}

			}
		}
	}

}

CSize PlateTrans::GetDimensions()
{	
	if(m_pImgDataOut == NULL) return CSize(0, 0);
	return CSize(m_imgWidthOut, m_imgHeightOut);
}

int PlateTrans::BeBlue(char *Col)
{
	float f1,f2;
	int C_b=(int)Col[0]&255;
	int C_g=(int)Col[1]&255;
	int C_r=(int)Col[2]&255;

	if(C_r<1) C_r=1;
	if(C_g<1) C_g=1;


	f1=(float)C_b/C_r;
	f2=(float)C_b/C_g;
	if((f1>1.4)&&(f2>1.4)&&(C_b>20)){
		return 1;
	}
	else{
	    return 0;
	}

}

int PlateTrans::BeWhite(char *Col)
{
	float C_sum;

	int C_b=(int)Col[0]&255;
	int C_g=(int)Col[1]&255;
	int C_r=(int)Col[2]&255;

	C_sum=C_b+C_r+C_g;
	if(C_sum>200)
	{
		if((C_r/C_sum)<0.4&&(C_b/C_sum)<0.4&&(C_g/C_sum)<0.4){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}

}

void PlateTrans::charrecognize()
{
	initmodule();
	int i,j,k,l,m,n;
	int pixels[7][20][40];
	CSize size;
	size.cx=160;
	size.cy=60;
	int lineByte=(size.cx+3)/4*4;

	for(i=0;i<7;i++){
		for(j=0;j<20;j++){
			for(l=0;l<40;l++){
				if(*(m_pImgDataIn1+(10+l)*lineByte+i*20+10+j)==0){
					pixels[i][j][l]=1;
				}
				else{
					pixels[i][j][l]=0;
				}
			}
		}
	}

    float dis[40],distemp,distemp1,distsum,distsum1;
    int nummatch[7],xx,yy;
    int mfrom,mnum;
	
	for(i=0;i<7;i++){
		switch (i){
            case 0:
                mfrom=36;
                mnum=2;
                break;
            case 1:
                mfrom=10;
                mnum=26;
                break;
            case 2:
                mfrom=0;
                mnum=36;
                break;
            default:
                mfrom=0;
                mnum=10;
                break;
        }

        for(l=mfrom;l<mfrom+mnum;l++){
			distsum=0;
            distsum1=0;
            for(j=0;j<20;j++){
                for(k=0;k<40;k++){
                    if(pixels[i][j][k]==1){
                        distemp=30;
                        for(m=j-3;m<=j+3;m++){
                            for(n=k-3;n<=k+3;n++){
                                xx=m;
                                yy=n;
                                if(m<0)  xx=0;
                                if(m>19) xx=19;
                                if(n<0)  yy=0;
                                if(n>39) yy=39;
                                if(m_modules[l][xx][yy]==1){
                                    distemp1=sqrt((xx-j)*(xx-j)+(yy-k)*(yy-k));
                                    if(distemp1<distemp) distemp=distemp1;
                                }
                            }
                        }
                        distsum+=distemp;
                    }
                }
            }

            for(j=0;j<20;j++){
                for(k=0;k<40;k++){
                    if(m_modules[l][j][k]==1){
                        distemp=30;
                        for(m=j-3;m<=j+3;m++){
                            for(n=k-3;n<=k+3;n++){
                                xx=m;
                                yy=n;
                                if(m<0)  xx=0;
                                if(m>19) xx=19;
                                if(n<0)  yy=0;
                                if(n>39) yy=39;
                                if(pixels[i][xx][yy]==1){
                                    distemp1=sqrt((xx-j)*(xx-j)+(yy-k)*(yy-k));
                                    if(distemp1<distemp) distemp=distemp1;
                                }
                            }
                        }
                        distsum1+=distemp;
                    }
                }
            }

            if(distsum>distsum1){
                dis[l]=distsum;
            }
            else{
                dis[l]=distsum1;
            }
        }

        nummatch[i]=mfrom;
        for(j=mfrom;j<mfrom+mnum;j++){
            if(dis[nummatch[i]]>dis[j]) nummatch[i]=j;
        }
    }
       
	CString Regresult;
	Regresult="识别结果:黑";
	for(i=1;i<7;i++){
		Regresult+=m_chars[nummatch[i]];
	}
	AfxMessageBox(Regresult);    
	
}

void PlateTrans::initmodule()
{
	int i,j,k,curpos;
//	char c1[1];
//	int p1,p2;
	
	TCHAR   sDrive[10];  
	TCHAR   sDir[200];  
	TCHAR   sFilename[200],Filename[200];  
	TCHAR   sExt[10];
	CString	homeDir,rfilename;

	GetModuleFileName(AfxGetInstanceHandle(),Filename,_MAX_PATH);  
	_tsplitpath(Filename,sDrive,sDir,sFilename,sExt);  
	homeDir=CString(sDrive)+CString(sDir);
	
	CFile file;
	LPBYTE mdata=NULL;

	for(i=0;i<36;i++){
		switch (i){
			case 0:
				m_chars[i]='0';
				rfilename=homeDir+"model\\0.txt";
				break;
			case 1:
				m_chars[i]='1';
				rfilename=homeDir+"model\\1.txt";
				break;
			case 2:
				m_chars[i]='2';
				rfilename=homeDir+"model\\2.txt";
				break;
	        case 3:
				m_chars[i]='3';
				rfilename=homeDir+"model\\3.txt";
				break;
	        case 4:
				m_chars[i]='4';
				rfilename=homeDir+"model\\4.txt";
				break;
	        case 5:
				m_chars[i]='5';
				rfilename=homeDir+"model\\5.txt";
				break;
	        case 6:
				m_chars[i]='6';
				rfilename=homeDir+"model\\6.txt";
				break;
			case 7:
				m_chars[i]='7';
				rfilename=homeDir+"model\\7.txt";       
				break;
	        case 8:
				m_chars[i]='8';
				rfilename=homeDir+"model\\8.txt";
				break;
	        case 9:
				m_chars[i]='9';
				rfilename=homeDir+"model\\9.txt";
				break;
	        case 10:
				m_chars[i]='A';
				rfilename=homeDir+"model\\a.txt";
				break;
	        case 11:	
				m_chars[i]='B';
				rfilename=homeDir+"model\\b.txt";
				break;
	        case 12:
				m_chars[i]='C';
				rfilename=homeDir+"model\\c.txt";
				break;
	        case 13:
				m_chars[i]='D';
				rfilename=homeDir+"model\\d.txt";
				break;
	        case 14:
				m_chars[i]='E';
				rfilename=homeDir+"model\\e.txt";
				break;
	        case 15:
				m_chars[i]='F';
				rfilename=homeDir+"model\\f.txt";
				break;
	        case 16:
				m_chars[i]='G';
				rfilename=homeDir+"model\\g.txt";
				break;
	        case 17:
				m_chars[i]='H';
				rfilename=homeDir+"model\\h.txt";
				break;
	        case 18:
				m_chars[i]='I';
				rfilename=homeDir+"model\\i.txt";
				break;
	        case 19:
				m_chars[i]='J';
				rfilename=homeDir+"model\\j.txt";
				break;
	        case 20:
				m_chars[i]='K';
				rfilename=homeDir+"model\\k.txt";
				break;
	        case 21:
				m_chars[i]='L';
				rfilename=homeDir+"model\\l.txt";
				break;
	        case 22:
				m_chars[i]='M';
				rfilename=homeDir+"model\\m.txt";
				break;
	        case 23:
				m_chars[i]='N';
				rfilename=homeDir+"model\\n.txt";
				break;
	        case 24:
				m_chars[i]='O';
				rfilename=homeDir+"model\\o.txt";
				break;
	        case 25:
				m_chars[i]='P';
				rfilename=homeDir+"model\\p.txt";
				break;
	        case 26:
				m_chars[i]='Q';
				rfilename=homeDir+"model\\q.txt";
				break;
	        case 27:
				m_chars[i]='R';
				rfilename=homeDir+"model\\r.txt";
				break;
	        case 28:
				m_chars[i]='S';
				rfilename=homeDir+"model\\s.txt";
				break;
	        case 29:
				m_chars[i]='T';
				rfilename=homeDir+"model\\t.txt";
				break;
	        case 30:
				m_chars[i]='U';
				rfilename=homeDir+"model\\u.txt";
				break;
	        case 31:
				m_chars[i]='V';
				rfilename=homeDir+"model\\v.txt";
				break;
	        case 32:
				m_chars[i]='W';
				rfilename=homeDir+"model\\w.txt";
				break;
	        case 33:
				m_chars[i]='X';
				rfilename=homeDir+"model\\x.txt";
				break;
	        case 34:
				m_chars[i]='Y';
				rfilename=homeDir+"model\\y.txt";
				break;
	        case 35:
				m_chars[i]='Z';
				rfilename=homeDir+"model\\z.txt";
				break;
		}

		if (!file.Open(rfilename, CFile::modeRead | CFile::shareDenyWrite))
			return ;
	
		try {
	
			if(mdata!=NULL)
				delete []mdata;
				mdata=new BYTE[file.GetLength()];
			
				file.Read(mdata, file.GetLength());
				curpos=0;
				j=file.GetLength();
				for(j=0;j<20;j++){
					for(k=0;k<40;k++){
						if((int)mdata[curpos]==13){
							curpos+=2;
						}
						m_modules[i][j][39-k]=mdata[curpos]-48;
						curpos++;
					}
				}

				file.Close();

			}
		catch(CException* pe) {

			AfxMessageBox("打不开模板文件!");
			pe->Delete();
			return;
		}
	}
	
	//函数返回
	return;

}


