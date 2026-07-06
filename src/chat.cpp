#include "chat.hpp"

#include "network.hpp"

#include <atomic>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

namespace {

bool send_all(int socket_fd, const std::string& data) {
    std::size_t total_sent = 0;

    while (total_sent < data.size()) {
        ssize_t sent = ::send(
            socket_fd,
            data.data() + total_sent,
            data.size() - total_sent,
            0
        );

        if (sent <= 0) {
            return false;
        }

        total_sent += static_cast<std::size_t>(sent);
    }

    return true;
}

void receive_loop(int socket_fd, std::atomic_bool& running) {
    char buffer[1024];

    while (running) {
        ssize_t received = ::recv(socket_fd, buffer, sizeof(buffer) - 1, 0);

        if (received > 0) {
            buffer[received] = '\0';
            std::cout << "\n" << buffer << "> " << std::flush;
        } else if (received == 0) {
            std::cout << "\nPeer disconnected.\n";
            running = false;
            break;
        } else {
            if (running) {
                std::cerr << "\nrecv() failed: " << std::strerror(errno) << "\n";
            }

            running = false;
            break;
        }
    }
}

} // namespace

void run_chat(int socket_fd, const std::string& username) {
    std::atomic_bool running = true;

    std::thread receiver(receive_loop, socket_fd, std::ref(running));

    std::cout << "Chat started. Type messages and press Enter.\n";
    std::cout << "> " << std::flush;

    std::string message;

    while (running && std::getline(std::cin, message)) {
        if (message.empty()) {
            std::cout << "> " << std::flush;
            continue;
        }

        std::string payload = "[" + username + "] " + message + "\n";

        if (!send_all(socket_fd, payload)) {
            std::cerr << "send() failed. Closing chat.\n";
            running = false;
            break;
        }

        std::cout << "> " << std::flush;
    }

    running = false;

    ::shutdown(socket_fd, SHUT_RDWR);

    if (receiver.joinable()) {
        receiver.join();
    }

    close_fd(socket_fd);
}