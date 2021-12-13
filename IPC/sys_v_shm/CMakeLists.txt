cmake_minimum_required(VERSION 3.17)
project(sys_v_shm)

set(CMAKE_CXX_STANDARD 14)

add_executable(sw sw.cpp)
add_executable(sr sr.cpp)
