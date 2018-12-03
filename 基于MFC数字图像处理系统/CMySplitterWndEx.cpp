// CMySplitterWndEx.cpp: 实现文件
//

#include "stdafx.h"
#include "MSMoneyDemo.h"
#include "CMySplitterWndEx.h"


// CMySplitterWndEx

IMPLEMENT_DYNAMIC(CMySplitterWndEx, CWnd)

CMySplitterWndEx::CMySplitterWndEx()
{
	// 对分割条的Size进行初始化调整
	m_cxSplitter = m_cySplitter = 6;         // size of splitter bar
	m_cxBorderShare = m_cyBorderShare = 0;   // space on either side of splitter
	m_cxSplitterGap = m_cySplitterGap = 6;   // amount of space between panes//间隙
	m_cxBorder = m_cyBorder = 0;             // borders in client area  外框 
}

CMySplitterWndEx::~CMySplitterWndEx()
{
}


BEGIN_MESSAGE_MAP(CMySplitterWndEx, CWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CMySplitterWndEx 消息处理程序




void CMySplitterWndEx::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	// TODO: 在此添加专用代码和/或调用基类
	CSplitterWnd::OnDrawSplitter(pDC, nType, rect);

	//蓝色填充
	if (nType == splitBar && pDC != NULL)//判断画分割条时
	{
		CRect rcD = rect, rcL = rect;

		//根据横纵向矩阵大小关系,将分割分为两部分刷两种颜色,

		//rcD 深色.rcL  浅色
		if (rect.right - rect.left < 10)//纵向
		{
			rcL.right = (rcL.left + rcL.right) / 2;//
			rcD.left = rcL.right;
		}
		else//横向
		{
			rcL.bottom = (rcL.top + rcL.bottom) / 2;
			rcD.top = rcL.bottom;
		}
		CBrush brushD(RGB(0, 0, 230));//深色画刷
		CBrush brushL(RGB(60, 150, 255)); // 浅色画刷

			//用指定画刷填充对应矩形区域
			pDC->FillRect(&rcD, &brushD);
		pDC->FillRect(&rcL, &brushL);
	}
}


BOOL CMySplitterWndEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//不刷新背景
	return TRUE;
	//return CSplitterWnd::OnEraseBkgnd(pDC);
}
