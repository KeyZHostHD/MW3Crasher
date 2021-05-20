// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"
#include "structs.hpp"
#include "network.hpp"

#include "utils\hook.hpp"
#include "utils\cryptography.hpp"

#define BUFFER_SIZE 4096

namespace client
{
	utils::hook::detour bdLogMessage_hook;

	std::mutex command_lock;
	std::mutex printf_lock;

	void CL_printf(const char *fmt, ...)
	{
		std::lock_guard<std::mutex> _(printf_lock);

		char buffer[BUFFER_SIZE] = {0};
		va_list args;
		va_start(args, fmt);
		_vsnprintf_s(buffer, BUFFER_SIZE, _TRUNCATE, fmt, args);
		va_end(args);

		game::CL_SendConsole(buffer);
	}

	/*
	* This is basically messes with a check in FoF Icons and the blindeye perk check
	*/
	void wallhack()
	{
		CL_printf("%s: Enabled cheats\n", __FUNCTION__);
		utils::hook::nop(0x00430568, 2);
		utils::hook::nop(0x005AA527, 2);
	}

	/*
	* Changes mruleResponse to nruleResponse so you can use customs classes on any server
	*/
	void forceClass()
	{
		CL_printf("%s: Forced class\n", __FUNCTION__);
		utils::hook::set<uint8_t>(0x007E75A0, 0x6E);
	}

	void juggHack()
	{
		const char *command = "mr 16 9 allies";
		game::CL_SendUICMD(0, command);
		CL_printf("F2: Jugg Hack command sent (%s)\n", command);
	}

	void switchAxis()
	{
		const char *command = "mr 16 2 axis";
		game::CL_SendUICMD(0, command);
		CL_printf("F3: Attempting to switch to team axis\n");
	}

	void switchAllies()
	{
		const char *command = "mr 16 2 allies";
		game::CL_SendUICMD(0, command);
		CL_printf("F4: Attempting to switch to team allies\n");
	}

	void switchSpec()
	{
		const char *command = "mr 16 2 spectator";
		game::CL_SendUICMD(0, command);
		CL_printf("F5: Attempting to switch to team spectator\n");
	}

	void quitGame()
	{
		CL_printf("END: Leaving the server\n");
		game::CL_SendCMDConsole(0, "disconnect\n");
	}

	void sub_4eb8f0Wrapper(const char *fmt)
	{
		std::lock_guard<std::mutex> _(command_lock);

		game::sub_4eb8f0(0, 0, fmt);
	}

	void bdLogMessage_stub(int a1, const char *a2, const char *a3, const char *a4, const char *a5, int a6, const char *fmt, ...)
	{
		char buf[512]{0};

		va_list args;
		va_start(args, fmt);
		_vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, args);

		game::CL_SendConsole(buf);

		bdLogMessage_hook.invoke<void>(a1, a2, a3, a4, a5, a6, "%s", buf);
	}

	void doHooks()
	{
		bdLogMessage_hook.create(0x6EA960, &bdLogMessage_stub);
	}

	void doTest()
	{
		uint8_t buf[2044] = {0};
		std::string randomData{};
		randomData.resize(8);
		utils::cryptography::random::get_data(randomData.data(), randomData.size());
		auto server = *reinterpret_cast<game::netadr_s *>(0x0B3D370);

		game::msg_t msg{};
		game::MSG_Init(&msg, buf, 0x800);
		game::MSG_WriteString(&msg, "steamAuth");
		auto unknown = game::sub_4a4050();
		game::sub_4906b0(&msg, (int)unknown, (int)(unknown >> 0x20));
		game::MSG_WriteShort(&msg, randomData.size());
		game::MSG_WriteData(&msg, randomData.data(), randomData.size());
		game::NET_OutOfBandPrint(0, server, msg.data, msg.cursize);
		CL_printf("Sending random Steam data\n");
	}
}