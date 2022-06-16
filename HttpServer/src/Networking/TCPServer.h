#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "Core.h"

#include <iostream>
#include <string>
#include <string_view>

namespace Willow
{
    class TCPServer
    {
    private:
        //Member variables
        //Ip address of the server
        const char* m_IpAddress;
        
        //Port of the server
        int m_Port;

        //Server socket
        uint64_t m_Socket;

        int m_EpollDescriptor;
    protected:
        //Handler functions for events

        //Handle a new connection
        virtual void OnClientConnected(uint64_t client) = 0;

        //Handle a client disconnecting
        virtual void OnClientDisconnected(uint64_t clientSocket) = 0;

        //Handle message received from client
        virtual void OnMessageReceived(uint64_t clientSocket, std::string message) = 0;

        void SendToClient(uint64_t clientSocket, std::string message);

    public:
        //Constructors / Destructors
        TCPServer(const char* ipAddress, int port);
        virtual ~TCPServer();

        //Initialize the TCP server
        int init();

        //Run the TCP server
        int run();
    };
}
#endif