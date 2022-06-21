#include "../HttpServer/src/API/HttpAPI.h"

int main() {
    Willow::HttpAPI* server = Willow::HttpAPI::CreateInstance();

    server->Listen(9000);

    server->AddHandler(Willow::HttpMethod::GET,"/", [](Willow::HttpRequest& req, Willow::HttpResponse& res){
       res.SetHttpStatusCode(200);
       res.AppendHttpHeader("Connection", "Keep-Alive");
       res.AppendHttpHeader("Server", "Willow");

       res.AppendHttpBody("<html><body>Salut !</body></html>");
    });

    server->AddHandler(Willow::HttpMethod::GET, "/Users", [&](Willow::HttpRequest& req, Willow::HttpResponse& res){
        auto respServer = server->SendRequestToServer("127.0.0.1", 8080, req);
        
        res = respServer;
    });

    

    server->PrintHandlersPaths();

    server->InitServer();

    server->RunServer();

    server->DeleteInstance();

    return 0;


    return 0;
}