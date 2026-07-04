#pragma once
#include <Windows.h>

/* ===== 进程创建 ===== */
/* 创建进程（自动填充 STARTUPINFO，可选的 PROCESS_INFORMATION 输出） */
BOOL prc_create(const char* command_line, const char* work_dir = NULL, DWORD flags = 0,
	PROCESS_INFORMATION* out_pi = NULL, BOOL inherit = FALSE);

/* 以管理员权限创建进程（ShellExecute + "runas"） */
BOOL prc_create_as_admin(const char* command_line, const char* work_dir = NULL);

/* ===== 权限判断 ===== */
BOOL prc_is_admin();          /* 判断当前是否以管理员权限运行 */
BOOL prc_is_wow64(HANDLE process); /* 判断进程是否运行在 WOW64 模式 */
BOOL prc_is_running(HANDLE process); /* 判断进程是否仍在运行（通过 STILL_ACTIVE） */

/* ===== 权限提升 ===== */
BOOL prc_enable_privilege(const char* privilege_name);  /* 启用指定特权 */
BOOL prc_disable_privilege(const char* privilege_name); /* 禁用指定特权 */

/* ===== 获取进程路径 ===== */
DWORD prc_get_process_path(HANDLE process, char* buffer, DWORD size);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 进程操作：CreateProcessA, OpenProcess, TerminateProcess,
              GetExitCodeProcess, ExitProcess
   - 进程信息：GetCurrentProcessId, GetCurrentProcess,
              GetPriorityClass, SetPriorityClass
   - 执行文件：ShellExecuteA (打开文件/URL/文件夹)
   - 等待：WaitForSingleObject(process, timeout)
   - 命令行：GetCommandLineA
   - 路径：GetModuleFileNameA(NULL, buf, size) 获取exe路径
   ================================================================ */
