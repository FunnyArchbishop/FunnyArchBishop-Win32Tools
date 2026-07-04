#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "cmd_file_tools.h"

/***************************************************************************
  函数名称：fil_read_all
  功    能：一次性读取整个文件内容到内存缓冲区
  输入参数：path：文件路径 / out_size：输出实际大小（可选）
  返 回 值：char* 堆缓冲区（用完后需 HeapFree(GetProcessHeap(), 0, ptr) 释放）
  说    明：基于 CreateFileA + GetFileSize + ReadFile + HeapAlloc
***************************************************************************/
char* fil_read_all(const char* path, DWORD* out_size)
{
	HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return NULL;
	DWORD size = GetFileSize(h, NULL);
	if (size == INVALID_FILE_SIZE) { CloseHandle(h); return NULL; }
	char* buf = (char*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size + 1);
	if (!buf) { CloseHandle(h); return NULL; }
	DWORD read;
	if (!ReadFile(h, buf, size, &read, NULL) || read != size) {
		HeapFree(GetProcessHeap(), 0, buf); CloseHandle(h); return NULL;
	}
	buf[size] = '\0';
	if (out_size) *out_size = size;
	CloseHandle(h);
	return buf;
}

/***************************************************************************
  函数名称：fil_write_all
  功    能：一次性将内存数据写入文件
  输入参数：path：文件路径 / data：数据指针 / size：数据大小
  返 回 值：BOOL
  说    明：基于 CreateFileA + WriteFile + CloseHandle
***************************************************************************/
BOOL fil_write_all(const char* path, const void* data, DWORD size)
{
	HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return FALSE;
	DWORD written;
	BOOL ret = WriteFile(h, data, size, &written, NULL) && (written == size);
	CloseHandle(h);
	return ret;
}

/***************************************************************************
  函数名称：fil_create_directories
  功    能：递归创建多级目录（类似 mkdir -p）
  输入参数：path：完整目录路径
  返 回 值：BOOL
  说    明：从根开始逐级检查并创建不存在的目录
***************************************************************************/
BOOL fil_create_directories(const char* path)
{
	char tmp[MAX_PATH];
	strncpy_s(tmp, sizeof(tmp), path, _TRUNCATE);
	DWORD len = (DWORD)strlen(tmp);
	for (DWORD i = 0; i < len; i++) {
		if (tmp[i] == '\\' || tmp[i] == '/') {
			tmp[i] = '\0';
			DWORD attr = GetFileAttributesA(tmp);
			if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
				CreateDirectoryA(tmp, NULL);
			tmp[i] = '\\';
		}
	}
	DWORD attr = GetFileAttributesA(tmp);
	if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
		return CreateDirectoryA(tmp, NULL) != FALSE;
	return TRUE;
}

/***************************************************************************
  函数名称：fil_create_temp
  功    能：创建临时文件（自动生成唯一名称）
  输入参数：dir：目录（NULL=系统临时目录）/ prefix：文件名前缀
			out_path：输出完整路径（可选）
  返 回 值：HANDLE 文件句柄（INVALID_HANDLE_VALUE 则失败）
  说    明：基于 GetTempFileNameA + CreateFileA，文件关闭时自动删除
***************************************************************************/
HANDLE fil_create_temp(const char* dir, const char* prefix, char* out_path)
{
	char tmp_path[MAX_PATH];
	if (!dir) { GetTempPathA(sizeof(tmp_path), tmp_path); dir = tmp_path; }
	char path[MAX_PATH];
	if (!GetTempFileNameA(dir, prefix ? prefix : "tmp", 0, path))
		return INVALID_HANDLE_VALUE;
	HANDLE h = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	if (h != INVALID_HANDLE_VALUE && out_path)
		strcpy_s(out_path, MAX_PATH, path);
	return h;
}
