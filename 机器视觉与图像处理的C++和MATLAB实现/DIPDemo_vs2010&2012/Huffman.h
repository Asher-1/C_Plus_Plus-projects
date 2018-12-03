// Huffman.h
// 霍夫曼编码头文件
#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 节点结构体
typedef struct huffNode
{
	int parent,lchild,rchild;
	unsigned long count;		// 字符出现的次数
	unsigned char c;			// 对于的字符
	char bits[256];				// 编码串
}HuffNode;

#define N_Node 512				// 256个字符，需要511个节点才能构成霍夫曼树

class HuffCode
{

public:
	HuffCode():m_fpInfile(NULL), m_fpOutFile(NULL){}
	~HuffCode(){ xInit_Tree(); }

	// 设置输入文件指针
	void SetInputFile(FILE *fpInfile)
	{
		m_fpInfile = fpInfile;
	}

	// 设置输出文件指针
	void SetOutputFile(FILE *fpOutFile)
	{
		m_fpOutFile = fpOutFile;
	}

	// get函数
	double GetRawFileEntropy();
	double GetAverCodeLen();
	double GetRate(){return double(m_nCompressFile)/double(m_nRawFile);}
	unsigned int GetRawSize() {return m_nRawFile;}
	unsigned int GetNUsedChar() {return m_nUsedChar;}
	unsigned int GetMaxCodeLen() {return m_nMaxCodeLen;}

	void BuildTree();				// 构建霍夫曼树
	unsigned int EncodeFile();		// 编码
	unsigned int DecodeFile();		// 解码
		
public:
	HuffNode m_HTree[N_Node];		// 霍夫曼节点数组

private:
	FILE *m_fpInfile;				// 输入文件指针
	FILE *m_fpOutFile;				// 输出文件指针
	unsigned int m_nRawFile;			// 未压缩文件字节数
	unsigned int m_nCompressFile;   // 压缩文件字节数
	unsigned int m_nUsedChar;		// 使用到的字符个数
	unsigned int m_nMaxCodeLen;		// 最长编码串的长度

private:
	void xInit_Tree();				// 初始化每个节点   
};

#endif


