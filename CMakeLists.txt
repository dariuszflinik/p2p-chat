cmake_minimum_required(VERSION 3.10)

project(p2p_chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(p2p-chat
    src/main.cpp
    src/parser.cpp
    src/network.cpp
    src/chat.cpp
    src/protocol.cpp
)

target_include_directories(p2p-chat PRIVATE src)

target_compile_options(p2p-chat PRIVATE -Wall -Wextra -pedantic)

enable_testing()

add_executable(protocol-tests
    tests/protocol_tests.cpp
    src/protocol.cpp
)

target_include_directories(protocol-tests PRIVATE src)

target_compile_options(protocol-tests PRIVATE -Wall -Wextra -pedantic)

add_test(NAME protocol-tests COMMAND protocol-tests)