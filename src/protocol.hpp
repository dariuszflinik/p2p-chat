#pragma once

#include <optional>
#include <string>

struct ChatMessage {
    std::string sender;
    std::string text;
};

std::string serialize_message(const std::string& sender, const std::string& text);
std::optional<ChatMessage> parse_message(const std::string& line);