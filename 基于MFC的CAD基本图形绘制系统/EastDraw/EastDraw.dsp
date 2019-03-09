# Microsoft Developer Studio Project File - Name="EastDraw" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EastDraw - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EastDraw.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EastDraw.mak" CFG="EastDraw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EastDraw - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EastDraw - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EastDraw - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EastDraw - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EastDraw - Win32 Release"
# Name "EastDraw - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\CDLine.cpp
# End Source File
# Begin Source File

SOURCE=.\CEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\CEllipseArc.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\CLiEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\CPolyBezier.cpp
# End Source File
# Begin Source File

SOURCE=.\CPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\CRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\CRound.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoundArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CRoundRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\CText.cpp
# End Source File
# Begin Source File

SOURCE=.\CUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\DLineDlg1.cpp
# End Source File
# Begin Source File

SOURCE=.\DLineDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\DLineProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\EastDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\EastDraw.rc
# End Source File
# Begin Source File

SOURCE=.\EastDrawDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EastDrawView.cpp
# End Source File
# Begin Source File

SOURCE=.\EditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Mode.cpp
# End Source File
# Begin Source File

SOURCE=.\ReginSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StyleBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TextEdit.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CCurve.h
# End Source File
# Begin Source File

SOURCE=.\CDLine.h
# End Source File
# Begin Source File

SOURCE=.\CEllipse.h
# End Source File
# Begin Source File

SOURCE=.\CEllipseArc.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\CLiEllipse.h
# End Source File
# Begin Source File

SOURCE=.\CPolyBezier.h
# End Source File
# Begin Source File

SOURCE=.\CPolygon.h
# End Source File
# Begin Source File

SOURCE=.\CRectangle.h
# End Source File
# Begin Source File

SOURCE=.\CRound.h
# End Source File
# Begin Source File

SOURCE=.\CRoundArc.h
# End Source File
# Begin Source File

SOURCE=.\CRoundRectangle.h
# End Source File
# Begin Source File

SOURCE=.\CText.h
# End Source File
# Begin Source File

SOURCE=.\CUnit.h
# End Source File
# Begin Source File

SOURCE=.\DLineDlg1.h
# End Source File
# Begin Source File

SOURCE=.\DLineDlg2.h
# End Source File
# Begin Source File

SOURCE=.\DLineProperty.h
# End Source File
# Begin Source File

SOURCE=.\EastDraw.h
# End Source File
# Begin Source File

SOURCE=.\EastDrawDoc.h
# End Source File
# Begin Source File

SOURCE=.\EastDrawView.h
# End Source File
# Begin Source File

SOURCE=.\EditBox.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Mode.h
# End Source File
# Begin Source File

SOURCE=.\ReginSet.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StyleBar.h
# End Source File
# Begin Source File

SOURCE=.\TextEdit.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\219.BMP
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap6.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap7.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap8.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap9.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_d.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor_m.cur
# End Source File
# Begin Source File

SOURCE=.\res\EastDraw.ico
# End Source File
# Begin Source File

SOURCE=.\res\EastDraw.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EastDrawDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\id_chang.cur
# End Source File
# Begin Source File

SOURCE=.\res\id_draw.cur
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Start.bmp
# End Source File
# Begin Source File

SOURCE=.\res\style_ba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\W0176.BMP
# End Source File
# End Group
# Begin Source File

SOURCE=.\EastDraw.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section EastDraw : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
