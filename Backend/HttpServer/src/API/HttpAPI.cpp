#include "HttpAPI.h"

#include "../Networking/HttpClient.h"


namespace Willow{
    HttpAPI* HttpAPI::s_Instance = nullptr;

    //Constructors / Destructors
    HttpAPI::HttpAPI(/* args */)
        : server()
    {

    }
    
    HttpAPI::~HttpAPI()
    {
        
    }  

    //Instance functions
    HttpAPI* HttpAPI::CreateInstance() {
        if(s_Instance == nullptr)
            s_Instance = new HttpAPI();
        
        return s_Instance;
    } 

    void HttpAPI::DeleteInstance(){
        if(s_Instance != nullptr)
            delete s_Instance;
        s_Instance = nullptr;
    }

    //Initialize the server
    void HttpAPI::InitServer(){
        //Initializes the listening socket for the http server
        //If the port is not set then an exception will be thrown (can be caught with std::exception)
    
        this->server.init();
    }

    //Start the server
    void HttpAPI::RunServer(){
        this->server.run();
    }

    //Connect to another HTTP server
    HttpResponse HttpAPI::SendRequestToServer(const char* ipAddress, unsigned int port, HttpRequest& request){
        HttpClient client(ipAddress, port);
    
        HttpResponse res = client.SendRequest(request);

        return res;
    }

    void HttpAPI::Listen(unsigned int port){
        //Listen for connections on the port given as a parameter
        //The port should not be greater than 65000 nor negative
        //If the port is bigger than 65000 then an exception will be thrown (can be caught with std::exception)
        this->server.ListenOnPort(port);
    }

    
    void HttpAPI::AddHandler(HttpMethod method, std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler){
        //Add a handler for a specific path in the HTTP request
        //If the path already has a handler then an exception is thrown (can be caught with std::exception)
        //Else the handler is registered for the path and will be called when the request has the path
        this->server.AddHandler(method, path, handler);
    }

    void HttpAPI::AddCommandHandler(std::string command, std::function<void()> handler){
        //Add a handler for a specific command from stdin
        //If the path already has a handler then an exception is thrown (can be caught with std::exception)
        //Else the handler is registered for the command and will be called when the command comes from stdin
        this->server.AddCommandHandler(command, handler);
    }

    //Print handlers paths (for DEBUG)
    void HttpAPI::PrintHandlersPaths() {
        this->server.PrintHandlersPaths();
    }
}