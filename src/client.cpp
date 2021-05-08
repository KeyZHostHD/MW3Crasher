// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include "std_include.hpp"

#include "utils/hook.hpp"

constexpr auto BUFFER_SIZE = 4096;

typedef void(__stdcall* CL_SENDUI)(int a1, const char* command);
typedef void (__cdecl* CL_SENDCMDCONSOLE)(int a, const char* message);
typedef int(__cdecl* CL_CMDCON)(int a1, int a2, const char* format);
typedef void(__cdecl* CL_SENDCONSOLE)(const char* fmt);

namespace client
{
	CL_SENDCONSOLE CL_SendConsole = reinterpret_cast<CL_SENDCONSOLE>(0x04F7300);
	void CL_printf(const char* fmt, ...)
	{
		const auto buffer = std::make_unique<char[]>(BUFFER_SIZE);
		va_list args;
		va_start(args, fmt);
		_vsnprintf_s(buffer.get(), BUFFER_SIZE, _TRUNCATE, fmt, args);
		va_end(args);

		CL_SendConsole(buffer.get());
	}

	CL_SENDUI CL_SendUICMD = reinterpret_cast<CL_SENDUI>(0x0429920);
	void sendUICMD(const char* command)
	{
		CL_SendUICMD(0, command);
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
		sendUICMD(command);
		CL_printf("F2: Jugg Hack command sent (%s)\n", command);
	}

	void switchAxis()
	{
		const char* command = "mr 16 2 axis";
		sendUICMD(command);
		CL_printf("F3: Attempting to switch to team axis\n");
	}

	void switchAllies()
	{
		const char* command = "mr 16 2 allies";
		sendUICMD(command);
		CL_printf("F4: Attempting to switch to team allies\n");
	}

	void switchSpec()
	{
		const char* command = "mr 16 2 spectator";
		sendUICMD(command);
		CL_printf("F5: Attempting to switch to team spectator\n");
	}

	CL_SENDCMDCONSOLE CL_SendCMDConsole = reinterpret_cast<CL_SENDCMDCONSOLE>(0x004C1030);
	void quitGame()
	{
		CL_printf("END: Leaving the server\n");
		CL_SendCMDConsole(0, "disconnect\n");
	}

	CL_CMDCON CL_SENCONCMD = reinterpret_cast<CL_CMDCON>(0x4EB8F0);
	void sendCommand(const char* fmt)
	{
		CL_SENCONCMD(0, 0, fmt);
	}
}