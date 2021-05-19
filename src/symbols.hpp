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
    WEAK symbol<void(game::msg_t* msg, void* data, int length)> MSG_Init{0, 0x040E030};
    WEAK symbol<void(game::msg_t* msg, const char* string)> MSG_WriteString{0, 0x042A560};
    WEAK symbol<void(game::msg_t* msg, uint32_t a2, uint32_t a3)> sub_4906b0{0, 0x04906B0};
    WEAK symbol<void(game::msg_t* msg, int c)> MSG_WriteShort{0, 0x04ACD80};
    WEAK symbol<void(game::msg_t* msg, void* data, int length)> MSG_WriteData{0, 0x04F8C20};
    WEAK symbol<void(int sock, game::netadr_s to, void* data, int length)> NET_OutOfBandPrint{0, 0x04639C0};
    WEAK symbol<uint64_t()> sub_4a4050{0, 0x04A4050};
}