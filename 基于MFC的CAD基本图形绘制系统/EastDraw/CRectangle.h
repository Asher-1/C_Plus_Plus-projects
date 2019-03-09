#include"CUnit.h"
#ifndef CRECTANGLE
#define CRECTANGLE
class CRectangle:public CUnit
{
public:
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	CRectangle();
	virtual void Serialize(CArchive &ar);
    
virtual void  DrawStatic(CDC*pDC);
DECLARE_SERIAL(CRectangle)
};
#endif