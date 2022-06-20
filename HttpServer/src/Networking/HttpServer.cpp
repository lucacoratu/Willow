#include "HttpServer.h"

#include "../Parser/HttpResponse.h"
#include "../Parser/HttpRequest.h"

#include "ServerExceptions.h"

namespace Willow {

    //Constructors / Destructors
    HttpServer::HttpServer() 
        : TCPServer() 
    {

    }

    HttpServer::HttpServer(const char* ipAddress, int port)
        : TCPServer(ipAddress, port)
    {

    }
    
    HttpServer::~HttpServer()
    {

    }

    //Protected functions
    void HttpServer::OnClientConnected(uint64_t clientSocket) {
        //Handle new connection
        std::cout << "Client connected, socket: " << clientSocket << std::endl;
    }

    void HttpServer::OnClientDisconnected(uint64_t clientSocket){
        //Handle client disconnection
        std::cout << "Client disconnected, socket: " << clientSocket << std::endl;
    }

    void HttpServer::OnMessageReceived(uint64_t clientSocket, std::string message){
        HttpRequest req(message);
        std::cout << "Location: " << req.GetHttpLocation() << std::endl;

        //Parse the request and call the handler for the path in the request if the path has a handler
        //Else send a 404 HTTP message back to the client

        for(auto it = this->m_Handlers.begin(); it != this->m_Handlers.end(); ++it) {
            if(it->first == req.GetHttpLocation()) {
                HttpResponse res;
                res.SetHttpVersion("HTTP/1.1");
                it->second(req, res);

                std::cout << "Response: " << res.GetRawHttpResponse() << std::endl;

                SendToClient(clientSocket, res.GetRawHttpResponse());
            }
        }    
    }


    //Public functions
    void HttpServer::ListenOnPort(unsigned int port){
        this->SetPort(port);
    }

    void HttpServer::AddHandler(std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler) {
        //Checks if there is a handler for the path specified already in the list
        //If it is then throw ServerException (can be caught with std::exception)
        //It it isn't then add it to the list
        if(this->m_Handlers.find(path) != this->m_Handlers.end())
            throw ServerException("Path has already a handler!", 2);

        this->m_Handlers[path] = handler;
    }

    //Print the handlers paths
    void HttpServer::PrintHandlersPaths(){
        std::cout << "Handlers: \n";
        for(auto it = this->m_Handlers.begin(); it != this->m_Handlers.end(); ++it){
            std::cout << "Path: " << it->first << std::endl;
        }
        std::cout << std::endl;
    }
}