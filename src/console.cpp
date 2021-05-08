// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "console.hpp"

static WNDPROC originalCallback = nullptr;
typedef LRESULT(__cdecl* InitExConsoleWindow_t)(void);
constexpr uint32_t initAddr = 0x00515CD0;
constexpr uint32_t hwndAddr = 0x05A86330;
constexpr uint32_t mainHwndAddr = 0x05A86AF0;

namespace console
{
    LRESULT __stdcall RWordProc(HWND /*hWnd*/, UINT /*Msg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
    {
        InitExConsoleWindow_t InitExConsoleWindow = reinterpret_cast<InitExConsoleWindow_t>(initAddr);
        InitExConsoleWindow();
        return 0;
    }

    bool ConsoleUnlock()
//      If it returns true -> Never call again
    {
        originalCallback = reinterpret_cast<WNDPROC>(SetWindowLongA(*reinterpret_cast<HWND*>(mainHwndAddr), GWL_WNDPROC, reinterpret_cast<LONG>(RWordProc)));
        if (originalCallback == nullptr)
        {
            return false;
        }

        SendMessage(*reinterpret_cast<HWND*>(mainHwndAddr), WM_QUIT, 0, 0);
        SetWindowLongA(*reinterpret_cast<HWND*>(mainHwndAddr), GWL_WNDPROC, reinterpret_cast<LONG>(originalCallback));
        return *reinterpret_cast<uint32_t*>(hwndAddr) > 0;
    }

    void ShowConsole()
    {
        if (*reinterpret_cast<uint32_t*>(hwndAddr))
        {
            ShowWindow(*reinterpret_cast<HWND*>(hwndAddr), SW_SHOW);
        }
    }
}