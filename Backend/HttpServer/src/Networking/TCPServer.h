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

        //Epoll Descriptor
        int m_EpollDescriptor;

        //Running status variable
        bool m_IsRunning;
    protected:
        //Handler functions for events

        //Handle a new connection
        virtual void OnClientConnected(uint64_t client) = 0;

        //Handle a client disconnecting
        virtual void OnClientDisconnected(uint64_t clientSocket) = 0;

        //Handle message received from client
        virtual void OnMessageReceived(uint64_t clientSocket, std::string message) = 0;

        //Handle command received from stdin
        virtual void OnCommandReceived(std::string command) = 0;

        //Handle sending a message to the client
        void SendToClient(uint64_t clientSocket, std::string message);

        //Setters for the derived class
        void SetPort(unsigned int port);

        //Close the server
        void Close();
    public:
        //Constructors / Destructors
        TCPServer();
        TCPServer(const char* ipAddress, int port);
        virtual ~TCPServer();

        //Initialize the TCP server
        int init();

        //Run the TCP server
        int run();

        //Connect to another server
        uint64_t ConnectToHttpServer(const char* ipAddress, unsigned int port);
    };
}
#endif