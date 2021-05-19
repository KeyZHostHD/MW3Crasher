#include "std_include.hpp"
#include "network.hpp"
#include "structs.hpp"
#include "client.hpp"

#include "utils/string.hpp"

#define MAX_MSGLEN 0x20000

namespace network
{
	int net_compare_base_address(const game::netadr_s *a1, const game::netadr_s *a2)
	{
		if (a1->type == a2->type)
		{
			switch (a1->type)
			{
			case game::netadrtype_t::NA_BOT:
			case game::netadrtype_t::NA_LOOPBACK:
				return a1->port == a2->port;

			case game::netadrtype_t::NA_IP:
				return !memcmp(a1->ip, a2->ip, 4);
			case game::netadrtype_t::NA_BROADCAST:
				return true;
			default:
				break;
			}
		}

		return false;
	}

	int net_compare_address(const game::netadr_s *a1, const game::netadr_s *a2)
	{
		return net_compare_base_address(a1, a2) && a1->port == a2->port;
	}

	bool are_addresses_equal(const game::netadr_s &a, const game::netadr_s &b)
	{
		return net_compare_address(&a, &b);
	}

	const char *net_adr_to_string(const game::netadr_s *a)
	{
		if (a->type == game::netadrtype_t::NA_LOOPBACK)
		{
			return "loopback";
		}

		if (a->type == game::netadrtype_t::NA_BOT)
		{
			return "bot";
		}

		if (a->type == game::netadrtype_t::NA_IP || a->type == game::netadrtype_t::NA_BROADCAST)
		{
			if (a->port)
			{
				return utils::string::va("%u.%u.%u.%u:%u", a->ip[0], a->ip[1], a->ip[2], a->ip[3], htons(a->port));
			}

			return utils::string::va("%u.%u.%u.%u", a->ip[0], a->ip[1], a->ip[2], a->ip[3]);
		}

		return "bad";
	}

	bool netSetup()
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD(2, 2);

		err = WSAStartup(wVersionRequested, &wsaData);
		if (err != 0)
		{
			client::CL_printf("WSAStartup failed with error: %d\n", err);
			return false;
		}

		if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
		{
			client::CL_printf("Could not find a usable version of Winsock.dll\n");
			WSACleanup();
			return false;
		}

		return true;
	}

	int NET_SendPacket(int length, const void *data, game::netadr_s *to)
	{
		if (to->type != 4)
		{
			client::CL_printf("%s has wrong type\n", net_adr_to_string(to));
			return 0;
		}

		auto ret{SOCKET_ERROR};
		struct sockaddr_in s{};
		memset(&s, 0, sizeof(sockaddr_in));
		auto socket = *reinterpret_cast<SOCKET *>(0x05A861EC);
		s.sin_family = AF_INET;
		s.sin_addr.s_addr = *(int *)&to->ip;
		s.sin_port = to->port;

		ret = sendto(socket, (char*)data, length, 0, (sockaddr*) &s, sizeof(sockaddr_in));
		if (ret == SOCKET_ERROR)
		{
			client::CL_printf("Socket error\n");
		}

		return ret;
	}

	void NET_OutOfBandPrint(game::netadr_s *adr, void *data, int length)
	{
		const auto buffer = std::make_unique<char[]>(MAX_MSGLEN);

		// set the header
		buffer.get()[0] = -1;
		buffer.get()[1] = -1;
		buffer.get()[2] = -1;
		buffer.get()[3] = -1;

		memcpy(buffer.get() + 4, data, length);

		// send the datagram
		NET_SendPacket(length + 4, buffer.get(), adr);
	}
}
