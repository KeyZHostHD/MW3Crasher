// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once

#include "std_include.hpp"

namespace console
{
	LRESULT __stdcall RWordProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	bool ConsoleUnlock();
	void ShowConsole();
}