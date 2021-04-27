#pragma once

#include <iostream>
#include <string>

#include "SimpleJSON/json.hpp"
#include "mongocxx/instance.hpp"
#include "served/multiplexer.hpp"
#include "served/net/server.hpp"
#include "lineReader.h"

using namespace std;

namespace learning {
    lineReader lector;
    constexpr char kNewLineEndpoint[] = "/newline";
    constexpr char kClearEndpoint[] = "/clear";
    constexpr char kIpAddress[] = "0.0.0.0"; // Localhost
    constexpr char kPort[] = "5000"; // Puerto
    constexpr int kThreads = 10; // Cantidad de peticiones que soporta al mismo tiempo

    class HttpServer {
        public:
        HttpServer(served::multiplexer multiplexer) : multiplexer(multiplexer) {}
        auto NewLine() {
            return [&](served::response &response, const served::request &request) {

                //Leer el cuerpo de la peticion
                json::JSON request_body = json::JSON::Load(request.body());
                string linea = request_body["line"].ToString();

                

                // Restornar si se agrego correctamente o no
                if (linea != "") {
                    
                    response << lector.leerLinea(linea);
                }
                else {
                    return served::response::stock_reply(400, response);;
                }


            };
        }
      
        auto ClearLines() {
            return [&](served::response &response, const served::request &request) {

                // MongoDbHandler mhandler;
                // bool delete_successful = mhandler.ClearLinesFromDb();

                // // Retornar si se elimino todo correctamente o no
                // delete_successful ? served::response::stock_reply(200, response)
                //                     : served::response::stock_reply(404, response);

            };
        }

        void InitializeEndpoints() {
            multiplexer.handle(kNewLineEndpoint).post(NewLine());
            multiplexer.handle(kClearEndpoint).get(ClearLines());

        }

        void StartServer() {
            mongocxx::instance instance;
            served::net::server server(kIpAddress, kPort, multiplexer);
            cout << "Servidor escuchando el puerto " << kPort << "..." << endl;
            server.run(kThreads);
        }

        private:
        served::multiplexer multiplexer;
    };
}  // namespace learning