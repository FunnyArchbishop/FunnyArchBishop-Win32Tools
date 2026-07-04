#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <CommDlg.h>
#include "cmd_dialog_tools.h"

/***************************************************************************
  函数名称：dlg_open_file
  功    能：打开文件选择对话框
  输入参数：owner：父窗口 / filter：过滤器（如 "Text\0*.txt\0All\0*.*\0"）
			title：标题 / initial_dir：初始目录
  返 回 值：char* 选中路径（需 HeapFree 释放），取消返回 NULL
  说    明：基于 GetOpenFileNameA
***************************************************************************/
char* dlg_open_file(HWND owner, const char* filter, const char* title, const char* initial_dir)
{
	char buf[MAX_PATH * 2] = { 0 };
	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = sizeof(buf);
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = initial_dir;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (!GetOpenFileNameA(&ofn)) return NULL;
	DWORD len = (DWORD)strlen(buf) + 1;
	char* ret = (char*)HeapAlloc(GetProcessHeap(), 0, len);
	if (ret) memcpy(ret, buf, len);
	return ret;
}

/***************************************************************************
  函数名称：dlg_save_file
  功    能：保存文件选择对话框
  说    明：基于 GetSaveFileNameA，自动添加默认扩展名
***************************************************************************/
char* dlg_save_file(HWND owner, const char* filter, const char* title, const char* initial_dir, const char* default_ext)
{
	char buf[MAX_PATH * 2] = { 0 };
	OPENFILENAMEA ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = sizeof(buf);
	ofn.lpstrTitle = title;
	ofn.lpstrInitialDir = initial_dir;
	ofn.lpstrDefExt = default_ext;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	if (!GetSaveFileNameA(&ofn)) return NULL;
	DWORD len = (DWORD)strlen(buf) + 1;
	char* ret = (char*)HeapAlloc(GetProcessHeap(), 0, len);
	if (ret) memcpy(ret, buf, len);
	return ret;
}

/***************************************************************************
  函数名称：dlg_choose_color
  功    能：颜色选择对话框
  说    明：基于 ChooseColorA，返回 RGB 值，取消返回 0x7FFFFFFF
***************************************************************************/
int dlg_choose_color(HWND owner, int default_color)
{
	CHOOSECOLORA cc = { 0 };
	static COLORREF cust_colors[16] = { 0 };
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = owner;
	cc.rgbResult = (COLORREF)default_color;
	cc.lpCustColors = cust_colors;
	cc.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (!ChooseColorA(&cc)) return 0x7FFFFFFF;
	return (int)cc.rgbResult;
}
