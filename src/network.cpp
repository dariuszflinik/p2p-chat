#include "network.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace {

void throw_system_error(const std::string& message) {
    throw std::runtime_error(message + ": " + std::strerror(errno));
}

} // namespace

int create_listener(std::uint16_t port) {
    int listener_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (listener_fd < 0) {
        throw_system_error("socket() failed");
    }

    int reuse = 1;
    if (::setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        close_fd(listener_fd);
        throw_system_error("setsockopt() failed");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(listener_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0) {
        close_fd(listener_fd);
        throw_system_error("bind() failed");
    }

    if (::listen(listener_fd, 1) < 0) {
        close_fd(listener_fd);
        throw_system_error("listen() failed");
    }

    return listener_fd;
}

int wait_for_peer(int listener_fd) {
    sockaddr_in peer_address{};
    socklen_t peer_address_len = sizeof(peer_address);

    int peer_fd = ::accept(
        listener_fd,
        reinterpret_cast<sockaddr*>(&peer_address),
        &peer_address_len
    );

    if (peer_fd < 0) {
        throw_system_error("accept() failed");
    }

    char peer_ip[INET_ADDRSTRLEN]{};

    if (::inet_ntop(AF_INET, &peer_address.sin_addr, peer_ip, sizeof(peer_ip)) != nullptr) {
        std::cout << "Incoming connection from "
                  << peer_ip
                  << ":"
                  << ntohs(peer_address.sin_port)
                  << "\n";
    }

    return peer_fd;
}

int connect_to_peer(const std::string& host, std::uint16_t port) {
    int socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
        throw_system_error("socket() failed");
    }

    sockaddr_in peer_address{};
    peer_address.sin_family = AF_INET;
    peer_address.sin_port = htons(port);

    if (::inet_pton(AF_INET, host.c_str(), &peer_address.sin_addr) <= 0) {
        close_fd(socket_fd);
        throw std::runtime_error("invalid IPv4 address: " + host);
    }

    if (::connect(socket_fd, reinterpret_cast<sockaddr*>(&peer_address), sizeof(peer_address)) < 0) {
        close_fd(socket_fd);
        throw_system_error("connect() failed");
    }

    return socket_fd;
}

void close_fd(int fd) {
    if (fd >= 0) {
        ::close(fd);
    }
}