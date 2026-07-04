#pragma once
#include <Windows.h>

/* ===== 剪贴板文本操作（自动管理 Open/Close/GlobalLock/GlobalUnlock） ===== */
/* 从剪贴板获取文本（返回 HeapAlloc 字符串，需 HeapFree 释放，失败返回 NULL） */
char* clp_get_text();

/* 将文本放入剪贴板 */
BOOL clp_set_text(const char* text);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 剪贴板：OpenClipboard(hwnd), CloseClipboard(), EmptyClipboard()
   - 数据操作：SetClipboardData(CF_TEXT, hMem),
              GetClipboardData(CF_TEXT), IsClipboardFormatAvailable(CF_TEXT)
   - 全局内存：GlobalAlloc(GMEM_MOVEABLE, size), GlobalLock(hMem),
              GlobalUnlock(hMem), GlobalFree(hMem)
   - 其他格式：CF_BITMAP, CF_DIB, CF_UNICODETEXT, CF_HDROP 等
   ================================================================ */
