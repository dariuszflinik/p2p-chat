#include "protocol.hpp"

std::string serialize_message(const std::string& sender, const std::string& text) {
    return sender + "\t" + text + "\n";
}

std::optional<ChatMessage> parse_message(const std::string& line) {
    const auto separator_pos = line.find('\t');

    if (separator_pos == std::string::npos) {
        return std::nullopt;
    }

    ChatMessage message;
    message.sender = line.substr(0, separator_pos);
    message.text = line.substr(separator_pos + 1);

    if (message.sender.empty() || message.text.empty()) {
        return std::nullopt;
    }

    return message;
}