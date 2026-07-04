#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <PowrProf.h>
#include "cmd_system_tools.h"

/* ===== 内部：通过 RtlGetVersion 获取真实版本 ===== */
typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
static void sys_get_real_version(DWORD* major, DWORD* minor, DWORD* build)
{
	*major = *minor = *build = 0;
	HMODULE ntdll = GetModuleHandleA("ntdll.dll");
	if (ntdll) {
		RtlGetVersionPtr fn = (RtlGetVersionPtr)GetProcAddress(ntdll, "RtlGetVersion");
		if (fn) {
			RTL_OSVERSIONINFOW rov = { 0 };
			rov.dwOSVersionInfoSize = sizeof(rov);
			if (fn(&rov) == 0) { *major = rov.dwMajorVersion; *minor = rov.dwMinorVersion; *build = rov.dwBuildNumber; }
		}
	}
}

/***************************************************************************
  函数名称：sys_is_win10_or_later / sys_is_win11_or_later
  功    能：判断系统版本
  说    明：基于 RtlGetVersion 获取真实内核版本号
***************************************************************************/
BOOL sys_is_win10_or_later()  { return sys_get_major_version() >= 10; }
BOOL sys_is_win11_or_later()  { return sys_get_major_version() >= 10 && sys_get_build_number() >= 22000; }
DWORD sys_get_major_version() { DWORD a, b, c; sys_get_real_version(&a, &b, &c); return a; }
DWORD sys_get_minor_version() { DWORD a, b, c; sys_get_real_version(&a, &b, &c); return b; }
DWORD sys_get_build_number()  { DWORD a, b, c; sys_get_real_version(&a, &b, &c); return c; }

/***************************************************************************
  函数名称：sys_get_total_phys_mem / sys_get_avail_phys_mem
  功    能：获取物理内存总量/可用量（字节）
  说    明：基于 GlobalMemoryStatusEx，从结构体中提取对应字段
***************************************************************************/
DWORDLONG sys_get_total_phys_mem() {
	MEMORYSTATUSEX ms = { sizeof(ms) };
	GlobalMemoryStatusEx(&ms);
	return ms.ullTotalPhys;
}
DWORDLONG sys_get_avail_phys_mem() {
	MEMORYSTATUSEX ms = { sizeof(ms) };
	GlobalMemoryStatusEx(&ms);
	return ms.ullAvailPhys;
}

/***************************************************************************
  函数名称：sys_get_disk_free_space
  功    能：获取磁盘可用空间和总空间（字节）
  说    明：基于 GetDiskFreeSpaceExA，从 ULARGE_INTEGER 提取 QuadPart
***************************************************************************/
BOOL sys_get_disk_free_space(const char* root_path, DWORDLONG* free_bytes, DWORDLONG* total_bytes)
{
	ULARGE_INTEGER free, total, dummy;
	if (!GetDiskFreeSpaceExA(root_path, &free, &total, &dummy)) return FALSE;
	if (free_bytes)  *free_bytes = free.QuadPart;
	if (total_bytes) *total_bytes = total.QuadPart;
	return TRUE;
}

/***************************************************************************
  函数名称：sys_get_battery_percent / sys_is_ac_power
  功    能：获取电池百分比 / 判断是否使用交流电源
  说    明：基于 GetSystemPowerStatus
***************************************************************************/
BYTE sys_get_battery_percent() {
	SYSTEM_POWER_STATUS sps;
	return GetSystemPowerStatus(&sps) ? sps.BatteryLifePercent : 255;
}
BOOL sys_is_ac_power() {
	SYSTEM_POWER_STATUS sps;
	return GetSystemPowerStatus(&sps) && sps.ACLineStatus == 1;
}

/***************************************************************************
  函数名称：sys_is_caps/num/scroll_lock_on
  功    能：判断键盘锁定键状态
  说    明：基于 GetKeyState，提取最低位判断开关状态
***************************************************************************/
BOOL sys_is_caps_lock_on()    { return (GetKeyState(VK_CAPITAL) & 1) != 0; }
BOOL sys_is_num_lock_on()     { return (GetKeyState(VK_NUMLOCK) & 1) != 0; }
BOOL sys_is_scroll_lock_on()  { return (GetKeyState(VK_SCROLL)  & 1) != 0; }

/***************************************************************************
  函数名称：sys_get_page_size / sys_get_alloc_granularity
  功    能：获取系统内存页大小和分配粒度
  说    明：基于 GetSystemInfo
***************************************************************************/
DWORD sys_get_page_size() {
	SYSTEM_INFO si; GetSystemInfo(&si); return si.dwPageSize;
}
DWORD sys_get_alloc_granularity() {
	SYSTEM_INFO si; GetSystemInfo(&si); return si.dwAllocationGranularity;
}

/***************************************************************************
  函数名称：sys_shutdown / sys_reboot / sys_logoff
  功    能：关机/重启/注销
  说    明：自动获取 SE_SHUTDOWN_NAME 特权后调用 ExitWindowsEx
***************************************************************************/
static BOOL sys_acquire_shutdown_privilege()
{
	HANDLE tk; TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tk))
		return FALSE;
	LookupPrivilegeValueA(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(tk, FALSE, &tkp, 0, NULL, 0);
	CloseHandle(tk);
	return GetLastError() == ERROR_SUCCESS;
}

BOOL sys_shutdown(BOOL force, BOOL reboot)
{
	if (!sys_acquire_shutdown_privilege()) return FALSE;
	UINT flags = (reboot ? EWX_REBOOT : EWX_SHUTDOWN);
	if (force) flags |= EWX_FORCE;
	return ExitWindowsEx(flags, SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
}
BOOL sys_reboot(BOOL force) { return sys_shutdown(force, TRUE); }
BOOL sys_logoff(BOOL force) {
	UINT flags = EWX_LOGOFF; if (force) flags |= EWX_FORCE;
	return ExitWindowsEx(flags, 0);
}

/***************************************************************************
  函数名称：sys_get_error_text / sys_show_error
  功    能：将错误码转为文字描述 / 弹出错误对话框
  说    明：基于 FormatMessageA / MessageBoxA
***************************************************************************/
DWORD sys_get_error_text(DWORD error_code, char* buffer, DWORD size)
{
	return FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, size, NULL);
}
void sys_show_error(DWORD error_code, const char* caption)
{
	char buf[512];
	sys_get_error_text(error_code, buf, sizeof(buf));
	MessageBoxA(NULL, buf, caption ? caption : "Error", MB_OK | MB_ICONERROR);
}

/***************************************************************************
  函数名称：sys_reg_* 系列
  功    能：注册表读写快捷操作（自动打开/关闭注册表键）
  说    明：基于 RegOpenKeyExA / RegQueryValueExA / RegSetValueExA / RegCreateKeyExA / RegDeleteKeyA / RegDeleteValueA
***************************************************************************/
LONG sys_reg_get_string(HKEY root, const char* sk, const char* vn, char* buf, DWORD* sz) {
	HKEY hk; LONG r = RegOpenKeyExA(root, sk, 0, KEY_READ, &hk);
	if (r) return r;
	r = RegQueryValueExA(hk, vn, NULL, NULL, (BYTE*)buf, sz);
	RegCloseKey(hk); return r;
}
LONG sys_reg_get_dword(HKEY root, const char* sk, const char* vn, DWORD* val) {
	HKEY hk; LONG r = RegOpenKeyExA(root, sk, 0, KEY_READ, &hk);
	if (r) return r;
	DWORD ty, sz = sizeof(DWORD);
	r = RegQueryValueExA(hk, vn, NULL, &ty, (BYTE*)val, &sz);
	RegCloseKey(hk); return r;
}
LONG sys_reg_set_string(HKEY root, const char* sk, const char* vn, const char* data) {
	HKEY hk; LONG r = RegCreateKeyExA(root, sk, 0, NULL, 0, KEY_WRITE, NULL, &hk, NULL);
	if (r) return r;
	r = RegSetValueExA(hk, vn, 0, REG_SZ, (const BYTE*)data, (DWORD)(strlen(data) + 1));
	RegCloseKey(hk); return r;
}
LONG sys_reg_set_dword(HKEY root, const char* sk, const char* vn, DWORD val) {
	HKEY hk; LONG r = RegCreateKeyExA(root, sk, 0, NULL, 0, KEY_WRITE, NULL, &hk, NULL);
	if (r) return r;
	r = RegSetValueExA(hk, vn, 0, REG_DWORD, (const BYTE*)&val, sizeof(DWORD));
	RegCloseKey(hk); return r;
}
LONG sys_reg_delete_value(HKEY root, const char* sk, const char* vn) {
	HKEY hk; LONG r = RegOpenKeyExA(root, sk, 0, KEY_SET_VALUE, &hk);
	if (r) return r;
	r = RegDeleteValueA(hk, vn);
	RegCloseKey(hk); return r;
}
LONG sys_reg_create_key(HKEY root, const char* sk, HKEY* out) {
	DWORD disp; return RegCreateKeyExA(root, sk, 0, NULL, 0, KEY_ALL_ACCESS, NULL, out, &disp);
}
LONG sys_reg_delete_key(HKEY root, const char* sk) {
	return RegDeleteKeyA(root, sk);
}
