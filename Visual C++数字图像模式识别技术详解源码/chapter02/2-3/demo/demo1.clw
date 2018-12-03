; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDemo1Doc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "demo1.h"
LastPage=0

ClassCount=16
Class1=CDemo1App
Class2=CDemo1Doc
Class3=CDemo1View
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_DEMO1TYPE
Resource2=IDD_DLG_MOVE
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_MAINFRAME
Class7=GeometryMoveDlg
Resource4=IDD_DIALOG_ZOOM
Class8=Zoomdlg
Resource5=IDD_ABOUTBOX
Class9=HistSegmentDlg
Class10=HistogramDrawDlg
Class11=ThreshStrechDlg
Class12=StrechWindowDlg
Class13=LinearStepParamDlg
Class14=NeiAverSmoothDlg
Class15=MedianSmoothDlg
Class16=RotateDlg
Resource6=IDD_DIALOG_Rotate

[CLS:CDemo1App]
Type=0
HeaderFile=demo1.h
ImplementationFile=demo1.cpp
Filter=N

[CLS:CDemo1Doc]
Type=0
HeaderFile=demo1Doc.h
ImplementationFile=demo1Doc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CDemo1Doc

[CLS:CDemo1View]
Type=0
HeaderFile=demo1View.h
ImplementationFile=demo1View.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=id_HistThreshSeg


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=demo1.cpp
ImplementationFile=demo1.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_DEMO1TYPE]
Type=1
Class=CDemo1View
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=id_Move
Command12=id_HorizontalMirror
Command13=id_VerticalMirror
Command14=id_Zoom
Command15=id_Clockwise90
Command16=id_Anticlockwise90
Command17=id_Rotate180
Command18=id_FreeRotate
Command19=ID_APP_ABOUT
CommandCount=19

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DLG_MOVE]
Type=1
Class=GeometryMoveDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_Edit_Xmove,edit,1350631552
Control6=IDC_Edit_Ymove,edit,1350631552
Control7=IDC_STATIC,button,1342177287

[CLS:GeometryMoveDlg]
Type=0
HeaderFile=GeometryMoveDlg.h
ImplementationFile=GeometryMoveDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_Edit_OffsetX

[DLG:IDD_DIALOG_ZOOM]
Type=1
Class=Zoomdlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_strechH,edit,1350631552
Control7=IDC_EDIT_strechW,edit,1350631552

[CLS:Zoomdlg]
Type=0
HeaderFile=Zoomdlg.h
ImplementationFile=Zoomdlg.cpp
BaseClass=CDialog
Filter=D
LastObject=id_Anticlockwise90
VirtualFilter=dWC

[DLG:IDD_DIALOG_Rotate]
Type=1
Class=RotateDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC_Rotate,static,1342308364
Control5=IDC_EDIT_Rotate,edit,1350631552

[CLS:HistogramDrawDlg]
Type=0
HeaderFile=HistogramDrawDlg.h
ImplementationFile=HistogramDrawDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=HistogramDrawDlg
VirtualFilter=dWC

[CLS:ThreshStrechDlg]
Type=0
HeaderFile=ThreshStrechDlg.h
ImplementationFile=ThreshStrechDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=id_Anticlockwise90

[CLS:StrechWindowDlg]
Type=0
HeaderFile=StrechWindowDlg.h
ImplementationFile=StrechWindowDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=id_Anticlockwise90

[CLS:LinearStepParamDlg]
Type=0
HeaderFile=LinearStepParamDlg.h
ImplementationFile=LinearStepParamDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_LinetransX1

[CLS:NeiAverSmoothDlg]
Type=0
HeaderFile=NeiAverSmoothDlg.h
ImplementationFile=NeiAverSmoothDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO4_temp
VirtualFilter=dWC

[CLS:MedianSmoothDlg]
Type=0
HeaderFile=MedianSmoothDlg.h
ImplementationFile=MedianSmoothDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=MedianSmoothDlg

[CLS:HistSegmentDlg]
Type=0
HeaderFile=HistSegmentDlg.h
ImplementationFile=HistSegmentDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=HistSegmentDlg
VirtualFilter=dWC

[CLS:RotateDlg]
Type=0
HeaderFile=RotateDlg.h
ImplementationFile=RotateDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=id_Anticlockwise90

