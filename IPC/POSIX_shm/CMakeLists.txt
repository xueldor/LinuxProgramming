cmake_minimum_required(VERSION 3.17)
project(posix_shm)

set(CMAKE_CXX_STANDARD 14)


add_executable(psw psw.cpp)
add_executable(psr psr.cpp ../socket/InetSocket/server.c)

find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(psw ${LIBRT})
    target_link_libraries(psr ${LIBRT})
endif()