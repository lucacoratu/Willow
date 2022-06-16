#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "TCPServer.h"

namespace Willow {
    class HttpServer : public TCPServer 
    {
    private:
        
    protected:
        //Handler functions for events
        //Handle new connection
        void OnClientConnected(uint64_t clientSocket) override final;
        
        //Handle client disconnection
        void OnClientDisconnected(uint64_t clientSocket) override final;
    
        //Handle for message received
        void OnMessageReceived(uint64_t clientSocket, std::string message) override final;
    public:
        //Constructors / Destructors
        HttpServer(const char* ipAddress, int port);
        ~HttpServer();
    };
    
}

#endif