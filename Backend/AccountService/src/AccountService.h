#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H

namespace Willow {
    class AccountService
    {
    private:
        //Member variables
        //Instance variable
        static AccountService* s_Instance;

        //Port it is running on
        unsigned int m_Port;

        AccountService(unsigned int port);
        ~AccountService();
        AccountService(const AccountService& other) = delete;
    public:
        static AccountService* CreateInstance(unsigned int port);
        static void DeleteInstance();
    };
    

    
}

#endif