#include "HttpServer.h"

#include "../Parser/HttpResponse.h"
#include "../Parser/HttpRequest.h"

#include "ServerExceptions.h"


namespace Willow {

    static std::map<std::string, HttpMethod> MethodMappings{
        {"GET", HttpMethod::GET},
        {"POST", HttpMethod::POST},
    };

    //Constructors / Destructors
    HttpServer::HttpServer() 
        : TCPServer() 
    {
        //Add a default handler for server close
        this->m_CommandHandlers["exit"] = [&](){
            this->Close();
        };
    }

    HttpServer::HttpServer(const char* ipAddress, int port)
        : TCPServer(ipAddress, port)
    {
        //Add a default handler for server close
        this->m_CommandHandlers["exit"] = [&](){
            this->Close();
        };
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
        std::cout << "Raw request: " << req.GetHttpRawRequest() << std::endl;
        //Parse the request and call the handler for the path in the request if the path has a handler
        //Else send a 404 HTTP message back to the client

        bool found = false;
        for(auto it = this->m_Handlers.begin(); it != this->m_Handlers.end(); ++it) {
            auto& [method, path] = it->first;
            HttpMethod reqMethod = MethodMappings.at(req.GetHttpMethod());
            if(path == req.GetHttpLocation() && method == reqMethod) {
                HttpResponse res;
                res.SetHttpVersion("HTTP/1.1");
                it->second(req, res);

                std::cout << "Response: " << res.GetRawHttpResponse() << std::endl;
                found = true;

                SendToClient(clientSocket, res.GetRawHttpResponse());
            }
        }

        //If the location was not found send a 404 response to the client
        if(found == false) {
            HttpResponse res;
            res.SetHttpVersion("HTTP/1.1");
            res.SetHttpStatusCode(404);
            res.AppendHttpHeader({"Server", "Willow"});
            res.AppendHttpHeader({"Connection", "Keep-Alive"});

            res.SetHttpBody("<html><head><title>404 Not Found</title></head><body bgcolor=\"white\"><center><h1>404 Not Found</h1></center><hr><center>Willow/0.0.1</center></body></html>");

            std::cout << "Response: " << res.GetRawHttpResponse() << std::endl;

            SendToClient(clientSocket, res.GetRawHttpResponse());
        }
    }

    void HttpServer::OnCommandReceived(std::string command){
        //Command received from stdin

        //Check if the command has a handler 
        //If it hasn't then output command not found warning
        //Else execute the callback defined for the command
        
        //Remove \n from the end
        command = command.substr(0, command.size() - 1);

        bool found = false;
        for(auto it = this->m_CommandHandlers.begin(); it != this->m_CommandHandlers.end(); ++it){
            if(it->first == command){
                it->second();
                found = true;
            }
        }

        if(!found){
            std::cout << "Command not found!" << std::endl;
        }
    }


    //Public functions
    void HttpServer::ListenOnPort(unsigned int port){
        this->SetPort(port);
    }

    void HttpServer::AddHandler(HttpMethod method, std::string path, std::function<void(HttpRequest&, HttpResponse&)> handler) {
        //Checks if there is a handler for the path specified already in the list
        //If it is then throw ServerException (can be caught with std::exception)
        //It it isn't then add it to the list
        for(auto it = this->m_Handlers.begin(); it != this->m_Handlers.end(); ++it){
            auto& [m_method, m_path] = it->first;
            if(m_method == method && m_path == path)
                throw ServerException("Path has already a handler!", 2);
        }

        this->m_Handlers[std::pair<HttpMethod, std::string>(method, path)] = handler;
    }

    void HttpServer::AddCommandHandler(std::string command, std::function<void()> handler){
        //Checks if there is a handler for the command specified already in the list
        //If it is then throw ServerException (can be caught with std::exception)
        //It it isn't then add it to the list
        if(this->m_CommandHandlers.find(command) != this->m_CommandHandlers.end())
            throw ServerException("Command has already a handler!", 2);

        this->m_CommandHandlers[command] = handler;
    }

    //Print the handlers paths
    void HttpServer::PrintHandlersPaths() {
        //Paths of Http request
        std::cout << "Handlers: \n";
        for(auto it = this->m_Handlers.begin(); it != this->m_Handlers.end(); ++it){
            std::cout << "Path: " << it->first.second << std::endl;
        }
        std::cout << std::endl;

        //Commands that have a handler
        std::cout << "Command Handlers: \n";
        for(auto it = this->m_CommandHandlers.begin(); it != this->m_CommandHandlers.end(); ++it){
            std::cout << "Command: " << it->first << std::endl;
        }
        std::cout << std::endl;
    }
}