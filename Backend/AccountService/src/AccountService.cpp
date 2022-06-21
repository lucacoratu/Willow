#include "AccountService.h"

namespace Willow {
    AccountService* AccountService::s_Instance = nullptr;

    //Constructors / Destructors
    AccountService::AccountService(unsigned int port)
        : m_Port(port) {

    }
    
    AccountService::~AccountService() {

    }

    //Public functions
    AccountService* AccountService::CreateInstance(unsigned int port){

    }
    
    void AccountService::DeleteInstance() {

    }
}