#pragma once
#include <Windows.h>

/* ===== 高精度计时器 ===== */
/* 初始化计时器（调用一次，获取性能计数频率） */
void tmr_init();

/* 开始计时（记录起始点） */
void tmr_start();

/* 获取自 tmr_start 以来经过的毫秒数（浮点，精度微秒级） */
double tmr_get_elapsed_ms();

/* 获取自 tmr_start 以来经过的微秒数 */
double tmr_get_elapsed_us();

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 系统时间：GetSystemTime(&st), GetLocalTime(&st)
   - 性能计数：QueryPerformanceCounter(&li),
              QueryPerformanceFrequency(&li)
   - 系统启动毫秒：GetTickCount(), GetTickCount64()
   - 窗口定时器：SetTimer(hwnd, id, ms, NULL),
                KillTimer(hwnd, id),
                WM_TIMER 消息
   - 格式化时间：GetTimeFormatA, GetDateFormatA
   ================================================================ */
