#pragma once
#include "MSMoneyDemoDoc.h"

// OutputView 视图

class OutputView : public CView
{
	DECLARE_DYNCREATE(OutputView)

protected:
	OutputView();           // 动态创建所使用的受保护的构造函数
	virtual ~OutputView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	CMSMoneyDemoDoc * GetDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


