cmake_minimum_required(VERSION 3.29)
project(thread_pool)

set(CMAKE_CXX_STANDARD 26)

add_library(thread_pool INTERFACE
        external/doctest/doctest.h)
target_include_directories(thread_pool INTERFACE include/)

add_subdirectory(tests)