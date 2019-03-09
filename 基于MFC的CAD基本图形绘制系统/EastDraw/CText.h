#ifndef	CTEXT
#define CTEXT
#include"CUnit.h"
class CTextEdit;	
class CText :public CUnit
{
DECLARE_SERIAL(CText)

public:
	virtual void ShowSelectPoint(CDC*pDC);
	virtual int IsOnMarginPoint(CPoint point);
	virtual BOOL IsInRgn(CPoint point);
	void OnMenuitemOk(CDC *pDC, CEastDrawView *pView);
	virtual void OnMouseMove(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void DrawStatic(CDC*pDC);
	virtual void ChangeFontSize(ENUMLOGFONT *lpelf);
	virtual void ChangeFont(ENUMLOGFONT *lpelf);
	virtual void SetFont(ENUMLOGFONT *lpelf);
	
	void SetTextEditPosition(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void OnLButtonDown(CDC*pDC,CEastDrawView*pView,CPoint point);
	virtual void Serialize(CArchive &ar);
	CText();
	~CText();
	CString m_String;
	BOOL m_HaveFinish;
	BOOL m_bHaveCreat;
	long m_Width;
	long m_Hight;
	CPoint m_PositionPoint;


};
#endif