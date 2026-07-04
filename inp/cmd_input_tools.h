#pragma once
#include <Windows.h>

/* ===== 键盘输入 ===== */
/* 判断虚拟键当前是否被按下（实时检测，适合游戏循环） */
BOOL inp_is_key_down(int vk_code);

/* 判断虚拟键是否处于"按下后切换"状态（CapsLock/NumLock/ScrollLock） */
BOOL inp_is_key_toggled(int vk_code);

/* ================================================================
   以下 Windows API 无需额外封装，建议直接调用：
   - 键盘：GetAsyncKeyState(vk_code), GetKeyState(vk_code)
   - 鼠标：GetCursorPos(&pt), SetCursorPos(x,y), ShowCursor(bShow)
   - 鼠标消息相关：GetDoubleClickTime, SwapMouseButton
   - 原始输入：RegisterRawInputDevices, GetRawInputData
   ================================================================ */
