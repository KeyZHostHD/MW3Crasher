#pragma once

class WinsockClientController
{
private:
	WinsockClientController();
	static std::once_flag m_createonceflag;
	static std::unique_ptr<Winsock_ClientController> m_pInstance;
	WinsockClientController(const WinsockClientController &) = delete;
	WinsockClientController(WinsockClientController &&) = delete;
	WinsockClientController &operator=(const WinsockClientController &) = delete;
	WinsockClientController &operator=(WinsockClientController &&) = delete;

	Winsock_Client *ptrClient = nullptr;

public:
	~WinsockClientController();
	static WinsockClientController &WinsockClientControllerInstance();
	Winsock_Client *GetClient();
};