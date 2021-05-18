#include "std_include.hpp"
#include "winsockClient.hpp"
#include "winsockClientController.hpp"

std::once_flag WinsockClientController::m_createonceflag;
std::unique_ptr<WinsockClientController> WinsockClientController::m_pInstance;

 WinsockClientController::WinsockClientController()
 {
    this->ptrClient = nullptr;
 }

WinsockClientController::~WinsockClientController()
{
    if (this->ptrClient != nullptr)
    {
        delete this->ptrClient;
    }
}

WinsockClientController& WinsockClientController::WinsockClientControllerInstance()
{
	std::call_once(WinsockClientController::m_createonceflag, []
        {
            WinsockClientController::m_pInstance.reset(new WinsockClientController);
		});
	return *WinsockClientController::m_pInstance.get();
}

WinsockClient* WinsockClientController::GetClient()
{
    if (this->ptrClient == nullptr)
    {
        this->ptrClient = new Winsock_Client();
    }
    return this->ptrClient;
}
