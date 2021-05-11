// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"
#include "client.hpp"
#include "console.hpp"
#include "patches.hpp"
#include "dvars.hpp"
#include "crasher.hpp"
#include "chat.hpp"

DWORD WINAPI EntryPoint(LPVOID /*_arguments*/)
{
    int keyState;
    int key;
    patches::dvarPatches();
    dvars::doHooks();

    client::CL_printf("Awaiting keystrokes\n");
    HANDLE thread = NULL;
    DWORD dwThreadId;
    bool created = false;

    while (1)
    {
        // F1-F12
        for (int i = 0x70; i < 0x7C; i++)
        {
            keyState = GetAsyncKeyState(i);
            if (!keyState)
                continue;

            if (keyState & 0x1)
            {
                key = i;
                switch (key)
                {
                case 0x70:
                    client::wallhack();
                    break;
                case 0x71:
                    client::juggHack();
                    break;
                case 0x72:
                    client::switchAxis();
                    break;
                case 0x73:
                    client::switchAllies();
                    break;
                case 0x74:
                    client::switchSpec();
                    break;
                case 0x75:
                    crasher::doCrash();
                    break;
                case 0x76:
                    crasher::undoCrash();
                    break;
                case 0x77:
                    if (created)
                    {
                        client::CL_printf("Thread already created\n");
                        continue;
                    }

                    InterlockedCompareExchange(&isSpam, 1, 0);
                    thread = CreateThread(NULL, 0, chat::spamChat, NULL, 0, &dwThreadId);
                    if (thread == NULL)
                    {
                        ExitProcess(3);
                    }

                    created = true;
                    break;
                case 0x78:
                    InterlockedCompareExchange(&isSpam, 0, 1);
                    if (thread != NULL)
                    {
                        WaitForSingleObject(thread, INFINITE);
                    }
                    thread = NULL;
                    created = false;
                    break;
                case 0x79:
                    console::ConsoleUnlock();
                    console::ShowConsole();
                    break;
                case 0x7A:
//                  Empty
                    break;
                case 0x7B:
                    client::forceClass();
                    break;
                default:
                    break;
                }
            }
        }

        keyState = GetAsyncKeyState(0x23);
        if (!keyState)
            continue;
        
        if (keyState & 0x1)
            client::quitGame();

        Sleep(10);
    }

    return 0x1337;
}

BOOL APIENTRY DllMain(HMODULE,
    DWORD  ul_reason_for_call,
    LPVOID
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, static_cast<LPTHREAD_START_ROUTINE>(EntryPoint), 0, 0, 0);
    }

    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        
    }

    return TRUE;
}
