#ifndef CROUNDARC
#define CROUNDARC
#include"CUnit.h"
class CRoundArc:public CUnit
{
	DECLARE_SERIAL(CRoundArc)

public:
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual BOOL IsInRgn(CPoint point);
	virtual void Serialize(CArchive &ar);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual double ComputeSloap2(CPoint firstpoint,CPoint secondpoint);
	virtual double ComputeSloap1(CPoint firstpoint,CPoint secondpoint);
	virtual void DrawStatic(CDC*pDC);
	CRoundArc();
};
#endif