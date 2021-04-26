
#include "served/multiplexer.hpp"
#include "http_server.h"


int main() {
    served::multiplexer multiplexer;
    learning::HttpServer http_server(multiplexer);

    http_server.InitializeEndpoints();
    http_server.StartServer();
}