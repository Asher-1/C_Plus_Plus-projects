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
		//p_MyRadio[m_Model]->SetCheck(1); //设置第一个单选为选中状态
		// 工具提示 
		p_MyRadio[0]->SetTooltip(_T("以原型模式显示"));
		p_MyRadio[1]->SetTooltip(_T("以三角面网格模式显示"));
		p_MyRadio[2]->SetTooltip(_T("以四面体网格模式显示"));
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

	if(!m_wndDisplayBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDisplayBar.LoadToolBar(IDR_TOOLBAR_DISPLAY))
	{
		TRACE0("Failed to create display toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	// 设置 m_wndToolBar可以停靠在窗口内的任意位置上
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	// 设置m_wndDisplayBar可以停靠在窗口内的任意位置上
	m_wndDisplayBar.EnableDocking(CBRS_ALIGN_ANY);

	// 给工具栏窗口命名
	m_wndToolBar.SetWindowText(_T("Standard Tool"));
	m_wndDisplayBar.SetWindowText(_T("Display Tool"));

	// 允许工具栏在窗口内的任意位置停靠
	EnableDocking(CBRS_ALIGN_ANY);

	// 停靠工具栏m_wndToolBar于默认位置
	DockControlBar(&m_wndToolBar);

	// 将工具栏m_wndDisplayBar与m_wndToolBar停靠于一行，并位于其右
	DockControlBarLeftOf(&m_wndDisplayBar,&m_wndToolBar);

	//------------------------------------------------------------
	//create LeftDockBar 创建左边的浮动窗口
	if (!m_LeftDockBar.Create(this,_T("Work Bar")))
	{
		TRACE0("Failed to create LeftDockBar\n");
		return -1;		// fail to create
	}

	m_OutputDockBar.SetDockSize(CSize(200,100));
	if(!m_OutputDockBar.Create(this,_T("Output Bar")))
	{
		TRACE0("Failed to create m_OutputDockBar\n");
		return -1;
	}

	// 只允许控制条m_LeftDockBar使用左右两个停靠栏
	m_LeftDockBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);

	// 使用左边的停靠栏来停靠m_LeftDockBar
	DockControlBar(&m_LeftDockBar,AFX_IDW_DOCKBAR_LEFT);

	// 只允许将控制条m_OutputDockBar停靠在上下两个停靠栏
	m_OutputDockBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);

	// 使用下面的停靠栏来停靠m_OutputDockBar
	DockControlBar(&m_OutputDockBar,AFX_IDW_DOCKBAR_BOTTOM);

	//---------------------------------------------------------------

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
void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect; // 定义矩形区域
	DWORD dw;
	UINT n;

	// 使用MFC来重新计算所有工具栏的尺寸，确保GetWindowRect()准确
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);

	// 设置偏移值以停靠在同一条边上
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

	// 将工具栏停靠在rect规定的矩形区域内
	DockControlBar(Bar,n,&rect);
}
