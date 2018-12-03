// dllmain.cpp: DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "STLViewerHandlers_i.h"
#include "dllmain.h"
#include "xdlldata.h"

CSTLViewerHandlersModule _AtlModule;

class CSTLViewerHandlersApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSTLViewerHandlersApp, CWinApp)
END_MESSAGE_MAP()

CSTLViewerHandlersApp theApp;

BOOL CSTLViewerHandlersApp::InitInstance()
{
	if (!PrxDllMain(m_hInstance, DLL_PROCESS_ATTACH, NULL))
		return FALSE;
	return CWinApp::InitInstance();
}

int CSTLViewerHandlersApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
