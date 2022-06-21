#include "HttpClient.h"

#include "ServerExceptions.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include <string.h>

namespace Willow {
    //Constructors / Destructors
    HttpClient::HttpClient(const char* serverAddress, unsigned int port)
    {
        //Connect to the server
        //Create the client socket
        int clientSock = -1;
        struct sockaddr_in serverAddr;

        clientSock = socket(AF_INET, SOCK_STREAM, 0);
        if(clientSock == -1)
            throw ClientException("Couldn't create the socket for connection!", 9);

        memset(&serverAddr, 0x00, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverAddress);
        serverAddr.sin_port = htons(port);

        int res = connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if(res != 0)
            throw ClientException("Couldn't connect to server!", 10);
        
       //Save the socket in the class member variable
       this->m_Socket = clientSock;
    }
    
    HttpClient::~HttpClient()
    {
        //Disconnect from the server
        close(this->m_Socket);
    }

    //Public functions
    //Send a request to the server and get the response back
    HttpResponse HttpClient::SendRequest(HttpRequest& request){
        //Get the raw request
        std::string rawRequest = request.GetHttpRawRequest();
        //Send the data on the socket
        std::cout << "request size: " << rawRequest.size();
        send(this->m_Socket, rawRequest.data(), rawRequest.size(), 0);

        //Receive the response from the server
        const unsigned int MAX_BUF_LENGTH = 32768;
        std::vector<unsigned char> buf(MAX_BUF_LENGTH);
        ssize_t bytesIn = recv(this->m_Socket, &buf[0], MAX_BUF_LENGTH, 0);

        if(bytesIn > 0) {
            std::string rawResponse = "";
            rawResponse.append(buf.cbegin(), buf.cend());

            std::cout << "Raw response:\n" << rawResponse << std::endl;
            return HttpResponse(rawResponse);
        }

        return HttpResponse();
    }
}