// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once

namespace dvars
{
	namespace override
	{
		void Dvar_RegisterBool(const std::string& name, bool value, const unsigned int flags, const std::string& description = "");
		void Dvar_RegisterFloat(const std::string& name, float value, float min, float max, const unsigned int flags, const std::string& description = "");
		void Dvar_RegisterInt(const std::string& name, int value, int min, int max, const unsigned int flags, const std::string& description = "");
		void Dvar_RegisterVector2(const std::string& name, float x, float y, float min, float max, const unsigned int flags, const std::string& description = "");
		void doHooks();
	}
}
