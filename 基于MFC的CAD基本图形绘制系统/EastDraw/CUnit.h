# ifndef CUNIT
# define CUNIT

#define DLine_Tool          10
#define Curve_Tool          11
#define Rectangle_Tool      12
#define RoundRectangle_Tool 13
#define Ellipse_Tool        14
#define Round_Tool          15
#define Polygon_Tool        16
#define LiEllipse_Tool      17
#define RoundArc_Tool       18
#define EllipseArc_Tool     19
#define PolyBezier_Tool     20
#define Text_Tool           21

#define Draw_Status          1
#define Drag_Status          2
#define Select_Status        3
#define Select_Color_Status  4
#define Change_Status        5
#define Circumrotate_Status  6
#include <afxtempl.h>
#include <math.h>
#include"resource.h"
class CEastDrawView;
class CTextEdit;
class CUnit:public CObject
{

public:
    DECLARE_SERIAL(CUnit)


	UINT m_PenStyle;
	int m_PenWidth;
	int m_BkMode;
	int m_DrawingMode;
	COLORREF m_PenColor;
	COLORREF m_BrushColor;
	COLORREF m_BackgroundColor;
	CPoint m_FirstPoint;
	CPoint m_SecondPoint;
	CPoint m_ThirdPoint;
	CPoint m_CenterPoint;
	CPoint m_FourthPoint;
	CPoint m_FifthPoint;
	CPoint m_MovingPoint;
	CPoint m_MinPoint;
	CPoint m_MaxPoint;
	CPoint m_IntersectionPointFirst;
	CPoint m_IntersectionPointSecond;
	CRect  m_BoundingRect;
	CRgn   *m_Rgn;
    CArray<CPoint,CPoint>*m_PointList;
	CArray<CPoint,CPoint>m_PointArray;
    UINT m_BrushStyle;
	LONG m_BrushHatch;
	int  m_FoundIndex;
	CPoint m_FoundPoint;
    int m_Radious;
	int    m_Xr;
	int    m_Yr;
	BOOL   m_HaveLBUp;
	double m_Cos;
	double m_Sin;
	double m_Sloap1;
	double m_Sloap2;
	double m_AngleFirst;
	double m_AngleSecond;
	BOOL  m_HaveFindFirst;
	CTextEdit* m_pTextEdit;
	CFont *m_Font;
	int m_DrawStatus;
	

public:
	void SetBrushOrg(CDC*pDC,CBrush*brush);
	virtual void OnMenuitemCirCu(CDC*pDC,CEastDrawView*pView);
	void OnMENUITEMSize();
	virtual void OnMenuitemOk(CDC *pDC, CEastDrawView *pView);
	virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	virtual void ChangeFontSize(ENUMLOGFONT*lpelf);
	virtual void ChangeFont(ENUMLOGFONT *lpelf);
	virtual void SetFont(ENUMLOGFONT* lpelf);
	virtual void OnLButtonDblClk(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual CPoint ComputeIntersectionPointSecond(CPoint point);
	virtual CPoint ComputeIntersectionPointFirst(CPoint point);
	virtual void DrawStaticStepOne(CDC*pDC,CPoint movingPoint);
	virtual void DrawActiveStepOne(CDC*pDC,CPoint movingPoint);
	virtual void DrawActive(CDC*pDC,CPoint point);
	virtual double ComputeSloap2(CPoint firstpoint,CPoint secondpoint);
	virtual double ComputeSloap1(CPoint firstpoint,CPoint secondpoint);
    virtual	CRect GetBoundingRect();
	virtual void DrawOldReferencePoint(CDC*pDC,CPoint point);
	virtual void ShowSelectPoint(CDC*pDC);
	virtual void ShowMovingLine(CDC*pDC,CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusX(CPoint firstPoint,CPoint secondPoint);
	virtual int ComputRadiusY(CPoint firstPoint,CPoint secondPoint);
	virtual float ComputSloap(CPoint firstPoint,CPoint secondPoint);
	virtual void ellipseMidpoint(CDC*pDC,int xCenter,int yCenter,int Rx,int Ry);
	virtual int ComputRadious(CPoint centerPoint,CPoint movingPoint);
	virtual void ExchangeDraw(CDC*pDC,CPoint point);
	virtual void DrawEnd(CDC*pDC,CPoint point);
	virtual void DrawMask(CDC*pDC,CPoint point);
	virtual void DrawMask(CDC*pDC,CPoint first,CPoint second);
	virtual void OnLButtonDown(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void Circumrotate(CPoint first,CPoint second);
	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	virtual CRgn* GetRgn();
	virtual void Serialize(CArchive&ar);
	CUnit();
	
	
	CUnit(CUnit&unit);
    CUnit operator=(CUnit&unit);
    virtual void Initial();
	virtual void DrawStatic(CDC*pDC){};
	
};
#endif