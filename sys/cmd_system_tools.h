#pragma once
#include <Windows.h>
#include <PowrProf.h>
#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "Advapi32.lib")

/* ===== 系统版本（通过 RtlGetVersion 获取真实版本） ===== */
BOOL  sys_is_win10_or_later();     /* 判断 >= Win10 */
BOOL  sys_is_win11_or_later();     /* 判断 >= Win11 (build>=22000) */
DWORD sys_get_major_version();     /* 主版本号 */
DWORD sys_get_minor_version();     /* 次版本号 */
DWORD sys_get_build_number();      /* 构建号 */

/* ===== 系统信息（从结构体中提取关键值） ===== */
DWORDLONG sys_get_total_phys_mem();    /* 总物理内存（字节） */
DWORDLONG sys_get_avail_phys_mem();    /* 可用物理内存（字节） */
BOOL sys_get_disk_free_space(const char* root_path, DWORDLONG* free_bytes, DWORDLONG* total_bytes);
BYTE sys_get_battery_percent();        /* 电池百分比(0-100)，非电池返回255 */
BOOL sys_is_ac_power();                /* 是否交流电源 */
BOOL sys_is_caps_lock_on();            /* CapsLock 状态 */
BOOL sys_is_num_lock_on();             /* NumLock 状态 */
BOOL sys_is_scroll_lock_on();          /* ScrollLock 状态 */
DWORD sys_get_page_size();             /* 系统页大小 */
DWORD sys_get_alloc_granularity();     /* 系统分配粒度 */

/* ===== 系统操作（需提权） ===== */
BOOL sys_shutdown(BOOL force = FALSE, BOOL reboot = FALSE);  /* 关机/重启（自动获取权限） */
BOOL sys_reboot(BOOL force = FALSE);   /* 重启 */
BOOL sys_logoff(BOOL force = FALSE);   /* 注销 */

/* ===== 错误处理 ===== */
DWORD sys_get_error_text(DWORD error_code, char* buffer, DWORD size);  /* 错误码→文字 */
void  sys_show_error(DWORD error_code, const char* caption = NULL);    /* 弹出错误对话框 */

/* ===== 注册表快捷操作（自动 Open+操作+Close） ===== */
LONG sys_reg_get_string(HKEY root, const char* sub_key, const char* value_name, char* buffer, DWORD* size);
LONG sys_reg_get_dword(HKEY root, const char* sub_key, const char* value_name, DWORD* value);
LONG sys_reg_set_string(HKEY root, const char* sub_key, const char* value_name, const char* data);
LONG sys_reg_set_dword(HKEY root, const char* sub_key, const char* value_name, DWORD value);
LONG sys_reg_delete_value(HKEY root, const char* sub_key, const char* value_name);
LONG sys_reg_create_key(HKEY root, const char* sub_key, HKEY* out_key);
LONG sys_reg_delete_key(HKEY root, const char* sub_key);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 系统度量：GetSystemMetrics(SM_CXSCREEN/SM_CYSCREEN/...)
   - 系统版本：GetVersionExA (需清单)
   - 计算机名：GetComputerNameA, GetUserNameA
   - 系统目录：GetSystemDirectoryA, GetWindowsDirectoryA, GetTempPathA
   - 工作目录：GetCurrentDirectoryA, SetCurrentDirectoryA
   - 环境变量：GetEnvironmentVariableA, SetEnvironmentVariableA, GetCommandLineA
   - 磁盘信息：GetDriveTypeA, GetLogicalDrives, GetLogicalDriveStringsA
   - 系统时间：GetTickCount, GetTickCount64
   - 内存状态：GlobalMemoryStatusEx
   - 电源状态：GetSystemPowerStatus
   - 蜂鸣：Beep(freq, duration)
   - 模块路径：GetModuleFileNameA(NULL, buf, size)
   ================================================================ */
