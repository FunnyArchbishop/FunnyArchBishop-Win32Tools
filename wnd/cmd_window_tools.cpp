#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "cmd_window_tools.h"

static HINSTANCE g_hinst = NULL;
static HINSTANCE wnd_get_hinst() {
	if (g_hinst == NULL) g_hinst = GetModuleHandle(NULL);
	return g_hinst;
}

/***************************************************************************
  函数名称：wnd_register_class
  功    能：注册窗口类（简化版，自动填充大部分字段）
  输入参数：class_name：窗口类名 / wnd_proc：窗口过程函数
			hinst：实例句柄（NULL=当前模块）/ icon：图标（NULL=默认）
			cursor：光标（NULL=默认箭头）/ bg_brush：背景画刷（NULL=COLOR_WINDOW+1）
  返 回 值：ATOM 类原子（0则失败）
  说    明：基于 RegisterClassExA，自动设置 CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS
***************************************************************************/
ATOM wnd_register_class(const char* class_name, WNDPROC wnd_proc, HINSTANCE hinst, HICON icon, HCURSOR cursor, HBRUSH bg_brush)
{
	if (hinst == NULL)   hinst = wnd_get_hinst();
	if (cursor == NULL)  cursor = LoadCursor(NULL, IDC_ARROW);
	if (bg_brush == NULL) bg_brush = (HBRUSH)(COLOR_WINDOW + 1);

	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = wnd_proc;
	wc.hInstance = hinst;
	wc.hIcon = icon ? icon : LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = cursor;
	wc.hbrBackground = bg_brush;
	wc.lpszClassName = class_name;
	wc.hIconSm = icon;
	return RegisterClassExA(&wc);
}

/***************************************************************************
  函数名称：wnd_create
  功    能：创建窗口（简化版，自动获取 HINSTANCE）
  输入参数：class_name：已注册的窗口类名 / title：窗口标题
			x,y,width,height：位置和大小 / style/ex_style：样式
			parent：父窗口 / menu：菜单
  返 回 值：HWND（NULL则失败）
  说    明：基于 CreateWindowExA
***************************************************************************/
HWND wnd_create(const char* class_name, const char* title, int x, int y, int width, int height,
	DWORD style, DWORD ex_style, HWND parent, HMENU menu)
{
	return CreateWindowExA(ex_style, class_name, title, style,
		x, y, width, height, parent, menu, wnd_get_hinst(), NULL);
}

/***************************************************************************
  函数名称：wnd_message_loop
  功    能：标准消息循环（GetMessage+TranslateMessage+DispatchMessage）
  输入参数：无
  返 回 值：int WM_QUIT 的 wParam 退出码
  说    明：阻塞直到收到 WM_QUIT
***************************************************************************/
int wnd_message_loop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

/***************************************************************************
  函数名称：wnd_subclass
  功    能：子类化——替换窗口过程，返回旧过程
  输入参数：hwnd：目标窗口 / new_proc：新窗口过程
  返 回 值：WNDPROC 旧窗口过程（供 wnd_call_default_proc 使用）
  说    明：基于 SetWindowLong(hwnd, GWL_WNDPROC, ...)
***************************************************************************/
WNDPROC wnd_subclass(HWND hwnd, WNDPROC new_proc)
{
	return (WNDPROC)SetWindowLongA(hwnd, GWL_WNDPROC, (LONG)new_proc);
}

/***************************************************************************
  函数名称：wnd_call_default_proc
  功    能：调用子类化前保存的旧窗口过程
  输入参数：old_proc：wnd_subclass 返回的旧过程
			hwnd, msg, wparam, lparam：消息参数
  返 回 值：LRESULT
  说    明：基于 CallWindowProcA
***************************************************************************/
LRESULT wnd_call_default_proc(WNDPROC old_proc, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return CallWindowProcA(old_proc, hwnd, msg, wparam, lparam);
}
