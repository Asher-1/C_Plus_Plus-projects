// GLPlotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GLPlot.h"
#include "GLPlotDlg.h"
#include "GLDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLPlotDlg dialog

CGLPlotDlg::CGLPlotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGLPlotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGLPlotDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	glDrawView_ = new CGLDrawView();
}

void CGLPlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGLPlotDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGLPlotDlg, CDialog)
	//{{AFX_MSG_MAP(CGLPlotDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLPlotDlg message handlers

BOOL CGLPlotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowPos(NULL,0,0,800,600,SWP_NOMOVE |SWP_NOZORDER);
	
    CRect viewRect;
    GetClientRect(viewRect);
    if (!((CView*)glDrawView_)->Create(NULL, NULL, WS_CHILD,
        viewRect, this, NULL, NULL))
    {
        AfxMessageBox(_T("Create view failed"));
        return TRUE;
    }
	
	glDrawView_->loadData();
    glDrawView_->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGLPlotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGLPlotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGLPlotDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (!IsWindowVisible())
    {
        return;                          
    }
    CRect   rect;  
    GetClientRect(rect); 
    glDrawView_->MoveWindow(rect);
}
