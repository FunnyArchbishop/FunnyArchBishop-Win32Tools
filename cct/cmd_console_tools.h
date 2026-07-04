#pragma once

#include <Windows.h>

/* ============================ 光标相关 ============================ */
/* 清除屏幕（使用当前背景色填充整个缓冲区，光标回到(0,0)） */
void cct_cls();

/* 将光标移动到指定位置，左上角为(0,0) */
void cct_gotoxy(const int X, const int Y);

/* 设置光标是否可见 */
void cct_setcursor(bool visible);

/* 获取当前光标位置 */
COORD cct_getcursorpos();

/* 获取当前光标X坐标 */
int cct_wherex();

/* 获取当前光标Y坐标 */
int cct_wherey();

/* ============================ 颜色相关 ============================ */
/* 设置控制台文字的前景色和背景色（使用Windows控制台颜色属性） */
void cct_setcolor(const int back_color, const int fore_color);

/* 用指定字符和颜色属性填充整个屏幕缓冲区 */
void cct_fillscreen(const TCHAR ch = ' ', const WORD attr = 0x07);

/* ============================ 窗口相关 ============================ */
/* 设置控制台窗口标题 */
void cct_settitle(const char* title);

/* 设置控制台字体名称和大小 */
void cct_setfontsize(const char* font_name, const int font_size);

/* 设置控制台屏幕缓冲区大小（按字符行列） */
void cct_setconsoleborder(const int cols, const int rows);

/* 获取控制台屏幕缓冲区的列数和行数 */
void cct_getconsolesize(int* cols, int* rows);

/* 获取控制台屏幕缓冲区宽度（列数） */
int cct_getconsolewidth();

/* 获取控制台屏幕缓冲区高度（行数） */
int cct_getconsoleheight();

/* ============================ 输入相关 ============================ */
/* 检测是否有按键按下（不阻塞） */
bool cct_kbhit();

/* 从控制台读取一个字符（不回显），返回ASCII码 */
int cct_getch();

/* 延时指定毫秒数 */
void cct_delay(const int ms);
