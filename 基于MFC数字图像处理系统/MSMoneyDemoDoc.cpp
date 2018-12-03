// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// MSMoneyDemoDoc.cpp : implementation of the CMSMoneyDemoDoc class
//

#include "stdafx.h"
#include "MSMoneyDemo.h"

#include "MSMoneyDemoDoc.h"
#include "MainFrm.h"
#include "MSMoneyDemoView.h"
#include "OriginView.h"
#include "OutputView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoDoc

IMPLEMENT_DYNCREATE(CMSMoneyDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CMSMoneyDemoDoc, CDocument)
	ON_COMMAND(ID_GO_BACK, OnGoBack)
	ON_UPDATE_COMMAND_UI(ID_GO_BACK, OnUpdateGoBack)
	ON_COMMAND(ID_GO_FORWARD, OnGoForward)
	ON_UPDATE_COMMAND_UI(ID_GO_FORWARD, OnUpdateGoForward)
	ON_COMMAND(ID_GO_ADDRESS, OnGoAddress)
	ON_UPDATE_COMMAND_UI(ID_GO_ADDRESS, OnUpdateGoAddress)
	ON_COMMAND(ID_VIEW_STOP, OnViewStop)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STOP, OnUpdateViewStop)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REFRESH, OnUpdateViewRefresh)
	ON_COMMAND(ID_LINKS_EXIT, &CMSMoneyDemoDoc::OnLinksExit)
	ON_COMMAND(ID_File_OPEN, &CMSMoneyDemoDoc::OnLinksOpen)
	ON_COMMAND(ID_LINKS_SAVE, &CMSMoneyDemoDoc::OnLinksSave)

	ON_COMMAND(id_AVIOpen, &CMSMoneyDemoDoc::OnAVIOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoDoc construction/destruction

CMSMoneyDemoDoc::CMSMoneyDemoDoc()
{
	m_dib = new ImageDib();
	m_dib_Result = new ImageDib();
	m_dib_SoleProc = new ImageDib();
	m_dib_Temp = new ImageDib();

	m_avi = NULL;
	isAvi = false;
}

CMSMoneyDemoDoc::~CMSMoneyDemoDoc()
{
	clearUp();
}

void CMSMoneyDemoDoc::clearUp()
{
	m_dib->Empty();
	m_dib_Result->Empty();
	m_dib_SoleProc->Empty();
	m_dib_Temp->Empty();
	if (m_avi != NULL) {
		delete m_avi;
		m_avi = NULL;
	}

}


BOOL CMSMoneyDemoDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	clearUp();
	//  读取bmp格式图像
	if (m_dib->Read(lpszPathName) == TRUE && m_dib_Result->Read(lpszPathName) == TRUE && m_dib_SoleProc->Read(lpszPathName) == TRUE)
	{
		SetModifiedFlag(FALSE);

		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
		pView->state2 = 1; // 关闭视频显示模式，开启图像显示模式
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();

		return TRUE;
	}
	else
		return 0;
}

BOOL CMSMoneyDemoDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_dib_Result->Write(lpszPathName) == TRUE) {
		SetModifiedFlag(FALSE);     // have saved
		return TRUE;
	}
	else
		return 0;

	// return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CMSMoneyDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle("数字图像处理系统");           //设置文档标题  
	return TRUE;
}


void CMSMoneyDemoDoc::OnLinksExit()
{
	// TODO: 在此添加命令处理程序代码
	exit(0);
}

void  CMSMoneyDemoDoc::OnAVIOpen()
{
	
	CString filename;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), _T("*.avi"), OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		clearUp();

		filename = opendlg.GetPathName();
		m_avi = new AVIHandle(filename);	// 打开AVI文件
		isAvi = true;
		Avidata = m_avi->GetSingleFrame(0);	// 获得第一帧图像
		SetModifiedFlag(FALSE);     // start off with unmodified

		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
		pView->state2 = 2; // 开启视频显示模式，关闭图像显示模式
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();
	}
}

void CMSMoneyDemoDoc::OnLinksOpen()
{
	// TODO: 在此添加命令处理程序代码
	
	CString filename;//保存路径
	CFileDialog opendlg(TRUE, _T("*"), _T("*.bmp"), OFN_OVERWRITEPROMPT, _T("所有文件(*.*;)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK)
	{
		clearUp();

		filename = opendlg.GetPathName();
		//  读取bmp格式图像
		if (m_dib->Read(filename) == TRUE && m_dib_Result->Read(filename) == TRUE && m_dib_SoleProc->Read(filename) == TRUE)
			SetModifiedFlag(FALSE);


		// 强制刷新所有视图
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
		pView->state2 = 1; //关闭视频显示模式，开启图像显示模式
		pFrame->InvalidateRect(NULL, FALSE);
		pFrame->UpdateWindow();

		//CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		//CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->GetActiveView();
		//UpdateAllViews(pView);
	}
}


void CMSMoneyDemoDoc::OnLinksSave()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(FALSE, _T("*.BMP"), NULL, NULL, _T("位图文件(*.BMP)|*.bmp;*.BMP|"));		// FALSE表示为“另存为”对话框，否则为“打开”对话框

	if (dlg.DoModal() == IDOK)
	{
		CString lpszPathName = dlg.GetPathName();		// 获取完整路径
		if (m_dib_Result->Write(lpszPathName) == TRUE)
			SetModifiedFlag(FALSE);     // have saved
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoDoc serialization

void CMSMoneyDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoDoc diagnostics

#ifdef _DEBUG
void CMSMoneyDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMSMoneyDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMSMoneyDemoDoc commands

void CMSMoneyDemoDoc::OnGoBack() 
{
}

void CMSMoneyDemoDoc::OnUpdateGoBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (IsBackAvailable ());
}

void CMSMoneyDemoDoc::OnGoForward() 
{
}

void CMSMoneyDemoDoc::OnUpdateGoForward(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (IsFrwdAvailable ());
}

void CMSMoneyDemoDoc::OnGoAddress() 
{

}

void CMSMoneyDemoDoc::OnUpdateGoAddress(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (TRUE);
}

void CMSMoneyDemoDoc::OnViewStop() 
{

	clearUp();

	// 强制刷新所有视图
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CMSMoneyDemoView* pView = (CMSMoneyDemoView*)pFrame->m_SoleProcView;
	pView->state2 = 0; //关闭所有显示模式
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoDoc::OnUpdateViewStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (TRUE);
}

void CMSMoneyDemoDoc::OnViewRefresh() 
{
	// 强制刷新所有视图
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->InvalidateRect(NULL, FALSE);
	pFrame->UpdateWindow();
}

void CMSMoneyDemoDoc::OnUpdateViewRefresh(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (TRUE);
}


void CMSMoneyDemoDoc::SetTitle(LPCTSTR lpszTitle)
{
	// TODO: 在此添加专用代码和/或调用基类
	CString str = "数字图像处理系统";
	CDocument::SetTitle(str);
}


