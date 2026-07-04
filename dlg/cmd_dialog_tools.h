#pragma once
#include <Windows.h>
#include <CommDlg.h>
#pragma comment(lib, "ComDlg32.lib")

/* ===== 通用对话框（自动填充 OWNER/结构体/错误处理） ===== */
/* 打开文件对话框（返回选择的文件路径，需 HeapFree 释放） */
char* dlg_open_file(HWND owner = NULL, const char* filter = "All Files\0*.*\0",
	const char* title = "打开文件", const char* initial_dir = NULL);

/* 保存文件对话框（返回保存路径，需 HeapFree 释放） */
char* dlg_save_file(HWND owner = NULL, const char* filter = "All Files\0*.*\0",
	const char* title = "保存文件", const char* initial_dir = NULL, const char* default_ext = NULL);

/* 颜色选择对话框（返回选择的 RGB 值，取消返回 0x7FFFFFFF） */
int dlg_choose_color(HWND owner = NULL, int default_color = 0);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 消息框：MessageBoxA(hwnd, text, caption, type)
   - 高级对话框：GetOpenFileNameA, GetSaveFileNameA, ChooseColorA,
                 ChooseFontA, FindTextA, ReplaceTextA
   - 打印：PrintDlgA, PageSetupDlgA
   - 文件夹选择：SHBrowseForFolderA (shell32)
   ================================================================ */
