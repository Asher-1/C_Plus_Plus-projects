// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "STLViewer.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_MYRADIO1, OnMyRadio1) //单选按钮1
	ON_BN_CLICKED(IDC_MYRADIO2, OnMyRadio2) //单选按钮2
	ON_BN_CLICKED(IDC_MYRADIO3, OnMyRadio3) //单选按钮3

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

int CMainFrame::m_Model = 0; // 默认为绘制模式0，即绘制原型

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
	for (int i = 0; i < 3; i++)
	{
		if (p_MyRadio[i])
			delete p_MyRadio[i];
	}
}


CMFCButton* CMainFrame::NewMyRadio(int nID, CString titile, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //取按钮标题
	CMFCButton *p_Radio = new CMFCButton();
	ASSERT_VALID(p_Radio);
	p_Radio->Create(m_Caption, WS_CHILD | WS_VISIBLE | nStyle | WS_TABSTOP | BS_AUTORADIOBUTTON, rect, this, nID); //创建按钮
																												   // 更改文字 
	p_Radio->SetWindowText(titile);
	//p_Radio->m_bTransparent = TRUE;

	// 更改背景颜色 
	p_Radio->SetFaceColor(RGB(153, 217, 234));

	// 鼠标经过时变成手势 
	p_Radio->SetMouseCursorHand();
	// 更改高亮颜色 
	p_Radio->SetTextHotColor(RGB(63, 72, 204));

	p_Radio->m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;

	p_Radio->SetCheck(TRUE);
	//p_Radio->SizeToContent();
	return p_Radio;
}

//单击单选按钮1
void CMainFrame::OnMyRadio1()
{
	m_Model = 0;
	//Redraw the view
	InvalidateRect(NULL, FALSE);
}
//单击单选按钮2
void CMainFrame::OnMyRadio2()
{
	m_Model = 1;
	//Redraw the view
	InvalidateRect(NULL, FALSE);
}
//单击单选按钮3 
void CMainFrame::OnMyRadio3()
{
	m_Model = 2;

	//Redraw the view
	InvalidateRect(NULL, FALSE);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	// 创建模式选择按钮
	{
		// 创建模式选择按钮
		p_MyRadio[0] = NewMyRadio(IDC_MYRADIO1, _T("原型"), CRect(500, 10, 600, 40), WS_GROUP);
		p_MyRadio[1] = NewMyRadio(IDC_MYRADIO2, _T("三角面网格"), CRect(600, 10, 700, 40), 0);
		p_MyRadio[2] = NewMyRadio(IDC_MYRADIO3, _T("四面体网格"), CRect(700, 10, 800, 40), 0);
		p_MyRadio[m_Model]->SetCheck(1); //设置第一个单选为选中状态
		// 工具提示 
		p_MyRadio[0]->SetTooltip(_T("以原型模式显示"));
		p_MyRadio[1]->SetTooltip(_T("以三角面网格模式显示"));
		p_MyRadio[2]->SetTooltip(_T("以四面体网格模式显示"));
	}


	// 创建显示工具条m_wndDisplayBar
	if (!m_wndDisplayBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDisplayBar.LoadToolBar(IDR_TOOLBAR_DISPLAY))
	{
		TRACE0("Failed to create display toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	// 设置m_wndDisplayBar的停靠属性
	m_wndDisplayBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar); // 工具栏控制函数

	// 将m_wndDisplayBar停靠在框架窗口的左边
	DockControlBar(&m_wndDisplayBar,AFX_IDW_DOCKBAR_LEFT);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

