#include"CUnit.h"
#ifndef CDLINE
#define CDLINE
class CDLine:public CUnit
{
public:
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual void Circumrotate(CPoint first,CPoint second);
	virtual int IsOnMarginPoint(CPoint point);
	 virtual void Initial();
	virtual CRgn* GetRgn();
	CDLine();
 virtual void DrawStatic(CDC*pDC);
 DECLARE_SERIAL(CDLine)
};
#endif