#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "cmd_timer_tools.h"

static LARGE_INTEGER _tmr_freq = { 0 };
static LARGE_INTEGER _tmr_start = { 0 };

/***************************************************************************
  函数名称：tmr_init
  功    能：初始化高精度计时器
  输入参数：无
  返 回 值：无
  说    明：必须在使用 tmr_start/tmr_get_elapsed 之前调用一次
***************************************************************************/
void tmr_init()
{
	QueryPerformanceFrequency(&_tmr_freq);
}

/***************************************************************************
  函数名称：tmr_start
  功    能：记录起始计时点
  输入参数：无
  返 回 值：无
  说    明：后续调用 tmr_get_elapsed_ms() 获取经过的毫秒
***************************************************************************/
void tmr_start()
{
	QueryPerformanceCounter(&_tmr_start);
}

/***************************************************************************
  函数名称：tmr_get_elapsed_ms
  功    能：获取自 tmr_start() 以来经过的毫秒数（浮点，微秒级精度）
  输入参数：无
  返 回 值：double 毫秒
  说    明：基于 QueryPerformanceCounter + QueryPerformanceFrequency
***************************************************************************/
double tmr_get_elapsed_ms()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (double)(now.QuadPart - _tmr_start.QuadPart) * 1000.0 / (double)_tmr_freq.QuadPart;
}

/***************************************************************************
  函数名称：tmr_get_elapsed_us
  功    能：获取自 tmr_start() 以来经过的微秒数
  输入参数：无
  返 回 值：double 微秒
  说    明：基于 QueryPerformanceCounter
***************************************************************************/
double tmr_get_elapsed_us()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (double)(now.QuadPart - _tmr_start.QuadPart) * 1000000.0 / (double)_tmr_freq.QuadPart;
}
