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

std::optional<AppConfig> parse_arguments(int argc, char** argv);
void print_usage(const char* program_name);