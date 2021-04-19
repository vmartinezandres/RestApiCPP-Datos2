#include <iostream>

#include "mongocxx/instance.hpp"
#include "mongodb_handler.h"

void print_result(const bool &result, const char* operation) {
    std::cout << "the " << operation << (result ? " worked." : " did't work.") << std::endl;
}
int main() {
    mongocxx::instance instance;
    learning::MongoDbHandler mhandler;
    // mhandler.AddCharacterToDb("Donkey Kong", "medium", 0);
    // mhandler.AddCharacterToDb("Mario", "small", 0);

    // bool result = mhandler.UpdateWins("607cc7da2731172d3e3a05c7");
    bool result = mhandler.RemoveCharacterFromDb("607cc63bb690a11c73711c42");
    print_result(result, "remove");
    std::cout << "Hello Easy C++ project!" << std::endl;
}