#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "cmd_input_tools.h"

/***************************************************************************
  函数名称：inp_is_key_down
  功    能：判断虚拟键当前是否被按下（实时检测）
  输入参数：vk_code：虚拟键码，如 VK_LEFT, VK_SPACE, 'A' 等
  返 回 值：BOOL
  说    明：基于 GetAsyncKeyState，提取最高位判断
***************************************************************************/
BOOL inp_is_key_down(int vk_code)
{
	return (GetAsyncKeyState(vk_code) & 0x8000) != 0;
}

/***************************************************************************
  函数名称：inp_is_key_toggled
  功    能：判断虚拟键是否处于切换状态
  输入参数：vk_code：虚拟键码
  返 回 值：BOOL
  说    明：基于 GetKeyState，提取最低位判断，适用于 CapsLock/NumLock/ScrollLock
***************************************************************************/
BOOL inp_is_key_toggled(int vk_code)
{
	return (GetKeyState(vk_code) & 1) != 0;
}
