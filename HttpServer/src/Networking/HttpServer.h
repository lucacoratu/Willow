#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "TCPServer.h"
#include "../Parser/HttpRequest.h"
#include "../Parser/HttpResponse.h"

#include <map>
#include <functional>

namespace Willow {
    class HttpServer : public TCPServer 
    {
    private:
        //Handlers for different paths that have callback for when the path is found in the request
        std::map<std::string, std::function<void(HttpRequest&, HttpResponse&)>> m_Handlers;

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
        HttpServer();
        HttpServer(const char* ipAddress, int port);
        ~HttpServer();

        //Setters
        void ListenOnPort(unsigned int port);

        //Add handler for a specific path of the HTTP request
        void AddHandler(std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler);

        //Print the handlers paths
        void PrintHandlersPaths();
    };
    
}

#endif