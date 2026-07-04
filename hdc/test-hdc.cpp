#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <conio.h>     
#include <Windows.h>   
using namespace std;

#include "cmd_hdc_tools.h"
#include "cmd_console_tools.h"

/***************************************************************************
  函数名称：to_be_continued
  功    能：显示提示信息，等待回车键后继续
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void to_be_continued(const char prompt[], const int pos_x = 0, const int pos_y = 22)
{
	cct_gotoxy(pos_x, pos_y);
	
	//因为每次都从22行0列，如果本次打印内容比上次短，后面会有残留，因此先用100个空格填充掉（未做准确判断，简单假设打印内容不超过100）
	cout << setw(100) << ' ' << '\r'; //'\r'表示回到本行开头，不换行
	cout << prompt << "，按回车键继续...";

	/* 忽略除回车键外的所有输入（注意：_getch的回车是\r，而getchar是\n）*/
	while (_getch() != '\r')
		;

	return;
}

/***************************************************************************
  函数名称：main
  功    能：测试入口
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	const int   win_width = 1400, win_high = 900;	//设定屏幕宽度
	const int   win_bgcolor = 7, win_fgcolor = 0;
	const int   color[] = { 0x00FF0000, 0x0000FF00, 0x000000FF }; //R G B

	cct_settitle("HDC & CCT Tools Demo");  // 设置窗口标题
	hdc_init(win_bgcolor, win_fgcolor, win_width, win_high);		//用(背景色，前景色，宽度，高度）初始化窗口
	hdc_cls();

	// ========== 第一部分：原有功能测试 ==========
	to_be_continued("1、正常图形显示(先调整cmd窗口字体为新宋体16点阵，屏幕大小30行*120列以上)");
	if (1) {
		/* 正常矩形 */
		hdc_rectangle(100, 101, 400, 20, color[0]);
		hdc_rectangle(100, 121, 400, 20, color[1]);
		hdc_rectangle(100, 141, 400, 20, color[2]);
		to_be_continued("宽度400，高度20的三个矩形显示完成");

		/* 横向的直线，高度为1/2/3 */
		hdc_rectangle(100, 200, 400, 1, color[1]);
		hdc_rectangle(100, 210, 400, 2, color[1]);
		hdc_rectangle(100, 220, 400, 3, color[1]);
		to_be_continued("宽度400，高度1/2/3的横线显示完成");

		/* 纵向的直线，宽度为1/2/3 */
		hdc_rectangle(520, 101, 1, 60, color[1]);
		hdc_rectangle(530, 101, 2, 60, color[1]);
		hdc_rectangle(540, 101, 3, 60, color[1]);
		to_be_continued("宽度1/2/3，高度60的竖线显示完成");

		/* 7个1*1/1*2/2*1/2*2的点 */
		hdc_rectangle(520, 210, 1, 1, color[1]);
		hdc_rectangle(520, 214, 1, 2, color[1]);
		hdc_rectangle(524, 210, 1, 2, color[1]);
		hdc_rectangle(520, 218, 2, 1, color[1]);
		hdc_rectangle(528, 210, 2, 1, color[1]);
		hdc_rectangle(520, 222, 2, 2, color[1]);
		hdc_rectangle(532, 210, 2, 2, color[1]);
		to_be_continued("1*1/1*2/2*1/2*2的7个点显示完成");

		/* 6个1*3/3*1/3*3的点 */
		hdc_rectangle(520, 226, 1, 3, color[1]);
		hdc_rectangle(536, 210, 1, 3, color[1]);
		hdc_rectangle(520, 230, 3, 1, color[1]);
		hdc_rectangle(540, 210, 3, 1, color[1]);
		hdc_rectangle(520, 234, 3, 3, color[1]);
		hdc_rectangle(544, 210, 3, 3, color[1]);
		to_be_continued("1*3/3*1/3*3的6个点显示完成");
	}

	// ========== 第二部分：新增CCT功能测试 ==========
	{
		cct_cls();
		hdc_cls();

		cct_gotoxy(0, 0);
		cout << "===== 新增 CCT 工具函数测试 =====" << endl;

		// 测试光标隐藏
		to_be_continued("准备隐藏光标");
		cct_setcursor(false);
		cout << "\n光标已隐藏（你看不到闪烁的光标了）" << endl;
		to_be_continued("光标隐藏测试完成");

		// 测试光标显示与位置
		cct_setcursor(true);
		cct_gotoxy(10, 5);
		cout << "光标在(10,5)";
		cct_gotoxy(0, 6);
		cout << "cct_wherex()=" << cct_wherex() << " (当前在行首，x应为0)";
		cct_gotoxy(0, 7);
		int cx = cct_wherex(), cy = cct_wherey();
		cout << "cct_wherey()=" << cy << " (应为7)";
		to_be_continued("光标位置测试完成");

		// 测试颜色设置
		cct_gotoxy(0, 9);
		cct_setcolor(0, 12); // 黑底红字
		cout << "黑底红字 ";
		cct_setcolor(0, 10); // 黑底绿字
		cout << "黑底绿字 ";
		cct_setcolor(0, 9);  // 黑底蓝字
		cout << "黑底蓝字 ";
		cct_setcolor(7, 0);  // 恢复默认
		cout << "灰底黑字（默认）";
		to_be_continued("颜色设置测试完成");

		// 测试fillscreen
		cct_setcolor(7, 0);
		to_be_continued("准备用cct_fillscreen填充整个屏幕");
		cct_fillscreen('*', 0x1E); // 黄底蓝字
		cct_gotoxy(30, 10);
		cout << "屏幕已被 '*' 字符填充（黄底蓝字）";
		to_be_continued("fillscreen测试完成");

		// 测试console尺寸获取
		cct_cls();
		hdc_cls();
		cct_gotoxy(0, 0);
		int cw, ch;
		cct_getconsolesize(&cw, &ch);
		cout << "控制台缓冲区大小：" << cw << " 列 x " << ch << " 行" << endl;
		cout << "cct_getconsolewidth() = " << cct_getconsolewidth() << endl;
		cout << "cct_getconsoleheight() = " << cct_getconsoleheight() << endl;
		to_be_continued("控制台尺寸获取测试完成");

		// 测试delay
		cct_gotoxy(0, 4);
		cout << "下面演示cct_delay延时效果：";
		for (int i = 1; i <= 5; i++) {
			cct_gotoxy(30, 4);
			cout << i << "/5  ";
			cct_delay(500); // 延时500ms
		}
		to_be_continued("延时测试完成");
	}

	// ========== 第三部分：原有图形文字混合测试（保持兼容） ==========
	if (1) {
		cct_cls();
		hdc_cls();

		cct_gotoxy(0, 0);
		cout << "2、图形文字混合：先文字，再图形，再文字" << endl; //0行
		cout << "\t2-1、先上部显示文字" << endl;
		cout << "\t2-2、下部显示图形" << endl;
		cout << "\t2-3、再上部显示文字（不跨越图形区）" << endl;
		to_be_continued("准备开始", 0, 4);

		cct_gotoxy(10, 5);
		cout << "Hello" << endl;
		Sleep(1000);
		hdc_rectangle(100, 200, 400, 60, color[0]);
		Sleep(1000);
		cct_gotoxy(20, 6);
		cout << "World" << endl;
		to_be_continued("测试完成", 0, 7);
	}

	if (1) {
		cct_cls();
		hdc_cls();

		cct_gotoxy(0, 10);
		cout << "3、图形文字混合：先文字，再图形，再文字" << endl; //10行
		cout << "\t3-1、先下部显示文字" << endl;
		cout << "\t3-2、上部显示图形" << endl;
		cout << "\t3-3、再下部显示文字（不跨越图形区）" << endl;
		to_be_continued("准备开始", 0, 14);

		cct_gotoxy(10, 15);
		cout << "Hello" << endl;
		Sleep(1000);
		hdc_rectangle(100, 50, 400, 60, color[1]);
		Sleep(1000);
		cct_gotoxy(20, 16);
		cout << "World" << endl;
		to_be_continued("测试完成", 0, 17);
	}

	if (1) {
		cct_cls();
		hdc_cls();

		cct_gotoxy(0, 0);
		cout << "4、图形文字混合：先文字，再图形，再文字" << endl; //0行
		cout << "\t4-1、先显示文字" << endl;
		cout << "\t4-2、和文字重叠的部分显示图形" << endl;
		cout << "\t4-3、再图形重叠位置显示文字" << endl;
		to_be_continued("准备开始", 0, 4);

		cct_gotoxy(0, 8);
		cout << "Hello";
		Sleep(1000); //停顿1秒，方便看清
		hdc_rectangle(100, 101, 400, 80, color[1]);
		Sleep(1000); //停顿1秒，方便看清
		cout << "World" << endl; //在上次光标的位置后面继续显示
		to_be_continued("测试完成");
	}

	if (1) {
		cct_cls();
		hdc_cls();

		cct_gotoxy(0, 0);
		cout << "5、图形文字混合：先文字，再图形，再文字" << endl; //0行
		cout << "\t5-1、先上部显示文字" << endl;
		cout << "\t5-2、中部显示图形" << endl;
		cout << "\t5-3、再下部显示文字（光标跨越图形区）" << endl;
		to_be_continued("准备开始", 0, 4);

		cct_gotoxy(0, 5);
		cout << "Hello";
		Sleep(1000); //停顿1秒，方便看清
		hdc_rectangle(100, 120, 400, 80, color[1]);
		Sleep(1000); //停顿1秒，方便看清
		cct_gotoxy(20, 15);
		cout << "World";
		Sleep(1000); //停顿1秒，方便看清
		to_be_continued("测试完成", 0, 20);
	}

	// ========== 第四部分：新增HDC图形功能测试 ==========
	{
		cct_cls();
		hdc_cls();
		cct_setcursor(false);
		cct_gotoxy(0, 0);
		cout << "===== 新增 HDC 绘图函数测试 =====" << endl;

		// 测试像素点
		to_be_continued("6、测试 hdc_pixel 画像素点");
		for (int i = 0; i < 200; i++) {
			int x = rand() % 800 + 100;
			int y = rand() % 400 + 100;
			hdc_pixel(x, y, RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		to_be_continued("200个随机彩色像素点绘制完成");

		// 测试直线
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(3);
		to_be_continued("7、测试 hdc_line 画直线（宽度3）");
		hdc_line(100, 100, 500, 100, color[0]);      // 水平线
		hdc_line(100, 150, 500, 300, color[1]);      // 斜线
		hdc_line(100, 200, 500, 200, color[2]);      // 水平线
		hdc_line(300, 50, 300, 350, RGB(255, 0, 255)); // 竖线（紫色）
		to_be_continued("直线测试完成");

		// 测试虚线样式
		hdc_set_penstyle(PS_DASH);
		hdc_set_pencolor(RGB(255, 128, 0));
		hdc_line(550, 100, 800, 100);
		hdc_set_penstyle(PS_DOT);
		hdc_set_pencolor(RGB(0, 128, 255));
		hdc_line(550, 130, 800, 130);
		hdc_set_penstyle(PS_DASHDOT);
		hdc_set_pencolor(RGB(128, 0, 255));
		hdc_line(550, 160, 800, 160);
		hdc_set_penstyle(PS_SOLID);
		hdc_set_penwidth(1);
		to_be_continued("虚线/点线/点划线样式测试完成");

		// 测试空心矩形边框
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(2);
		to_be_continued("8、测试 hdc_frame_rect 空心矩形");
		hdc_frame_rect(100, 100, 200, 150, color[0]);
		hdc_frame_rect(350, 100, 200, 150, color[1]);
		hdc_frame_rect(600, 100, 200, 150, color[2]);
		hdc_set_penwidth(1);
		to_be_continued("空心矩形边框测试完成");

		// 测试填充矩形
		cct_cls();
		hdc_cls();
		to_be_continued("9、测试 hdc_fill_rect 填充矩形");
		hdc_fill_rect(100, 100, 200, 150, RGB(255, 200, 100));
		hdc_fill_rect(350, 100, 200, 150, RGB(100, 255, 200));
		hdc_fill_rect(600, 100, 200, 150, RGB(100, 200, 255));
		// 边框+填充对比
		hdc_set_penwidth(3);
		hdc_set_pencolor(RGB(0, 0, 0));
		hdc_frame_rect(100, 100, 200, 150);
		hdc_frame_rect(350, 100, 200, 150);
		hdc_frame_rect(600, 100, 200, 150);
		hdc_set_penwidth(1);
		to_be_continued("填充矩形测试完成");

		// 测试圆
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(2);
		to_be_continued("10、测试 hdc_circle 空心圆");
		hdc_circle(200, 200, 80, color[0]);
		hdc_circle(450, 200, 100, color[1]);
		hdc_circle(750, 200, 120, color[2]);
		hdc_set_penwidth(1);
		to_be_continued("空心圆测试完成");

		// 测试填充圆
		cct_cls();
		hdc_cls();
		to_be_continued("11、测试 hdc_filled_circle 填充圆");
		hdc_filled_circle(200, 200, 80, RGB(255, 150, 150));
		hdc_filled_circle(450, 200, 100, RGB(150, 255, 150));
		hdc_filled_circle(750, 200, 120, RGB(150, 150, 255));
		// 加边框
		hdc_set_penwidth(2);
		hdc_circle(200, 200, 80, RGB(200, 0, 0));
		hdc_circle(450, 200, 100, RGB(0, 200, 0));
		hdc_circle(750, 200, 120, RGB(0, 0, 200));
		hdc_set_penwidth(1);
		to_be_continued("填充圆测试完成");

		// 测试椭圆
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(2);
		to_be_continued("12、测试 hdc_ellipse 椭圆");
		hdc_ellipse(100, 100, 350, 300, color[0]);     // 横椭圆
		hdc_ellipse(400, 100, 550, 400, color[1]);     // 竖椭圆
		hdc_filled_ellipse(600, 100, 850, 250, RGB(200, 200, 100)); // 填充椭圆
		hdc_set_penwidth(1);
		to_be_continued("椭圆测试完成");

		// 测试三角形
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(2);
		to_be_continued("13、测试 hdc_triangle 三角形");
		hdc_triangle(200, 350, 100, 150, 300, 150, color[0]);          // 正三角形
		hdc_triangle(450, 350, 350, 200, 550, 150, color[1]);          // 不等边
		hdc_filled_triangle(650, 350, 550, 150, 750, 200, RGB(255, 200, 255)); // 填充三角形
		hdc_set_penwidth(1);
		to_be_continued("三角形测试完成");

		// 测试圆角矩形
		cct_cls();
		hdc_cls();
		hdc_set_penwidth(2);
		to_be_continued("14、测试 hdc_roundrect 圆角矩形");
		hdc_roundrect(100, 100, 400, 300, 30, 30, color[0]);
		hdc_roundrect(500, 100, 800, 300, 50, 30, color[1]);
		hdc_roundrect(100, 350, 800, 500, 20, 20, color[2]);
		hdc_set_penwidth(1);
		to_be_continued("圆角矩形测试完成");

		// 测试扇形和弦
		cct_cls();
		hdc_cls();
		to_be_continued("15、测试 hdc_pie 扇形图");
		hdc_pie(100, 100, 500, 500, 500, 300, 100, 300, RGB(255, 100, 100));
		hdc_pie(100, 100, 500, 500, 100, 300, 300, 100, RGB(100, 255, 100));
		hdc_pie(100, 100, 500, 500, 300, 100, 500, 300, RGB(100, 100, 255));
		to_be_continued("扇形图测试完成");

		// 测试hdc_textout
		cct_cls();
		hdc_cls();
		to_be_continued("16、测试 hdc_textout GDI文字输出");
		hdc_textout(100, 100, "Hello from HDC TextOut!", RGB(255, 0, 0));
		hdc_textout(100, 130, "This is drawn with GDI", RGB(0, 128, 0));
		hdc_textout(100, 160, "支持大字体，不依赖控制台字符网格", RGB(0, 0, 255));
		hdc_textout(100, 200, "可以和控制台cout文字共存", RGB(128, 0, 128));
		to_be_continued("GDI文字输出测试完成");
	}

	// ========== 第五部分：综合演示（动画） ==========
	{
		cct_cls();
		hdc_cls();
		cct_setcursor(false);

		to_be_continued("17、综合动画演示：移动的小球");

		// 动画循环：一个彩色小球在屏幕中弹跳
		int ball_x = 400, ball_y = 300;
		int ball_r = 30;
		int dx = 3, dy = 2;
		int max_x = hdc_get_width(), max_y = hdc_get_height();

		for (int frame = 0; frame < 300; frame++) {
			// 检查按键退出
			if (cct_kbhit()) {
				int key = cct_getch();
				if (key == 27) break; // ESC退出
			}

			// 用背景色擦除旧位置
			hdc_filled_circle(ball_x, ball_y, ball_r, RGB(255, 255, 255));

			// 更新位置
			ball_x += dx;
			ball_y += dy;

			// 边界碰撞检测
			if (ball_x - ball_r <= 0 || ball_x + ball_r >= max_x) dx = -dx;
			if (ball_y - ball_r <= 0 || ball_y + ball_r >= max_y) dy = -dy;

			// 画新位置（颜色随位置变化）
			int r = (ball_x * 255) / max_x;
			int g = (ball_y * 255) / max_y;
			int b = 255 - r;
			hdc_filled_circle(ball_x, ball_y, ball_r, RGB(r, g, b));
			hdc_circle(ball_x, ball_y, ball_r, RGB(0, 0, 0));

			cct_delay(10);
		}

		to_be_continued("动画演示结束");
	}

	to_be_continued("演示全部结束，记住结论：画图区域和文字输出区域不要有任何重叠，包括移动光标.\n\t注：如果测试5的图形没有消除，多跑几遍");
	cout << endl;

	hdc_release();
	cct_setcursor(true);

	return 0;
}
