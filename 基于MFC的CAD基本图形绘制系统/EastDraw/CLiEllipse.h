#ifndef CLIELLIPSE
#define CLIELLIPSE
#include"CUnit.h"
class CLiEllipse:public CUnit
{
public:
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	CRoundRectangle();
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	virtual void Serialize(CArchive &ar);
    
virtual void  DrawStatic(CDC*pDC);
DECLARE_SERIAL(CLiEllipse)

};
#endif