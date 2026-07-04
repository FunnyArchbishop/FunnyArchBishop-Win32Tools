#pragma once
#include <Windows.h>

/* ===== 一站式文件读写 ===== */
/* 一次性读取整个文件到内存，返回 HeapAlloc 分配的缓冲区（需 HeapFree 释放） */
char* fil_read_all(const char* path, DWORD* out_size = NULL);

/* 一次性将内存数据写入文件（CreateFile+WriteFile+CloseHandle 合为一体） */
BOOL fil_write_all(const char* path, const void* data, DWORD size);

/* ===== 递归创建多级目录（类似 mkdir -p） ===== */
BOOL fil_create_directories(const char* path);

/* ===== 创建临时文件（自动命名+返回句柄） ===== */
HANDLE fil_create_temp(const char* dir, const char* prefix, char* out_path = NULL);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 文件存在/属性：GetFileAttributesA, SetFileAttributesA,
                    GetFileAttributesExA (获取大小), PathFileExistsA
   - 文件操作：CreateFileA, ReadFile, WriteFile, CloseHandle,
              SetFilePointer, GetFileSize, SetEndOfFile, FlushFileBuffers
   - 文件管理：CopyFileA, MoveFileA, DeleteFileA
   - 目录操作：CreateDirectoryA, RemoveDirectoryA,
              GetCurrentDirectoryA, SetCurrentDirectoryA
   - 文件查找：FindFirstFileA, FindNextFileA, FindClose
   - 临时文件：GetTempFileNameA, GetTempPathA
   - 文件映射：CreateFileMappingA, OpenFileMappingA,
              MapViewOfFile, UnmapViewOfFile
   - 路径处理：PathFindFileNameA, PathFindExtensionA, PathCombineA (shlwapi)
   ================================================================ */
