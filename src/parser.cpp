#include "parser.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {

std::uint16_t parse_port(const std::string& value) {
    int port = std::stoi(value);

    if (port <= 0 || port > 65535) {
        throw std::runtime_error("port must be in range 1-65535");
    }

    return static_cast<std::uint16_t>(port);
}

bool is_valid_username(const std::string& username) {
    if (username.empty()) {
        return false;
    }

    if (username.find('\t') != std::string::npos || username.find('\n') != std::string::npos) {
        return false;
    }

    return true;
}

} // namespace

void print_usage(const char* program_name) {
    std::cout
        << "Usage:\n"
        << "  " << program_name << " --name <name> --listen <port>\n"
        << "  " << program_name << " --name <name> --connect <host> <port>\n"
        << "\nExamples:\n"
        << "  " << program_name << " --name Alice --listen 5000\n"
        << "  " << program_name << " --name Bob --connect 127.0.0.1 5000\n";
}

ParseResult parse_arguments(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        print_usage(argv[0]);
        return {ParseStatus::Help, std::nullopt};
    }

    AppConfig config;
    bool has_name = false;
    bool has_mode = false;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--name" && i + 1 < argc) {
                config.username = argv[++i];
                has_name = true;
            } else if (arg == "--listen" && i + 1 < argc) {
                config.mode = AppMode::Listen;
                config.port = parse_port(argv[++i]);
                has_mode = true;
            } else if (arg == "--connect" && i + 2 < argc) {
                config.mode = AppMode::Connect;
                config.host = argv[++i];
                config.port = parse_port(argv[++i]);
                has_mode = true;
            } else {
                std::cerr << "Invalid argument: " << arg << "\n\n";
                print_usage(argv[0]);
                return {ParseStatus::Error, std::nullopt};
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Argument error: " << e.what() << "\n\n";
        print_usage(argv[0]);
        return {ParseStatus::Error, std::nullopt};
    }

    if (!has_name || !is_valid_username(config.username)) {
        std::cerr << "Missing or invalid argument: --name\n\n";
        print_usage(argv[0]);
        return {ParseStatus::Error, std::nullopt};
    }

    if (!has_mode) {
        std::cerr << "Missing mode: --listen or --connect\n\n";
        print_usage(argv[0]);
        return {ParseStatus::Error, std::nullopt};
    }

    return {ParseStatus::Ok, config};
}