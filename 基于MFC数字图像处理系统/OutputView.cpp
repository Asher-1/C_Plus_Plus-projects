// OutputView.cpp: 实现文件
//

#include "stdafx.h"
#include "MSMoneyDemo.h"
#include "OutputView.h"
#include "MainFrm.h"


// OutputView

IMPLEMENT_DYNCREATE(OutputView, CView)

OutputView::OutputView()
{

}

OutputView::~OutputView()
{
}

BEGIN_MESSAGE_MAP(OutputView, CView)
END_MESSAGE_MAP()


// OutputView 绘图

void OutputView::OnDraw(CDC* pDC)
{
	// TODO:  在此添加绘制代码
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
	if (pView->state2 == 1)
	{
		CMSMoneyDemoDoc* pDoc = GetDocument();								// 获取文档类指针
		ASSERT(pDoc != NULL);
		ImageDib* pDib = pDoc->m_dib_Result;											// 返回m_dib的指针
		CSize sizeFileDib = pDib->GetDimensions();										// 获取DIB的尺寸
		pDib->Draw(pDC, CPoint(0, 0), sizeFileDib);									   //显示DIB
	}

	
}


// OutputView 诊断

#ifdef _DEBUG
void OutputView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void OutputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CMSMoneyDemoDoc* OutputView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMSMoneyDemoDoc)));
	return (CMSMoneyDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// OutputView 消息处理程序
