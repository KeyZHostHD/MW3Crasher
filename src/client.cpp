// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"
#include "structs.hpp"

#include "utils/hook.hpp"

constexpr auto BUFFER_SIZE = 4096;

namespace client
{
	std::recursive_mutex command_lock;
	std::recursive_mutex printf_lock;

	void CL_printf(const char* fmt, ...)
	{
		std::lock_guard<std::recursive_mutex> $(printf_lock);

		const auto buffer = std::make_unique<char[]>(BUFFER_SIZE);
		va_list args;
		va_start(args, fmt);
		_vsnprintf_s(buffer.get(), BUFFER_SIZE, _TRUNCATE, fmt, args);
		va_end(args);

		game::CL_SendConsole(buffer.get());
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
		const char* command = "mr 16 9 allies";
		game::CL_SendUICMD(0, command);
		CL_printf("F2: Jugg Hack command sent (%s)\n", command);
	}

	void switchAxis()
	{
		const char* command = "mr 16 2 axis";
		game::CL_SendUICMD(0, command);
		CL_printf("F3: Attempting to switch to team axis\n");
	}

	void switchAllies()
	{
		const char* command = "mr 16 2 allies";
		game::CL_SendUICMD(0, command);
		CL_printf("F4: Attempting to switch to team allies\n");
	}

	void switchSpec()
	{
		const char* command = "mr 16 2 spectator";
		game::CL_SendUICMD(0, command);
		CL_printf("F5: Attempting to switch to team spectator\n");
	}

	void quitGame()
	{
		CL_printf("END: Leaving the server\n");
		game::CL_SendCMDConsole(0, "disconnect\n");
	}

	void sub_4eb8f0Wrapper(const char* fmt)
	{
		std::lock_guard<std::recursive_mutex> $(command_lock);

		game::sub_4eb8f0(0, 0, fmt);
	}
}