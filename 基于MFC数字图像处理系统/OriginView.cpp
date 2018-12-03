// OriginView.cpp: 实现文件
//

#include "stdafx.h"
#include "MSMoneyDemo.h"
#include "OriginView.h"
#include "MainFrm.h"


// OriginView

IMPLEMENT_DYNCREATE(OriginView, CView)

OriginView::OriginView()
{

}

OriginView::~OriginView()
{
}

BEGIN_MESSAGE_MAP(OriginView, CView)
END_MESSAGE_MAP()


// OriginView 绘图
void OriginView::OnDraw(CDC* pDC)
{

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
	if (pView->state2 == 1)
	{
		CMSMoneyDemoDoc* pDoc = GetDocument();									// 获取文档类指针
		ASSERT(pDoc != NULL);
		ImageDib* pDib = pDoc->m_dib;																// 返回m_dib的指针
		CSize sizeFileDib = pDib->GetDimensions();										// 获取DIB的尺寸
		pDib->Draw(pDC, CPoint(0, 0), sizeFileDib);									   //显示DIB
	}
}


// OriginView 诊断

#ifdef _DEBUG
void OriginView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void OriginView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

//int OriginView::OnCreate(LPCREATESTRUCT lpcs)
//{
//
//	if (CView::OnCreate(lpcs) == -1)
//		return -1;
//
//	m_Button.Create(
//		"原图", // 按钮的标题 相当于属性里面的Caption
//		WS_VISIBLE | WS_CHILD | WS_BORDER, // 按钮的风格
//		CRect(40, 10, 200, 60), // 按钮所在的位置和大小
//		this,//父窗口指针
//		0x1001//控件的ID
//	);
//	m_Button.ShowWindow(SW_SHOWNORMAL);//显示出来
//
//	return 0;
//}

#endif

CMSMoneyDemoDoc* OriginView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMSMoneyDemoDoc)));
	return (CMSMoneyDemoDoc*)m_pDocument;
}

#endif //_DEBUG

