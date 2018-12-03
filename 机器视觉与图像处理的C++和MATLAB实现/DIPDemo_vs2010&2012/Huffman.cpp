// Huffman.cpp
// 霍夫曼编码实现文件

#include "Huffman.h"
#include <math.h>


/**************************************************
unsigned int HuffCode::DecodeFile()
功能：	设置输入文件指针和输出文件指针后进行霍夫曼解码
参数：	无
返回值：	unsigned int，解码得到的原始文件字节数
***************************************************/
unsigned int HuffCode::DecodeFile()
{
	xInit_Tree();
	if (NULL == m_fpInfile || NULL == m_fpOutFile)
	{
		return -1;
	}
	fseek(m_fpInfile, 0, 2);
	m_nCompressFile = ftell(m_fpInfile);
	fseek(m_fpInfile, 0, 0);
	// 读取头信息:
	// m_nRawFile：原始文件的长度
	// m_nUsedChar:有用的字符数
	// m_nMaxCodeLen：最大码长
	// m_HTree:霍夫曼表
	fread(&m_nRawFile, sizeof(unsigned int),1,m_fpInfile); 
	fread(&m_nUsedChar, sizeof(unsigned int),1,m_fpInfile);
	fread(&m_nMaxCodeLen, sizeof(unsigned int),1,m_fpInfile);
		
	unsigned int i;
	for(i=0; i<=m_nUsedChar; i++)
	{
		fread(&m_HTree[i].c, sizeof(unsigned char), 1, m_fpInfile);
		fread(&m_HTree[i].bits, m_nMaxCodeLen, 1, m_fpInfile);
	}

	// 开始解压文件
	unsigned int nOutChar = 0;
	unsigned char c;		// 读取的字符
	char ac_digital[256];	// 读到的字符转为二进制字符串
	char buffer[256];		// 总的二进制字符串
	buffer[0] = '\0';
	while(true)
	{
		// 读取m_nMaxCodeLen个bit到buffer中 
		while(strlen(buffer) < m_nMaxCodeLen) 
		{
			fread(&c,1,1,m_fpInfile); 
			int k=c; 
			_itoa_s(k, ac_digital, 2); 
			k=strlen(ac_digital); 
			int l;
			for(l=8;l>k;l--) //在单字节内对相应位置补0
			{
				strcat(buffer,"0"); 
			}
			strcat(buffer,ac_digital); 
		}
		// buffer与码表中的值比较
		unsigned int i;
		for(i=0; i<=m_nUsedChar; i++) 
		{
			if( memcmp(m_HTree[i].bits, buffer, strlen(m_HTree[i].bits)) == 0)
			{
				break; 
			}
		}
		strcpy(buffer, buffer+strlen(m_HTree[i].bits)); 
		c = m_HTree[i].c; 
		fwrite(&c, 1, 1, m_fpOutFile); 
		m_HTree[i].count++;
		//统计解压缩后文件的长度，判断是否解压完成
		nOutChar++;   
		if(nOutChar == m_nRawFile) 
			break;  
	}
	return nOutChar;
}

/**************************************************
unsigned int HuffCode::EncodeFile()
功能：	设置输入文件指针和输出文件指针后进行霍夫曼编码
参数：	无
返回值：	unsigned int，压缩后的文件的字节数
***************************************************/
unsigned int HuffCode::EncodeFile()
{
	if (NULL == m_fpInfile || NULL == m_fpOutFile)
	{
		return -1;
	}

	// 写头信息：
	// 原文件总长度，4字节
	// 用到的字符个数，4字节
	// 最大码长，4字节
	// 霍夫曼码表
	fseek(m_fpOutFile,0,0);
	fwrite(&m_nRawFile, sizeof(unsigned int), 1, m_fpOutFile);         
	fwrite(&m_nUsedChar, sizeof(unsigned int), 1, m_fpOutFile);
	fwrite(&m_nMaxCodeLen, sizeof(unsigned int), 1, m_fpOutFile);
	unsigned int i;
	for(i=0; i<=m_nUsedChar; i++)
	{
		fwrite(&m_HTree[i].c, sizeof(unsigned char), 1, m_fpOutFile);
		fwrite(&m_HTree[i].bits, m_nMaxCodeLen, 1, m_fpOutFile);
	}

	// 压缩主文件
	char buf[N_Node];	// 存放编码结果
	int j = 0;			// 读取位置游标
	int c;				// 读取的字符
	int k;
	unsigned int n_wr = 12+(1+m_nMaxCodeLen)*(m_nUsedChar+1); // 写入的字节数

	buf[0] = '\0';
	fseek(m_fpInfile, 0, SEEK_SET);
	while(!feof(m_fpInfile))
	{
		c = fgetc(m_fpInfile);
		j++;
		for(i = 0; i <= m_nUsedChar; i++)
		{
			if(m_HTree[i].c == c)
				break;
		}
		strcat(buf, m_HTree[i].bits);


		// 累计编码位数超过一个字节，即对文件执行写入
		k = strlen(buf);
		c = 0;
		while(k>=8)
		{
			for(i=0;i<8;i++)
			{
				if(buf[i]=='1') 
					c=(c<<1)|1;
				else
					c=c<<1;
			}
			fwrite(&c,1,1,m_fpOutFile);
			n_wr++;
			strcpy(buf, buf+8);
			k = strlen(buf);
		}
		if(j == m_nRawFile)
			break;
	}
	if( k>0 )            //可能还有剩余字符
	{
		strcat(buf,"00000000");
		for(i=0; i<8; i++)
		{
			if(buf[i] == '1') 
				c = (c<<1)|1;
			else
				c = c<<1;
		}
		fwrite(&c, 1, 1, m_fpOutFile);
		n_wr++;
	}
	m_nCompressFile = n_wr;
	return n_wr;
}

/**************************************************
void HuffCode::xInit_Tree()
功能：	初始化每个霍夫曼树节点
参数：	无
返回值：	无
***************************************************/
void HuffCode::xInit_Tree()
{
	int i;
	// -1表示没有父母或子女
	for(i=0;i<N_Node;i++)
	{
		// 出现的次数为零
		m_HTree[i].count=0;
		// 各节点表示的字符
		m_HTree[i].c=(unsigned char)i;
		m_HTree[i].lchild=-1;
		m_HTree[i].parent=-1;
		m_HTree[i].rchild=-1;
	}
}

/**************************************************
double HuffCode::GetInFileEntropy()
功能：	返回未压缩文件的熵值
参数：	无
返回值：	double，未压缩文件的熵值
***************************************************/
double HuffCode::GetRawFileEntropy()
{
	if (NULL==m_fpInfile)
		return -1.0;
	unsigned int i;
	double entropy=0.0;
	
	for (i=0; i<=m_nUsedChar; i++)
	{
		
		if (m_HTree[i].count != 0)
		{
			double rate = 1.0*m_HTree[i].count/(double)m_nRawFile;
			entropy += rate*log(double(rate))/log(2.0);
		}
	}
	
	return -entropy;
}

/**************************************************
double HuffCode::GetAverCodeLen()
功能：	计算编码的平均码长
参数：	无
返回值：	double，编码的平均码长
***************************************************/
double HuffCode::GetAverCodeLen()
{
	if (NULL==m_fpInfile)
		return -1.0;
	unsigned int i;
	double len=0.0;
	int s=0;
	for (i=0; i<=m_nUsedChar; i++)
	{
		if (m_HTree[i].count != 0)
		{
			len+=strlen(m_HTree[i].bits)*m_HTree[i].count;
			s+=m_HTree[i].count;
		}
	}
	return len*1.0/s;;
}

/**************************************************
void HuffCode::BuildTree()
功能：	根据m_fpInfile指定的文件，读取文件内容，构建霍夫曼树
参数：	无
返回值：	无
***************************************************/
void HuffCode::BuildTree()
{
	unsigned int i,j,k;

	// 1.初始化树
	xInit_Tree();

	// 2.统计输入文件中各个字符出现次数
	m_nRawFile = 0;	
	unsigned char c; 
	while(!feof(m_fpInfile))
	{
		fread(&c,1,1,m_fpInfile);
		m_HTree[c].count++;
		m_nRawFile++;
	}
	m_nRawFile--;
	m_HTree[c].count--;

	// 3.count为0的不要，按count从大到小排列
	HuffNode temp;
	for(i=0;i<255;i++)
	{
		for(j=i+1;j<256;j++)
		{
			if(m_HTree[i].count<m_HTree[j].count)
			{
				temp=m_HTree[i];
				m_HTree[i]=m_HTree[j];
				m_HTree[j]=temp;
			}
		}
	}

	// 4.统计有用字符数
	for(i=0;i<256;i++)
		if(m_HTree[i].count==0)
			break;
	m_nUsedChar = i-1;   

	// 5.构建霍夫曼树
	unsigned int min_count;		// count的最小值
	unsigned int m=2*i-1;				// 整棵树共有 m 个节点
	int min_index;				// 最小count值节点的序号
	for(i=m_nUsedChar+1; i<m; i++)
	{
		min_count=UINT_MAX;
		for(j=0;j<i;j++)
		{
			// 在没有父节点的节点中寻找最小count值，若有父节点则跳过
			if(m_HTree[j].parent!=-1)  
				continue;
			if(min_count > m_HTree[j].count)
			{
				min_index=j;
				min_count=m_HTree[min_index].count;
			}
		}
		// pt是i的左孩子
		m_HTree[i].count=min_count; 
		m_HTree[min_index].parent=i;
		m_HTree[i].lchild=min_index;

		min_count=UINT_MAX;
		for(j=0;j<i;j++)
		{
			// 在没有父节点的节点中寻找最小count值，若有父节点则跳过
			if(m_HTree[j].parent!=-1) 
				continue;
			if(min_count>m_HTree[j].count)
			{
				min_index=j;
				min_count=m_HTree[min_index].count;
			}
		}
		// pt是i的右孩子
		m_HTree[i].count += min_count;
		m_HTree[min_index].parent=i;
		m_HTree[i].rchild=min_index;
	}

	// 6.为每个有权值的字符编码
	for(i=0; i<=m_nUsedChar; i++)
	{
		k=i;
		m_HTree[i].bits[0]=0;
		// 一直向上追溯，到达根节点为止，记录各个节点的编码符号，形成编码字符串，如“00110”
		while(m_HTree[k].parent != -1) 
		{
			j=k;
			k=m_HTree[k].parent;
			// 当前节点是父节点的左节点
			// 编码为0
			if(m_HTree[k].lchild==j)  
			{
				j=strlen(m_HTree[i].bits);  
				memmove(m_HTree[i].bits+1,m_HTree[i].bits,j+1);
				m_HTree[i].bits[0]='0';    
			}
			// 当前节点是父节点的右节点
			// 编码为1
			else						
			{
				j=strlen(m_HTree[i].bits); 
				memmove(m_HTree[i].bits+1, m_HTree[i].bits, j+1);
				m_HTree[i].bits[0]='1'; 
			}
		}
	}

	// 7.记录最长的编码长度
    m_nMaxCodeLen=0;           
	for(i=0;i<=m_nUsedChar;i++)
	{
		if(m_nMaxCodeLen<strlen(m_HTree[i].bits))
			m_nMaxCodeLen=strlen(m_HTree[i].bits);
	}

}


