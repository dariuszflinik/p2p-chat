#include "parser.hpp"

#include <iostream>

int main(int argc, char** argv) {
    auto config = parse_arguments(argc, argv);

    if (!config.has_value()) {
        return 1;
    }

    std::cout << "Arguments:\n";
    std::cout << "name: " << config->username << "\n";

    if (config->mode == AppMode::Listen) {
        std::cout << "mode: listen\n";
        std::cout << "port: " << config->port << "\n";
    } else {
        std::cout << "mode: connect\n";
        std::cout << "host: " << config->host << "\n";
        std::cout << "port: " << config->port << "\n";
    }

    return 0;
}