
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "FeatureReuse.h"

#include "MainFrm.h"
#include "FeatureReuseDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	// 将按钮消息响应函数和创建的按钮对象绑定在一起，并响应单击事件
	ON_BN_CLICKED(ID_MYBUT1, OnMybut1)
	ON_BN_CLICKED(ID_MYBUT2, OnMybut2)
	ON_BN_CLICKED(ID_MYBUT3, OnMybut3)

	ON_BN_CLICKED(ID_MYBUT4, OnMybut4)
	ON_BN_CLICKED(ID_MYBUT5, OnMybut5)
	ON_BN_CLICKED(ID_MYBUT6, OnMybut6)
	ON_BN_CLICKED(ID_MYBUT7, OnMybut7)

	ON_BN_CLICKED(IDC_COLOR, OnMyColorbut)

	ON_BN_CLICKED(IDC_MYRADIO1, OnMyRadio1) //单选按钮1
	ON_BN_CLICKED(IDC_MYRADIO2, OnMyRadio2) //单选按钮2
	ON_BN_CLICKED(IDC_MYRADIO3, OnMyRadio3) //单选按钮3
	ON_BN_CLICKED(IDC_MYRADIO4, OnMyRadio4) //单选按钮3

	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// 静态变量初始化
COLORREF CMainFrame::m_Color = RGB(128, 128, 256); // 初始化默认颜色
int  CMainFrame::m_Mode = 0;
bool  CMainFrame::is_PrintArrows = false;
bool  CMainFrame::is_Draw_cloudPicture = false;

int  CMainFrame::m_stage = 0;
int  CMainFrame::m_delta = 0;
int  CMainFrame::m_Counts = 1;

// CMainFrame 构造/析构
CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2003);

	m_Mode = 0; // 默认选择第一个按钮
	m_stage = 0; // 默认值显示第一阶段工件模型
	m_delta = 0;
	m_Counts = 1;
	is_PrintArrows = false;
	isChecked = true;
	is_Draw_cloudPicture = false;
	isChecked2 = true;
}

CMainFrame::~CMainFrame()
{
	for (int i = 0; i < size(p_MyBut); i++)  // 回收用户创建的按钮
	{
		if (p_MyBut[i])
			delete p_MyBut[i];
	}

	for (int i = 0; i < size(p_MyRadio); i++)
	{
		if (p_MyRadio[i])
			delete p_MyRadio[i];
	}

	if (m_ColorPicker != NULL)
	{
		delete m_ColorPicker;
		m_ColorPicker = NULL;
	}
}

// 工件变形显示按钮响应函数
void CMainFrame::OnMybut1() // 动态显示按钮
{
	//MessageBox(_T("哈！你单击了动态按钮1。"));
	m_Mode = 4;			// 设置绘制模式为动态连续显示
	m_delta = 0;			// 重置动态显示的阶段循环增量为0
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}

void CMainFrame::OnMybut2() // 单步显示按钮
{
	m_Mode = 3; // 设置绘制模式为分步绘制
	m_stage = 0; // 重置工件模型阶段
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}

void CMainFrame::OnMybut3() // 下一步显示按钮
{
	m_stage = m_stage + (CFeatureReuseDoc::steps) / 6; // 每次点击下一步按钮，工件模型刷新到下一个受力状态显示
	// 待修改代码
	if (m_stage > CFeatureReuseDoc::steps - 1)
	{
		m_stage = CFeatureReuseDoc::steps - 1;
	}
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}

// 绘制云图按钮
void CMainFrame::OnMybut4() 
{
	//MessageBox(_T("哈！你单击了动态按钮1。"));
	if (isChecked2)	 // 选上 
	{

		is_Draw_cloudPicture = true;
		isChecked2 = false;
	}
	else
	{
		is_Draw_cloudPicture = false;
		isChecked2 = true;
	}

	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}

void CMainFrame::OnMybut5() // 动态显示按钮
{
	MessageBox(_T("哈！你单击了动态按钮1。"));

}
void CMainFrame::OnMybut6() // 动态显示按钮
{
	//MessageBox(_T("哈！你单击了动态按钮1。"));

}
void CMainFrame::OnMybut7() // 动态显示按钮
{
	MessageBox(_T("哈！你单击了动态按钮1。"));
}


// 颜色选择按钮
void CMainFrame::OnMyColorbut()
{
	m_Color = m_ColorPicker->GetColor();
	if (m_Color == -1)
	{
		m_Color = m_ColorPicker->GetAutomaticColor();
	}
}

//单击原型绘制按钮1
void CMainFrame::OnMyRadio1()
{
	m_Mode = 0; // 原型绘制
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}
//单击绘制三角面按钮2
void CMainFrame::OnMyRadio2()
{
	m_Mode = 1; // 设置绘制模式为绘制三角面模式
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}
//单击绘制四面体按钮3 
void CMainFrame::OnMyRadio3()
{

	m_Mode = 2; // 设置绘制模式为绘制四面体模式
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
}
//单击绘制矢量箭头按钮4
void CMainFrame::OnMyRadio4()
{
	if (isChecked)	 // 选上 
	{

		is_PrintArrows = true;
		isChecked = false;
	}
	else
	{
		is_PrintArrows = false;
		isChecked = true;
	}
	
	InvalidateRect(NULL, FALSE);  // 强制刷新场景
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

	//字体大小
	CFont font;
	font.CreateFont(380, 0, 0, 0, FW_BOLD, TRUE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));
	p_Radio->SetFont(&font);

	// 鼠标经过时变成手势 
	p_Radio->SetMouseCursorHand();
	// 更改高亮颜色 
	p_Radio->SetTextHotColor(RGB(63, 72, 204));

	p_Radio->m_nFlatStyle = CMFCButton::BUTTONSTYLE_3D;

	p_Radio->SetCheck(TRUE);
	//p_Radio->SizeToContent();
	return p_Radio;
}

CMFCButton* CMainFrame::NewMyButton(int nID, CString titile, CRect rect, int nStyle)
{
	CString m_Caption;
	m_Caption.LoadString(nID); //取按钮标题
	CMFCButton *p_Button = new CMFCButton();
	ASSERT_VALID(p_Button);
	p_Button->Create(m_Caption, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | nStyle, rect, this, nID); //创建按钮
	// 更改文字 
	p_Button->SetWindowText(titile);

	CFont font;
	font.CreatePointFont(90, _T("黑体"));//字体可改成自己需要的
	p_Button->SetFont(&font);

	// 鼠标经过时变成手势 
	p_Button->SetMouseCursorHand();
	// 更改高亮颜色 
	p_Button->SetTextHotColor(RGB(63, 72, 204));
	//p_Button->SizeToContent();
	return p_Button;
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类
	DDX_Control(pDX, ID_MYBUT5, m_btn);

	CFrameWndEx::DoDataExchange(pDX);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);


	//CString m_Caption1;
	//m_Caption1.LoadString(ID_MYBUT5); //取按钮标题
	//CMFCButton *mp_Radio = new CMFCButton();
	//ASSERT_VALID(mp_Radio);
	//mp_Radio->Create(m_Caption1, WS_CHILD | WS_VISIBLE | 0 | WS_TABSTOP | BS_AUTORADIOBUTTON, CRect(1200, 60, 1300, 90), this, ID_MYBUT5); //创建按钮

	// //m_btn.SubclassDlgItem(ID_MYBUT5, this);
	//m_btn.SetIcon(IDI_ICON2);
	//m_btn.SetFlat(FALSE);

	// 创建自定义功能按钮
	{
	
		p_MyBut[0] = NewMyButton(ID_MYBUT1, _T("动态显示"), CRect(800, 50, 900, 90), BS_DEFPUSHBUTTON);
		p_MyBut[1] = NewMyButton(ID_MYBUT2, _T("单步显示"), CRect(900, 50, 1000, 90), 0);
		p_MyBut[2] = NewMyButton(ID_MYBUT3, _T("下一步"), CRect(1000, 50, 1100, 90), 0);
		p_MyBut[3] = NewMyButton(ID_MYBUT4, _T("云图"), CRect(1300, 50, 1400, 90), 0);
		//// 更改位置 
		//p_MyBut[2]->MoveWindow(500, 50, 100, 70);
		//p_MyBut[2]->SetImage(IDB_BITMAP1);
		// 更改背景颜色 
		p_MyBut[2]->SetFaceColor(RGB(153, 217, 234));
		// 更改字体颜色 
		p_MyBut[2]->SetTextColor(RGB(0, 0, 0));
		// 工具提示 
		p_MyBut[0]->SetTooltip(_T("显示工件受力变形的动态变化"));
		p_MyBut[1]->SetTooltip(_T("通过单击下一步按钮执行单步显示静态模型"));
		p_MyBut[2]->SetTooltip(_T("显示下一阶段工件受力模型"));
		p_MyBut[3]->SetTooltip(_T("工件应力应变云图显示"));
	}

	// 创建模式选择按钮
	{
		// 创建模式选择按钮
		p_MyRadio[0] = NewMyRadio(IDC_MYRADIO1, _T("原型"), CRect(500, 50, 600, 90), WS_GROUP);
		p_MyRadio[1] = NewMyRadio(IDC_MYRADIO2, _T("三角面网格"), CRect(600, 50, 700, 90), 0);
		p_MyRadio[2] = NewMyRadio(IDC_MYRADIO3, _T("四面体网格"), CRect(700, 50, 800, 90), 0);
		p_MyRadio[3] = NewMyRadio(IDC_MYRADIO4, _T("绘制矢量箭头"), CRect(1200, 50, 1300, 90), 0);
		//p_MyRadio[m_Mode]->SetCheck(1); //设置第一个单选为选中状态
		// 工具提示 
		p_MyRadio[0]->SetTooltip(_T("以原型模式显示"));
		p_MyRadio[1]->SetTooltip(_T("以三角面网格模式显示"));
		p_MyRadio[2]->SetTooltip(_T("以四面体网格模式显示"));
		p_MyRadio[3]->SetTooltip(_T("显示矢量箭头"));
	}

	//创建 颜色选择按钮
	{
		CString m_Caption;
		m_Caption.LoadString(IDC_COLOR); //取按钮标题
		CRect m_Rect = CRect(1100, 50, 1200, 90);
		m_ColorPicker = new CMFCColorButton();
		ASSERT_VALID(m_ColorPicker);
		m_ColorPicker->Create(m_Caption, WS_CHILD | WS_VISIBLE | 0 | WS_TABSTOP | BS_PUSHBUTTON | 0, m_Rect, this, IDC_COLOR); //创建按钮
		m_ColorPicker->EnableAutomaticButton(_T("默认值"), RGB(0, 0, 0));
		m_ColorPicker->EnableOtherButton(_T("其余颜色"));
		m_ColorPicker->SetColorName((COLORREF)-1, "Default Color");
		m_ColorPicker->SetColor((COLORREF)-1);
		m_ColorPicker->SetColumnsNumber(10);
	}



	// 创建显示工具条m_wndDisplayBar
	if (!m_wndDisplayBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndDisplayBar.LoadToolBar(IDR_TOOLBAR_DISPLAY))
		{
			TRACE0("Failed to create display toolbar\n");
			return -1;      // fail to create
		}
	m_wndDisplayBar.SetWindowText(_T("图形操作工具栏"));



	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		{
			TRACE0("未能创建工具栏\n");
			return -1;      // 未能创建
		}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));


	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	// 设置m_wndDisplayBar的停靠属性
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDisplayBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	DockPane(&m_wndDisplayBar);
	//DockPaneLeftOf(&m_wndToolBar, &m_wndDisplayBar); // 将m_wndToolBar停靠在m_wndDisplayBar的左边 


	//m_wndDisplayBar.MoveWindow(CRect(m_wndDisplayBar.GetCaretPos().x, m_wndDisplayBar.GetCaretPos().y, 500, 100), 1);
	
	//DockPaneLeftOf(&m_wndMenuBar, &m_wndDisplayBar);

	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//DockPaneLeftOf(&m_wndDisplayBar,&m_wndToolBar);

	// 将m_wndDisplayBar停靠在框架窗口的左边
	//DockControlBar(&m_wndDisplayBar, AFX_IDW_DOCKBAR_LEFT);

	//DockControlBarLeftOf(&m_wndToolBar,&m_wndDisplayBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);

	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(32, 32), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~FWS_ADDTOTITLE;   //去除标题条中的文档名 

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust  = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}
	return TRUE;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	DockControlBar(Bar,n,&rect);
}




void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	// TODO: 在此添加专用代码和/或调用基类

	CString csAppName;
	csAppName.Format(AFX_IDS_APP_TITLE);
	SetWindowText(csAppName);

	CFrameWndEx::OnUpdateFrameTitle(bAddToTitle);
}
