#include"CUnit.h"
#ifndef CCURVE
#define CCURVE
class CCurve :public CUnit
{
  
public:
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void OnMouseMove(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	CRect GetBoundingRect();
	virtual void ShowSelectPoint(CDC*pDC);
	virtual BOOL IsInRgn(CPoint point);
	virtual void DrawMask(CDC*pDC,CPoint first,CPoint second);

	CCurve();
	virtual void Serialize(CArchive&ar);
	virtual void Initial();
	virtual int  IsOnMarginPoint(CPoint point);
	
	void DrawStatic(CDC*pDC);
    DECLARE_SERIAL(CCurve)
 };
#endif