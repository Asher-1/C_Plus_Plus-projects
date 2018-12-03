// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// MSMoneyDemoView.h : interface of the CMSMoneyDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "imageTools\ImageDib.h"
#include "MSMoneyDemoDoc.h"



class CMSMoneyDemoView : public CView
{
protected: // create from serialization only
	CMSMoneyDemoView();
	DECLARE_DYNCREATE(CMSMoneyDemoView)

// Attributes
public:
	int state2; //  视频和图像显示模式切换


protected:
	CBitmap	m_Image;
	CSize	m_sizeImage;

// Operations
public:
	CMSMoneyDemoDoc * GetDocument();
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMSMoneyDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	afx_msg void OnMove();
	afx_msg void OnHorizontalMirror();
	afx_msg void OnVerticalMirror();
	afx_msg void OnZoom();
	afx_msg void OnClockwise90();
	afx_msg void OnAnticlockwise90();
	afx_msg void OnRotate180();
	afx_msg void OnFreeRotate();
	afx_msg void OnBinary();
	afx_msg void OnHistogramDraw();
	afx_msg void OnHistgramAver();
	afx_msg void OnImageReverse();
	afx_msg void OnImgThresh();
	afx_msg void OnThresholdWindow();
	afx_msg void OnLinearStrecth();
	afx_msg void OnPepperSaltNoise();
	afx_msg void OnGaussNoise();
	afx_msg void OnChooseMaskSmooth();
	afx_msg void OnGradeSharp();
	afx_msg void OnNeiAveSmooth();
	afx_msg void OnMedianSmooth();
	afx_msg void OnLaplaceSharp();
	afx_msg void OnQuickFFt();
	afx_msg void OnQuickFFtReverse();
	afx_msg void OnIdealLowPass();
	afx_msg void OnButterLowPass();
	afx_msg void OnIdealHighPass();
	afx_msg void OnButterHighPass();
	afx_msg void OnHarrWaveletTrans();
	afx_msg void OnHarrWavRevTrans();
	afx_msg void OnErosion();
	afx_msg void OnDilation();
	afx_msg void OnOpen();
	afx_msg void OnClose();
	afx_msg void OnThinning();
	afx_msg void OnRobert();
	afx_msg void OnSobel();
	afx_msg void OnPrewitt();
	afx_msg void OnLaplacian();
	afx_msg void OnRegionGrow();
	afx_msg void OnEdgeTrace();
	afx_msg void OnAdaptThresholdSeg();
	afx_msg void OnHistThreshSeg();

	afx_msg void OnSimilardegree();
	afx_msg void Onboundary();
	afx_msg void Onfacelocate();
	afx_msg void Oninnerboundary();
	afx_msg void Oneyelocate();
	afx_msg void Onnoselocate();
	afx_msg void Onmouthlocate();


	afx_msg void OnColorToGray();
	afx_msg void OnBinary2();
	afx_msg void OnColorPair();
	afx_msg void Onplatelocate();
	afx_msg void Onplaterelocate();
	afx_msg void Oncharseparate();
	afx_msg void Oncharrecognition();

	afx_msg void OnPaperBinary();
	afx_msg void Onlocation();
	afx_msg void Onrecognition();

	afx_msg void OnObjectDetect();
	afx_msg void OnObjectTrace();
	

	DECLARE_MESSAGE_MAP()
public:
	
	CPoint m_LeftEye;
	CPoint m_LeftEyeLeftCorner;
	CPoint m_LeftEyeRightCorner;
	CPoint m_RightEye;
	CPoint m_RightEyeLeftCorner;
	CPoint m_RightEyeRightCorner;
	CPoint m_LeftNostril;
	CPoint m_RightNostril;
	CPoint m_LeftMouthCorner;
	CPoint m_RightMouthCorner;
	CPoint m_MidMouth;
	CPoint m_MidNose;

	int left;
	int right;
	int top;
	int bottom;


	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnPaint();

	
};

#ifndef _DEBUG  // debug version in MSMoneyDemoView.cpp
inline CMSMoneyDemoDoc* CMSMoneyDemoView::GetDocument()
{
	return (CMSMoneyDemoDoc*)m_pDocument;
}
#endif
