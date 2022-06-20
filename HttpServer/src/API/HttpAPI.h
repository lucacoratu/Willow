#ifndef HTTPAPI_H
#define HTTPAPI_H

#include "../Networking/HttpServer.h"

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

        //Listen on a specific port
        void Listen(unsigned int port);

        //Add a handler for a path
        void AddHandler(std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler);

        //Print handlers paths (for DEBUG)
        void PrintHandlersPaths();
    };
    

    
}

#endif