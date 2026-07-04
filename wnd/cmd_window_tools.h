#pragma once
#include <Windows.h>

/* === 以下函数封装了需要多步操作的 Windows API，简化使用 === */

/* 注册窗口类（自动填充 style/cbSize/默认图标/默认光标/默认背景） */
ATOM wnd_register_class(const char* class_name, WNDPROC wnd_proc, HINSTANCE hinst = NULL,
	HICON icon = NULL, HCURSOR cursor = NULL, HBRUSH bg_brush = NULL);

/* 创建窗口（自动获取 HINSTANCE） */
HWND wnd_create(const char* class_name, const char* title, int x, int y, int width, int height,
	DWORD style = WS_OVERLAPPEDWINDOW, DWORD ex_style = 0, HWND parent = NULL, HMENU menu = NULL);

/* 标准消息循环（GetMessage+TranslateMessage+DispatchMessage 循环，阻塞直到 WM_QUIT） */
int wnd_message_loop();

/* 窗口子类化：替换窗口过程，返回旧过程供 wnd_call_default_proc 使用 */
WNDPROC wnd_subclass(HWND hwnd, WNDPROC new_proc);
LRESULT wnd_call_default_proc(WNDPROC old_proc, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 显示/刷新：ShowWindow, UpdateWindow, InvalidateRect
   - 窗口属性：SetWindowText, GetWindowText, GetWindowRect, GetClientRect,
               SetWindowPos, MoveWindow, GetWindowLong, SetWindowLong
   - 设备上下文：GetDC, ReleaseDC, BeginPaint, EndPaint, GetWindowDC
   - 消息处理：GetMessage, PeekMessage, TranslateMessage, DispatchMessage,
               SendMessage, PostMessage, PostQuitMessage
   - 控件/查找：GetDlgItem, FindWindow, EnumWindows, EnumChildWindows,
                BringWindowToTop
   - 状态判断：IsWindowVisible, IsIconic, IsZoomed
   - 销毁/控制台：DestroyWindow, GetConsoleWindow
   - 对话框：CreateDialog, DialogBox, EndDialog
   ================================================================ */
