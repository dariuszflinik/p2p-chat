#pragma once

#include <cstdint>
#include <string>

int create_listener(std::uint16_t port);
int wait_for_peer(int listener_fd);

int connect_to_peer(const std::string& host, std::uint16_t port);

void close_fd(int fd);