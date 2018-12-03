
// FeatureReuseView.h : CFeatureReuseView 类的接口
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

#include "opengl\OpenGLDC.h"
#pragma once

class CFeatureReuseView : public CGLView
{
protected: // 仅从序列化创建
	CFeatureReuseView();
	DECLARE_DYNCREATE(CFeatureReuseView)

// 属性
public:
	CFeatureReuseDoc* GetDocument() const;


	double CalFrequency(); // 用于屏幕刷新帧率的显示FPS

// 操作
public:
	GLfloat m_xAngle;
	GLfloat m_yAngle; // 声明旋转角度
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	float m_Scale;


	void myIdle(void); // 用于动画显示间隔
	static int time; // 动画间隔时间

// 重写
public:
	virtual void			RenderScene(COpenGLDC* pDC);
	virtual BOOL		PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL		OnPreparePrinting(CPrintInfo* pInfo);
	virtual void			OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void			OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	afx_msg void			OnSize(UINT nType, int cx, int cy);
	virtual void				OnInitialUpdate();
	virtual BOOL			GetBox(double& x0,double& y0,double& z0,double& x1,double& y1,double& z1);


// 实现
public:
	virtual ~CFeatureReuseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:

	// 视图选择的命令处理函数
	afx_msg void OnViewBack();
	afx_msg void OnViewBottom();
	afx_msg void OnViewFront();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewSWIsometric();
	afx_msg void OnViewSEIsometric();
	afx_msg void OnViewNEIsometric();
	afx_msg void OnViewNWIsometric();
	// 全局缩放函数
	afx_msg void OnViewZoomall();
	// 模型显示放大函数
	afx_msg void OnViewZoomin();
	// 模型显示缩小函数
	afx_msg void OnViewZoomout();
	// 渲染/非渲染切换
	afx_msg void OnViewShade();
	// 响应键盘消息
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // FeatureReuseView.cpp 中的调试版本
inline CFeatureReuseDoc* CFeatureReuseView::GetDocument() const
   { return reinterpret_cast<CFeatureReuseDoc*>(m_pDocument); }
#endif

