// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#include <unordered_set>
#include "std_include.hpp"
#include "dvars.hpp"
#include "structs.hpp"

#include "utils/hook.hpp"

namespace dvars
{
	struct dvar_base
	{
		unsigned int flags{};
		std::string description{};
	};

	struct dvar_bool : dvar_base
	{
		bool value{};
	};

	struct dvar_float : dvar_base
	{
		float value{};
		float min{};
		float max{};
	};

	struct dvar_vector2 : dvar_base
	{
		float x{};
		float y{};
		float min{};
		float max{};
	};

	struct dvar_int : dvar_base
	{
		int value{};
		int min{};
		int max{};
	};

	namespace
	{
		template <typename T>
		T* find_dvar(std::unordered_map<std::string, T>& map, const std::string& name)
		{
			auto i = map.find(name);
			if (i != map.end())
			{
				return &i->second;
			}

			return nullptr;
		}

		bool find_dvar(std::unordered_set<std::string>& set, const std::string& name)
		{
			return set.find(name) != set.end();
		}
	}

	namespace override
	{
		static std::unordered_map<std::string, dvar_bool> register_bool_overrides;
		static std::unordered_map<std::string, dvar_float> register_float_overrides;
		static std::unordered_map<std::string, dvar_int> register_int_overrides;
		static std::unordered_map<std::string, dvar_vector2> register_vector2_overrides;

		void Dvar_RegisterBool(const std::string& name, const bool value, const unsigned int flags,
		                       const std::string& description)
		{
			dvar_bool values;
			values.value = value;
			values.flags = flags;
			values.description = description;
			register_bool_overrides[name] = std::move(values);
		}

		void Dvar_RegisterFloat(const std::string& name, const float value, const float min, const float max,
		                        const unsigned int flags,
		                        const std::string& description)
		{
			dvar_float values;
			values.value = value;
			values.min = min;
			values.max = max;
			values.flags = flags;
			values.description = description;
			register_float_overrides[name] = std::move(values);
		}

		void Dvar_RegisterInt(const std::string& name, const int value, const int min, const int max,
		                      const unsigned int flags,
		                      const std::string& description)
		{
			dvar_int values;
			values.value = value;
			values.min = min;
			values.max = max;
			values.flags = flags;
			values.description = description;
			register_int_overrides[name] = std::move(values);
		}

		void Dvar_RegisterVector2(const std::string& name, float x, float y, float min, float max,
                                  const unsigned int flags, const std::string& description)
		{
			dvar_vector2 values;
			values.x = x;
			values.y = y;
			values.min = min;
			values.max = max;
			values.flags = flags;
			values.description = description;
			register_vector2_overrides[name] = std::move(values);
		}
	}

	utils::hook::detour dvar_register_bool_hook;
	utils::hook::detour dvar_register_float_hook;
	utils::hook::detour dvar_register_int_hook;
	utils::hook::detour dvar_register_vector2_hook;
	utils::hook::detour dvar_set_server_command_hook;

	game::dvar_t* dvar_register_bool(const char* name, bool value, unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_bool_overrides, name);
		if (var)
		{
			value = var->value;
			flags = var->flags;
			description = var->description.data();
			client::CL_printf("Overriding bool dvar: %s\n", name);
		}

		return dvar_register_bool_hook.invoke<game::dvar_t*>(name, value, flags, description);
	}

	game::dvar_t* dvar_register_float(const char* name, float value, float min, float max, unsigned int flags,
	                                  const char* description)
	{
		auto* var = find_dvar(override::register_float_overrides, name);
		if (var)
		{
			value = var->value;
			min = var->min;
			max = var->max;
			flags = var->flags;
			description = var->description.data();
			client::CL_printf("Overriding float dvar: %s\n", name);
		}

		return dvar_register_float_hook.invoke<game::dvar_t*>(name, value, min, max, flags, description);
	}

	game::dvar_t* dvar_register_int(const char* name, int value, int min, int max, unsigned int flags,
	                                const char* description)
	{
		auto* var = find_dvar(override::register_int_overrides, name);
		if (var)
		{
			value = var->value;
			min = var->min;
			max = var->max;
			flags = var->flags;
			description = var->description.data();
			client::CL_printf("Overriding int dvar: %s\n", name);
		}

		return dvar_register_int_hook.invoke<game::dvar_t*>(name, value, min, max, flags, description);
	}

	game::dvar_t* dvar_register_vector2(const char* name, float x, float y, float min, float max,
		                                unsigned int flags, const char* description)
	{
		auto* var = find_dvar(override::register_vector2_overrides, name);
		if (var)
		{
			x = var->x;
			y = var->y;
			min = var->min;
			max = var->max;
			flags = var->flags;
			description = var->description.data();
			client::CL_printf("Overriding vector 2 dvar: %s\n", name);
		}

		return dvar_register_vector2_hook.invoke<game::dvar_t*>(name, x, y, min, max, flags, description);
	}

	void dvar_set_server_command(const char* dvar, const char* value)
	{
		client::CL_printf("Server attempted to change dvar: %s with value: %s\n", dvar, value);
	}

	void doHooks()
	{
		dvar_register_bool_hook.create(0x004A3300, &dvar_register_bool);
		dvar_register_float_hook.create(0x004A5CF0, &dvar_register_float);
		dvar_register_int_hook.create(0x0050C760, &dvar_register_int);
		dvar_register_vector2_hook.create(0x048AFE0, &dvar_register_vector2);
		dvar_set_server_command_hook.create(0x0552320, &dvar_set_server_command);
	}
}
