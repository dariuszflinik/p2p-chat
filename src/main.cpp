#include "chat.hpp"
#include "parser.hpp"
#include "network.hpp"

#include <exception>
#include <iostream>

int main(int argc, char** argv) {
    auto config = parse_arguments(argc, argv);

    if (!config.has_value()) {
        return 1;
    }

    try {
        int peer_fd = -1;

        if (config->mode == AppMode::Listen) {
            std::cout << "Listening on port " << config->port << "...\n";

            int listener_fd = create_listener(config->port);
            peer_fd = wait_for_peer(listener_fd);

            close_fd(listener_fd);

            std::cout << "Peer connected.\n";
        } else {
            std::cout << "Connecting to "
                      << config->host
                      << ":"
                      << config->port
                      << "...\n";

            peer_fd = connect_to_peer(config->host, config->port);

            std::cout << "Connected.\n";
        }

        run_chat(peer_fd, config->username);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}