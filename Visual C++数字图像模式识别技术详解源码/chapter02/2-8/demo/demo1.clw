; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDemo1View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "demo1.h"
LastPage=0

ClassCount=17
Class1=CChildFrame
Class2=CDemo1App
Class3=CAboutDlg
Class4=CDemo1Doc
Class5=CDemo1View
Class6=GeometryMoveDlg
Class7=HistogramDrawDlg
Class8=HistSegmentDlg
Class9=LinearStepParamDlg
Class10=CMainFrame
Class11=MedianSmoothDlg
Class12=NeiAverSmoothDlg
Class13=StrechWindowDlg
Class14=ThreshStrechDlg
Class15=Zoomdlg
Class16=ZoomDlg

ResourceCount=13
Resource1=IDD_DLG_MOVE
Resource2=IDD_DLG_NeiborSmooth
Resource3=IDD_ABOUTBOX
Resource4=IDD_DLG_MedianSmooth
Resource5=IDD_DLG_THRESH
Resource6=IDD_DIALOG_Rotate
Resource7=IDR_MAINFRAME
Resource8=IDD_DLG_LinearStretch
Resource9=IDD_DIALOG_Histogram
Resource10=IDD_Dlg_THRESHWD
Resource11=IDD_DIALOG_ZOOM
Resource12=IDR_DEMO1TYPE
Class17=RotateDlg
Resource13=IDD_DIALOG_HistSegment

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CDemo1App]
Type=0
BaseClass=CWinApp
HeaderFile=demo1.h
ImplementationFile=demo1.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=demo1.cpp
ImplementationFile=demo1.cpp

[CLS:CDemo1Doc]
Type=0
BaseClass=CDocument
HeaderFile=demo1Doc.h
ImplementationFile=demo1Doc.cpp

[CLS:CDemo1View]
Type=0
BaseClass=CView
HeaderFile=demo1View.h
ImplementationFile=demo1View.cpp
LastObject=id_RegionGrow

[CLS:GeometryMoveDlg]
Type=0
BaseClass=CDialog
HeaderFile=GeometryMoveDlg.h
ImplementationFile=GeometryMoveDlg.cpp

[CLS:HistogramDrawDlg]
Type=0
BaseClass=CDialog
HeaderFile=HistogramDrawDlg.h
ImplementationFile=HistogramDrawDlg.cpp

[CLS:HistSegmentDlg]
Type=0
BaseClass=CDialog
HeaderFile=HistSegmentDlg.h
ImplementationFile=HistSegmentDlg.cpp
LastObject=HistSegmentDlg

[CLS:LinearStepParamDlg]
Type=0
BaseClass=CDialog
HeaderFile=LinearStepParamDlg.h
ImplementationFile=LinearStepParamDlg.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:MedianSmoothDlg]
Type=0
BaseClass=CDialog
HeaderFile=MedianSmoothDlg.h
ImplementationFile=MedianSmoothDlg.cpp

[CLS:NeiAverSmoothDlg]
Type=0
BaseClass=CDialog
HeaderFile=NeiAverSmoothDlg.h
ImplementationFile=NeiAverSmoothDlg.cpp

[CLS:StrechWindowDlg]
Type=0
BaseClass=CDialog
HeaderFile=StrechWindowDlg.h
ImplementationFile=StrechWindowDlg.cpp
LastObject=id_Robert

[CLS:ThreshStrechDlg]
Type=0
BaseClass=CDialog
HeaderFile=ThreshStrechDlg.h
ImplementationFile=ThreshStrechDlg.cpp

[CLS:Zoomdlg]
Type=0
BaseClass=CDialog
HeaderFile=Zoomdlg.h
ImplementationFile=Zoomdlg.cpp

[CLS:ZoomDlg]
Type=0
BaseClass=CDialog
HeaderFile=Zoomdlg.h
ImplementationFile=Zoomdlg.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

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

[DLG:IDD_DIALOG_Histogram]
Type=1
Class=HistogramDrawDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_Hist,static,1207959559
Control4=IDC_STATIC_GrayColor,static,1342177294
Control5=IDC_STATIC_Dev,static,1342308352
Control6=IDC_STATIC_Ave2,static,1342308352
Control7=IDC_STATIC_AveShow,static,1342308352
Control8=IDC_STATIC_DevShow,static,1342308352
Control9=IDC_STATIC,button,1342177287

[DLG:IDD_DIALOG_HistSegment]
Type=1
Class=HistSegmentDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_Hist,static,1207959559
Control4=IDC_STATIC_GrayColor,static,1342177294
Control5=IDC_SLIDER_GrayValue,msctls_trackbar32,1342242840
Control6=IDC_STATIC_Value,static,1342308352
Control7=IDC_STATIC_text,static,1342308352

[DLG:IDD_DLG_LinearStretch]
Type=1
Class=LinearStepParamDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_LinetransX1,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_LinetransY1,edit,1350631552
Control11=IDC_EDIT_LinetransX2,edit,1350631552
Control12=IDC_EDIT_LinetransY2,edit,1350631552
Control13=IDC_LINTRANS_SHOW,static,1342308352

[DLG:IDD_DLG_MediaSmooth]
Type=1
Class=MedianSmoothDlg

[DLG:IDD_DLG_NeiborSmooth]
Type=1
Class=NeiAverSmoothDlg
ControlCount=7
Control1=IDC_RADIO1_temp,button,1342373897
Control2=IDC_RADIO2_temp,button,1342242825
Control3=IDC_RADIO3_temp,button,1342242825
Control4=IDC_RADIO4_temp,button,1342242825
Control5=IDC_STATIC,button,1342177287
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816

[DLG:IDD_Dlg_THRESHWD]
Type=1
Class=StrechWindowDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_StrechTop,edit,1350631552
Control4=IDC_EDIT_StrechBottom,edit,1350631552
Control5=IDC_STATIC_strechBottom,static,1342308352
Control6=IDC_STATIC_strechTop,static,1342308352
Control7=IDC_STATIC,button,1342177287

[DLG:IDD_DLG_THRESH]
Type=1
Class=ThreshStrechDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_Thresh,edit,1350631552

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

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

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
Command19=id_Binary
Command20=id_HistogramDraw
Command21=id_HistgramAver
Command22=id_ImageReverse
Command23=id_ImgThresh
Command24=id_ThresholdWindow
Command25=id_LinearStrecth
Command26=id_GaussNoise
Command27=id_PepperSaltNoise
Command28=id_NeiAveSmooth
Command29=id_MedianSmooth
Command30=id_ChooseMaskSmooth
Command31=id_GradeSharp
Command32=id_LaplaceSharp
Command33=id_QuickFFt
Command34=id_QuickFFt_Reverse
Command35=id_IdealLowPass
Command36=id_ButterLowPass
Command37=id_IdealHighPass
Command38=id_ButterHighPass
Command39=id_HarrWaveletTrans
Command40=id_HarrWavRevTrans
Command41=id_Erosion
Command42=id_Dilation
Command43=id_Open
Command44=id_Close
Command45=id_Thinning
Command46=id_HistThreshSeg
Command47=id_AdaptiveThreshold
Command48=id_Robert
Command49=id_Sobel
Command50=id_Prewitt
Command51=id_Laplacian
Command52=id_EdgeTrace
Command53=id_RegionGrow
Command54=ID_APP_ABOUT
CommandCount=54

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

[DLG:IDD_DIALOG_Rotate]
Type=1
Class=RotateDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC_Rotate,static,1342308364
Control5=IDC_EDIT_Rotate,edit,1350631552

[DLG:IDD_DLG_MedianSmooth]
Type=1
Class=?
ControlCount=6
Control1=IDC_RADIO1_temp,button,1342373897
Control2=IDC_RADIO2_temp,button,1342242825
Control3=IDC_RADIO3_temp,button,1342242825
Control4=IDC_STATIC,button,1342177287
Control5=IDOK,button,1342242817
Control6=IDCANCEL,button,1342242816

[CLS:RotateDlg]
Type=0
HeaderFile=RotateDlg.h
ImplementationFile=RotateDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

