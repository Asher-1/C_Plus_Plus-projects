// firework.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

/********************************************************
*														*
*	程序名称：祝福烟花，祝福朋友						*
*	编译环境：VC++6.0 && easyx(立冬版)					*
*	作者相关：yy滺滺__<837943056@qq.com>				*
*	最后修改：2012.11.22								*
*														*
********************************************************/
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <Mmsystem.h>		// 使用该计时器必须包含的文件
#pragma comment ( lib, "Winmm.lib" )

/***** 宏定义区 ******/

#define NUM		13			// 烟花种类数量宏定义

/***** 结构定义区 **********/

// 烟花结构
struct FIRE
{
	int r;					// 当前爆炸半径
	int max_r;				// 爆炸中心距离边缘最大半径
	int x, y;				// 爆炸中心在窗口的坐标
	int cen_x, cen_y;		// 爆炸中心相对图片左上角的坐标
	int width, height;		// 图片的宽高
	int xy[240][240];		// 储存图片像素点

	bool show;				// 是否绽放
	bool draw;				// 开始输出像素点
	DWORD t1, t2, dt;		// 绽放速度
}Fire[NUM];

// 烟花弹结构
struct JET
{
	int x, y;				// 喷射点坐标
	int hx, hy;				// 最高点坐标------将赋值给 FIRE 里面的 x, y
	int height;				// 烟花高度
	bool shoot;				// 是否可以发射

	DWORD t1, t2, dt;		// 发射速度
	IMAGE img[2];			// 储存花弹一亮一暗图片
	byte n : 1;				// 图片下标
}Jet[NUM];

// happy birthday 艺术字结构
struct HAPPY				// 储存 Happy Birthday 艺术字图片
{
	int x, y;				// 每个字的图片的左上角坐标
	IMAGE img;				// 储存单个字图片
	static int num;			// 字母序号
}Happy[NUM];
int HAPPY::num = 0;

// For.2012の夭女 结构
struct WISH					// 滚动祝福图片结构
{
	int x, y;				// 图片左上角坐标
	DWORD t1, t2, dt;		// 图片运动时间间隔
	IMAGE img;				// 储存图片
	int dxy;				// 每次的移动量
	byte dir : 1;			// 限制两个滚动方向
}Wish;

/**** 函数申明区 ****/

void Init(int);		// 初始化烟花
void Load();		// 加载烟花图片
void Shoot(TCHAR*);		// 发射烟花
void Chose(DWORD&, TCHAR*);		// 筛选烟花
void Wishing();		// 滚动祝福
void Style(DWORD&, TCHAR*);		// 发射样式
void Show(DWORD*);		// 绽放烟花
bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName);

bool ctrlhandler(DWORD);		// 关闭捕捉




int main()
{
	initgraph(1200, 800);
	srand(time(0));

	// 产生临时文件的文件名
	// 注：如果 MP3 资源的 ID 为宏 IDR_BACKGROUND，这样做：
	// ExtractResource(tmpmp3, _T("MP3"), MAKEINTRESOURCE(IDR_BACKGROUND));

	// 设置发射音乐
	TCHAR shootMusic[_MAX_PATH];
	::GetTempPath(_MAX_PATH, shootMusic);
	_tcscat_s(shootMusic, _T("testshoot.mp3"));
	ExtractResource(shootMusic, _T("WAVE"), _T("shoot.mp3"));

	// 设置爆炸音乐
	TCHAR bombMusic[_MAX_PATH];
	::GetTempPath(_MAX_PATH, bombMusic);
	_tcscat_s(bombMusic, _T("testbomb.wav"));
	ExtractResource(bombMusic, _T("WAVE"), _T("bomb.wav"));



	// 设置背景音乐
	TCHAR backMusic[_MAX_PATH];
	::GetTempPath(_MAX_PATH, backMusic);
	_tcscat_s(backMusic, _T("testapp_background.mp3"));		// VC2008、VC2010 请用 _tcscat_s 函数
															// 将 MP3 资源提取为临时文件
	ExtractResource(backMusic, _T("WAVE"), _T("background.mp3"));

	// 打开背景音乐
	TCHAR mcicmd[300];
	_stprintf_s(mcicmd, _T("open \"%s\" alias mymusic"), backMusic);     // VC2008、VC2010 请用 _stprintf_s 函数
	mciSendString(mcicmd, 0, 0, 0);

	// 播放音乐
	mciSendString(_T("play mymusic repeat"), 0, 0, 0);

	// 播放背景音乐
	//mciSendString("open ./fire/bk.mp3 alias bk", 0, 0, 0);
	//mciSendString("play bk repeat", 0, 0, 0);

	setfillstyle(0);
	settextstyle(36, 0, "楷体");
	setcolor(YELLOW);
	outtextxy(370, 100, "... ～～陆大海(*^__^*)～～ ...");
	outtextxy(370, 150, "9月2号，欣茵 HappyBirthday！，希望您会喜欢:)");
	//outtextxy(380, 200, "可不要吓到哦(*^__^*)");

	DWORD t1 = timeGetTime();		// 筛选烟花计时
	DWORD st1 = timeGetTime();		// 播放花样计时
	DWORD* pMem = GetImageBuffer();		// 获取窗口显存指针

	for (int i = 0; i < NUM; i++)		// 初始化烟花
	{
		Init(i);
	}
	Load();								// 将烟花图片信息加载进相应结构中
	BeginBatchDraw();					// 开始批量绘图

	while (!_kbhit())
	{
		Sleep(10);

		// 随机选择 4000 个像素点擦除
		for (int clr = 0; clr < 1000; clr++)
		{
			for (int j = 0; j < 2; j++)
			{
				int px1 = rand() % 1200;
				int py1 = rand() % 800;

				if (py1 < 799)				// 防止越界
					pMem[py1 * 1200 + px1] = pMem[py1 * 1200 + px1 + 1] = BLACK;	// 对显存赋值擦出像素点
			}
		}
		Chose(t1, shootMusic);		// 筛选烟花
		Shoot(bombMusic);		// 发射烟花
		Show(pMem);		// 绽放烟花
		Wishing();		// 滚动字符
		Style(st1, shootMusic);		// 花样发射
		FlushBatchDraw();		// 显示前面的所有绘图操作

	}

	// 删除临时文件
	DeleteFile(backMusic);
	DeleteFile(shootMusic);
	DeleteFile(bombMusic);

    return 0;
}


// 初始化烟花参数
void Init(int i)
{
	// 分别为：烟花中心到图片边缘的最远距离、烟花中心到图片左上角的距离 (x、y) 两个分量
	int r[13] = { 120, 120, 155, 123, 130, 147, 138, 138, 130, 135, 140, 132, 155 };
	int x[13] = { 120, 120, 110, 117, 110, 93,  102, 102, 110, 105, 100, 108, 110 };
	int y[13] = { 120, 120, 85,  118, 120, 103, 105, 110, 110, 120, 120, 104, 85 };

	/**** 初始化烟花 *****/

	Fire[i].x = 0;				// 烟花中心坐标
	Fire[i].y = 0;
	Fire[i].width = 240;				// 图片宽
	Fire[i].height = 240;				// 图片高
	Fire[i].max_r = r[i];				// 最大半径
	Fire[i].cen_x = x[i];				// 中心距左上角距离
	Fire[i].cen_y = y[i];
	Fire[i].show = false;			// 是否绽放
	Fire[i].dt = 5;				// 绽放时间间隔
	Fire[i].t1 = timeGetTime();
	Fire[i].r = 0;				// 从 0 开始绽放

								/**** 初始化烟花弹 *****/

	Jet[i].x = -240;				// 烟花弹左上角坐标
	Jet[i].y = -240;
	Jet[i].hx = -240;				// 烟花弹发射最高点坐标
	Jet[i].hy = -240;
	Jet[i].height = 0;				// 发射高度
	Jet[i].t1 = timeGetTime();
	Jet[i].dt = rand() % 10;		// 发射速度时间间隔
	Jet[i].n = 0;				// 烟花弹闪烁图片下标
	Jet[i].shoot = false;			// 是否发射
}


// 加载图片
void Load()
{
	/**** 储存烟花的像素点颜色 ****/
	IMAGE fm, gm;
	//loadimage(&fm, "./fire/flower.jpg", 3120, 240);
	loadimage(&fm, _T("IMAGE"), _T("flower"), 3120, 240);
	

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&fm);
		getimage(&gm, i * 240, 0, 240, 240);
		SetWorkingImage(&gm);

		for (int a = 0; a < 240; a++)
			for (int b = 0; b < 240; b++)
				Fire[i].xy[a][b] = getpixel(a, b);
	}

	/**** 加载烟花弹 ************/
	IMAGE sm;
	//loadimage(&sm, "./fire/shoot.jpg", 200, 50);
	loadimage(&sm, _T("IMAGE"), _T("shoot"), 200, 50);

	for (int i = 0; i < 13; i++)
	{
		SetWorkingImage(&sm);
		int n = rand() % 5;

		getimage(&Jet[i].img[0], n * 20, 0, 20, 50);			// 暗
		getimage(&Jet[i].img[1], (n + 5) * 20, 0, 20, 50);	// 亮
	}

	/***** 加载 Happy Birthday 图片 ********/
	IMAGE hm;
	//loadimage(&hm, "./fire/happy.jpg", 689, 115);
	loadimage(&hm, _T("IMAGE"), _T("happy"), 689, 115);
	SetWorkingImage(&hm);

	for (int i = 0; i < 13; i++)
	{
		Happy[i].x = i * 90;
		Happy[i].y = rand() % 100 + 500;
		getimage(&Happy[i].img, i * 53, 0, 53, 115);
	}

	/***** 加载图片 *********/
	Wish.x = 0;
	Wish.y = 100;
	Wish.t1 = timeGetTime();
	Wish.dt = 46;
	Wish.dir = 0;
	Wish.dxy = rand() % 8 + 1;

	loadimage(&Wish.img, _T("IMAGE"), _T("寿星.jpg"), 490, 100);
	//loadimage(&Wish.img, "./fire/寿星.jpg", 490, 100);
	putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

	SetWorkingImage();		// 设置回绘图窗口
}


// 在一定范围内筛选可发射的烟花，并初始化发射参数，输出烟花弹到屏幕，播放声音
void Chose(DWORD& t1, TCHAR* shootMusic)
{
	DWORD t2 = timeGetTime();

	if (t2 - t1 > 100)
	{

		int n = rand() % 20;

		if (n < 13 && Jet[n].shoot == false && Fire[n].show == false)
		{
			/**** 重置烟花弹，预备发射 *****/
			Jet[n].x = rand() % 1200;
			Jet[n].y = rand() % 100 + 600;
			Jet[n].hx = Jet[n].x;
			Jet[n].hy = rand() % 400;
			Jet[n].height = Jet[n].y - Jet[n].hy;
			Jet[n].shoot = true;
			putimage(Jet[n].x, Jet[n].y, &Jet[n].img[Jet[n].n], SRCINVERT);

			/**** 播放每个烟花弹的声音 *****/
			char c1[300], c2[30], c3[30];
			sprintf_s(c1, "open \"%s\" alias s%d", shootMusic, n);
			//sprintf_s(c1, "open ./fire/shoot.mp3 alias s%d", n);
			sprintf_s(c2, "play s%d", n);
			sprintf_s(c3, "close n%d", n);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		t1 = t2;
	}
}


// 扫描烟花弹并发射
void Shoot(TCHAR* bombMusic)
{

	for (int i = 0; i < 13; i++)
	{
		Jet[i].t2 = timeGetTime();

		if (Jet[i].t2 - Jet[i].t1 > Jet[i].dt && Jet[i].shoot == true)
		{
			/**** 烟花弹的上升 *****/
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			if (Jet[i].y > Jet[i].hy)
			{
				Jet[i].n++;
				Jet[i].y -= 5;
			}

			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);

			/**** 上升到高度的 3 / 4，减速 *****/
			if ((Jet[i].y - Jet[i].hy) * 4 < Jet[i].height)
				Jet[i].dt = rand() % 4 + 10;

			/**** 上升到最大高度 *****/
			if (Jet[i].y <= Jet[i].hy)
			{
				// 播放爆炸声
				CHAR c1[300], c2[30], c3[30];

				//sprintf_s(c1, "open ./fire/bomb.wav alias n%d", i);
				sprintf_s(c1, "open \"%s\" alias n%d", bombMusic, i);
				sprintf_s(c2, "play n%d", i);
				sprintf_s(c3, "close s%d", i);

				mciSendString(c3, 0, 0, 0);
				mciSendString(c1, 0, 0, 0);
				mciSendString(c2, 0, 0, 0);

				putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 擦掉烟花弹
				Fire[i].x = Jet[i].hx + 10;											// 在烟花弹中间爆炸
				Fire[i].y = Jet[i].hy;												// 在最高点绽放
				Fire[i].show = true;					// 开始绽放
				Jet[i].shoot = false;					// 停止发射

														// 显示对应字母
				putimage(Happy[HAPPY::num].x, Happy[HAPPY::num].y, &Happy[HAPPY::num].img, SRCINVERT);
				HAPPY::num++;

				if (HAPPY::num > 12)
					HAPPY::num = 0;
			}
			Jet[i].t1 = Jet[i].t2;
		}
	}
}


// 制作水平、竖直移动的图片
void Wishing()
{
	Wish.t2 = timeGetTime();

	if (Wish.t2 - Wish.t1 > Wish.dt)
	{
		/**** 图片的移动 *****/
		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		if (Wish.dir == 0)
			Wish.x += Wish.dxy;
		else
			Wish.y -= Wish.dxy;

		putimage(Wish.x, Wish.y, &Wish.img, SRCINVERT);

		/**** 图片出界后 *****/
		if (Wish.x > 1200 || Wish.y < 0)
		{
			Wish.dir = rand() % 2;			// 获取随机方向

			if (Wish.dir == 0)			// 如果左右运动
			{
				Wish.y = rand() % 700;		// 初始位置
				Wish.x = -490;
				Wish.dxy = rand() % 7 + 1;	// 随机运动分量
			}
			else							// 上下运动
			{
				Wish.dxy = rand() % 7 + 1;
				Wish.x = rand() % 700;
				Wish.y = 800;
			}
		}
		Wish.t1 = Wish.t2;
	}
}


// 显示花样
void Style(DWORD& st1, TCHAR* shootMusic)
{
	DWORD st2 = timeGetTime();

	if (st2 - st1 > 266000)		// 一首歌的时间
	{

		// 心形坐标
		int x[13] = { 60, 75, 91, 100, 95, 75, 60, 45, 25, 15, 25, 41, 60 };
		int y[13] = { 65, 53, 40, 22, 5, 4, 20, 4, 5, 22, 40, 53, 65 };
		for (int i = 0; i < NUM; i++)
		{
			cleardevice();
			/**** 规律分布烟花弹 ***/
			Jet[i].x = x[i] * 10;
			Jet[i].y = (y[i] + 75) * 10;
			Jet[i].hx = Jet[i].x;
			Jet[i].hy = y[i] * 10;
			Jet[i].height = Jet[i].y - Jet[i].hy;
			Jet[i].shoot = true;
			Jet[i].dt = 7;
			putimage(Jet[i].x, Jet[i].y, &Jet[i].img[Jet[i].n], SRCINVERT);	// 显示烟花弹

																			/**** 设置烟花参数 ***/
			Fire[i].x = Jet[i].x + 10;
			Fire[i].y = Jet[i].hy;
			Fire[i].show = false;
			Fire[i].r = 0;

			/**** 播放发射声音 ***/
			char c1[300], c2[30], c3[30];
			sprintf_s(c1, "open \"%s\" alias s%d", shootMusic, i);
			//sprintf_s(c1, "open ./fire/shoot.mp3 alias s%d", i);
			sprintf_s(c2, "play s%d", i);
			sprintf_s(c3, "close n%d", i);

			mciSendString(c3, 0, 0, 0);
			mciSendString(c1, 0, 0, 0);
			mciSendString(c2, 0, 0, 0);
		}
		st1 = st2;
	}
}


// 绽放烟花
void Show(DWORD* pMem)
{
	// 烟花个阶段绽放时间间隔，制作变速绽放效果
	int drt[16] = { 5, 5, 5, 5, 5, 6, 25, 25, 25, 25, 55, 55, 55, 55, 55 };

	for (int i = 0; i < NUM; i++)
	{
		Fire[i].t2 = timeGetTime();

		// 增加爆炸半径，绽放烟花，增加时间间隔做变速效果
		if (Fire[i].t2 - Fire[i].t1 > Fire[i].dt && Fire[i].show == true)
		{
			if (Fire[i].r < Fire[i].max_r)
			{
				Fire[i].r++;
				Fire[i].dt = drt[Fire[i].r / 10];
				Fire[i].draw = true;
			}

			if (Fire[i].r >= Fire[i].max_r - 1)
			{
				Fire[i].draw = false;
				Init(i);
			}
			Fire[i].t1 = Fire[i].t2;
		}

		// 如果该号炮花可爆炸，根据当前爆炸半径画烟花，颜色值接近黑色的不输出。
		if (Fire[i].draw)
		{
			for (double a = 0; a <= 6.28; a += 0.01)
			{
				int x1 = (int)(Fire[i].cen_x + Fire[i].r * cos(a));				// 相对于图片左上角的坐标
				int y1 = (int)(Fire[i].cen_y - Fire[i].r * sin(a));

				if (x1 > 0 && x1 < Fire[i].width && y1 > 0 && y1 < Fire[i].height)	// 只输出图片内的像素点
				{
					int b = Fire[i].xy[x1][y1] & 0xff;
					int g = (Fire[i].xy[x1][y1] >> 8) & 0xff;
					int r = (Fire[i].xy[x1][y1] >> 16);

					// 烟花像素点在窗口上的坐标
					int xx = (int)(Fire[i].x + Fire[i].r * cos(a));
					int yy = (int)(Fire[i].y - Fire[i].r * sin(a));

					// 较暗的像素点不输出、防止越界
					if (r > 0x20 && g > 0x20 && b > 0x20 && xx > 0 && xx < 1200 && yy > 0 && yy < 800)
						pMem[yy * 1200 + xx] = BGR(Fire[i].xy[x1][y1]);	// 显存操作绘制烟花
				}
			}
			Fire[i].draw = false;
		}
	}
}


//////////////////////////////////////////////////////////////////
// 提取指定模块中的资源文件
// 参数：
//		strDstFile:		目标文件名。提取的资源将保存在这里；
//		strResType:		资源类型；
//		strResName:		资源名称；
// 返回值：
//		true: 执行成功；
//		false: 执行失败。
bool ExtractResource(LPCTSTR strDstFile, LPCTSTR strResType, LPCTSTR strResName)
{
	// 创建文件
	HANDLE hFile = ::CreateFile(strDstFile, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// 查找资源文件中、加载资源到内存、得到资源大小
	HRSRC	hRes = ::FindResource(NULL, strResName, strResType);
	HGLOBAL	hMem = ::LoadResource(NULL, hRes);
	DWORD	dwSize = ::SizeofResource(NULL, hRes);

	// 写入文件
	DWORD dwWrite = 0;  	// 写入大小
	::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
	::CloseHandle(hFile);

	return true;
}


bool ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal.
	case CTRL_C_EVENT:
		printf("ctrl-c event\n\n");
		return(true);
		// ctrl-close: confirm that the user wants to exit.
	case CTRL_CLOSE_EVENT:
		printf("ctrl-close event\n\n");
		return(true);
		// pass other signals to the next handler.
	case CTRL_BREAK_EVENT:
		printf("ctrl-break event\n\n");
		return false;
	case CTRL_LOGOFF_EVENT:
		printf("ctrl-logoff event\n\n");
		return false;
	case CTRL_SHUTDOWN_EVENT:
		printf("ctrl-shutdown event\n\n");
		return false;
	default:
		return false;
	}
}