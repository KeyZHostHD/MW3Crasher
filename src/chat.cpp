// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"
#include "chat.hpp"
#include "client.hpp"

volatile long isSpam = 0;

namespace chat
{
	DWORD WINAPI spamChat(LPVOID /*lpParam*/)
	{
		while (isSpam)
		{
//			Godmode glitch
			client::sub_4eb8f0Wrapper("cmd mr 16 9 custom1");
//			client::juggHack();
			client::sub_4eb8f0Wrapper("say !spec");
//			client::sendCommand("say offensive offensive");
			Sleep(1000);
		}

		client::CL_printf("%s: Exit\n", __FUNCTION__);
		return 0;
	}
}