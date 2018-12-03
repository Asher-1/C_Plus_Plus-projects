// dllmain.h: 模块类的声明。

class CSTLViewerHandlersModule : public ATL::CAtlDllModuleT< CSTLViewerHandlersModule >
{
public :
	DECLARE_LIBID(LIBID_STLViewerHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_STLVIEWERHANDLERS, "{89B17D7B-CEE5-44D8-A7B8-525CB3AE01F1}")
};

extern class CSTLViewerHandlersModule _AtlModule;
