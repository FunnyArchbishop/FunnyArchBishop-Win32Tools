#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <Shellapi.h>
#include "cmd_process_tools.h"
#pragma comment(lib, "Shell32.lib")

/***************************************************************************
  函数名称：prc_create
  功    能：创建进程（简化版）
  说    明：自动填充 STARTUPINFO，若 out_pi==NULL 则自动关闭返回的句柄
***************************************************************************/
BOOL prc_create(const char* command_line, const char* work_dir, DWORD flags,
	PROCESS_INFORMATION* out_pi, BOOL inherit)
{
	STARTUPINFOA si = { sizeof(si) };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	char* cmd = _strdup(command_line);
	if (!cmd) return FALSE;
	PROCESS_INFORMATION pi = { 0 };
	BOOL ret = CreateProcessA(NULL, cmd, NULL, NULL, inherit, flags, NULL, work_dir, &si, &pi);
	free(cmd);
	if (ret) {
		if (out_pi) *out_pi = pi;
		else { CloseHandle(pi.hProcess); CloseHandle(pi.hThread); }
	}
	return ret;
}

/***************************************************************************
  函数名称：prc_create_as_admin
  功    能：以管理员权限创建进程
  说    明：基于 ShellExecuteA("runas")，返回 >32 表示成功
***************************************************************************/
BOOL prc_create_as_admin(const char* command_line, const char* work_dir)
{
	return (DWORDLONG)ShellExecuteA(NULL, "runas", command_line, NULL, work_dir, SW_SHOW) > 32;
}

/***************************************************************************
  函数名称：prc_is_admin
  功    能：判断当前进程是否以管理员权限运行
  说    明：先查 TokenElevation，回退查 Administrators SID 成员身份
***************************************************************************/
BOOL prc_is_admin()
{
	BOOL is_admin = FALSE;
	HANDLE tk;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &tk)) {
		TOKEN_ELEVATION elev;
		DWORD sz = sizeof(elev);
		if (GetTokenInformation(tk, TokenElevation, &elev, sz, &sz))
			is_admin = elev.TokenIsElevated;
		CloseHandle(tk);
	}
	if (!is_admin) {
		PSID sid;
		SID_IDENTIFIER_AUTHORITY nt = SECURITY_NT_AUTHORITY;
		if (AllocateAndInitializeSid(&nt, 2, SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &sid)) {
			CheckTokenMembership(NULL, sid, &is_admin);
			FreeSid(sid);
		}
	}
	return is_admin;
}

/***************************************************************************
  函数名称：prc_is_wow64
  功    能：判断进程是否在 WOW64（32位模拟层）下运行
  说    明：动态加载 IsWow64Process
***************************************************************************/
BOOL prc_is_wow64(HANDLE process)
{
	typedef BOOL(WINAPI* FN)(HANDLE, PBOOL);
	FN fn = (FN)GetProcAddress(GetModuleHandleA("kernel32.dll"), "IsWow64Process");
	if (fn) { BOOL r = FALSE; fn(process, &r); return r; }
	return FALSE;
}

/***************************************************************************
  函数名称：prc_is_running
  功    能：判断进程是否仍在运行
  说    明：基于 GetExitCodeProcess，若退出码==STILL_ACTIVE 则仍在运行
***************************************************************************/
BOOL prc_is_running(HANDLE process)
{
	DWORD code;
	return GetExitCodeProcess(process, &code) && code == STILL_ACTIVE;
}

/***************************************************************************
  函数名称：prc_get_process_path
  功    能：获取进程的可执行文件完整路径
  说    明：动态加载 QueryFullProcessImageNameA（Vista+）
***************************************************************************/
DWORD prc_get_process_path(HANDLE process, char* buffer, DWORD size)
{
	typedef DWORD(WINAPI* FN)(HANDLE, DWORD, LPSTR, PDWORD);
	FN fn = (FN)GetProcAddress(GetModuleHandleA("kernel32.dll"), "QueryFullProcessImageNameA");
	if (fn) { fn(process, 0, buffer, &size); return size; }
	return 0;
}

/***************************************************************************
  函数名称：prc_enable_privilege / prc_disable_privilege
  功    能：启用/禁用当前进程的指定特权
  说    明：OpenProcessToken + LookupPrivilegeValue + AdjustTokenPrivileges
***************************************************************************/
static BOOL prc_set_privilege(const char* name, BOOL enable)
{
	HANDLE tk; TOKEN_PRIVILEGES tp; LUID luid;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tk))
		return FALSE;
	if (!LookupPrivilegeValueA(NULL, name, &luid)) { CloseHandle(tk); return FALSE; }
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;
	BOOL r = AdjustTokenPrivileges(tk, FALSE, &tp, sizeof(tp), NULL, NULL);
	CloseHandle(tk);
	return r && GetLastError() == ERROR_SUCCESS;
}
BOOL prc_enable_privilege(const char* name)  { return prc_set_privilege(name, TRUE); }
BOOL prc_disable_privilege(const char* name) { return prc_set_privilege(name, FALSE); }
