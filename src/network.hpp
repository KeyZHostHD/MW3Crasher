// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once
#include "structs.hpp"

namespace network
{
	bool are_addresses_equal(const game::netadr_s &a, const game::netadr_s &b);
	const char *net_adr_to_string(const game::netadr_s* a);
	bool netSetup();
	int NET_SendPacket(int length, const void *data, game::netadr_s *to, uint16_t defaultPort = 0);
	void NET_OutOfBandPrint(game::netadr_s* adr, uint8_t* data, int length);
}

inline bool operator==(const game::netadr_s &a, const game::netadr_s &b)
{
	return network::are_addresses_equal(a, b); //
}

inline bool operator!=(const game::netadr_s &a, const game::netadr_s &b)
{
	return !(a == b); //
}

namespace std
{
	template <>
	struct equal_to<game::netadr_s>
	{
		using result_type = bool;

		bool operator()(const game::netadr_s &lhs, const game::netadr_s &rhs) const
		{
			return network::are_addresses_equal(lhs, rhs);
		}
	};

	template <>
	struct hash<game::netadr_s>
	{
		size_t operator()(const game::netadr_s &x) const noexcept
		{
			return hash<uint32_t>()(*reinterpret_cast<const uint32_t *>(&x.ip[0])) ^ hash<uint16_t>()(x.port);
		}
	};
}
