cmake_minimum_required(VERSION 3.17)
project(inet_socket)

set(CMAKE_CXX_STANDARD 14)


add_executable(server server.c)
add_executable(client client.c)
