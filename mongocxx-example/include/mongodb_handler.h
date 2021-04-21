#pragma once

#include <cstdint>
#include <string>
#include <iostream>

#include "SimpleJSON/json.hpp"
#include "bsoncxx/builder/stream/document.hpp"
#include "bsoncxx/json.hpp"
#include "bsoncxx/oid.hpp"
#include "mongocxx/client.hpp"
#include "mongocxx/database.hpp"
#include "mongocxx/uri.hpp"

using namespace std;

namespace learning {
  constexpr char kMongoDbUri[] = "mongodb://0.0.0.0:27017";
  constexpr char kDatabaseName[] = "MongoDataBase";
  constexpr char kCollectionName[] = "CodeLines";
  
  class MongoDbHandler {
    public:
    MongoDbHandler()
      : uri(mongocxx::uri(kMongoDbUri)),
        client(mongocxx::client(uri)),
        db(client[kDatabaseName]) {}

    // Agregar linea de codigo de C!
    bool AddCodeLineToDb(const std::string &codeLine) {
      mongocxx::collection collection = db[kCollectionName];
      auto builder = bsoncxx::builder::stream::document{};
      
      bsoncxx::v_noabi::document::value doc_value = builder 
      << "line" << codeLine << bsoncxx::builder::stream::finalize;
        
      bsoncxx::stdx::optional<mongocxx::result::insert_one> maybe_result = 
      collection.insert_one(doc_value.view());

      // // Si todo salio bien o no
      if(maybe_result) {
        return maybe_result->inserted_id().get_oid().value.to_string().size() != 0;
      }

      return false;
    }

    // Limpiar la base de datos
    bool ClearLinesFromDb() {
      mongocxx::collection collection = db[kCollectionName];
      collection.drop();
      return true;
    }

    json::JSON GetAllDocuments() {
      mongocxx::collection collection = db[kCollectionName];
      mongocxx::cursor cursor = collection.find({});
      json::JSON result;
      result["lines"] = json::Array();
      string line;
      if (cursor.begin() != cursor.end()) {
        for (auto doc : cursor) {
          line = bsoncxx::to_json(doc);
          result["lines"].append(line);
        }
      }
      return result;
    }

    private:
    mongocxx::uri uri;
    mongocxx::client client;
    mongocxx::database db;
  };
}  // namespace learning