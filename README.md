# P2P-chat

Simple console peer-to-peer chat written in C++17 for Linux.

The application allows two instances to communicate directly over TCP. One instance starts in listening mode and waits for an incoming connection. The second instance connects to it using an IP address and port. After the connection is established, both users can send and receive messages independently.


## Requirements

- Linux
- CMake >= 3.10
- C++17 compiler, for example `g++`

On Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential cmake
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

The application binary will be created as:

```bash
./p2p-chat
```

## Running locally

Open two terminal windows.

Terminal 1:

```bash
./p2p-chat --name Dariusz --listen 5000
```

Terminal 2:

```bash
./p2p-chat --name Mariusz --connect 127.0.0.1 5000
```

After the connection is established, both terminals can send and receive messages.

To exit the chat, type:

```text
/quit
```

## Running on two Linux machines

Assume that Machine A has IP address:

```text
192.168.1.10
```

On Machine A:

```bash
./p2p-chat --name Dariusz --listen 5000
```

On Machine B:

```bash
./p2p-chat --name Mariusz --connect 192.168.1.10 5000
```

Both machines must be able to reach each other over TCP. In a typical test setup, both machines should be in the same LAN or VPN.

If a firewall is enabled on Machine A, allow the selected TCP port:

```bash
sudo ufw allow 5000/tcp
```

## Command-line options

```text
--name <name>
    User name displayed next to sent messages.

--listen <port>
    Start the application in listening mode and wait for an incoming peer connection.

--connect <host> <port>
    Connect to another instance using its IP address and TCP port.

--help
    Show usage information.
```

Examples:

```bash
./p2p-chat --name Alice --listen 5000
./p2p-chat --name Bob --connect 127.0.0.1 5000
```

## Message format

Messages are sent as line-based TCP payloads.

Internal format:

```text
<sender>\t<message>\n
```

Example:

```text
[Dariusz]    Witaj Mariusz
```

The tab character is used as a separator between sender name and message text. Newline marks the end of a message.

## Tests

The project contains simple unit-like tests for message serialization and parsing.

Build the project first:

```bash
mkdir build
cd build
cmake ..
make
```

Run tests:

```bash
ctest --output-on-failure
```

You can also run the test binary directly:

```bash
./protocol-tests
```

Expected output:

```text
protocol tests passed
```

## Project structure

```text
.
├── CMakeLists.txt
├── README.md
├── src
│   ├── main.cpp
│   ├── network.hpp
│   ├── network.cpp
│   ├── chat.hpp
│   ├── chat.cpp
│   ├── parser.cpp
│   ├── parser.hpp
│   ├── protocol.hpp
│   └── protocol.cpp
└── tests
    └── protocol_tests.cpp
```

## Limitations

- supports only two instances
- requires direct TCP connectivity between machines
- peer address must be provided manually
- no peer discovery
- no NAT traversal
- no encryption
- no authentication
- terminal output may interrupt a message currently being typed

## Security considerations

This project is a simple recruitment task and does not implement encryption or peer authentication.

Messages are sent as plain text over TCP. The application should be used only in trusted local networks.

A production-ready version should use TLS or another authenticated encryption mechanism.

## Possible improvements

- hybrid peer mode where both instances listen and connect at the same time
- lightweight discovery/signaling server for finding peers by name
- TLS encryption
- better terminal UI
- reconnect support
- integration tests for network communication
