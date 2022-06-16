#include "HttpServer.h"

#include "../Parser/HttpResponse.h"
#include "../Parser/HttpRequest.h"

namespace Willow {

    //Constructors / Destructors
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

        HttpResponse res;
        res.SetHttpVersion("HTTP/1.1");
        res.SetHttpStatusCode(200);
        res.AppendHttpHeader({"Server", "MyHttp"});
        res.AppendHttpHeader({"Connection", "Keep-Alive"});
        res.SetHttpBody("<html><body>Salut!</body></html>");

        SendToClient(clientSocket, res.GetRawHttpResponse());
    }


    //Public functions
}