#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "cmd_hdc_tools.h"
#include "cmd_console_tools.h"
//using namespace std;

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static int _BgColor_, _FgColor_, _Width_, _High;

/* 画笔与画刷相关的静态全局变量 */
static HPEN  _hPen_ = NULL;			// 当前画笔句柄
static HBRUSH _hBrush_ = NULL;		// 当前画刷句柄
static int   _PenRGB_ = RGB(0, 0, 0);	// 当前画笔颜色
static int   _PenWidth_ = 1;		// 当前画笔宽度
static int   _PenStyle_ = PS_SOLID;	// 当前画笔样式
static int   _BrushRGB_ = RGB(255, 255, 255); // 当前画刷颜色

/* ---- 内部辅助函数：应用当前画笔到DC ---- */
static void hdc_apply_pen()
{
	if (_hPen_) DeleteObject(_hPen_);
	_hPen_ = CreatePen(_PenStyle_, _PenWidth_, _PenRGB_);
	SelectObject(hdc, _hPen_);
}

/* ---- 内部辅助函数：应用当前画刷到DC ---- */
static void hdc_apply_brush()
{
	if (_hBrush_) DeleteObject(_hBrush_);
	_hBrush_ = CreateSolidBrush(_BrushRGB_);
	SelectObject(hdc, _hBrush_);
}

/***************************************************************************
  函数名称：hdc_base_line
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
		   const int y1：起点y坐标，左上角为(0,0)
		   const int x2：终点y坐标，左上角为(0,0)
		   const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static void hdc_base_line(int x1, int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2 + 1, y2 + 1);
#if 0
	if (x1 == x2 && y1 == y2) { //一个点的情况
		LineTo(hdc, x2 + 1, y2 + 1);
	}
	else if (x1 == x2 && abs(y1 - y2) == 1) { // 上下两个点的情况
		LineTo(hdc, x2 + 1, y2 + 1);
	}
	else if (abs(x1-x2)==1 && y1 == y2) {	//左右两个点的情况
		LineTo(hdc, x2 + 1, y2 + 1);
	}
	else
		LineTo(hdc, x2 + 1, y2 + 1);
#endif
}

/***************************************************************************
  函数名称：hdc_init
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	/* 初始化画笔与画刷 */
	_PenRGB_ = fgcolor;
	_PenWidth_ = 1;
	_PenStyle_ = PS_SOLID;
	_BrushRGB_ = bgcolor;
	hdc_apply_pen();
	hdc_apply_brush();
}

/***************************************************************************
  函数名称：hdc_release
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	/* 释放GDI对象 */
	if (_hPen_) {
		DeleteObject(_hPen_);
		_hPen_ = NULL;
	}
	if (_hBrush_) {
		DeleteObject(_hBrush_);
		_hBrush_ = NULL;
	}

	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
	}
}

/***************************************************************************
  函数名称：hdc_set_pencolor
  功    能：设置画笔颜色，传入RGB值
  输入参数：const int RGB_value：RGB颜色值
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	_PenRGB_ = RGB_value;
	hdc_apply_pen();
}

/***************************************************************************
  函数名称：hdc_set_pencolor
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：unsigned char red, green, blue
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：hdc_set_brushcolor
  功    能：设置画刷（填充）颜色
  输入参数：const int rgb_value：RGB颜色值
  返 回 值：无
  说    明：影响后续所有填充类绘制函数
***************************************************************************/
void hdc_set_brushcolor(const int rgb_value)
{
	_BrushRGB_ = rgb_value;
	hdc_apply_brush();
}

/***************************************************************************
  函数名称：hdc_set_penwidth
  功    能：设置画笔宽度（像素）
  输入参数：const int width：画笔宽度，>=1
  返 回 值：无
  说    明：影响后续所有线条绘制函数
***************************************************************************/
void hdc_set_penwidth(const int width)
{
	_PenWidth_ = (width < 1) ? 1 : width;
	hdc_apply_pen();
}

/***************************************************************************
  函数名称：hdc_set_penstyle
  功    能：设置画笔样式
  输入参数：const int style：样式，如PS_SOLID, PS_DASH, PS_DOT等
  返 回 值：无
  说    明：常用样式：PS_SOLID(实线), PS_DASH(虚线), PS_DOT(点线),
			PS_DASHDOT(点划线), PS_DASHDOTDOT(双点划线), PS_NULL(不可见)
***************************************************************************/
void hdc_set_penstyle(const int style)
{
	_PenStyle_ = style;
	hdc_apply_pen();
}

/***************************************************************************
  函数名称：hdc_cls
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
	/* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_ + 1) % 16, _Width_, _High);

	/* 在hdc_init中会修改_FgColor_的值，因此先+1，再+15，正好保持hdc_cls前的颜色 */
	hdc_init(_BgColor_, (_FgColor_ + 15) % 16, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：hdc_pixel
  功    能：在指定坐标画一个像素点
  输入参数：const int x,y    ：像素坐标
			const int RGB_value：颜色（INVALID_RGB则使用当前画笔颜色）
  返 回 值：无
  说    明：
***************************************************************************/
void hdc_pixel(const int x, const int y, const int RGB_value)
{
	int color = (RGB_value == INVALID_RGB) ? _PenRGB_ : RGB_value;
	SetPixel(hdc, x, y, color);
}

/***************************************************************************
  函数名称：hdc_line
  功    能：画一条从(x1,y1)到(x2,y2)的直线
  输入参数：const int x1,y1    ：起点坐标
			const int x2,y2    ：终点坐标
			const int RGB_value：颜色（INVALID_RGB则使用当前画笔颜色）
  返 回 值：无
  说    明：
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

/***************************************************************************
  函数名称：hdc_rectangle
  功    能：给出左上角坐标及宽度、高度，画出一个填充长方形
  输入参数：const int x1,y1       ：左上角坐标
		   const int width,high  ：宽度和高度
		   const int RGB_value   ：颜色（INVALID_RGB则不改变）
  返 回 值：
  说    明：原有函数，用逐行画线方式填充矩形
***************************************************************************/
void hdc_rectangle(const int x1, const int y1, const int width, const int high, const int RGB_value)
{
	const int x2 = x1 + (width < 1 ? 1 : width) - 1;	//最小宽度为1
	const int y2 = y1 + (high < 1 ? 1 : high) - 1;		//最小高度为1

	/* 如果坐标没有在 hdc_init 初始化的区间内，则直接返回 */
	if (x1 < 0 || x1 >= _Width_ || x2 < 0 || x2 >= _Width_ || y1 < 0 || y1 >= _High || y2 < 0 || y2 >= _High)
		return;

	/* 如果给了新的颜色，则需要设置 */
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

#if 1
	for (int y = y1; y <= y2; y++) {
		MoveToEx(hdc, x1, y, NULL);
		LineTo(hdc, x2 + 1, y);
	}
#elif 0
	if (abs(x1-x2)<=abs(y1-y2)) {
		/* 用y1~y2条竖线模拟画出矩形 */
		for (int i = x1; i <= x2; i++) {
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2 + 1, y2 + 1);
		}
	}
	else {
		/* 用y1~y2条横线模拟画出矩形 */
		for (int i = y1; i <= y2; i++) {
			MoveToEx(hdc, x1, y1, NULL);
			LineTo(hdc, x2 + 1, y2 + 1);
		}
	}
#elif 0
	/* 如果x1==x2，此时矩形变为一个宽度为1的直线，要特殊处理 */
	if (x1 == x2) {
		hdc_base_line(x1, y1, x2, y2);
	}
	else {
		/* 用y1~y2条横线模拟画出矩形 */
		for (int i = y1; i <= y2; i++)
			hdc_base_line(x1, i, x2, i);
	}
#endif
}

/***************************************************************************
  函数名称：hdc_frame_rect
  功    能：画一个空心矩形（只画边框，不填充内部）
  输入参数：const int x1,y1       ：左上角坐标
		   const int width,high  ：宽度和高度
		   const int RGB_value   ：颜色
  返 回 值：无
  说    明：使用GDI Rectangle + NULL_BRUSH实现
***************************************************************************/
void hdc_frame_rect(const int x1, const int y1, const int width, const int high, const int RGB_value)
{
	const int x2 = x1 + width;
	const int y2 = y1 + high;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 选中空画刷，只画边框 */
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, x1, y1, x2, y2);
	SelectObject(hdc, oldBrush);
}

/***************************************************************************
  函数名称：hdc_fill_rect
  功    能：画一个填充矩形（使用当前画刷颜色填充）
  输入参数：const int x1,y1       ：左上角坐标
		   const int width,high  ：宽度和高度
		   const int RGB_value   ：填充颜色
  返 回 值：无
  说    明：使用GDI Rectangle实现，用画刷填充
***************************************************************************/
void hdc_fill_rect(const int x1, const int y1, const int width, const int high, const int RGB_value)
{
	const int x2 = x1 + width;
	const int y2 = y1 + high;

	if (RGB_value != INVALID_RGB)
		hdc_set_brushcolor(RGB_value);

	/* 选中不画边框的画笔 */
	HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Rectangle(hdc, x1, y1, x2, y2);
	SelectObject(hdc, oldPen);
}

/***************************************************************************
  函数名称：hdc_circle
  功    能：画一个空心圆
  输入参数：const int cx,cy       ：圆心坐标
		   const int radius      ：半径（像素）
		   const int RGB_value   ：颜色
  返 回 值：无
  说    明：使用GDI Ellipse实现（正方形外接矩形的内切椭圆即为圆）
***************************************************************************/
void hdc_circle(const int cx, const int cy, const int radius, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, cx - radius, cy - radius, cx + radius, cy + radius);
	SelectObject(hdc, oldBrush);
}

/***************************************************************************
  函数名称：hdc_filled_circle
  功    能：画一个填充圆
  输入参数：const int cx,cy       ：圆心坐标
		   const int radius      ：半径（像素）
		   const int RGB_value   ：填充颜色
  返 回 值：无
  说    明：
***************************************************************************/
void hdc_filled_circle(const int cx, const int cy, const int radius, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_brushcolor(RGB_value);

	HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Ellipse(hdc, cx - radius, cy - radius, cx + radius, cy + radius);
	SelectObject(hdc, oldPen);
}

/***************************************************************************
  函数名称：hdc_ellipse
  功    能：画一个空心椭圆
  输入参数：const int x1,y1    ：外接矩形左上角
			const int x2,y2    ：外接矩形右下角
			const int RGB_value：颜色
  返 回 值：无
  说    明：
***************************************************************************/
void hdc_ellipse(const int x1, const int y1, const int x2, const int y2, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Ellipse(hdc, x1, y1, x2, y2);
	SelectObject(hdc, oldBrush);
}

/***************************************************************************
  函数名称：hdc_filled_ellipse
  功    能：画一个填充椭圆
  输入参数：const int x1,y1    ：外接矩形左上角
			const int x2,y2    ：外接矩形右下角
			const int RGB_value：填充颜色
  返 回 值：无
  说    明：
***************************************************************************/
void hdc_filled_ellipse(const int x1, const int y1, const int x2, const int y2, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_brushcolor(RGB_value);

	HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Ellipse(hdc, x1, y1, x2, y2);
	SelectObject(hdc, oldPen);
}

/***************************************************************************
  函数名称：hdc_triangle
  功    能：画一个空心三角形
  输入参数：三个顶点坐标(x1,y1),(x2,y2),(x3,y3)
			const int RGB_value：颜色
  返 回 值：无
  说    明：使用GDI Polygon实现
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	POINT pts[3] = { {x1, y1}, {x2, y2}, {x3, y3} };

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Polygon(hdc, pts, 3);
	SelectObject(hdc, oldBrush);
}

/***************************************************************************
  函数名称：hdc_filled_triangle
  功    能：画一个填充三角形
  输入参数：三个顶点坐标(x1,y1),(x2,y2),(x3,y3)
			const int RGB_value：填充颜色
  返 回 值：无
  说    明：使用GDI Polygon实现
***************************************************************************/
void hdc_filled_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_brushcolor(RGB_value);

	POINT pts[3] = { {x1, y1}, {x2, y2}, {x3, y3} };

	HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
	Polygon(hdc, pts, 3);
	SelectObject(hdc, oldPen);
}

/***************************************************************************
  函数名称：hdc_roundrect
  功    能：画一个圆角矩形
  输入参数：const int x1,y1         ：外接矩形左上角
			const int x2,y2         ：外接矩形右下角
			const int corner_w,h    ：圆角的宽度和高度
			const int RGB_value     ：颜色
  返 回 值：无
  说    明：使用GDI RoundRect实现
***************************************************************************/
void hdc_roundrect(const int x1, const int y1, const int x2, const int y2, const int corner_w, const int corner_h, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	RoundRect(hdc, x1, y1, x2, y2, corner_w, corner_h);
	SelectObject(hdc, oldBrush);
}

/***************************************************************************
  函数名称：hdc_arc
  功    能：画一段椭圆弧
  输入参数：const int x1,y1    ：外接矩形左上角
			const int x2,y2    ：外接矩形右下角
			const int x3,y3    ：起始点（定义弧的起点方向）
			const int x4,y4    ：终止点（定义弧的终点方向）
			const int RGB_value：颜色
  返 回 值：无
  说    明：弧从(x3,y3)沿逆时针方向画到(x4,y4)
***************************************************************************/
void hdc_arc(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value)
{
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	Arc(hdc, x1, y1, x2, y2, x3, y3, x4, y4);
}

/***************************************************************************
  函数名称：hdc_pie
  功    能：画一个扇形（饼图的一瓣）
  输入参数：const int x1,y1    ：外接矩形左上角
			const int x2,y2    ：外接矩形右下角
			const int x3,y3    ：起始点
			const int x4,y4    ：终止点
			const int RGB_value：颜色
  返 回 值：无
  说    明：使用当前画刷填充扇形区域
***************************************************************************/
void hdc_pie(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value)
{
	if (RGB_value != INVALID_RGB) {
		hdc_set_pencolor(RGB_value);
		hdc_set_brushcolor(RGB_value);
	}

	Pie(hdc, x1, y1, x2, y2, x3, y3, x4, y4);
}

/***************************************************************************
  函数名称：hdc_chord
  功    能：画一条弦（椭圆弧+连接弧两端点的线段围成的区域）
  输入参数：const int x1,y1    ：外接矩形左上角
			const int x2,y2    ：外接矩形右下角
			const int x3,y3    ：起始点
			const int x4,y4    ：终止点
			const int RGB_value：颜色
  返 回 值：无
  说    明：使用当前画刷填充弦区域
***************************************************************************/
void hdc_chord(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value)
{
	if (RGB_value != INVALID_RGB) {
		hdc_set_pencolor(RGB_value);
		hdc_set_brushcolor(RGB_value);
	}

	Chord(hdc, x1, y1, x2, y2, x3, y3, x4, y4);
}

/***************************************************************************
  函数名称：hdc_textout
  功    能：在指定坐标输出文字
  输入参数：const int x,y        ：输出起始坐标
			const char* text     ：要输出的文字（ANSI字符串）
			const int RGB_value  ：文字颜色
  返 回 值：无
  说    明：使用GDI TextOutA输出文字，坐标对应左上角
***************************************************************************/
void hdc_textout(const int x, const int y, const char* text, const int RGB_value)
{
	if (text == NULL || hdc == NULL) return;

	if (RGB_value != INVALID_RGB)
		SetTextColor(hdc, RGB_value);

	/* 设置背景为透明模式 */
	int oldBkMode = SetBkMode(hdc, TRANSPARENT);
	TextOutA(hdc, x, y, text, (int)strlen(text));
	SetBkMode(hdc, oldBkMode);
}

/***************************************************************************
  函数名称：hdc_get_width
  功    能：获取当前绘图区域的宽度（像素）
  输入参数：无
  返 回 值：int 宽度
  说    明：
***************************************************************************/
int hdc_get_width()
{
	return _Width_;
}

/***************************************************************************
  函数名称：hdc_get_height
  功    能：获取当前绘图区域的高度（像素）
  输入参数：无
  返 回 值：int 高度
  说    明：
***************************************************************************/
int hdc_get_height()
{
	return _High;
}

/***************************************************************************
  函数名称：hdc_get_bgcolor
  功    能：获取当前设置的背景色
  输入参数：无
  返 回 值：int 背景色RGB值
  说    明：
***************************************************************************/
int hdc_get_bgcolor()
{
	return _BgColor_;
}

/***************************************************************************
  函数名称：hdc_get_fgcolor
  功    能：获取当前设置的前景色
  输入参数：无
  返 回 值：int 前景色RGB值
  说    明：
***************************************************************************/
int hdc_get_fgcolor()
{
	return _FgColor_;
}
