#ifndef HTTPAPI_H
#define HTTPAPI_H

#include "../Networking/HttpServer.h"
#include "../Parser/Utils.h"

namespace Willow {

    class HttpAPI
    {
    private:
        //Instance
        static HttpAPI* s_Instance;

        //Http Server
        HttpServer server;
        
        //Constructors / Destructors
        HttpAPI();
        ~HttpAPI();
        HttpAPI(const HttpAPI& other) = delete;
    public:
        //Instance functions
        static HttpAPI* CreateInstance();
        static void DeleteInstance();

        //Initialize the server
        void InitServer();

        //Start the server
        void RunServer();

        //Connect to another HTTP server
        HttpResponse SendRequestToServer(const char* ipAddress, unsigned int port, HttpRequest& request);

        //Listen on a specific port
        void Listen(unsigned int port);

        //Add a handler for a path
        void AddHandler(HttpMethod method, std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler);

        //Add a handler for a command
        void AddCommandHandler(std::string command, std::function<void()> handler);

        //Print handlers paths (for DEBUG)
        void PrintHandlersPaths();
    };
    

    
}

#endif