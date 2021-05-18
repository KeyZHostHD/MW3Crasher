#pragma once

class WSAController
{
private:
	WSAController();
	static std::once_flag m_createonceflag;
	static std::unique_ptr<WSAController> m_pInstance;
	WSAController(const WSAController &) = delete;
	WSAController(WSAController &&) = delete;
	WSAController &operator=(const WSAController &) = delete;
	WSAController &operator=(WSAController &&) = delete;
	static WSADATA wsa_data;
	static bool isValid;

public:
	~WSAController();
	static WSAController &WSAControllerInstance();
	bool IsValid();
};
