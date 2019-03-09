#ifndef CELLIPSE
#define CELLIPSE
#include"CUnit.h"
#define ROUND(a) ((int)(a+0.5))
class CEllipse:public CUnit
{
public:
	virtual void OnContextMenu(CWnd *pWnd, CPoint point);
	CEllipse();
	
	virtual void OnMenuitemCirCu(CDC *pDC, CEastDrawView *pView);
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC *pDC, CEastDrawView *pView, CPoint point);
	virtual void DrawActive(CDC *pDC,CPoint point);
	virtual CRect GetBoundingRect();
	virtual void ShowSelectPoint(CDC*pDC);
	void ComputeOffSet();
	virtual void ShowMovingLine(CDC*pDC,CPoint firstPoint, CPoint secondPoint);
	virtual int ComputRadiusY(CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusX(CPoint firstPoint,CPoint secondPoint);
	virtual void PdcSetPixel(CDC*pDC,int xCenter,int yCenter,COLORREF m_PenColor);
	
	
	virtual float ComputSloap(CPoint firstPoint, CPoint secondPoint);
	void ellipsePlotPoints(CDC*pDC,int xCenter, int yCenter, int x, int y);
	virtual void ellipseMidpoint(CDC*pDC,int xCenter,int yCenter,int Rx,int Ry);
	virtual BOOL IsInRgn(CPoint point);
	virtual int IsOnMarginPoint(CPoint point);
	virtual void Serialize(CArchive &ar);
    CPoint  m_MarginPoint[5];
	int m_OffSetX;
	int m_OffSetY;
	BOOL m_IsCirCu;
	
virtual void  DrawStatic(CDC*pDC);
DECLARE_SERIAL(CEllipse)

};
#endif