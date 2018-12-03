
// MainFrm.h : CMainFrame 类的接口
//

#ifdef _DEBUG // 内存泄漏检测支持。
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <malloc.h>    // 解决 malloc.h 与 crtdbg.h 顺序导致的 Debug Assertion Failed, "Corrupted pointer passed to _freea" 。
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "DockingBar/ViewBar.h"
#include "CButtonST/BCMenu.h"
#include "CButtonST/BtnST.h"

#pragma once

class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

	static int m_Mode; // 单选模式选择变量
	static bool is_PrintArrows; // 是否绘制矢量箭头
	static bool is_Draw_cloudPicture; // 是否绘制彩色云图
	
	static int m_stage; // 下一步按钮的循环增量，用于每一步模型的重绘

	static int m_delta; // 用于动态显示的阶段循环增量
	static int m_Counts; // 单选模式选择变量

	static COLORREF m_Color; //静态成员 材质颜色属性


	bool isChecked;
	bool isChecked2;

// 操作
public:
	
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = 
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL,
		CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
	CMFCButton * NewMyRadio(int nID, CString titile, CRect rect, int nStyle);
	CMFCButton * NewMyButton(int nID, CString titile, CRect rect, int nStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;

	CMFCToolBarImages m_UserImages;

	CMFCToolBar		m_wndDisplayBar; // 自定义图形操作工具栏

	CMFCToolBar		ButtonBar; // 自定义图形操作工具栏

	CMFCButton *p_MyBut[4]; // 自定义按钮
	CMFCButton *p_MyRadio[4]; // 自定义单选按钮

	CMFCColorButton* m_ColorPicker;  // 颜色选择按钮


	void	DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

// 生成的消息映射函数
protected:
	// 按钮消息响应函数

	CButtonST m_btn;


	afx_msg void OnMybut1();
	afx_msg void OnMybut2();
	afx_msg void OnMybut3();

	afx_msg void OnMybut4();
	afx_msg void OnMybut5();
	afx_msg void OnMybut6();
	afx_msg void OnMybut7(); 

	afx_msg void OnMyColorbut();

	afx_msg void OnMyRadio1(); //单选按钮1
	afx_msg void OnMyRadio2(); //单选按钮2
	afx_msg void OnMyRadio3(); //单选按钮3
	afx_msg void OnMyRadio4();// 单选按钮4

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
};

