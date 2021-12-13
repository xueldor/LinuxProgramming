cmake_minimum_required(VERSION 3.17)
project(inet_socket)

set(CMAKE_CXX_STANDARD 17)


add_executable(server ServerSocket.cpp)
add_executable(client ClientSocket.cpp)
