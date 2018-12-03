#pragma once


// CMySplitterWndEx

class CMySplitterWndEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitterWndEx)

public:
	CMySplitterWndEx();
	virtual ~CMySplitterWndEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


