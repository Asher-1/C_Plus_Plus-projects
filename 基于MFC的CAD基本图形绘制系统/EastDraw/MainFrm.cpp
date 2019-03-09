// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EastDraw.h"
#include "MainFrm.h"
#include "Splash.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(Menu_ShowPorDlg, OnShowPorDlg)
	ON_UPDATE_COMMAND_UI(Menu_ShowPorDlg, OnUpdateShowPorDlg)
	ON_COMMAND(ID_ShowDrawTool_Bar, OnShowDrawToolBar)
	ON_UPDATE_COMMAND_UI(ID_ShowDrawTool_Bar, OnUpdateShowDrawToolBar)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    
	
	ON_UPDATE_COMMAND_UI(ID_MousPoint,OnUpdateMousPoint)
	ON_UPDATE_COMMAND_UI(ID_MousCaption,OnUpdateMousCaption)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR, // status line indicator
    ID_MousCaption,
	ID_MousPoint,
    ID_SEPARATOR,
	ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);
	
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
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
	if (!m_wndDrawToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDrawToolBar.LoadToolBar(IDR_TOOLBAR1_DrawTool))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	

	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
    m_wndDrawToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDrawToolBar);

   
	//m_DLineProperty->MoveWindow(500,500,340,240);
    
    
   if (!m_wndStyleBar.Create (this, WS_CHILD| CBRS_ALIGN_TOP|
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, IDW_STYLE_BAR))
        return FALSE;

    m_wndStyleBar.SetWindowText (_T ("文字工具条"));
    m_wndStyleBar.EnableDocking (CBRS_ALIGN_ANY);
    DockControlBar (&m_wndStyleBar);
	FloatControlBar(&m_wndStyleBar,CPoint(300,10));

   
	m_DLineProperty=new CDLineProperty(_T("图元属性页"));
    m_DLineProperty->Create(this,WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME,WS_EX_TOOLWINDOW);
    
    SetTimer(1, 1000, NULL);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.cy = ::GetSystemMetrics(SM_CYSCREEN); 
    cs.cx = ::GetSystemMetrics(SM_CXSCREEN); 
    cs.y = ::GetSystemMetrics(SM_CYSCREEN); 
    cs.x = -::GetSystemMetrics(SM_CYSCREEN);
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
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
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers




void CMainFrame::OnShowPorDlg() 
{
	// TODO: Add your command handler code here
if(m_DLineProperty)
  m_DLineProperty->ShowWindow(!m_DLineProperty->IsWindowVisible());
	
}

void CMainFrame::OnUpdateShowPorDlg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_DLineProperty)
	pCmdUI->SetCheck(m_DLineProperty->IsWindowVisible());

}

void CMainFrame::ShowFontBar(BOOL isShow)
{
        
	  if((!(!isShow&&!m_wndStyleBar.IsWindowVisible()))&&!(isShow&&m_wndStyleBar.IsWindowVisible()))
		OnBarCheck(IDW_STYLE_BAR);
}


void CMainFrame::OnUpdateMousPoint(CCmdUI* pCmdUI)
{
pCmdUI->Enable();	
}

void CMainFrame::OnUpdateMousCaption(CCmdUI* pCmdUI)
{
pCmdUI->Enable();
}



void CMainFrame::SetMousePosText(CPoint Logpoint,CPoint Devpoint)
{

	CString strText;
	strText.Format("逻辑坐标:(%d,%d)",Logpoint.x,-Logpoint.y);
	int nIndex=m_wndStatusBar.CommandToIndex(ID_MousPoint);
	m_wndStatusBar.SetPaneText(nIndex,strText);
	
	CWindowDC dc(&m_wndStatusBar);
	CSize sizeText=dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,ID_MousPoint,SBPS_NORMAL,sizeText.cx);

    strText.Format("屏幕坐标:(%d,%d)",Devpoint.x,Devpoint.y);
	nIndex=m_wndStatusBar.CommandToIndex(ID_MousCaption);
	m_wndStatusBar.SetPaneText(nIndex,strText);
	
    sizeText=dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,ID_MousCaption,SBPS_NORMAL,sizeText.cx);




}






void CMainFrame::OnShowDrawToolBar() 
{
	// TODO: Add your command handler code here
	ShowControlBar(&m_wndDrawToolBar,!m_wndDrawToolBar.IsWindowVisible(),false);
}

void CMainFrame::OnUpdateShowDrawToolBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->SetCheck(m_wndDrawToolBar.IsWindowVisible()?1:0);
}





void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static BOOL bShow=false;
	if(!bShow)
	{  
	     int cscy = ::GetSystemMetrics(SM_CYSCREEN) / 4*3; 
         int cscx = ::GetSystemMetrics(SM_CXSCREEN) / 4*3; 
         int csy = cscy/6; 
         int csx = cscx/6;
         CRect rect;
	     
		 this->MoveWindow(csx,csy,cscx,cscy);
         this->ShowWindow(SW_SHOWMAXIMIZED);
         this->m_DLineProperty->GetWindowRect(&rect);
         this->m_DLineProperty->MoveWindow(cscx*4/3-rect.Width(),0,rect.Width(),rect.Height());
	     this->m_DLineProperty->ShowWindow(SW_SHOW);
	}
	bShow=true;
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::AddTempFile()
{
   
}
