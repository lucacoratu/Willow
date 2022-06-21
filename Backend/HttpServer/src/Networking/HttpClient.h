#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "../Parser/HttpRequest.h"
#include "../Parser/HttpResponse.h"

namespace Willow{
    class HttpClient
    {
    private:
        //Socket for read / write
        uint64_t m_Socket;
    public:
        HttpClient(const char* serverAddress, unsigned int port);
        ~HttpClient();

        //Send a request to the server and get the response back
        HttpResponse SendRequest(HttpRequest& request);
    };
}

#endif