#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum class AppMode {
    Listen,
    Connect
};

struct AppConfig {
    std::string username;
    AppMode mode;
    std::string host;
    std::uint16_t port{};
};

enum class ParseStatus {
    Ok,
    Help,
    Error
};

struct ParseResult {
    ParseStatus status;
    std::optional<AppConfig> config;
};

ParseResult parse_arguments(int argc, char** argv);
void print_usage(const char* program_name);