// GetAsynckeyStateDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
using std::cout;
using std::endl;
int main()
{
	while (true)
	{
		// 我们刚刚敲击空格键的时候
		if (-32767 == GetAsyncKeyState(VK_SPACE))
			cout << " ";
		else if (-32767 == GetAsyncKeyState('A'))
			cout << 'A';
		else if (-32767 == GetAsyncKeyState('B'))
			cout << 'B';
		else if (-32767 == GetAsyncKeyState('C'))
			cout << 'C';
		else if (-32767 == GetAsyncKeyState('D'))
			cout << 'D';
		else if (-32767 == GetAsyncKeyState('E'))
			cout << 'E';
		else if (-32767 == GetAsyncKeyState('F'))
			cout << 'F';
		else if (-32767 == GetAsyncKeyState('G'))
			cout << 'G';
		else if (-32767 == GetAsyncKeyState('H'))
			cout << 'H';
		else if (-32767 == GetAsyncKeyState('I'))
			cout << 'I';
		else if (-32767 == GetAsyncKeyState('J'))
			cout << 'J';
		else if (-32767 == GetAsyncKeyState('K'))
			cout << 'K';
		else if (-32767 == GetAsyncKeyState('L'))
			cout << 'L';
		else if (-32767 == GetAsyncKeyState('M'))
			cout << 'M';
		else if (-32767 == GetAsyncKeyState('N'))
			cout << 'N';
		else if (-32767 == GetAsyncKeyState('O'))
			cout << 'O';
		else if (-32767 == GetAsyncKeyState('P'))
			cout << 'P';
		else if (-32767 == GetAsyncKeyState('Q'))
			cout << 'Q';
		else if (-32767 == GetAsyncKeyState('R'))
			cout << 'R';
		else if (-32767 == GetAsyncKeyState('S'))
			cout << 'S';
		else if (-32767 == GetAsyncKeyState('T'))
			cout << 'T';
		else if (-32767 == GetAsyncKeyState('U'))
			cout << 'U';
		else if (-32767 == GetAsyncKeyState('V'))
			cout << 'V';
		else if (-32767 == GetAsyncKeyState('W'))
			cout << 'W';
		else if (-32767 == GetAsyncKeyState('X'))
			cout << 'X';
		else if (-32767 == GetAsyncKeyState('Y'))
			cout << 'Y';
		else if (-32767 == GetAsyncKeyState('0'))
			cout << '0';
		else if (-32767 == GetAsyncKeyState('1'))
			cout << '1';
		else if (-32767 == GetAsyncKeyState('2'))
			cout << '2';
		else if (-32767 == GetAsyncKeyState('3'))
			cout << '3';
		else if (-32767 == GetAsyncKeyState('4'))
			cout << '4';
		else if (-32767 == GetAsyncKeyState('5'))
			cout << '5';
		else if (-32767 == GetAsyncKeyState('6'))
			cout << '6';
		else if (-32767 == GetAsyncKeyState('7'))
			cout << '7';
		else if (-32767 == GetAsyncKeyState('8'))
			cout << '8';
		else if (-32767 == GetAsyncKeyState('9'))
			cout << '9';

		else if (-32767 == GetAsyncKeyState(VK_RETURN))
			cout << endl;
		
		Sleep(10);
	}
	
    return 0;
}

