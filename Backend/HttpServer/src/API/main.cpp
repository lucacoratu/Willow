#include "HttpAPI.h"

int main() {

    Willow::HttpAPI* server = Willow::HttpAPI::CreateInstance();

    server->Listen(8080);

    server->AddHandler("/", [](Willow::HttpRequest& req, Willow::HttpResponse& res){
       res.SetHttpStatusCode(200);
       res.AppendHttpHeader("Connection", "Keep-Alive");
       res.AppendHttpHeader("Server", "Willow");

       res.AppendHttpBody("<html><body>Salut !</body></html>");
    });

    server->AddHandler("/Users", [](Willow::HttpRequest& req, Willow::HttpResponse& res){
       res.SetHttpStatusCode(200);
       res.AppendHttpHeader("Connection", "Keep-Alive");
       res.AppendHttpHeader("Server", "Willow");

       res.AppendHttpBody("<html><body>User1: Luca</body></html>");
    });

    server->PrintHandlersPaths();

    server->InitServer();

    server->RunServer();

    server->DeleteInstance();

    return 0;
}