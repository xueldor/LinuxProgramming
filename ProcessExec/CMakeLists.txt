cmake_minimum_required(VERSION 3.17)
project(ProcessExec)

set(CMAKE_CXX_STANDARD 14)

add_executable(ProcessExec error.c ProcessExec.c)