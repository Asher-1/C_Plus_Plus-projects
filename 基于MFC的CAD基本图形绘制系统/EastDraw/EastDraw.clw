; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "eastdraw.h"
LastPage=0

ClassCount=15
Class1=CChildFrame
Class2=CDLineDlg1
Class3=CDLineDlg2
Class4=CDLineProperty
Class5=CEastDrawApp
Class6=CAboutDlg
Class7=CEastDrawDoc
Class8=CEastDrawView
Class9=CEditBox
Class10=CMainFrame
Class11=CMode
Class12=CReginSet
Class13=CSplashWnd
Class14=CStyleBar
Class15=CTextEdit

ResourceCount=13
Resource1=IDD_PROPPAGE_DLine2
Resource2=IDR_MENU_TextEdit
Resource3=IDD_PROPPAGE_DLine1
Resource4=IDD_ABOUTBOX
Resource5=IDR_MAINFRAME
Resource6=IDD_PROPPAGE_Mode
Resource7=IDR_TOOLBAR1_DrawTool
Resource8=IDR_STYLE_BAR (English (U.S.))
Resource9=IDR_MENU_Canvas_Edit
Resource10=IDR_MENU_OK
Resource11=IDR_MENU_Delete
Resource12=IDR_EASTDRTYPE
Resource13=IDD_DIALOG_ReginSet

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CDLineDlg1]
Type=0
BaseClass=CPropertyPage
HeaderFile=DLineDlg1.h
ImplementationFile=DLineDlg1.cpp

[CLS:CDLineDlg2]
Type=0
BaseClass=CPropertyPage
HeaderFile=DLineDlg2.h
ImplementationFile=DLineDlg2.cpp

[CLS:CDLineProperty]
Type=0
BaseClass=CPropertySheet
HeaderFile=DLineProperty.h
ImplementationFile=DLineProperty.cpp

[CLS:CEastDrawApp]
Type=0
BaseClass=CWinApp
HeaderFile=EastDraw.h
ImplementationFile=EastDraw.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=EastDraw.cpp
ImplementationFile=EastDraw.cpp
LastObject=CAboutDlg

[CLS:CEastDrawDoc]
Type=0
BaseClass=CDocument
HeaderFile=EastDrawDoc.h
ImplementationFile=EastDrawDoc.cpp

[CLS:CEastDrawView]
Type=0
BaseClass=CScrollView
HeaderFile=EastDrawView.h
ImplementationFile=EastDrawView.cpp

[CLS:CEditBox]
Type=0
BaseClass=CDialog
HeaderFile=EditBox.h
ImplementationFile=EditBox.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMode]
Type=0
BaseClass=CPropertyPage
HeaderFile=Mode.h
ImplementationFile=Mode.cpp

[CLS:CReginSet]
Type=0
BaseClass=CDialog
HeaderFile=ReginSet.h
ImplementationFile=ReginSet.cpp

[CLS:CSplashWnd]
Type=0
BaseClass=CWnd
HeaderFile=Splash.h
ImplementationFile=Splash.cpp

[CLS:CStyleBar]
Type=0
BaseClass=CToolBar
HeaderFile=StyleBar.h
ImplementationFile=StyleBar.cpp

[CLS:CTextEdit]
Type=0
BaseClass=CEdit
HeaderFile=TextEdit.h
ImplementationFile=TextEdit.cpp

[DLG:IDD_PROPPAGE_DLine1]
Type=1
Class=CDLineDlg1
ControlCount=26
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO_Ps_Solid,button,1342308361
Control3=IDC_RADIO2,button,1342177289
Control4=IDC_RADIO3,button,1342177289
Control5=IDC_RADIO4,button,1342177289
Control6=IDC_RADIO5,button,1342177289
Control7=IDC_RADIO15,button,1342177289
Control8=IDC_RADIO16,button,1342177289
Control9=IDC_STATIC_ShowPenStyle,static,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_EDIT_LineWidth,edit,1350631552
Control12=IDC_SPIN_DDLineWidth,msctls_updown32,1342177334
Control13=IDC_STATIC,button,1342177287
Control14=IDC_RADIO6_bs_hollow,button,1342308361
Control15=IDC_RADIO7_bs_solid,button,1342177289
Control16=IDC_RADIO8_hatched,button,1342177289
Control17=IDC_STATIC,button,1342177287
Control18=IDC_RADIO9_HS_BDIAGONAL,button,1342308361
Control19=IDC_RADIO11_HS_DIAGCROSS,button,1342177289
Control20=IDC_RADIO13_HS_FDIAGONAL,button,1342177289
Control21=IDC_RADIO10_HS_CROSS,button,1342177289
Control22=IDC_RADIO14_HS_HORIZONTAL,button,1342177289
Control23=IDC_RADIO_HS_VERTICAL,button,1342177289
Control24=IDC_STATIC_ShowBrushResult,static,1342177287
Control25=IDC_COMBO_BackgroundMode,combobox,1344340227
Control26=IDC_STATIC,button,1342177287

[DLG:IDD_PROPPAGE_DLine2]
Type=1
Class=CDLineDlg2
ControlCount=38
Control1=IDC_STATIC_Black,static,1350570766
Control2=IDC_STATIC_RarkRed,static,1350570766
Control3=IDC_STATIC_DrakGreen,static,1350570766
Control4=IDC_STATIC_DarkYellow,static,1350570766
Control5=IDC_STATIC_DrakBlue,static,1350570766
Control6=IDC_STATIC_Yellow,static,1350570766
Control7=IDC_STATIC_DarkGray,static,1350570766
Control8=IDC_STATIC_LightRray,static,1350570766
Control9=IDC_STATIC_White,static,1350570766
Control10=IDC_STATIC_Red,static,1350570766
Control11=IDC_STATIC_Blue,static,1350570766
Control12=IDC_STATIC_BITMAP5,static,1350570766
Control13=IDC_STATIC_Green,static,1350570766
Control14=IDC_STATIC_BITMAP6,static,1350570766
Control15=IDC_STATIC_BITMAP9,static,1350570766
Control16=IDC_STATIC_BITMAP10,static,1350570766
Control17=IDC_EDIT_Red,edit,1350633600
Control18=IDC_STATIC_Red2,static,1350570766
Control19=IDC_STATIC_Green2,static,1350570766
Control20=IDC_EDIT_Green,edit,1350633600
Control21=IDC_STATIC_Blue2,static,1350570766
Control22=IDC_EDIT_Blue,edit,1350633600
Control23=IDC_SPIN_Red,msctls_updown32,1073741856
Control24=IDC_SPIN_Green,msctls_updown32,1073741856
Control25=IDC_SPIN_Blue,msctls_updown32,1073741856
Control26=IDC_SLIDER_Red,msctls_trackbar32,1342242821
Control27=IDC_SLIDER_Green,msctls_trackbar32,1342242821
Control28=IDC_SLIDER_Blue,msctls_trackbar32,1342242821
Control29=IDC_STATIC_ShowColor,static,1350569992
Control30=IDC_STATIC,button,1342177287
Control31=IDC_BUTTON_Select,button,1342242816
Control32=IDC_BUTTON_ColorDlg,button,1342242816
Control33=IDC_CHECK_Pen,button,1342242819
Control34=IDC_CHECK_Brush,button,1342242819
Control35=IDC_CHECK_Background,button,1342242819
Control36=IDC_ShowPenColor,static,1350569991
Control37=IDC_ShowBrushColor,static,1350569991
Control38=IDC_ShowBackground,static,1350569991

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=2
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342177294

[DLG:IDD_DIALOG1]
Type=1
Class=CEditBox

[DLG:IDD_PROPPAGE_Mode]
Type=1
Class=CMode
ControlCount=7
Control1=IDC_COMBO_BackgroundMode,combobox,1344340227
Control2=IDC_TREE_DrawingMode,SysTreeView32,1350631975
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT_Detail,edit,1350637572
Control5=IDC_STATIC,button,1342177287
Control6=IDC_CHECK_ModeCheck,button,1476460547
Control7=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG_ReginSet]
Type=1
Class=CReginSet
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Width,edit,1350640768
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_Length,edit,1350640768
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDIT3,edit,1484849280
Control12=IDC_EDIT4,edit,1484849280
Control13=IDC_STATIC,static,1342177287

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS_Bmp
Command5=ID_FILE_PRINT
Command6=ID_BUTTON32830_Lock
Command7=ID_MENUITEM32794_SetRegin
Command8=ID_BUTTON32850_ReDraw
Command9=ID_MouseCap_On
Command10=ID_ShowDrawTool_Bar
Command11=Menu_ShowPorDlg
Command12=ID_BUTTON32845_Axis
Command13=ID_BUTTON32858_OnDo
Command14=ID_BUTTON32859_ReDo
CommandCount=14

[TB:IDR_TOOLBAR1_DrawTool]
Type=1
Class=?
Command1=ID_BUTTON_DLine
Command2=ID_BUTTON32776_Rectangle
Command3=ID_BUTTON32778_RoundRectangle
Command4=ID_BUTTON32791_Polygon
Command5=ID_BUTTON32779_Ellipse
Command6=ID_BUTTON32780_Round
Command7=ID_BUTTON32777_Curve
Command8=ID_BUTTON_LiEllipse
Command9=ID_BUTTON32800_RoundArc
Command10=ID_BUTTON32805_EllipseArc
Command11=ID_BUTTON32807_PolyBezier
Command12=ID_BUTTON32809_Text
Command13=ID_BUTTON32774_Move
Command14=ID_BUTTON32856_DONothing
CommandCount=14

[TB:IDR_STYLE_BAR (English (U.S.))]
Type=1
Class=?
Command1=ID_CHAR_BOLD
Command2=ID_CHAR_ITALIC
Command3=ID_CHAR_UNDERLINE
Command4=IDC_F
Command5=IDC_FONT
CommandCount=5

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_EASTDRTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_SAVE_AS_Bmp
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_MENUITEM32794_SetRegin
Command13=ID_BUTTON32850_ReDraw
Command14=ID_BUTTON32830_Lock
Command15=ID_BUTTON32858_OnDo
Command16=ID_BUTTON32859_ReDo
Command17=ID_MouseCap_On
Command18=Menu_ShowPorDlg
Command19=ID_BUTTON32845_Axis
Command20=ID_VIEW_STATUS_BAR
Command21=ID_VIEW_TOOLBAR
Command22=ID_ShowDrawTool_Bar
Command23=ID_WINDOW_NEW
Command24=ID_WINDOW_CASCADE
Command25=ID_WINDOW_TILE_HORZ
Command26=ID_WINDOW_ARRANGE
Command27=ID_APP_ABOUT
CommandCount=27

[MNU:IDR_MENU_TextEdit]
Type=1
Class=?
Command1=ID_UnDo
Command2=ID_Cut
Command3=ID_Copy
Command4=ID_Past
Command5=ID_Delete
Command6=ID_SelectAll
Command7=ID_Move
Command8=ID_MENUITEM_Size
CommandCount=8

[MNU:IDR_MENU_Canvas_Edit]
Type=1
Class=?
Command1=ID_CirCu
CommandCount=1

[MNU:IDR_MENU_OK]
Type=1
Class=?
CommandCount=0

[MNU:IDR_MENU_Delete]
Type=1
Class=?
CommandCount=0

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_EDIT_UNDO
Command3=ID_FILE_NEW
Command4=ID_FILE_OPEN
Command5=ID_FILE_PRINT
Command6=ID_FILE_SAVE
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_NEXT_PANE
Command11=ID_PREV_PANE
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_ACCEL32877
Command15=ID_EDIT_CUT
CommandCount=15

