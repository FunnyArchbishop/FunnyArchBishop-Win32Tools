#include <iostream>
#include <iomanip>
#include <cstdio>
#include <conio.h>
#include <windows.h>
using namespace std;

/* 静态全局变量（同一项目中的其它cpp不能访问本变量） */
static HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄

/***************************************************************************
  函数名称：cct_cls
  功    能：完成与system("cls")一样的功能，但效率高
  输入参数：
  返 回 值：
  说    明：清除整个屏幕缓冲区，不仅仅是可见窗口区域(使用当前颜色)
***************************************************************************/
void cct_cls()
{
	COORD coord = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO binfo; /* to get buffer info */
	DWORD num;

	/* 取当前缓冲区信息 */
	GetConsoleScreenBufferInfo(hout, &binfo);
	/* 填充字符 */
	FillConsoleOutputCharacter(hout, (TCHAR)' ', binfo.dwSize.X * binfo.dwSize.Y, coord, &num);
	/* 填充属性 */
	FillConsoleOutputAttribute(hout, binfo.wAttributes, binfo.dwSize.X * binfo.dwSize.Y, coord, &num);

	/* 光标回到(0,0) */
	SetConsoleCursorPosition(hout, coord);
	return;
}

/***************************************************************************
  函数名称：cct_gotoxy
  功    能：将光标移动到指定位置
  输入参数：int X：指定位置的x坐标（列）
			int Y：指定位置的y坐标（行）
  返 回 值：无
  说    明：左上角为(0,0)，此函数不准修改
***************************************************************************/
void cct_gotoxy(const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/***************************************************************************
  函数名称：cct_setcursor
  功    能：设置控制台光标是否可见
  输入参数：bool visible：true显示光标，false隐藏光标
  返 回 值：无
  说    明：常用于游戏界面隐藏光标
***************************************************************************/
void cct_setcursor(bool visible)
{
	CONSOLE_CURSOR_INFO cursor_info;
	GetConsoleCursorInfo(hout, &cursor_info);
	cursor_info.bVisible = visible ? TRUE : FALSE;
	SetConsoleCursorInfo(hout, &cursor_info);
}

/***************************************************************************
  函数名称：cct_getcursorpos
  功    能：获取当前光标所在位置
  输入参数：无
  返 回 值：COORD 结构，包含 X(列) 和 Y(行)
  说    明：左上角为(0,0)
***************************************************************************/
COORD cct_getcursorpos()
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hout, &binfo);
	return binfo.dwCursorPosition;
}

/***************************************************************************
  函数名称：cct_wherex
  功    能：获取当前光标的X坐标（列）
  输入参数：无
  返 回 值：int 当前列号，从0开始
  说    明：
***************************************************************************/
int cct_wherex()
{
	return cct_getcursorpos().X;
}

/***************************************************************************
  函数名称：cct_wherey
  功    能：获取当前光标的Y坐标（行）
  输入参数：无
  返 回 值：int 当前行号，从0开始
  说    明：
***************************************************************************/
int cct_wherey()
{
	return cct_getcursorpos().Y;
}

/***************************************************************************
  函数名称：cct_setcolor
  功    能：设置控制台文字的前景色和背景色
  输入参数：int back_color：背景色（0-15，Windows控制台颜色常量）
			int fore_color：前景色（0-15，Windows控制台颜色常量）
  返 回 值：无
  说    明：颜色值参考：0=黑,1=深蓝,2=深绿,3=深青,4=深红,5=深紫,
			6=深黄,7=灰白,8=灰,9=蓝,10=绿,11=青,12=红,13=紫,14=黄,15=白
***************************************************************************/
void cct_setcolor(const int back_color, const int fore_color)
{
	SetConsoleTextAttribute(hout, (WORD)((back_color << 4) | fore_color));
}

/***************************************************************************
  函数名称：cct_fillscreen
  功    能：用指定字符和颜色属性填充整个屏幕缓冲区
  输入参数：TCHAR ch  ：填充字符（默认空格）
			WORD attr ：颜色属性（默认0x07，灰底白字）
  返 回 值：无
  说    明：功能比cct_cls更灵活，可指定填充字符和颜色
***************************************************************************/
void cct_fillscreen(const TCHAR ch, const WORD attr)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hout, &binfo);

	COORD coord = { 0, 0 };
	DWORD num;
	DWORD total = binfo.dwSize.X * binfo.dwSize.Y;

	FillConsoleOutputCharacter(hout, ch, total, coord, &num);
	FillConsoleOutputAttribute(hout, attr, total, coord, &num);
	SetConsoleCursorPosition(hout, coord);
}

/***************************************************************************
  函数名称：cct_settitle
  功    能：设置控制台窗口标题栏文字
  输入参数：const char* title：标题字符串
  返 回 值：无
  说    明：使用SetConsoleTitleA设置ANSI标题
***************************************************************************/
void cct_settitle(const char* title)
{
	SetConsoleTitleA(title);
}

/***************************************************************************
  函数名称：cct_setfontsize
  功    能：设置控制台字体名称和大小
  输入参数：const char* font_name：字体名称，如"新宋体"、"Consolas"
			const int font_size ：字体大小（点阵高度）
  返 回 值：无
  说    明：通过修改注册表或使用CONSOLE_FONT_INFOEX结构设置
***************************************************************************/
void cct_setfontsize(const char* font_name, const int font_size)
{
	CONSOLE_FONT_INFOEX font_info = { 0 };
	font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	font_info.nFont = 0;
	font_info.dwFontSize.X = 0;				// 宽度为0则由系统自动匹配
	font_info.dwFontSize.Y = font_size;		// 高度
	font_info.FontFamily = FF_DONTCARE;
	font_info.FontWeight = FW_NORMAL;
	wcscpy_s(font_info.FaceName, LF_FACESIZE, L"新宋体"); // 默认使用新宋体

	/* 如果指定了字体名称，则转换并设置 */
	if (font_name != NULL && font_name[0] != '\0') {
		int len = MultiByteToWideChar(CP_ACP, 0, font_name, -1, NULL, 0);
		if (len > 0 && len <= LF_FACESIZE) {
			MultiByteToWideChar(CP_ACP, 0, font_name, -1, font_info.FaceName, len);
		}
	}

	SetCurrentConsoleFontEx(hout, FALSE, &font_info);
}

/***************************************************************************
  函数名称：cct_setconsoleborder
  功    能：设置控制台屏幕缓冲区大小（行列数）
  输入参数：const int cols：列数（字符宽度）
			const int rows：行数（字符高度）
  返 回 值：无
  说    明：同时调整屏幕缓冲区和窗口大小
***************************************************************************/
void cct_setconsoleborder(const int cols, const int rows)
{
	COORD coord;
	coord.X = (SHORT)cols;
	coord.Y = (SHORT)rows;

	/* 先设置屏幕缓冲区大小 */
	SetConsoleScreenBufferSize(hout, coord);

	/* 再设置窗口大小 */
	SMALL_RECT rect;
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = (SHORT)(cols - 1);
	rect.Bottom = (SHORT)(rows - 1);
	SetConsoleWindowInfo(hout, TRUE, &rect);
}

/***************************************************************************
  函数名称：cct_getconsolesize
  功    能：获取当前控制台屏幕缓冲区的列数和行数
  输入参数：int* cols：输出参数，列数
			int* rows：输出参数，行数
  返 回 值：无
  说    明：
***************************************************************************/
void cct_getconsolesize(int* cols, int* rows)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hout, &binfo);
	if (cols) *cols = binfo.dwSize.X;
	if (rows) *rows = binfo.dwSize.Y;
}

/***************************************************************************
  函数名称：cct_getconsolewidth
  功    能：获取当前控制台屏幕缓冲区的宽度（列数）
  输入参数：无
  返 回 值：int 列数
  说    明：
***************************************************************************/
int cct_getconsolewidth()
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hout, &binfo);
	return binfo.dwSize.X;
}

/***************************************************************************
  函数名称：cct_getconsoleheight
  功    能：获取当前控制台屏幕缓冲区的高度（行数）
  输入参数：无
  返 回 值：int 行数
  说    明：
***************************************************************************/
int cct_getconsoleheight()
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(hout, &binfo);
	return binfo.dwSize.Y;
}

/***************************************************************************
  函数名称：cct_kbhit
  功    能：检测当前是否有键盘按键被按下
  输入参数：无
  返 回 值：bool：true表示有按键，false表示没有
  说    明：非阻塞函数，常用于游戏循环中检测输入
***************************************************************************/
bool cct_kbhit()
{
	return _kbhit() != 0;
}

/***************************************************************************
  函数名称：cct_getch
  功    能：从控制台读取一个字符，不在屏幕上回显
  输入参数：无
  返 回 值：int 读取到的字符ASCII码
  说    明：阻塞等待直到有按键按下，常用于游戏输入
***************************************************************************/
int cct_getch()
{
	return _getch();
}

/***************************************************************************
  函数名称：cct_delay
  功    能：延时指定的毫秒数
  输入参数：const int ms：延时的毫秒数
  返 回 值：无
  说    明：基于Windows Sleep实现
***************************************************************************/
void cct_delay(const int ms)
{
	Sleep(ms);
}
