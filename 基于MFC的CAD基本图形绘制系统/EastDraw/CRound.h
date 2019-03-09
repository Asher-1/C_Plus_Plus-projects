#ifndef CROUND
#define CROUND
#include"CUnit.h"
class CRound:public CUnit
{
 

  DECLARE_SERIAL(CRound)
public:
	void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual void ShowSelectPoint(CDC *pDC);
	virtual void Serialize(CArchive &ar);
	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual int ComputRadious(CPoint centerPoint, CPoint movingPoint);
	virtual void  DrawStatic(CDC*pDC);
	CRound();
	
};
#endif