#pragma once

#include "Huffman.h"
#include "afxcmn.h"
// CDlgHuffman 对话框

class CDlgHuffman : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHuffman)

public:
	CDlgHuffman(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHuffman();
	HuffCode huffcode;
// 对话框数据
	enum { IDD = IDD_DLG_Huffman };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrInput();
	// 输入文件路径，绑定编辑框
	CString m_inFilePath;
	// 输出文件路径，绑定编辑框
	CString m_OutFilePath;
	afx_msg void OnBnClickedButtonEncode();
	double m_entropy;
	double m_dAverCodeLen;
	double m_rate;
	CListCtrl m_list;
	virtual BOOL OnInitDialog();
	void Update();
	afx_msg void OnBnClickedButtonDecode();
	afx_msg void OnBnClickedButtonBrOutput();
	afx_msg void OnBnClickedClose();
	double m_dCodeEff;
};

