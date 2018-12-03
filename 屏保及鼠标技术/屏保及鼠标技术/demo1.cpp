#include <graphics.h>
#include <time.h>
#include <conio.h>
int Height;
int Width;
/*
此程序无法通过，可能此插件不兼容此vs版本
*/
int main()
{
	//设置随机函数种子
	srand((unsigned)time(NULL));
	//初始化窗口
	initgraph(1920, 1080);
	int x, y; // （x，y）坐标
	char c;  //字符
	//设置字体
	settextstyle(16, 8, _T("Courier")); // 因为版本问题，自适应

	//去边框设置新样式
	//获取用户界面的宽和高
	Width = GetSystemMetrics(SM_CXSCREEN);
	Height = GetSystemMetrics(SM_CYSCREEN);

	//重新调用初始化图形窗口并返回句柄
	HWND hwnd = initgraph(Width, Height);

	//获取窗体当前属性写入style
	long style = GetWindowLong(hwnd, GWL_STYLE);

	//去掉窗体边框
	style &= ~WS_CAPTION;

	//修改style设置窗体
	SetWindowLong(hwnd, GWL_STYLE,style);

	//移动窗体至全屏
	MoveWindow(hwnd, 0, 0, Width, Height, true);




	//设置颜色
	settextcolor(GREEN);
	setlinecolor(BLACK);
	for (int i = 0; i <= 1919; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			x = (rand() % 80) * 24; // 24*80 = 1920
			y = (rand() % 20) * 54; // 54*20 = 1080
			c = (rand() % 26) + 65; // 大写英文26个字母随机产生
			outtextxy(x, y, c);
		}
		
		line(0, i, 1919, i);		//画线擦掉以前字母
		Sleep(20);					// 延时处理
		if (i >= 1919) i = -1;		//循环处理
		if (_kbhit()) break;			//_kbhit()按任意键退出
	}

	//关闭窗口
	closegraph();
	return 0;
}