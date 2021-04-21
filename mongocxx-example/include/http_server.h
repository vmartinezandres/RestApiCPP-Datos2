#pragma once

#include <iostream>
#include <string>

#include "SimpleJSON/json.hpp"
#include "mongocxx/instance.hpp"
#include "mongodb_handler.h"
#include "served/multiplexer.hpp"
#include "served/net/server.hpp"

using namespace std;

namespace learning {
    constexpr char kSaveEndpoint[] = "/save";
    constexpr char kClearEndpoint[] = "/clear";
    constexpr char kAllLinesEndpoint[] = "/";
    constexpr char kIpAddress[] = "0.0.0.0"; // Localhost
    constexpr char kPort[] = "5000"; // Puerto
    constexpr int kThreads = 10; // Cantidad de peticiones que soporta al mismo tiempo

    class HttpServer {
        public:
        HttpServer(served::multiplexer multiplexer) : multiplexer(multiplexer) {}
        auto SaveLineToMongoDb() {
            return [&](served::response &response, const served::request &request) {

                //Leer el cuerpo de la peticion
                json::JSON request_body = json::JSON::Load(request.body());
                string linea = request_body["line"].ToString();
              
                // Llamar MongoDbHandler para agregar la informacion
                MongoDbHandler mhandler;
                bool insert_successful = mhandler.AddCodeLineToDb(request_body["line"].ToString());

                // response << "{\"line\" : \"" << linea << "\"}";

                // Restornar si se agrego correctamente o no
                insert_successful ? served::response::stock_reply(200, response) // 200: bien
                                    : served::response::stock_reply(400, response); // 400: error
            };
        }
      
        auto ClearLinesFromMongoDb() {
            return [&](served::response &response, const served::request &request) {

                MongoDbHandler mhandler;
                bool delete_successful = mhandler.ClearLinesFromDb();

                // // Retornar si se elimino todo correctamente o no
                delete_successful ? served::response::stock_reply(200, response)
                                    : served::response::stock_reply(404, response);

            };
        }
        
        auto GetAllLines() {
            return [&](served::response &response, const served::request &request) {
            MongoDbHandler mhandler;
            const json::JSON &all_documents = mhandler.GetAllDocuments();
            ostringstream stream;
            stream << all_documents;
            response << stream.str();
            };
        }

        void InitializeEndpoints() {
            multiplexer.handle(kSaveEndpoint).post(SaveLineToMongoDb());
            multiplexer.handle(kClearEndpoint).get(ClearLinesFromMongoDb());
            multiplexer.handle(kAllLinesEndpoint).get(GetAllLines());

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