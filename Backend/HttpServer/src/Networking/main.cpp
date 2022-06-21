#include "HttpServer.h"

#include <memory>

int main() {
    std::unique_ptr<Willow::HttpServer> server = std::make_unique<Willow::HttpServer>("localhost", 8080);

    server->init();

    server->run();
    
    return 0;
}