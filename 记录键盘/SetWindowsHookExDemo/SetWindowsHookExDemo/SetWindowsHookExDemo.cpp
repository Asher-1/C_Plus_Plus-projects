// SetWindowsHookExDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>


typedef BOOL(*Func)();

int main()
{
	DWORD dwError = NOERROR;
	BOOL bRet = FALSE;
	HINSTANCE hDll = nullptr;
	do 
	{
		hDll = LoadLibraryW(L"SetWindowsHookExDLL.dll");
		if (!hDll)
		{
			dwError = GetLastError();
			break;
		}

		Func fun = (Func)GetProcAddress(hDll, "StartHook");
		if (!fun)
		{
			dwError = GetLastError();
			break;
		}

		if (!fun())
		{
			dwError = GetLastError();
			break;
		}
		

		while (true)
		{
			char szInput[MAXBYTE] = { 0 };
			std::cin >> szInput;
			
			if (0 == strcmp(szInput, "exit"))
			{
				break;
			}
		}

		bRet = TRUE;
	} while (FALSE);

	if (FALSE == bRet)
	{
		std::cout << "出现错误！！！" << dwError << std::endl;
	}

	if (hDll)
	{
		FreeLibrary(hDll);
	}
	
	system("pause");
    return 0;
}

