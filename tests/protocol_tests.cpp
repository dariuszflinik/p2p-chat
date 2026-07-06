#include "protocol.hpp"

#include <cassert>
#include <iostream>
#include <string>

namespace {

void test_serialize_message() {
    const std::string result = serialize_message("Dariusz", "hello");

    assert(result == "Dariusz\thello\n");
}

void test_parse_valid_message() {
    const auto message = parse_message("Dariusz\thello");

    assert(message.has_value());
    assert(message->sender == "Dariusz");
    assert(message->text == "hello");
}

void test_parse_text_with_colon() {
    const auto message = parse_message("Mariusz\ttime is 12:30");

    assert(message.has_value());
    assert(message->sender == "Mariusz");
    assert(message->text == "time is 12:30");
}

void test_parse_text_with_tabs_inside_message() {
    const auto message = parse_message("Dariusz\thello\twith tab");

    assert(message.has_value());
    assert(message->sender == "Dariusz");
    assert(message->text == "hello\twith tab");
}

void test_reject_missing_separator() {
    const auto message = parse_message("Dariusz:hello");

    assert(!message.has_value());
}

void test_reject_empty_sender() {
    const auto message = parse_message("\thello");

    assert(!message.has_value());
}

void test_reject_empty_text() {
    const auto message = parse_message("Dariusz\t");

    assert(!message.has_value());
}

} // namespace

int main() {
    test_serialize_message();
    test_parse_valid_message();
    test_parse_text_with_colon();
    test_parse_text_with_tabs_inside_message();
    test_reject_missing_separator();
    test_reject_empty_sender();
    test_reject_empty_text();

    std::cout << "protocol tests passed\n";

    return 0;
}