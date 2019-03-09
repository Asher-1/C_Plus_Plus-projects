// EastDrawDoc.h : interface of the CEastDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EASTDRAWDOC_H__B33FB26B_D8F0_445D_AD69_61266F720880__INCLUDED_)
#define AFX_EASTDRAWDOC_H__B33FB26B_D8F0_445D_AD69_61266F720880__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CEastDrawDoc : public CDocument
{
protected: // create from serialization only
	CEastDrawDoc();
	DECLARE_DYNCREATE(CEastDrawDoc)

// Attributes
public:
    CObArray m_DLineArray;
	CObArray m_CurveArray;
	CObArray m_RectangleArray;
	CObArray m_RoundRectangleArray;
	CObArray m_EllipseArray;
	CObArray m_RoundArray;
	CObArray m_PolygonArray;
	CObArray m_LiEllipseArray;
	CObArray m_RoundArcArray;
	CObArray m_EllipseArcArray;
	CObArray m_PolyBezierArray;
	CObArray m_TextArray;
	BOOL m_bChangeTitle;
	CSize m_TotalSize;
	CPoint m_CenterPoint;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEastDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void SetTitle(LPCTSTR lpszTitle);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Requeue(int &index,int &rtimes);
	void ReMoveTemp();
	void OnBUTTON32858OnDo(int index);
	void ExchangeSave(int index,int&rtimes);
	void AutoSave();
	void AddTempFile();
	BOOL SaveBitMap(CFile&file);
	virtual ~CEastDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEastDrawDoc)
	afx_msg void OnFILESAVEASBmp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASTDRAWDOC_H__B33FB26B_D8F0_445D_AD69_61266F720880__INCLUDED_)
