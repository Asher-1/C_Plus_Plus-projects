#ifndef POLYBEZIER
#define POLYBEZIER
#include"CUnit.h"

class CPolyBezier:public CUnit
{
   DECLARE_SERIAL(CPolyBezier)
public:
	virtual void OnContextMenu(CWnd *pWnd, CPoint point);
	virtual void Serialize(CArchive& ar);
	virtual void OnMenuitemOk(CDC *pDC, CEastDrawView *pView);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	void DrawMaskPoint(CDC*pDC);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual void DrawStatic(CDC*pDC);
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	CPolyBezier();
	CPoint*m_PointList;
	
	BOOL m_HaveDrawFirst;
};
#endif