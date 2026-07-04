#pragma once

#include <Windows.h>

#define INVALID_RGB		0x7FFFFFFF

/* ============================ 初始化与释放 ============================ */
/* 初始化绘图环境（具体参数含义参见cpp） */
void hdc_init(const int bgcolor = RGB(255, 255, 255), const int fgcolor = RGB(0, 0, 0), const int width = 8 * 120, const int high = 16 * 30);
/* 释放绘图资源（可重入） */
void hdc_release(void);

/* ============================ 颜色与画笔设置 ============================ */
/* 设置画笔颜色（RGB整型值） */
void hdc_set_pencolor(const int rgb_value);
/* 设置画笔颜色（R/G/B分量，0-255） */
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue);
/* 设置画刷（填充）颜色 */
void hdc_set_brushcolor(const int rgb_value);
/* 设置画笔宽度（像素） */
void hdc_set_penwidth(const int width);
/* 设置画笔样式：PS_SOLID(实线), PS_DASH(虚线), PS_DOT(点线), PS_DASHDOT(点划线)等 */
void hdc_set_penstyle(const int style);

/* ============================ 屏幕清除 ============================ */
/* 清除屏幕（颜色、窗口大小不变） */
void hdc_cls(void);

/* ============================ 基本图形绘制 ============================ */
/* 画单个像素点 */
void hdc_pixel(const int x, const int y, const int RGB_value = INVALID_RGB);

/* 画直线（从(x1,y1)到(x2,y2)） */
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int RGB_value = INVALID_RGB);

/* 画矩形（给定左上角坐标、宽度、高度） */
void hdc_rectangle(const int x1, const int y1, const int width, const int high, const int RGB_value = INVALID_RGB);
/* 画空心矩形边框（内部不填充） */
void hdc_frame_rect(const int x1, const int y1, const int width, const int high, const int RGB_value = INVALID_RGB);
/* 画填充矩形（用当前画刷颜色填充） */
void hdc_fill_rect(const int x1, const int y1, const int width, const int high, const int RGB_value = INVALID_RGB);

/* 画圆（给定圆心坐标和半径） */
void hdc_circle(const int cx, const int cy, const int radius, const int RGB_value = INVALID_RGB);
/* 画填充圆 */
void hdc_filled_circle(const int cx, const int cy, const int radius, const int RGB_value = INVALID_RGB);

/* 画椭圆（给定外接矩形左上角和右下角） */
void hdc_ellipse(const int x1, const int y1, const int x2, const int y2, const int RGB_value = INVALID_RGB);
/* 画填充椭圆 */
void hdc_filled_ellipse(const int x1, const int y1, const int x2, const int y2, const int RGB_value = INVALID_RGB);

/* 画三角形（给定三个顶点坐标） */
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int RGB_value = INVALID_RGB);
/* 画填充三角形 */
void hdc_filled_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int RGB_value = INVALID_RGB);

/* 画圆角矩形（给定外接矩形和圆角宽度、高度） */
void hdc_roundrect(const int x1, const int y1, const int x2, const int y2, const int corner_w, const int corner_h, const int RGB_value = INVALID_RGB);

/* 画圆弧（给定外接矩形和起止点） */
void hdc_arc(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value = INVALID_RGB);

/* 画扇形（饼图，给定外接矩形和起止点） */
void hdc_pie(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value = INVALID_RGB);

/* 画弦（给定外接矩形和起止点） */
void hdc_chord(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int x4, const int y4, const int RGB_value = INVALID_RGB);

/* ============================ 文字输出 ============================ */
/* 在指定位置输出文字（使用当前文本颜色） */
void hdc_textout(const int x, const int y, const char* text, const int RGB_value = INVALID_RGB);

/* ============================ 属性获取 ============================ */
/* 获取当前绘图区域宽度 */
int hdc_get_width();
/* 获取当前绘图区域高度 */
int hdc_get_height();
/* 获取当前背景色 */
int hdc_get_bgcolor();
/* 获取当前前景色 */
int hdc_get_fgcolor();
