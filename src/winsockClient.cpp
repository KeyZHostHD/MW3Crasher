#include "std_include.hpp"
#include "structs.hpp"
#include "client.hpp"
#include "WSAController.hpp"
#include "winsockClient.hpp"

#include "utils\cryptography.hpp"

WinsockClient::WinsockClient()
{
	this->bdSecurityID.init(0x31);
}

WinsockClient::~WinsockClient()
{
	closesocket(this->m_Socket);
}

void WinsockClient::DTLSconnect(sockaddr_in *address)
{
	if (WSAController::WSAControllerInstance().IsValid() == false)
		return;

	this->m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	this->m_ServerAddr.sin_addr = address->sin_addr;
	this->m_ServerAddr.sin_family = AF_INET;
	this->m_ServerAddr.sin_port = address->sin_port;

	auto timeout = 2 * 1000;
	setsockopt(this->m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

	this->buffer_for_dtls.bufFush();
	this->sendInit();
	client::CL_printf("Winsock_Client::DTLSconnect: sent init\n");

	if (NO_ERROR != this->recvFrom(this->buffer_for_dtls.getData(), this->buffer_for_dtls.Size()))
	{
		return;
	}

	memcpy((char *)&this->initialACK, this->buffer_for_dtls.getData(), sizeof(this->initialACK));
	this->buffer_for_dtls.bufFush();
	this->sendCookieAck();
	client::CL_printf("Winsock_Client::DTLSconnect: sent cookieack\n");
}

void WinsockClient::setPlayerNetData(game::playerNetData &playerDat)
{
	client::CL_printf("Winsock_Client::setPlayerNetData\n");
	this->playerNetData = playerDat;
}

uint32_t WinsockClient::sendTo(const char *buf, int len, int flags)
{
	auto sendret = sendto(this->m_Socket, buf, len, flags, reinterpret_cast<const SOCKADDR *>(&this->m_ServerAddr), sizeof(this->m_ServerAddr));
	if (sendret <= 0)
	{
		client::CL_printf("Winsock_Client::SendTo failed\n");
		return WSABASEERR;
	}

	return NO_ERROR;
}

uint32_t WinsockClient::recvFrom(char *buf, int len, int flags)
{
	this->m_recvbytes = 0;
	int fromLen = sizeof(this->m_ServerAddr);
	auto recvRet = recvfrom(this->m_Socket, buf, len, flags, reinterpret_cast<SOCKADDR *>(&this->m_ServerAddr), &fromLen);

	if (recvRet == SOCKET_ERROR)
	{
		auto wsaError = WSAGetLastError();
		client::CL_printf("WinsockClient::recvFrom failed: %d\n", wsaError);
		return WSABASEERR;
	}

	this->m_recvbytes = recvRet;
	return NO_ERROR;
}

void WinsockClient::flushStruct(char *dst, const size_t size)
{
	uint8_t crypt{0};
	utils::cryptography::random::get_data(&crypt, sizeof(uint8_t));

	for (size_t i = 0; i < size; i++)
	{
		dst[i] = (dst[i] + 1) ^ crypt;
		crypt++;
	}
}

void WinsockClient::sendInit()
{
	game::DTLS::initPacket initial;
	initial.header_high = 0x01;
	initial.header_low = 0x02;
	initial.unknown = 0;
	/* bytes */
	uint16_t high{0};
	uint16_t low{0};
	utils::cryptography::random::get_data(&high, sizeof(uint16_t));
	utils::cryptography::random::get_data(&low, sizeof(uint16_t));

	initial.m_initTag = MAKEWORD(LOBYTE(low), HIBYTE(high));
	initial.bdSecurityID.set(&(this->bdSecurityID));
	this->sendTo((char *)&initial, sizeof(game::DTLS::initPacket));
}

void WinsockClient::sendCookieAck()
{
	this->cookieAckPacket.header_high = 0x03;
	this->cookieAckPacket.header_low = 0x02;
	/* initack part */
	this->cookieAckPacket.initack = this->initialACK;
	this->initialACK.cookie.localTag = 0;
	this->cookieAckPacket.cookie = this->initialACK.cookie;
	this->flushStruct((char *)&this->initialACK, sizeof(this->initialACK));
	/* netdata */
	this->cookieAckPacket.pND = this->playerNetData;
	/* bdsecurity */
	this->cookieAckPacket.bdSecurityID.set(&this->bdSecurityID);
	/* ecckey */
	auto newKey = utils::cryptography::ecc::generate_key(224);
	utils::cryptography::ecc::key key(newKey);
	auto binaryKey = key.serialize(PK_PUBLIC);
	memcpy(this->cookieAckPacket.ECCkeyExportkey, binaryKey.data(), sizeof(this->cookieAckPacket.ECCkeyExportkey));

	this->sendTo((char *)&this->cookieAckPacket, sizeof(this->cookieAckPacket));
	this->flushStruct((char *)&this->cookieAckPacket, sizeof(this->cookieAckPacket));
}