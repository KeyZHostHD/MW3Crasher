// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once

#define WEAK __declspec(selectany)

namespace game
{
    WEAK symbol<void(const char* message)> CL_SendConsole{0, 0x04F7300};
    WEAK symbol<void(int a1, const char* command)> CL_SendUICMD{0, 0x0429920};
    WEAK symbol<void(int a1, const char* command)> CL_SendCMDConsole{0, 0x04C1030};
    WEAK symbol<void(int a1, int a2, const char* command)> sub_4eb8f0{0, 0x04EB8F0};
}