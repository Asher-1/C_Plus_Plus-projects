#ifndef CPOLYGON
#define CPOLYGON
#include"CUnit.h"
class CPolygon:public CUnit
{
 DECLARE_SERIAL(CPolygon)
 
 
 public:
	 virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	 virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	 virtual void DrawActive(CDC *pDC,CPoint point);
	 virtual void ShowSelectPoint(CDC *pDC);
	 
	 virtual void ExchangeDraw(CDC *pDC, CPoint point);
	 virtual BOOL IsInRgn(CPoint point);
	 virtual int IsOnMarginPoint(CPoint point);
	 virtual void DrawEnd(CDC*pDC,CPoint point);
	 CPolygon();
	 virtual void DrawMask(CDC*pDC,CPoint point);
	 virtual void DrawStatic(CDC*pDC);
	 virtual void Initial();
	void Serialize(CArchive &ar);
};
#endif