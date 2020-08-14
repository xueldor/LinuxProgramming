cmake_minimum_required(VERSION 3.17)
project(FileIO)

set(CMAKE_CXX_STANDARD 14)

add_executable(FileIO EPollTest.c FileIO.c FileIOAdvance.c StdFileIO.c)