#pragma once

class WinsockClient
{
public:
	WinsockClient();
	~WinsockClient();
	void DTLSconnect(sockaddr_in *address);
	void setPlayerNetData(game::playerNetData &playerNetData);

private:
	template <size_t size>
	class memBuffer
	{
	private:
		char _mem[size] = {0};

	public:
		memBuffer()
		{
			for (size_t i = 0; i < size; i++)
			{
				_mem[i] = 0;
			}
		}
		~memBuffer() = default;
		void bufFush()
		{
			for (size_t i = 0; i < size; i++)
			{
				_mem[i] = 0;
			}
		}
		char *getData() { return this->_mem; }
		size_t Size() const { return size; }
	};

	SOCKET m_Socket = {0};
	sockaddr_in m_ServerAddr = {0};
	memBuffer<256> buffer_for_dtls;

	int m_recvbytes = {0};
	uint32_t sendTo(const char *buf, int len, int flags = 0);
	uint32_t recvFrom(char *buf, int len, int flags = 0);
	void flushStruct(char *dst, const size_t sz);
	game::bdSecurityKeyMap_SecurityID bdSecurityID;

	void sendInit();
	game::DTLS::initAck initialACK = {0};
	game::DTLS::cookieAckPacket cookieAckPacket = {0};
	game::playerNetData playerNetData = {0};
	void sendCookieAck();
};