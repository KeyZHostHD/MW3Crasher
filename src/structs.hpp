// ==================== MW3Crasher ========================
// Project: https://github.com/diamante0018/MW3Crasher
// Author: momo5502 (https://github.com/momo5502), Diavolo (https://github.com/diamante0018)
// License: GNU GPL v3.0
// ========================================================

#pragma once

namespace game
{
	enum DvarFlags
	{
		DVAR_FLAG_NONE = 0,
		DVAR_FLAG_SAVED = 0x1,
		DVAR_FLAG_LATCHED = 0x2,
		DVAR_FLAG_CHEAT = 0x4,
		DVAR_FLAG_REPLICATED = 0x8,
		DVAR_FLAG_WRITE = 0x800,
		DVAR_FLAG_READ = 0x2000,
	};

	typedef float vec_t;
	typedef vec_t vec2_t[2];
	typedef vec_t vec3_t[3];
	typedef vec_t vec4_t[4];
	typedef vec_t vec5_t[5];

	typedef struct
	{
		unsigned char red;
		unsigned char green;
		unsigned char blue;
		unsigned char alpha;
	} ucolor_t;

	union DvarValue
	{
		bool enabled;
		int integer;
		unsigned int unsignedInt;
		float value;
		vec4_t vector;
		const char *string;
		ucolor_t color;
	};

	struct enum_limit
	{
		int stringCount;
		const char **strings;
	};

	struct int_limit
	{
		int min;
		int max;
	};

	struct float_limit
	{
		float min;
		float max;
	};

	union DvarLimits
	{
		enum_limit enumeration;
		int_limit integer;
		float_limit value;
		float_limit vector;
	};

	struct dvar_t
	{
		const char *name;
		unsigned int flags;
		char type;
		bool modified;
		DvarValue current;
		DvarValue latched;
		DvarValue reset;
		DvarLimits domain;
		bool(__cdecl *domainFunc)(dvar_t *, DvarValue);
		dvar_t *hashNext;
	};

#pragma pack(push, 1)
	enum netadrtype_t : std::uint32_t
	{
		NA_BOT = 0x0,
		NA_BAD = 0x1,
		NA_LOOPBACK = 0x2,
		NA_BROADCAST = 0x3,
		NA_IP = 0x4,
	};

	struct netadr_s
	{
		netadrtype_t type;
		unsigned char ip[4];
		uint16_t port;
		uint16_t field_A;
		uint32_t field_C;
		uint32_t field_10;
		uint32_t index;
	};

	static_assert(sizeof(netadr_s) == 0x18);

	struct ipNport
	{
		uint8_t ip[4];
		uint16_t port;
		ipNport &operator=(const ipNport &data)
		{
			this->ip[0] = data.ip[0];
			this->ip[1] = data.ip[1];
			this->ip[2] = data.ip[2];
			this->ip[3] = data.ip[3];
			this->port = data.port;
			return *this;
		}
	};

	static_assert(sizeof(ipNport) == 6);

	struct playerNetData
	{
		ipNport local;
		ipNport route1;
		ipNport route2;
		ipNport external;
		uint8_t isValid;
		playerNetData &operator=(const playerNetData &data)
		{
			this->local = data.local;
			this->route1 = data.route1;
			this->route2 = data.route2;
			this->external = data.external;
			this->isValid = data.isValid;
			return *this;
		}
		void SetData(const playerNetData *nd)
		{
			this->local = nd->local;
			this->route1 = nd->route1;
			this->route2 = nd->route2;
			this->external = nd->external;
			this->isValid = nd->isValid;
		}
	};

	static_assert(sizeof(playerNetData) == 0x19);

	struct bdSecurityKeyMap_SecurityID
	{
		uint8_t id[8];
		bdSecurityKeyMap_SecurityID &operator=(const bdSecurityKeyMap_SecurityID &data)
		{
			memcpy(id, data.id, sizeof(id));
			return *this;
		}
		void init(uint8_t key)
		{
			memset(this->id, key, sizeof(id));
		}
		void set(bdSecurityKeyMap_SecurityID *res)
		{
			memcpy(id, res->id, sizeof(id));
		}
	};

	struct bdSecurityKeyMap_SecurityKey
	{
		uint8_t key[16];
	};

	struct hostInfo
	{
		uint8_t isValid;
		bdSecurityKeyMap_SecurityID bdID;
		playerNetData ND;
		bdSecurityKeyMap_SecurityKey bdKey;
	};

	static_assert(sizeof(hostInfo) == 0x32);

	namespace DTLS
	{
		struct initPacket
		{
			uint8_t header_high;
			uint8_t header_low;
			uint32_t unknown;
			uint16_t m_initTag;
			bdSecurityKeyMap_SecurityID bdSecurityID;
		};

		static_assert(sizeof(initPacket) == 16);

		struct cookieAck
		{
			uint16_t m_localTag;
			uint16_t localTag;
		};

		struct initAck
		{
			uint8_t header_high;
			uint8_t header_low;
			uint32_t m_initTag;
			uint32_t d_0;
			uint32_t d_1;
			cookieAck cookie;
			uint32_t m_peerTag;
			uint16_t unknown;
			uint32_t client_ip;
			uint16_t client_port;
			bdSecurityKeyMap_SecurityID bdSecurityID;
		};

		static_assert(sizeof(initAck) == 38);
		static_assert(offsetof(initAck, cookie) == 14);

		struct cookieAckPacket
		{
			uint8_t header_high;
			uint8_t header_low;
			cookieAck cookie;
			initAck initack;
			playerNetData pND;
			bdSecurityKeyMap_SecurityID bdSecurityID;
			uint8_t ECCkeyExportkey[100];
		};

		static_assert(sizeof(cookieAckPacket) == 177);
	}

#pragma pack(pop)

	template <typename T>
	class symbol
	{
	public:
		symbol(const size_t sp_address, const size_t mp_address)
			: sp_object_(reinterpret_cast<T *>(sp_address)), mp_object_(reinterpret_cast<T *>(mp_address))
		{
		}

		T *get() const
		{
			return mp_object_;
		}

		operator T *() const
		{
			return this->get();
		}

		T *operator->() const
		{
			return this->get();
		}

	private:
		T *sp_object_;
		T *mp_object_;
	};

}

#include "symbols.hpp"
