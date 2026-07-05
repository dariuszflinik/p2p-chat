cmake_minimum_required(VERSION 3.10)

project(p2p_chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(p2p-chat
    src/main.cpp
    src/parser.cpp
)

target_include_directories(p2p-chat PRIVATE src)

target_compile_options(p2p-chat PRIVATE -Wall -Wextra -pedantic)