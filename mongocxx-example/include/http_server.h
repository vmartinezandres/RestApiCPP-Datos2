#pragma once

#include <iostream>
#include <string>

#include "SimpleJSON/json.hpp"
#include "mongocxx/instance.hpp"
#include "mongodb_handler.h"
#include "served/multiplexer.hpp"
#include "served/net/server.hpp"

namespace learning {
    constexpr char kSaveEndpoint[] = "/save";
    constexpr char kDeleteEndpoint[] = "/delete";
    constexpr char kAllLinesEndpoint[] = "/";
    constexpr char kIpAddress[] = "0.0.0.0"; //localhost
    constexpr char kPort[] = "5000";
    constexpr int kThreads = 10;

    class HttpServer {
        public:
        HttpServer(served::multiplexer multiplexer) : multiplexer(multiplexer) {}
        auto SaveLineToMongoDb() {
            return [&](served::response &response, const served::request &request) {

                //Leer el cuerpo de la peticion
                json::JSON request_body = json::JSON::Load(request.body());
              
                // Llamar MongoDbHandler para agregar la informacion
                MongoDbHandler mhandler;
                bool insert_successful = mhandler.AddCodeLineToDb(request_body["line"].ToString());

                // Restornar si se agrego correctamente o no
                insert_successful ? served::response::stock_reply(200, response) // 200: true
                                    : served::response::stock_reply(400, response); // 400: false
            };
        }
      
        auto DeleteLineFromMongoDb() {
            return [&](served::response &response, const served::request &request) {

                // Leer el cuerpo de la peticion
                json::JSON request_body = json::JSON::Load(request.body());

                // Llamar MongoDbHandler para eliminar la linea de codigo
                MongoDbHandler mhandler;
                bool delete_successful = mhandler.RemoveLineFromDb(request_body["lineId"].ToString());

                // Retornar si se elimino correctamente o no
                delete_successful ? served::response::stock_reply(200, response)
                                    : served::response::stock_reply(404, response);
            };
        }
        
        auto GetAllLines() {
            return [&](served::response &response, const served::request &request) {
            MongoDbHandler mhandler;
            const json::JSON &all_documents = mhandler.GetAllDocuments();
            std::ostringstream stream;
            stream << all_documents;
            response << stream.str();
            };
        }

        void InitializeEndpoints() {
            multiplexer.handle(kSaveEndpoint).post(SaveLineToMongoDb());
            multiplexer.handle(kDeleteEndpoint).post(DeleteLineFromMongoDb());
            multiplexer.handle(kAllLinesEndpoint).get(GetAllLines());
        }

        void StartServer() {
            mongocxx::instance instance;
            served::net::server server(kIpAddress, kPort, multiplexer);
            std::cout << "Starting server to listen on port " << kPort << "..." << std::endl;
            server.run(kThreads);
        }

        private:
        served::multiplexer multiplexer;
    };
}  // namespace learning