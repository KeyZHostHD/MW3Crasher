// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once

namespace client
{
	void CL_printf(const char* fmt, ...);
	void sendUICMD(const char* command);
	void wallhack();
	void forceClass();
	void juggHack();
	void switchAxis();
	void switchAllies();
	void switchSpec();
	void quitGame();
	void sendCommand(const char* fmt);
}